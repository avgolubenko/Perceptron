#include "main-window.h"
#include "worker-thread.h"
#include "ui_main-window.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_bTrained(false)
{
    ui->setupUi(this);
    //
    m_pLabelImage = new QLabel;
    m_pLabelImage->setAlignment(Qt::AlignCenter);
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
                                                    QString(":/images"),
                                                    tr("Images (*.jpg)"));
    if(!fileName.isEmpty())
        m_pThreadWork->startLoadFile(fileName);
}

void MainWindow::onActionTrainTriggered()
{
    m_pThreadWork->startTrainModel(m_Image.width() + m_Image.height(), 3);
}

void MainWindow::onActionRecognizeTriggered()
{
    m_pThreadWork->startClassifyImage(m_Image);
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
}

void MainWindow::onThreadFinished()
{
    ui->menuBar->setEnabled(true);
    ui->progressBar->hide();
    ui->cancelButton->hide();
    //
    QApplication::restoreOverrideCursor();
    //    
    switch (m_pThreadWork->m_nOperation) {
        case LoadFile:
        {
            const QImage *pcImage = m_pThreadWork->getResultImage();
            if(pcImage)
            {
                m_Image = *pcImage;
                m_pLabelImage->setPixmap(QPixmap::fromImage(m_Image));
                //
                ui->action_Train->setEnabled(!m_bTrained);
                statusBar()->showMessage(m_bCanceled ? tr("Canceled") : tr("File is loaded"), 2000);
            }
            break;
        }
        case RecognizeImage:
        {
            QString ptrName;
            switch (m_pThreadWork->getImgType()) {
            case 0:
                ptrName = tr("Up");
                break;
            case 1:
                ptrName = tr("Down");
                break;
            default:
                ptrName = tr("Unknown");
                break;
            }
            //
            statusBar()->showMessage(m_bCanceled ? tr("Canceled") : ptrName);
            break;
        }
        case TrainModel:
        {
            m_bTrained = true;
            ui->action_Train->setEnabled(false);
            ui->action_Recognize->setEnabled(true);
            statusBar()->showMessage(m_bCanceled ? tr("Canceled") : tr("Model is trained"), 2000);
            break;
        }
    }
}

void MainWindow::onThreadCanceled()
{
    m_bCanceled = true;
}
