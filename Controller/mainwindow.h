#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Communication/ControllerClient.h"
#include "../Communication/Utility.h"
#include <unistd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

	void on_pushButton_7_clicked();

	//client(CanTouchThis::LOCALHOST, CanTouchThis::PORT)

    void on_pushButton_clicked();

    //void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_6_clicked();

private:
    char isPushed[6];
    bool flag1;
    int flag2;
    Ui::MainWindow *ui;
    CanTouchThis::ControllerClient* client;

    pid_t conversionPID;
    pid_t viewerPID;
    pid_t commPID;
};

#endif // MAINWINDOW_H
