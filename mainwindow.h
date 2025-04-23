#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qbuttongroup.h>
#include <bits/stdc++.h>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTimer>


#include "include/Scheduler.h"
#include "include/SJF_NonPreemptive.h"
#include "include/SJF_Preemptive.h"
#include "include/FCFS.h"
#include "include/priorityNonPreemtive.h"
#include "include/priorityPreemtive.h"
#include "include/RR_Scheduler.h"

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int scheduler=-1,count=0,pid=0,qtm=0;
    vector<int> vec;
    Scheduler* myscheduler;
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
    int currentTime=-3;
    QTimer *timer;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawGanttChart();
    void addRectangleInGantt(process cur);
    void setupAverageTable();
    void setupInitialTable(int pid,int arrival,int burst,int priority);
    void setupRemTable();
    void updateRemTable(process current);
    void setupCompletedProcesses();
    void setupNotCompletedProcesses();
    void displayInitialTable();

    void upfn();
    void fetchWaiting(Scheduler * myScheduler);
    void startLiveExec(Scheduler *myScheduler);
    void updateTables(process current, Scheduler *myScheduler);

private slots:
    void on_chose_Scheduler_clicked();

    void on_add_process_clicked();

    void on_quantum_clicked();

    void on_start_clicked();

    void on_add_process_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
