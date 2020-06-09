#ifndef WRITE_CODE_IN_C_CPP_CONTROLLER_H
#define WRITE_CODE_IN_C_CPP_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QTextStream>

#include <iostream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Write_Code_In_C_CPP_Controller : public QObject
{
    Q_OBJECT
public:
    Write_Code_In_C_CPP_Controller();//constructor
    ~Write_Code_In_C_CPP_Controller();//destructor

    //accessor
    QString get_text();

    //mutator
    void set_cur_wind(MainWindow *cur);
    void set_text(QString txt);
    void set_enabled_code_input_textbox(bool set);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString text;

};

#endif // WRITE_CODE_IN_C_CPP_CONTROLLER_H
