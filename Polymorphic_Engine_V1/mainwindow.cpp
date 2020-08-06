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
#include "morph_executable_controller.h"
#include "run_executable_controller.h"
#include "clear_output_log_controller.h"
#include "view_about_information_controller.h"
#include "view_executable_sizes_controller.h"
#include "view_executable_signatures_controller.h"
#include "view_disassembly_controller.h"




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

    this->payload_group.addButton(this->ui->Calculator_Payload_RButton,0);
    this->payload_group.addButton(this->ui->CMD_Payload_RButton,1);
    this->payload_group.addButton(this->ui->Message_Box_Payload_RButton,2);
    this->payload_group.addButton(this->ui->Fatality_Payload_RButton,3);
    this->payload_group.addButton(this->ui->System_Info_Payload_RButton,4);
    this->payload_group.addButton(this->ui->Download_Putty_Payload_RButton,5);
    this->payload_group.addButton(this->ui->New_Admin_Payload_RButton,6);

    this->payload_group.setExclusive(true);
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

int MainWindow::get_payload_radio_button()
{
//    //save all the radio button objects under Payload_RButton_Group parent into a List
//    QList<QRadioButton *> buttons = this->ui->Payload_RButton_Group->findChildren<QRadioButton *>();

//    //cycle through each with an enhanced for loop, if isChecked is true, return the object name as a QString
//    foreach(QRadioButton *r, buttons)
//    {
//        if(r->isChecked())
//        {
//            this->ui->Payload_Info_Textbox->setText(QString::number(this->payload_group.checkedId()));
//            return ;
//        }
//    }
    return this->payload_group.checkedId();


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

void MainWindow::set_enabled_run_original_button(bool set)
{
    this->ui->Run_Original_Button->setEnabled(set);
}

void MainWindow::set_enabled_run_morphed_button(bool set)
{
    this->ui->Run_Morphed_Button->setEnabled(set);
}

void MainWindow::set_enabled_view_executable_signature_button(bool set)
{
    this->ui->View_Executable_Signature_Button->setEnabled(set);
}

void MainWindow::set_enabled_view_executable_size_button(bool set)
{
    this->ui->View_Executable_Size_Button->setEnabled(set);
}

void MainWindow::set_enabled_view_disassembly_button(bool set)
{
    this->ui->View_Disassembly_Button->setEnabled(set);
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

void MainWindow::set_disassembly_log(QString disassembly_log)
{
    this->disassembly_log = disassembly_log;
}

void MainWindow::set_text_and_color_payload_info_textbox(QString text, QColor color)
{
    this->ui->Payload_Info_Textbox->setTextColor(color);
    this->ui->Payload_Info_Textbox->setText(text);
}

//button triggers below here
void MainWindow::on_Exit_Menu_Item_triggered()
{
    Exit_Application_Controller exiter;
    exiter.exit_application();
}

void MainWindow::on_New_File_Menu_Item_triggered()
{
    Create_New_File_Controller newer;
    newer.create_new_file();
}

void MainWindow::on_Save_As_Menu_Item_triggered()
{
    Save_As_New_File_Controller saver;
    saver.save_as_new_file();
}

void MainWindow::on_Save_Menu_Item_triggered()
{
    Save_Current_File_Controller saver;
    saver.save();
}

void MainWindow::on_Open_File_Menu_Item_triggered()
{
    Open_Existing_File_Controller opener;
    opener.open_existing_file();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    Exit_Application_Controller exiter;
    exiter.exit_application(event);
}


void MainWindow::on_Undo_Menu_Item_triggered()
{
    Undo_Action_Controller undoer;
    undoer.undo_action();
}

void MainWindow::on_Redo_Menu_Item_triggered()
{
    Redo_Action_Controller redoer;
    redoer.redo_action();
}

void MainWindow::on_Compile_Button_clicked()
{
    Compile_Code_Controller compiler;
    compiler.get_file_name();
    compiler.get_file_path();
    compiler.set_exe_name();//the exe will be filename_original.exe
    compiler.compile_code();//the log is temp_compile.txt

    QString original_exe_file_path = compiler.get_exe_file_path();
    QString original_exe_name = compiler.get_exe_name();
	
	//setting the variables
    set_original_exe_file_path(original_exe_file_path);
    set_original_exe_name(original_exe_name);

    //to update analysis textbox
    QString analysis_textbox_status = compiler.get_analysis_textbox_status();
    qint64 elapsed_time = compiler.get_elapsed_time();
    QString temp_compile = compiler.get_temp_compile();

    View_Compilation_Status_Controller viewer;
    viewer.update_analysis_textbox_and_enable_morph(analysis_textbox_status,elapsed_time,temp_compile);

    QString compilation_status = viewer.get_compilation_status();

    Choose_Payload_Controller payloader;
    payloader.set_enabled_payload_groupbox(compilation_status);//enables the payload radio buttons
    payloader.set_initial_payload_info(compilation_status);//sets the initial calc.exe info

	//enable the run original button
    Run_Executable_Controller runner;
    runner.set_original_exe_name(original_exe_name);
    runner.set_original_executable_label(compilation_status);
    runner.set_enabled_run_original_button(compilation_status);
	
	//enable the view signature button
    View_Executable_Signatures_Controller signer;
    signer.set_enabled_view_executable_signature_button(compilation_status);
	
	//enabled the view sized button
    View_Executable_Sizes_Controller sizer;
    sizer.set_enabled_view_executable_size_button(compilation_status);
}

void MainWindow::on_Compile_Menu_Item_triggered()
{
    Compile_Code_Controller compiler;
    compiler.get_file_name();
    compiler.get_file_path();
    compiler.set_exe_name();//the exe will be filename_original.exe
    compiler.compile_code();//the log is temp_compile.txt

    QString original_exe_file_path = compiler.get_exe_file_path();
    QString original_exe_name = compiler.get_exe_name();
	
	//setting the variables
    set_original_exe_file_path(original_exe_file_path);
    set_original_exe_name(original_exe_name);

    //to update analysis textbox
    QString analysis_textbox_status = compiler.get_analysis_textbox_status();
    qint64 elapsed_time = compiler.get_elapsed_time();
    QString temp_compile = compiler.get_temp_compile();

    View_Compilation_Status_Controller viewer;
    viewer.update_analysis_textbox_and_enable_morph(analysis_textbox_status,elapsed_time,temp_compile);

    QString compilation_status = viewer.get_compilation_status();
	
    Choose_Payload_Controller payloader;
    payloader.set_enabled_payload_groupbox(compilation_status);//enables the payload radio buttons
    payloader.set_initial_payload_info(compilation_status);//sets the initial calc.exe info

	//enable the run original button
    Run_Executable_Controller runner;
    runner.set_original_exe_name(original_exe_name);
    runner.set_original_executable_label(compilation_status);
    runner.set_enabled_run_original_button(compilation_status);
	
	//enable the view signature button
    View_Executable_Signatures_Controller signer;
    signer.set_enabled_view_executable_signature_button(compilation_status);
	
	//enabled the view sized button
    View_Executable_Sizes_Controller sizer;
    sizer.set_enabled_view_executable_size_button(compilation_status);
}

void MainWindow::on_Morph_Menu_Item_triggered()
{

    Morph_Executable_Controller morpher;
    QString morph_status = morpher.morph_exe_with_encryption_junk_alt_instructions(this->original_exe_file_path);//morphing
    //QString morph_status = morpher.morph_exe_with_encryption(this->original_exe_file_path);//morphing
    //QString morph_status = morpher.morph_exe_no_encryption(this->original_exe_file_path);//morphing

    QString morphed_exe_name = morpher.get_morphed_exe_name();
    QString morphed_exe_file_path = morpher.get_morphed_exe_file_path();
    QString disassembly_log = morpher.get_disassembly_log();//getting the disassebly of the payload
	
    //setting variables
    set_morphed_exe_name(morphed_exe_name);
    set_morphed_exe_file_path(morphed_exe_file_path);
    set_disassembly_log(disassembly_log);
	
    //setting the anaylsis textbox
    qint64 elapsed_time = morpher.get_elapsed_time();
    QString analysis_textbox_status = get_text_analysis_textbox();
    morpher.update_analysis_textbox(analysis_textbox_status,elapsed_time,morph_status);
	
    //enable the run morphed button
    Run_Executable_Controller runner;
    runner.set_morphed_exe_name(morphed_exe_name);
    runner.set_morphed_executable_label(morph_status);
    runner.set_enabled_run_morphed_button(morph_status);
	
    //enabled the view disassebly button
    View_Disassembly_Controller disassembler;
    disassembler.set_enabled_view_disassembly_button(morph_status);
}

void MainWindow::on_Morph_Button_clicked()
{
    Morph_Executable_Controller morpher;
    QString morph_status = morpher.morph_exe_with_encryption_junk_alt_instructions(this->original_exe_file_path);//morphing
    //QString morph_status = morpher.morph_exe_with_encryption(this->original_exe_file_path);//morphing
    //QString morph_status = morpher.morph_exe_no_encryption(this->original_exe_file_path);//morphing

    QString morphed_exe_name = morpher.get_morphed_exe_name();
    QString morphed_exe_file_path = morpher.get_morphed_exe_file_path();
    QString disassembly_log = morpher.get_disassembly_log();//getting the disassebly of the payload
	
	//setting variables
    set_morphed_exe_name(morphed_exe_name);
    set_morphed_exe_file_path(morphed_exe_file_path);
    set_disassembly_log(disassembly_log);
	
	//setting the anaylsis textbox
    qint64 elapsed_time = morpher.get_elapsed_time();
    QString analysis_textbox_status = get_text_analysis_textbox();
    morpher.update_analysis_textbox(analysis_textbox_status,elapsed_time,morph_status);
	
	//enable the run morphed button
    Run_Executable_Controller runner;
    runner.set_morphed_exe_name(morphed_exe_name);
    runner.set_morphed_executable_label(morph_status);
    runner.set_enabled_run_morphed_button(morph_status);
	
	//enabled the view disassebly button
    View_Disassembly_Controller disassembler;
    disassembler.set_enabled_view_disassembly_button(morph_status);
}

void MainWindow::on_Run_Original_Button_clicked()
{
    Run_Executable_Controller runner;
    runner.run_original_executable(this->original_exe_file_path);
}

void MainWindow::on_Run_Morphed_Button_clicked()
{
    Run_Executable_Controller runner;
    runner.run_morphed_executable(this->morphed_exe_file_path);
}

void MainWindow::on_Clear_Output_Log_Button_clicked()
{
    Clear_Output_Log_Controller clearer ;
    clearer.clear_output_log();
}

void MainWindow::on_About_Menu_Item_triggered()
{
    View_About_Information_Controller abouter;
    abouter.view_about_us();
}

void MainWindow::on_View_Executable_Size_Button_clicked()
{
    View_Executable_Sizes_Controller sizer;
    sizer.set_executable_sizes_in_analysis_textbox(this->original_exe_file_path,this->original_exe_name
                                                    ,this->morphed_exe_file_path,this->morphed_exe_name);
}

void MainWindow::on_View_Executable_Signature_Button_clicked()
{
    View_Executable_Signatures_Controller signer ;
    signer.set_executable_signature_in_analysis_textbox(this->original_exe_file_path,this->original_exe_name
                                                         ,this->morphed_exe_file_path,this->morphed_exe_name);
}

void MainWindow::on_View_Disassembly_Button_clicked()
{
    View_Disassembly_Controller disassembler;
    disassembler.write_disassembly(this->cur_file_path,this->disassembly_log);
}


void MainWindow::on_CMD_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}

void MainWindow::on_Calculator_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}

void MainWindow::on_Download_Putty_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}

void MainWindow::on_Fatality_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}

void MainWindow::on_Message_Box_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}

void MainWindow::on_New_Admin_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}

void MainWindow::on_System_Info_Payload_RButton_toggled(bool checked)
{
    if(checked == true)
    {
        int id = this->payload_group.checkedId();
        Choose_Payload_Controller payloader;
        payloader.set_text_and_color_payload_info_textbox(id);
    }
}
