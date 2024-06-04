#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::on_helpButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_openFileButton_clicked() {
    currentFileName = QFileDialog::getOpenFileName(this, "Open BMP File", "", "BMP Files (*.bmp)");
    if (currentFileName.isEmpty()) {
        return;
    }

    QFile file(currentFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file");
        return;
    }

    QDataStream in(&file);
    in.readRawData(reinterpret_cast<char*>(&header), sizeof(headerFileBitMap));
    in.readRawData(reinterpret_cast<char*>(&info), sizeof(infoHeaderBitMap));

    int lessThanEight = 0;
    if (header.bfType != 0x4D42 || checkBitCount(info, &lessThanEight)) {
        QMessageBox::warning(this, "Error", "Incorrect BMP format");
        return;
    }

    file.close();
    ui->statusLabel->setText("File loaded successfully: " + currentFileName);
}

void MainWindow::on_wnbButton_clicked() {
    if (currentFileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file loaded");
        return;
    }

    QFile inFile(currentFileName);
    if (!inFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading");
        return;
    }

    QString resultName = formName(currentFileName, "wnb_");
    wnb(inFile, header, info, resultName);

    inFile.close();
    ui->statusLabel->setText("W&B filter applied. Result saved as: " + resultName);
}

void MainWindow::on_negativeButton_clicked() {
    if (currentFileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file loaded");
        return;
    }
    QFile inFile(currentFileName);
    if (!inFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading");
        return;
    }

    QString resultName = formName(currentFileName, "negative_");
    negative(inFile, header, info, resultName);

    inFile.close();
    ui->statusLabel->setText("Negative filter applied. Result saved as: " + resultName);
}

void MainWindow::on_gammaButton_clicked() {
    if (currentFileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file loaded");
        return;
    }

    QFile inFile(currentFileName);
    if (!inFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading");
        return;
    }

    QString resultName = formName(currentFileName, "gamma_");
    gammaCorrection(inFile, header, info, resultName);

    inFile.close();
    ui->statusLabel->setText("Gamma correction applied. Result saved as: " + resultName);
}

    void MainWindow::on_medianButton_clicked() {
        if (currentFileName.isEmpty()) {
            QMessageBox::warning(this, "Error", "No file loaded");
            return;
        }

        QFile inFile(currentFileName);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Could not open file for reading");
            return;
        }

        QString resultName = formName(currentFileName, "median_");
        medianFilter(inFile, header, info, resultName);
        inFile.close();
        ui->statusLabel->setText("Median filter applied. Result saved as: " + resultName);
    }
    void MainWindow::on_brightnessButton_clicked() {
        if (currentFileName.isEmpty()) {
            QMessageBox::warning(this, "Error", "No file loaded");
            return;
        }

        QFile inFile(currentFileName);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Could not open file for reading");
            return;
        }

        QString resultName = formName(currentFileName, "brightness");
        medianFilter(inFile, header, info, resultName);
        inFile.close();
        ui->statusLabel->setText("Brighness filter applied. Result saved as: " + resultName);
    }
    void MainWindow::on_blurButton_clicked() {
        if (currentFileName.isEmpty()) {
            QMessageBox::warning(this, "Error", "No file loaded");
            return;
        }

        QFile inFile(currentFileName);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Could not open file for reading");
            return;
        }

        QString resultName = formName(currentFileName, "blur");
        medianFilter(inFile, header, info, resultName);
        inFile.close();
        ui->statusLabel->setText("Blur filter applied. Result saved as: " + resultName);
    }
    void MainWindow::on_helpButton_clicked()
    {
        QString rules = "Правила использования:\n"
                        "1. Для выбора изображения нажмите на 'OPEN BMP FILE'!'.\n"
                        "2. Для использования фильтра негатив нажмите на кнопку Apply Negativ Filter'.\n"
                        "3. Для использования фильтра гамма-коррекция нажмите на кнопку Apply Gamma Filter.'\n"
                        "4. Для использования медианная фильтрация нажмите на кнопку Apply Median Filter.\n"
                        "5. Для использования фильтра размытие нажмите на кнопку Apply Blur Filter'.\n"
                        "6. Для использования фильтра яркость нажмите на кнопку Brightness Filter.";

        QMessageBox::information(this, "Руководство по использованию", rules);
    }
    void MainWindow::on_cutButton_clicked() {
        if (currentFileName.isEmpty()) {
            QMessageBox::warning(this, "Error", "No file loaded");
            return;
        }

        QFile inFile(currentFileName);
        if (!inFile.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Could not open file for reading");
            return;
        }

        QString resultName = formName(currentFileName, "cut");
        medianFilter(inFile, header, info, resultName);
        inFile.close();
        ui->statusLabel->setText("Cut filter applied. Result saved as: " + resultName);
    }
