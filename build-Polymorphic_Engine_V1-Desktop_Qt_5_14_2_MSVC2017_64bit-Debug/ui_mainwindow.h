/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *New_File_Menu_Item;
    QAction *Open_File_Menu_Item;
    QAction *Save_Menu_Item;
    QAction *Save_As_Menu_Item;
    QAction *Exit_Menu_Item;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QTextEdit *Code_Input_Textbox;
    QWidget *tab_2;
    QMenuBar *menubar;
    QMenu *File_Menu_Header;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(676, 711);
        New_File_Menu_Item = new QAction(MainWindow);
        New_File_Menu_Item->setObjectName(QString::fromUtf8("New_File_Menu_Item"));
        Open_File_Menu_Item = new QAction(MainWindow);
        Open_File_Menu_Item->setObjectName(QString::fromUtf8("Open_File_Menu_Item"));
        Save_Menu_Item = new QAction(MainWindow);
        Save_Menu_Item->setObjectName(QString::fromUtf8("Save_Menu_Item"));
        Save_Menu_Item->setEnabled(false);
        Save_As_Menu_Item = new QAction(MainWindow);
        Save_As_Menu_Item->setObjectName(QString::fromUtf8("Save_As_Menu_Item"));
        Save_As_Menu_Item->setEnabled(false);
        Exit_Menu_Item = new QAction(MainWindow);
        Exit_Menu_Item->setObjectName(QString::fromUtf8("Exit_Menu_Item"));
        Exit_Menu_Item->setCheckable(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 651, 661));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setEnabled(true);
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 101, 16));
        Code_Input_Textbox = new QTextEdit(tab);
        Code_Input_Textbox->setObjectName(QString::fromUtf8("Code_Input_Textbox"));
        Code_Input_Textbox->setEnabled(false);
        Code_Input_Textbox->setGeometry(QRect(10, 30, 421, 571));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 676, 21));
        File_Menu_Header = new QMenu(menubar);
        File_Menu_Header->setObjectName(QString::fromUtf8("File_Menu_Header"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(File_Menu_Header->menuAction());
        File_Menu_Header->addAction(New_File_Menu_Item);
        File_Menu_Header->addAction(Open_File_Menu_Item);
        File_Menu_Header->addSeparator();
        File_Menu_Header->addAction(Save_Menu_Item);
        File_Menu_Header->addAction(Save_As_Menu_Item);
        File_Menu_Header->addSeparator();
        File_Menu_Header->addAction(Exit_Menu_Item);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Polymorphic Engine", nullptr));
        New_File_Menu_Item->setText(QCoreApplication::translate("MainWindow", "New File", nullptr));
        Open_File_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        Save_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Save ", nullptr));
#if QT_CONFIG(shortcut)
        Save_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        Save_As_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Save As", nullptr));
        Exit_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "C0d3 here", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "C0d3z", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Analysis", nullptr));
        File_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
