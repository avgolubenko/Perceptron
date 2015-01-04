#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QImage>

#include "perceptron.h"

class MainWindow;

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(MainWindow *pWindow);
    ~WorkerThread();
    const QImage *getResultImage() const;
    //
    void startLoadFile(const QString &rcFilePath);
    //
    void startModelTraining(int nSensors);
    //
    void startImageRecognition(const QImage &rcImageInput);
    //
    void stop();
signals:
//    void started();
//    void finished();
    void canceled();
private:
    enum Operation
    {
        LoadFile,
        TrainModel,
        RecognizeImage
    };
    //
    MainWindow *m_pWindow;
    //
    Perceptron *m_Perceptron;
    //
    Operation m_nOperation;
    QString m_FilePath;
    QScopedPointer<QImage> m_ptrInput, m_ptrResult;
    //
    virtual void run();
};

#endif // WORKERTHREAD_H
