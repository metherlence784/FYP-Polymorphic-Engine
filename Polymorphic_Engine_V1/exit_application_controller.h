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
    //constructor
    Exit_Application_Controller();

    //destructor
    ~Exit_Application_Controller();

    void exit_application();

private:
    MainWindow *cur_wind;
};

#endif // EXIT_APPLICATION_CONTROLLER_H
