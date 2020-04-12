#pragma once

#include "Common.h"
#include "Opcodes.h"

class Ram;

enum FlagsMask
{
    FLAG_CARRY = 1,
    FLAG_AUX_CARRY = 2,
    FLAG_SIGN = 4,
    FLAG_ZERO = 8,
    FLAG_PARITY = 16,
    FLAG_ALL = FLAG_CARRY | FLAG_AUX_CARRY | FLAG_SIGN | FLAG_ZERO | FLAG_PARITY,
    FLAG_ALL_BUT_CARRY = FLAG_ALL & ~FLAG_CARRY,
    FLAG_ALL_BUT_AUX_CARRY = FLAG_ALL & ~FLAG_AUX_CARRY
};

class CPU8080
{
public:
    CPU8080(Ram* ram);
    ~CPU8080();

    void Execute();
    void CalculateFlags(uint16_t value1, uint16_t value2, bool add, FlagsMask flags);
    void ReLinkMemoryReference();
    void PrintDebug();
    void StackPush(uint16_t value);
    uint16_t StackPop();

public:
    void NotImplementedHandler(OpcodeHandler* opcode_handler);
    void NOP(OpcodeHandler* opcode_handler);
    void LXI(OpcodeHandler* opcode_handler);
    void STAX(OpcodeHandler* opcode_handler);
    void SHLD(OpcodeHandler* opcode_handler);
    void STA(OpcodeHandler* opcode_handler);
    void INX(OpcodeHandler* opcode_handler);
    void INR(OpcodeHandler* opcode_handler);
    void DCR(OpcodeHandler* opcode_handler);
    void LDAX(OpcodeHandler* opcode_handler);
    void LHLD(OpcodeHandler* opcode_handler);
    void LDA(OpcodeHandler* opcode_handler);
    void DCX(OpcodeHandler* opcode_handler);
    void MVI(OpcodeHandler* opcode_handler);
    void RLC(OpcodeHandler* opcode_handler);
    void RRC(OpcodeHandler* opcode_handler);
    void RAL(OpcodeHandler* opcode_handler);
    void RAR(OpcodeHandler* opcode_handler);
    void DAD(OpcodeHandler* opcode_handler);
    void MOV(OpcodeHandler* opcode_handler);
private:
    Ram* _ram;
    uint16_t _pc;
    uint16_t _stack_pointer;

    //Registers
    uint8_t _reg_B;
    uint8_t _reg_C;
    uint8_t _reg_D;
    uint8_t _reg_E;
    uint8_t _reg_H;
    uint8_t _reg_L;
    uint8_t* _reg_M;
    uint8_t _reg_A;

    // Conditional bits (flags)
    uint8_t _carry_flag;
    uint8_t _aux_carry_flag;
    uint8_t _sign_flag;
    uint8_t _zero_flag;
    uint8_t _parity_flag;
};

