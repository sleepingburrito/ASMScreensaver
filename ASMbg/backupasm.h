#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "SDLStartWindow.h"
#include "Setting.h"

#define BITS_PER_WORD 16
#define BITS_PER_HALF_WORD ( BITS_PER_WORD / 2 )
#define BITS_PER_QUARTER_WORD ( BITS_PER_HALF_WORD / 2 )
#define MAX_WORD_COUNT 0xFFFF
uint16_t * mainMemory = NULL;
uint16_t * deltaDrawMemory = NULL; //used to compare to current ram to only draw the diffrances

//note: registers are address 0x0000 to 0x000F
#define PROGRAME_COUNTER_ADDRESS 0x0010
#define FIRST_INSTRUCTION_ADRESS 0x0012

#define STACK_STARTING_ADDRESS 0xFFFF //grows backwards
#define STACK_POINTER_ADDRESS 0x0011


//0 - F is the regs
enum InstructionIndex
{
	HALT, //stops excution forever

	//outside//
	COPY_FROM_AD, //arg0 is reg to save, next word is where to read from, pc is moved by 2
	COPY_TO_AD, //arg0 is reg to copy from, next word is where to save to, pc is moved by 2
	COPY_TO_AD_REG, //arg0 (reg) is the reg to copy from, arg1(reg) holds the adress in main memery to copy to, pc moved by 1
	COPY_FROM_AD_REG, //arg0 (reg) is where it will be saved, arg1(reg) holds the addres to copy from, pc moved by 1
	SET_REG_VALUE_FULL, //arg0 is the reg to set, next word in the value to set it to, pc is moved by 2
	
	//inside//
	SET_REG, //arg0 is reg to set, arg1 is data, pc moved by 1
	COPY_REG, //arg0 is the reg to get set, arg1 is where the data comes from (4bit value), pc moved by 1

	//math
	ADD_REG, //arg0 is reg to save and first number, arg1 is reg to apply to arg 0, pc moved by 1
	SUB_REG,
	MULT_REG,
	DIV_REG,
	MOD_REG,
	AND_REG,
	OR_REG,
	XOR_REG,

	//other tools
	INC_REG, //arg0 is reg to effect one to
	DINC_REG,

	LS_REG, //left Shift: arg0 is reg to shift arg1 is how much by
	RS_REG, //right shift

	//testing
	JMP_REG, //pc will be set to value of arg0 
	JMP, //pc will be set to the value of the next word
	JMP_IF_GT, //greater than: arg0 is left side reg, arg1 is right side arg, next word is what pc is set to if true, pc is moved by at least 2
	JMP_IF_LT, //less than
	JMP_IF_EQ, //equal too
	JMP_NOT_EQ, //not equal
	JMP_EQ_VAL, //arg0 is reg to compare, arg1 is vale what to compare it too(4bit value), next word is where to jump if true, pc is moved by at least 2pc is moved by at least 2

	//stack
	PUSH_STACK, //value of arg0(reg adress) is pushed onto stack
	POP_STACK, //value is poped off stack and saved into reg (arg0)
	PEEK_STACK, //value is copied (not poped) off stack into reg (arg0)

	//subrutines
	GOSUB, //next word is the address to jump too. Current pc location and all regasters are pushed onto the stack
	RETURN, //jump back to last value put on the stack and restore all regasters

	RAND //arg0 is regaster to save, fills with psedo bits
};


//memory management
//programing tools
void SetInst( uint16_t addr, uint16_t inst, uint16_t arg0, uint16_t arg1 )
{
	mainMemory[addr] = inst << BITS_PER_HALF_WORD;
	mainMemory[addr] |= ( arg0 << BITS_PER_QUARTER_WORD );
	mainMemory[addr] |= arg1;
}

void SetAddrs( uint16_t addr, uint16_t value )
{
	mainMemory[addr] = value;
}

void CopyAddrVal( uint16_t addr1, uint16_t addr2 )
{
	mainMemory[ addr1 ] = mainMemory[ addr2 ];
}

uint16_t GetAddrValue( uint16_t addr )
{
	return mainMemory[addr];
}

//decoding tools
void IncPc()
{
	mainMemory[PROGRAME_COUNTER_ADDRESS]++;
}

uint16_t GetPcVal()
{
	return mainMemory[PROGRAME_COUNTER_ADDRESS];
}

uint16_t GetValAtPc()
{
	return mainMemory[GetPcVal()];
}

uint16_t DerefPointerValueAtPc()
{
	return GetAddrValue( mainMemory[GetPcVal()] );
}

void SetPc( uint16_t addr )
{
	mainMemory[PROGRAME_COUNTER_ADDRESS] = addr;
}

uint16_t GetInst()
{
	return mainMemory[GetPcVal()] >> BITS_PER_HALF_WORD;
}

uint16_t GetArg0()
{
	return ( mainMemory[GetPcVal()] >> BITS_PER_QUARTER_WORD ) & 0x000F;
}

uint16_t GetArg1()
{
	return ( mainMemory[GetPcVal()] ) & 0x000F;
}

void PushStack( uint16_t value )
{
	mainMemory[	--mainMemory[STACK_POINTER_ADDRESS] ] = value;
}

uint16_t PopStack()
{
	return mainMemory[ mainMemory[STACK_POINTER_ADDRESS]++ ];
}

uint16_t PeekStack()
{
	return mainMemory[ mainMemory[STACK_POINTER_ADDRESS] ];
}



//initialization
void Start()
{
	mainMemory = (uint16_t *)calloc( MAX_WORD_COUNT + 1, sizeof( uint16_t ) );
	if ( mainMemory == NULL )
		printf( "\nerror: start calloc failed\n" ), exit( 1 );

	deltaDrawMemory = (uint16_t *)calloc( MAX_WORD_COUNT + 1, sizeof( uint16_t ) );
	if ( deltaDrawMemory == NULL )
		printf( "\nerror: start calloc failed\n" ), exit( 1 );

	SetPc( FIRST_INSTRUCTION_ADRESS );



	//init values
	SetAddrs( 0x0, 1 );
	SetAddrs( 0x1, 2 );
	SetAddrs( 0x2, 3 );
	SetAddrs( 0x3, 4 );
	SetAddrs( 0x4, 5 );
	SetAddrs( 0x5, 6 );
	SetAddrs( 0x6, 7 );
	SetAddrs( 0x7, 8 );
	SetAddrs( 0x8, 9 );
	SetAddrs( 0x9, 10 );
	SetAddrs( 0xA, 11 );
	SetAddrs( 0xB, 12 );
	SetAddrs( 0xC, 13 );
	SetAddrs( 0xD, 14 );
	SetAddrs( 0xE, 15 );
	SetAddrs( 0xF, 16 );

	//testing program
	uint16_t startaddr = FIRST_INSTRUCTION_ADRESS;

	//go to subruteen
	SetInst( startaddr++, GOSUB, 0, 0 ); 
	SetAddrs( startaddr++, 0xF000 ); //set subruteen adress

	//loop back to start
	SetInst( startaddr++, JMP, 0, 0 ); 
	SetAddrs( startaddr++, FIRST_INSTRUCTION_ADRESS );

	//do random stuff
	startaddr = 0xF000; //jump to subsuteen start
	SetInst( startaddr++, RAND, 0x0, 0 );
	SetInst( startaddr++, RAND, 0x1, 0 );
	SetInst( startaddr++, RAND, 0x2, 0 );
	SetInst( startaddr++, RAND, 0x3, 0 );
	SetInst( startaddr++, RAND, 0x4, 0 );
	SetInst( startaddr++, RAND, 0x5, 0 );
	SetInst( startaddr++, RAND, 0x6, 0 );
	SetInst( startaddr++, RAND, 0x7, 0 );
	SetInst( startaddr++, RAND, 0x8, 0 );
	SetInst( startaddr++, RAND, 0x9, 0 );
	SetInst( startaddr++, RAND, 0xA, 0 );
	SetInst( startaddr++, RAND, 0xB, 0 );
	SetInst( startaddr++, RAND, 0xC, 0 );
	SetInst( startaddr++, RAND, 0xD, 0 );
	SetInst( startaddr++, RAND, 0xE, 0 );
	SetInst( startaddr++, RAND, 0xF, 0 );

	//go to subruteen
	SetInst( startaddr++, GOSUB, 0, 0 ); 
	SetAddrs( startaddr++, 0x0F00 ); //set subruteen adress

	//go back
	SetInst( startaddr++, RETURN, 0, 0 ); 

	//new subruteen
	startaddr = 0x0F00; //jump to subsuteen start
	SetInst( startaddr++, XOR_REG, 0x0, 0x0 ); 
	SetInst( startaddr++, XOR_REG, 0x1, 0x1 ); 
	SetInst( startaddr++, XOR_REG, 0x2, 0x2 ); 
	SetInst( startaddr++, XOR_REG, 0x3, 0x3 ); 
	SetInst( startaddr++, XOR_REG, 0x4, 0x4 ); 
	SetInst( startaddr++, XOR_REG, 0x5, 0x5 ); 
	SetInst( startaddr++, XOR_REG, 0x6, 0x6 ); 
	SetInst( startaddr++, XOR_REG, 0x7, 0x7 ); 
	SetInst( startaddr++, XOR_REG, 0x8, 0x8 ); 
	SetInst( startaddr++, XOR_REG, 0x9, 0x9 ); 
	SetInst( startaddr++, XOR_REG, 0xA, 0xA ); 
	SetInst( startaddr++, XOR_REG, 0xB, 0xB ); 
	SetInst( startaddr++, XOR_REG, 0xC, 0xC ); 
	SetInst( startaddr++, XOR_REG, 0xD, 0xD ); 
	SetInst( startaddr++, XOR_REG, 0xE, 0xE ); 
	SetInst( startaddr++, XOR_REG, 0xF, 0xF ); 
	SetInst( startaddr++, RETURN, 0, 0 );

}



//main logic
void TickComputer()
{
	uint16_t tmpWord0 = 0;
	uint16_t tmpAddr0 = 0;
	uint16_t tmpArg0 = 0;

	uint16_t tmpWord1 = 0;
	uint16_t tmpAddr1 = 0;
	uint16_t tmpArg1 = 0;

	//decode address
	switch( GetInst() )
	{
		case HALT:
		return;

		case COPY_FROM_AD:
			tmpAddr0 = GetArg0(); //get what reg to save to
			IncPc(); //move to next word thats the location to load from
			CopyAddrVal( tmpAddr0, GetValAtPc() ); //copy to reg
		break;

		case COPY_TO_AD:
			IncPc(); //move to next word which is the pointer where to save
			CopyAddrVal( GetValAtPc(), GetArg0() ); //copy value to mem
		break;

		case COPY_TO_AD_REG:
			CopyAddrVal( GetAddrValue( GetArg1() ), GetArg0() );
		break;

		case COPY_FROM_AD_REG: //needs testing
			CopyAddrVal( GetArg0(), GetAddrValue( GetArg1() ) );
		break;

		case SET_REG_VALUE_FULL:
			tmpAddr0 = GetArg0(); //get reg address to save to
			IncPc();
			CopyAddrVal( tmpAddr0, GetPcVal() );
		break;

		case SET_REG:
			SetAddrs
			( 
				GetArg0() //get regaster to set
			   ,GetArg1() //get what value to set to it
			);
		break;

		case COPY_REG:
			CopyAddrVal
			( 
				GetArg0() //get what reg to save to
			   ,GetArg1() //put value from this reg into first arg
			);
		break;

		case ADD_REG:
			SetAddrs
			(
				 GetArg0() //save resaults to this regaster
				,GetAddrValue( GetArg0() ) + GetAddrValue( GetArg1() ) //add the values at these regasters
			);
		break;

		case SUB_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) - GetAddrValue( GetArg1() )
			);
		break;

		case MULT_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) * GetAddrValue( GetArg1() )
			);
		break;

		case DIV_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) / GetAddrValue( GetArg1() )
			);
		break;

		case MOD_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) % GetAddrValue( GetArg1() )
			);
		break;

		case AND_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) & GetAddrValue( GetArg1() )
			);
		break;

		case OR_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) | GetAddrValue( GetArg1() )
			);
		break;

		case XOR_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) ^ GetAddrValue( GetArg1() )
			);
		break;

		case INC_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) + 1
			);
		break;

		case DINC_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) - 1
			);
		break;

		case LS_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) << GetArg1()
			);
		break;

		case RS_REG:
			SetAddrs
			(
				 GetArg0()
				,GetAddrValue( GetArg0() ) >> GetArg1()
			);
		break;

		case JMP_REG:
			SetPc( GetAddrValue( GetArg0() ) ); //jump to the address stored in this reg
		return; //dont allow pc to incroment

		case JMP:
			IncPc();
			SetPc( GetValAtPc() );
		return;

		case JMP_IF_GT:
			if ( GetAddrValue( GetArg0() ) > GetAddrValue( GetArg1() ) )
			{
				IncPc();
				SetPc( GetValAtPc() );
				return;
			}
			else
				IncPc();
		break;

		case JMP_IF_LT:
			if ( GetAddrValue( GetArg0() ) < GetAddrValue( GetArg1() ) )
			{
				IncPc();
				SetPc( GetValAtPc() );
				return;
			}
			else
				IncPc();
		break;

		case JMP_IF_EQ:
			if ( GetAddrValue( GetArg0() ) == GetAddrValue( GetArg1() ) )
			{
				IncPc();
				SetPc( GetValAtPc() );
				return;
			}
			else
				IncPc();
		break;

		case JMP_NOT_EQ:
			if ( GetAddrValue( GetArg0() ) != GetAddrValue( GetArg1() ) )
			{
				IncPc();
				SetPc( GetValAtPc() );
				return;
			}
			else
				IncPc();
		break;

		case JMP_EQ_VAL:
			if ( GetAddrValue( GetArg0() ) ==  GetArg1() )
			{
				IncPc();
				SetPc( GetValAtPc() );
				return;
			}
			else
				IncPc();
		break;

		case PUSH_STACK:
			PushStack( GetAddrValue(  GetArg0() ) );
		break;

		case POP_STACK:
			SetAddrs( GetArg0(), PopStack() );
		break;

		case PEEK_STACK:
			SetAddrs( GetArg0(), PeekStack() );
		break;

		case GOSUB:
			PushStack( GetPcVal() );
			for( uint16_t i = 0; i <= 0x000F; i++ )
				PushStack( GetAddrValue(  i ) );
			
			//jump to next word
			IncPc();
			SetPc( GetValAtPc() );
		return;

		case RETURN:
			for( uint16_t i = 0; i <= 0x000F; i++ )
				SetAddrs( 0x000F - i, PopStack() );

			SetPc( PopStack() + 1 ); //pluse one to skip over word that contines tha adress to go to
		break;

		case RAND:
			SetAddrs( GetArg0(), (uint16_t)rand() );
		break;

		default:
			printf( "\nInvalid instruction at: %u\n", GetPcVal() );
	}

	IncPc();
}



//drawing
void DrawMemory()
{
	//starting pixel of eatch word
	size_t x = 0;
	size_t y = 0;

	for( size_t wordIndex = 0; wordIndex <= MAX_WORD_COUNT; wordIndex++ )
	{
		uint16_t tmpWord = mainMemory[wordIndex];
		if ( tmpWord != deltaDrawMemory[wordIndex] || wordIndex == GetPcVal() )
		{
			for( size_t bitIndex = 0; bitIndex < BITS_PER_WORD; bitIndex++, tmpWord <<= 1 )
				if ( ( tmpWord & 0x8000 ) != 0 ) //if pixel is on draw it white
				{
					SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
					SDL_RenderDrawPoint( renderer, (int)( x + bitIndex ), (int)y );
				}
				else if ( wordIndex == GetPcVal() ) //if pixel is background and is the value of pc then draw red background
				{
					SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
					SDL_RenderDrawPoint( renderer, (int)( x + bitIndex ), (int)y );
				}
				else //normal black background if nothing specal
				{
					SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
					SDL_RenderDrawPoint( renderer, (int)( x + bitIndex ), (int)y );
				}

			deltaDrawMemory[wordIndex] = mainMemory[wordIndex]; //reset delat
		}

		//move drawing to next free space
		if ( ++y >= WINDOW_HIGHT )
			y = 0, x += BITS_PER_WORD;
	}

	//reset this value to get redrawn next time to remove red background
	deltaDrawMemory[GetPcVal()] = deltaDrawMemory[GetPcVal()] + 1;
}