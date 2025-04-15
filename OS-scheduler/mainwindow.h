#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../include/Scheduler.h"
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_algorithmsList_currentIndexChanged(int index);

    void on_startBtn_clicked();

    void on_pushButton_clicked();

private:
    QGraphicsRectItem* lastRect = nullptr;
    QGraphicsTextItem* lastLabel = nullptr;
    int lastStartTime = -1;
    QGraphicsScene* ganttScene;
    int ganttRight = 10;
    int ganttTop = 40;
    int ganttBoxHeight = 40;
    int unitWidth = 20;
    int last_pid = -1;
    bool offlinePressed;
    vector<process> WaitingProcesses , MyProcesses;
    int currentTime;
    Ui::MainWindow *ui;
    void drawGanttChart();
    void addRectangleInGantt(process cur);
    void setupAverageTable();
    void setupCompletedProcesses();
    void setupNotCompletedProcesses();
    void setupInitialProcesses();
    void disableComboChoose();
    void displayInitialTable();
    void getAllwaiting();
    void startLiveExec(Scheduler *myScheduler);
    void updateTables(process current , Scheduler *myScheduler);
    void fetchWaiting(Scheduler * myScheduler);
};


#endif // MAINWINDOW_H


