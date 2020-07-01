#ifndef FILE_READER_H
#define FILE_READER_H

#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <QStringList>
#include <QStringListIterator>

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

#include "open_existing_file_controller.h"

class File_Reader : public QObject
{
    Q_OBJECT
public:
    File_Reader();
    File_Reader(MainWindow *mw_ptr);//Constructor
    ~File_Reader();

    QStringList open_file();
    QString file_check(QString file_path);
    QString read_compile_status(QString file_path);
    QString read_file_into_vector(QString exe_file_path,std::vector<char>& buffer);


private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

    template<typename T>
    std::string num_to_hex(T i)
    {
        std::stringstream stream;
        stream << "0x"
            << std::setfill('0') << std::setw(sizeof(T) * 2)
            << std::hex << i;
        return stream.str();
    }

};

#endif // FILE_READER_H
