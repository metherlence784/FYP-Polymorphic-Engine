#ifndef RUN_EXECUTABLE_CONTROLLER_H
#define RUN_EXECUTABLE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QStandardPaths>
#include <QProcess>

#include <iostream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Run_Executable_Controller : public QObject
{
    Q_OBJECT
public:
    Run_Executable_Controller();
    ~Run_Executable_Controller();

    //accessor
    MainWindow *get_cur_wind();
    QString get_original_exe_name();
    QString get_original_exe_file_path();
    QString get_morphed_exe_name();
    QString get_morphed_exe_file_path();

    //mutator
    void set_cur_wind(MainWindow *cur);
    void set_original_exe_name(QString original_exe_name);
    void set_original_exe_file_path(QString original_exe_file_path);
    void set_morphed_exe_name(QString morphed_exe_name);
    void set_morphed_exe_file_path(QString morphed_exe_file_path);

    void set_original_executable_label(QString original_status);
    void set_morphed_executable_label(QString morphed_status);

    void run_original_executable(QString original_exe_file_path);
    void run_morphed_executable(QString morphed_exe_file_path);


private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString original_exe_name;
    QString original_exe_file_path;
    QString morphed_exe_name;
    QString morphed_exe_file_path;

};

#endif // RUN_EXECUTABLE_CONTROLLER_H
