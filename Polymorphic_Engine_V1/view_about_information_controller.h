#ifndef VIEW_ABOUT_INFORMATION_CONTROLLER_H
#define VIEW_ABOUT_INFORMATION_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class View_About_Information_Controller : public QObject
{
    Q_OBJECT
public:
    View_About_Information_Controller();
    ~View_About_Information_Controller();

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void view_about_us();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString about_us;

};

#endif // VIEW_ABOUT_INFORMATION_CONTROLLER_H
