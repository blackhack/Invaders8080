#include <iostream>
#include "Ram.h"
#include "Disassembler.h"
#include "CPU8080.h"

int main()
{
    Ram ram;
    Disassembler disassembler;
    CPU8080 cpu(&ram);

    ram.LoadRom("../../Roms/testrom");
    disassembler.Disassemble(ram.GetRamCopy().data(), ram.GetProgramSize());

    while (true)
    {
        cpu.PrintDebug();
        system("PAUSE");
        cpu.Execute();
    }

    return 0;
}