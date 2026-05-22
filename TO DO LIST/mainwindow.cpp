#include "mainwindow.h"
#include <QIcon>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDate(QDate::currentDate())
{
    setWindowTitle("To-Do List Saya");
    setWindowIcon(QIcon("logo.ico"));
    resize(1450, 850);

    QWidget *root = new QWidget(this);
    root->setObjectName("root");

    QVBoxLayout *mainLayout = new QVBoxLayout(root);
    mainLayout->setContentsMargins(18, 10, 18, 10);
    mainLayout->setSpacing(8);

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
    calendarLayout->setContentsMargins(20, 12, 20, 12);
    calendarLayout->setSpacing(8);

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
    calendarGrid->setSpacing(6);

    calendarLayout->addLayout(calendarHeader);
    calendarLayout->addLayout(calendarGrid);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(calendarPanel);
    scrollArea->setFrameShape(QFrame::NoFrame);

    contentLayout->addWidget(scrollArea, 3);

    QFrame *controlPanel = new QFrame;
    controlPanel->setObjectName("panel");

    QVBoxLayout *panelLayout = new QVBoxLayout(controlPanel);
    panelLayout->setContentsMargins(20, 14, 20, 14);
    panelLayout->setSpacing(10);

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

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(doneButton);
    buttonLayout->addWidget(notifButton);
    buttonLayout->addWidget(soundButton);
    buttonLayout->addWidget(themeButton);

    searchBox = new QLineEdit;
    searchBox->setPlaceholderText("Cari tugas...");

    categoryBox = new QComboBox;
    categoryBox->addItems({"Semua Kategori", "Kuliah", "Kerja", "Pribadi", "Umum"});

    priorityBox = new QComboBox;
    priorityBox->addItems({"Semua Prioritas", "Tinggi", "Sedang", "Rendah", "Normal"});

    taskList = new QListWidget;
    taskList->setObjectName("taskList");
    taskList->setSelectionMode(QAbstractItemView::SingleSelection);
    taskList->setMinimumHeight(260);
    taskList->setMaximumHeight(420);

    QLabel *taskTitle = new QLabel("Daftar Tugas");
    taskTitle->setObjectName("smallTitle");

    panelLayout->addLayout(buttonLayout);
    panelLayout->addWidget(searchBox);
    panelLayout->addWidget(categoryBox);
    panelLayout->addWidget(priorityBox);
    panelLayout->addWidget(taskTitle);
    panelLayout->addWidget(taskList);
    panelLayout->addStretch();

    contentLayout->addWidget(controlPanel, 2);

    mainLayout->addLayout(contentLayout, 1);

    setCentralWidget(root);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteTask);
    connect(doneButton, &QPushButton::clicked, this, &MainWindow::markTaskDone);
    connect(notifButton, &QPushButton::clicked, this, &MainWindow::checkNotifications);
    connect(soundButton, &QPushButton::clicked, this, &MainWindow::chooseAlarmSound);

    connect(themeButton, &QPushButton::clicked, this, [=]() {
        darkMode = !darkMode;
        applyTheme();

        if (darkMode) {
            themeButton->setText("☀ Light");
        } else {
            themeButton->setText("🌙 Dark");
        }
    });

    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::refreshTaskList);
    connect(categoryBox, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);
    connect(priorityBox, &QComboBox::currentTextChanged, this, &MainWindow::refreshTaskList);

    connect(prevButton, &QPushButton::clicked, this, [=]() {
        currentDate = currentDate.addMonths(-1);
        buildCalendar();
    });

    connect(nextButton, &QPushButton::clicked, this, [=]() {
        currentDate = currentDate.addMonths(1);
        buildCalendar();
    });

    connect(todayButton, &QPushButton::clicked, this, [=]() {
        currentDate = QDate::currentDate();
        buildCalendar();
    });

    notificationTimer = new QTimer(this);

    connect(notificationTimer, &QTimer::timeout, this, [=]() {
        checkNotifications();
        updateStats();
        refreshTaskList();
        buildCalendar();
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

    searchBox->clear();
    categoryBox->setCurrentText("Semua Kategori");
    priorityBox->setCurrentText("Semua Prioritas");

    refreshTaskList();
    buildCalendar();
    updateStats();
    applyTheme();
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

    if (days > 0) {
        return QString::number(days) + " hari lagi";
    }

    if (hours > 0) {
        return QString::number(hours) + " jam lagi";
    }

    return QString::number(minutes) + " menit lagi";
}

QString MainWindow::taskText(const TaskData &task) const
{
    QString status = task.isDone ? "✓ Selesai" : "⏳ Belum selesai";

    return task.title + " | " +
           task.category + " | " +
           task.priority + " | " +
           task.reminderDate.toString("dd MMM yyyy") + " " +
           task.reminderTime.toString("HH:mm") + " | " +
           status + " | " +
           countdownText(task);
}

void MainWindow::addTask()
{
    openTaskDialog(-1);
}

void MainWindow::editTask()
{
    QListWidgetItem *item = taskList->currentItem();

    if (!item) {
        QMessageBox::warning(this, "Peringatan", "Pilih tugas yang ingin diedit.");
        return;
    }

    int index = item->data(Qt::UserRole).toInt();

    if (index < 0 || index >= tasks.size()) {
        QMessageBox::warning(this, "Peringatan", "Data tugas tidak valid.");
        return;
    }

    openTaskDialog(index);
}

void MainWindow::openTaskDialog(int editIndex, QDate selectedDate)
{
    QDialog dialog(this);
    dialog.setWindowTitle(editIndex >= 0 ? "Edit Tugas" : "Tambah Tugas");
    dialog.resize(380, 260);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *taskInput = new QLineEdit;
    taskInput->setPlaceholderText("Masukkan nama tugas");

    QComboBox *categoryInput = new QComboBox;
    categoryInput->addItems({"Kuliah", "Kerja", "Pribadi", "Umum"});

    QComboBox *priorityInput = new QComboBox;
    priorityInput->addItems({"Tinggi", "Sedang", "Rendah", "Normal"});

    QDateEdit *dateInput = new QDateEdit;
    dateInput->setCalendarPopup(true);
    dateInput->setDisplayFormat("dd MMMM yyyy");

    QTimeEdit *timeInput = new QTimeEdit;
    timeInput->setDisplayFormat("HH:mm");

    if (editIndex >= 0 && editIndex < tasks.size()) {
        TaskData task = tasks[editIndex];
        taskInput->setText(task.title);
        categoryInput->setCurrentText(task.category);
        priorityInput->setCurrentText(task.priority);
        dateInput->setDate(task.reminderDate);
        timeInput->setTime(task.reminderTime);
    } else {
        dateInput->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
        timeInput->setTime(QTime::currentTime());
    }

    QFormLayout *form = new QFormLayout;
    form->addRow("Nama Tugas:", taskInput);
    form->addRow("Kategori:", categoryInput);
    form->addRow("Prioritas:", priorityInput);
    form->addRow("Tanggal:", dateInput);
    form->addRow("Jam:", timeInput);

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
            QMessageBox::warning(this, "Peringatan", "Nama tugas tidak boleh kosong.");
            return;
        }

        TaskData task;
        task.title = title;
        task.category = categoryInput->currentText();
        task.priority = priorityInput->currentText();
        task.reminderDate = dateInput->date();
        task.reminderTime = timeInput->time();

        if (editIndex >= 0 && editIndex < tasks.size()) {
            task.isDone = tasks[editIndex].isDone;
            tasks[editIndex] = task;
        } else {
            task.isDone = false;
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
    int row = taskList->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Peringatan", "Klik/pilih dulu tugas yang ingin dihapus.");
        return;
    }

    QListWidgetItem *item = taskList->item(row);

    if (!item) {
        QMessageBox::warning(this, "Error", "Item tugas tidak ditemukan.");
        return;
    }

    int index = item->data(Qt::UserRole).toInt();

    if (index < 0 || index >= tasks.size()) {
        QMessageBox::warning(this, "Peringatan", "Data tugas tidak valid.");
        return;
    }

    int confirm = QMessageBox::question(
        this,
        "Konfirmasi Hapus",
        "Yakin ingin menghapus tugas ini?\n\n" + tasks[index].title,
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirm != QMessageBox::Yes) {
        return;
    }

    tasks.removeAt(index);

    saveTasks();

    searchBox->clear();
    categoryBox->setCurrentText("Semua Kategori");
    priorityBox->setCurrentText("Semua Prioritas");

    refreshTaskList();
    buildCalendar();
    updateStats();

    QMessageBox::information(this, "Berhasil", "Tugas berhasil dihapus.");
}

void MainWindow::markTaskDone()
{
    QListWidgetItem *item = taskList->currentItem();

    if (!item) {
        QMessageBox::warning(this, "Peringatan", "Pilih tugas dulu.");
        return;
    }

    int index = item->data(Qt::UserRole).toInt();

    if (index < 0 || index >= tasks.size()) {
        QMessageBox::warning(this, "Peringatan", "Data tugas tidak valid.");
        return;
    }

    tasks[index].isDone = !tasks[index].isDone;

    saveTasks();
    refreshTaskList();
    buildCalendar();
    updateStats();
}

void MainWindow::refreshTaskList()
{
    if (!taskList || !searchBox || !categoryBox || !priorityBox) {
        return;
    }

    int selectedIndex = -1;

    if (taskList->currentItem()) {
        selectedIndex = taskList->currentItem()->data(Qt::UserRole).toInt();
    }

    taskList->clear();

    QString search = searchBox->text().trimmed().toLower();
    QString selectedCategory = categoryBox->currentText();
    QString selectedPriority = priorityBox->currentText();

    for (int i = 0; i < tasks.size(); i++) {
        const TaskData &task = tasks[i];

        bool searchOk = search.isEmpty() || task.title.toLower().contains(search);
        bool categoryOk = selectedCategory == "Semua Kategori" || task.category == selectedCategory;
        bool priorityOk = selectedPriority == "Semua Prioritas" || task.priority == selectedPriority;

        if (searchOk && categoryOk && priorityOk) {
            QListWidgetItem *item = new QListWidgetItem(taskText(task));
            item->setData(Qt::UserRole, i);

            if (task.isDone) {
                item->setForeground(Qt::gray);
            } else if (countdownText(task) == "Terlambat") {
                item->setForeground(QBrush(Qt::red));
            } else if (task.priority == "Tinggi") {
                item->setForeground(QBrush(Qt::red));
            } else if (task.priority == "Sedang") {
                item->setForeground(QBrush(Qt::darkYellow));
            } else {
                item->setForeground(QBrush(Qt::darkBlue));
            }

            taskList->addItem(item);

            if (i == selectedIndex) {
                taskList->setCurrentItem(item);
            }
        }
    }
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

void MainWindow::addTaskToCalendarBox(QVBoxLayout *boxLayout, const QDate &date)
{
    int shown = 0;

    for (const TaskData &task : tasks) {
        if (task.reminderDate == date) {
            QLabel *taskLabel = new QLabel(task.isDone ? "✓ " + task.title : "• " + task.title);
            taskLabel->setWordWrap(true);

            if (task.isDone) {
                taskLabel->setStyleSheet("background:#E5E7EB;color:#6B7280;border-radius:4px;padding:2px 4px;font-size:11px;");
            } else if (countdownText(task) == "Terlambat") {
                taskLabel->setStyleSheet("background:#FEE2E2;color:#991B1B;border-radius:4px;padding:2px 4px;font-size:11px;font-weight:bold;");
            } else if (task.priority == "Tinggi") {
                taskLabel->setStyleSheet("background:#FEE2E2;color:#991B1B;border-radius:4px;padding:2px 4px;font-size:11px;font-weight:bold;");
            } else if (task.priority == "Sedang") {
                taskLabel->setStyleSheet("background:#FEF3C7;color:#92400E;border-radius:4px;padding:2px 4px;font-size:11px;");
            } else {
                taskLabel->setStyleSheet("background:#DBEAFE;color:#1E3A8A;border-radius:4px;padding:2px 4px;font-size:11px;");
            }

            boxLayout->addWidget(taskLabel);
            shown++;

            if (shown == 2) {
                break;
            }
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

    for (int row = 1; row <= 6; row++) {
        for (int col = 0; col < 7; col++) {
            QFrame *box = new QFrame;
            box->setObjectName("dateBox");
            box->setCursor(Qt::PointingHandCursor);
            box->setMinimumHeight(88);

            QVBoxLayout *boxLayout = new QVBoxLayout(box);
            boxLayout->setContentsMargins(7, 5, 7, 5);
            boxLayout->setSpacing(3);

            QLabel *number = new QLabel("");

            if ((row == 1 && col >= startColumn) || row > 1) {
                if (day <= totalDays) {
                    QDate thisDate(currentDate.year(), currentDate.month(), day);

                    box->installEventFilter(this);
                    box->setProperty("date", thisDate);

                    number->setText(QString::number(day));
                    number->setObjectName("dateNumber");

                    boxLayout->addWidget(number);
                    addTaskToCalendarBox(boxLayout, thisDate);

                    if (thisDate == QDate::currentDate()) {
                        box->setStyleSheet("background:#E6E4FF;border:2px solid #2563FF;border-radius:8px;");
                    }

                    day++;
                } else {
                    boxLayout->addWidget(number);
                }
            } else {
                boxLayout->addWidget(number);
            }

            boxLayout->addStretch();
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

        if (task.reminderDate == today &&
            task.reminderTime.hour() == now.hour() &&
            task.reminderTime.minute() == now.minute()) {

            QString key = task.title + current.toString("yyyyMMddHHmm");

            if (!notifiedTasks.contains(key)) {
                notifiedTasks.insert(key);
                startAlarm(task.title);
            }
        }
    }
}

void MainWindow::startAlarm(const QString &taskTitle)
{
    currentAlarmTask = taskTitle;

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
    alarmBox->setWindowTitle("⏰ Alarm Tugas");
    alarmBox->setText("Waktunya mengerjakan tugas!");
    alarmBox->setInformativeText("Tugas: " + taskTitle);
    alarmBox->setIcon(QMessageBox::Warning);
    alarmBox->setStandardButtons(QMessageBox::Ok);
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
                       + "\nJam: " + task.reminderTime.toString("HH:mm")
                       + "\nStatus: " + QString(task.isDone ? "Selesai" : "Belum selesai")
                       + "\nSisa waktu: " + countdownText(task)
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
    if (event->type() == QEvent::MouseButtonPress) {
        QVariant dateValue = obj->property("date");

        if (dateValue.isValid()) {
            QDate date = dateValue.toDate();
            showTasksByDate(date);
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
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
        settings.setValue("date", tasks[i].reminderDate);
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

            QPushButton#addBtn {
                background:#2563FF;
            }

            QPushButton#editBtn {
                background:#F59E0B;
            }

            QPushButton#listBtn {
                background:#EF4444;
            }

            QPushButton#doneBtn {
                background:#00A650;
            }

            QPushButton#notifBtn {
                background:#9817F4;
            }

            QPushButton#soundBtn {
                background:#0EA5E9;
            }

            QPushButton#themeBtn {
                background:#111827;
            }

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

            QComboBox,
            QLineEdit {
                background:white;
                color:black;
                border:1px solid #DDDDDD;
                border-radius:10px;
                padding:8px 14px;
                font-size:14px;
            }

            QListWidget#taskList {
                background:white;
                color:black;
                border:1px solid #E5E7EB;
                border-radius:10px;
                padding:6px;
                font-size:14px;
            }

            QListWidget#taskList::item {
                padding:8px;
                border-radius:6px;
            }

            QListWidget#taskList::item:selected {
                background:#DBEAFE;
                color:black;
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

            QScrollArea {
                background:transparent;
                border:none;
            }
        )");
    } else {
        setStyleSheet(R"(
            QWidget#root {
                background-color:#0F172A;
                font-family:Arial;
            }

            QLabel#title {
                font-size:30px;
                font-weight:800;
                color:white;
            }

            QLabel#subtitle {
                font-size:16px;
                color:#CBD5E1;
            }

            QLabel#smallTitle {
                font-size:15px;
                font-weight:bold;
                color:white;
            }

            QFrame#card,
            QFrame#panel,
            QFrame#calendarPanel {
                background:#1E293B;
                border-radius:14px;
                border:1px solid #334155;
            }

            QLabel#cardNumber {
                font-size:24px;
                font-weight:bold;
            }

            QLabel#cardText {
                font-size:14px;
                color:#CBD5E1;
            }

            QPushButton {
                border:none;
                border-radius:9px;
                padding:9px 12px;
                color:white;
                font-size:14px;
                font-weight:bold;
            }

            QPushButton#addBtn {
                background:#2563FF;
            }

            QPushButton#editBtn {
                background:#F59E0B;
            }

            QPushButton#listBtn {
                background:#EF4444;
            }

            QPushButton#doneBtn {
                background:#00A650;
            }

            QPushButton#notifBtn {
                background:#9817F4;
            }

            QPushButton#soundBtn {
                background:#0EA5E9;
            }

            QPushButton#themeBtn {
                background:#475569;
            }

            QPushButton#navBtn,
            QPushButton#todayBtn {
                background:transparent;
                color:white;
                font-size:18px;
                font-weight:bold;
            }

            QComboBox,
            QLineEdit {
                background:#0F172A;
                color:white;
                border:1px solid #475569;
                border-radius:10px;
                padding:8px 14px;
                font-size:14px;
            }

            QListWidget#taskList {
                background:#0F172A;
                color:white;
                border:1px solid #475569;
                border-radius:10px;
                padding:6px;
                font-size:14px;
            }

            QListWidget#taskList::item {
                padding:8px;
                border-radius:6px;
            }

            QListWidget#taskList::item:selected {
                background:#334155;
                color:white;
            }

            QLabel#monthTitle,
            QLabel#dayName,
            QLabel#dateNumber {
                color:white;
                font-weight:bold;
            }

            QFrame#dateBox {
                background:#1E293B;
                border:1px solid #475569;
                border-radius:8px;
                min-height:88px;
            }

            QFrame#dateBox:hover {
                background:#243044;
                border:1px solid #60A5FA;
            }

            QProgressBar {
                border:1px solid #475569;
                border-radius:8px;
                height:18px;
                background:#0F172A;
                color:white;
                text-align:center;
            }

            QProgressBar::chunk {
                background:#22C55E;
                border-radius:8px;
            }

            QScrollArea {
                background:transparent;
                border:none;
            }
        )");
    }
}