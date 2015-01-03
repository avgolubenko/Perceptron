#include "worker-thread.h"
#include "main-window.h"

WorkerThread::WorkerThread(MainWindow *pWindow) : m_pWindow(pWindow){}

WorkerThread::~WorkerThread()
{
//    delete m_pWindow;
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

void WorkerThread::startImageRecognition(const QImage &rcImageInput)
{
    if (isRunning())
        return;
    //    
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
        case RecognizeImage:
            if (m_ptrInput)
            {
                m_ptrResult.reset(new QImage(*m_ptrInput));
            }
            break;
    }
}
