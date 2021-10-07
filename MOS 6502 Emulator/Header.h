#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <functional>
void ExecuteNextInstruction();
int NumberOfInstructionsExecuted = 0;
uint16_t PreviousPC;

uint8_t** CPUMemory;

uint16_t PC;
uint8_t SP;//STACK IS AT MEMORY 0x01FF downto 0x0100
uint8_t A;//Accumulator
uint8_t X;//Index Register X
uint8_t Y;//Index Register Y
uint8_t P = 0b00100100;//Status Register

bool FinishedExecutingCurrentInsctruction = true;

uint16_t Get16BitAddressFromMemoryLocation(uint16_t StartingAddress) {
	uint8_t Lower = *CPUMemory[StartingAddress];
	uint8_t Upper = *CPUMemory[StartingAddress + 1];
	uint16_t ActualAddress = Upper;
	ActualAddress = ActualAddress << 8;
	ActualAddress = ActualAddress | Lower;
	return ActualAddress;
}
void set_pc(uint16_t new_pc) {
	PC = new_pc;
}
uint8_t* GetPointerToDataInCPUMemoryUsing_IME_MODE() {
	return  CPUMemory[PC + 1];
}

uint8_t* GetPointerToDataInCPUMemoryUsing_ZABS_MODE() {
	return CPUMemory[*CPUMemory[PC + 1]];
}
uint8_t* GetPointerToDataInCPUMemoryUsing_ZINX_MODE() {
	return CPUMemory[(uint8_t)(*CPUMemory[PC + 1] + X)];//casting to uint8_t is a must, so that the address wrap around after 0xff
}
uint8_t* GetPointerToDataInCPUMemoryUsing_ZINY_MODE() {
	return CPUMemory[(uint8_t)(*CPUMemory[PC + 1] + Y)];//casting to uint8_t is a must, so that the address wrap around after 0xff
}
uint8_t* GetPointerToDataInCPUMemoryUsing_ABS_MODE() {
	return CPUMemory[Get16BitAddressFromMemoryLocation(PC + 1)];
}
uint8_t* GetPointerToDataInCPUMemoryUsing_INX_X_MODE() {
	return CPUMemory[Get16BitAddressFromMemoryLocation(PC + 1) + X];
}
uint8_t* GetPointerToDataInCPUMemoryUsing_INX_Y_MODE() {
	return CPUMemory[Get16BitAddressFromMemoryLocation(PC + 1) + Y];
}
uint8_t* GetPointerToDataInCPUMemoryUsing_PRII_MODE() {
	return CPUMemory[Get16BitAddressFromMemoryLocation((uint8_t)(*CPUMemory[PC + 1] + X))];
}
uint8_t* GetPointerToDataInCPUMemoryUsing_POII_MODE() {
	return CPUMemory[Get16BitAddressFromMemoryLocation(*CPUMemory[PC + 1]) + Y];
}


void PushToStack(uint8_t data) {
	uint8_t upperhalf = 0b00000001;//this is always 1 so that SP will range from 0x01ff down to 0x0100 https://superuser.com/questions/346658/does-the-6502-put-ff-in-the-stack-pointer-register-as-soon-as-it-gets-power-for
	uint16_t theactualaddress = (upperhalf << 8) | SP;
	*CPUMemory[theactualaddress] = data;
	SP--;
}
uint8_t PopStack() {
	SP++;
	uint8_t upperhalf = 0b00000001;//this is always 1 so that SP will range from 0x01ff down to 0x0100 https://superuser.com/questions/346658/does-the-6502-put-ff-in-the-stack-pointer-register-as-soon-as-it-gets-power-for
	uint16_t theactualaddress = (upperhalf << 8) | SP;
	return *CPUMemory[theactualaddress];
}
uint16_t PopPCfromStack() {
	uint8_t Lower = PopStack();
	uint8_t Upper = PopStack();
	uint16_t PC = Upper;
	PC = PC << 8;
	PC = PC | Lower;
	return PC;
}
void PushPCtoStack() {
	uint8_t Upper = (PC & 0xFF00) >> 8;
	uint8_t Lower = PC & 0x00FF;
	uint16_t PC = Upper;
	//JSR (jump to subroutine) requires upper first then lower if it breaks something then i will make a dedicated one for it
	PushToStack(Upper);
	PushToStack(Lower);
}
//////should change parameter later to uint8_t not a pointer
bool GetNegativeFromData(uint8_t* DataToCheck) {
	if (*DataToCheck & 0b10000000) {
		return true;
	}
	else {
		return false;
	}
}

bool GetCarry() {
	if (P & 0b00000001) {
		return true;
	}
	else {
		return false;
	}
}
void SetCarry() {
	P = P | 0b00000001;
}
void ResetCarry() {
	P = P & 0b11111110;
}

bool GetZero() {
	if (P & 0b00000010) {
		return true;
	}
	else {
		return false;
	}
}
void SetZero() {
	P = P | 0b00000010;
}
void ResetZero() {
	P = P & 0b11111101;
}

bool GetInterruptDisable() {
	if (P & 0b00000100) {
		return true;
	}
	else {
		return false;
	}
}
void SetInterruptDisbale() {
	P = P | 0b00000100;
}
void ResetInterruptDisable() {
	P = P & 0b11111011;
}

bool GetDecimalMode() {
	if (P & 0b00001000) {
		return true;
	}
	else {
		return false;
	}
}
void SetDecimalMode() {
	P = P | 0b00001000;
}
void ResetDecimalMode() {
	P = P & 0b11110111;
}

bool GetBreak() {
	if (P & 0b00010000) {
		return true;
	}
	else {
		return false;
	}
}
void SetBreak() {
	P = P | 0b00010000;
}
void ResetBreak() {
	P = P & 0b11101111;
}

///R flag is always 1 no need for it
/// 
bool GetOverflow() {
	if (P & 0b01000000) {
		return true;
	}
	else {
		return false;
	}
}
void SetOverflow() {
	P = P | 0b01000000;
}
void ResetOverflow() {
	P = P & 0b10111111;
}

bool GetNegative() {
	if (P & 0b10000000) {
		return true;
	}
	else {
		return false;
	}
}
void SetNegative() {
	P = P | 0b10000000;
}
void ResetNegative() {
	P = P & 0b01111111;
}

void BaseOverflowCheckOnSubtraction(uint8_t Value1, uint8_t Value2);
void BaseLSR(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered);
void BaseROR(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered);
void BaseROL(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered);
void BaseOverflowCheckOnAddition(uint8_t Value1, uint8_t Value2);
void BaseCOMPARE(uint8_t InstructionLength, uint8_t ValueToCompare1, uint8_t ValueToCompare2);
void BaseSZCheck(uint8_t InstructionLength, uint8_t DataToCheck);
void BaseASL(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered);
struct BaseBranchReturnType {
	bool Sign;
	uint8_t UnSignedOperand;
};
BaseBranchReturnType BaseBranch();

void SBC(std::function<uint8_t* ()> GetDataFunc, uint8_t InstructionLength);
void ADC(std::function<uint8_t* ()> GetDataFunc, uint8_t InstructionLength);
void  BRK();
void  ORA_PRII();
void  ORA_ZABS();
void  ASL_ZABS();
void  PHP();
void  ORA_IME();
void  ASL_ACC();
void  ORA_ABS();
void  ASL_ABS();
void  BPL();
void  ORA_POII();
void  ORA_ZINX();
void  ASL_ZINX();
void  CLC();
void  ORA_INX_Y();
void  ORA_INX_X();
void  ASL_INX_X();
void  JSR();
void  AND_PRII();
void  BIT_ZABS();
void  AND_ZABS();
void  ROL_ZABS();
void  PLP();
void  AND_IME();
void  ROL_ACC();
void  BIT_ABS();
void  AND_ABS();
void  ROL_ABS();
void  BMI();
void  AND_POII();
void  AND_ZINX();
void  ROL_ZINX();
void  SEC();
void  AND_INX_Y();
void  AND_INX_X();
void  ROL_INX_X();
void  RTI();
void  EOR_PRII();
void  EOR_ZABS();
void  LSR_ZABS();
void  PHA();
void  EOR_IME();
void  LSR_ACC();
void  JMP_ABS();
void  EOR_ABS();
void  LSR_ABS();
void  BVC();
void  EOR_POII();
void  EOR_ZINX();
void  LSR_ZINX();
void  CLI();
void  EOR_INX_Y();
void  EOR_INX_X();
void  LSR_INX_X();
void  RTS();
void  ROR_ZABS();
void  PLA();
void  ROR_ACC();
void  JMP_IND();
void  ROR_ABS();
void  BVS();
void  ROR_ZINX();
void  SEI();
void  ROR_INX_X();
void  STA_PRII();
void  STY_ZABS();
void  STA_ZABS();
void  STX_ZABS();
void  DEY();
void  TXA();
void  STY_ABS();
void  STA_ABS();
void  STX_ABS();
void  BCC();
void  STA_POII();
void  STY_ZINX();
void  STA_ZINX();
void  STX_ZINY();
void  TYA();
void  STA_INX_Y();
void  TXS();
void  STA_INX_X();
void  LDY_IME();
void  LDA_PRII();
void  LDX_IME();
void  LDY_ZABS();
void  LDA_ZABS();
void  LDX_ZABS();
void  TAY();
void  LDA_IME();
void  TAX();
void  LDY_ABS();
void  LDA_ABS();
void  LDX_ABS();
void  BCS();
void  LDA_POII();
void  LDY_ZINX();
void  LDA_ZINX();
void  LDX_ZINY();
void  CLV();
void  LDA_INX_Y();
void  TSX();
void  LDY_INX_X();
void  LDA_INX_X();
void  LDX_INX_Y();
void  CPY_IME();
void  CMP_PRII();
void  CPY_ZABS();
void  CMP_ZABS();
void  DEC_ZABS();
void  INY();
void  CMP_IME();
void  DEX();
void  CPY_ABS();
void  CMP_ABS();
void  DEC_ABS();
void  BNE();
void  CMP_POII();
void  CMP_ZINX();
void  DEC_ZINX();
void  CLD();
void  CMP_INX_Y();
void  CMP_INX_X();
void  DEC_INX_X();
void  CPX_IME();
void  CPX_ZABS();
void  INC_ZABS();
void  INX();
void  NOP();
void  CPX_ABS();
void  INC_ABS();
void  BEQ();
void  INC_ZINX();
void  SED();
void  INC_INX_X();
void LoadRom(std::vector<uint8_t>* Rom) {

	std::ifstream file;
	file.open("test.bin", std::ios::binary);

	if (!file) {
		std::cout << "Rom not found" << std::endl;
		exit(-1);
	}

	*Rom = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});
}