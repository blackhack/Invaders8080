#include "CPU8080.h"
#include "Ram.h"
#include  <iomanip>

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
    _reg_M = _ram->GetPointerToByte(0);
    _reg_A = 0;

    _carry_flag = 0;
    _aux_carry_flag = 0;
    _sign_flag = 0;
    _zero_flag = 0;
    _parity_flag = 0;
}

CPU8080::~CPU8080()
{

}

void CPU8080::Execute()
{
    Opcode opcode = static_cast<Opcode>(_ram->Read(_pc));
    OpcodeMap::iterator opcodeItr = OpcodeTable.find(opcode);
    OpcodeHandler* opHandler = opcodeItr != OpcodeTable.end() ? &opcodeItr->second : nullptr;

    if (opHandler)
    {
        std::cout << "Current opcode: " << opHandler->name << std::endl;
        opHandler->callback(this, opHandler);
    }
    else
        assert(false);

    ReLinkMemoryReference(); // Relink reg_m to the memory address specified by H(LSB) and L(MSB)
}

void CPU8080::CalculateFlags(uint16_t value1, uint16_t value2, bool add, FlagsMask flags)
{
    uint32_t result = value1;
    if (add)
        result += value2;
    else
        result -= value2;

    if (flags & FLAG_CARRY)
    {
        if (add)
            _carry_flag = result > 0xFFFF ? 1 : 0;
        else
            _carry_flag = (value1 <= value2) ? 1 : 0;
    }
    if (flags & FLAG_AUX_CARRY)
    {
        if (add)
            _aux_carry_flag = result > 0xFF ? 1 : 0;
        else
            _aux_carry_flag = (value1 <= value2) ? 1 : 0;
    }
    if (flags & FLAG_SIGN)
        _sign_flag = ((result & 0b10000000) >> 7) == 1 ? 1 : 0;
    if (flags & FLAG_ZERO)
        _zero_flag = (result & 0xFF) == 0 ? 1 : 0;

    auto CheckParity = [](uint8_t value)
    { 
        uint8_t count = 0;
        for (int i = 0; i < 8; ++i)
            if ((value & 0x1 << i) != 0)
                ++count;

        return (count % 2) == 0 ? 1 : 0;
    };

    if (flags & FLAG_PARITY)
        _parity_flag = CheckParity((uint8_t)result);
}

void CPU8080::ReLinkMemoryReference()
{
    uint16_t address = (uint16_t)_reg_L << 8;
    address |= _reg_H;

    _reg_M = _ram->GetPointerToByte(address);
}

void CPU8080::PrintDebug()
{
    std::cout << "------Intel 8080 - Debug Information------\n";
    std::cout << "  _pc: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_pc) << std::endl;
    std::cout << "  _stack_pointer: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_stack_pointer) << std::endl;
    std::cout << "  _reg_B: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_B) << std::endl;
    std::cout << "  _reg_C: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_C) << std::endl;
    std::cout << "  _reg_D: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_D) << std::endl;
    std::cout << "  _reg_E: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_E) << std::endl;
    std::cout << "  _reg_H: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_H) << std::endl;
    std::cout << "  _reg_L: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_L) << std::endl;
    std::cout << "  _reg_M: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(*_reg_M) << std::endl;
    std::cout << "  _reg_A: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_reg_A) << std::endl;
    std::cout << "  _carry_flag: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_carry_flag) << std::endl;
    std::cout << "  _aux_carry_flag: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_aux_carry_flag) << std::endl;
    std::cout << "  _sign_flag: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_sign_flag) << std::endl;
    std::cout << "  _zero_flag: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_zero_flag) << std::endl;
    std::cout << "  _parity_flag: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_parity_flag) << std::endl;
    //std::cout << "  MemAt: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_ram->Read(0x010a)) << std::endl;
    //std::cout << "  MemAt: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << static_cast<uint32_t>(_ram->Read(0x010b)) << std::endl;
    std::cout << "------Debug information end------\n";
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
    std::cerr << "Executing not implemented opcode: " << opcode_handler->name << std::endl;
}

void CPU8080::NOP(OpcodeHandler* opcode_handler)
{
    // nopping!
    _pc += opcode_handler->length;
}

void CPU8080::LXI(OpcodeHandler* opcode_handler)
{
    switch (opcode_handler->opcode)
    {
        case Opcode::LXI_B:
            _reg_B = _ram->Read(_pc + 2);
            _reg_C = _ram->Read(_pc + 1);
            break;
        case Opcode::LXI_D:
            _reg_D = _ram->Read(_pc + 2);
            _reg_E = _ram->Read(_pc + 1);
            break;
        case Opcode::LXI_H:
            _reg_H = _ram->Read(_pc + 2);
            _reg_L = _ram->Read(_pc + 1);
            break;
        case Opcode::LXI_SP:
            _stack_pointer = _ram->Read(_pc + 2);
            _stack_pointer |= (uint16_t)_ram->Read(_pc + 1) << 8;
            break;
    }

    _pc += opcode_handler->length;
}

void CPU8080::STAX(OpcodeHandler* opcode_handler)
{
    uint16_t address = 0x0000;
    if (opcode_handler->opcode == Opcode::STAX_B)
    {
        address = (uint16_t)_reg_B << 8;
        address |= _reg_C;
    }
    else
    {
        address = (uint16_t)_reg_D << 8;
        address |= _reg_E;
    }

    _ram->Write(address, _reg_A);

    _pc += opcode_handler->length;
}

void CPU8080::SHLD(OpcodeHandler* opcode_handler)
{
    uint16_t address = _ram->Read(_pc + 1);
    address |= (uint16_t)_ram->Read(_pc + 2) << 8;

    _ram->Write(address, _reg_L);
    _ram->Write(address + 1, _reg_H);

    _pc += opcode_handler->length;
}

void CPU8080::STA(OpcodeHandler* opcode_handler)
{
    uint16_t address = _ram->Read(_pc + 1);
    address |= (uint16_t)_ram->Read(_pc + 2) << 8;

    _ram->Write(address, _reg_A);

    _pc += opcode_handler->length;
}

void CPU8080::INX(OpcodeHandler* opcode_handler)
{
    uint8_t* reg_high = nullptr;
    uint8_t* reg_low = nullptr;

    switch (opcode_handler->opcode)
    {
        case Opcode::INX_B:
            reg_high = &_reg_B;
            reg_low = &_reg_C;
            break;
        case Opcode::INX_D:
            reg_high = &_reg_D;
            reg_low = &_reg_E;
            break;
        case Opcode::INX_H:
            reg_high = &_reg_H;
            reg_low = &_reg_L;
            break;
        case Opcode::INX_SP:
            ++_stack_pointer;
            break;
    }

    if (reg_low && reg_high)
    {
        uint16_t value = *reg_low;
        value |= (uint16_t)(*reg_high) << 8;

        ++value;

        *reg_low = value & 0x00FF;
        *reg_high = (value & 0xFF00) >> 8;
    }

    _pc += opcode_handler->length;
}

void CPU8080::INR(OpcodeHandler* opcode_handler)
{
    switch (opcode_handler->opcode)
    {
        case Opcode::INR_B: CalculateFlags(_reg_B, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_B; break;
        case Opcode::INR_C: CalculateFlags(_reg_C, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_C; break;
        case Opcode::INR_D: CalculateFlags(_reg_D, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_D; break;
        case Opcode::INR_E: CalculateFlags(_reg_E, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_E; break;
        case Opcode::INR_H: CalculateFlags(_reg_H, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_H; break;
        case Opcode::INR_L: CalculateFlags(_reg_L, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_L; break;
        case Opcode::INR_M: CalculateFlags(*_reg_M, 1, true, FLAG_ALL_BUT_CARRY); ++(*_reg_M); break;
        case Opcode::INR_A: CalculateFlags(_reg_A, 1, true, FLAG_ALL_BUT_CARRY); ++_reg_A; break;
    }

    _pc += opcode_handler->length;
}

void CPU8080::DCR(OpcodeHandler* opcode_handler)
{
    switch (opcode_handler->opcode)
    {
        case Opcode::DCR_B: CalculateFlags(_reg_B, 1, false, FLAG_ALL_BUT_CARRY); --_reg_B; break;
        case Opcode::DCR_C: CalculateFlags(_reg_C, 1, false, FLAG_ALL_BUT_CARRY); --_reg_C; break;
        case Opcode::DCR_D: CalculateFlags(_reg_D, 1, false, FLAG_ALL_BUT_CARRY); --_reg_D; break;
        case Opcode::DCR_E: CalculateFlags(_reg_E, 1, false, FLAG_ALL_BUT_CARRY); --_reg_E; break;
        case Opcode::DCR_H: CalculateFlags(_reg_H, 1, false, FLAG_ALL_BUT_CARRY); --_reg_H; break;
        case Opcode::DCR_L: CalculateFlags(_reg_L, 1, false, FLAG_ALL_BUT_CARRY); --_reg_L; break;
        case Opcode::DCR_M: CalculateFlags(*_reg_M, 1, false, FLAG_ALL_BUT_CARRY); --(*_reg_M); break;
        case Opcode::DCR_A: CalculateFlags(_reg_A, 1, false, FLAG_ALL_BUT_CARRY); --_reg_A; break;
    }

    _pc += opcode_handler->length;
}

void CPU8080::LDAX(OpcodeHandler* opcode_handler)
{
    uint16_t address = 0x0000;
    if (opcode_handler->opcode == Opcode::LDAX_B)
    {
        address = (uint16_t)_reg_B << 8;
        address |= _reg_C;
    }
    else
    {
        address = (uint16_t)_reg_D << 8;
        address |= _reg_E;
    }

    _reg_A = _ram->Read(address);

    _pc += opcode_handler->length;
}

void CPU8080::LHLD(OpcodeHandler* opcode_handler)
{
    uint16_t address = _ram->Read(_pc + 1);
    address |= (uint16_t)_ram->Read(_pc + 2) << 8;

    _reg_L = _ram->Read(address);
    _reg_H = _ram->Read(address + 1);

    _pc += opcode_handler->length;
}

void CPU8080::LDA(OpcodeHandler* opcode_handler)
{
    uint16_t address = _ram->Read(_pc + 1);
    address |= (uint16_t)_ram->Read(_pc + 2) << 8;

    _reg_A = _ram->Read(address);

    _pc += opcode_handler->length;
}

void CPU8080::DCX(OpcodeHandler* opcode_handler)
{
    uint8_t* reg_high = nullptr;
    uint8_t* reg_low = nullptr;

    switch (opcode_handler->opcode)
    {
        case Opcode::DCX_B:
            reg_high = &_reg_B;
            reg_low = &_reg_C;
            break;
        case Opcode::DCX_D:
            reg_high = &_reg_D;
            reg_low = &_reg_E;
            break;
        case Opcode::DCX_H:
            reg_high = &_reg_H;
            reg_low = &_reg_L;
            break;
        case Opcode::DCX_SP:
            --_stack_pointer;
            break;
    }

    if (reg_low && reg_high)
    {
        uint16_t value = *reg_low;
        value |= (uint16_t)(*reg_high) << 8;

        --value;

        *reg_low = value & 0x00FF;
        *reg_high = (value & 0xFF00) >> 8;
    }

    _pc += opcode_handler->length;
}

void CPU8080::MVI(OpcodeHandler* opcode_handler)
{
    switch (opcode_handler->opcode)
    {
        case Opcode::MVI_B: _reg_B = _ram->Read(_pc + 1); break;
        case Opcode::MVI_C: _reg_C = _ram->Read(_pc + 1); break;
        case Opcode::MVI_D: _reg_D = _ram->Read(_pc + 1); break;
        case Opcode::MVI_E: _reg_E = _ram->Read(_pc + 1); break;
        case Opcode::MVI_H: _reg_H = _ram->Read(_pc + 1); break;
        case Opcode::MVI_L: _reg_L = _ram->Read(_pc + 1); break;
        case Opcode::MVI_M: *_reg_M = _ram->Read(_pc + 1); break;
        case Opcode::MVI_A: _reg_A = _ram->Read(_pc + 1); break;
    }

    _pc += opcode_handler->length;
}

void CPU8080::RLC(OpcodeHandler* opcode_handler)
{
    _carry_flag = (_reg_A & 0b10000000) >> 7;
    _reg_A <<= 1;
    _reg_A |= _carry_flag;

    _pc += opcode_handler->length;
}

void CPU8080::RRC(OpcodeHandler* opcode_handler)
{
    _carry_flag = _reg_A & 0b00000001;
    _reg_A >>= 1;
    _reg_A |= _carry_flag << 7;

    _pc += opcode_handler->length;
}

void CPU8080::RAL(OpcodeHandler* opcode_handler)
{
    uint8_t old_carry = _carry_flag;
    _carry_flag = (_reg_A & 0b10000000) >> 7;
    _reg_A <<= 1;
    _reg_A |= old_carry;

    _pc += opcode_handler->length;
}

void CPU8080::RAR(OpcodeHandler* opcode_handler)
{
    uint8_t old_carry = _carry_flag;
    _carry_flag = _reg_A & 0b00000001;
    _reg_A >>= 1;
    _reg_A |= old_carry << 7;

    _pc += opcode_handler->length;
}

void CPU8080::DAD(OpcodeHandler* opcode_handler)
{
    uint16_t value_reg = 0;
    uint16_t value_HL = _reg_L;
    value_HL |= (uint16_t)(_reg_H) << 8;

    switch (opcode_handler->opcode)
    {
        case Opcode::DAD_B:
            value_reg = _reg_C;
            value_reg |= (uint16_t)(_reg_B) << 8;
            break;
        case Opcode::DAD_D:
            value_reg = _reg_E;
            value_reg |= (uint16_t)(_reg_D) << 8;
            break;
        case Opcode::DAD_H:
            value_reg = _reg_L;
            value_reg |= (uint16_t)(_reg_H) << 8;
            break;
        case Opcode::DAD_SP:
            value_reg = _stack_pointer;
            break;
    }

    CalculateFlags(value_reg, value_HL, true, FLAG_CARRY);

    uint16_t total = value_reg + value_HL;

    _reg_L = total & 0x00FF;
    _reg_H = (total & 0xFF00) >> 8;

    _pc += opcode_handler->length;
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
    _pc += opcode_handler->length;
}