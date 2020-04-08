#include "Ram.h"
#include <fstream>

Ram::Ram()
{
    ClearRam();
}

bool Ram::LoadRom(std::string filepath)
{
    std::ifstream fileStream;
    fileStream.open(filepath, std::ios::binary | std::ios::ate);
    fileStream >> std::noskipws;
    std::streamoff fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    if (!fileStream.is_open())
    {
        std::cerr << "Error, cant open the file\n";
        return false;
    }

    if (fileSize > MEMORY_SIZE)
    {
        std::cerr << "Error, The rom is too big: " << fileSize << " bytes (Maximum: 65535 bytes)\n";
        return false;
    }

    ClearRam();

    uint8_t read_byte = 0;
    while (fileStream >> read_byte)
    {
        std::streamoff pos = fileStream.tellg();
        _memory[pos - 1] = read_byte;
        //std::cout << std::hex << (int)read_byte;
    }

    _programSize = static_cast<std::size_t>(fileSize);

    std::cout << "ROM loaded!, Size: " << _programSize << " bytes" << std::endl;

    return true;
}

void Ram::ClearRam()
{
    _programSize = 0;
    std::fill(_memory, _memory + MEMORY_SIZE, 0);
}

void Ram::Write(uint16_t address, uint8_t data)
{
    _memory[address] = data;
}

uint8_t Ram::Read(uint16_t address)
{
    return _memory[address];
}

std::vector<uint8_t> Ram::GetRamCopy()
{
    std::vector<uint8_t> ramCopy(_memory, _memory + MEMORY_SIZE);
    return ramCopy;
}
