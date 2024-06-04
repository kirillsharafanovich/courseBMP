#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "functions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();
    void on_wnbButton_clicked();
    void on_negativeButton_clicked();
    void on_gammaButton_clicked();
    void on_medianButton_clicked();
    void on_brightnessButton_clicked();
    void saveImage(const QString &filePath);
    void on_blurButton_clicked();
    void on_helpButton_clicked();
    void on_cutButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFileName;
    headerFileBitMap header;
    infoHeaderBitMap info;

};

#endif // MAINWINDOW_H
