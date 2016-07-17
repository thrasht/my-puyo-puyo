#include "StdAfx.h"
#include "Puyo.h"
#include <Windows.h>
#include <stdlib.h>
#include <iomanip>

Puyo::Puyo(void)
{
	pos = 7;
	active = true;
	figure = ('0' - 46) + (rand() % 6);

	switch (figure)
	{
		case 0x02:
			color = BACKGROUND_BLUE;
			break;

		case 0x03:
			color = BACKGROUND_RED;
			break;

		case 0x04:
			color = BACKGROUND_GREEN;
			break;

		case 0x05:
			color = BACKGROUND_BLUE | BACKGROUND_RED;
			break;

		case 0x06:
			color = BACKGROUND_RED | BACKGROUND_GREEN;
			break;

		case 0x07:
			color = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
			break;
	}

}

Puyo::Puyo(bool act)
{
	pos = 19;
	active = act;
	figure = ('0' - 46) + (rand() % 6);

	switch (figure)
	{
		case 0x02:
			color = BACKGROUND_BLUE;
			break;

		case 0x03:
			color = BACKGROUND_RED;
			break;

		case 0x04:
			color = BACKGROUND_GREEN;
			break;

		case 0x05:
			color = BACKGROUND_BLUE | BACKGROUND_RED;
			break;

		case 0x06:
			color = BACKGROUND_RED | BACKGROUND_GREEN;
			break;

		case 0x07:
			color = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
			break;
	}

}

Puyo::~Puyo(void)
{
}

int Puyo::Pos()
{
 
       return pos;      
}


int Puyo::Avanza()
{
    pos+=15;
    
    return pos;    
     
}

char Puyo::Figure()
{
    
    return figure;   
}

void Puyo::Right()
{
     pos++;
     
}

void Puyo::Left()
{
     pos--;
}

void Puyo::Down()
{
     pos+=15;
}

bool Puyo::Check()
{
	return active;
}

bool Puyo::ChangeState(bool state)
{
	active = state;

	return active;
}

int Puyo::Color()
{
	return color;
}


