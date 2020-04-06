#include <iostream>
#include "Ram.h"
#include "Disassembler.h"

int main()
{
    Ram ram;
    Disassembler disassembler;

    ram.LoadRom("../../Roms/invaders/invaders.h");
    disassembler.Disassemble(ram.GetRamCopy().data(), ram.GetProgramSize());

    return 0;
}