#ifndef UNDO_ACTION_CONTROLLER_H
#define UNDO_ACTION_CONTROLLER_H

#include <QMainWindow>
#include <QObject>


#include "ui_mainwindow.h"
#include "mainwindow.h"


class Undo_Action_Controller : public QObject
{
    Q_OBJECT
public:
    Undo_Action_Controller();
    ~Undo_Action_Controller();

    void undo_action();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // UNDO_ACTION_CONTROLLER_H
