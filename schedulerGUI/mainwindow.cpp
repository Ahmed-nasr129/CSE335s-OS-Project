#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QStandardItemModel>
#include <QStandardItem>


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
    setupInitialProcesses();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawGanttChart()
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    ui->graphicsView->setScene(scene);

    QGraphicsTextItem* title = scene->addText("Gantt Chart");
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setPos(100, 0);


    struct Task {
        QString label;
        int start;
        int end;
    };

    QVector<Task> tasks = {
        {"A", 0, 2},
        {"B", 2, 6},
        {"C", 6, 12}
    };

    int right = 10;
    int top = 40;
    int boxHeight = 40;
    int unitWidth = 20;

    for (const Task& task : tasks) {
        int x = task.start * unitWidth + right;
        int width = (task.end - task.start) * unitWidth;

        QGraphicsRectItem* rect = scene->addRect(x, top, width, boxHeight, QPen(Qt::blue), QBrush(QColor(230, 240, 255)));

        QGraphicsTextItem* label = scene->addText(task.label);
        label->setPos(x + width/2 - 5, top + 10);
    }

    QVector<int> timePoints = {0, 2, 6, 12};
    int timeTop = top + boxHeight + 10;

    for (int t : timePoints) {
        QGraphicsTextItem* timeLabel = scene->addText(QString::number(t));
        timeLabel->setPos(t * unitWidth - 5 + right, timeTop);
    }

    scene->setSceneRect(0, 0, 300, 120);
}


void MainWindow::setupAverageTable()
{
    QTableView* tableView = ui->averageTable;

    QStandardItemModel* model = new QStandardItemModel(0, 3, this);

    model->setHorizontalHeaderLabels({"Column 1", "Column 2", "Column 3"});

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}
void MainWindow::setupCompletedProcesses()
{
    QTableView* tableView = ui->completedProcesses;

    QStandardItemModel* model = new QStandardItemModel(0, 2, this);

    model->setHorizontalHeaderLabels({"Column 1", "Column 2"});

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}

void MainWindow::setupNotCompletedProcesses()
{
    QTableView* tableView = ui->notcompletedProcesses;

    QStandardItemModel* model = new QStandardItemModel(0, 2, this);

    model->setHorizontalHeaderLabels({"Column 1", "Column 2"});

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}

void MainWindow::setupInitialProcesses()
{
    QTableView* tableView = ui->initialProcesses;

    QStandardItemModel* model = new QStandardItemModel(0, 3, this);

    model->setHorizontalHeaderLabels({"Column 1", "Column 2", "Column 3"});

    QHeaderView* header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
}
