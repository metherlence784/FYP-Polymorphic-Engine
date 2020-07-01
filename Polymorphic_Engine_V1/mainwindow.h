#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include "error.h"

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
    QString get_exe_file_path();
    QString get_text_code_input_textbox();
    QString get_text_analysis_textbox();
    QString get_payload_radio_button();

    //mutator
    void set_cur_file_path(QString file_path);
    void set_enabled_code_input_textbox(bool set);
    void set_enabled_payload_groupbox(bool set);
    void set_enabled_compile_button(bool set);
    void set_enabled_analysis_textbox(bool set);
    void set_text_code_input_textbox(QString txt);
    void set_text_analysis_textbox(QString txt);
    void set_exe_file_path(QString exe_file_path);

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

    void on_actionMorph_triggered();

private:
    static MainWindow *MWptr;
    QString cur_file_path;
    QString exe_file_path;

    void closeEvent (QCloseEvent *event);//this is to overload the top right X button of the window
};
#endif // MAINWINDOW_H
