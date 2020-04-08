#include "CPU8080.h"
#include "Ram.h"

CPU8080::CPU8080(Ram* ram) : _ram(ram)
{
    _pc = 0;
    _stack_pointer = 0;
    _reg_B = 0;
    _reg_C = 0;
    _reg_D = 0;
    _reg_E = 0;
    _reg_H = 0;
    _reg_L = 0;
    _reg_A = 0;
}

CPU8080::~CPU8080()
{

}

void CPU8080::Execute()
{
        Opcode opcode = static_cast<Opcode>(_ram->Read(_pc));
        OpcodeMap::iterator opcodeItr = OpcodeTable.find(opcode);
        OpcodeHandler* opHandler = opcodeItr != OpcodeTable.end() ? &opcodeItr->second : nullptr;

        opHandler->callback(this, opHandler);
}

void CPU8080::StackPush(uint16_t value)
{
    _ram->Write(_stack_pointer - 1, value & 0x00FF);
    _ram->Write(_stack_pointer - 2, (value >> 8) & 0x00FF);
    _stack_pointer -= 2;
}

uint16_t CPU8080::StackPop()
{
    uint16_t value = 0x0000;
    value |= _ram->Read(_stack_pointer);
    value |= (uint16_t)_ram->Read(_stack_pointer + 1) << 8;

    _stack_pointer += 2;

    return value;
}

void CPU8080::NotImplementedHandler(OpcodeHandler* opcode_handler)
{

}

void CPU8080::NOP(OpcodeHandler* opcode_handler)
{
    // nopping!
}

void CPU8080::MOV(OpcodeHandler* opcode_handler)
{
    uint8_t op = static_cast<uint8_t>(opcode_handler->opcode);

    uint8_t source_reg_id = op & 0b00000111;
    uint8_t dest_reg_id = (op & 0b00111000) >> 3;
    uint8_t* source_reg;
    uint8_t* dest_reg;

    switch (source_reg_id)
    {
        case 0b000: source_reg = &_reg_B; break;
        case 0b001: source_reg = &_reg_C; break;
        case 0b010: source_reg = &_reg_D; break;
        case 0b011: source_reg = &_reg_E; break;
        case 0b100: source_reg = &_reg_H; break;
        case 0b101: source_reg = &_reg_L; break;
        case 0b110: source_reg = _reg_M; break;
        case 0b111: source_reg = &_reg_A; break;
        default:source_reg = _reg_M; break;
    }

    switch (dest_reg_id)
    {
        case 0b000: dest_reg = &_reg_B; break;
        case 0b001: dest_reg = &_reg_C; break;
        case 0b010: dest_reg = &_reg_D; break;
        case 0b011: dest_reg = &_reg_E; break;
        case 0b100: dest_reg = &_reg_H; break;
        case 0b101: dest_reg = &_reg_L; break;
        case 0b110: dest_reg = _reg_M; break;
        case 0b111: dest_reg = &_reg_A; break;
        default:dest_reg = _reg_M; break;
    }

    *dest_reg = *source_reg;
}