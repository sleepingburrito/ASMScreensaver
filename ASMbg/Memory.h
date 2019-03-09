//
//Struct for ram layout and initialization
//
//

#pragma once
#include <stdlib.h>
#include <stdint.h>

//
//definitions
//
#define RAM_CAPACITY ( 0xFFFF + 1 ) //+1 to make sure you can use last address

//word size
#define BITS_PER_WORD 16
#define BITS_PER_HALF_WORD ( BITS_PER_WORD / 2 )
#define BITS_PER_QUARTER_WORD ( BITS_PER_HALF_WORD / 2 )

//predefined addresses
#define PROGRAM_COUNTER_ADDRESS 0x0010
#define FIRST_INSTRUCTION_ADDRESS 0x0012

#define STACK_STARTING_ADDRESS 0xFFFF //grows backwards
#define STACK_POINTER_ADDRESS 0x0011

#define MAX_ADDRESS_REGISTER 0x000F

//memory tools
uint16_t * InitializedMainMemory()
{
	return ( uint16_t * )calloc( RAM_CAPACITY, sizeof( uint16_t ) );
}

void SetInstruction( uint16_t * memory, uint16_t address, uint16_t instruction, uint16_t argument0, uint16_t argument1 )
{
	memory[address]  = instruction << BITS_PER_HALF_WORD;
	memory[address] |= ( argument0 << BITS_PER_QUARTER_WORD );
	memory[address] |= argument1;
}

uint16_t GetAddressValue( const uint16_t * memory, uint16_t Address )
{
	return memory[ Address ];
}

void SetAddressValue( uint16_t * memory, uint16_t Address, uint16_t value )
{
	memory[ Address ] = value;
}

void CopyAddressValue( uint16_t * memory, uint16_t Address0, uint16_t Address1 )
{
	SetAddressValue( memory, Address0, GetAddressValue( memory, Address0 ) );
}

uint16_t ProgramCounterValue( const uint16_t * memory )
{
	return GetAddressValue( memory, PROGRAM_COUNTER_ADDRESS );
}

void SetProgramCounter( uint16_t * memory, uint16_t address )
{
	SetAddressValue( memory, PROGRAM_COUNTER_ADDRESS,  address );
}

void IncrementProgramCounter( uint16_t * memory )
{
	SetProgramCounter( memory, ProgramCounterValue( memory ) + 1 );
}

uint16_t DereferenceProgramCounter( const uint16_t * memory )
{
	return GetAddressValue( memory, ProgramCounterValue( memory ) );
}

uint16_t DereferenceProgramCounterOffsetOneWord( const uint16_t * memory )
{
	return GetAddressValue( memory, ProgramCounterValue( memory ) + 1 );
}

uint16_t GetArgument0( const uint16_t * memory  )
{
	return ( (ProgramCounterValue( memory ) >> BITS_PER_QUARTER_WORD ) & MAX_ADDRESS_REGISTER );
}

uint16_t GetArgument1( const uint16_t * memory  )
{
	return ( ProgramCounterValue( memory ) & MAX_ADDRESS_REGISTER );
}

uint16_t DereferenceArgument0( const uint16_t * memory )
{
	return GetAddressValue( memory, GetArgument0( memory ) );
}

uint16_t DereferenceArgument1( const uint16_t * memory )
{
	return GetAddressValue( memory, GetArgument1( memory ) );
}

void PushStack( uint16_t * memory, uint16_t value )
{
	SetAddressValue( memory, GetAddressValue( memory, STACK_POINTER_ADDRESS ), value );
	SetAddressValue( memory, STACK_POINTER_ADDRESS, GetAddressValue( memory, STACK_POINTER_ADDRESS ) - 1 );
}

uint16_t PopStack( uint16_t * memory )
{
	return 
		SetAddressValue( memory, STACK_POINTER_ADDRESS, GetAddressValue( memory, STACK_POINTER_ADDRESS ) + 1 ),
		GetAddressValue( memory, GetAddressValue( memory, STACK_POINTER_ADDRESS ) - 1 );
}

uint16_t PeekStack( const uint16_t * memory )
{
	return GetAddressValue( memory, GetAddressValue( memory, STACK_POINTER_ADDRESS ) );
}