#ifndef VIEW_COMPILATION_STATUS_CONTROLLER_H
#define VIEW_COMPILATION_STATUS_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QDateTime>
#include <QStandardPaths>

#include <iostream>
#include <stdlib.h>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class View_Compilation_Status_Controller : public QObject
{
    Q_OBJECT
public:
    View_Compilation_Status_Controller();
    ~View_Compilation_Status_Controller();

    //accessor
    MainWindow *get_cur_wind();
    QString get_compilation_status();


    //mutator
    void set_cur_wind(MainWindow *cur);
    void set_text_analysis_textbox(QString txt);
    void set_compilation_status(QString compilation_status);


    void update_analysis_textbox_and_enable_morph(QString analysis_textbox_status, qint64 elapsed_time, QString txt_file);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString compilation_status;

};

#endif // VIEW_COMPILATION_STATUS_CONTROLLER_H
