#include "mainwindow.h"
#include <QIcon>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QComboBox>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QDialog>
#include <QDateEdit>
#include <QTimeEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QScrollArea>
#include <QApplication>
#include <QEvent>
#include <QVariant>
#include <QSettings>
#include <QBrush>
#include <QProgressBar>
#include <QTimer>
#include <QFileDialog>
#include <QUrl>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCalendarWidget>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDate(QDate::currentDate())
{
    setWindowTitle("To-Do List Saya");
    setWindowIcon(QIcon("logo.ico"));
    resize(1450, 850);

    QWidget *root = new QWidget(this);
    root->setObjectName("root");

    QVBoxLayout *mainLayout = new QVBoxLayout(root);
    mainLayout->setContentsMargins(18, 10, 18, 0);
    mainLayout->setSpacing(6);

    QLabel *title = new QLabel("To-Do List Saya");
    title->setObjectName("title");

    QLabel *subtitle = new QLabel("jadi gampang tugas kamu kan!!!");
    subtitle->setObjectName("subtitle");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    QHBoxLayout *statLayout = new QHBoxLayout;
    statLayout->setSpacing(14);

    statLayout->addWidget(createStatCard(totalLabel, "Total Tugas", "#2563EB"));
    statLayout->addWidget(createStatCard(doneLabel, "Selesai", "#00A650"));
    statLayout->addWidget(createStatCard(notDoneLabel, "Belum Selesai", "#F0440A"));
    statLayout->addWidget(createStatCard(lateLabel, "Terlambat", "#E60000"));

    mainLayout->addLayout(statLayout);

    progressText = new QLabel("Progress: 0%");
    progressText->setObjectName("smallTitle");

    progressBar = new QProgressBar;
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);

    mainLayout->addWidget(progressText);
    mainLayout->addWidget(progressBar);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setSpacing(14);

    QFrame *calendarPanel = new QFrame;
    calendarPanel->setObjectName("calendarPanel");

    QVBoxLayout *calendarLayout = new QVBoxLayout(calendarPanel);
    calendarLayout->setContentsMargins(18, 8, 18, 8);
    calendarLayout->setSpacing(4);

    // --- Section Jadwal Kuliah ---
    scheduleFrame = new QFrame;
    scheduleFrame->setObjectName("scheduleFrame");
    QVBoxLayout *scheduleOuterLayout = new QVBoxLayout(scheduleFrame);
    scheduleOuterLayout->setContentsMargins(0, 0, 0, 2);
    scheduleOuterLayout->setSpacing(3);

    QHBoxLayout *scheduleHeaderRow = new QHBoxLayout;
    QLabel *scheduleTitle = new QLabel("Jadwal Kuliah");
    scheduleTitle->setObjectName("scheduleTitle");

    semesterNameLabel = new QLabel("(belum ada semester)");
    semesterNameLabel->setObjectName("semesterNameLabel");

    QPushButton *manageScheduleBtn = new QPushButton("Atur Jadwal");
    manageScheduleBtn->setObjectName("manageScheduleBtn");
    manageScheduleBtn->setCursor(Qt::PointingHandCursor);
    manageScheduleBtn->setMinimumHeight(26);

    scheduleHeaderRow->addWidget(scheduleTitle);
    scheduleHeaderRow->addSpacing(4);
    scheduleHeaderRow->addWidget(semesterNameLabel);
    scheduleHeaderRow->addStretch();
    scheduleHeaderRow->addWidget(manageScheduleBtn);

    QHBoxLayout *scheduleDaysLayout = new QHBoxLayout;
    scheduleDaysLayout->setSpacing(3);

    QStringList dayNames = {"Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"};
    for (int d = 0; d < 7; d++) {
        QFrame *dayFrame = new QFrame;
        dayFrame->setObjectName("scheduleDayFrame");

        QVBoxLayout *dayLayout = new QVBoxLayout(dayFrame);
        dayLayout->setContentsMargins(3, 3, 3, 3);
        dayLayout->setSpacing(1);

        scheduleDayLabels[d] = new QLabel(dayNames[d]);
        scheduleDayLabels[d]->setObjectName("scheduleDayName");
        scheduleDayLabels[d]->setAlignment(Qt::AlignCenter);

        scheduleDateLabels[d] = new QLabel;
        scheduleDateLabels[d]->setObjectName("scheduleDateLabel");
        scheduleDateLabels[d]->setAlignment(Qt::AlignCenter);

        scheduleDayCourseLayouts[d] = new QVBoxLayout;
        scheduleDayCourseLayouts[d]->setSpacing(1);

        dayLayout->addWidget(scheduleDayLabels[d]);
        dayLayout->addWidget(scheduleDateLabels[d]);
        dayLayout->addLayout(scheduleDayCourseLayouts[d]);
        dayLayout->addStretch();

        scheduleDaysLayout->addWidget(dayFrame);
    }

    scheduleOuterLayout->addLayout(scheduleHeaderRow);
    scheduleOuterLayout->addLayout(scheduleDaysLayout);

    calendarLayout->addWidget(scheduleFrame);

    connect(manageScheduleBtn, &QPushButton::clicked, this, &MainWindow::openScheduleManager);

    QHBoxLayout *calendarHeader = new QHBoxLayout;

    monthLabel = new QLabel;
    monthLabel->setObjectName("monthTitle");

    QPushButton *prevButton = new QPushButton("‹");
    QPushButton *todayButton = new QPushButton("Hari Ini");
    QPushButton *nextButton = new QPushButton("›");

    prevButton->setObjectName("navBtn");
    todayButton->setObjectName("todayBtn");
    nextButton->setObjectName("navBtn");

    calendarHeader->addWidget(monthLabel);
    calendarHeader->addStretch();
    calendarHeader->addWidget(prevButton);
    calendarHeader->addWidget(todayButton);
    calendarHeader->addWidget(nextButton);

    calendarGrid = new QGridLayout;
    calendarGrid->setSpacing(3);

    calendarLayout->addLayout(calendarHeader);
    calendarLayout->addLayout(calendarGrid);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(calendarPanel);
    scrollArea->setFrameShape(QFrame::NoFrame);

    contentLayout->addWidget(scrollArea, 2);

    QFrame *controlPanel = new QFrame;
    controlPanel->setObjectName("panel");
    controlPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *panelLayout = new QVBoxLayout(controlPanel);
    panelLayout->setContentsMargins(20, 14, 20, 0);
    panelLayout->setSpacing(8);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(8);

    QPushButton *addButton = new QPushButton("+ Tambah");
    QPushButton *editButton = new QPushButton("✎ Edit");
    QPushButton *deleteButton = new QPushButton("☷ Hapus");
    QPushButton *doneButton = new QPushButton("✓ Selesai");
    QPushButton *notifButton = new QPushButton("⏰ Cek Alarm");
    QPushButton *soundButton = new QPushButton("🎵 Lagu");
    QPushButton *themeButton = new QPushButton("🌙 Dark");

    addButton->setObjectName("addBtn");
    editButton->setObjectName("editBtn");
    deleteButton->setObjectName("listBtn");
    doneButton->setObjectName("doneBtn");
    notifButton->setObjectName("notifBtn");
    soundButton->setObjectName("soundBtn");
    themeButton->setObjectName("themeBtn");

    historyButton = new QPushButton(this);

    historyButton->setText("📋 Menampilkan: Tugas Aktif (Klik untuk melihat Riwayat)");
    historyButton->setStyleSheet("background-color: #17a2b8; color: white; font-weight: bold; padding: 6px; border-radius: 5px;");

    panelLayout->addWidget(historyButton);

    connect(historyButton, &QPushButton::clicked, this, [this]() {
        showHistory = !showHistory;

        if (showHistory) {
            historyButton->setText("📜 Menampilkan: Riwayat Tugas Selesai");
            historyButton->setStyleSheet("background-color: #6c757d; color: white; font-weight: bold; padding: 6px; border-radius: 5px;");
        } else {
            historyButton->setText("📋 Menampilkan: Tugas Aktif (Klik untuk melihat Riwayat)");
            historyButton->setStyleSheet("background-color: #17a2b8; color: white; font-weight: bold; padding: 6px; border-radius: 5px;");
        }
        refreshTaskList();
    });

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(doneButton);
    notifButton->hide();
    buttonLayout->addWidget(soundButton);
    buttonLayout->addWidget(themeButton);

    searchBox = new QLineEdit;
    searchBox->setPlaceholderText("Cari tugas...");

    categoryBox = new QComboBox;
    categoryBox->addItems({"Semua Kategori", "Kuliah", "Kerja", "Pribadi", "Umum"});
    categoryBox->setMaxVisibleItems(10);
    categoryBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    categoryBox->installEventFilter(this);

    priorityBox = new QComboBox;
    priorityBox->addItems({"Semua Prioritas", "Tinggi", "Sedang", "Rendah", "Normal"});
    priorityBox->setMaxVisibleItems(10);
    priorityBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    priorityBox->installEventFilter(this);

    taskList = new QTableWidget;
    taskList->setObjectName("taskList");
    taskList->setColumnCount(9);
    taskList->setHorizontalHeaderLabels({"", "Nama Tugas", "Kategori", "Prioritas", "Tgl Kerjakan", "Jam Kerjakan", "Tgl Deadline", "Jam Deadline", "Status"});
    taskList->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskList->setSelectionMode(QAbstractItemView::MultiSelection);
    taskList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    taskList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    taskList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    taskList->setColumnWidth(0, 25);
    taskList->setColumnWidth(2, 60);
    taskList->setColumnWidth(3, 60);
    taskList->setColumnWidth(4, 85);
    taskList->setColumnWidth(5, 80);
    taskList->setColumnWidth(6, 85);
    taskList->setColumnWidth(7, 80);
    taskList->setColumnWidth(8, 90);
    taskList->verticalHeader()->setVisible(false);
    taskList->setShowGrid(true);
    taskList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    taskList->setWordWrap(true);
    taskList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    taskList->setMinimumHeight(0);
    taskList->setAlternatingRowColors(true);

    QLabel *taskTitle = new QLabel("Daftar Tugas");
    taskTitle->setObjectName("smallTitle");

    panelLayout->addLayout(buttonLayout);
    panelLayout->addWidget(searchBox);
    panelLayout->addWidget(categoryBox);
    panelLayout->addWidget(priorityBox);
    panelLayout->addWidget(taskTitle);
    panelLayout->addWidget(taskList);

    contentLayout->addWidget(controlPanel, 3);

    mainLayout->addLayout(contentLayout, 10);

    setCentralWidget(root);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteTask);
    connect(doneButton, &QPushButton::clicked, this, &MainWindow::markTaskDone);

    connect(notifButton, &QPushButton::clicked, this, [=]() {
        QTableWidgetItem *item = taskList->currentItem();
        if (!item) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Peringatan");
            msgBox.setText("Pilih salah satu tugas terlebih dahulu untuk mengecek alarm.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet(
                "QMessageBox { background-color: #121212; }"
                "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
                "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
                "QPushButton:hover { background-color: #1565C0; }"
                );
            msgBox.exec();
            return;
        }

        int index = item->data(Qt::UserRole).toInt();
        if (index >= 0 && index < tasks.size()) {
            tasks[index].isAlarmActive = true;
            saveTasks();
            checkNotifications();
            refreshTaskList();
        }
    });

    connect(soundButton, &QPushButton::clicked, this, &MainWindow::chooseAlarmSound);

    connect(themeButton, &QPushButton::clicked, this, [=]() {
        darkMode = !darkMode;
        applyTheme();
        buildCalendar();

        if (darkMode) {
            themeButton->setText("☀️ Light");
        } else {
            themeButton->setText("🌙 Dark");
        }
    });

    this->setStyleSheet(this->styleSheet() +
                        "QMessageBox {"
                        "   background-color: #121212;"
                        "}"
                        "QMessageBox QLabel {"
                        "   color: #FFFFFF;"
                        "   font-size: 12px;"
                        "}"
                        "QMessageBox QPushButton {"
                        "   background-color: #2979FF;"
                        "   color: white;"
                        "   border-radius: 4px;"
                        "   padding: 5px 15px;"
                        "   font-weight: bold;"
                        "   min-width: 65px;"
                        "}"
                        "QMessageBox QPushButton:hover {"
                        "   background-color: #1565C0;"
                        "}"
                        );

    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::refreshTaskList);
    connect(categoryBox, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);
    connect(priorityBox, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);

    connect(prevButton, &QPushButton::clicked, this, [=]() {
        currentDate = currentDate.addMonths(-1);
        buildCalendar();
        buildScheduleView();
    });

    connect(nextButton, &QPushButton::clicked, this, [=]() {
        currentDate = currentDate.addMonths(1);
        buildCalendar();
        buildScheduleView();
    });

    connect(todayButton, &QPushButton::clicked, this, [=]() {
        currentDate = QDate::currentDate();
        buildCalendar();
        buildScheduleView();
    });

    notificationTimer = new QTimer(this);

    connect(notificationTimer, &QTimer::timeout, this, [=]() {
        checkNotifications();
        updateStats();
        buildScheduleView();
    });

    notificationTimer->start(1000);

    alarmAudio = new QAudioOutput(this);
    alarmAudio->setVolume(0.9);

    alarmPlayer = new QMediaPlayer(this);
    alarmPlayer->setAudioOutput(alarmAudio);

    QSettings settings("MyCompany", "TodoListSaya");
    alarmSoundPath = settings.value("alarmSoundPath").toString();

    if (!alarmSoundPath.isEmpty()) {
        alarmPlayer->setSource(QUrl::fromLocalFile(alarmSoundPath));
    }

    loadTasks();
    loadSchedule();

    if (semesters.isEmpty()) {
        SemesterData defaultSem;
        defaultSem.name = "Semester 1";
        defaultSem.weeklySchedule.resize(7);
        semesters.append(defaultSem);
        activeSemesterIndex = 0;
        saveSchedule();
    }

    searchBox->clear();
    categoryBox->setCurrentText("Semua Kategori");
    priorityBox->setCurrentText("Semua Prioritas");

    refreshTaskList();
    buildCalendar();
    buildScheduleView();
    updateStats();
    applyTheme();

    connect(taskList, &QTableWidget::itemClicked, this, [this](QTableWidgetItem *item) {
        if (!item) return;
        int row = item->row();
        QTableWidgetItem *checkItem = taskList->item(row, 0);
        if (!checkItem) return;
        int index = checkItem->data(Qt::UserRole).toInt();
        if (index >= 0 && index < tasks.size()) {
            taskList->blockSignals(true);
            bool newChecked = !(checkItem->checkState() == Qt::Checked);
            checkItem->setCheckState(newChecked ? Qt::Checked : Qt::Unchecked);
            tasks[index].isAlarmActive = newChecked;
            saveTasks();
            taskList->blockSignals(false);
            updateStats();
        }
    });
}

QFrame* MainWindow::createStatCard(QLabel *&numberLabel, const QString &text, const QString &color)
{
    QFrame *card = new QFrame;
    card->setObjectName("card");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 10, 20, 10);

    numberLabel = new QLabel("0");
    numberLabel->setObjectName("cardNumber");
    numberLabel->setStyleSheet("color: " + color + ";");

    QLabel *label = new QLabel(text);
    label->setObjectName("cardText");

    layout->addWidget(numberLabel);
    layout->addWidget(label);

    return card;
}

QString MainWindow::countdownText(const TaskData &task) const
{
    if (task.isDone) {
        return "Selesai";
    }

    QDateTime now = QDateTime::currentDateTime();
    QDateTime deadline(task.reminderDate, task.reminderTime);

    qint64 seconds = now.secsTo(deadline);

    if (seconds < 0) {
        return "Terlambat";
    }

    int days = seconds / 86400;
    int hours = (seconds % 86400) / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;

    if (days > 0) {
        return QString("%1 hari %2 jam lagi").arg(days).arg(hours);
    }

    if (hours > 0) {
        return QString("%1 jam %2 menit %3 detik lagi").arg(hours).arg(minutes).arg(secs);
    }

    return QString("%1 menit %2 detik lagi").arg(minutes).arg(secs);
}

QString MainWindow::taskText(const TaskData &task) const
{
    QString status;

    if (task.isDone) {
        status = "✔️ Selesai";
    } else {
        if (task.isAlarmActive) {
            status = countdownText(task);
        } else {
            status = "⏳ Belum selesai";
        }
    }

    return task.title + " | " +
           task.category + " | " +
           task.priority + " | " +
           task.workDate.toString("dd MMM yyyy") + " | " +
           task.workTime.toString("hh:mm") + " | " +
           task.reminderDate.toString("dd MMM yyyy") + " | " +
           task.reminderTime.toString("hh:mm") + " | " +
           status;
}

void MainWindow::addTask()
{
    openTaskDialog(-1);
}

void MainWindow::editTask()
{
    int currentRow = taskList->currentRow();

    if (currentRow < 0) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Peringatan");
        msgBox.setText("Pilih tugas yang ingin diedit.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #121212; }"
            "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
            "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
            "QPushButton:hover { background-color: #1565C0; }"
            );
        msgBox.exec();
        return;
    }

    QTableWidgetItem *item = taskList->item(currentRow, 0);

    if (!item) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Peringatan");
        msgBox.setText("Data tugas tidak valid.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #121212; }"
            "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
            "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
            "QPushButton:hover { background-color: #1565C0; }"
            );
        msgBox.exec();
        return;
    }

    int index = item->data(Qt::UserRole).toInt();

    if (index < 0 || index >= tasks.size()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Peringatan");
        msgBox.setText("Data tugas tidak valid.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #121212; }"
            "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
            "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
            "QPushButton:hover { background-color: #1565C0; }"
            );
        msgBox.exec();
        return;
    }

    openTaskDialog(index);
}

void MainWindow::openTaskDialog(int editIndex, QDate selectedDate)
{
    QDialog dialog(this);
    dialog.setWindowTitle(editIndex >= 0 ? "Edit Tugas" : "Tambah Tugas");
    dialog.resize(380, 340);

    dialog.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    dialog.setStyleSheet(
        "QDialog {"
        "   background-color: #121212;"
        "}"
        "QLabel {"
        "   color: #FFFFFF;"
        "   font-weight: bold;"
        "}"
        "QLineEdit, QComboBox, QDateEdit, QTimeEdit {"
        "   background-color: #1E1E1E;"
        "   color: #FFFFFF;"
        "   border: 1px solid #333333;"
        "   border-radius: 4px;"
        "   padding: 4px;"
        "}"
        "QAbstractSpinBox {"
        "   background-color: #1E1E1E;"
        "   color: #FFFFFF;"
        "   border: 1px solid #333333;"
        "   border-radius: 4px;"
        "   padding: 4px;"
        "}"
        "QAbstractSpinBox::up-button, QAbstractSpinBox::down-button {"
        "   background-color: #2A2A2A;"
        "   border: 1px solid #444444;"
        "   width: 16px;"
        "}"
        "QAbstractSpinBox::up-button:hover, QAbstractSpinBox::down-button:hover {"
        "   background-color: #3A3A3A;"
        "}"
        "QAbstractSpinBox::up-arrow {"
        "   image: none;"
        "   border-left: 4px solid transparent;"
        "   border-right: 4px solid transparent;"
        "   border-bottom: 5px solid #FFFFFF;"
        "   width: 0; height: 0;"
        "}"
        "QAbstractSpinBox::down-arrow {"
        "   image: none;"
        "   border-left: 4px solid transparent;"
        "   border-right: 4px solid transparent;"
        "   border-top: 5px solid #FFFFFF;"
        "   width: 0; height: 0;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #1E1E1E;"
        "   color: #FFFFFF;"
        "   selection-background-color: #2979FF;"
        "}"
        "QPushButton {"
        "   color: white;"
        "   border-radius: 4px;"
        "   padding: 6px 14px;"
        "   font-weight: bold;"
        "   min-width: 70px;"
        "}"
        "QPushButton[text=\"OK\"] {"
        "   background-color: #00A650;"
        "}"
        "QPushButton[text=\"OK\"]:hover {"
        "   background-color: #008540;"
        "}"
        "QPushButton[text=\"Cancel\"] {"
        "   background-color: #E60000;"
        "}"
        "QPushButton[text=\"Cancel\"]:hover {"
        "   background-color: #B30000;"
        "}"
        );

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *taskInput = new QLineEdit;
    taskInput->setPlaceholderText("Masukkan nama tugas");

    QComboBox *categoryInput = new QComboBox;
    categoryInput->addItems({"Kuliah", "Kerja", "Pribadi", "Umum"});

    QComboBox *priorityInput = new QComboBox;
    priorityInput->addItems({"Tinggi", "Sedang", "Rendah", "Normal"});

    QDateEdit *workDateInput = new QDateEdit;
    workDateInput->setCalendarPopup(true);
    workDateInput->setDisplayFormat("dd MMMM yyyy");
    workDateInput->calendarWidget()->setStyleSheet(
        "QCalendarWidget QWidget { background-color: #1E1E1E; color: white; }"
        "QCalendarWidget QMenu { background-color: #1E1E1E; color: white; }"
        "QCalendarWidget QSpinBox { color: white; background-color: #121212; }"
        );

    QTimeEdit *workTimeInput = new QTimeEdit;
    workTimeInput->setDisplayFormat("HH:mm");
    workTimeInput->setTime(QTime(8, 0));
    workTimeInput->setKeyboardTracking(true);
    workTimeInput->setWrapping(true);

    QDateEdit *deadlineDateInput = new QDateEdit;
    deadlineDateInput->setCalendarPopup(true);
    deadlineDateInput->setDisplayFormat("dd MMMM yyyy");
    deadlineDateInput->calendarWidget()->setStyleSheet(
        "QCalendarWidget QWidget { background-color: #1E1E1E; color: white; }"
        "QCalendarWidget QMenu { background-color: #1E1E1E; color: white; }"
        "QCalendarWidget QSpinBox { color: white; background-color: #121212; }"
        );

    QTimeEdit *timeInput = new QTimeEdit;
    timeInput->setDisplayFormat("HH:mm");
    timeInput->setKeyboardTracking(true);
    timeInput->setWrapping(true);

    if (editIndex >= 0 && editIndex < tasks.size()) {
        TaskData task = tasks[editIndex];
        taskInput->setText(task.title);
        categoryInput->setCurrentText(task.category);
        priorityInput->setCurrentText(task.priority);
        workDateInput->setDate(task.workDate.isValid() ? task.workDate : task.reminderDate);
        workTimeInput->setTime(task.workTime.isValid() ? task.workTime : QTime(8, 0));
        deadlineDateInput->setDate(task.reminderDate);
        timeInput->setTime(task.reminderTime);
    } else {
        QDate defaultDate = selectedDate.isValid() ? selectedDate : QDate::currentDate();
        workDateInput->setDate(defaultDate);
        deadlineDateInput->setDate(defaultDate);
        timeInput->setTime(QTime::currentTime());
    }

    QFormLayout *form = new QFormLayout;
    form->addRow("Nama Tugas:", taskInput);
    form->addRow("Kategori:", categoryInput);
    form->addRow("Prioritas:", priorityInput);
    form->addRow("Tanggal Kerjakan:", workDateInput);
    form->addRow("Jam Kerjakan:", workTimeInput);
    form->addRow("Tanggal Deadline:", deadlineDateInput);
    form->addRow("Jam Deadline:", timeInput);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
        );

    layout->addLayout(form);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString title = taskInput->text().trimmed();

        if (title.isEmpty()) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Peringatan");
            msgBox.setText("Nama tugas tidak boleh kosong.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet(
                "QMessageBox { background-color: #121212; }"
                "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
                "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
                "QPushButton:hover { background-color: #1565C0; }"
                );
            msgBox.exec();
            return;
        }

        TaskData task;
        task.title = title;
        task.category = categoryInput->currentText();
        task.priority = priorityInput->currentText();
        task.workDate = workDateInput->date();
        task.workTime = workTimeInput->time();
        task.reminderDate = deadlineDateInput->date();
        task.reminderTime = timeInput->time();

        if (editIndex >= 0 && editIndex < tasks.size()) {
            task.isDone = tasks[editIndex].isDone;
            task.isAlarmActive = tasks[editIndex].isAlarmActive;
            tasks[editIndex] = task;
        } else {
            task.isDone = false;
            task.isAlarmActive = false;
            tasks.append(task);
        }

        saveTasks();

        searchBox->clear();
        categoryBox->setCurrentText("Semua Kategori");
        priorityBox->setCurrentText("Semua Prioritas");

        refreshTaskList();
        buildCalendar();
        updateStats();
    }
}

void MainWindow::deleteTask()
{
    QList<int> indicesToRemove;

    for (int i = 0; i < taskList->rowCount(); ++i) {
        QTableWidgetItem *item = taskList->item(i, 0);
        if (item && item->checkState() == Qt::Checked) {
            int index = item->data(Qt::UserRole).toInt();
            if (index >= 0 && index < tasks.size()) {
                indicesToRemove.append(index);
            }
        }
    }

    if (indicesToRemove.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Peringatan");
        msgBox.setText("Centang satu atau beberapa kotak tugas yang ingin dihapus terlebih dahulu.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #121212; }"
            "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
            "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
            "QPushButton:hover { background-color: #1565C0; }"
            );
        msgBox.exec();
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Konfirmasi Hapus Massal");
    msgBox.setText(QString("Yakin ingin menghapus %1 tugas yang dicentang?").arg(indicesToRemove.size()));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #121212; }"
        "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
        "QPushButton { background-color: #E60000; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
        "QPushButton:hover { background-color: #B30000; }"
        );

    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    std::sort(indicesToRemove.begin(), indicesToRemove.end(), std::greater<int>());

    for (int index : indicesToRemove) {
        tasks.removeAt(index);
    }

    saveTasks();
    searchBox->clear();
    categoryBox->setCurrentText("Semua Kategori");
    priorityBox->setCurrentText("Semua Prioritas");
    refreshTaskList();
    buildCalendar();
    updateStats();

    QMessageBox infoBox(this);
    infoBox.setWindowTitle("Berhasil");
    infoBox.setText(QString("%1 tugas berhasil dihapus.").arg(indicesToRemove.size()));
    infoBox.setIcon(QMessageBox::Information);
    infoBox.setStyleSheet(
        "QMessageBox { background-color: #121212; }"
        "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
        "QPushButton { background-color: #00A650; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
        );
    infoBox.exec();
}

void MainWindow::markTaskDone()
{
    int updatedCount = 0;

    for (int i = 0; i < taskList->rowCount(); ++i) {
        QTableWidgetItem *item = taskList->item(i, 0);
        if (item && item->checkState() == Qt::Checked) {
            int index = item->data(Qt::UserRole).toInt();
            if (index >= 0 && index < tasks.size()) {
                tasks[index].isDone = true;
                tasks[index].isAlarmActive = false;
                updatedCount++;
            }
        }
    }

    if (updatedCount == 0) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Peringatan");
        msgBox.setText("Centang satu atau beberapa kotak tugas terlebih dahulu.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #121212; }"
            "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
            "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
            "QPushButton:hover { background-color: #1565C0; }"
            );
        msgBox.exec();
        return;
    }

    saveTasks();
    refreshTaskList();
    buildCalendar();
    updateStats();
}

void MainWindow::refreshTaskList()
{
    if (!taskList || !searchBox || !categoryBox || !priorityBox)
        return;

    taskList->blockSignals(true);
    taskList->setRowCount(0);

    QString search = searchBox->text().trimmed().toLower();
    QString selectedCategory = categoryBox->currentText();
    QString selectedPriority = priorityBox->currentText();

    for (int i = 0; i < tasks.size(); i++) {
        const TaskData &task = tasks[i];

        bool searchOk = search.isEmpty() || task.title.toLower().contains(search);
        bool categoryOk = selectedCategory == "Semua Kategori" || task.category == selectedCategory;
        bool priorityOk = selectedPriority == "Semua Prioritas" || task.priority == selectedPriority;
        bool historyFilterOk = showHistory ? task.isDone : !task.isDone;

        if (!searchOk || !categoryOk || !priorityOk || !historyFilterOk)
            continue;

        int row = taskList->rowCount();
        taskList->insertRow(row);

        // Kolom 0: Checkbox
        QTableWidgetItem *checkItem = new QTableWidgetItem;
        checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        checkItem->setCheckState(task.isAlarmActive ? Qt::Checked : Qt::Unchecked);
        checkItem->setData(Qt::UserRole, i);
        checkItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 0, checkItem);

        // Kolom 1: Nama Tugas
        QTableWidgetItem *titleItem = new QTableWidgetItem(task.title);
        titleItem->setData(Qt::UserRole, i);
        taskList->setItem(row, 1, titleItem);

        // Kolom 2: Kategori
        QTableWidgetItem *catItem = new QTableWidgetItem(task.category);
        catItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 2, catItem);

        // Kolom 3: Prioritas
        QTableWidgetItem *priItem = new QTableWidgetItem(task.priority);
        priItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 3, priItem);

        // Kolom 4: Tanggal Kerjakan
        QTableWidgetItem *workDateItem = new QTableWidgetItem(
            task.workDate.isValid() ? task.workDate.toString("dd MMM yyyy") : task.reminderDate.toString("dd MMM yyyy")
            );
        workDateItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 4, workDateItem);

        // Kolom 5: Jam Kerjakan
        QTableWidgetItem *workTimeItem = new QTableWidgetItem(task.workTime.toString("hh:mm"));
        workTimeItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 5, workTimeItem);

        // Kolom 6: Tanggal Deadline
        QTableWidgetItem *deadlineDateItem = new QTableWidgetItem(task.reminderDate.toString("dd MMM yyyy"));
        deadlineDateItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 6, deadlineDateItem);

        // Kolom 7: Jam Deadline
        QTableWidgetItem *timeItem = new QTableWidgetItem(task.reminderTime.toString("hh:mm"));
        timeItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 7, timeItem);

        // Kolom 8: Status
        QString statusText;
        if (task.isDone) {
            statusText = "Selesai";
        } else {
            QDateTime now = QDateTime::currentDateTime();
            QDateTime deadline(task.reminderDate, task.reminderTime);
            statusText = (now > deadline) ? "Terlambat" : "Belum Selesai";
        }
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);
        statusItem->setTextAlignment(Qt::AlignCenter);
        taskList->setItem(row, 8, statusItem);

        // Warna baris
        QColor rowColor;
        bool late = statusText == "Terlambat";
        if (task.isDone) {
            rowColor = QColor("#9CA3AF");
        } else if (late || task.priority == "Tinggi") {
            rowColor = QColor("#EF4444");
        } else if (task.priority == "Sedang") {
            rowColor = QColor("#F59E0B");
        } else {
            rowColor = darkMode ? QColor("#93C5FD") : QColor("#1D4ED8");
        }

        for (int col = 0; col < 9; col++) {
            if (taskList->item(row, col)) {
                taskList->item(row, col)->setForeground(rowColor);
            }
        }
    }

    taskList->blockSignals(false);
}

void MainWindow::updateStats()
{
    int total = tasks.size();
    int done = 0;
    int late = 0;

    QDateTime now = QDateTime::currentDateTime();

    for (const TaskData &task : tasks) {
        if (task.isDone) {
            done++;
        }

        QDateTime deadline(task.reminderDate, task.reminderTime);

        if (!task.isDone && deadline < now) {
            late++;
        }
    }

    int progress = total == 0 ? 0 : (done * 100 / total);

    totalLabel->setText(QString::number(total));
    doneLabel->setText(QString::number(done));
    notDoneLabel->setText(QString::number(total - done));
    lateLabel->setText(QString::number(late));

    progressBar->setValue(progress);
    progressText->setText("Progress: " + QString::number(progress) + "%");
}

void MainWindow::clearCalendar()
{
    while (QLayoutItem *item = calendarGrid->takeAt(0)) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::addTaskToCalendarBox(QLayout *Layout, const QDate &date)
{
    for (const TaskData &task : tasks) {
        if (task.reminderDate == date) {
            QLabel *taskLabel = new QLabel(task.isDone ? "✓ " + task.title : "• " + task.title);
            taskLabel->setWordWrap(true);
            taskLabel->setMaximumWidth(90);

            if (darkMode) {
                if (task.isDone) {
                    taskLabel->setStyleSheet("background:#374151;color:#9CA3AF;border-radius:4px;padding:2px 4px;font-size:10px;");
                } else if (countdownText(task) == "Terlambat") {
                    taskLabel->setStyleSheet("background:#7F1D1D;color:#FCA5A5;border-radius:4px;padding:2px 4px;font-size:10px;font-weight:bold;");
                } else if (task.priority == "Tinggi") {
                    taskLabel->setStyleSheet("background:#7F1D1D;color:#FCA5A5;border-radius:4px;padding:2px 4px;font-size:10px;font-weight:bold;");
                } else if (task.priority == "Sedang") {
                    taskLabel->setStyleSheet("background:#78350F;color:#FCD34D;border-radius:4px;padding:2px 4px;font-size:10px;");
                } else {
                    taskLabel->setStyleSheet("background:#1E3A5F;color:#BFDBFE;border-radius:4px;padding:2px 4px;font-size:10px;");
                }
            } else {
                if (task.isDone) {
                    taskLabel->setStyleSheet("background:#E5E7EB;color:#6B7280;border-radius:4px;padding:2px 4px;font-size:10px;");
                } else if (countdownText(task) == "Terlambat") {
                    taskLabel->setStyleSheet("background:#FEE2E2;color:#991B1B;border-radius:4px;padding:2px 4px;font-size:10px;font-weight:bold;");
                } else if (task.priority == "Tinggi") {
                    taskLabel->setStyleSheet("background:#FEE2E2;color:#991B1B;border-radius:4px;padding:2px 4px;font-size:10px;font-weight:bold;");
                } else if (task.priority == "Sedang") {
                    taskLabel->setStyleSheet("background:#FEF3C7;color:#92400E;border-radius:4px;padding:2px 4px;font-size:10px;");
                } else {
                    taskLabel->setStyleSheet("background:#DBEAFE;color:#1E3A8A;border-radius:4px;padding:2px 4px;font-size:10px;");
                }
            }

            Layout->addWidget(taskLabel);
        }
    }
}

void MainWindow::buildCalendar()
{
    clearCalendar();

    monthLabel->setText(currentDate.toString("MMMM yyyy"));

    QStringList days = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};

    for (int i = 0; i < 7; i++) {
        QLabel *dayLabel = new QLabel(days[i]);
        dayLabel->setAlignment(Qt::AlignCenter);
        dayLabel->setObjectName("dayName");
        calendarGrid->addWidget(dayLabel, 0, i);
    }

    QDate firstDay(currentDate.year(), currentDate.month(), 1);
    int startColumn = firstDay.dayOfWeek() % 7;
    int totalDays = firstDay.daysInMonth();

    int day = 1;

    int totalRows = (startColumn + totalDays + 6) / 7;
    for (int row = 1; row <= totalRows; row++) {
        for (int col = 0; col < 7; col++) {
            QFrame *box = new QFrame;
            box->setObjectName("dateBox");
            box->setCursor(Qt::PointingHandCursor);
            box->setFixedHeight(90);

            QVBoxLayout *boxLayout = new QVBoxLayout(box);
            boxLayout->setContentsMargins(5, 5, 5, 5);
            boxLayout->setSpacing(2);

            QLabel *number = new QLabel("");

            if ((row == 1 && col >= startColumn) || row > 1) {
                if (day <= totalDays) {
                    QDate thisDate(currentDate.year(), currentDate.month(), day);

                    number->setText(QString::number(day));
                    number->setObjectName("dateNumber");
                    boxLayout->addWidget(number);

                    QScrollArea *scrollArea = new QScrollArea;
                    scrollArea->setWidgetResizable(true);
                    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                    scrollArea->setStyleSheet("background: transparent; border: none;");
                    scrollArea->setFixedHeight(54);
                    scrollArea->setFocusPolicy(Qt::NoFocus);

                    QWidget *scrollContent = new QWidget;
                    scrollContent->setStyleSheet("background: transparent;");
                    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
                    scrollLayout->setContentsMargins(0, 0, 0, 0);
                    scrollLayout->setSpacing(2);

                    scrollArea->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                    scrollArea->setFocusPolicy(Qt::NoFocus);

                    addTaskToCalendarBox(scrollLayout, thisDate);
                    scrollLayout->addStretch();

                    scrollArea->setWidget(scrollContent);
                    boxLayout->addWidget(scrollArea);

                    if (thisDate == QDate::currentDate()) {
                        if (darkMode) {
                            box->setStyleSheet("background:#1E2A4A;border:2px solid #2563FF;border-radius:8px;");
                        } else {
                            box->setStyleSheet("background:#E6E4FF;border:2px solid #2563FF;border-radius:8px;");
                        }
                    }
                    day++;
                } else {
                    delete box;
                    continue;
                }
            } else {
                boxLayout->addWidget(number);
                boxLayout->addStretch();
            }

            calendarGrid->addWidget(box, row, col);
        }
    }
}

void MainWindow::checkNotifications()
{
    QDateTime current = QDateTime::currentDateTime();
    QDate today = current.date();
    QTime now = current.time();

    for (const TaskData &task : tasks) {
        if (task.isDone) {
            continue;
        }

        // --- Notifikasi WAKTU MENGERJAKAN ---
        QDate workDateToCheck = task.workDate.isValid() ? task.workDate : task.reminderDate;
        if (workDateToCheck == today &&
            task.workTime.isValid() &&
            task.workTime.hour()   == now.hour() &&
            task.workTime.minute() == now.minute()) {

            QString key = task.title + current.toString("yyyyMMddHHmm") + "_work";

            if (!notifiedTasks.contains(key)) {
                notifiedTasks.insert(key);
                startAlarm(
                    "Waktunya mulai mengerjakan!\n\nTugas: " + task.title,
                    false
                    );
            }
        }

        // --- Notifikasi DEADLINE ---
        if (task.reminderDate == today &&
            task.reminderTime.isValid() &&
            task.reminderTime.hour()   == now.hour() &&
            task.reminderTime.minute() == now.minute()) {

            QString key = task.title + current.toString("yyyyMMddHHmm") + "_deadline";

            if (!notifiedTasks.contains(key)) {
                notifiedTasks.insert(key);
                startAlarm(
                    "DEADLINE SEKARANG! Segera selesaikan!\n\nTugas: " + task.title,
                    true
                    );
            }
        }
    }
}

void MainWindow::startAlarm(const QString &message, bool isDeadline)
{
    currentAlarmTask = message;

    QApplication::alert(this);

    if (alarmPlayer && !alarmSoundPath.isEmpty()) {
        alarmPlayer->stop();
        alarmPlayer->setSource(QUrl::fromLocalFile(alarmSoundPath));
        alarmPlayer->setPosition(0);
        alarmPlayer->play();
    } else {
        QApplication::beep();
    }

    QMessageBox *alarmBox = new QMessageBox(this);
    alarmBox->setWindowTitle(isDeadline ? "🚨 Deadline Tugas!" : "⏰ Alarm Tugas");
    alarmBox->setText(message);
    alarmBox->setIcon(isDeadline ? QMessageBox::Critical : QMessageBox::Warning);
    alarmBox->setStandardButtons(QMessageBox::Ok);
    alarmBox->setStyleSheet(
        "QMessageBox { background-color: #121212; }"
        "QLabel { color: #FFFFFF; font-size: 12px; font-weight: bold; }"
        "QPushButton { background-color: #2979FF; color: white; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 65px; }"
        "QPushButton:hover { background-color: #1565C0; }"
        );
    alarmBox->button(QMessageBox::Ok)->setText("Berhenti Alarm");

    connect(alarmBox, &QMessageBox::finished, this, [=]() {
        stopAlarm();
        alarmBox->deleteLater();
    });

    alarmBox->show();
}

void MainWindow::stopAlarm()
{
    if (alarmPlayer) {
        alarmPlayer->stop();
    }
}

void MainWindow::chooseAlarmSound()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Pilih Lagu Alarm",
        "",
        "Audio Files (*.mp3 *.wav)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    alarmSoundPath = fileName;

    if (alarmPlayer) {
        alarmPlayer->setSource(QUrl::fromLocalFile(alarmSoundPath));
    }

    if (alarmAudio) {
        alarmAudio->setVolume(0.9);
    }

    QSettings settings("MyCompany", "TodoListSaya");
    settings.setValue("alarmSoundPath", alarmSoundPath);

    QMessageBox::information(
        this,
        "Berhasil",
        "Lagu alarm berhasil dipilih.\n\nFile:\n" + alarmSoundPath
        );
}

void MainWindow::showTasksByDate(const QDate &date)
{
    QString message;

    for (const TaskData &task : tasks) {
        if (task.reminderDate == date) {
            message += "• " + task.title
                       + "\nKategori: " + task.category
                       + "\nPrioritas: " + task.priority
                       + "\nTgl Kerjakan: " + (task.workDate.isValid() ? task.workDate.toString("dd MMM yyyy") : task.reminderDate.toString("dd MMM yyyy"))
                       + "\nJam Kerjakan: " + task.workTime.toString("HH:mm")
                       + "\nTgl Deadline: " + task.reminderDate.toString("dd MMM yyyy")
                       + "\nJam Deadline: " + task.reminderTime.toString("HH:mm")
                       + "\nStatus: " + QString(task.isDone ? "Selesai" : "Belum selesai")
                       + "\n\n";
        }
    }

    if (message.isEmpty()) {
        message = "Tidak ada tugas pada tanggal ini.";
    }

    int choice = QMessageBox::question(
        this,
        "Tugas " + date.toString("dd MMMM yyyy"),
        message + "\nTambah tugas pada tanggal ini?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (choice == QMessageBox::Yes) {
        openTaskDialog(-1, date);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Blokir scroll pada combobox filter
    if ((obj == categoryBox || obj == priorityBox) && event->type() == QEvent::Wheel) {
        return true;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        QVariant dateValue = obj->property("date");

        if (dateValue.isValid() && dateValue.canConvert<QDate>()) {
            QDate date = dateValue.toDate();
            if (date.isValid()) {
                showTasksByDate(date);
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void clearLayout(QLayout *layout)
{
    if (!layout) return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (item->widget()) {
            delete item->widget();
        }
        if (item->layout()) {
            clearLayout(item->layout());
        }
        delete item;
    }
}

QDate MainWindow::weekStartDate() const
{
    int dow = currentDate.dayOfWeek();
    return currentDate.addDays(-(dow - 1));
}

void MainWindow::buildScheduleView()
{
    if (!scheduleFrame) return;

    QDate mon = weekStartDate();

    for (int d = 0; d < 7; d++) {
        QDate dayDate = mon.addDays(d);
        scheduleDateLabels[d]->setText(dayDate.toString("dd/MM"));

        clearLayout(scheduleDayCourseLayouts[d]);

        if (activeSemesterIndex < 0 || activeSemesterIndex >= semesters.size()) {
            continue;
        }

        const auto &courses = semesters[activeSemesterIndex].weeklySchedule[d];
        int shown = 0;

        for (const CourseEntry &ce : courses) {
            if (shown >= 3) {
                QLabel *moreLabel = new QLabel(QString("+%1 lagi").arg(courses.size() - 3));
                moreLabel->setObjectName("scheduleMoreLabel");
                moreLabel->setAlignment(Qt::AlignCenter);
                scheduleDayCourseLayouts[d]->addWidget(moreLabel);
                break;
            }

            QLabel *courseLabel = new QLabel(ce.name + " " + ce.startTime.toString("HH:mm") + "-" + ce.endTime.toString("HH:mm"));
            courseLabel->setObjectName("scheduleCourseLabel");
            courseLabel->setWordWrap(true);
            scheduleDayCourseLayouts[d]->addWidget(courseLabel);
            shown++;
        }

        if (shown == 0) {
            QLabel *emptyLabel = new QLabel("—");
            emptyLabel->setObjectName("scheduleEmptyLabel");
            emptyLabel->setAlignment(Qt::AlignCenter);
            scheduleDayCourseLayouts[d]->addWidget(emptyLabel);
        }
    }

    if (activeSemesterIndex >= 0 && activeSemesterIndex < semesters.size()) {
        semesterNameLabel->setText(semesters[activeSemesterIndex].name);
    } else {
        semesterNameLabel->setText("(tidak ada semester aktif)");
    }
}

void MainWindow::openScheduleManager()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Atur Jadwal Kuliah");
    dialog.resize(620, 520);
    dialog.setObjectName("scheduleDialog");

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setSpacing(12);

    QFrame *semGroup = new QFrame;
    semGroup->setObjectName("scheduleGroupBox");
    QVBoxLayout *semLayout = new QVBoxLayout(semGroup);
    semLayout->setContentsMargins(12, 10, 12, 10);
    semLayout->setSpacing(8);

    QLabel *semTitle = new QLabel("Semester");
    semTitle->setObjectName("scheduleGroupTitle");

    QHBoxLayout *semRow = new QHBoxLayout;
    QComboBox *semCombo = new QComboBox;
    semCombo->setMinimumWidth(200);

    for (int i = 0; i < semesters.size(); i++) {
        semCombo->addItem(semesters[i].name);
    }
    if (activeSemesterIndex >= 0 && activeSemesterIndex < semesters.size()) {
        semCombo->setCurrentIndex(activeSemesterIndex);
    }

    QPushButton *addSemBtn = new QPushButton("+ Tambah");
    QPushButton *renameSemBtn = new QPushButton("✎ Nama");
    QPushButton *deleteSemBtn = new QPushButton("✕ Hapus");
    QPushButton *activateSemBtn = new QPushButton("✓ Aktifkan");

    addSemBtn->setObjectName("smBtn");
    renameSemBtn->setObjectName("smBtn");
    deleteSemBtn->setObjectName("smBtn");
    activateSemBtn->setObjectName("smBtn");

    QLabel *activeSemLabel = new QLabel;
    if (activeSemesterIndex >= 0 && activeSemesterIndex < semesters.size()) {
        activeSemLabel->setText("✔ Aktif: " + semesters[activeSemesterIndex].name);
    } else {
        activeSemLabel->setText("— Belum ada semester aktif");
    }
    activeSemLabel->setObjectName("activeSemLabel");

    semRow->addWidget(semCombo);
    semRow->addWidget(addSemBtn);
    semRow->addWidget(renameSemBtn);
    semRow->addWidget(deleteSemBtn);
    semRow->addWidget(activateSemBtn);

    semLayout->addWidget(semTitle);
    semLayout->addLayout(semRow);
    semLayout->addWidget(activeSemLabel);

    QFrame *courseGroup = new QFrame;
    courseGroup->setObjectName("scheduleGroupBox");
    QVBoxLayout *courseLayout = new QVBoxLayout(courseGroup);
    courseLayout->setContentsMargins(12, 10, 12, 10);
    courseLayout->setSpacing(8);

    QLabel *courseTitle = new QLabel("Mata Kuliah");
    courseTitle->setObjectName("scheduleGroupTitle");

    QHBoxLayout *daySelectRow = new QHBoxLayout;
    QComboBox *dayCombo = new QComboBox;
    dayCombo->addItems({"Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"});
    dayCombo->setMinimumWidth(120);
    QLabel *dayLabel = new QLabel("Pilih hari:");
    daySelectRow->addWidget(dayLabel);
    daySelectRow->addWidget(dayCombo);
    daySelectRow->addStretch();

    QTableWidget *courseList = new QTableWidget;
    courseList->setObjectName("courseList");
    courseList->setMinimumHeight(160);

    QHBoxLayout *courseBtnRow = new QHBoxLayout;
    QPushButton *addCourseBtn = new QPushButton("+ Tambah Matkul");
    QPushButton *editCourseBtn = new QPushButton("✎ Edit");
    QPushButton *deleteCourseBtn = new QPushButton("✕ Hapus");
    addCourseBtn->setObjectName("smBtn");
    editCourseBtn->setObjectName("smBtn");
    deleteCourseBtn->setObjectName("smBtn");
    courseBtnRow->addWidget(addCourseBtn);
    courseBtnRow->addWidget(editCourseBtn);
    courseBtnRow->addWidget(deleteCourseBtn);
    courseBtnRow->addStretch();

    courseLayout->addWidget(courseTitle);
    courseLayout->addLayout(daySelectRow);
    courseLayout->addWidget(courseList);
    courseLayout->addLayout(courseBtnRow);

    mainLayout->addWidget(semGroup);
    mainLayout->addWidget(courseGroup);
    mainLayout->addStretch();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    mainLayout->addWidget(buttonBox);

    auto refreshCourseList = [&]() {
        courseList->clearContents();
        courseList->setRowCount(0);
        courseList->setColumnCount(3);
        courseList->setHorizontalHeaderLabels({"Mata Kuliah", "Mulai", "Selesai"});
        courseList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        courseList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        courseList->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        courseList->verticalHeader()->setVisible(false);
        courseList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        courseList->setSelectionBehavior(QAbstractItemView::SelectRows);

        int semIdx = semCombo->currentIndex();
        int dayIdx = dayCombo->currentIndex();
        if (semIdx >= 0 && semIdx < semesters.size() && dayIdx >= 0 && dayIdx < 7) {
            const auto &courses = semesters[semIdx].weeklySchedule[dayIdx];
            for (int i = 0; i < courses.size(); i++) {
                const auto &c = courses[i];
                int row = courseList->rowCount();
                courseList->insertRow(row);

                QTableWidgetItem *nameItem = new QTableWidgetItem(c.name);
                QTableWidgetItem *startItem = new QTableWidgetItem(c.startTime.toString("HH:mm"));
                QTableWidgetItem *endItem = new QTableWidgetItem(c.endTime.toString("HH:mm"));

                startItem->setTextAlignment(Qt::AlignCenter);
                endItem->setTextAlignment(Qt::AlignCenter);

                courseList->setItem(row, 0, nameItem);
                courseList->setItem(row, 1, startItem);
                courseList->setItem(row, 2, endItem);
            }
        }
        if (semCombo->currentIndex() == activeSemesterIndex) {
            activeSemLabel->setText("✔ Aktif: " + semesters[activeSemesterIndex].name);
        }
    };

    auto scheduleChanged = [&]() {
        saveSchedule();
        buildScheduleView();
    };

    connect(dayCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int) {
        refreshCourseList();
    });

    connect(semCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int) {
        refreshCourseList();
    });

    connect(addSemBtn, &QPushButton::clicked, [&]() {
        bool ok;
        QString name = QInputDialog::getText(&dialog, "Tambah Semester", "Nama semester:", QLineEdit::Normal, "", &ok);
        if (ok && !name.trimmed().isEmpty()) {
            SemesterData sem;
            sem.name = name.trimmed();
            sem.weeklySchedule.resize(7);
            semesters.append(sem);
            semCombo->addItem(sem.name);
            semCombo->setCurrentIndex(semesters.size() - 1);
            scheduleChanged();
        }
    });

    connect(renameSemBtn, &QPushButton::clicked, [&]() {
        int idx = semCombo->currentIndex();
        if (idx < 0 || idx >= semesters.size()) return;
        bool ok;
        QString name = QInputDialog::getText(&dialog, "Ubah Nama Semester", "Nama baru:", QLineEdit::Normal, semesters[idx].name, &ok);
        if (ok && !name.trimmed().isEmpty()) {
            semesters[idx].name = name.trimmed();
            semCombo->setItemText(idx, name);
            scheduleChanged();
        }
    });

    connect(deleteSemBtn, &QPushButton::clicked, [&]() {
        int idx = semCombo->currentIndex();
        if (idx < 0 || idx >= semesters.size()) return;
        if (semesters.size() <= 1) {
            QMessageBox::warning(&dialog, "Peringatan", "Tidak bisa menghapus semester terakhir.");
            return;
        }
        int ret = QMessageBox::question(&dialog, "Konfirmasi", "Hapus semester \"" + semesters[idx].name + "\"?");
        if (ret != QMessageBox::Yes) return;

        semesters.removeAt(idx);
        semCombo->removeItem(idx);
        if (idx == activeSemesterIndex) {
            activeSemesterIndex = 0;
        } else if (idx < activeSemesterIndex) {
            activeSemesterIndex--;
        }
        if (!semesters.isEmpty()) {
            semCombo->setCurrentIndex(qMin(activeSemesterIndex, semesters.size() - 1));
        }
        scheduleChanged();
    });

    connect(activateSemBtn, &QPushButton::clicked, [&]() {
        int idx = semCombo->currentIndex();
        if (idx < 0 || idx >= semesters.size()) return;
        activeSemesterIndex = idx;
        activeSemLabel->setText("✔ Aktif: " + semesters[idx].name);
        scheduleChanged();
    });

    connect(addCourseBtn, &QPushButton::clicked, [&]() {
        int semIdx = semCombo->currentIndex();
        int dayIdx = dayCombo->currentIndex();
        if (semIdx < 0 || semIdx >= semesters.size()) return;

        QDialog inputDialog(&dialog);
        inputDialog.setWindowTitle("Tambah Mata Kuliah");
        inputDialog.resize(320, 200);

        QVBoxLayout *inputLayout = new QVBoxLayout(&inputDialog);
        QFormLayout *form = new QFormLayout;

        QLineEdit *nameInput = new QLineEdit;
        nameInput->setPlaceholderText("Nama mata kuliah");
        QTimeEdit *startInput = new QTimeEdit(QTime(7, 0));
        startInput->setDisplayFormat("HH:mm");
        startInput->setWrapping(true);
        QTimeEdit *endInput = new QTimeEdit(QTime(9, 0));
        endInput->setDisplayFormat("HH:mm");
        endInput->setWrapping(true);

        form->addRow("Matkul:", nameInput);
        form->addRow("Mulai:", startInput);
        form->addRow("Selesai:", endInput);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(box, &QDialogButtonBox::accepted, &inputDialog, &QDialog::accept);
        connect(box, &QDialogButtonBox::rejected, &inputDialog, &QDialog::reject);

        inputLayout->addLayout(form);
        inputLayout->addWidget(box);

        if (inputDialog.exec() == QDialog::Accepted) {
            QString cname = nameInput->text().trimmed();
            if (cname.isEmpty()) {
                QMessageBox::warning(&dialog, "Peringatan", "Nama mata kuliah tidak boleh kosong.");
                return;
            }
            CourseEntry ce;
            ce.name = cname;
            ce.startTime = startInput->time();
            ce.endTime = endInput->time();
            semesters[semIdx].weeklySchedule[dayIdx].append(ce);
            refreshCourseList();
            scheduleChanged();
        }
    });

    connect(editCourseBtn, &QPushButton::clicked, [&]() {
        int semIdx = semCombo->currentIndex();
        int dayIdx = dayCombo->currentIndex();
        int courseRow = courseList->currentRow();
        if (semIdx < 0 || semIdx >= semesters.size()) return;
        if (courseRow < 0 || courseRow >= semesters[semIdx].weeklySchedule[dayIdx].size()) {
            QMessageBox::warning(&dialog, "Peringatan", "Pilih mata kuliah yang ingin diedit.");
            return;
        }

        CourseEntry &ce = semesters[semIdx].weeklySchedule[dayIdx][courseRow];

        QDialog inputDialog(&dialog);
        inputDialog.setWindowTitle("Edit Mata Kuliah");
        inputDialog.resize(320, 200);

        QVBoxLayout *inputLayout = new QVBoxLayout(&inputDialog);
        QFormLayout *form = new QFormLayout;

        QLineEdit *nameInput = new QLineEdit(ce.name);
        QTimeEdit *startInput = new QTimeEdit(ce.startTime);
        startInput->setDisplayFormat("HH:mm");
        startInput->setWrapping(true);
        QTimeEdit *endInput = new QTimeEdit(ce.endTime);
        endInput->setDisplayFormat("HH:mm");
        endInput->setWrapping(true);

        form->addRow("Matkul:", nameInput);
        form->addRow("Mulai:", startInput);
        form->addRow("Selesai:", endInput);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(box, &QDialogButtonBox::accepted, &inputDialog, &QDialog::accept);
        connect(box, &QDialogButtonBox::rejected, &inputDialog, &QDialog::reject);

        inputLayout->addLayout(form);
        inputLayout->addWidget(box);

        if (inputDialog.exec() == QDialog::Accepted) {
            QString cname = nameInput->text().trimmed();
            if (cname.isEmpty()) {
                QMessageBox::warning(&dialog, "Peringatan", "Nama mata kuliah tidak boleh kosong.");
                return;
            }
            ce.name = cname;
            ce.startTime = startInput->time();
            ce.endTime = endInput->time();
            refreshCourseList();
            scheduleChanged();
        }
    });

    connect(deleteCourseBtn, &QPushButton::clicked, [&]() {
        int semIdx = semCombo->currentIndex();
        int dayIdx = dayCombo->currentIndex();
        int courseRow = courseList->currentRow();
        if (semIdx < 0 || semIdx >= semesters.size()) return;
        if (courseRow < 0 || courseRow >= semesters[semIdx].weeklySchedule[dayIdx].size()) {
            QMessageBox::warning(&dialog, "Peringatan", "Pilih mata kuliah yang ingin dihapus.");
            return;
        }
        semesters[semIdx].weeklySchedule[dayIdx].removeAt(courseRow);
        refreshCourseList();
        scheduleChanged();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    refreshCourseList();
    dialog.exec();

    saveSchedule();
    buildScheduleView();
}

void MainWindow::saveSchedule()
{
    QSettings settings("MyCompany", "TodoListSaya");

    settings.remove("semesters");
    settings.beginWriteArray("semesters");

    for (int s = 0; s < semesters.size(); s++) {
        settings.setArrayIndex(s);
        settings.setValue("name", semesters[s].name);

        settings.beginWriteArray("days");
        for (int d = 0; d < 7; d++) {
            settings.setArrayIndex(d);
            const auto &courses = semesters[s].weeklySchedule[d];

            settings.beginWriteArray("courses");
            for (int c = 0; c < courses.size(); c++) {
                settings.setArrayIndex(c);
                settings.setValue("name", courses[c].name);
                settings.setValue("start", courses[c].startTime);
                settings.setValue("end", courses[c].endTime);
            }
            settings.endArray();
        }
        settings.endArray();
    }

    settings.endArray();
    settings.setValue("activeSemester", activeSemesterIndex);
}

void MainWindow::loadSchedule()
{
    QSettings settings("MyCompany", "TodoListSaya");

    semesters.clear();

    int semCount = settings.beginReadArray("semesters");

    for (int s = 0; s < semCount; s++) {
        settings.setArrayIndex(s);
        SemesterData sem;
        sem.name = settings.value("name").toString();
        sem.weeklySchedule.resize(7);

        int dayCount = settings.beginReadArray("days");
        for (int d = 0; d < dayCount && d < 7; d++) {
            settings.setArrayIndex(d);

            int courseCount = settings.beginReadArray("courses");
            for (int c = 0; c < courseCount; c++) {
                settings.setArrayIndex(c);
                CourseEntry ce;
                ce.name = settings.value("name").toString();
                ce.startTime = settings.value("start").toTime();
                ce.endTime = settings.value("end").toTime();
                if (!ce.name.isEmpty()) {
                    sem.weeklySchedule[d].append(ce);
                }
            }
            settings.endArray();
        }
        settings.endArray();

        if (!sem.name.isEmpty()) {
            semesters.append(sem);
        }
    }

    settings.endArray();
    activeSemesterIndex = settings.value("activeSemester", -1).toInt();

    if (activeSemesterIndex < 0 || activeSemesterIndex >= semesters.size()) {
        activeSemesterIndex = semesters.isEmpty() ? -1 : 0;
    }
}

void MainWindow::saveTasks()
{
    QSettings settings("MyCompany", "TodoListSaya");

    settings.remove("tasks");
    settings.beginWriteArray("tasks");

    for (int i = 0; i < tasks.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("title", tasks[i].title);
        settings.setValue("category", tasks[i].category);
        settings.setValue("priority", tasks[i].priority);
        settings.setValue("workDate", tasks[i].workDate);
        settings.setValue("date", tasks[i].reminderDate);
        settings.setValue("workTime", tasks[i].workTime);
        settings.setValue("time", tasks[i].reminderTime);
        settings.setValue("done", tasks[i].isDone);
    }

    settings.endArray();
}

void MainWindow::loadTasks()
{
    tasks.clear();

    QSettings settings("MyCompany", "TodoListSaya");

    int size = settings.beginReadArray("tasks");

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        TaskData task;
        task.title = settings.value("title").toString();
        task.category = settings.value("category").toString();
        task.priority = settings.value("priority").toString();
        task.reminderDate = settings.value("date").toDate();
        task.workDate = settings.value("workDate").toDate();
        // Fallback untuk data lama yang belum punya workDate
        if (!task.workDate.isValid()) {
            task.workDate = task.reminderDate;
        }
        task.workTime = settings.value("workTime").toTime();
        task.reminderTime = settings.value("time").toTime();
        task.isDone = settings.value("done").toBool();

        if (!task.title.isEmpty()) {
            tasks.append(task);
        }
    }

    settings.endArray();
}

void MainWindow::applyTheme()
{
    if (!darkMode) {
        setStyleSheet(R"(
            QWidget#root {
                background-color:#F4F7FF;
                font-family:Arial;
            }

            QLabel#title {
                font-size:30px;
                font-weight:800;
                color:#14243D;
            }

            QLabel#subtitle {
                font-size:16px;
                color:#334155;
            }

            QLabel#smallTitle {
                font-size:15px;
                font-weight:bold;
                color:#14243D;
            }

            QFrame#card,
            QFrame#panel,
            QFrame#calendarPanel {
                background:white;
                border-radius:14px;
                border:1px solid #E5E7EB;
            }

            QLabel#cardNumber {
                font-size:24px;
                font-weight:bold;
            }

            QLabel#cardText {
                font-size:14px;
                color:#334155;
            }

            QPushButton {
                border:none;
                border-radius:9px;
                padding:9px 12px;
                color:white;
                font-size:14px;
                font-weight:bold;
            }

            QPushButton#addBtn { background:#2563FF; }
            QPushButton#editBtn { background:#F59E0B; }
            QPushButton#listBtn { background:#EF4444; }
            QPushButton#doneBtn { background:#00A650; }
            QPushButton#notifBtn { background:#9817F4; }
            QPushButton#soundBtn { background:#0EA5E9; }
            QPushButton#themeBtn { background:#111827; }

            QPushButton#navBtn {
                background:transparent;
                color:black;
                font-size:28px;
                padding:2px 10px;
            }

            QPushButton#todayBtn {
                background:transparent;
                color:black;
                font-size:15px;
                font-weight:bold;
            }

            QComboBox, QLineEdit {
                background:white;
                color:black;
                border:1px solid #DDDDDD;
                border-radius:10px;
                padding:8px 14px;
                font-size:14px;
            }

            QComboBox QAbstractItemView {
                background:white;
                color:black;
                selection-background-color:#DBEAFE;
                selection-color:black;
                border:1px solid #DDDDDD;
                outline:none;
            }

            QTableWidget#taskList {
                background:white;
                color:black;
                border:1px solid #E5E7EB;
                border-radius:10px;
                font-size:11px;
                gridline-color:#E5E7EB;
            }
            QTableWidget#taskList::item { padding:6px; }
            QTableWidget#taskList::item:selected {
                background:#DBEAFE;
                color:black;
            }
            QHeaderView::section {
                background:#F1F5F9;
                color:black;
                font-weight:bold;
                font-size:11px;
                padding:4px;
                border:1px solid #E5E7EB;
            }
            QScrollBar:vertical {
                background:#F1F5F9;
                width:8px;
                border-radius:4px;
            }
            QScrollBar::handle:vertical {
                background:#CBD5E1;
                border-radius:4px;
            }

            QLabel#monthTitle {
                font-size:24px;
                font-weight:bold;
                color:black;
            }

            QLabel#dayName {
                font-size:14px;
                font-weight:bold;
                color:black;
            }

            QFrame#dateBox {
                background:white;
                border:1px solid #E0E0E0;
                border-radius:8px;
                min-height:88px;
            }

            QFrame#dateBox:hover {
                background:#F1F5FF;
                border:1px solid #2563FF;
            }

            QLabel#dateNumber {
                font-size:14px;
                font-weight:bold;
                color:black;
            }

            QProgressBar {
                border:1px solid #CBD5E1;
                border-radius:8px;
                height:18px;
                background:white;
                text-align:center;
            }

            QProgressBar::chunk {
                background:#2563FF;
                border-radius:8px;
            }

            QFrame#scheduleFrame {
                background:#F8FAFF;
                border:1px solid #E5E7EB;
                border-radius:10px;
                padding:6px;
            }

            QLabel#scheduleTitle { font-size:13px; font-weight:bold; color:#14243D; }
            QLabel#semesterNameLabel { font-size:11px; color:#2563EB; font-weight:bold; }

            QPushButton#manageScheduleBtn {
                background:#2563FF; color:white; border:none;
                border-radius:5px; padding:4px 10px;
                font-size:11px; font-weight:bold;
            }
            QPushButton#manageScheduleBtn:hover { background:#1D4ED8; }

            QFrame#scheduleDayFrame {
                background:white;
                border:1px solid #E5E7EB;
                border-radius:6px;
            }

            QLabel#scheduleDayName { font-size:10px; font-weight:bold; color:#334155; }
            QLabel#scheduleDateLabel { font-size:10px; color:#6B7280; }
            QLabel#scheduleCourseLabel {
                font-size:9px; color:#1E3A8A; background:#DBEAFE;
                border-radius:3px; padding:1px 3px;
            }
            QLabel#scheduleMoreLabel { font-size:9px; color:#6B7280; font-style:italic; }
            QLabel#scheduleEmptyLabel { font-size:9px; color:#D1D5DB; }

            QFrame#scheduleGroupBox {
                background:#F8FAFF;
                border:1px solid #E5E7EB;
                border-radius:10px;
            }

            QLabel#scheduleGroupTitle { font-size:14px; font-weight:bold; color:#14243D; }

            QPushButton#smBtn {
                background:#2563FF; color:white; border:none;
                border-radius:5px; padding:5px 10px;
                font-size:11px; font-weight:bold;
            }
            QPushButton#smBtn:hover { background:#1D4ED8; }

            QLabel#activeSemLabel { font-size:12px; color:#00A650; font-weight:bold; }

            QTableWidget#courseList {
                background:white; color:black;
                border:1px solid #E5E7EB; border-radius:8px; font-size:13px;
            }
            QTableWidget#courseList::item { padding:6px; border-radius:4px; }
            QTableWidget#courseList::item:selected { background:#DBEAFE; color:black; }

            QScrollArea { background:transparent; border:none; }
        )");
    } else {
        setStyleSheet(R"(
            QWidget#root {
                background-color:#0F172A;
                font-family:Arial;
            }

            QLabel#title { font-size:30px; font-weight:800; color:white; }
            QLabel#subtitle { font-size:16px; color:#CBD5E1; }
            QLabel#smallTitle { font-size:15px; font-weight:bold; color:white; }

            QFrame#card, QFrame#panel, QFrame#calendarPanel {
                background:#1E293B;
                border-radius:14px;
                border:1px solid #334155;
            }

            QLabel#cardNumber { font-size:24px; font-weight:bold; }
            QLabel#cardText { font-size:14px; color:#CBD5E1; }

            QPushButton {
                border:none; border-radius:9px;
                padding:9px 12px; color:white;
                font-size:14px; font-weight:bold;
            }

            QPushButton#addBtn { background:#2563FF; }
            QPushButton#editBtn { background:#F59E0B; }
            QPushButton#listBtn { background:#EF4444; }
            QPushButton#doneBtn { background:#00A650; }
            QPushButton#notifBtn { background:#9817F4; }
            QPushButton#soundBtn { background:#0EA5E9; }
            QPushButton#themeBtn { background:#475569; }

            QPushButton#navBtn, QPushButton#todayBtn {
                background:transparent; color:white;
                font-size:18px; font-weight:bold;
            }

            QComboBox, QLineEdit {
                background:#0F172A; color:white;
                border:1px solid #475569; border-radius:10px;
                padding:8px 14px; font-size:14px;
            }

            QComboBox QAbstractItemView {
                background:#0F172A; color:white;
                selection-background-color:#334155;
                selection-color:white;
                border:1px solid #475569; outline:none;
            }

            QTableWidget#taskList {
                background:#2D3748; color:white;
                border:1px solid #475569; border-radius:10px;
                font-size:11px; gridline-color:#475569;
            }
            QTableWidget#taskList::item { padding:6px; color:white; }
            QTableWidget#taskList::item:selected { background:#334155; color:white; }

            QHeaderView::section {
                background:#263548; color:white;
                font-weight:bold; font-size:11px;
                padding:4px; border:1px solid #475569;
            }
            QScrollBar:vertical {
                background:#1E293B; width:8px; border-radius:4px;
            }
            QScrollBar::handle:vertical {
                background:#475569; border-radius:4px;
            }

            QLabel#monthTitle, QLabel#dayName, QLabel#dateNumber {
                color:white; font-weight:bold;
            }

            QFrame#dateBox {
                background:#1E293B; border:1px solid #475569;
                border-radius:8px; min-height:88px;
            }
            QFrame#dateBox:hover {
                background:#243044; border:1px solid #60A5FA;
            }

            QProgressBar {
                border:1px solid #475569; border-radius:8px;
                height:18px; background:#0F172A; color:white; text-align:center;
            }
            QProgressBar::chunk { background:#22C55E; border-radius:8px; }

            QFrame#scheduleFrame {
                background:#1E293B; border:1px solid #334155;
                border-radius:10px; padding:6px;
            }

            QLabel#scheduleTitle { font-size:13px; font-weight:bold; color:white; }
            QLabel#semesterNameLabel { font-size:11px; color:#60A5FA; font-weight:bold; }

            QPushButton#manageScheduleBtn {
                background:#2563FF; color:white; border:none;
                border-radius:5px; padding:4px 10px;
                font-size:11px; font-weight:bold;
            }
            QPushButton#manageScheduleBtn:hover { background:#1D4ED8; }

            QFrame#scheduleDayFrame {
                background:#0F172A; border:1px solid #475569; border-radius:6px;
            }

            QLabel#scheduleDayName { font-size:10px; font-weight:bold; color:#CBD5E1; }
            QLabel#scheduleDateLabel { font-size:10px; color:#94A3B8; }
            QLabel#scheduleCourseLabel {
                font-size:9px; color:#BFDBFE; background:#1E3A5F;
                border-radius:3px; padding:1px 3px;
            }
            QLabel#scheduleMoreLabel { font-size:9px; color:#94A3B8; font-style:italic; }
            QLabel#scheduleEmptyLabel { font-size:9px; color:#475569; }

            QFrame#scheduleGroupBox {
                background:#1E293B; border:1px solid #334155; border-radius:10px;
            }

            QLabel#scheduleGroupTitle { font-size:14px; font-weight:bold; color:white; }

            QPushButton#smBtn {
                background:#2563FF; color:white; border:none;
                border-radius:5px; padding:5px 10px;
                font-size:11px; font-weight:bold;
            }
            QPushButton#smBtn:hover { background:#1D4ED8; }

            QLabel#activeSemLabel { font-size:12px; color:#22C55E; font-weight:bold; }

            QTableWidget#courseList {
                background:#2D3748; color:white;
                border:1px solid #475569; border-radius:8px; font-size:13px;
            }
            QTableWidget#courseList::item { padding:6px; border-radius:4px; color:white; }
            QTableWidget#courseList::item:selected { background:#334155; color:white; }

            QScrollArea { background:transparent; border:none; }
        )");
    }
}