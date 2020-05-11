#ifndef FILE_SAVER_H
#define FILE_SAVER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <iostream>

#include "create_new_file_controller.h"

class File_Saver : public QObject
{
    Q_OBJECT
public:
     File_Saver();
     File_Saver(MainWindow *mw_ptr);//Constructor
     ~File_Saver();//Desctructor

     QString save_file();
     void save_file_as(QString file_path);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // FILE_SAVER_H
