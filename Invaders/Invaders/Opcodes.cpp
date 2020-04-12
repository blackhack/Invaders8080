#include "Opcodes.h"
#include "CPU8080.h"
#include <algorithm>

OpcodeHandler::OpcodeHandler() : opcode(Opcode::NOP), name("NOP"), dataFlag(InstructionDataFlag::def), callback(nullptr)
{
    length = 1;
}

void ReplaceFirstOccurrence(std::string& source, char oldVal, char newVal)
{
    std::size_t pos = source.find(oldVal);
    if (pos == std::string::npos)
        return;

    source[pos] = newVal;
}

OpcodeHandler::OpcodeHandler(Opcode opcodeId, std::string opName, InstructionDataFlag dataFlagI, OpcodeCallback opCallback) 
    : opcode(opcodeId), dataFlag(dataFlagI), name(opName), callback(opCallback)
{
    name.erase(name.begin(), name.begin() + 8); // Remove the Opcode:: part from the name
    ReplaceFirstOccurrence(name, '_', ' '); // First _ is converted to space
    ReplaceFirstOccurrence(name, '_', ','); // Second _ is converted to ,

    switch (dataFlagI)
    {
        case InstructionDataFlag::a16:
            length = 3;
            break;
        case InstructionDataFlag::a8:
            length = 2;
            break;
        case InstructionDataFlag::d16:
            length = 3;
            break;
        case InstructionDataFlag::d8:
            length = 2;
            break;
        default:
            length = 1;
            break;
    }
}

void OpcodeHelper::AddOpcodeHandler(Opcode opcodeId, std::string opName, InstructionDataFlag dataFlagI, OpcodeCallback opCallback)
{
    handlers[opcodeId] = OpcodeHandler(opcodeId, opName, dataFlagI, opCallback);
}

void OpcodeHelper::InitializeOpcodes()
{
#define DEFINE_HANDLER(opcode, dataFlag, callback) AddOpcodeHandler(opcode, #opcode, dataFlag, callback)

    //0
    DEFINE_HANDLER(Opcode::NOP, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::LXI_B, InstructionDataFlag::d16, &CPU8080::LXI);
    DEFINE_HANDLER(Opcode::STAX_B, InstructionDataFlag::def, &CPU8080::STAX);
    DEFINE_HANDLER(Opcode::INX_B, InstructionDataFlag::def, &CPU8080::INX);
    DEFINE_HANDLER(Opcode::INR_B, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_B, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_B, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::RLC, InstructionDataFlag::def, &CPU8080::RLC);
    DEFINE_HANDLER(Opcode::NOP2, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::DAD_B, InstructionDataFlag::def, &CPU8080::DAD);
    DEFINE_HANDLER(Opcode::LDAX_B, InstructionDataFlag::def, &CPU8080::LDAX);
    DEFINE_HANDLER(Opcode::DCX_B, InstructionDataFlag::def, &CPU8080::DCX);
    DEFINE_HANDLER(Opcode::INR_C, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_C, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_C, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::RRC, InstructionDataFlag::def, &CPU8080::RRC);
    //1
    DEFINE_HANDLER(Opcode::NOP3, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::LXI_D, InstructionDataFlag::d16, &CPU8080::LXI);
    DEFINE_HANDLER(Opcode::STAX_D, InstructionDataFlag::def, &CPU8080::STAX);
    DEFINE_HANDLER(Opcode::INX_D, InstructionDataFlag::def, &CPU8080::INX);
    DEFINE_HANDLER(Opcode::INR_D, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_D, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_D, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::RAL, InstructionDataFlag::def, &CPU8080::RAL);
    DEFINE_HANDLER(Opcode::NOP4, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::DAD_D, InstructionDataFlag::def, &CPU8080::DAD);
    DEFINE_HANDLER(Opcode::LDAX_D, InstructionDataFlag::def, &CPU8080::LDAX);
    DEFINE_HANDLER(Opcode::DCX_D, InstructionDataFlag::def, &CPU8080::DCX);
    DEFINE_HANDLER(Opcode::INR_E, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_E, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_E, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::RAR, InstructionDataFlag::def, &CPU8080::RAR);
    //2
    DEFINE_HANDLER(Opcode::NOP5, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::LXI_H, InstructionDataFlag::d16, &CPU8080::LXI);
    DEFINE_HANDLER(Opcode::SHLD, InstructionDataFlag::a16, &CPU8080::SHLD);
    DEFINE_HANDLER(Opcode::INX_H, InstructionDataFlag::def, &CPU8080::INX);
    DEFINE_HANDLER(Opcode::INR_H, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_H, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_H, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::DAA, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::NOP6, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::DAD_H, InstructionDataFlag::def, &CPU8080::DAD);
    DEFINE_HANDLER(Opcode::LHLD, InstructionDataFlag::a16, &CPU8080::LHLD);
    DEFINE_HANDLER(Opcode::DCX_H, InstructionDataFlag::def, &CPU8080::DCX);
    DEFINE_HANDLER(Opcode::INR_L, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_L, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_L, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::CMA, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    //3
    DEFINE_HANDLER(Opcode::NOP7, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::LXI_SP, InstructionDataFlag::d16, &CPU8080::LXI);
    DEFINE_HANDLER(Opcode::STA, InstructionDataFlag::a16, &CPU8080::STA);
    DEFINE_HANDLER(Opcode::INX_SP, InstructionDataFlag::def, &CPU8080::INX);
    DEFINE_HANDLER(Opcode::INR_M, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_M, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_M, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::STC, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::NOP8, InstructionDataFlag::def, &CPU8080::NOP);
    DEFINE_HANDLER(Opcode::DAD_SP, InstructionDataFlag::def, &CPU8080::DAD);
    DEFINE_HANDLER(Opcode::LDA, InstructionDataFlag::a16, &CPU8080::LDA);
    DEFINE_HANDLER(Opcode::DCX_SP, InstructionDataFlag::def, &CPU8080::DCX);
    DEFINE_HANDLER(Opcode::INR_A, InstructionDataFlag::def, &CPU8080::INR);
    DEFINE_HANDLER(Opcode::DCR_A, InstructionDataFlag::def, &CPU8080::DCR);
    DEFINE_HANDLER(Opcode::MVI_A, InstructionDataFlag::d8, &CPU8080::MVI);
    DEFINE_HANDLER(Opcode::CMC, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    //4
    DEFINE_HANDLER(Opcode::MOV_B_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_B_A, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_C_A, InstructionDataFlag::def, &CPU8080::MOV);
    //5
    DEFINE_HANDLER(Opcode::MOV_D_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_D_A, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_E_A, InstructionDataFlag::def, &CPU8080::MOV);
    //6
    DEFINE_HANDLER(Opcode::MOV_H_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_H_A, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_L_A, InstructionDataFlag::def, &CPU8080::MOV);
    //7
    DEFINE_HANDLER(Opcode::MOV_M_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_M_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_M_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_M_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_M_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_M_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::HLT, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::MOV_M_A, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_B, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_C, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_D, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_E, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_H, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_L, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_M, InstructionDataFlag::def, &CPU8080::MOV);
    DEFINE_HANDLER(Opcode::MOV_A_A, InstructionDataFlag::def, &CPU8080::MOV);
    //8
    DEFINE_HANDLER(Opcode::ADD_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADD_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADC_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    //9
    DEFINE_HANDLER(Opcode::SUB_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUB_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBB_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    //A
    DEFINE_HANDLER(Opcode::ANA_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANA_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRA_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    //B
    DEFINE_HANDLER(Opcode::ORA_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORA_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_C, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_E, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_L, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_M, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CMP_A, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);

    //C
    DEFINE_HANDLER(Opcode::RNZ, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::POP_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JNZ, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JMP, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CNZ, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::PUSH_B, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ADI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_0, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RZ, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RET, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JZ, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JMP2, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CZ, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CALL, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ACI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_1, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);

    //D
    DEFINE_HANDLER(Opcode::RNC, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::POP_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JNC, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::OUT, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CNC, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::PUSH_D, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SUI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_2, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RC, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RET2, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JC, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::IN, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CC, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CALL2, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SBI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_3, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);

    //E
    DEFINE_HANDLER(Opcode::RPO, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::POP_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JPO, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XTHL, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CPO, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::PUSH_H, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ANI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_4, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RPE, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::PCHL, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JPE, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XCHG, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CPE, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CALL3, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::XRI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_5, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);

    //F
    DEFINE_HANDLER(Opcode::RP, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::POP_PSW, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JP, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::DI, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CP, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::PUSH_PSW, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::ORI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_6, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RM, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::SPHL, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::JM, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::EI, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CM, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CALL4, InstructionDataFlag::a16, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::CPI, InstructionDataFlag::d8, &CPU8080::NotImplementedHandler);
    DEFINE_HANDLER(Opcode::RST_7, InstructionDataFlag::def, &CPU8080::NotImplementedHandler);
}

OpcodeHelper opcodeHelper;
