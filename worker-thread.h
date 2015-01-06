#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QImage>

#include "perceptron.h"

class MainWindow;

enum Operation
{
    LoadFile,
    TrainModel,
    RecognizeImage
};

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(MainWindow *pWindow);
    ~WorkerThread();
    // тип вычислительной операции
    Operation m_nOperation;
    // геттер для m_ptrResult
    const QImage *getResultImage() const;
    // геттер для m_imgType
    int getImgType() const;
    //
    void startLoadFile(const QString &rcFilePath);
    //
    void startTrainModel(int nSensors);
    //
    void startClassifyImage(const QImage &rcImageInput);
    //
    void stop();
signals:
//    void started();
//    void finished();
    void canceled();
private:    
    //
    MainWindow *m_pWindow;
    //
    Perceptron *m_Perceptron;
    //
    QString m_FilePath;
    QScopedPointer<QImage> m_ptrInput, m_ptrResult;
    // класс изображения
    int m_imgType;
    //
    virtual void run();
};

#endif // WORKERTHREAD_H
