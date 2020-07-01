#include "morph_executable_controller.h"
#include "file_reader.h"
#include <stdio.h>

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
}

Morph_Executable_Controller::~Morph_Executable_Controller()
{
    this->cur_wind = nullptr;
    delete this->cur_wind;

    this->dos_header_pointer = nullptr;
    delete this->dos_header_pointer;

    this->image_NT_header_ptr = nullptr;
    delete this->image_NT_header_ptr;
}

QString Morph_Executable_Controller::morph_exe_no_encryption(QString exe_file_path)
{
    //reading bytes of a exe file
    qDebug() << "STARTING MORPH ";
    if (read_file_into_vector(exe_file_path) == ERROR_INVALID_EXECUTABLE)
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid exe file: " + exe_file_path);
        return ERROR_INVALID_EXECUTABLE;
    }
    //get image dos header pointer
    //image_dos_header_file_cursor is at offset 0 as this pe header is located at the first offset of the exe
    this->dos_header_pointer = get_ptr_image_dos_header(this->buffer,this->image_dos_header_file_cursor);

    //validate the image dos header must be equals to MZ(ASCII)
    if(!validate_image_dos_signature(dos_header_pointer))
    {
        QMessageBox::warning(cur_wind, "Warning",
                             "Invalid DOS Signature");

        return ERROR_INVALID_DOS_SIGNATURE;
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

        return ERROR_INVALID_PE_SIGNATURE;
    }

    printf("Before: Number of Sections : 0x%x\n", this->image_NT_header_ptr->FileHeader.NumberOfSections);
    //increasing the number of sections to allocate memory for payload
    this->image_NT_header_ptr->FileHeader.NumberOfSections += 1;
    printf("After: Number of Sections : 0x%x\n", this->image_NT_header_ptr->FileHeader.NumberOfSections);

    //randomizing the new payload section name
    std::vector<BYTE> new_payload_section_name_vec;
    randomize_payload_section_name(new_payload_section_name_vec);

    std::cout << std::endl;

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

void Morph_Executable_Controller::randomize_payload_section_name(std::vector<BYTE> &new_payload_section_name_vec)
{
    srand(time(NULL));
    std::string alpha_num =  "0123456789"; // numbers
    alpha_num +=             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // uppercase letters
    alpha_num +=             "abcdefghijklmnopqrstuvwxyz"; // lowercase letters

    //new payload section must start with '.' character
    new_payload_section_name_vec.emplace_back('.');

    // randomization here
    for (int i = 1; i < IMAGE_SIZEOF_SHORT_NAME; i++)
    {
        new_payload_section_name_vec.emplace_back(alpha_num[rand() % (alpha_num.length())]);
    }
}
