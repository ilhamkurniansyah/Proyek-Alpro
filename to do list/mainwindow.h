#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QDate>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QGridLayout>
#include <QTimer>
#include <QSet>
#include <Qtime>
#include <Qtimer>

struct TaskData
{
    QString title;
    QString category;
    QString priority;
    QDate reminderDate;
    QTime reminderTime;
    bool isDone = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTimer *timer;
    QVector<QTime> taskTimes;
    QDate currentDate;

    QLabel *totalLabel;
    QLabel *doneLabel;
    QLabel *notDoneLabel;
    QLabel *lateLabel;
    QLabel *monthLabel;

    QComboBox *categoryBox;
    QComboBox *priorityBox;
    QListWidget *taskList;
    QGridLayout *calendarGrid;

    QList<TaskData> tasks;

    QTimer *notificationTimer;
    QSet<QString> notifiedTasks;

    QFrame* createStatCard(QLabel *&numberLabel, const QString &text, const QString &color);

    void addTask();
    void deleteTask();
    void updateStats();
    void buildCalendar();
    void clearCalendar();
    void addTaskToCalendarBox(QVBoxLayout *boxLayout, const QDate &date);
    void checkNotifications();
    void showTasksByDate(const QDate &date);
    void markTaskDone();
};

#endif