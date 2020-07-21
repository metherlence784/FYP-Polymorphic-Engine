#ifndef CLEAR_OUTPUT_LOG_CONTROLLER_H
#define CLEAR_OUTPUT_LOG_CONTROLLER_H

#include <QMainWindow>
#include <QObject>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Clear_Output_Log_Controller : public QObject
{
    Q_OBJECT
public:
    Clear_Output_Log_Controller();
    ~Clear_Output_Log_Controller();

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void clear_output_log();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // CLEAR_OUTPUT_LOG_CONTROLLER_H
