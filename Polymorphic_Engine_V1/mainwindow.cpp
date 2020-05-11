#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exit_application_controller.h"
#include "create_new_file_controller.h"

//set pointer to null first
MainWindow* MainWindow::MWptr = nullptr;

//constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MWptr = this;
    cur_file_path = "";
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//accesors below
//get mainwindow ptr
MainWindow* MainWindow::getMWptr()
{
    return MWptr;
}
QString MainWindow::get_cur_filename()
{
    return this->cur_file_path;
}


//mutator below
void MainWindow::set_cur_filename(QString str)
{
    this->cur_file_path = str;
}

//button triggers below here
void MainWindow::on_Exit_Menu_Item_triggered()
{
    Exit_Application_Controller *myExit = new Exit_Application_Controller();
    myExit->exit_application();
}
void MainWindow::on_actionNew_File_triggered()
{
    Create_New_File_Controller *myNew = new Create_New_File_Controller();
    myNew->create_new_file();
}
