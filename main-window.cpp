#include "main-window.h"
#include "worker-thread.h"
#include "ui_main-window.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    m_pLabelImage = new QLabel;
    m_pLabelImage->setAlignment(Qt::AlignCenter);
//    m_pLabelImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    m_pLabelImage->setScaledContents(true);
    //
    m_pAreaImage = new QScrollArea(this);
    m_pAreaImage->setBackgroundRole(QPalette::Dark);
    m_pAreaImage->setWidget(m_pLabelImage);
    m_pAreaImage->setWidgetResizable(true);
    setCentralWidget(m_pAreaImage);
    //
    connect(ui->action_Open, &QAction::triggered, this, &MainWindow::onActionOpenTriggered);
    connect(ui->action_Train, &QAction::triggered, this, &MainWindow::onActionTrainTriggered);
    connect(ui->action_Recognize, &QAction::triggered, this, &MainWindow::onActionRecognizeTriggered);
    connect(ui->cancelButton, &QAbstractButton::pressed, this, &MainWindow::onActionCancelTriggered);
    //
    ui->progressBar->hide();
    ui->cancelButton->hide();
    ui->statusBar->addWidget(ui->progressBar);
    ui->statusBar->addWidget(ui->cancelButton);
    //
    m_pThreadWork = new WorkerThread(this);
    connect(m_pThreadWork, &QThread::started, this, &MainWindow::onThreadStarted);
    connect(m_pThreadWork, &QThread::finished, this, &MainWindow::onThreadFinished);
    connect(m_pThreadWork, &WorkerThread::canceled, this, &MainWindow::onThreadCanceled);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pLabelImage;
    delete m_pAreaImage;
    delete m_pThreadWork;
}

void MainWindow::updateProgress(int nPercent)
{
    ui->progressBar->setValue(nPercent);
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
    onActionCancelTriggered();
}

void MainWindow::onActionOpenTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image File"),
                                                    QString("../../SimplePerceptron/images"),
                                                    tr("Images (*.png *.bmp *.jpg)"));
    if(!fileName.isEmpty())
        m_pThreadWork->startLoadFile(fileName);
}

void MainWindow::onActionTrainTriggered()
{
    m_pThreadWork->startModelTraining(m_Image.width() + m_Image.height());
}

void MainWindow::onActionRecognizeTriggered()
{
    m_pThreadWork->startImageRecognition(m_Image);
}

void MainWindow::onActionCancelTriggered()
{
    m_pThreadWork->stop();
}

void MainWindow::onThreadStarted()
{
    ui->menuBar->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    ui->cancelButton->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_bCanceled = false;
//    m_pThreadWork->sleep(2);
}

void MainWindow::onThreadFinished()
{
    ui->menuBar->setEnabled(true);
    ui->progressBar->hide();
    ui->cancelButton->hide();
    //
    QApplication::restoreOverrideCursor();
    //
    const QImage *pcImage = m_pThreadWork->getResultImage();
    if(pcImage)
    {
        m_Image = *pcImage;
        m_pLabelImage->setPixmap(QPixmap::fromImage(m_Image));
//        m_pLabelImage->adjustSize();
    }
    //
    statusBar()->showMessage(m_bCanceled ? tr("Canceled") : tr("Finished"), 2000);
}

void MainWindow::onThreadCanceled()
{
    m_bCanceled = true;
}
