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
#include "save_as_new_file_controller.h"
#include "save_current_file_controller.h"

class File_Saver : public QObject
{
    Q_OBJECT
public:
    File_Saver();//Constructor
    File_Saver(MainWindow *mw_ptr);//Constructor
    ~File_Saver();//Desctructor

    QString new_file();
    QString save_file_as(QString text);
    void save_file(QString file_path,QString text);


private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // FILE_SAVER_H
