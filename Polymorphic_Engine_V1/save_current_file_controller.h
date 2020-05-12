#ifndef SAVE_CURRENT_FILE_CONTROLLER_H
#define SAVE_CURRENT_FILE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include <iostream>



#include "ui_mainwindow.h"
#include "mainwindow.h"

class Save_Current_File_Controller : public QObject
{
    Q_OBJECT
public:
    Save_Current_File_Controller();
    ~Save_Current_File_Controller();

    void save();

    MainWindow *get_cur_wind();//accessor

    void set_cur_wind(MainWindow *cur);//mutator

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui


};

#endif // SAVE_CURRENT_FILE_CONTROLLER_H
