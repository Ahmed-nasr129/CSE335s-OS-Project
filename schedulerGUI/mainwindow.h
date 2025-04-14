#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    void drawGanttChart();
    void setupAverageTable();
    void setupCompletedProcesses();
    void setupNotCompletedProcesses();
    void setupInitialProcesses();
};


#endif // MAINWINDOW_H


