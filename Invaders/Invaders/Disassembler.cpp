#include "Disassembler.h"
#include "Opcodes.h"
#include  <iomanip>

void Disassembler::Disassemble(uint8_t* memory, std::size_t size)
{
    for (std::size_t i = 0; i < size;)
    {
        Opcode opcode = static_cast<Opcode>(memory[i]);
        OpcodeMap::iterator opcodeItr = OpcodeTable.find(opcode);
        OpcodeHandler* opHandler = opcodeItr != OpcodeTable.end() ? &opcodeItr->second : nullptr;

        std::cout << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(i) << " - ";

        // Developing debug
        assert(opHandler);

        std::cout << opHandler->name;
        if (opHandler->dataFlag != InstructionDataFlag::def)
        {
            uint16_t data;
            if (opHandler->dataFlag == InstructionDataFlag::a16 || opHandler->dataFlag == InstructionDataFlag::d16)
                data = memory[i + 2] << 8 | memory[i + 1];
            else
                data = memory[i + 1];

            std::cout << " ";
            if (opHandler->dataFlag == InstructionDataFlag::d16 || opHandler->dataFlag == InstructionDataFlag::d8)
                std::cout << "#";

            std::cout << "$" << std::setfill('0') << std::setw(4) << std::right << std::hex << data;
        }

        std::cout << std::endl;

        i += opHandler->length;
    }
}
