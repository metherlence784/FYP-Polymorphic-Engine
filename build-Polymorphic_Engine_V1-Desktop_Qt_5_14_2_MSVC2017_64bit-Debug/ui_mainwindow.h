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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
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
    QAction *Undo_Menu_Item;
    QAction *Redo_Menu_Item;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QTextEdit *Code_Input_Textbox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *Payload_RButton_Group;
    QRadioButton *Calculator_Payload_RButton;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QWidget *tab_2;
    QTextEdit *Analysis_Textbox;
    QMenuBar *menubar;
    QMenu *File_Menu_Header;
    QMenu *Edit_Menu_Header;
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
        Undo_Menu_Item = new QAction(MainWindow);
        Undo_Menu_Item->setObjectName(QString::fromUtf8("Undo_Menu_Item"));
        Redo_Menu_Item = new QAction(MainWindow);
        Redo_Menu_Item->setObjectName(QString::fromUtf8("Redo_Menu_Item"));
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
        verticalLayoutWidget = new QWidget(tab);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(450, 210, 160, 151));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Payload_RButton_Group = new QGroupBox(verticalLayoutWidget);
        Payload_RButton_Group->setObjectName(QString::fromUtf8("Payload_RButton_Group"));
        Calculator_Payload_RButton = new QRadioButton(Payload_RButton_Group);
        Calculator_Payload_RButton->setObjectName(QString::fromUtf8("Calculator_Payload_RButton"));
        Calculator_Payload_RButton->setGeometry(QRect(0, 20, 158, 17));
        Calculator_Payload_RButton->setChecked(true);
        radioButton = new QRadioButton(Payload_RButton_Group);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(0, 50, 158, 17));
        radioButton_2 = new QRadioButton(Payload_RButton_Group);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(0, 80, 151, 17));
        radioButton_3 = new QRadioButton(Payload_RButton_Group);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(0, 110, 151, 17));

        verticalLayout->addWidget(Payload_RButton_Group);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        Analysis_Textbox = new QTextEdit(tab_2);
        Analysis_Textbox->setObjectName(QString::fromUtf8("Analysis_Textbox"));
        Analysis_Textbox->setEnabled(false);
        Analysis_Textbox->setGeometry(QRect(10, 30, 421, 571));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 676, 21));
        File_Menu_Header = new QMenu(menubar);
        File_Menu_Header->setObjectName(QString::fromUtf8("File_Menu_Header"));
        Edit_Menu_Header = new QMenu(menubar);
        Edit_Menu_Header->setObjectName(QString::fromUtf8("Edit_Menu_Header"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(File_Menu_Header->menuAction());
        menubar->addAction(Edit_Menu_Header->menuAction());
        File_Menu_Header->addAction(New_File_Menu_Item);
        File_Menu_Header->addAction(Open_File_Menu_Item);
        File_Menu_Header->addSeparator();
        File_Menu_Header->addAction(Save_Menu_Item);
        File_Menu_Header->addAction(Save_As_Menu_Item);
        File_Menu_Header->addSeparator();
        File_Menu_Header->addAction(Exit_Menu_Item);
        Edit_Menu_Header->addAction(Undo_Menu_Item);
        Edit_Menu_Header->addAction(Redo_Menu_Item);

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
        Undo_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
#if QT_CONFIG(shortcut)
        Undo_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        Redo_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
#if QT_CONFIG(shortcut)
        Redo_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        label->setText(QCoreApplication::translate("MainWindow", "C0d3 here", nullptr));
        Payload_RButton_Group->setTitle(QCoreApplication::translate("MainWindow", "Choose your payload", nullptr));
        Calculator_Payload_RButton->setText(QCoreApplication::translate("MainWindow", "Spawn Calculator", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Payload X", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "Payload Y", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "Payload Z", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "C0d3z", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Analysis", nullptr));
        File_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        Edit_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
