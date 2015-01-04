#include "worker-thread.h"
#include "main-window.h"

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

void WorkerThread::startModelTraining(int nSensors)
{
    if (isRunning())
        return;
    //
    m_nOperation = TrainModel;
    m_Perceptron = new Perceptron(nSensors,1);
    QThread::start();
}

void WorkerThread::startImageRecognition(const QImage &rcImageInput)
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
            // сформировать обучающую выборку из папки images
            QList<Pattern> trainSet;
            m_Perceptron->train(trainSet);
            break;
    }
        case RecognizeImage:
            if(m_ptrInput)
            {
                m_Perceptron->recognize(*m_ptrInput.data());
                m_ptrResult.reset(new QImage(*m_ptrInput));
            }
            break;
    }
}
