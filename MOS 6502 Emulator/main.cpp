#include "Header.h"


int main() {
	
	std::vector<uint8_t>* Rom = new std::vector<uint8_t>();
	LoadRom(Rom);
	CPUMemory = new uint8_t*[0X10000];
	uint8_t** MemoryForPureCpuTesting = new uint8_t * [0xffff];
	uint8_t* Memory = new uint8_t[0x10000];

	for (int i = 0; i < 0x10000; i++) {
		CPUMemory[i] = &Memory[i];
	}
	//Test rom must be loaded at 0x0a offset
	for (int i = 0; i + 0x000a < 0x10000; i++) {
		*CPUMemory[i + 0x000a] = Rom->at(i);
	}
	set_pc(0x0400);

	while (true) {
	  ExecuteNextInstruction();
	}

	return 0;
}



void  ExecuteNextInstruction() {
	FinishedExecutingCurrentInsctruction = false;

	uint8_t OPCode = *CPUMemory[PC];

	if (PC == PreviousPC) {
		printf("PC:%02x OPCode:%02x after execution PC:%02x A:%02x X:%02x Y:%02x P:%02x SP:%02X (SP+1):%02X N#Inst:%d Memory: %02x\n", PC, OPCode, PC, A, X, Y, P, SP, *CPUMemory[(0b00000001 << 8) | (SP + 1)], NumberOfInstructionsExecuted, *CPUMemory[0x0d]);
		printf("Unplanned Break here\n");
	}
	
	if (NumberOfInstructionsExecuted == 26769590) {
		printf("Planned Break\n");
	}
	if (PC == 0x3469 || PC == 0x346c) {
		printf("CPU TEST PASSED SUCCESSFULLY\n");
		exit(0);
	}
	
	PreviousPC = PC;
	uint16_t PC_Old = PC;//JUST TO keep a copy to display later since instruction will change it
	char str[50];
	
	//IME Immediate
	//ABS Absolute
	//ZABS Zeropage Absolute
	//IMP implied
	//ACC Accumulator
	//INX Indexed
	//ZINX Zero Page Indexed X register only
	//ZINY Zero Page Indexed Y
	//IND Indirect applies only to Jump instruction
	//PRII Pre Indexed Indirect
	//POII Post Indexed Indirect
	//REL Relative // signed number added to PC

	switch (OPCode) {

	case 0x00:  BRK(); break;
	case 0x01:  ORA_PRII(); break;
	case 0x05:  ORA_ZABS(); break;
	case 0x06:  ASL_ZABS(); break;
	case 0x08:  PHP(); break;
	case 0x09:  ORA_IME(); break;
	case 0x0A:  ASL_ACC(); break;
	case 0x0D:  ORA_ABS(); break;
	case 0x0E:  ASL_ABS(); break;
	case 0x10:  BPL(); break;
	case 0x11:  ORA_POII(); break;
	case 0x15:  ORA_ZINX(); break;
	case 0x16:  ASL_ZINX(); break;
	case 0x18:  CLC(); break;
	case 0x19:  ORA_INX_Y(); break;
	case 0x1D:  ORA_INX_X(); break;
	case 0x1E:  ASL_INX_X(); break;
	case 0x20:  JSR(); break;
	case 0x21:  AND_PRII(); break;
	case 0x24:  BIT_ZABS(); break;
	case 0x25:  AND_ZABS(); break;
	case 0x26:  ROL_ZABS(); break;
	case 0x28:  PLP(); break;
	case 0x29:  AND_IME(); break;
	case 0x2A:  ROL_ACC(); break;
	case 0x2C:  BIT_ABS(); break;
	case 0x2D:  AND_ABS(); break;
	case 0x2E:  ROL_ABS(); break;
	case 0x30:  BMI(); break;
	case 0x31:  AND_POII(); break;
	case 0x35:  AND_ZINX(); break;
	case 0x36:  ROL_ZINX(); break;
	case 0x38:  SEC(); break;
	case 0x39:  AND_INX_Y(); break;
	case 0x3D:  AND_INX_X(); break;
	case 0x3E:  ROL_INX_X(); break;
	case 0x40:  RTI(); break;
	case 0x41:  EOR_PRII(); break;
	case 0x45:  EOR_ZABS(); break;
	case 0x46:  LSR_ZABS(); break;
	case 0x48:  PHA(); break;
	case 0x49:  EOR_IME(); break;
	case 0x4A:  LSR_ACC(); break;
	case 0x4C:  JMP_ABS(); break;
	case 0x4D:  EOR_ABS(); break;
	case 0x4E:  LSR_ABS(); break;
	case 0x50:  BVC(); break;
	case 0x51:  EOR_POII(); break;
	case 0x55:  EOR_ZINX(); break;
	case 0x56:  LSR_ZINX(); break;
	case 0x58:  CLI(); break;
	case 0x59:  EOR_INX_Y(); break;
	case 0x5D:  EOR_INX_X(); break;
	case 0x5E:  LSR_INX_X(); break;
	case 0x60:  RTS(); break;
	case 0x61:  ADC(GetPointerToDataInCPUMemoryUsing_PRII_MODE,2); break;
	case 0x65:  ADC(GetPointerToDataInCPUMemoryUsing_ZABS_MODE,2); break;
	case 0x66:  ROR_ZABS(); break;
	case 0x68:  PLA(); break;
	case 0x69:  ADC(GetPointerToDataInCPUMemoryUsing_IME_MODE,2); break;
	case 0x6A:  ROR_ACC(); break;
	case 0x6C:  JMP_IND(); break;
	case 0x6D:  ADC(GetPointerToDataInCPUMemoryUsing_ABS_MODE,3); break;
	case 0x6E:  ROR_ABS(); break;
	case 0x70:  BVS(); break;
	case 0x71:  ADC(GetPointerToDataInCPUMemoryUsing_POII_MODE,2); break;
	case 0x75:  ADC(GetPointerToDataInCPUMemoryUsing_ZINX_MODE,2); break;
	case 0x76:  ROR_ZINX(); break;
	case 0x78:  SEI(); break;
	case 0x79:  ADC(GetPointerToDataInCPUMemoryUsing_INX_Y_MODE,3); break;
	case 0x7D:  ADC(GetPointerToDataInCPUMemoryUsing_INX_X_MODE,3); break;
	case 0x7E:  ROR_INX_X(); break;
	case 0x81:  STA_PRII(); break;
	case 0x84:  STY_ZABS(); break;
	case 0x85:  STA_ZABS(); break;
	case 0x86:  STX_ZABS(); break;
	case 0x88:  DEY(); break;
	case 0x8A:  TXA(); break;
	case 0x8C:  STY_ABS(); break;
	case 0x8D:  STA_ABS(); break;
	case 0x8E:  STX_ABS(); break;
	case 0x90:  BCC(); break;
	case 0x91:  STA_POII(); break;
	case 0x94:  STY_ZINX(); break;
	case 0x95:  STA_ZINX(); break;
	case 0x96:  STX_ZINY(); break;
	case 0x98:  TYA(); break;
	case 0x99:  STA_INX_Y(); break;
	case 0x9A:  TXS(); break;
	case 0x9D:  STA_INX_X(); break;
	case 0xA0:  LDY_IME(); break;
	case 0xA1:  LDA_PRII(); break;
	case 0xA2:  LDX_IME(); break;
	case 0xA4:  LDY_ZABS(); break;
	case 0xA5:  LDA_ZABS(); break;
	case 0xA6:  LDX_ZABS(); break;
	case 0xA8:  TAY(); break;
	case 0xA9:  LDA_IME(); break;
	case 0xAA:  TAX(); break;
	case 0xAC:  LDY_ABS(); break;
	case 0xAD:  LDA_ABS(); break;
	case 0xAE:  LDX_ABS(); break;
	case 0xB0:  BCS(); break;
	case 0xB1:  LDA_POII(); break;
	case 0xB4:  LDY_ZINX(); break;
	case 0xB5:  LDA_ZINX(); break;
	case 0xB6:  LDX_ZINY(); break;
	case 0xB8:  CLV(); break;
	case 0xB9:  LDA_INX_Y(); break;
	case 0xBA:  TSX(); break;
	case 0xBC:  LDY_INX_X(); break;
	case 0xBD:  LDA_INX_X(); break;
	case 0xBE:  LDX_INX_Y(); break;
	case 0xC0:  CPY_IME(); break;
	case 0xC1:  CMP_PRII(); break;
	case 0xC4:  CPY_ZABS(); break;
	case 0xC5:  CMP_ZABS(); break;
	case 0xC6:  DEC_ZABS(); break;
	case 0xC8:  INY(); break;
	case 0xC9:  CMP_IME(); break;
	case 0xCA:  DEX(); break;
	case 0xCC:  CPY_ABS(); break;
	case 0xCD:  CMP_ABS(); break;
	case 0xCE:  DEC_ABS(); break;
	case 0xD0:  BNE(); break;
	case 0xD1:  CMP_POII(); break;
	case 0xD5:  CMP_ZINX(); break;
	case 0xD6:  DEC_ZINX(); break;
	case 0xD8:  CLD(); break;
	case 0xD9:  CMP_INX_Y(); break;
	case 0xDD:  CMP_INX_X(); break;
	case 0xDE:  DEC_INX_X(); break;
	case 0xE0:  CPX_IME(); break;
	case 0xE1:  SBC(GetPointerToDataInCPUMemoryUsing_PRII_MODE, 2); break;
	case 0xE4:  CPX_ZABS(); break;
	case 0xE5:  SBC(GetPointerToDataInCPUMemoryUsing_ZABS_MODE, 2); break;
	case 0xE6:  INC_ZABS(); break;
	case 0xE8:  INX(); break;
	case 0xE9:  SBC(GetPointerToDataInCPUMemoryUsing_IME_MODE, 2); break;
	case 0xEA:  NOP(); break;
	case 0xEC:  CPX_ABS(); break;
	case 0xED:  SBC(GetPointerToDataInCPUMemoryUsing_ABS_MODE, 3); break;
	case 0xEE:  INC_ABS(); break;
	case 0xF0:  BEQ(); break;
	case 0xF1:  SBC(GetPointerToDataInCPUMemoryUsing_POII_MODE, 2); break;
	case 0xF5:  SBC(GetPointerToDataInCPUMemoryUsing_ZINX_MODE, 2); break;
	case 0xF6:  INC_ZINX(); break;
	case 0xF8:  SED(); break;
	case 0xF9:  SBC(GetPointerToDataInCPUMemoryUsing_INX_Y_MODE, 3); break;
	case 0xFD:  SBC(GetPointerToDataInCPUMemoryUsing_INX_X_MODE, 3); break;
	case 0xFE:  INC_INX_X(); break;
	default:std::cout<<"OPCode not supported"<<std::endl; break;
	}
	NumberOfInstructionsExecuted++;
	//printf("PC:%02x OPCode:%02x after execution PC:%02x A:%02x X:%02x Y:%02x P:%02x SP:%02X (SP+1):%02X N#Inst:%d Memory: %02x\n", PC_Old, OPCode, PC, A, X, Y, P,SP,*CPUMemory[(0b00000001<<8)|(SP+1)],NumberOfInstructionsExecuted,*CPUMemory[0x0d]);

}
/////////   ADC_INSTRUCTIONS
void BaseOverflowCheckOnAddition(uint8_t Value1, uint8_t Value2) {

	if (GetNegativeFromData(&Value1) != GetNegativeFromData(&Value2)) {
		//no overflow
		ResetOverflow();
	}
	else {
		//overflow is a possibility
		uint8_t sum = Value1 + Value2;

		if (GetNegativeFromData(&sum) != GetNegativeFromData(&Value2)) {
			//overflow happened
			SetOverflow();
		}
		else {
			ResetOverflow();
		}
	}

}

void ADC(std::function<uint8_t* ()> GetDataFunc, uint8_t InstructionLength) {
	//2 bytes instruction
	if (!GetDecimalMode()) {
		uint8_t Carry = 0;
		if (GetCarry()) {
			uint8_t SmallOperand;
			uint8_t BigOperand;
			if (A > *GetDataFunc()) {//thin in two's complement if both are positive then it does not matter which one is bigger
																   //if both negative the bigger one in unsigned form is the smaller
																   //if of different signs then the bigger one in unsigned form is the smaller
				SmallOperand = A;
				BigOperand = *GetDataFunc();
			}
			else {
				SmallOperand = *GetDataFunc();
				BigOperand = A;
			}
			Carry = 1;
			BaseOverflowCheckOnAddition(SmallOperand, Carry);
			if (!GetOverflow()) {
				uint8_t ByteSum = SmallOperand + 0x1;
				BaseOverflowCheckOnAddition(BigOperand, ByteSum);
			}
		}
		else {
			BaseOverflowCheckOnAddition(A, *GetDataFunc());
		}
		uint16_t sum = A + *GetDataFunc() + Carry;

		if (sum & 0x0100) { SetCarry(); }
		else { ResetCarry(); }
		A = (uint8_t)sum;//truncated
		BaseSZCheck(InstructionLength, A);
	}
	else {
		uint8_t LowerNibbleOperand = 0x0f & *GetDataFunc();
		uint8_t HigherNibbleOperand = (0xf0 & *GetDataFunc())>>4;
		uint8_t LowerNibbleA = 0x0f & A;
		uint8_t HigherNibbleA = (0xf0 & A)>>4;
		uint8_t CarryOfBCD=0;
		uint8_t result = LowerNibbleOperand + LowerNibbleA;
		if (GetCarry()) {
			result += 0x01;
		}
		if (result > 0x09) {
			result += 0x06;
		}
		CarryOfBCD = (result & 0x10)>>4;
		uint8_t temp = CarryOfBCD + HigherNibbleOperand + HigherNibbleA;
		if (temp > 0x09) {
			temp += 0x06;
		}
		if (temp & 0x10) {
			SetCarry();
		}
		else {
			ResetCarry();
		}
		A = ((temp & 0x0f) << 4) | (result & 0x0f);
		printf("Has used ADC in BCD Mode \n");
		BaseSZCheck(InstructionLength, A);
	}
	
}

////////////////  END    ///////////////


/////////   SBC_INSTRUCTIONS

void BaseOverflowCheckOnSubtraction(uint8_t Value1, uint8_t Value2) {

	if (Value1 == 0x80 && Value2 == 0x01 && !GetCarry()) {//since 80 is -128 it has no +128 representation but the processor treats it as +128 when checking overflow
		ResetOverflow(); //so +128 - 1(from carry)=+127 no carry
	}
	else

		if (GetNegativeFromData(&Value1) == GetNegativeFromData(&Value2)) {
			//no overflow
			ResetOverflow();
		}
		else {
			//overflow is a possibility
			uint8_t Diff = Value1 - Value2;

			if (GetNegativeFromData(&Diff) != GetNegativeFromData(&Value1)) {//value 1 is not interchanble with value2
				//overflow happened
				SetOverflow();
			}
			else {
				ResetOverflow();
			}
		}

	//BaseOverflowCheckOnAddition(Value1, (~Value2 + 0x01));


}
void SBC(std::function<uint8_t* ()> GetDataFunc, uint8_t InstructionLength) {
	// E9 2 bytes instruction
	if (!GetDecimalMode()) {
		uint8_t NotCarry = 0;
		if (!GetCarry()) {
			NotCarry = 1;
			BaseOverflowCheckOnSubtraction((~*GetDataFunc() + 1), NotCarry);
			if (!GetOverflow()) {
				uint8_t ByteDiff = (~*GetDataFunc() + 1) - NotCarry;
				BaseOverflowCheckOnAddition(A, ByteDiff);
			}
		}
		else {
			BaseOverflowCheckOnSubtraction(A, *GetDataFunc());
		}
		if (A == 0 && *GetDataFunc() == 0 && GetCarry()) {
			SetCarry();
			A = 0x00;
		}
		else if (*GetDataFunc() == 0 && GetCarry()) {
			SetCarry();
			//A will not change
		}
		else {
			uint16_t result = A + (uint8_t)(~((uint8_t)(*GetDataFunc() + NotCarry)) + 0x01);
			if (result & 0x100) {
				SetCarry();
			}
			else {
				ResetCarry();
			}

			A = (uint8_t)result;
		}
		BaseSZCheck(InstructionLength, A);
	}
	else {
		uint8_t LowerNibbleOperand = 0x0f & *GetDataFunc();
		uint8_t HigherNibbleOperand = (0xf0 & *GetDataFunc()) >> 4;
		uint8_t LowerNibbleA = 0x0f & A;
		uint8_t HigherNibbleA = (0xf0 & A) >> 4;
		uint8_t BorrowOfBCD = 0;
		if (!GetCarry()) {
			LowerNibbleOperand += 0x01;
		}
		uint8_t result = LowerNibbleA - LowerNibbleOperand;

		BorrowOfBCD = (result & 0x10) >> 4;

		if (result > 0x09) {
			result -= 0x06;
		}
		
		uint8_t temp = HigherNibbleA - HigherNibbleOperand-BorrowOfBCD;
		if (temp > 0x09) {
			temp -= 0x06;
		}
		if (temp & 0x10) {
			ResetCarry();
		}
		else {
			SetCarry();
		}
		A = ((temp & 0x0f) << 4) | (result & 0x0f);
		printf("Has used SBC in BCD Mode \n");
		BaseSZCheck(InstructionLength, A);
	}
}

////////////////  END    ///////////////

/////////   LSR_INSTRUCTIONS
void BaseLSR(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered) {
	if (*DataThaWillBeAltered & 0x01) { SetCarry(); }
	else { ResetCarry(); }

	*DataThaWillBeAltered = *DataThaWillBeAltered >> 1;
	BaseSZCheck(InstructionLength, *DataThaWillBeAltered);

}
void LSR_ACC() {
	//opcode 0x4A 1byte long
	BaseLSR(1, &A);
}
void LSR_ZABS() {
	//opcode 0x46 2bytes long
	BaseLSR(2, GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void LSR_ZINX() {
	//opcode 0x56 2 bytes long
	BaseLSR(2, GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void LSR_ABS() {
	//opcode 0x4E 3 bytes long
	BaseLSR(3, GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
void LSR_INX_X() {
	//opcode 0x5E 3 bytes long
	BaseLSR(3, GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
////////////////  END    ///////////////

void BaseROR(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered) {
	bool CarryWas = GetCarry();
	if (*DataThaWillBeAltered & 0x01) {
		SetCarry();
	}
	else { ResetCarry(); }
	*DataThaWillBeAltered = *DataThaWillBeAltered >> 1;
	if (CarryWas == true) {
		*DataThaWillBeAltered = *DataThaWillBeAltered | 0x80;
	}
	BaseSZCheck(InstructionLength, *DataThaWillBeAltered);
}
/////////   ROR_INSTRUCTION
void ROR_ACC() {
	//opcode 0x6A 1 byte long
	BaseROR(1, &A);
}
void ROR_ZABS() {
	//opcode 0x66 2 byte long
	BaseROR(2, GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void ROR_ZINX() {
	//opcode 0x76 2 byte long
	BaseROR(2, GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void ROR_ABS() {
	//opcode 0x6E 3 byte long
	BaseROR(3, GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
void ROR_INX_X() {
	//opcode 0x7E 3 byte long
	BaseROR(3, GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
////////////////  END    ///////////////



void BaseROL(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered) {
	bool CarryWas = GetCarry();
	if (GetNegativeFromData(DataThaWillBeAltered)) {
		SetCarry();
	}
	else { ResetCarry(); }
	*DataThaWillBeAltered = *DataThaWillBeAltered << 1;
	if (CarryWas == true) {
		*DataThaWillBeAltered = *DataThaWillBeAltered | 0x01;
	}
	BaseSZCheck(InstructionLength, *DataThaWillBeAltered);
}
/////////   ROL_INSTRUCTION
void ROL_ACC() {
	//opcode 0x2A 1 byte long
	BaseROL(1, &A);
}
void ROL_ZABS() {
	//opcode 0x26 2 byte long
	BaseROL(2, GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void ROL_ZINX() {
	//opcode 0x36 2 byte long
	BaseROL(2, GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void ROL_ABS() {
	//opcode 0x2E 3 byte long
	BaseROL(3, GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
void ROL_INX_X() {
	//opcode 0x3E 3 byte long
	BaseROL(3, GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
////////////////  END    ///////////////

/////////   EOR_INSTRUCTION
void EOR_IME() {
	//OPCOE 0x49 2BYTES LONG
	A = A ^ *GetPointerToDataInCPUMemoryUsing_IME_MODE();
	BaseSZCheck(2, A);
}
void EOR_ZABS() {
	//opcode 0x45 2bytes long
	A = A ^ *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	BaseSZCheck(2, A);
}
void EOR_ZINX() {
	//opcode 0x55 2bytes long
	A = A ^ *GetPointerToDataInCPUMemoryUsing_ZINX_MODE();
	BaseSZCheck(2, A);
}
void EOR_ABS() {
	//opcode 4D 3bytes long

	A = A ^ *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	BaseSZCheck(3, A);
}
void EOR_INX_X() {
	//opcode 5D 3bytes long
	A = A ^ *GetPointerToDataInCPUMemoryUsing_INX_X_MODE();
	BaseSZCheck(3, A);
}
void EOR_INX_Y() {
	//opcode 59 3bytes long
	A = A ^ *GetPointerToDataInCPUMemoryUsing_INX_Y_MODE();
	BaseSZCheck(3, A);
}
void EOR_PRII() {
	//opcode 41  2bytes long

	A = A ^ *GetPointerToDataInCPUMemoryUsing_PRII_MODE();
	BaseSZCheck(2, A);
}
void EOR_POII() {
	//opcode 51  2bytes long
	A = A ^ *GetPointerToDataInCPUMemoryUsing_POII_MODE();
	BaseSZCheck(2, A);
}
////////////////  END    ///////////////


//////////// BRK INSTRUCTION
void BRK() {
	//OPCODE 0x00 1 byte long
	PC = PC + 2;
	PushPCtoStack();

	PushToStack(P | 0b00110000);//read about it here called "The B flag" https://wiki.nesdev.org/w/index.php?title=Status_flags
	SetInterruptDisbale();
	PC = Get16BitAddressFromMemoryLocation(0xFFFE);
	FinishedExecutingCurrentInsctruction = true;
}
////////////    END ////////////

///////////CMP_INSTRUCTIONS
void BaseCOMPARE(uint8_t InstructionLength, uint8_t RegisterValue, uint8_t MemoryValue) {
	if (RegisterValue >= MemoryValue) {
		//we dont need a borrow thus SetCarry
		SetCarry();
	}
	else {
		ResetCarry();
	}
	if (RegisterValue == MemoryValue) {
		SetZero();
	}
	else {
		ResetZero();
	}
	MemoryValue = ~MemoryValue;
	MemoryValue = MemoryValue + 0b00000001;
	uint8_t data = RegisterValue + MemoryValue;
	if (GetNegativeFromData(&data)) { SetNegative(); }
	else { ResetNegative(); }

	PC = PC + InstructionLength;
	FinishedExecutingCurrentInsctruction = true;
}


void CMP_IME() {
	//OPCODE 0xC9
	BaseCOMPARE(2, A, *GetPointerToDataInCPUMemoryUsing_IME_MODE());
}
void CMP_ZABS() {
	//opcode 0xC5
	BaseCOMPARE(2, A, *GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void CMP_ZINX() {
	//opcode 0xD5
	BaseCOMPARE(2, A, *GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void CMP_ABS() {
	//opcode 0xCD
	BaseCOMPARE(3, A, *GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
void CMP_INX_X() {
	//opcode 0xDD
	BaseCOMPARE(3, A, *GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
void CMP_INX_Y() {
	//opcode 0xD9
	BaseCOMPARE(3, A, *GetPointerToDataInCPUMemoryUsing_INX_Y_MODE());
}
void CMP_PRII() {
	//opcode 0xC1
	BaseCOMPARE(2, A, *GetPointerToDataInCPUMemoryUsing_PRII_MODE());
}
void CMP_POII() {
	//opcode 0xD1
	BaseCOMPARE(2, A, *GetPointerToDataInCPUMemoryUsing_POII_MODE());
}
////////////    END //////////// 

///////////CPX_INSTRUCTIONS
void CPX_IME() {
	//OPCODE 0xE0
	BaseCOMPARE(2, X, *GetPointerToDataInCPUMemoryUsing_IME_MODE());
}
void CPX_ZABS() {
	//opcode 0xE4
	BaseCOMPARE(2, X, *GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void CPX_ABS() {
	//opcode 0xEC
	BaseCOMPARE(3, X, *GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
////////////    END //////////// 


///////////CPY_INSTRUCTIONS
void CPY_IME() {
	//OPCODE 0xC0
	BaseCOMPARE(2, Y, *GetPointerToDataInCPUMemoryUsing_IME_MODE());
}
void CPY_ZABS() {
	//opcode 0xC4
	BaseCOMPARE(2, Y, *GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void CPY_ABS() {
	//opcode 0xCC
	BaseCOMPARE(3, Y, *GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
////////////    END //////////// 


///////// DECREMENT INSTRUCTIONS
void DEC_ZABS() {
	//OPCODE 0xC6
	*GetPointerToDataInCPUMemoryUsing_ZABS_MODE() -= 1;
	BaseSZCheck(2, *GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void DEC_INX_X() {
	//OPCODE 0xDE
	*GetPointerToDataInCPUMemoryUsing_INX_X_MODE() -= 1;
	BaseSZCheck(3, *GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
void DEC_ZINX() {
	//OPCODE 0xD6
	*GetPointerToDataInCPUMemoryUsing_ZINX_MODE() -= 1;
	BaseSZCheck(2, *GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void DEC_ABS() {
	//OPCODE 0xCE
	*GetPointerToDataInCPUMemoryUsing_ABS_MODE() -= 1;
	BaseSZCheck(3, *GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}

///////////////////////////////////
void DEX() {
	//0xCA
	X = X - 1;
	BaseSZCheck(1, X);

}
void DEY() {
	//0x88
	Y = Y - 1;
	BaseSZCheck(1, Y);
}
////////////    END //////////// 



///////// INCREMENT INSTRUCTIONS
void INC_ZABS() {
	//OPCODE 0xE6
	*GetPointerToDataInCPUMemoryUsing_ZABS_MODE() += 1;
	BaseSZCheck(2, *GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void INC_INX_X() {
	//OPCODE 0xFE
	*GetPointerToDataInCPUMemoryUsing_INX_X_MODE() += 1;
	BaseSZCheck(3, *GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
void INC_ZINX() {
	//OPCODE 0xF6
	*GetPointerToDataInCPUMemoryUsing_ZINX_MODE() += 1;
	BaseSZCheck(2, *GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void INC_ABS() {
	//OPCODE 0xEE
	*GetPointerToDataInCPUMemoryUsing_ABS_MODE() += 1;
	BaseSZCheck(3, *GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}

///////////////////////////////////
void INX() {
	//0xE8
	X = X + 1;
	BaseSZCheck(1, X);

}
void INY() {
	//0xC8
	Y = Y + 1;
	BaseSZCheck(1, Y);
}
////////////    END //////////// 

///////// JUMP INSTRUCTIONS
void JSR() {
	//opcode 0x20 
	uint16_t address = Get16BitAddressFromMemoryLocation(PC + 1);

	PC = PC + 2;
	PushPCtoStack();

	PC = address;
	FinishedExecutingCurrentInsctruction = true;
}
void JMP_ABS() {
	//OPCODE 0x4C
	PC = Get16BitAddressFromMemoryLocation(PC + 1);
	FinishedExecutingCurrentInsctruction = true;
}
void JMP_IND() {
	//opcode 0x6C
	uint8_t PC_L = *CPUMemory[Get16BitAddressFromMemoryLocation(PC + 1)];
	uint8_t PC_H = *CPUMemory[Get16BitAddressFromMemoryLocation(PC + 1) + 1];
	PC = (PC_H << 8) | PC_L;
	FinishedExecutingCurrentInsctruction = true;
}
////////////    END //////////// 

/////////   LDA INSTRUCTIONS 
void LDA_IME() {
	//opcode 0xA9 2 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_IME_MODE();
	BaseSZCheck(2, A);
}
void LDA_ZABS() {
	//opcode 0xA5 2 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	BaseSZCheck(2, A);
}
void LDA_ZINX() {
	//opcode 0xB5 2 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_ZINX_MODE();
	BaseSZCheck(2, A);
}
void LDA_ABS() {
	//opcode 0xAD 3 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	BaseSZCheck(3, A);
}
void LDA_INX_X() {
	//opcode 0xBD 3 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_INX_X_MODE();
	BaseSZCheck(3, A);
}
void LDA_INX_Y() {
	//opcode 0xB9 3 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_INX_Y_MODE();
	BaseSZCheck(3, A);
}
void LDA_PRII() {
	//opcode 0xA1 2 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_PRII_MODE();
	BaseSZCheck(2, A);
}
void LDA_POII() {
	//opcode 0xB1 2 bytes long
	A = *GetPointerToDataInCPUMemoryUsing_POII_MODE();
	BaseSZCheck(2, A);
}
////////////////  END    /////////////// 

//Check the sign and whether data is zero and Set/Reset flags appropriatly
void BaseSZCheck(uint8_t InstructionLength, uint8_t DataToCheck) {
	if (DataToCheck == 0) { SetZero(); }
	else { ResetZero(); }

	if (GetNegativeFromData(&DataToCheck)) { SetNegative(); }
	else { ResetNegative(); }

	PC = PC + InstructionLength;
	FinishedExecutingCurrentInsctruction = true;
}


/////////   AND_INSTRUCTIONS
void AND_IME() {
	//OPCOE 0x29 2BYTES LONG
	A = A & *GetPointerToDataInCPUMemoryUsing_IME_MODE();
	BaseSZCheck(2, A);
}
void AND_ZABS() {
	//opcode 0x25 2bytes long
	A = A & *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	BaseSZCheck(2, A);
}
void AND_ZINX() {
	//opcode 0x35 2bytes long
	A = A & *GetPointerToDataInCPUMemoryUsing_ZINX_MODE();
	BaseSZCheck(2, A);
}
void AND_ABS() {
	//opcode 2D 3bytes long

	A = A & *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	BaseSZCheck(3, A);
}
void AND_INX_X() {
	//opcode 3D 3bytes long
	A = A & *GetPointerToDataInCPUMemoryUsing_INX_X_MODE();
	BaseSZCheck(3, A);
}
void AND_INX_Y() {
	//opcode 39 3bytes long
	A = A & *GetPointerToDataInCPUMemoryUsing_INX_Y_MODE();
	BaseSZCheck(3, A);
}
void AND_PRII() {
	//opcode 21  2bytes long

	A = A & *GetPointerToDataInCPUMemoryUsing_PRII_MODE();
	BaseSZCheck(2, A);
}
void AND_POII() {
	//opcode 31  2bytes long

	A = A & *GetPointerToDataInCPUMemoryUsing_POII_MODE();
	BaseSZCheck(2, A);
}
////////////////  END    ///////////////

/////////   ASL_INSTRUCTIONS
void BaseASL(uint8_t InstructionLength, uint8_t* DataThaWillBeAltered) {
	if (GetNegativeFromData(DataThaWillBeAltered)) { SetCarry(); }
	else { ResetCarry(); }

	*DataThaWillBeAltered = *DataThaWillBeAltered << 1;

	if (*DataThaWillBeAltered == 0) { SetZero(); }
	else { ResetZero(); }
	if (GetNegativeFromData(DataThaWillBeAltered)) { SetNegative(); }
	else { ResetNegative(); }

	PC = PC + InstructionLength;
	FinishedExecutingCurrentInsctruction = true;
}
void ASL_ACC() {
	//opcode 0x0A 1byte long
	BaseASL(1, &A);
}
void ASL_ZABS() {
	//opcode 0x06 2bytes long
	BaseASL(2, GetPointerToDataInCPUMemoryUsing_ZABS_MODE());
}
void ASL_ZINX() {
	//opcode 0x16 2 bytes long
	BaseASL(2, GetPointerToDataInCPUMemoryUsing_ZINX_MODE());
}
void ASL_ABS() {
	//opcode 0x0E 3 bytes long
	BaseASL(3, GetPointerToDataInCPUMemoryUsing_ABS_MODE());
}
void ASL_INX_X() {
	//opcode 0x1E 3 bytes long
	BaseASL(3, GetPointerToDataInCPUMemoryUsing_INX_X_MODE());
}
////////////////  END    ///////////////

/////////   BRANCH FAMILY INSTRUCTIONS 
BaseBranchReturnType BaseBranch() {
	uint8_t SignedOperand = *GetPointerToDataInCPUMemoryUsing_IME_MODE(); //this jump value is signed
	bool Sign = GetNegativeFromData(&SignedOperand);
	uint8_t UnSignedOperand = SignedOperand;

	if (Sign) { //the number is negative in two's complement so we need to revert it
		UnSignedOperand = ~SignedOperand;//reverting the two's complement
		UnSignedOperand = UnSignedOperand + 0b00000001;//reverting the two's complement
	}

	return { Sign , UnSignedOperand };
}
void BCC() {
	//opcode 0x90 2bytes
	//THE DOC FOR RELATIVE ADDRESSING IS CONTRADICTING THE DOC FOR PROGRAM COUNTER
	//IN THAT PC POINTS TO CURRENT INSTRUCTION BEING EXECUTED(which is used now) 
	// AND POINTING TO NEXT INSTRUCTION TO BE EXECUTED
	//AFTER LOOKING AT PROGRAMMING MANUAL AND TESTING I FOUND THAT PC MUST POINT TO THE NEXT INSTRUCTION TO BE EXECRUTED
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetCarry() == false)
	{
		PC = PC + 2;
		if (BranchData.Sign) {
			PC = PC - BranchData.UnSignedOperand;
		}
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BCS() {
	//opcode 0xB0 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetCarry() == true)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BEQ() {
	//opcode 0xF0 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetZero() == true)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BMI() {
	//opcode 0x30 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetNegative() == true)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BNE() {
	//opcode 0xD0 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetZero() == false)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BPL() {
	//opcode 0x10 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetNegative() == false)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BVC() {
	//opcode 0x50 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetOverflow() == false)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
void BVS() {
	//opcode 0x70 2 BYTES LONG
	BaseBranchReturnType BranchData = BaseBranch();

	if (GetOverflow() == true)
	{
		PC = PC + 2;
		if (BranchData.Sign) { PC = PC - BranchData.UnSignedOperand; }
		else { PC = PC + BranchData.UnSignedOperand; }
	}

	else { PC = PC + 2; }

	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    ///////////////

/////////   BIT_INSTRUCTIONS
void BIT_ABS() {
	//opcode 0x2C 3 byte long
	uint8_t AndResult = A & *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	bool IsBit7Set = *GetPointerToDataInCPUMemoryUsing_ABS_MODE() & 0b10000000;//bit7 aka M7 in docs
	bool IsBit6Set = *GetPointerToDataInCPUMemoryUsing_ABS_MODE() & 0b01000000;//bit6 aka M6 in docs

	if (IsBit7Set) { SetNegative(); }
	else { ResetNegative(); }

	if (IsBit6Set) { SetOverflow(); }
	else { ResetOverflow(); }

	if (AndResult == 0) { SetZero(); }
	else { ResetZero(); }
	PC = PC + 3;
	FinishedExecutingCurrentInsctruction = true;
}
void BIT_ZABS() {
	//opcode 0x24 2 byte long
	uint8_t AndResult = A & *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	bool IsBit7Set = *GetPointerToDataInCPUMemoryUsing_ZABS_MODE() & 0b10000000;//bit7 aka M7 in docs
	bool IsBit6Set = *GetPointerToDataInCPUMemoryUsing_ZABS_MODE() & 0b01000000;//bit6 aka M6 in docs

	if (IsBit7Set) { SetNegative(); }
	else { ResetNegative(); }

	if (IsBit6Set) { SetOverflow(); }
	else { ResetOverflow(); }

	if (AndResult == 0) { SetZero(); }
	else { ResetZero(); }
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    /////////////// 

/////////   OperationsOnFlags_INSTRUCTIONS
void CLC() {
	//opcode 0x18 1 byte long
	ResetCarry();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void CLD() {
	//opcode 0xD8 1 byte long
	ResetDecimalMode();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void CLI() {
	//opcode 0x58 1 byte long
	ResetInterruptDisable();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void CLV() {
	//opcode 0xB8 1 byte long
	ResetOverflow();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}

void SEC() {
	//opcode 0x38 1 byte long
	SetCarry();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void SED() {
	//opcode 0xF8 1 byte long
	SetDecimalMode();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void SEI() {
	//opcode 0x78 1 byte long
	SetInterruptDisbale();
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    /////////////// 

/////////   TRANSFER FAMILY INSTRUCTIONS 
void TYA() {
	//opcode 0x98 1 byte long
	A = Y;
	if (GetNegativeFromData(&A)) { SetNegative(); }
	else { ResetNegative(); }
	if (A == 0) { SetZero(); }
	else { ResetZero(); }

	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void TXS() {
	//OPCODE 0x9A 1 byte long
	SP = X;
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void TXA() {
	//opcode 0x8A 1 byte long
	A = X;
	if (GetNegativeFromData(&A)) { SetNegative(); }
	else { ResetNegative(); }
	if (A == 0) { SetZero(); }
	else { ResetZero(); }

	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void TSX() {
	//opcode 0xBA 1 byte long
	X = SP;
	if (GetNegativeFromData(&X)) { SetNegative(); }
	else { ResetNegative(); }
	if (X == 0) { SetZero(); }
	else { ResetZero(); }

	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void TAY() {
	//opcode 0xA8 1 byte long
	Y = A;
	if (GetNegativeFromData(&Y)) { SetNegative(); }
	else { ResetNegative(); }
	if (Y == 0) { SetZero(); }
	else { ResetZero(); }

	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void TAX() {
	//opcode 0xAA 1 byte long
	X = A;
	if (GetNegativeFromData(&X)) { SetNegative(); }
	else { ResetNegative(); }
	if (X == 0) { SetZero(); }
	else { ResetZero(); }

	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    /////////////// 

/////////   STA INSTRUCTIONS 
void STA_ZABS() {
	//opcode 0x85 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_ZABS_MODE() = A;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STA_ZINX() {
	//opcode 0x95 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_ZINX_MODE() = A;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STA_ABS() {
	//opcode 0x8D 3 bytes long
	*GetPointerToDataInCPUMemoryUsing_ABS_MODE() = A;
	PC = PC + 3;
	FinishedExecutingCurrentInsctruction = true;
}
void STA_INX_X() {
	//opcode 0x9D 3 bytes long
	*GetPointerToDataInCPUMemoryUsing_INX_X_MODE() = A;
	PC = PC + 3;
	FinishedExecutingCurrentInsctruction = true;
}
void STA_INX_Y() {
	//opcode 0x99 3 bytes long
	*GetPointerToDataInCPUMemoryUsing_INX_Y_MODE() = A;
	PC = PC + 3;
	FinishedExecutingCurrentInsctruction = true;
}
void STA_PRII() {
	//opcode 0x81 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_PRII_MODE() = A;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STA_POII() {
	//opcode 0x91 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_POII_MODE() = A;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    /////////////// 

/////////   STX INSTRUCTIONS 
void STX_ZABS() {
	//opcode 0x86 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_ZABS_MODE() = X;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STX_ZINY() {
	//opcode 0x96 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_ZINY_MODE() = X;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STX_ABS() {
	//opcode 0x8E 3 bytes long
	*GetPointerToDataInCPUMemoryUsing_ABS_MODE() = X;
	PC = PC + 3;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    /////////////// 

/////////   STY INSTRUCTIONS 
void STY_ZABS() {
	//opcode 0x84 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_ZABS_MODE() = Y;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STY_ZINX() {
	//opcode 0x94 2 bytes long
	*GetPointerToDataInCPUMemoryUsing_ZINX_MODE() = Y;
	PC = PC + 2;
	FinishedExecutingCurrentInsctruction = true;
}
void STY_ABS() {
	//opcode 0x8C 3 bytes long
	*GetPointerToDataInCPUMemoryUsing_ABS_MODE() = Y;
	PC = PC + 3;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    /////////////// 

/////////   OPERATIONS WITH STACK INSTRUCTIONS 
   //NOTE: JSR of JUMP_INSTRUCTIONS also uses the stack
/// ////////////////////////////
void RTS() {
	//opcode 0x60 1 byte long
	PC = PopPCfromStack() + 0x01;
	FinishedExecutingCurrentInsctruction = true;
}
void RTI() {
	//opcode 0x40 1 byte long
	P = PopStack();
	PC = PopPCfromStack();
	FinishedExecutingCurrentInsctruction = true;
}
///////////////////////////////
void PLP() {
	//opcode 0x28 1 BYTE LONG
	P = PopStack();
	P = P & 0b11001111;
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void PLA() {
	//opcode 0x68 1 BYTELONG
	A = PopStack();
	if (GetNegativeFromData(&A)) {
		SetNegative();
	}
	else {
		ResetNegative();
	}
	if (A == 0x00) {
		SetZero();
	}
	else {
		ResetZero();
	}
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void PHP() {
	//opcode 0x08 1 BYTELONG

	//results set bit 4 and 5 only on stack P should not be altered
	PushToStack(P | 0b00110000);//read about it here called "The B flag" https://wiki.nesdev.org/w/index.php?title=Status_flags
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
void PHA() {
	//opcode 0x48 1 BYTELONG
	PushToStack(A);
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}

////////////////  END    /////////////// 

/////////   NOP INSTRUCTION 
void NOP() {
	//opcode 0xEA 1 byte long
	PC = PC + 1;
	FinishedExecutingCurrentInsctruction = true;
}
////////////////  END    ///////////////

/////////   ORA_INSTRUCTION
void ORA_IME() {
	//OPCOE 0x09 2BYTES LONG
	A = A | *GetPointerToDataInCPUMemoryUsing_IME_MODE();
	BaseSZCheck(2, A);
}
void ORA_ZABS() {
	//opcode 0x05 2bytes long
	A = A | *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	BaseSZCheck(2, A);
}
void ORA_ZINX() {
	//opcode 0x15 2bytes long
	A = A | *GetPointerToDataInCPUMemoryUsing_ZINX_MODE();
	BaseSZCheck(2, A);
}
void ORA_ABS() {
	//opcode 0D 3bytes long

	A = A | *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	BaseSZCheck(3, A);
}
void ORA_INX_X() {
	//opcode 1D 3bytes long
	A = A | *GetPointerToDataInCPUMemoryUsing_INX_X_MODE();
	BaseSZCheck(3, A);
}
void ORA_INX_Y() {
	//opcode 19 3bytes long
	A = A | *GetPointerToDataInCPUMemoryUsing_INX_Y_MODE();
	BaseSZCheck(3, A);
}
void ORA_PRII() {
	//opcode 01  2bytes long

	A = A | *GetPointerToDataInCPUMemoryUsing_PRII_MODE();
	BaseSZCheck(2, A);
}
void ORA_POII() {
	//opcode 11  2bytes long
	A = A | *GetPointerToDataInCPUMemoryUsing_POII_MODE();
	BaseSZCheck(2, A);
}
////////////////  END    ///////////////

/////////   LDY INSTRUCTIONS 
void LDY_IME() {
	//opcode 0xA0 2 bytes long
	Y = *GetPointerToDataInCPUMemoryUsing_IME_MODE();
	BaseSZCheck(2, Y);
}
void LDY_ZABS() {
	//opcode 0xA4 2 bytes long
	Y = *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	BaseSZCheck(2, Y);
}
void LDY_ZINX() {
	//opcode 0xB4 2 bytes long
	Y = *GetPointerToDataInCPUMemoryUsing_ZINX_MODE();
	BaseSZCheck(2, Y);
}
void LDY_ABS() {
	//opcode 0xAC 3 bytes long
	Y = *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	BaseSZCheck(3, Y);
}
void LDY_INX_X() {
	//opcode 0xBC 3 bytes long
	Y = *GetPointerToDataInCPUMemoryUsing_INX_X_MODE();
	BaseSZCheck(3, Y);
}
////////////////  END    /////////////// 

/////////   LDX INSTRUCTIONS 
void LDX_IME() {
	//opcode 0xA2 2 bytes long
	X = *GetPointerToDataInCPUMemoryUsing_IME_MODE();
	BaseSZCheck(2, X);
}
void LDX_ZABS() {
	//opcode 0xA6 2 bytes long
	X = *GetPointerToDataInCPUMemoryUsing_ZABS_MODE();
	BaseSZCheck(2, X);
}
void LDX_ZINY() {
	//opcode 0xB6 2 bytes long
	X = *GetPointerToDataInCPUMemoryUsing_ZINY_MODE();
	BaseSZCheck(2, X);
}
void LDX_ABS() {
	//opcode 0xAE 3 bytes long
	X = *GetPointerToDataInCPUMemoryUsing_ABS_MODE();
	BaseSZCheck(3, X);
}
void LDX_INX_Y() {
	//opcode 0xBE 3 bytes long
	X = *GetPointerToDataInCPUMemoryUsing_INX_Y_MODE();
	BaseSZCheck(3, X);
}
////////////////  END    /////////////// 