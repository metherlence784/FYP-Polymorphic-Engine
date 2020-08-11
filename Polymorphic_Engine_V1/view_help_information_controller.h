#ifndef VIEW_HELP_INFORMATION_CONTROLLER_H
#define VIEW_HELP_INFORMATION_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QDesktopServices>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class View_Help_Information_Controller : public QObject
{
    Q_OBJECT
public:
    View_Help_Information_Controller();
    ~View_Help_Information_Controller();

    //accessor
    MainWindow *get_cur_wind();

    //mutator
    void set_cur_wind(MainWindow *cur);

    void help();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString website_link;
};

#endif // VIEW_HELP_INFORMATION_CONTROLLER_H
