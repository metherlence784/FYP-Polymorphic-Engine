#ifndef OPEN_EXISTING_FILE_CONTROLLER_H
#define OPEN_EXISTING_FILE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QStringListIterator>
#include <iostream>

#include "write_code_in_c_cpp_controller.h"
#include "file_reader.h"
#include "choose_payload_controller.h"
#include "compile_code_controller.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Open_Existing_File_Controller : public QObject
{
    Q_OBJECT
public:
    Open_Existing_File_Controller();//constructor
    ~Open_Existing_File_Controller();//destructor

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void open_existing_file();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // OPEN_EXISTING_FILE_CONTROLLER_H
