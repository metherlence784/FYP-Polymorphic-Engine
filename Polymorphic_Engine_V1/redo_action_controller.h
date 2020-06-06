#ifndef REDO_ACTION_CONTROLLER_H
#define REDO_ACTION_CONTROLLER_H

#include <QMainWindow>
#include <QObject>


#include "ui_mainwindow.h"
#include "mainwindow.h"


class Redo_Action_Controller : public QObject
{
    Q_OBJECT
public:
    Redo_Action_Controller();
    ~Redo_Action_Controller();

    void redo_action();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // REDO_ACTION_CONTROLLER_H
