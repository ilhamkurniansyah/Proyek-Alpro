//Ilham kurniansyah
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QDate>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QDate currentDate;

    QLabel *totalLabel;
    QLabel *doneLabel;
    QLabel *notDoneLabel;
    QLabel *lateLabel;
    QLabel *monthLabel;

    QListWidget *taskList;
    QComboBox *categoryBox;
    QComboBox *priorityBox;

    QGridLayout *calendarGrid;

    QFrame* createStatCard(QLabel *&numberLabel, const QString &text, const QString &color);

    void addTask();
    void deleteTask();
    void updateStats();
    void buildCalendar();
    void clearCalendar();
};

#endif
