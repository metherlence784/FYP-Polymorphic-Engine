#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include <QButtonGroup>

#include "error.h"
#include <stdio.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;//needed for the other controllers to access the ui object

    //constructor
    MainWindow(QWidget *parent = nullptr);

    //destructor
    ~MainWindow();

    //accesor
    static MainWindow* getMWptr();
    QString get_cur_file_path();
    QString get_original_exe_file_path();
    QString get_morphed_exe_file_path();
    QString get_original_exe_name();
    QString get_morphed_exe_name();
    QString get_text_code_input_textbox();
    QString get_text_analysis_textbox();
    int get_payload_radio_button();

    //mutator
    void set_cur_file_path(QString file_path);
    void set_enabled_code_input_textbox(bool set);
    void set_enabled_payload_groupbox(bool set);
    void set_enabled_compile_button(bool set);
    void set_enabled_analysis_textbox(bool set);
    void set_enabled_run_original_button(bool set);
    void set_enabled_run_morphed_button(bool set);
    void set_enabled_view_executable_signature_button(bool set);
    void set_enabled_view_executable_size_button(bool set);
    void set_enabled_view_disassembly_button(bool set);
    void set_text_code_input_textbox(QString txt);
    void set_text_analysis_textbox(QString txt);
    void set_original_exe_file_path(QString original_exe_file_path);
    void set_morphed_exe_file_path(QString morphed_exe_file_path);
    void set_original_exe_name(QString original_exe_name);
    void set_morphed_exe_name(QString morphed_exe_name);
    void set_disassembly_log(QString disassembly_log);
    void set_original_executable_label(QString original_exe_name);
    void set_morphed_executable_label(QString morphed_exe_name);
    void set_text_and_color_payload_info_textbox(QString text, QColor color);



private slots://note that slots is a QT ui syntax
    void on_Exit_Menu_Item_triggered();
    void on_New_File_Menu_Item_triggered();
    void on_Save_As_Menu_Item_triggered();
    void on_Save_Menu_Item_triggered();
    void on_Open_File_Menu_Item_triggered();

    void on_Undo_Menu_Item_triggered();

    void on_Redo_Menu_Item_triggered();

    void on_Compile_Button_clicked();

    void on_Compile_Menu_Item_triggered();



    void on_Morph_Menu_Item_triggered();

    void on_Morph_Button_clicked();

    void on_Run_Original_Button_clicked();

    void on_Run_Morphed_Button_clicked();

    void on_Clear_Output_Log_Button_clicked();

    void on_About_Menu_Item_triggered();

    void on_View_Executable_Size_Button_clicked();

    void on_View_Executable_Signature_Button_clicked();

    void on_View_Disassembly_Button_clicked();


    void on_CMD_Payload_RButton_toggled(bool checked);

    void on_Calculator_Payload_RButton_toggled(bool checked);

    void on_Download_Putty_Payload_RButton_toggled(bool checked);

    void on_Fatality_Payload_RButton_toggled(bool checked);

    void on_Message_Box_Payload_RButton_toggled(bool checked);

    void on_New_Admin_Payload_RButton_toggled(bool checked);

    void on_System_Info_Payload_RButton_toggled(bool checked);

    void on_Help_Menu_Item_triggered();

private:
    static MainWindow *MWptr;
    QString cur_file_path;
    QString original_exe_file_path;
    QString morphed_exe_file_path;
    QString original_exe_name;
    QString morphed_exe_name;
    QString disassembly_log;

    QButtonGroup payload_group;

    void closeEvent (QCloseEvent *event);//this is to overload the top right X button of the window
};
#endif // MAINWINDOW_H
