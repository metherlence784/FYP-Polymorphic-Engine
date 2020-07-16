#include "mainwindow.h"
#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


// capstone_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <stdio.h>
//#include <inttypes.h>
//#include <iostream>

//#include <capstone.h>

//const unsigned char CODE[] = "\x55\x89\xe5\x83\xec\x24\xB8\x78\x65\x63\x41\x25\xFF\xFF\xFF\x00\x50\x68\x57\x69\x6E\x45\x31\xD2\x64\x8B\x5A\x30\x8B\x5B\x0C\x8B\x5B\x14\x8B\x1B\x8B\x1B\x8B\x5B\x10\x89\x5D\xF8\x8B\x43\x3C\x01\xD8\x89\x45\xF4\x8B\x48\x78\x01\xD9\x89\x4D\xF0\x8B\x51\x14\x89\x55\xEC\x8B\x79\x1C\x01\xDF\x89\x7D\xE8\x8B\x71\x20\x01\xDE\x89\x75\xE0\x8B\x51\x24\x01\xDA\x89\x55\xDC\x31\xD2\x89\xE6\x8B\x7D\xE0\x31\xC9\xFC\x8B\x3C\x97\x01\xDF\x66\x83\xC1\x08\xF3\xA6\x74\x0D\x42\x8B\x45\xEC\x39\xD0\x77\xE3\x83\xC4\x26\xEB\x3C\x8B\x75\xDC\x8B\x7D\xE8\x66\x8B\x04\x56\x8B\x04\x87\x01\xD8\x31\xD2\x52\x68\x2E\x65\x78\x65\x68\x63\x61\x6C\x63\x68\x6D\x33\x32\x5C\x68\x79\x73\x74\x65\x68\x77\x73\x5C\x53\x68\x69\x6E\x64\x6F\x68\x43\x3A\x5C\x57\x89\xE7\x6A\x0A\x57\xFF\xD0\x83\xC4\x28";

//int main(void)
//{
//    csh handle;
//    cs_insn *insn;
//    size_t count;
//    std::cout << "START OF THE CAPSTONE " << std::endl;
//    if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK)
//        return -1;
//    count = cs_disasm(handle, CODE, sizeof(CODE) - 1, 0x1000, 0, &insn);
//    if (count > 0) {
//        size_t j;
//        for (j = 0; j < count; j++) {
//            printf("0x%" PRIx64 ":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
//                insn[j].op_str);
//        }

//        cs_free(insn, count);
//    }
//    else
//        printf("ERROR: Failed to disassemble given code!\n");

//    cs_close(&handle);

//	while (true)
//	{

//	}
//    std::cout << "END OF THE CAPSTONE " << std::endl;
//    return 0;
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
