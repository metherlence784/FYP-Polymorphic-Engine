#ifndef SAVE_AS_NEW_FILE_CONTROLLER_H
#define SAVE_AS_NEW_FILE_CONTROLLER_H
#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include <iostream>



#include "ui_mainwindow.h"
#include "mainwindow.h"

class Save_As_New_File_Controller : public QObject
{
    Q_OBJECT
public:
    Save_As_New_File_Controller();//constructor
    ~Save_As_New_File_Controller();//destructor

    void save_as_new_file();

    MainWindow *get_cur_wind();//accessor

    void set_cur_wind(MainWindow *cur);//mutator

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // SAVE_AS_NEW_FILE_CONTROLLER_H
