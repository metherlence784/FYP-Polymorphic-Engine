#include "mainwindow.h"
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

//set pointer to null first
MainWindow* MainWindow::MWptr = nullptr;

//constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MWptr = this;
    cur_file_path = "";
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
void MainWindow::set_cur_file_path(QString str)
{
    this->cur_file_path = str;
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

       //to update analysis textbox
    QString status = compiler->get_status();
    qint64 elapsed_time = compiler->get_elapsed_time();
    QString temp_compile = compiler->get_temp_compile();

    View_Compilation_Status_Controller *viewer = new View_Compilation_Status_Controller();
    viewer->update_analysis_textbox(status,elapsed_time,temp_compile);

}

void MainWindow::on_Compile_Menu_Item_triggered()
{
    Compile_Code_Controller *compiler = new Compile_Code_Controller();
    compiler->get_file_name();
    compiler->get_file_path();
    compiler->set_exe_name();//the exe will be filename_original.exe
    compiler->compile_code();//the log is temp_compile.txt

    //to update analysis textbox
    QString status = compiler->get_status();
    qint64 elapsed_time = compiler->get_elapsed_time();
    QString temp_compile = compiler->get_temp_compile();

    View_Compilation_Status_Controller *viewer = new View_Compilation_Status_Controller();
    viewer->update_analysis_textbox(status,elapsed_time,temp_compile);
}
