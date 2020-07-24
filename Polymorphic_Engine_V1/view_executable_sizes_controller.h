#ifndef VIEW_EXECUTABLE_SIZES_H
#define VIEW_EXECUTABLE_SIZES_H

#include <QMainWindow>
#include <QObject>
#include <iostream>

#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "file_reader.h"

class View_Executable_Sizes_Controller : public QObject
{
    Q_OBJECT
public:
    View_Executable_Sizes_Controller();//constructor
    ~View_Executable_Sizes_Controller();//destructor

    //accessor
    MainWindow *get_cur_wind();
	QString get_text_analysis_textbox();


    //mutator
    void set_cur_wind(MainWindow *cur);
	void set_text_analysis_textbox(QString txt);


    void set_enabled_view_executable_size_button(QString compilation_status);
    void set_executable_sizes_in_analysis_textbox(QString original_exe_file_path,
                                    QString original_exe_name,
                                    QString morphed_exe_file_path,
                                    QString morphed_exe_name);
    QString convert_b_to_kb(qint64 exe_size);
    bool check_if_file_exist(qint64 exe_size);
    QString format_text(QString exe_name, qint64 exe_size);


private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui

};

#endif // WRITE_CODE_IN_C_CPP_CONTROLLER_H
