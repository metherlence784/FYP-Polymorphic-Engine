#ifndef COMPILE_CODE_CONTROLLER_H
#define COMPILE_CODE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>

#include <iostream>
#include <stdlib.h>

#include "ui_mainwindow.h"
#include "mainwindow.h"


class Compile_Code_Controller : public QObject
{
    Q_OBJECT
public:
     Compile_Code_Controller();
     ~Compile_Code_Controller();

     //accessor
     QString get_file_name();
     QString get_file_path();
     MainWindow *get_cur_wind();
     QString get_status();
     QString get_temp_compile();
     qint64 get_elapsed_time();


     //mutator
     void set_cur_wind(MainWindow *cur);
     void set_exe_name();
     void set_enabled_compile_button(bool set);
     void set_enabled_analysis_textbox(bool set);
     void set_status(QString txt);
     void set_temp_compile(QString temp_compile);
     void set_elapsed_time(qint64 elapsed_time);

     void compile_code();


 private:
     MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
     QString file_name;
     QString file_path;
     QString exe_name;
     QString status;
     QString temp_compile;
     qint64 elapsed_time;

};

#endif // COMPILE_CODE_CONTROLLER_H
