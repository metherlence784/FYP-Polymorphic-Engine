#ifndef SAVE_CURRENT_FILE_CONTROLLER_H
#define SAVE_CURRENT_FILE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>

#include <iostream>

#include "write_code_in_c_cpp_controller.h"
#include "file_saver.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Save_Current_File_Controller : public QObject
{
    Q_OBJECT
public:
    Save_Current_File_Controller();//constructor
    ~Save_Current_File_Controller();//destructor

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void save();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // SAVE_CURRENT_FILE_CONTROLLER_H
