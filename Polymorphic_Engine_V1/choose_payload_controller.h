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
     int get_payload();

     //mutator
     void set_cur_wind(MainWindow *cur);
     void set_enabled_payload_groupbox(QString compilation_status);
     void set_text_and_color_payload_info_textbox(int id);
     void set_initial_payload_info(QString compilation_status);

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    int chosen_payload;

    const QString calculator_payload_info = QString("Runs your systems calculator!");
    const QString CMD_payload_info = QString("Runs an instance of the command prompt!");
    const QString message_box_payload_info = QString("Spawns a message box with the text \"O_o!\"");
    const QString fatality_payload_info = QString("Spawns a message box with the text \"fatality: get rekt\" and then crashes your program!");
    const QString system_info_payload_info = QString("Prints out important system information!");
    const QString download_putty_payload_info = QString("Downloads and executes putty!");
    const QString new_admin_payload_info = QString("Creates an admin account.\nName = \"ROOT\"\nPassword = \"I@mR00T$\"");
    const QString warning = QString("This payload is potentially dangerous!!!\n\n");
    const QString as_admin = QString("\n\nNote: the morphed executable must be run as admin");
};

#endif // CHOOSE_PAYLOAD_CONTROLLER_H
