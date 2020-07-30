#ifndef FILE_SAVER_H
#define FILE_SAVER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QRegExpValidator>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <iostream>
#include <fstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

class File_Saver : public QObject
{
    Q_OBJECT
public:
    File_Saver();//Constructor

    ~File_Saver();//Desctructor

    QString new_file();
    QString save_file_as(QString text);
    void save_file(QString file_path,QString text);
    void write_exe_file(QString morphed_exe_file_path, std::vector<char> &buffer);

    //accessor
    MainWindow* get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow* cur_wind);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // FILE_SAVER_H
