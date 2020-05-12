#ifndef OPEN_EXISTING_FILE_CONTROLLER_H
#define OPEN_EXISTING_FILE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QStringListIterator>
#include <iostream>


#include "ui_mainwindow.h"
#include "mainwindow.h"

class Open_Existing_File_Controller : public QObject
{
    Q_OBJECT
public:
    Open_Existing_File_Controller();
    ~Open_Existing_File_Controller();

    MainWindow *get_cur_wind();//accessor

    void set_cur_wind(MainWindow *cur);//mutator

    void open_existing_file();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // OPEN_EXISTING_FILE_CONTROLLER_H
