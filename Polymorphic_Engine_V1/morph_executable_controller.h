#ifndef MORPH_EXECUTABLE_CONTROLLER_H
#define MORPH_EXECUTABLE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>

#include "PE32.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <keystone.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <intrin.h>

#include "ui_mainwindow.h"
#include "mainwindow.h"

class Morph_Executable_Controller : public QObject
{
    Q_OBJECT
public:
     Morph_Executable_Controller();
     ~Morph_Executable_Controller();

     //accessor
     QString get_file_name();
     QString get_file_path();
     MainWindow *get_cur_wind();

     //mutator
     void set_cur_wind(MainWindow *cur);
     void set_exe_name();
     void set_enabled_compile_button(bool set);
     void set_enabled_analysis_textbox(bool set);

     QString morph_exe_no_encryption(QString exe_file_path);



private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString file_name;
    QString file_path;
    QString exe_name;
    std::vector<char> buffer;

    //pe file stuff
    PIMAGE_DOS_HEADER dos_header_pointer;
    PIMAGE_NT_HEADERS32 image_NT_header_ptr;
    unsigned int image_dos_header_file_cursor;
    unsigned int image_NT_header_file_cursor;
    LONG exe_header_file_offset;

    QString read_file_into_vector(QString exe_file_path);
    PIMAGE_DOS_HEADER get_ptr_image_dos_header(std::vector<char> &buffer, unsigned int image_dos_header_file_cursor);
    bool validate_image_dos_signature(PIMAGE_DOS_HEADER &dos_header_pointer);
    PIMAGE_NT_HEADERS32 get_ptr_image_NT_header(std::vector<char> &buffer, unsigned int image_NT_header_file_cursor);
    bool validate_PE_signature(PIMAGE_NT_HEADERS32 &image_NT_header_ptr);
    void randomize_payload_section_name(std::vector<BYTE> &new_payload_section_name_vec) ;

};

#endif // MORPH_EXECUTABLE_CONTROLLER_H
