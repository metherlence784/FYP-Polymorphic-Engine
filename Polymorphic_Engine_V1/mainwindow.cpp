#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testingClass.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	testingClass test(10);
	QString ss = QString::number(test.getNum());

	ui->pushButton->setText(ss);

}

MainWindow::~MainWindow()
{
    delete ui;
}

