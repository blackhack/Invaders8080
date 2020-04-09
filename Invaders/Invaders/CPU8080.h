#pragma once

#include "Common.h"
#include "Opcodes.h"

class Ram;

class CPU8080
{
public:
    CPU8080(Ram* ram);
    ~CPU8080();

    void Execute();
    void PrintDebug();
    void StackPush(uint16_t value);
    uint16_t StackPop();

public:
    void NotImplementedHandler(OpcodeHandler* opcode_handler);
    void NOP(OpcodeHandler* opcode_handler);
    void LXI(OpcodeHandler* opcode_handler);
    void MVI(OpcodeHandler* opcode_handler);
    void MOV(OpcodeHandler* opcode_handler);
private:
    Ram* _ram;
    uint16_t _pc;
    uint16_t _stack_pointer;

    //Register
    uint8_t _reg_B;
    uint8_t _reg_C;
    uint8_t _reg_D;
    uint8_t _reg_E;
    uint8_t _reg_H;
    uint8_t _reg_L;
    uint8_t* _reg_M = &_reg_H;
    uint8_t _reg_A;

};

