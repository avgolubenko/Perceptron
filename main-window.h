#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

class WorkerThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //
public slots:
    void updateProgress(int nPercent);
    //
protected:
    virtual void closeEvent(QCloseEvent *pEvent);
    //
private slots:
    void onActionOpenTriggered();
    void onActionTrainTriggered();
    void onActionRecognizeTriggered();
    void onActionCancelTriggered();
    // реакция на запуск потока
    void onThreadStarted();
    // реакция на завершение потока
    void onThreadFinished();
    // реакция на оcтановку потока
    void onThreadCanceled();
    //
private:
    Ui::MainWindow *ui;
    QLabel *m_pLabelImage;
    QScrollArea *m_pAreaImage;
    QImage m_Image;
    WorkerThread *m_pThreadWork;
    bool m_bCanceled;
    bool m_bTrained;
};

#endif // MAINWINDOW_H
