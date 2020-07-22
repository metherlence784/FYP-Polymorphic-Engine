﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exit_application_controller.h"
#include "create_new_file_controller.h"
#include "save_as_new_file_controller.h"
#include "save_current_file_controller.h"
#include "open_existing_file_controller.h"
#include "undo_action_controller.h"
#include "redo_action_controller.h"
#include "compile_code_controller.h"
#include "view_compilation_status_controller.h"
#include "morph_executable_controller.h"
#include "run_executable_controller.h"
#include "clear_output_log_controller.h"
#include "view_about_information_controller.h"
#include "view_executable_sizes_controller.h"
#include "view_executable_signatures_controller.h"


#include <stdio.h>
#include <keystone.h>

#define CODE "INC ecx; DEC edx"

//set pointer to null first
MainWindow* MainWindow::MWptr = nullptr;

//constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MWptr = this;
    this->cur_file_path = "";
    this->original_exe_file_path = "";

}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//accesors below
//get mainwindow ptr
MainWindow* MainWindow::getMWptr()
{
    return MWptr;
}

QString MainWindow::get_cur_file_path()
{
    return this->cur_file_path;
}

QString MainWindow::get_original_exe_file_path()
{
    return this->original_exe_file_path;
}

QString MainWindow::get_morphed_exe_file_path()
{
    return this->morphed_exe_file_path;
}

QString MainWindow::get_original_exe_name()
{
    return this->original_exe_name;
}

QString MainWindow::get_morphed_exe_name()
{
    return this->morphed_exe_name;
}

QString MainWindow::get_text_code_input_textbox()
{
    return this->ui->Code_Input_Textbox->toPlainText();
}

QString MainWindow::get_text_analysis_textbox()
{
    return this->ui->Analysis_Textbox->toPlainText();
}

QString MainWindow::get_payload_radio_button()
{
    //save all the radio button objects under Payload_RButton_Group parent into a List
    QList<QRadioButton *> buttons = this->ui->Payload_RButton_Group->findChildren<QRadioButton *>();

    //cycle through each with an enhanced for loop, if isChecked is true, return the object name as a QString
    foreach(QRadioButton *r, buttons)
    {
        if(r->isChecked())
        {
            return QString(r->objectName());
        }
    }

    return ERROR_PAYLOAD_NOT_FOUND;
}

//mutator below
void MainWindow::set_cur_file_path(QString file_path)
{
    this->cur_file_path = file_path;
}

void MainWindow::set_original_exe_file_path(QString original_exe_file_path)
{
    this->original_exe_file_path = original_exe_file_path;
}

void MainWindow::set_morphed_exe_file_path(QString morphed_exe_file_path)
{
    this->morphed_exe_file_path = morphed_exe_file_path;
}

void MainWindow::set_morphed_exe_name(QString morphed_exe_name)
{
    this->morphed_exe_name = morphed_exe_name;
}

void MainWindow::set_original_exe_name(QString original_exe_name)
{
    this->original_exe_name = original_exe_name;
}

void MainWindow::set_enabled_code_input_textbox(bool set)
{
    this->ui->Code_Input_Textbox->setEnabled(set);
}

void MainWindow::set_enabled_payload_groupbox(bool set)
{
    this->ui->Payload_RButton_Group->setEnabled(set);
}

void MainWindow::set_enabled_compile_button(bool set)
{
    this->ui->Compile_Button->setEnabled(set);
}

void MainWindow::set_enabled_analysis_textbox(bool set)
{
    this->ui->Analysis_Textbox->setEnabled(set);
}

void MainWindow::set_text_code_input_textbox(QString txt)
{
    this->ui->Code_Input_Textbox->setText(txt);
}

void MainWindow::set_text_analysis_textbox(QString txt)
{
    this->ui->Analysis_Textbox->setText(txt);
}

void MainWindow::set_original_executable_label(QString original_exe_name)
{
    this->ui->Original_Executable_Label->setText(original_exe_name);
}

void MainWindow::set_morphed_executable_label(QString morphed_exe_name)
{
    this->ui->Morphed_Executable_Label->setText(morphed_exe_name);
}

//button triggers below here
void MainWindow::on_Exit_Menu_Item_triggered()
{
    Exit_Application_Controller *my_exit = new Exit_Application_Controller();
    my_exit->exit_application();
}

void MainWindow::on_New_File_Menu_Item_triggered()
{
    Create_New_File_Controller *my_new = new Create_New_File_Controller();
    my_new->create_new_file();
}

void MainWindow::on_Save_As_Menu_Item_triggered()
{
    Save_As_New_File_Controller *my_save_as = new Save_As_New_File_Controller();
    my_save_as->save_as_new_file();
}

void MainWindow::on_Save_Menu_Item_triggered()
{
    Save_Current_File_Controller *my_save = new Save_Current_File_Controller();
    my_save->save();
}

void MainWindow::on_Open_File_Menu_Item_triggered()
{
    Open_Existing_File_Controller *my_open = new Open_Existing_File_Controller();
    my_open->open_existing_file();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    Exit_Application_Controller *my_exit = new Exit_Application_Controller();
    my_exit->exit_application(event);
}


void MainWindow::on_Undo_Menu_Item_triggered()
{
    Undo_Action_Controller *my_undo = new Undo_Action_Controller();
    my_undo -> undo_action();
}

void MainWindow::on_Redo_Menu_Item_triggered()
{
    Redo_Action_Controller *my_redo = new Redo_Action_Controller();
    my_redo -> redo_action();
}

void MainWindow::on_Compile_Button_clicked()
{
    Compile_Code_Controller *compiler = new Compile_Code_Controller();
    compiler->get_file_name();
    compiler->get_file_path();
    compiler->set_exe_name();//the exe will be filename_original.exe
    compiler->compile_code();//the log is temp_compile.txt

    QString original_exe_file_path = compiler->get_exe_file_path();
    QString original_exe_name = compiler->get_exe_name();

    set_original_exe_file_path(original_exe_file_path);
    set_original_exe_name(original_exe_name);

       //to update analysis textbox
    QString analysis_textbox_status = compiler->get_analysis_textbox_status();
    qint64 elapsed_time = compiler->get_elapsed_time();
    QString temp_compile = compiler->get_temp_compile();

    View_Compilation_Status_Controller *viewer = new View_Compilation_Status_Controller();
    viewer->update_analysis_textbox_and_enable_morph(analysis_textbox_status,elapsed_time,temp_compile);

    QString compilation_status = viewer->get_compilation_status();

    Run_Executable_Controller *runner = new Run_Executable_Controller();
    runner->set_original_exe_name(original_exe_name);
    runner->set_original_executable_label(compilation_status);

}

void MainWindow::on_Compile_Menu_Item_triggered()
{
    Compile_Code_Controller *compiler = new Compile_Code_Controller();
    compiler->get_file_name();
    compiler->get_file_path();
    compiler->set_exe_name();//the exe will be filename_original.exe
    compiler->compile_code();//the log is temp_compile.txt

    QString original_exe_file_path = compiler->get_exe_file_path();
    QString original_exe_name = compiler->get_exe_name();

    set_original_exe_file_path(original_exe_file_path);
    set_original_exe_name(original_exe_name);

    //to update analysis textbox
    QString status = compiler->get_analysis_textbox_status();
    qint64 elapsed_time = compiler->get_elapsed_time();
    QString temp_compile = compiler->get_temp_compile();

    View_Compilation_Status_Controller *viewer = new View_Compilation_Status_Controller();
    viewer->update_analysis_textbox_and_enable_morph(status,elapsed_time,temp_compile);

    QString compilation_status = viewer->get_compilation_status();

    Run_Executable_Controller *runner = new Run_Executable_Controller();
    runner->set_original_exe_name(original_exe_name);
    runner->set_original_executable_label(compilation_status);

}

void MainWindow::on_Morph_Menu_Item_triggered()
{
    Morph_Executable_Controller *morpher = new Morph_Executable_Controller();
    QString morph_status = morpher->morph_exe_with_encryption_junk_alt_instructions(this->original_exe_file_path);

    QString morphed_exe_name = morpher->get_morphed_exe_name();
    QString morphed_exe_file_path = morpher->get_morphed_exe_file_path();

    set_morphed_exe_name(morphed_exe_name);
    set_morphed_exe_file_path(morphed_exe_file_path);

    qint64 elapsed_time = morpher->get_elapsed_time();
    QString analysis_textbox_status = morpher->get_analysis_textbox_status();
    morpher->update_analysis_textbox(analysis_textbox_status,elapsed_time,morph_status);

    Run_Executable_Controller *runner = new Run_Executable_Controller();
    runner->set_morphed_exe_name(morphed_exe_name);
    runner->set_morphed_executable_label(morph_status);

}

void MainWindow::on_Morph_Button_clicked()
{

    Morph_Executable_Controller *morpher = new Morph_Executable_Controller();
    QString morph_status = morpher->morph_exe_with_encryption_junk_alt_instructions(this->original_exe_file_path);
    //QString morph_status = morpher->morph_exe_with_encryption(this->original_exe_file_path);

    QString morphed_exe_name = morpher->get_morphed_exe_name();
    QString morphed_exe_file_path = morpher->get_morphed_exe_file_path();

    set_morphed_exe_name(morphed_exe_name);
    set_morphed_exe_file_path(morphed_exe_file_path);

    qint64 elapsed_time = morpher->get_elapsed_time();
    QString analysis_textbox_status = morpher->get_analysis_textbox_status();
    morpher->update_analysis_textbox(analysis_textbox_status,elapsed_time,morph_status);

    Run_Executable_Controller *runner = new Run_Executable_Controller();
    runner->set_morphed_exe_name(morphed_exe_name);
    runner->set_morphed_executable_label(morph_status);

}

void MainWindow::on_Run_Original_Button_clicked()
{
    Run_Executable_Controller *runner = new Run_Executable_Controller();
    runner->run_original_executable(this->original_exe_file_path);
}

void MainWindow::on_Run_Morphed_Button_clicked()
{
    Run_Executable_Controller *runner = new Run_Executable_Controller();
    runner->run_morphed_executable(this->morphed_exe_file_path);
}

void MainWindow::on_Clear_Output_Log_Button_clicked()
{
    Clear_Output_Log_Controller *clearer = new Clear_Output_Log_Controller();
    clearer->clear_output_log();
}

void MainWindow::on_About_Menu_Item_triggered()
{
    View_About_Information_Controller *abouter = new View_About_Information_Controller();
    abouter->view_about_us();
}

void MainWindow::on_View_Executable_Size_Button_clicked()
{
    View_Executable_Sizes_Controller *sizer = new View_Executable_Sizes_Controller();
    sizer->set_executable_sizes_in_analysis_textbox(this->original_exe_file_path,this->original_exe_name
                                                    ,this->morphed_exe_file_path,this->morphed_exe_name);
}

void MainWindow::on_View_Executable_Signature_Button_clicked()
{
    View_Executable_Signatures_Controller *signer = new View_Executable_Signatures_Controller();
    signer->set_executable_signature_in_analysis_textbox(this->original_exe_file_path,this->original_exe_name
                                                         ,this->morphed_exe_file_path,this->morphed_exe_name);
}
