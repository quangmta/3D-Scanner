#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_But_Connect_clicked();

    void on_But_Settings_clicked();

    void on_But_Send1_clicked();

    void on_But_Send2_clicked();

    void Send_to_Serial(QString str_to_Serial);

    void readData();
private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    SettingsDialog *settings;
};

#endif // MAINWINDOW_H
