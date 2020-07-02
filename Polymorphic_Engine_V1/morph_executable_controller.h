#ifndef MORPH_EXECUTABLE_CONTROLLER_H
#define MORPH_EXECUTABLE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QFile>

#include "PE32.h"
#include "template_functions.h"
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

    //list of payloads
    const std::vector<QString> PAYLOADS_VEC = {"Calculator_Payload_RButton"};

    //pe file stuff
    const std::string TEXT_SECTION_NAME = ".text";
    const DWORD SECTIONCHARACTERISTICSTOSET = 0xE0000020; //for read/write/execute

    std::vector<char> buffer;
    std::vector<PIMAGE_SECTION_HEADER> image_section_header_vec;
    PIMAGE_SECTION_HEADER text_section_header_ptr;
    PIMAGE_SECTION_HEADER payload_section_header_ptr;
    PIMAGE_DOS_HEADER dos_header_pointer;
    PIMAGE_NT_HEADERS32 image_NT_header_ptr;
    unsigned int image_dos_header_file_cursor;
    unsigned int image_NT_header_file_cursor;
    LONG exe_header_file_offset;
    DWORD entry_point_of_text_section;
    DWORD file_alignment;
    DWORD section_alignment;
    DWORD text_section_raw_data_offset;
    unsigned int size_of_text_section;
    int index_of_text_section;
    int index_of_payload_section;
    unsigned int payload_num_of_bytes;
    unsigned int file_alignment_factor;
    unsigned int payload_raw_data_size;
    unsigned int section_alignment_factor;
    unsigned int payload_virtual_size;
    unsigned int payload_raw_address;
    unsigned int payload_virtual_address;
    size_t machine_code_num_of_bytes;
    std::vector<unsigned char> machine_code_vec;
    DWORD entry_point_to_patch;
    DWORD image_base;
    unsigned int section_pivot_gadget_byte_length;
    char* text_section_buffer_original;
    char* text_section_buffer_ptr;

    QString read_file_into_vector(QString exe_file_path);
    PIMAGE_DOS_HEADER get_ptr_image_dos_header(std::vector<char> &buffer, unsigned int image_dos_header_file_cursor);
    bool validate_image_dos_signature(PIMAGE_DOS_HEADER &dos_header_pointer);
    PIMAGE_NT_HEADERS32 get_ptr_image_NT_header(std::vector<char> &buffer, unsigned int image_NT_header_file_cursor);
    bool validate_PE_signature(PIMAGE_NT_HEADERS32 &image_NT_header_ptr);
    std::string randomize_payload_section_name();
    PIMAGE_SECTION_HEADER get_ptr_image_section_header(std::vector<char> &buffer, unsigned int section_header_cursor);
    void store_image_section_headers_in_vec(std::vector<char> &buffer, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec,
                                            PIMAGE_NT_HEADERS32& image_NT_header_ptr, unsigned int section_header_cursor);
    QString get_section_header_index_by_name(const std::string section_name, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec,
                                         PIMAGE_NT_HEADERS32 &image_NT_header_ptr,int &index_of_text_section);
    void set_new_section_name(PIMAGE_SECTION_HEADER &payload_section,const std::string payload_section_name);
    int get_payload_radio_button();
    void calculate_file_alignment_factor (unsigned int &payload_num_of_bytes,
                                          DWORD &file_alignment, unsigned int &file_alignment_factor);
    void calculate_section_alignment_factor(unsigned int &payload_num_of_bytes,
                                            DWORD &section_aligment,unsigned int &section_alignment_factor,
                                            PIMAGE_SECTION_HEADER &previous_section_header_ptr);
    void init_payload_section_header(PIMAGE_SECTION_HEADER &payload_section_header_ptr,
                                     unsigned int &payload_raw_address,
                                     unsigned int &payload_raw_data_size,
                                     unsigned int &payload_virtual_address,
                                     unsigned int &payload_virtual_size,
                                     DWORD SECTIONCHARACTERISTICSTOSET);
    void print_section_headers(std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec);
    QString asm_to_machine_code(const char * code, std::vector<unsigned char>& machine_code_vec, size_t &machine_code_num_of_bytes);
    char * get_section_data_from_buffer(std::vector <char> &buffer, unsigned int file_offset, unsigned int size_of_section);

};

#endif // MORPH_EXECUTABLE_CONTROLLER_H
