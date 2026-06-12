#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QDate>
#include <QTime>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QTimer>
#include <QSet>
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <QVBoxLayout>
#include <QEvent>
#include <QLineEdit>
#include <QProgressBar>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>

struct TaskData
{
    QString title;
    QString category;
    QString priority;
    QDate reminderDate;
    QTime reminderTime;
    QTime workTime;
    bool isDone = false;
    bool isAlarmActive = false;
};

struct CourseEntry
{
    QString name;
    QTime startTime;
    QTime endTime;
};

struct SemesterData
{
    QString name;
    QList<QList<CourseEntry>> weeklySchedule; // [0]=Senin ...[6]=Minggu
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QList<TaskData> tasks;
    QSet<QString> notifiedTasks;
    QDate currentDate;

    QList<SemesterData> semesters;
    int activeSemesterIndex = -1;

    bool darkMode = false;
    bool showHistory = false;

    QLabel *totalLabel;
    QLabel *doneLabel;
    QLabel *notDoneLabel;
    QLabel *lateLabel;
    QLabel *monthLabel;
    QLabel *progressText;

    QProgressBar *progressBar;

    QComboBox *categoryBox;
    QComboBox *priorityBox;
    QLineEdit *searchBox;

    QTableWidget *taskList;
    QGridLayout *calendarGrid;

    QTimer *notificationTimer;

    QMediaPlayer *alarmPlayer;
    QAudioOutput *alarmAudio;
    QString alarmSoundPath;

    QString currentAlarmTask;

    QPushButton *historyButton;

    QFrame *scheduleFrame;
    QLabel *semesterNameLabel;
    QLabel *scheduleDayLabels[7];
    QLabel *scheduleDateLabels[7];
    QVBoxLayout *scheduleDayCourseLayouts[7];

    QFrame* createStatCard(QLabel *&numberLabel, const QString &text, const QString &color);

    void addTask();
    void editTask();
    void deleteTask();
    void markTaskDone();

    void openTaskDialog(int editIndex = -1, QDate selectedDate = QDate());

    void refreshTaskList();
    void updateStats();

    void buildCalendar();
    void clearCalendar();
    void addTaskToCalendarBox(QLayout *Layout, const QDate &date);

    void checkNotifications();
    void startAlarm(const QString &message, bool isDeadline = false);
    void stopAlarm();
    void chooseAlarmSound();

    void showTasksByDate(const QDate &date);

    void saveTasks();
    void loadTasks();

    QString taskText(const TaskData &task) const;
    QString countdownText(const TaskData &task) const;

    void buildScheduleView();
    void openScheduleManager();
    QDate weekStartDate() const;

    void saveSchedule();
    void loadSchedule();

    void applyTheme();
};

#endif