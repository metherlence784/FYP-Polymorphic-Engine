#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exit_application_controller.h"

//set pointer to null first
MainWindow* MainWindow::MWptr = nullptr;

//constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MWptr = this;
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//get mainwindow ptr
MainWindow* MainWindow::getMWptr()
{
    return MWptr;
}

//button trigger
void MainWindow::on_Exit_Menu_Item_triggered()
{
    Exit_Application_Controller *myExit = new Exit_Application_Controller();
    myExit->exit_application();
}


