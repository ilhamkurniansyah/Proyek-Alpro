#include "mainwindow.h"

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
#include <QDate>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QDialog>
#include <QDateEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QScrollArea>
#include <QApplication>
#include <QEvent>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDate(QDate::currentDate())
{
    setWindowTitle("To-Do List Saya");
    resize(1450, 850);

    QWidget *root = new QWidget(this);
    root->setObjectName("root");

    QVBoxLayout *mainLayout = new QVBoxLayout(root);
    mainLayout->setContentsMargins(18, 14, 18, 14);
    mainLayout->setSpacing(14);

    QLabel *title = new QLabel("📝 To-Do List Saya");
    title->setObjectName("title");

    QLabel *subtitle = new QLabel("Kelola tugas Anda dengan mudah dan efisien");
    subtitle->setObjectName("subtitle");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    QHBoxLayout *statLayout = new QHBoxLayout;
    statLayout->setSpacing(18);

    statLayout->addWidget(createStatCard(totalLabel, "Total Tugas", "#2563EB"));
    statLayout->addWidget(createStatCard(doneLabel, "Selesai", "#00A650"));
    statLayout->addWidget(createStatCard(notDoneLabel, "Belum Selesai", "#F0440A"));
    statLayout->addWidget(createStatCard(lateLabel, "Terlambat", "#E60000"));

    mainLayout->addLayout(statLayout);

    QFrame *controlPanel = new QFrame;
    controlPanel->setObjectName("panel");

    QVBoxLayout *panelLayout = new QVBoxLayout(controlPanel);
    panelLayout->setContentsMargins(28, 22, 28, 22);
    panelLayout->setSpacing(14);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(14);

    QPushButton *addButton = new QPushButton("+  Tambah Tugas");
    QPushButton *deleteButton = new QPushButton("☷  Hapus Tugas");
    QPushButton *notifButton = new QPushButton("🔔  Cek Notifikasi");

    addButton->setObjectName("addBtn");
    deleteButton->setObjectName("listBtn");
    notifButton->setObjectName("notifBtn");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(notifButton);
    buttonLayout->addStretch();

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->setSpacing(14);

    QLabel *filterIcon = new QLabel("▽");
    filterIcon->setObjectName("filterIcon");

    categoryBox = new QComboBox;
    categoryBox->addItems({"Semua Kategori", "Kuliah", "Kerja", "Pribadi", "Umum"});

    priorityBox = new QComboBox;
    priorityBox->addItems({"Semua Prioritas", "Tinggi", "Sedang", "Rendah", "Normal"});

    filterLayout->addWidget(filterIcon);
    filterLayout->addWidget(categoryBox);
    filterLayout->addWidget(priorityBox);
    filterLayout->addStretch();

    taskList = new QListWidget;
    taskList->setObjectName("taskList");
    taskList->setSelectionMode(QAbstractItemView::SingleSelection);

    QLabel *taskTitle = new QLabel("Daftar Tugas");
    taskTitle->setObjectName("smallTitle");

    panelLayout->addLayout(buttonLayout);
    panelLayout->addLayout(filterLayout);
    panelLayout->addWidget(taskTitle);
    panelLayout->addWidget(taskList);

    mainLayout->addWidget(controlPanel);

    QFrame *calendarPanel = new QFrame;
    calendarPanel->setObjectName("calendarPanel");

    QVBoxLayout *calendarLayout = new QVBoxLayout(calendarPanel);
    calendarLayout->setContentsMargins(28, 18, 28, 18);
    calendarLayout->setSpacing(12);

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
    calendarGrid->setSpacing(7);

    calendarLayout->addLayout(calendarHeader);
    calendarLayout->addLayout(calendarGrid);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(calendarPanel);
    scrollArea->setFrameShape(QFrame::NoFrame);

    mainLayout->addWidget(scrollArea, 1);

    setCentralWidget(root);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteTask);
    connect(notifButton, &QPushButton::clicked, this, &MainWindow::checkNotifications);

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
    connect(notificationTimer, &QTimer::timeout, this, &MainWindow::checkNotifications);
    notificationTimer->start(60000);

    buildCalendar();
    updateStats();

    setStyleSheet(R"(
        QWidget#root {
            background-color: #F4F7FF;
            font-family: Arial;
        }

        QLabel#title {
            font-size: 34px;
            font-weight: 800;
            color: #14243D;
        }

        QLabel#subtitle {
            font-size: 18px;
            color: #334155;
        }

        QFrame#card {
            background-color: white;
            border-radius: 10px;
            border: 1px solid #E5E7EB;
            min-height: 72px;
        }

        QLabel#cardNumber {
            font-size: 26px;
            font-weight: bold;
        }

        QLabel#cardText {
            font-size: 15px;
            color: #334155;
        }

        QFrame#panel,
        QFrame#calendarPanel {
            background-color: white;
            border-radius: 12px;
            border: 1px solid #EDF0F7;
        }

        QLabel#smallTitle {
            font-size: 16px;
            font-weight: bold;
            color: #14243D;
        }

        QPushButton {
            border: none;
            border-radius: 9px;
            padding: 12px 22px;
            color: white;
            font-size: 17px;
            font-weight: bold;
        }

        QPushButton#addBtn {
            background-color: #2563FF;
        }

        QPushButton#listBtn {
            background-color: #9817F4;
        }

        QPushButton#notifBtn {
            background-color: #00A94F;
        }

        QPushButton#navBtn {
            background: transparent;
            color: black;
            font-size: 30px;
            padding: 4px 12px;
        }

        QPushButton#todayBtn {
            background: transparent;
            color: black;
            font-size: 17px;
            font-weight: bold;
        }

        QComboBox {
            background-color: white;
            color: black;
            border: 1px solid #DDDDDD;
            border-radius: 10px;
            padding: 10px 18px;
            font-size: 16px;
            min-width: 230px;
        }

        QListWidget#taskList {
            background-color: white;
            color: black;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
            padding: 6px;
            font-size: 15px;
            min-height: 70px;
            max-height: 95px;
        }

        QListWidget#taskList::item:selected {
            background-color: #DBEAFE;
            color: black;
        }

        QLabel#filterIcon {
            font-size: 28px;
            color: #475569;
        }

        QLabel#monthTitle {
            font-size: 26px;
            font-weight: bold;
            color: black;
        }

        QLabel#dayName {
            font-size: 15px;
            font-weight: bold;
            color: black;
        }

        QFrame#dateBox {
            background-color: white;
            border: 1px solid #E0E0E0;
            border-radius: 5px;
            min-height: 95px;
        }

        QLabel#dateNumber {
            font-size: 15px;
            font-weight: bold;
            color: black;
        }

        QScrollArea {
            background: transparent;
            border: none;
        }
    )");
}

QFrame* MainWindow::createStatCard(QLabel *&numberLabel, const QString &text, const QString &color)
{
    QFrame *card = new QFrame;
    card->setObjectName("card");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 12, 20, 12);

    numberLabel = new QLabel("0");
    numberLabel->setObjectName("cardNumber");
    numberLabel->setStyleSheet("color: " + color + ";");

    QLabel *label = new QLabel(text);
    label->setObjectName("cardText");

    layout->addWidget(numberLabel);
    layout->addWidget(label);

    return card;
}

void MainWindow::addTask()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Tambah Tugas");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *taskInput = new QLineEdit;
    taskInput->setPlaceholderText("Masukkan nama tugas");

    QComboBox *categoryInput = new QComboBox;
    categoryInput->addItems({"Kuliah", "Kerja", "Pribadi", "Umum"});

    QComboBox *priorityInput = new QComboBox;
    priorityInput->addItems({"Tinggi", "Sedang", "Rendah", "Normal"});

    QDateEdit *dateInput = new QDateEdit;
    dateInput->setCalendarPopup(true);
    dateInput->setDate(QDate::currentDate());
    dateInput->setDisplayFormat("dd MMMM yyyy");

    QFormLayout *form = new QFormLayout;
    form->addRow("Nama Tugas:", taskInput);
    form->addRow("Kategori:", categoryInput);
    form->addRow("Prioritas:", priorityInput);
    form->addRow("Tanggal Pengingat:", dateInput);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
        );

    layout->addLayout(form);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString taskName = taskInput->text().trimmed();

        if (taskName.isEmpty()) {
            QMessageBox::warning(this, "Peringatan", "Nama tugas tidak boleh kosong.");
            return;
        }

        TaskData task;
        task.title = taskName;
        task.category = categoryInput->currentText();
        task.priority = priorityInput->currentText();
        task.reminderDate = dateInput->date();

        tasks.append(task);

        taskList->addItem(
            task.title + " | " +
            task.category + " | " +
            task.priority + " | " +
            task.reminderDate.toString("dd MMM yyyy")
            );

        currentDate = QDate(task.reminderDate.year(), task.reminderDate.month(), 1);

        updateStats();
        buildCalendar();
    }
}

void MainWindow::deleteTask()
{
    int row = taskList->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Peringatan", "Klik salah satu tugas dulu sebelum menghapus.");
        return;
    }

    delete taskList->takeItem(row);

    if (row >= 0 && row < tasks.size()) {
        tasks.removeAt(row);
    }

    updateStats();
    buildCalendar();
}

void MainWindow::updateStats()
{
    int total = tasks.size();
    int late = 0;

    for (const TaskData &task : tasks) {
        if (task.reminderDate < QDate::currentDate()) {
            late++;
        }
    }

    totalLabel->setText(QString::number(total));
    doneLabel->setText("0");
    notDoneLabel->setText(QString::number(total));
    lateLabel->setText(QString::number(late));
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
            QLabel *taskLabel = new QLabel("• " + task.title);
            taskLabel->setWordWrap(true);

            if (date == QDate::currentDate()) {
                taskLabel->setStyleSheet(
                    "background-color:#FEE2E2;"
                    "color:#991B1B;"
                    "border-radius:4px;"
                    "padding:2px 4px;"
                    "font-size:11px;"
                    "font-weight:bold;"
                    );
            } else if (task.priority == "Tinggi") {
                taskLabel->setStyleSheet(
                    "background-color:#FEE2E2;"
                    "color:#991B1B;"
                    "border-radius:4px;"
                    "padding:2px 4px;"
                    "font-size:11px;"
                    );
            } else if (task.priority == "Sedang") {
                taskLabel->setStyleSheet(
                    "background-color:#FEF3C7;"
                    "color:#92400E;"
                    "border-radius:4px;"
                    "padding:2px 4px;"
                    "font-size:11px;"
                    );
            } else {
                taskLabel->setStyleSheet(
                    "background-color:#DBEAFE;"
                    "color:#1E3A8A;"
                    "border-radius:4px;"
                    "padding:2px 4px;"
                    "font-size:11px;"
                    );
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
                        box->setStyleSheet(
                            "background-color: #E6E4FF;"
                            "border: 2px solid #2563FF;"
                            "border-radius: 5px;"
                            );
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
    QDate today = QDate::currentDate();
    bool found = false;

    for (const TaskData &task : tasks) {
        if (task.reminderDate == today) {
            QString key = task.title + today.toString("yyyyMMdd");

            if (!notifiedTasks.contains(key)) {
                QApplication::beep();

                QMessageBox::information(
                    this,
                    "🔔 Pengingat Tugas",
                    "Tugas hari ini:\n\n" + task.title +
                        "\nKategori: " + task.category +
                        "\nPrioritas: " + task.priority
                    );

                notifiedTasks.insert(key);
                found = true;
            }
        }
    }

    if (!found) {
        QMessageBox::information(this, "Notifikasi", "Tidak ada tugas baru untuk hari ini.");
    }
}

void MainWindow::showTasksByDate(const QDate &date)
{
    QString message;

    for (const TaskData &task : tasks) {
        if (task.reminderDate == date) {
            message += "• " + task.title
                       + "\nKategori: " + task.category
                       + "\nPrioritas: " + task.priority
                       + "\n\n";
        }
    }

    if (message.isEmpty()) {
        message = "Tidak ada tugas pada tanggal ini.";
    }

    QMessageBox::information(
        this,
        "Tugas " + date.toString("dd MMMM yyyy"),
        message
        );
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