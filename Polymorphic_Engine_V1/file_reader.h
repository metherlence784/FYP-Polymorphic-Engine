#ifndef FILE_READER_H
#define FILE_READER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <QStringList>
#include <QStringListIterator>
#include <QFileInfo>

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

#include "template_functions.h"
#include "PE32.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

class File_Reader : public QObject
{
    Q_OBJECT
public:
    File_Reader();
    ~File_Reader();

    QStringList open_file();
    QString file_check(QString file_path);
    QString read_compile_status(QString file_path);
    QString read_file_into_vector(QString exe_file_path,std::vector<char>& buffer);
	
	qint64 get_executable_size(QString exe_file_path);

    //accessor
    MainWindow* get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow* cur_wind);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui



};

#endif // FILE_READER_H
