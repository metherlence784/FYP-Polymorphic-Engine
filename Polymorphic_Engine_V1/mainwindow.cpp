﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exit_application_controller.h"
#include "create_new_file_controller.h"
#include "save_as_new_file_controller.h"
#include "save_current_file_controller.h"
#include "open_existing_file_controller.h"

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
    Exit_Application_Controller *my_exit = new Exit_Application_Controller();
    my_exit->exit_application();
}


void MainWindow::on_New_File_Menu_Item_triggered()
{
    Create_New_File_Controller *my_new = new Create_New_File_Controller();
    my_new->create_new_file();
}

void MainWindow::on_Save_As_Menu_Item_triggered()
{
    Save_As_New_File_Controller *my_save_as = new Save_As_New_File_Controller();
    my_save_as->save_as_new_file();
}

void MainWindow::on_Save_Menu_Item_triggered()
{
    Save_Current_File_Controller *my_save = new Save_Current_File_Controller();
    my_save->save();
}

void MainWindow::on_Open_File_Menu_Item_triggered()
{
    Open_Existing_File_Controller *my_open = new Open_Existing_File_Controller();
    my_open->open_existing_file();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    Exit_Application_Controller *my_exit = new Exit_Application_Controller();
    my_exit->exit_application(event);
}

