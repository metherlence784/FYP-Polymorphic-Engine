#include "mainwindow.h"
#include "testingClass.h"


#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{
	testingClass test(10);
	std::cout << test.getNum() << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
