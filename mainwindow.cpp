#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    drawGanttChart();
    setupAverageTable();
    setupCompletedProcesses();
    setupNotCompletedProcesses();

    ui->error_Count->setVisible(false);
    ui->error_Scheduler->setVisible(false);
    ui->Error1->setVisible(false);
    ui->Error2->setVisible(false);
    ui->Error3->setVisible(false);

    ui->Error1_1->setVisible(false);
    ui->Error1_2->setVisible(false);
    ui->Error1_3->setVisible(false);

    ui->priority_Count->hide();
    ui->label_Priority->hide();
    ui->Error2->hide();

    ui->priority_Count_2->hide();
    ui->label_Priority_2->hide();
    ui->Error1_3->hide();

    ui->label_Quantum->hide();
    ui->Quantum_Count->hide();
    ui->quantum->hide();
    ui->Error3->hide();
    ui->header_3->hide();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_chose_Scheduler_clicked()
{
    scheduler=ui->buttonGroup->checkedId();
    count=ui->spin_Count->value();

    if(count<1)
        ui->error_Count->setVisible(true);
    else
        ui->error_Count->setVisible(false);
    if(scheduler==-1)
        ui->error_Scheduler->setVisible(true);
    else
        ui->error_Scheduler->setVisible(false);
    if(scheduler==-5 || scheduler==-6){
        ui->priority_Count->show();
        ui->label_Priority->show();
        ui->priority_Count_2->show();
        ui->label_Priority_2->show();
        //ui->Error2->show();
    }
    if(scheduler==-7){
        ui->label_Quantum->show();
        ui->Quantum_Count->show();
        ui->quantum->show();
        ui->header_3->show();
        //ui->Error3->show();
    }
    if(count>0 && scheduler!=-1){
        switch (scheduler) {
        case -2: myscheduler=new FCFS_Scheduler(); break;
        case -3: myscheduler=new SJF_NonPreemptive(); break;
        case -4: myscheduler=new SJF_Preemptive(); break;
        case -5: myscheduler=new priorityNonPreemtive(); break;
        case -6: myscheduler=new priorityPreemtive(); break;
        case -7: myscheduler=new RR_Scheduler(0); break;
        }
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        displayInitialTable();
    }
}


void MainWindow::on_add_process_clicked()
{
    int btime,atime,p;
    btime=ui->burst_Count->value();
    atime=ui->arrival_Count->value();
    p=ui->priority_Count->value();
    if(btime<1)
        ui->Error1->show();
    else
        ui->Error1->hide();
    if(scheduler==-5 || scheduler==-6){
        if(p<1)
            ui->Error2->show();
        else
            ui->Error2->hide();
    }
    if(btime>0 && ( (scheduler!=-5 && scheduler!=-6)   || p>0 )  ){
        if(++pid<=count){
            MyProcesses.push_back(process(pid,atime,btime,p));
            if(atime>currentTime)
                WaitingProcesses.push_back(process(pid,atime,btime,p));
            else
                myscheduler->addProcess(process(pid,atime,btime,p),atime);
            setupInitialTable(pid,atime,btime,p);
            ui->header_2->setText("Enter P"+QString::number(pid+1)+" Data");
            ui->burst_Count->setValue(0);
            ui->arrival_Count->setValue(0);
            ui->priority_Count->setValue(0);
        }
        if(pid>=count){
            ui->burst_Count->setDisabled(true);
            ui->arrival_Count->setDisabled(true);
            ui->priority_Count->setDisabled(true);
        }
    }
}


void MainWindow::on_quantum_clicked()
{
    int q=ui->Quantum_Count->value();
    if(q<1)
        ui->Error3->show();
    else{
        ui->Error3->hide();
        qtm=q;
        myscheduler =new RR_Scheduler(qtm);
        ui->Quantum_Count->setDisabled(true);
    }
}


void MainWindow::on_start_clicked()
{
    if(pid>=count && ((scheduler==-7 && qtm>0) ||  scheduler!=-7)){
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        setupRemTable();
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::upfn);
        timer->start(1000); // Update every 1000 milliseconds (1 second)


    }else{
        if(qtm<1 && scheduler==-7)
            ui->Error3->show();
        if(count>pid){
            ui->Error1->show();
            if(scheduler==-5 || scheduler==-6)
                ui->Error1->show();
        }
    }
}


void MainWindow::on_add_process_2_clicked()
{
    int btime,atime,p;
    btime=ui->burst_Count_2->value();
    atime=ui->arrival_Count_2->value();
    p=ui->priority_Count_2->value();
    if(btime<1)
        ui->Error1_1->show();
    else
        ui->Error1_1->hide();
    if(scheduler==-5 || scheduler==-6){
        if(p<1)
            ui->Error1_3->show();
        else
            ui->Error1_3->hide();
    }
    if(atime<=currentTime)
        ui->Error1_2->show();
    else
        ui->Error1_2->hide();
    if(btime>0 && ( (scheduler!=-5 && scheduler!=-6)   || p>0 ) && atime>currentTime ){
        MyProcesses.push_back( process(++pid,atime,btime,p));
        if(atime>currentTime)
            WaitingProcesses.push_back( process(pid,atime,btime,p));
        else
            myscheduler->addProcess(process(pid,atime,btime,p),atime);

        QStandardItemModel* model =qobject_cast<QStandardItemModel*>(ui->incompletedProcesses->model());
        int row=model->rowCount();
        model->insertRow(row);
        model->setItem(row, 0, new QStandardItem(QString::number(pid)));
        model->setItem(row, 1, new QStandardItem(QString::number(btime)));

        ui->burst_Count_2->setValue(0);
        ui->arrival_Count_2->setValue(0);
        ui->priority_Count_2->setValue(0);


    }
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
    title->setPos(10, 0);

    ganttScene->setSceneRect(0, 0, 1000, 120);
}

void MainWindow::addRectangleInGantt(process cur)
{
    int x = lastStartTime * unitWidth + ganttRight;
    int width = (currentTime - lastStartTime ) * unitWidth;

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


void MainWindow::upfn(){

    currentTime++;
    if(currentTime<0)
        ui->header_5->setText("Execution starts in "+QString::number(abs(currentTime)));
    else if(currentTime==0){
        ui->header_5->setText("Execution started !");
        fetchWaiting(myscheduler);

        startLiveExec(myscheduler);
    }else{
        fetchWaiting(myscheduler);
        updateRemTable(myscheduler->getCurrentProcess());
        startLiveExec(myscheduler);
    }
    /*
    if(myscheduler->getCompletedProcesses().size()>0 && currentTime==myscheduler->getCompletedProcesses()[myscheduler->getCompletedProcesses().size()-1].getCompletionTime()){
        QGraphicsTextItem* timeLabel = ganttScene->addText(QString::number(currentTime));
        timeLabel->setPos(currentTime * unitWidth - 5 + ganttRight, ganttTop + ganttBoxHeight + 10);
    }
    */
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

void MainWindow::startLiveExec(Scheduler *myScheduler)
{
    offlinePressed = false;

    if(!MyProcesses.empty()){
        // check if offline is pressed
        if(offlinePressed) return; // terminate

        // check if there is a waiting process
        fetchWaiting(myScheduler);
        updateTables(process() , myScheduler);

        if(myscheduler->getCompletedProcesses().size()==MyProcesses.size() && myScheduler->getCompletedProcesses()[myscheduler->getCompletedProcesses().size()-1].getCompletionTime()<currentTime){

            return;
        }
        myScheduler->beginOperation(currentTime);
        process current = myScheduler->getCurrentProcess();

        // simulate sleeping 1 second
        QGraphicsTextItem* timeLabel = ganttScene->addText(QString::number(currentTime));
        timeLabel->setPos(currentTime * unitWidth - 5 + ganttRight, ganttTop + ganttBoxHeight + 10);

        if(current.getPid()==0)
            return;
        int x = currentTime * unitWidth + ganttRight;
        int width = 0;
        if (current.getPid() != last_pid) {
                // New process, draw new rectangle

                if(lastStartTime!=-1){
                    lastLabel->show();
                    addRectangleInGantt(current);
                }

                // QColor color = getColorForPid(current.getPid()); // do that if u want to get a random color
                lastRect = ganttScene->addRect(x, ganttTop, width, ganttBoxHeight,
                                               QPen(Qt::black), QBrush(QColor(230, 240, 255)));

                lastLabel = ganttScene->addText(QString::fromStdString("P" + to_string(current.getPid())));
                lastLabel->setPos(x + width/2 - 5, ganttTop + 10);
                lastLabel->hide();


                lastStartTime = currentTime;
                last_pid = current.getPid();
        } else {
                // Same process — extend existing bar
            if(currentTime>current.getCompletionTime() && current.getCompletionTime()!=0){
                    lastRect = ganttScene->addRect(x, ganttTop, width, ganttBoxHeight,
                                                   QPen(Qt::black), QBrush(QColor(255, 255, 255)));
                    lastLabel = ganttScene->addText("");
                    lastLabel->setPos(x + width/2 - 5, ganttTop + 10);
                    lastStartTime=-1;
            }else{
                lastLabel->show();
                addRectangleInGantt(current);
            }
        }
        QCoreApplication::processEvents();
        // update tables

        // increment current time
        //currentTime++;
    }
}

void MainWindow::updateTables(process current, Scheduler *myScheduler)
{
    // if finished mark as complete
    if(myscheduler->getCompletedProcesses().size()>0 && currentTime==myscheduler->getCompletedProcesses()[myscheduler->getCompletedProcesses().size()-1].getCompletionTime()){
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->completedProcesses->model());

        // Get the current row count
        int row = model->rowCount();

        // Insert a new row
        model->insertRow(row);

        // Set data in the new row
        model->setItem(row, 0, new QStandardItem(QString::number(myscheduler->getCompletedProcesses()[myscheduler->getCompletedProcesses().size()-1].getPid())));
        model->setItem(row, 1, new QStandardItem(QString::number(myscheduler->getCompletedProcesses()[myscheduler->getCompletedProcesses().size()-1].getCompletionTime())));
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
    model->setItem(row, 2, new QStandardItem(QString::number(myScheduler->getAverageWaitingTime())));
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

void MainWindow::setupInitialTable(int pid,int arrival,int burst,int priority){
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

}

void MainWindow::setupRemTable(){
    QStandardItemModel* model =qobject_cast<QStandardItemModel*>(ui->incompletedProcesses->model());
    for(int i=0;i<MyProcesses.size();i++){
        // Insert a new row
        model->insertRow(i);
        // Set data in the new row
        model->setItem(i, 0, new QStandardItem(QString::number(MyProcesses[i].getPid())));
        model->setItem(i, 1, new QStandardItem(QString::number(MyProcesses[i].getRemainingTime())));
    }
}

void MainWindow::updateRemTable(process current){
    QStandardItemModel* model =qobject_cast<QStandardItemModel*>(ui->incompletedProcesses->model());

    for(int i=0;i<model->rowCount();i++){
        if(model->data(model->index(i,0)).toInt()==current.getPid()){
            if(current.getRemainingTime()==0)
                model->removeRow(i);
            else
                model->setItem(i, 1, new QStandardItem(QString::number(current.getRemainingTime())));
        }

    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->header_5->setText("Execution paused !");
    timer->stop();
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->header_5->setText("Execution resumed !");
    if(!(timer->isActive()))
        timer->start(1000);
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->header_5->setText("Live execution disabled !");
    timer->stop();
    timer->start(0);
}

