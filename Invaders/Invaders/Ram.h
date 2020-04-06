#pragma once

#include "Common.h"
#include <vector>

constexpr std::size_t MEMORY_SIZE = 0xFFFF;

class Ram
{
public:
    Ram();

    bool LoadRom(std::string filepath);
    void ClearRam();

    std::vector<uint8_t> GetRamCopy();
    std::size_t GetProgramSize() { return _programSize; }

private:
    uint8_t _memory[MEMORY_SIZE];
    std::size_t _programSize;
};

