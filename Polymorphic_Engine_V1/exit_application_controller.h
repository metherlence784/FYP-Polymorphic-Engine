#ifndef EXIT_APPLICATION_CONTROLLER_H
#define EXIT_APPLICATION_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include "ui_mainwindow.h"
#include "mainwindow.h"


class Exit_Application_Controller : public QObject
{
    Q_OBJECT
public:

    Exit_Application_Controller();//constructor
    ~Exit_Application_Controller();//destructor

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void exit_application();
    void exit_application(QCloseEvent *event);

private:
    MainWindow *cur_wind;// variable pointer for mainwindow class to access ui
};

#endif // EXIT_APPLICATION_CONTROLLER_H
