#ifndef CREATE_NEW_FILE_CONTROLLER_H
#define CREATE_NEW_FILE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include <iostream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Create_New_File_Controller : public QObject
{
    Q_OBJECT
public:

    Create_New_File_Controller(); //constructor
    ~Create_New_File_Controller(); //destructor

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void create_new_file();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
};

#endif // CREATE_NEW_FILE_CONTROLLER_H