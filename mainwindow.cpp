#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDate(2026, 5, 1)
{
    setWindowTitle("To-Do List Saya");
    resize(1450, 850);

    QWidget *root = new QWidget(this);
    root->setObjectName("root");

    QVBoxLayout *mainLayout = new QVBoxLayout(root);
    mainLayout->setContentsMargins(20, 18, 20, 20);
    mainLayout->setSpacing(22);

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
    panelLayout->setContentsMargins(30, 30, 30, 30);
    panelLayout->setSpacing(20);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(15);

    QPushButton *addButton = new QPushButton("+  Tambah Tugas");
    QPushButton *deleteButton = new QPushButton("☷  Hapus Tugas");
    QPushButton *notifButton = new QPushButton("🔔  Aktifkan Notifikasi");

    addButton->setObjectName("addBtn");
    deleteButton->setObjectName("listBtn");
    notifButton->setObjectName("notifBtn");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(notifButton);
    buttonLayout->addStretch();

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->setSpacing(15);

    QLabel *filterIcon = new QLabel("▽");
    filterIcon->setObjectName("filterIcon");

    categoryBox = new QComboBox;
    categoryBox->addItems({"Semua Kategori", "Kuliah", "Kerja", "Pribadi"});

    priorityBox = new QComboBox;
    priorityBox->addItems({"Semua Prioritas", "Tinggi", "Sedang", "Rendah"});

    filterLayout->addWidget(filterIcon);
    filterLayout->addWidget(categoryBox);
    filterLayout->addWidget(priorityBox);
    filterLayout->addStretch();

    taskList = new QListWidget;
    taskList->setObjectName("taskList");

    panelLayout->addLayout(buttonLayout);
    panelLayout->addLayout(filterLayout);
    panelLayout->addWidget(taskList);

    mainLayout->addWidget(controlPanel);

    QFrame *calendarPanel = new QFrame;
    calendarPanel->setObjectName("calendarPanel");

    QVBoxLayout *calendarLayout = new QVBoxLayout(calendarPanel);
    calendarLayout->setContentsMargins(30, 30, 30, 30);
    calendarLayout->setSpacing(25);

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
    calendarGrid->setSpacing(10);

    calendarLayout->addLayout(calendarHeader);
    calendarLayout->addLayout(calendarGrid);

    mainLayout->addWidget(calendarPanel);
    setCentralWidget(root);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteTask);

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

    buildCalendar();
    updateStats();

    setStyleSheet(R"(
        QWidget#root {
            background-color: #F4F7FF;
            font-family: Arial;
        }

        QLabel#title {
            font-size: 42px;
            font-weight: 800;
            color: #14243D;
        }

        QLabel#subtitle {
            font-size: 20px;
            color: #334155;
        }

        QFrame#card {
            background-color: white;
            border-radius: 10px;
            border: 1px solid #E5E7EB;
            min-height: 90px;
        }

        QLabel#cardNumber {
            font-size: 28px;
            font-weight: bold;
        }

        QLabel#cardText {
            font-size: 16px;
            color: #334155;
        }

        QFrame#panel,
        QFrame#calendarPanel {
            background-color: white;
            border-radius: 12px;
            border: 1px solid #EDF0F7;
        }

        QPushButton {
            border: none;
            border-radius: 9px;
            padding: 13px 22px;
            color: white;
            font-size: 18px;
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
            font-size: 32px;
            padding: 8px 16px;
        }

        QPushButton#todayBtn {
            background: transparent;
            color: black;
            font-size: 18px;
            font-weight: bold;
        }

        QComboBox {
            background-color: white;
            border: 1px solid #DDDDDD;
            border-radius: 10px;
            padding: 12px 18px;
            font-size: 18px;
            min-width: 160px;
        }

        QListWidget#taskList {
            border: 1px solid #E5E7EB;
            border-radius: 10px;
            padding: 8px;
            font-size: 16px;
        }

        QLabel#filterIcon {
            font-size: 30px;
            color: #475569;
        }

        QLabel#monthTitle {
            font-size: 30px;
            font-weight: bold;
            color: black;
        }

        QLabel#dayName {
            font-size: 16px;
            font-weight: bold;
            color: black;
        }

        QFrame#dateBox {
            background-color: white;
            border: 1px solid #E0E0E0;
            border-radius: 5px;
            min-height: 80px;
        }

        QLabel#dateNumber {
            font-size: 16px;
            font-weight: bold;
            color: black;
        }
    )");
}

MainWindow::~MainWindow()
{
}

QFrame* MainWindow::createStatCard(QLabel *&numberLabel, const QString &text, const QString &color)
{
    QFrame *card = new QFrame;
    card->setObjectName("card");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 18, 20, 18);

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
    bool ok;
    QString task = QInputDialog::getText(
        this,
        "Tambah Tugas",
        "Masukkan nama tugas:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (ok && !task.trimmed().isEmpty()) {
        QString category = categoryBox->currentText();
        QString priority = priorityBox->currentText();

        if (category == "Semua Kategori") {
            category = "Umum";
        }

        if (priority == "Semua Prioritas") {
            priority = "Normal";
        }

        taskList->addItem(task + " | " + category + " | " + priority);
        updateStats();
    }
}

void MainWindow::deleteTask()
{
    QListWidgetItem *item = taskList->currentItem();

    if (!item) {
        QMessageBox::warning(this, "Peringatan", "Pilih tugas yang ingin dihapus.");
        return;
    }

    delete item;
    updateStats();
}

void MainWindow::updateStats()
{
    int total = taskList->count();

    totalLabel->setText(QString::number(total));
    doneLabel->setText("0");
    notDoneLabel->setText(QString::number(total));
    lateLabel->setText("0");
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

            QVBoxLayout *boxLayout = new QVBoxLayout(box);
            boxLayout->setContentsMargins(8, 8, 8, 8);

            QLabel *number = new QLabel("");

            if ((row == 1 && col >= startColumn) || row > 1) {
                if (day <= totalDays) {
                    number->setText(QString::number(day));

                    QDate thisDate(currentDate.year(), currentDate.month(), day);
                    if (thisDate == QDate::currentDate()) {
                        box->setStyleSheet("background-color: #E6E4FF; border: 2px solid #2563FF; border-radius: 5px;");
                    }

                    day++;
                }
            }

            number->setObjectName("dateNumber");
            boxLayout->addWidget(number);
            boxLayout->addStretch();

            calendarGrid->addWidget(box, row, col);
        }
    }
}
