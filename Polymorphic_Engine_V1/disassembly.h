#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "PE32.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

class Disassembly
{

    friend std::ostream &operator<<(std::ostream &os, Disassembly &d); //cout

public:
    Disassembly();
    ~Disassembly();

    Disassembly(int id, int address, std::string mnemonic, std::string ops, int size, unsigned char byte_array[]);

    int get_id();
    //std::string get_address();
    int get_address();
    std::string get_mnemonic();
    std::string get_ops();
    std::string get_full_instruction();
    int get_size();
    std::vector<unsigned char> get_bytes_vec();
    int get_twos_complement_offset();
    bool get_not_in_jump(); // this will return if it is not in a jump, set to true be default first
    bool get_modified_status();


    void set_id(int id);
    void set_address(int address);
    void set_mnemonic(std::string mnemonic);
    void set_ops(std::string ops);
    void set_full_instruction(std::string full_instruction);
    void set_size(int size);
    void set_bytes_vec(std::vector<unsigned char> bytes_vec);
    void set_twos_complement_offset(int twos_complement_offset);
    void set_not_in_jump(bool not_in_jump);
    void set_modified_true();

    //functions to interface with the vector
    void clear_byte_vec();
    unsigned int convert_string_to_hex(std::string hex_str);
    std::string convert_byte_to_string(char byte);


private:
    int id;
    int address;
    std::string mnemonic;//jmp instruction
    std::string ops;//value of the jump
    std::string full_instruction;
    int size;
    std::vector<unsigned char> bytes_vec;
    int twos_complement_offset;
    bool not_in_jump;// this will return if it is not in a jump, set to true be default first
    bool has_been_modified; // this is so that the instructions will not get alternated more than once, set to false be default


    int calculate_twos_complement_offset_char(signed char offset);

};

#endif // DISASSEMBLY_H
