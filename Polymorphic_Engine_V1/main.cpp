#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion")); // to have a clean ui look
    a.setAttribute(Qt::AA_EnableHighDpiScaling);
    MainWindow w;
    w.show();//run the application
    return a.exec();
}
