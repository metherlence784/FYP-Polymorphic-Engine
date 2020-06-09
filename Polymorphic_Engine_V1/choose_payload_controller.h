#ifndef CHOOSE_PAYLOAD_CONTROLLER_H
#define CHOOSE_PAYLOAD_CONTROLLER_H

#include <QMainWindow>
#include <QObject>


#include "ui_mainwindow.h"
#include "mainwindow.h"


class Choose_Payload_Controller : public QObject
{
    Q_OBJECT
public:
     Choose_Payload_Controller();
     ~Choose_Payload_Controller();

     //accessor
     MainWindow *get_cur_wind();
     QString get_payload();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // CHOOSE_PAYLOAD_CONTROLLER_H
