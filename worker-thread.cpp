#include "worker-thread.h"
#include "main-window.h"

#include <QDir>

WorkerThread::WorkerThread(MainWindow *pWindow) : m_pWindow(pWindow){}

WorkerThread::~WorkerThread()
{
//    delete m_pWindow;
    delete m_Perceptron;
}

const QImage *WorkerThread::getResultImage() const
{
    return m_ptrResult.data();
}

int WorkerThread::getImgType() const
{
    return m_imgType;
}

void WorkerThread::startLoadFile(const QString &rcFilePath)
{
    if (isRunning())
        return;
    //
    m_nOperation = LoadFile;
    m_FilePath = rcFilePath;
    m_ptrInput.reset(new QImage);
    QThread::start();
}

void WorkerThread::startTrainModel(int nSensors, int nPatterns)
{
    if (isRunning())
        return;
    //
    m_nOperation = TrainModel;
    m_Perceptron = new Perceptron(nSensors,nPatterns);
    QThread::start();
}

void WorkerThread::startClassifyImage(const QImage &rcImageInput)
{
    if (isRunning())
        return;
    //
    m_nOperation = RecognizeImage;
    m_ptrInput.reset(new QImage(rcImageInput));    
    QThread::start();
}

void WorkerThread::stop()
{
    requestInterruption();
}

void WorkerThread::run()
{
    switch (m_nOperation)
    {
        case LoadFile:
            if(m_ptrInput->load(m_FilePath))
                m_ptrResult.reset(new QImage(*m_ptrInput));
            break;
        //
        case TrainModel:
    {
            // формирование обучающей выборки
            QList<Pattern> trainSet;
            // папка с обучающими изображениями
            QDir imgDir(":/images/images");
            QFileInfoList files = imgDir.entryInfoList(QStringList("*.jpg"));
            foreach (QFileInfo file, files) {
                QString fName = file.fileName();
                int type = fName[fName.indexOf(QChar('-')) - 1].digitValue();
                if(type != -1)
                {
                    QImage img(imgDir.absoluteFilePath(fName));
                    trainSet << Pattern(img,type);
                }
            }
            int nIters = 2; // число циклов обучения
            for(int iter = 0; iter < nIters; ++iter)
                m_Perceptron->train(trainSet);
            break;
    }
        case RecognizeImage:
            if(m_ptrInput)            
                m_imgType = m_Perceptron->classify(*m_ptrInput.data());
            break;
    }
}
