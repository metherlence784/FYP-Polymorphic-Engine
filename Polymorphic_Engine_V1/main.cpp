#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","0");
    a.setStyle(QStyleFactory::create("Fusion")); // to have a clean ui look
    MainWindow w;
    w.show();//run the application
    return a.exec();
}
