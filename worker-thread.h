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
    //
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
    void startTrainModel();
    //
    void startClassifyImage(const QImage &rcImageInput);
    //
    void stop();
    // создание сети
    void createModel(int nSensors, int nHiddenLayers, int nHiddenNeurons, int nPatterns);
signals:
    void canceled();
private:        
    //
    Perceptron *m_Perceptron;
    //
    MainWindow *m_pWindow;
    QString m_FilePath;
    QScopedPointer<QImage> m_ptrInput, m_ptrResult;
    // класс изображения
    int m_imgType;
    //
    virtual void run();
};

#endif // WORKERTHREAD_H
