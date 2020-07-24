#ifndef VIEW_DISASSEMBLY_CONTROLLER_H
#define VIEW_DISASSEMBLY_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QDesktopServices>
#include <vector>
#include <iostream>
#include <sstream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "file_reader.h"
#include "file_saver.h"

class View_Disassembly_Controller : public QObject
{
    Q_OBJECT
public:
    View_Disassembly_Controller();
    ~View_Disassembly_Controller();

    //accessor
    MainWindow *get_cur_wind();
    QString get_file_text();

    //mutator
    void set_cur_wind(MainWindow *cur);
    void set_file_text(QString file_text);

    void set_enabled_view_disassembly_button(QString morph_status);
    void get_text_from_disassembly_log(QString file_path);
    void write_disassembly(QString file_path, QString disassembly);
    QString set_disassembly_log_path(QString file_path);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString file_text;

};

#endif // VIEW_DISASSEMBLY_CONTROLLER_H
