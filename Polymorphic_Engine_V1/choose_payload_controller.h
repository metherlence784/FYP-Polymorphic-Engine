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

     //mutator
     void set_cur_wind(MainWindow *cur);
     void set_enabled_payload_groupbox(QString compilation_status);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString chosen_payload;

};

#endif // CHOOSE_PAYLOAD_CONTROLLER_H
