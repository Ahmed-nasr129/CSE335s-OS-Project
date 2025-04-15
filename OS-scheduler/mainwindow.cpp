#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QThread>
#include "../include/Scheduler.h"
#include "../include/SJF_NonPreemptive.h"
#include "../include/SJF_Preemptive.h"

struct Task {
    QString label;
    int start;
    int end;
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Apply border and padding to a styled widget
    ui->widget->setStyleSheet(
        "QWidget#widget {"
        "  border-radius: 10px;"
        "  background-color: white;"
        "}"
        );
    ui->verticalLayout_2->setContentsMargins(20, 20, 20, 20);

    // Draw the Gantt chart
    drawGanttChart();
    setupAverageTable();
    setupCompletedProcesses();
    setupNotCompletedProcesses();
    // setupInitialProcesses();

    // disable 'choose' choice
    disableComboChoose();

    // initial processes table
    displayInitialTable();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::drawGanttChart()
{
    // QGraphicsScene* scene = new QGraphicsScene(this);
    // scene->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    // ui->graphicsView->setScene(scene);

    // QGraphicsTextItem* title = scene->addText("Gantt Chart");
    // QFont font = title->font();
    // font.setPointSize(16);
    // font.setBold(true);
    // title->setFont(font);
    // title->setPos(100, 0);

    // QVector<Task> tasks = {
    //     {"A", 0, 2},
    //     {"B", 2, 6},
    //     {"C", 6, 12}
    // };

    // int right = 10;
    // int top = 40;
    // int boxHeight = 40;
    // int unitWidth = 20;

    // for (const Task& task : tasks) {
    //     int x = task.start * unitWidth + right;
    //     int width = (task.end - task.start) * unitWidth;

    //     QGraphicsRectItem* rect = scene->addRect(x, top, width, boxHeight, QPen(Qt::blue), QBrush(QColor(230, 240, 255)));

    //     QGraphicsTextItem* label = scene->addText(task.label);
    //     label->setPos(x + width/2 - 5, top + 10);
    // }

    // QVector<int> timePoints = {0, 2, 6, 12};
    // int timeTop = top + boxHeight + 10;

    // for (int t : timePoints) {
    //     QGraphicsTextItem* timeLabel = scene->addText(QString::number(t));
    //     timeLabel->setPos(t * unitWidth - 5 + right, timeTop);
    // }

    // scene->setSceneRect(0, 0, 300, 120);

    ganttScene = new QGraphicsScene(this);
    ganttScene->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    ui->graphicsView->setScene(ganttScene);

    QGraphicsTextItem* title = ganttScene->addText("Gantt Chart");
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setPos(100, 0);

    ganttScene->setSceneRect(0, 0, 1000, 120);
}

void MainWindow::addRectangleInGantt(process cur)
{
    int x = lastStartTime * unitWidth + ganttRight;
    int width = (currentTime - lastStartTime + 1) * unitWidth;

    // Update the width of the existing rectangle
    lastRect->setRect(x, ganttTop, width, ganttBoxHeight);

    // Adjust label position (keep it centered)
    lastLabel->setPos(x + width/2 - 5, ganttTop + 10);

    // Optional: update time tick mark
    QGraphicsTextItem* timeLabel = ganttScene->addText(QString::number(currentTime));
    timeLabel->setPos(currentTime * unitWidth - 5 + ganttRight, ganttTop + ganttBoxHeight + 10);
}


void MainWindow::setupAverageTable()
{
    QTableView* tableView = ui->averageTable;

    QStandardItemModel* model = new QStandardItemModel(0, 3, this);

    model->setHorizontalHeaderLabels({"Avg Response", "Avg Turnaround", "Avg Waiting"});

    QList<QStandardItem*> rowItems;
    rowItems << new QStandardItem("0")  // Avg Response
             << new QStandardItem("0")  // Avg Turnaround
             << new QStandardItem("0"); // Avg Waiting

    // Add the row to the model
    model->appendRow(rowItems);

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}
void MainWindow::setupCompletedProcesses()
{
    QTableView* tableView = ui->completedProcesses;

    QStandardItemModel* model = new QStandardItemModel(0, 2, this);

    model->setHorizontalHeaderLabels({"PID", "Completion Time"});

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}

void MainWindow::setupNotCompletedProcesses()
{
    QTableView* tableView = ui->incompletedProcesses;

    QStandardItemModel* model = new QStandardItemModel(0, 2, this);

    model->setHorizontalHeaderLabels({"PID", "Remaining Time"});

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}

// void MainWindow::setupInitialProcesses()
// {
//     QTableView* tableView = ui->initialProcesses;

//     QStandardItemModel* model = new QStandardItemModel(0, 3, this);

//     model->setHorizontalHeaderLabels({"Column 1", "Column 2", "Column 3"});

//     QHeaderView* header = tableView->horizontalHeader();
//     header->setSectionResizeMode(QHeaderView::Stretch);
//     tableView->setModel(model);
// }

void MainWindow::disableComboChoose()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->algorithmsList->model());
    QStandardItem* item = model->item(0);
    item->setEnabled(false);
    ui->Priority_lineEdit->setDisabled(true);
    ui->Priority_lineEdit->setText("Disabled");
}

void MainWindow::displayInitialTable()
{
    // Create the model with 0 rows and 4 columns, for example
    QStandardItemModel *model = new QStandardItemModel(0, 4, this);

    // Set column names
    model->setHeaderData(0, Qt::Horizontal, "PID");
    model->setHeaderData(1, Qt::Horizontal, "Arrival");
    model->setHeaderData(2, Qt::Horizontal, "Burst");
    model->setHeaderData(3, Qt::Horizontal, "Priority");

    // Assign the model to your QTableView
    ui->initialProcesses->setModel(model);
    QTableView* tableView = ui->initialProcesses;
    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_algorithmsList_currentIndexChanged(int index)
{
    ui->Priority_lineEdit->setDisabled(true);
    ui->Priority_lineEdit->setText("Disabled");

    if(index == 5){
        ui->Priority_lineEdit->setText("");
        ui->Priority_lineEdit->setDisabled(false);
    }
}

void MainWindow::fetchWaiting(Scheduler * myScheduler){
    for (auto it = WaitingProcesses.begin(); it != WaitingProcesses.end(); ) {
        if (it->getArrivalTime() <= currentTime) {
            myScheduler->addProcess(*it, it->getArrivalTime());
            it = WaitingProcesses.erase(it);  // erase returns the next valid iterator
        } else {
            ++it;  // only advance if not erasing
        }
    }
}


void MainWindow::on_startBtn_clicked()
{
    int choice = ui->algorithmsList->currentIndex();
    if(choice == 0){
        QMessageBox::warning(this, "No Selection", "Please select a scheduling algorithm.");
        return;
    }
    // if choice is RR ask for quantum
    //////////////////////////////////

    switch (choice) {
    case 2:{
        SJF_NonPreemptive *myScheduler = new SJF_NonPreemptive();
        fetchWaiting(myScheduler);
        startLiveExec(myScheduler);
        break;
    }
    case 3:{
        SJF_Preemptive *myScheduler = new SJF_Preemptive();
        fetchWaiting(myScheduler);
        startLiveExec(myScheduler);
        break;
    }
    default:
        break;
    }


}

QColor getColorForPid(int pid) {
    // simple hash to RGB range
    int r = (pid * 70) % 256;
    int g = (pid * 130) % 256;
    int b = (pid * 200) % 256;
    return QColor(r, g, b);
}

void MainWindow::startLiveExec(Scheduler *myScheduler)
{
    offlinePressed = false;

    while(!MyProcesses.empty() && myScheduler->getCompletedProcesses().size() != MyProcesses.size()){
        // check if offline is pressed
        if(offlinePressed) return; // terminate

        // check if there is a waiting process
        fetchWaiting(myScheduler);

        myScheduler->beginOperation(currentTime);
        process current = myScheduler->getCurrentProcess();

        // simulate sleeping 1 second
        QThread::sleep(1);

        if (current.getPid() != last_pid) {
            // New process, draw new rectangle
            int x = currentTime * unitWidth + ganttRight;
            int width = unitWidth;

            // QColor color = getColorForPid(current.getPid()); // do that if u want to get a random color
            lastRect = ganttScene->addRect(x, ganttTop, width, ganttBoxHeight,
                                           QPen(Qt::black), QBrush(QColor(230, 240, 255)));
            lastLabel = ganttScene->addText(QString::fromStdString("P" + to_string(current.getPid())));
            lastLabel->setPos(x + width/2 - 5, ganttTop + 10);

            QGraphicsTextItem* timeLabel = ganttScene->addText(QString::number(currentTime));
            timeLabel->setPos(currentTime * unitWidth - 5 + ganttRight, ganttTop + ganttBoxHeight + 10);

            lastStartTime = currentTime;
            last_pid = current.getPid();
        } else {
            // Same process — extend existing bar
            addRectangleInGantt(current);
        }

        QCoreApplication::processEvents();
        // update tables
        updateTables(current , myScheduler);
        // increment current time
        currentTime++;
    }
}

void MainWindow::updateTables(process current, Scheduler *myScheduler)
{
    // if finished mark as complete
    if(current.getRemainingTime() == 0){
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->completedProcesses->model());

        // Get the current row count
        int row = model->rowCount();

        // Insert a new row
        model->insertRow(row);

        // Set data in the new row
        model->setItem(row, 0, new QStandardItem(QString::number(current.getPid())));
        model->setItem(row, 1, new QStandardItem(QString::number(current.getCompletionTime())));
    }
    // update averages
    myScheduler->calculateAverageResponseTime();
    myScheduler->calculateAverageTurnAroundTime();
    myScheduler->calculateAverageWaitingTime();

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->averageTable->model());

    // Get the current row count
    int row = model->rowCount() - 1;

    // Set data in the new row
    model->setItem(row, 0, new QStandardItem(QString::number(myScheduler->getAverageResponseTime())));
    model->setItem(row, 1, new QStandardItem(QString::number(myScheduler->getAverageTurnAroundTime())));
    model->setItem(row, 1, new QStandardItem(QString::number(myScheduler->getAverageWaitingTime())));
}


void MainWindow::on_pushButton_clicked()
{
    bool missing = ui->ID_lineEdit->text().isEmpty() ||
                   ui->Arrival_lineEdit->text().isEmpty() ||
                   ui->Burst_lineEdit->text().isEmpty() ||
                   ui->Priority_lineEdit->text().isEmpty();
    if(missing){
        QMessageBox::warning(this, "Missing Fields", "Please fill all the required fields");
        return;
    }
    int pid = ui->ID_lineEdit->text().toInt();
    int arrival = ui->Arrival_lineEdit->text().toInt();
    int burst = ui->Burst_lineEdit->text().toInt();
    int priority = -1;
    if(ui->Priority_lineEdit->text() != "Disabled"){
        priority = ui->Priority_lineEdit->text().toInt();
        ui->Priority_lineEdit->setText("");
    }
    ui->Arrival_lineEdit->setText("");
    ui->ID_lineEdit->setText("");
    ui->Burst_lineEdit->setText("");
    process newProcess(pid , arrival , burst , priority);
    // added in the waiting queue
    WaitingProcesses.push_back(newProcess);
    // added in the system processes
    MyProcesses.push_back(newProcess);
    // appear in the tables
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->initialProcesses->model());

    // Get the current row count
    int row = model->rowCount();

    // Insert a new row
    model->insertRow(row);

    // Set data in the new row
    model->setItem(row, 0, new QStandardItem(QString::number(pid)));
    model->setItem(row, 1, new QStandardItem(QString::number(arrival)));
    model->setItem(row, 2, new QStandardItem(QString::number(burst)));
    model->setItem(row, 3, new QStandardItem(QString::number(priority)));
    /////////////////////////////////////////////////////////////////////
    //
    // add it the dynamic as incompleted
    model = qobject_cast<QStandardItemModel*>(ui->incompletedProcesses->model());
    newProcess.setRemainingTime(burst);
    // Insert a new row
    model->insertRow(row);

    // Set data in the new row
    model->setItem(row, 0, new QStandardItem(QString::number(pid)));
    model->setItem(row, 1, new QStandardItem(QString::number(newProcess.getRemainingTime())));
}



