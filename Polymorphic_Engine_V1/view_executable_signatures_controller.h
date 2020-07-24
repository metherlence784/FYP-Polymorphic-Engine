#ifndef VIEW_EXECUTABLE_SIGNATURES_CONTROLLER_H
#define VIEW_EXECUTABLE_SIGNATURES_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QCryptographicHash>
#include <QByteArray>
#include <iostream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "file_reader.h"

class View_Executable_Signatures_Controller : public QObject
{
    Q_OBJECT
public:
     View_Executable_Signatures_Controller();
     ~View_Executable_Signatures_Controller();

     //accessor
     MainWindow *get_cur_wind();
     QString get_text_analysis_textbox();


     //mutator
     void set_cur_wind(MainWindow *cur);
     void set_text_analysis_textbox(QString txt);

     void set_executable_signature_in_analysis_textbox(QString original_exe_file_path,
                                     QString original_exe_name,
                                     QString morphed_exe_file_path,
                                     QString morphed_exe_name);
     bool check_if_file_exist(QString status);
     QString hash_buffer_MD5(std::vector<char> buffer, QString status);
     QString format_text(QString hash, QString exe_name, QString status);
     QString compare_hashes(QString original_hash, QString morphed_hash);
     void set_enabled_view_executable_signature_button(QString compilation_status);

 private:

     MainWindow *cur_wind; // variable pointer for mainwindow class to access ui



};

#endif // VIEW_EXECUTABLE_SIGNATURES_CONTROLLER_H
