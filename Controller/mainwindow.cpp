#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    flag1 = true;
    flag2 = false;

    for (int i = 0; i < 6; ++i)
        isPushed[i] = '0';
    ui->setupUi(this);
	ui->pushButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
#ifdef _WIN64
	system("taskkill /IM Communication.exe");
#endif
}

void MainWindow::on_pushButton_2_clicked()
{
    if (isPushed[1]=='0') {
        ui->pushButton_2->setText("MX1 Start R");
        client->commandScanner(CanTouchThis::Command::MOTOR_X1_START);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[1] = '1';
    }else if (isPushed[1]=='1') {
        ui->pushButton_2->setText("MX1 Stop");
        client->commandScanner(CanTouchThis::Command::MOTOR_X1_STARTR);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[1] = '2';
    } else {
        ui->pushButton_2->setText("MX1 Start");
        client->commandScanner(CanTouchThis::Command::MOTOR_X1_STOP);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton->setEnabled(true);
        isPushed[1] = '0';
    }



}

void MainWindow::on_pushButton_3_clicked()
{
    if (isPushed[2]=='0') {
        ui->pushButton_3->setText("MX2 Start R");
        client->commandScanner(CanTouchThis::Command::MOTOR_X2_START);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[2] = '1';
    }else if (isPushed[2]=='1') {
        ui->pushButton_3->setText("MX2 Stop");
        client->commandScanner(CanTouchThis::Command::MOTOR_X2_STARTR);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[2] = '2';
    } else {
        ui->pushButton_3->setText("MX2 Start");
        client->commandScanner(CanTouchThis::Command::MOTOR_X2_STOP);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton->setEnabled(true);
        isPushed[2] = '0';
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    if (isPushed[3]=='0') {
        ui->pushButton_4->setText("MY Start R");
        client->commandScanner(CanTouchThis::Command::MOTOR_Y_START);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[3] = '1';
    }else  if (isPushed[3]=='1') {
        ui->pushButton_4->setText("MY Stop");
        client->commandScanner(CanTouchThis::Command::MOTOR_Y_STARTR);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[3] = '2';
    }else {
        ui->pushButton_4->setText("MY Start");
        client->commandScanner(CanTouchThis::Command::MOTOR_Y_STOP);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton->setEnabled(true);
        isPushed[3] = '0';
    }

}

void MainWindow::on_pushButton_5_clicked()
{
    if (isPushed[4]=='0') {
        ui->pushButton_5->setText("MZ Start R");
        client->commandScanner(CanTouchThis::Command::MOTOR_Z_START);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[4] = '1';
    }else  if (isPushed[4]=='1') {
        ui->pushButton_5->setText("MZ Stop");
        client->commandScanner(CanTouchThis::Command::MOTOR_Z_STARTR);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton->setDisabled(true);
        isPushed[4] = '2';
    } else {
        ui->pushButton_5->setText("MZ Start");
        client->commandScanner(CanTouchThis::Command::MOTOR_Z_STOP);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton->setEnabled(true);
        isPushed[4] = '0';
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (isPushed[0]=='0') {
        ui->pushButton->setText("Stop Scan");
        client->commandScanner(CanTouchThis::Command::START_SCAN);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
#ifdef _WIN64
		system("start Conversion.exe");
		system("start Viewer.exe");
#endif
        isPushed[0] = '1';
        //ui->checkBox->setDisabled(true);
    } else {
        ui->pushButton->setText("Start Scan");
        client->commandScanner(CanTouchThis::Command::STOP_SCAN);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        //ui->checkBox->setEnabled(true);
#ifdef _WIN64
		system("taskkill /IM Conversion.exe");
		system("taskkill /IM Viewer.exe");
#endif
        isPushed[0] = '0';
    }
}

void MainWindow::on_pushButton_6_clicked()
{

    if (isPushed[0]=='0') {
        ui->pushButton_6->setText("MX Start R");
        client->commandScanner(CanTouchThis::Command::MOTOR_X_START);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton->setDisabled(true);
        //ui->checkBox->setDisabled(true);
        isPushed[0]='1';
    }else  if (isPushed[0]=='1') {
        ui->pushButton_6->setText("MX Stop");
        client->commandScanner(CanTouchThis::Command::MOTOR_X_STARTR);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton->setDisabled(true);
        //ui->checkBox->setDisabled(true);
        isPushed[0]='2';
    } else {
        ui->pushButton_6->setText("MX Start");
        client->commandScanner(CanTouchThis::Command::MOTOR_X_STOP);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton->setEnabled(true);
        //ui->checkBox->setEnabled(true);
        isPushed[0]='0';
    }
}


void MainWindow::on_pushButton_7_clicked() {
#ifdef _WIN64
	system("start Communication.exe");
#endif
	ui->label_2->setText("Connected");
	client = new CanTouchThis::ControllerClient(CanTouchThis::LOCALHOST, CanTouchThis::PORT);
	ui->pushButton->setEnabled(true);
	ui->pushButton_7->setEnabled(false);
}