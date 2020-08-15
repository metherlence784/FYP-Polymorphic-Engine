#include "disassembly.h"

//constructor
Disassembly::Disassembly()
{
    this->id = 0;
    this->address = 0;
    this->twos_complement_offset = 0;
    this->mnemonic = "";
    this->ops = "";
    this->full_instruction = "";
    this->size = 0;
    this->not_in_jump = true;
    this->has_been_modified = false;
}

//desttructor
Disassembly::~Disassembly()
{

}

//other constructor to store capstone details
Disassembly::Disassembly(int id, int address, std::string mnemonic, std::string ops, int size, unsigned char byte_array[])
{
    this->id = id;
    this->address = address;
    this->mnemonic = mnemonic;
    this->ops = ops;
    this->full_instruction = mnemonic + " " + ops;
    this->size = size;
    this->not_in_jump = true;
    this->has_been_modified = false;

	//putting the array into a vector
    for (int i = 0; i < size; i++)
    {
        this->bytes_vec.emplace_back(byte_array[i]);
        std::cout << convert_byte_to_string(bytes_vec[i]) <<" ";
    }

    //need to check the exact opcode for the jmp instructions
    //stored in index 0
    int jump_machine_code = this->bytes_vec[0];
    switch (jump_machine_code)
    {
        case 0xeb:
        case 0xFF:
        case 0xEA:
        case 0xE2:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x7e:
        case 0x7f:
        case 0xe3:	{
                        //calculate the twos complement
                        // look at the next byte only since this is a near jump
                        this->twos_complement_offset = calculate_twos_complement_offset_char(this->bytes_vec[1]); //stored in index 1
                    }
                          break;

        case 0x0f:	{
                        std::cout << "hits 0f" << std::endl;
                        if (((this->bytes_vec[1] >> 4) & 0xf) == 8)
                        {
                            // This is a valid jump instruction
                            std::cout << "THIS PART HAS NOT BEEN IMPLEMENTED YET. SO PLEASE DO IMPLEMENT IT IF REALLY NEEDED" << std::endl;
                            // This is now to calculate the offset
                        }
                    }
                        break;

        case 0xE9:	{

                        std::cout << "hits e9" << std::endl;
                        // This is a valid jump instruction
                        std::cout << "THIS PART HAS NOT BEEN IMPLEMENTED YET. SO PLEASE DO IMPLEMENT IT IF REALLY NEEDED" << std::endl;
                    }
                   break;



        default:	{
                        std::cout << "Something wrong with the detection of jump instructions" << std::endl;
                        this->twos_complement_offset = 0;
                    }
                        break;

    }
}

//accessors
int Disassembly::get_id()
{
    return this->id;
}

int Disassembly::get_address()
{
    return this->address;
}

std::string Disassembly::get_mnemonic()
{
    return this->mnemonic;
}

std::string Disassembly::get_ops()
{
    return this->ops;
}

std::string Disassembly::get_full_instruction()
{
    return this->full_instruction;
}

int Disassembly::get_size()
{
    return this->size;
}

std::vector<unsigned char> Disassembly::get_bytes_vec()
{
    return this->bytes_vec;
}

int Disassembly::get_twos_complement_offset()
{
    return this->twos_complement_offset;
}

//check if its not within a jump instruction
bool Disassembly::get_not_in_jump()
{
    return this->not_in_jump;
}

//check if it has been modified
bool Disassembly::get_modified_status()
{
    return this->has_been_modified;
}

//mutators
void Disassembly::set_id(int id)
{
    this->id = id;
}

void Disassembly::set_address(int address)
{
    this->address = address;
}

void Disassembly::set_mnemonic(std::string mnemonic)
{
    this->mnemonic = mnemonic;
}

void Disassembly::set_ops(std::string ops)
{
    this->ops = ops;
}

void Disassembly::set_full_instruction(std::string full_instruction)
{
    this->full_instruction = full_instruction;
}

void Disassembly::set_size(int size)
{
    this->size = size;
}

void Disassembly::set_bytes_vec(std::vector<unsigned char> bytes_vec)
{
    this->bytes_vec = bytes_vec;
}

void Disassembly::set_twos_complement_offset(int twos_complement_offset)
{
    this->twos_complement_offset = twos_complement_offset;
}

void Disassembly::set_not_in_jump(bool not_in_jump)
{
    this->not_in_jump = not_in_jump;
}

void Disassembly::set_modified_true()
{
    this->has_been_modified = true;

}

//calculate the twos complement offset
int Disassembly::calculate_twos_complement_offset_char(signed char offset)
{
    bool is_negative = false;
    int result = offset;
    unsigned char temp = 0;

    if (((offset & 0x80) >> 7) == 1) //to get the most left significant bit //masking 8th bit
    {
        is_negative = true; //if it is one means its a negative value
    }

    if (is_negative == true) //if its a negative value
    {
        temp = (offset ^ 0xff) + 1; //twos comeplement calculation
        result = temp * -1; //make the number to negative
    }

    return result;
}

std::string Disassembly::convert_byte_to_string(char byte)
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

void Disassembly::clear_byte_vec()
{
    this->bytes_vec.clear();
}

unsigned int Disassembly::convert_string_to_hex(std::string hexstr)
{
    //convert string to hex
    unsigned int hex_val;
    std::stringstream hex_char;
    hex_char << std::hex << hexstr;
    hex_char >> hex_val;
    return hex_val;
}

//overload the cout operator so its easier to print
std::ostream &operator<<(std::ostream &os, Disassembly &d)
{
    std::stringstream ss;

    int flag = ss.flags();
    ss << "Addr: " << std::hex << std::left << std::setw(10) << d.get_address();

    ss.flags(flag);

    ss << std::left << std::setw(6) << d.get_mnemonic() << std::left << std::setw(45) << d.get_ops() << "\t";
    ss << "Bytes: ";

    std::vector<unsigned char> temp = d.get_bytes_vec();
    for (int i = 0; i < temp.size(); i++)
    {
        if (i == temp.size() - 1)
        {
            ss << d.convert_byte_to_string(temp[i]) << " ";
        }
        else
        {
            ss << d.convert_byte_to_string(temp[i]) << ", ";
        }
    }

    ss.flags(flag);//resetting flags to take out hexadecimal
    ss << " ";

    os << ss.str();

    return os;
}

