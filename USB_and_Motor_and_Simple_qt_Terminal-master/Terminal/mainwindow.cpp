#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QString>
#include <QLabel>
#include <QDebug>
#include <QTime>

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Terminal");
    //read avaible comports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            ui->comboBox->addItem(info.portName());

    serial = new QSerialPort;
    settings = new SettingsDialog;

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::on_But_Settings_clicked()
{
    settings->show();
}

void MainWindow::on_But_Connect_clicked()
{
    if (ui->But_Connect->text() == "Connect")
    {
        //select name for our serial port from combobox
        if (serial->portName() != ui->comboBox->currentText())
        {
            serial->close();
            serial->setPortName(ui->comboBox->currentText());
        }

        if (serial->open(QIODevice::ReadWrite))
        {
            QPixmap pixmap(":/images/greenStone.png");
            ui->label->setPixmap(pixmap);
            ui->But_Connect->setText("Disconnect");
            QString str2 = "0";
            serial->write(str2.toUtf8());
        }else
        {
            ;
        }
    }
    else
    {
        QPixmap pixmap(":/images/redStone.png");
        ui->label->setPixmap(pixmap);
        serial->close();
        ui->But_Connect->setText("Connect");
    }
}

void MainWindow::on_But_Send1_clicked()
{
    QString str_t = "1";
    Send_to_Serial(str_t);
    str_t.clear();

    delay(5);
    if (ui->checkBox_N_Enable->isChecked())
        str_t.append("1");
    else
        str_t.append("0");

    if (ui->checkBox_MS1->isChecked())
        str_t.append("1");
    else
        str_t.append("0");

    if (ui->checkBox_MS2->isChecked())
        str_t.append("1");
    else
        str_t.append("0");

    if (ui->checkBox_MS3->isChecked())
        str_t.append("1");
    else
        str_t.append("0");

    if (ui->checkBox_STEP->isChecked())
        str_t.append("1");
    else
        str_t.append("0");

    if (ui->checkBox_DIR->isChecked())
        str_t.append("1");
    else
        str_t.append("0");

    Send_to_Serial(str_t);
}

void MainWindow::on_But_Send2_clicked()
{
    QString str_t = "2";
    Send_to_Serial(str_t);
}

void MainWindow::Send_to_Serial(QString str_to_Serial)
{
    if (serial->isOpen() && serial->isWritable())
    {
        serial->write(str_to_Serial.toUtf8());
        //ui->textEdit->append(str_to_Serial);
    }
    else
        qDebug() << "i try send" + str_to_Serial;
}

void MainWindow::readData()
{
    QByteArray temp_data;
    //if (serial->waitForReadyRead(1))
        temp_data = serial->readAll();
    qDebug() << temp_data;
    ui->textEdit->append(temp_data);
}
