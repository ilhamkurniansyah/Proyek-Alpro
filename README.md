# TickTask

TickTask adalah Aplikasi manajemen tugas berbasis desktop yang dibangun dengan **Qt C++**. Dirancangg agar bisa mengatur tugas kuliah, jadwal, dan deadline dengan mudah.

## Deskripsi
Project ini dibuat sebagai tugas Algoritma dan Pemrograman menggunakan framework Qt6.  
Aplikasi memiliki fitur pencatatan tugas, pengingat waktu, kalender, dan statistik tugas.

## Fitur Utama

### Manajemen Tugas
- Tambah, edit, dan hapus tugas
- Tandai tugas sebagai **selesai / belum selesai**
- Filter tugas berdasarkan **kategori** (Kuliah, Kerja, Pribadi, Umum) dan **prioritas** (Tinggi, Sedang, Rendah, Normal)
- Pencarian tugas secara real-time
- Tampilkan **riwayat tugas selesai** dengan satu klik

### Kalender Interaktif
- Tampilan kalender bulanan
- Klik pada tanggal untuk melihat atau menambah tugas
- Navigasi antar bulan (prev/next/hari ini)
- Tugas tampil langsung di kotak tanggal dengan kode warna berdasarkan prioritas/status

### Jadwal Kuliah
- Kelola **semester** (tambah, ubah nama, hapus, aktifkan)
- Tambah **mata kuliah** per hari lengkap dengan jam mulai dan selesai
- Tampilan jadwal mingguan di atas kalender

### Sistem Alarm & Notifikasi
- Alarm otomatis saat waktu deadline tiba
- Pilih file musik sendiri sebagai suara alarm (`.mp3` / `.wav`)
- Countdown sisa waktu ditampilkan langsung di daftar tugas

### Statistik & Progress
- Kartu statistik: **Total Tugas**, **Selesai**, **Belum Selesai**, **Terlambat**
- Progress bar persentase penyelesaian tugas

### Tema Gelap / Terang
- Toggle dark mode / light mode dengan satu klik

## Teknologi

| Komponen | Detail |
| Bahasa | C++ |
| Framework | Qt 6 (Qt Widgets, Qt Multimedia) |
| Penyimpanan | `QSettings` (registry/INI lokal) |
| Build System | qmake / CMake |

## Cara Build & Jalankan

### Menggunakan Qt Creator
1. Buka aplikasi **Qt Creator**.
2. Pilih **Open Project** → buka file `ayam.pro` atau `CMakeLists.txt` yang berada di dalam folder `TO DO LIST`.
3. Pilih kit yang sesuai (Qt 6).
4. Klik **Build** (ikon palu) lalu **Run** (ikon panah hijau).

## Struktur File
## 📂 Struktur File

```text
Proyek-Alpro/
├── TO DO LIST/
│   ├── .qtcreator/
│   ├── build/
│   ├── screenshots/
│   │   └── Tampilan_APK.jpeg
│   ├── CMakeLists.txt
│   ├── ayam.pro
│   ├── logo.ico
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── resources.qrc
│   ├── tambahan.cpp
│   └── tambahan.h
├── .gitignore
└── README.md
```

##  Penyimpanan Data

Semua data disimpan secara lokal menggunakan `QSettings` dengan identifier:
- **Organization:** `MyCompany`
- **Application:** `TodoListSaya`

Data yang disimpan meliputi tugas, jadwal kuliah per semester, semester aktif, dan path file alarm.

## Tampilan Aplikasi
![Tampilan TickTask](TO%20DO%20LIST/screenshots/Tampilan_APK.jpeg)
