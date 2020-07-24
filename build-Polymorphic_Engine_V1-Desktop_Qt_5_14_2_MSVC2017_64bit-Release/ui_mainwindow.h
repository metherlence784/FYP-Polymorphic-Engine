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
#include <QtWidgets/QPushButton>
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
    QAction *Compile_Menu_Item;
    QAction *Morph_Menu_Item;
    QAction *About_Menu_Item;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *Cod3z;
    QLabel *label;
    QTextEdit *Code_Input_Textbox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *Payload_RButton_Group;
    QRadioButton *Calculator_Payload_RButton;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QPushButton *Compile_Button;
    QLabel *label_2;
    QPushButton *Morph_Button;
    QWidget *Analysis;
    QTextEdit *Analysis_Textbox;
    QLabel *Original_Executable_Label;
    QLabel *Morphed_Executable_Label;
    QPushButton *Run_Original_Button;
    QPushButton *Run_Morphed_Button;
    QPushButton *Clear_Output_Log_Button;
    QPushButton *View_Executable_Size_Button;
    QPushButton *View_Executable_Signature_Button;
    QPushButton *View_Disassembly_Button;
    QLabel *label_3;
    QMenuBar *menubar;
    QMenu *File_Menu_Header;
    QMenu *Edit_Menu_Header;
    QMenu *Morph_Menu_Header;
    QMenu *About_Menu_Header;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(535, 700);
        MainWindow->setMinimumSize(QSize(535, 700));
        MainWindow->setMaximumSize(QSize(535, 700));
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
        Compile_Menu_Item = new QAction(MainWindow);
        Compile_Menu_Item->setObjectName(QString::fromUtf8("Compile_Menu_Item"));
        Compile_Menu_Item->setEnabled(false);
        Morph_Menu_Item = new QAction(MainWindow);
        Morph_Menu_Item->setObjectName(QString::fromUtf8("Morph_Menu_Item"));
        Morph_Menu_Item->setEnabled(false);
        About_Menu_Item = new QAction(MainWindow);
        About_Menu_Item->setObjectName(QString::fromUtf8("About_Menu_Item"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 10, 505, 621));
        tabWidget->setMinimumSize(QSize(505, 621));
        tabWidget->setMaximumSize(QSize(521, 621));
        Cod3z = new QWidget();
        Cod3z->setObjectName(QString::fromUtf8("Cod3z"));
        Cod3z->setEnabled(true);
        label = new QLabel(Cod3z);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 101, 20));
        QFont font;
        font.setPointSize(9);
        label->setFont(font);
        Code_Input_Textbox = new QTextEdit(Cod3z);
        Code_Input_Textbox->setObjectName(QString::fromUtf8("Code_Input_Textbox"));
        Code_Input_Textbox->setEnabled(false);
        Code_Input_Textbox->setGeometry(QRect(10, 30, 341, 531));
        verticalLayoutWidget = new QWidget(Cod3z);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(359, 80, 131, 181));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Payload_RButton_Group = new QGroupBox(verticalLayoutWidget);
        Payload_RButton_Group->setObjectName(QString::fromUtf8("Payload_RButton_Group"));
        Payload_RButton_Group->setEnabled(false);
        Calculator_Payload_RButton = new QRadioButton(Payload_RButton_Group);
        Calculator_Payload_RButton->setObjectName(QString::fromUtf8("Calculator_Payload_RButton"));
        Calculator_Payload_RButton->setGeometry(QRect(0, 20, 111, 17));
        Calculator_Payload_RButton->setChecked(true);
        radioButton = new QRadioButton(Payload_RButton_Group);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(0, 50, 111, 17));
        radioButton_2 = new QRadioButton(Payload_RButton_Group);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(0, 80, 111, 17));
        radioButton_3 = new QRadioButton(Payload_RButton_Group);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(0, 110, 101, 17));

        verticalLayout->addWidget(Payload_RButton_Group);

        Compile_Button = new QPushButton(Cod3z);
        Compile_Button->setObjectName(QString::fromUtf8("Compile_Button"));
        Compile_Button->setEnabled(false);
        Compile_Button->setGeometry(QRect(360, 30, 101, 31));
        label_2 = new QLabel(Cod3z);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 600, 251, 31));
        Morph_Button = new QPushButton(Cod3z);
        Morph_Button->setObjectName(QString::fromUtf8("Morph_Button"));
        Morph_Button->setEnabled(false);
        Morph_Button->setGeometry(QRect(360, 270, 101, 31));
        tabWidget->addTab(Cod3z, QString());
        Analysis = new QWidget();
        Analysis->setObjectName(QString::fromUtf8("Analysis"));
        Analysis_Textbox = new QTextEdit(Analysis);
        Analysis_Textbox->setObjectName(QString::fromUtf8("Analysis_Textbox"));
        Analysis_Textbox->setEnabled(false);
        Analysis_Textbox->setGeometry(QRect(10, 130, 351, 431));
        Analysis_Textbox->setMinimumSize(QSize(351, 431));
        Analysis_Textbox->setFrameShadow(QFrame::Raised);
        Analysis_Textbox->setReadOnly(true);
        Original_Executable_Label = new QLabel(Analysis);
        Original_Executable_Label->setObjectName(QString::fromUtf8("Original_Executable_Label"));
        Original_Executable_Label->setEnabled(true);
        Original_Executable_Label->setGeometry(QRect(10, 30, 351, 21));
        Original_Executable_Label->setAutoFillBackground(true);
        Original_Executable_Label->setFrameShape(QFrame::Box);
        Original_Executable_Label->setFrameShadow(QFrame::Sunken);
        Morphed_Executable_Label = new QLabel(Analysis);
        Morphed_Executable_Label->setObjectName(QString::fromUtf8("Morphed_Executable_Label"));
        Morphed_Executable_Label->setEnabled(true);
        Morphed_Executable_Label->setGeometry(QRect(10, 80, 351, 21));
        Morphed_Executable_Label->setAutoFillBackground(true);
        Morphed_Executable_Label->setFrameShape(QFrame::Box);
        Morphed_Executable_Label->setFrameShadow(QFrame::Sunken);
        Run_Original_Button = new QPushButton(Analysis);
        Run_Original_Button->setObjectName(QString::fromUtf8("Run_Original_Button"));
        Run_Original_Button->setEnabled(false);
        Run_Original_Button->setGeometry(QRect(370, 30, 91, 23));
        Run_Morphed_Button = new QPushButton(Analysis);
        Run_Morphed_Button->setObjectName(QString::fromUtf8("Run_Morphed_Button"));
        Run_Morphed_Button->setEnabled(false);
        Run_Morphed_Button->setGeometry(QRect(370, 80, 91, 23));
        Clear_Output_Log_Button = new QPushButton(Analysis);
        Clear_Output_Log_Button->setObjectName(QString::fromUtf8("Clear_Output_Log_Button"));
        Clear_Output_Log_Button->setGeometry(QRect(370, 540, 81, 23));
        View_Executable_Size_Button = new QPushButton(Analysis);
        View_Executable_Size_Button->setObjectName(QString::fromUtf8("View_Executable_Size_Button"));
        View_Executable_Size_Button->setEnabled(false);
        View_Executable_Size_Button->setGeometry(QRect(370, 130, 91, 23));
        View_Executable_Signature_Button = new QPushButton(Analysis);
        View_Executable_Signature_Button->setObjectName(QString::fromUtf8("View_Executable_Signature_Button"));
        View_Executable_Signature_Button->setEnabled(false);
        View_Executable_Signature_Button->setGeometry(QRect(370, 160, 91, 23));
        View_Disassembly_Button = new QPushButton(Analysis);
        View_Disassembly_Button->setObjectName(QString::fromUtf8("View_Disassembly_Button"));
        View_Disassembly_Button->setEnabled(false);
        View_Disassembly_Button->setGeometry(QRect(370, 190, 91, 23));
        tabWidget->addTab(Analysis, QString());
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 630, 241, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 535, 21));
        File_Menu_Header = new QMenu(menubar);
        File_Menu_Header->setObjectName(QString::fromUtf8("File_Menu_Header"));
        Edit_Menu_Header = new QMenu(menubar);
        Edit_Menu_Header->setObjectName(QString::fromUtf8("Edit_Menu_Header"));
        Morph_Menu_Header = new QMenu(menubar);
        Morph_Menu_Header->setObjectName(QString::fromUtf8("Morph_Menu_Header"));
        About_Menu_Header = new QMenu(menubar);
        About_Menu_Header->setObjectName(QString::fromUtf8("About_Menu_Header"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(File_Menu_Header->menuAction());
        menubar->addAction(Edit_Menu_Header->menuAction());
        menubar->addAction(Morph_Menu_Header->menuAction());
        menubar->addAction(About_Menu_Header->menuAction());
        File_Menu_Header->addAction(New_File_Menu_Item);
        File_Menu_Header->addAction(Open_File_Menu_Item);
        File_Menu_Header->addSeparator();
        File_Menu_Header->addAction(Save_Menu_Item);
        File_Menu_Header->addAction(Save_As_Menu_Item);
        File_Menu_Header->addSeparator();
        File_Menu_Header->addAction(Exit_Menu_Item);
        Edit_Menu_Header->addAction(Undo_Menu_Item);
        Edit_Menu_Header->addAction(Redo_Menu_Item);
        Morph_Menu_Header->addAction(Compile_Menu_Item);
        Morph_Menu_Header->addAction(Morph_Menu_Item);
        About_Menu_Header->addAction(About_Menu_Item);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Polymorphic Engine", nullptr));
        New_File_Menu_Item->setText(QCoreApplication::translate("MainWindow", "New File", nullptr));
#if QT_CONFIG(shortcut)
        New_File_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        Open_File_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
#if QT_CONFIG(shortcut)
        Open_File_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
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
        Compile_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Compile", nullptr));
#if QT_CONFIG(shortcut)
        Compile_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+K", nullptr));
#endif // QT_CONFIG(shortcut)
        Morph_Menu_Item->setText(QCoreApplication::translate("MainWindow", "Morph", nullptr));
#if QT_CONFIG(shortcut)
        Morph_Menu_Item->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+M", nullptr));
#endif // QT_CONFIG(shortcut)
        About_Menu_Item->setText(QCoreApplication::translate("MainWindow", "About Us", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "C0d3z here", nullptr));
        Payload_RButton_Group->setTitle(QCoreApplication::translate("MainWindow", "Payloadz", nullptr));
        Calculator_Payload_RButton->setText(QCoreApplication::translate("MainWindow", "Pop Calc.exe", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Payload X", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "Payload Y", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "Payload Z", nullptr));
        Compile_Button->setText(QCoreApplication::translate("MainWindow", "Compile", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Program Created By : Yoges And Friends", nullptr));
        Morph_Button->setText(QCoreApplication::translate("MainWindow", "M0rph", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Cod3z), QCoreApplication::translate("MainWindow", "C0d3z", nullptr));
        Original_Executable_Label->setText(QString());
        Morphed_Executable_Label->setText(QString());
        Run_Original_Button->setText(QCoreApplication::translate("MainWindow", "Run Original", nullptr));
        Run_Morphed_Button->setText(QCoreApplication::translate("MainWindow", "Run Morphed", nullptr));
        Clear_Output_Log_Button->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        View_Executable_Size_Button->setText(QCoreApplication::translate("MainWindow", "View Size", nullptr));
        View_Executable_Signature_Button->setText(QCoreApplication::translate("MainWindow", "View Signature", nullptr));
        View_Disassembly_Button->setText(QCoreApplication::translate("MainWindow", "View Disassembly", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Analysis), QCoreApplication::translate("MainWindow", "Analysis", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Program Created By : CHIMERA", nullptr));
        File_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        Edit_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        Morph_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "Morph", nullptr));
        About_Menu_Header->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
