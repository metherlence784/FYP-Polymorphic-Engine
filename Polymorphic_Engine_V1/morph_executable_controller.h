#ifndef MORPH_EXECUTABLE_CONTROLLER_H
#define MORPH_EXECUTABLE_CONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QDateTime>


#include "PE32.h"
#include "payloads.h"
#include "template_functions.h"
#include <capstone.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <keystone.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <regex>

#include <disassembly.h>

#include "file_reader.h"
#include "file_saver.h"
#include "choose_payload_controller.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"


class Morph_Executable_Controller : public QObject
{
    Q_OBJECT
public:
    const int INSERTION_PROBABILITY = 10;

     Morph_Executable_Controller();
     ~Morph_Executable_Controller();

     //accessor
     QString get_file_name();
     QString get_file_path();
     QString get_original_exe_name();
     QString get_morphed_exe_name();
     QString get_morphed_exe_file_path();
     MainWindow *get_cur_wind();
     qint64 get_elapsed_time();
     QString get_analysis_textbox_status();


     //mutator
     void set_cur_wind(MainWindow *cur);
     void set_original_exe_name(QString original_exe_filepath);
     void set_enabled_compile_button(bool set);
     void set_enabled_analysis_textbox(bool set);
     void set_elapsed_time(qint64 elapsed_time);
     void set_analysis_textbox_status(QString status);

     QString morph_exe_no_encryption(QString original_exe_file_path);
     QString morph_exe_with_encryption(QString original_exe_file_path);
     QString morph_exe_with_encryption_junk_alt_instructions(QString original_exe_file_path);
     void set_morphed_exe_file_path(QString original_exe_file_path);
     void set_morphed_exe_name(QString original_exe_file_path, std::string modifier);

     void update_analysis_textbox(QString analysis_textbox_status,
                                  qint64 elapsed_time,
                                  QString morph_status);

     QString get_disassembly_log();

private:
    MainWindow *cur_wind; // variable pointer for mainwindow class to access ui
    QString original_exe_name;
    QString original_exe_file_path;
    QString morphed_exe_name;
    QString morphed_exe_file_path;
    QString disassembly_log;

    //pe file stuff
    const std::string TEXT_SECTION_NAME = ".text";
    const DWORD SECTION_CHARACTERISTICS_TO_SET = 0xE0000020; //for read/write/execute

    std::vector<char> buffer;
    std::vector<PIMAGE_SECTION_HEADER> image_section_header_vec;
    PIMAGE_SECTION_HEADER text_section_header_ptr;
    PIMAGE_SECTION_HEADER payload_section_header_ptr;
    PIMAGE_DOS_HEADER dos_header_ptr;
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
    DWORD starting_of_text_section_offset_on_load;
    DWORD image_base;
    unsigned int jump_to_payload_byte_length;
    char* text_section_buffer_original;
    char* text_section_buffer_ptr;
    unsigned int num_of_stosd;
    std::vector<unsigned char> payload_vec;
    unsigned int buffer_cursor;
    DWORD start_of_payload_section_offset;
    char random_key;
    unsigned int length_to_decrypt;
    std::vector<Disassembly> dis_asm_vec;
    QString morph_status;
    qint64 elapsed_time;

    QString read_file_into_vector(QString original_exe_file_path);
    PIMAGE_DOS_HEADER get_ptr_image_dos_header(std::vector<char> &buffer, unsigned int image_dos_header_file_cursor);
    QString validate_image_dos_signature(PIMAGE_DOS_HEADER &dos_header_ptr);
    PIMAGE_NT_HEADERS32 get_ptr_image_NT_header(std::vector<char> &buffer, unsigned int image_NT_header_file_cursor);
    QString validate_PE_signature(PIMAGE_NT_HEADERS32 &image_NT_header_ptr);
    std::string randomize_payload_section_name();
    PIMAGE_SECTION_HEADER get_ptr_image_section_header(std::vector<char> &buffer, unsigned int section_header_cursor);
    void store_image_section_headers_in_vec(std::vector<char> &buffer, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec,
                                            PIMAGE_NT_HEADERS32& image_NT_header_ptr, unsigned int section_header_cursor);
    QString get_section_header_index_by_name(const std::string section_name, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec,
                                         PIMAGE_NT_HEADERS32 &image_NT_header_ptr,int &index_of_section);
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
                                     DWORD SECTION_CHARACTERISTICS_TO_SET);
    void print_section_headers(std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec);

    QString asm_to_machine_code(std::string asm_code, std::vector<unsigned char>& machine_code_vec, size_t &machine_code_num_of_bytes);
    char * get_section_data_from_buffer(std::vector <char> &buffer, unsigned int file_offset, unsigned int size_of_section);
    void error_warning_message_box(QString morph_status);
    void editing_text_section_ptr(char* &text_section_buffer_ptr,
                                  unsigned int jump_to_payload_byte_length,
                                  DWORD text_section_buffer_offset,
                                  std::vector<unsigned char> &machine_code_vec);
    void calculate_num_of_stosd_for_patching(std::string &text_section_memorized_entry_bytes,
                                             unsigned int &num_of_stosd,
                                             unsigned int jump_to_payload_byte_length,
                                             DWORD starting_of_text_section_offset_on_load,
                                             std::vector<char> &buffer);
    void store_edi(std::string &store_payload_entry_point_in_edi_asm,
                   DWORD entry_point_of_text_section);
    void get_stosd_instruction_asm(std::string &patching_entry_bytes_asm,
                                   std::string text_section_memorized_entry_bytes,
                                   unsigned int num_of_stosd);

    void populate_section_ptr(char *&section_ptr, char *machine_code, unsigned int size_of_machine_code);
    void populate_payload_vec(std::vector<unsigned char> &payload_vec, const char payload_shell_code[],unsigned int payload_num_of_bytes );
    std::string convert_byte_to_string(char byte);

    void get_jump_back_to_text_section_instruction_asm(std::string &jump_back_to_text_section_asm,
                                                       DWORD entry_point_of_text_section);
    void rewrite_bytes_to_buffer(std::vector<char> &buffer, char *section_buffer_ptr,
                                 unsigned int offset, unsigned int byte_size);
    void write_exe_file(QString morphed_exe_file_path, std::vector<char> &buffer);
    char generate_random_key();
    void add_bytes_into_vec(std::vector<unsigned char> &bytes_after_decryption_instructions_vec,
                            std::vector<unsigned char> machine_code_vec);

    void get_decryption_asm(std::string &decrypt_asm,
                            DWORD start_of_payload_section_offset,
                            unsigned int length_of_random_key,
                            std::vector<unsigned char> &machine_code_vec,
                            size_t &machine_code_num_of_bytes,
                            unsigned int size_of_bytes_after_decryption_instructions_vec);

    void encrypt_bytes_after_decryption_instruction_vec(std::vector<unsigned char> &bytes_after_decryption_instructions_vec,
                                                        char random_key);

    void add_random_key_to_payload_section_buffer_ptr(char *&payload_section_buffer_ptr, char random_key );
	void calculate_jne_short_backwards(std::vector<unsigned char> &machine_code_vec, unsigned int length_of_jne,
										size_t &machine_code_num_of_bytes);
    //QString machine_code_to_asm(std::vector<unsigned char> temp, std::vector<std::string> &dis_asm_vec);
    QString machine_code_to_asm(std::vector<unsigned char> payload_vec, std::vector<Disassembly> &dis_asm_vec);

    //============================================= for alternate instructions ==================================
    void modify_payload_vec_with_alternative_instructions(std::vector<unsigned char> &payload_vec,
                                                          std::vector<Disassembly> &dis_asm_vec);
    void check_for_relative_jumps(std::vector<Disassembly> &dis_asm_vec);
    void check_not_in_jump(std::vector<Disassembly> &dis_asm_vec, int index);
    void alternative_inc_or_dec_instruction(std::vector<Disassembly> &dis_asm_vec,
                                            std::vector<unsigned char> &machine_code_vec,
                                            size_t &machine_code_num_of_bytes);
    void gen_new_machine_code(Disassembly &instruction,
                              std::string new_instruction,
                              std::vector<unsigned char> &machine_code_vec,
                              size_t &machine_code_num_of_bytes);
    std::string convert_decimal_to_hexa(int num);
    bool check_instruction_is_ptr(std::string ops);
    std::string generate_alternative_sub_instructions(std::string original_instruction,
                                                      std::string right_op,
                                                      std::string left_op,
                                                      bool right_op_is_reg,
                                                      unsigned int &right_op_hex);
    void alternative_sub_instruction(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<unsigned char> &machine_code_vec,
                                     size_t &machine_code_num_of_bytes);
    std::string generate_alternative_add_instructions(std::string original_instruction,
                                                      std::string right_op,
                                                      std::string left_op,
                                                      bool right_op_is_reg,
                                                      unsigned int &right_op_hex);
    void alternative_add_instruction(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<unsigned char> &machine_code_vec,
                                     size_t &machine_code_num_of_bytes);
    void alternative_xor_clear_instruction(std::vector<Disassembly> &dis_asm_vec,
                                           std::vector<unsigned char> &machine_code_vec,
                                           size_t &machine_code_num_of_bytes);
    void alternative_xor_general_instruction(std::vector<Disassembly> &dis_asm_vec,
                                             std::vector<unsigned char> &machine_code_vec,
                                             size_t &machine_code_num_of_bytes);
    void alternative_and_instruction(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<unsigned char> &machine_code_vec,
                                     size_t &machine_code_num_of_bytes);
    void alternative_push_instruction(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<unsigned char> &machine_code_vec,
                                     size_t &machine_code_num_of_bytes);
    void alternative_pop_instruction(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<unsigned char> &machine_code_vec,
                                     size_t &machine_code_num_of_bytes);


    //============================================ for junk instructions ====================================
    void add_junk_instructions(std::vector<unsigned char> &payload_vec,
                                    std::vector<Disassembly> &dis_asm_vec,
                                    std::vector<unsigned char> &machine_code_vec,
                                    size_t &machine_code_num_of_bytes,
                                    std::vector<uint64_t> &address_of_insertions_vec,
                                    std::vector<Disassembly> &jump_instructions_vec);

    void add_conditional_jumps_instructions(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<unsigned char> &machine_code_vec,
                                     size_t &machine_code_num_of_bytes);


    bool to_insert_or_not(int chance);//chance to insert
    bool check_is_jump_instruction(int id);
    std::string	generate_conditional_jumps_instructions();
    QString	junk_code_generator(std::vector<unsigned char>& machine_code_vec, size_t &machine_code_num_of_bytes);
    void print_dis_asm_vec(std::vector<Disassembly> &dis_asm_vec); //printing disassembled vector
    void randomize_and_store_insertions(std::vector<uint64_t> &address_of_insertions_vec,
                                        std::vector<Disassembly> &jump_instructions_vec,
                                        std::vector<Disassembly> dis_asm_vec);
    void init_tracker_vec(std::vector<Disassembly> &tracker_vec, std::vector<Disassembly> jump_instructions_vec);
    void calculate_and_adjust_offsets_within_jump_range(std::vector<uint64_t> &address_of_insertions_vec,
                                                        std::vector<Disassembly> &jump_instructions_vec,
                                                        std::vector<Disassembly> tracker_vec,
                                                        size_t &machine_code_num_of_bytes);
    void adjust_offsets_within_jump_range(uint64_t &address_of_insertion,
                                          Disassembly &jump_instruction,
                                          Disassembly &tracker_instruction,
                                          size_t &machine_code_num_of_bytes);
    unsigned char calculate_signed_bit(char original_relative_offset);
    void modify_jumps_in_dis_asm_vec(std::vector<Disassembly> &dis_asm_vec,
                                     std::vector<Disassembly> &jump_instructions_vec);
    void modify_payload_vec_with_junk_instructions(std::vector<uint64_t> &address_of_insertions_vec,
                                                    std::vector<Disassembly> &dis_asm_vec,
                                                    std::vector<unsigned char> &machine_code_vec,
                                                    std::vector<unsigned char> &payload_vec);
    QString concat_disassembly(std::vector<Disassembly> &dis_asm_vec, QString line_header);


};


#endif // MORPH_EXECUTABLE_CONTROLLER_H
