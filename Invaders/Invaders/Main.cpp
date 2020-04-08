#include <iostream>
#include "Ram.h"
#include "Disassembler.h"
#include "CPU8080.h"

int main()
{
    Ram ram;
    Disassembler disassembler;
    CPU8080 cpu(&ram);

    ram.LoadRom("../../Roms/cpu_tests/TST8080.COM");
    cpu.Execute();
    disassembler.Disassemble(ram.GetRamCopy().data(), ram.GetProgramSize());

    return 0;
}