// my_puyo_puyo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <Tchar.h>
#include "Puyo.h"

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.
int HEIGHT = 15;
int WIDTH = 20;
int CONT_AVANZA = 250000;

using namespace std;

void muevePuyo(INPUT_RECORD, CHAR_INFO *, Puyo *, bool *);
void avanzaPuyo(Puyo *, int *, CHAR_INFO *, COORD, COORD, SMALL_RECT *);

int _tmain(int argc, _TCHAR* argv[])
{
    int avanza = 0;
	int cont = 0;
    
    
     // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

    // Change the window title:
    SetConsoleTitle(TEXT("Win32 Console Control Demo"));

    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, HEIGHT - 1, WIDTH - 1};

    // Change the console window size:
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    // Create a COORD to hold the buffer size:
    COORD bufferSize = {HEIGHT, WIDTH};

    // Change the internal buffer size:
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    // Set up the character buffer:
    CHAR_INFO *consoleBuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * HEIGHT * WIDTH);

    // Clear the CHAR_INFO buffer:
    for (int i=0; i < HEIGHT * WIDTH; ++i) {

        // Fill it with white-backgrounded spaces
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = 0;
    }

    // Set up the positions:
    COORD charBufSize = {HEIGHT,WIDTH};
    COORD characterPos = {0,0};
    SMALL_RECT writeArea = {0,0,HEIGHT - 1, WIDTH - 1}; 

    // Copy to display:
    WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

    // How many events have happened?
    DWORD numEvents = 0;

    // How many events have we read from the console?
    DWORD numEventsRead = 0;

    // Boolean flag to state whether app is running or not.
    bool appIsRunning = true;
    // If we set appIsRunning to false, the program will end!
    while (appIsRunning) 
	{
        Puyo* p = new Puyo();
		
		while (p->Check() == true)
		{
			// Find out how many console events have happened:
			GetNumberOfConsoleInputEvents(rHnd, &numEvents);
	        
			// If it's not zero (something happened...)
			if (numEvents != 0) 
			{
				// Create a buffer of that size to store the events
				INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

				// Read the console events into that buffer, and save how
				// many events have been read into numEventsRead.
				ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

				// Now, cycle through all the events that have happened:
				for (DWORD i = 0; i < numEventsRead; ++i) 
				{
					// Check the event type: was it a key?
					if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown)
					{
						muevePuyo(eventBuffer[i], consoleBuffer, p, &appIsRunning);
						WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
					}
				}
				// Clean up our event buffer:
				delete[] eventBuffer;
			}
			
	        avanzaPuyo(p, &avanza, consoleBuffer, charBufSize, characterPos, &writeArea);
		}
        
	}

	return 0;
}


void muevePuyo(INPUT_RECORD eventBuffer, CHAR_INFO *consoleBuffer, Puyo *p, bool *appIsRunning)
{

	switch (eventBuffer.Event.KeyEvent.wVirtualKeyCode) 
	{
		case VK_ESCAPE:

		// Yes, it was, so set the appIsRunning to false.
		appIsRunning = false;
		break;

		case VK_RIGHT:
			if((p->Pos() + 1) % HEIGHT != 0 && !(consoleBuffer[p->Pos() + 1].Char.AsciiChar != ' '))
			{
				// Yes, so clear the buffer to spaces:
				consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
				consoleBuffer[p->Pos()].Attributes = 0;
				p->Right();
				consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
				consoleBuffer[p->Pos()].Attributes = p->Color();  
			}
		break;

		case VK_LEFT:
			
			if(p->Pos() % HEIGHT != 0 && !(consoleBuffer[p->Pos() - 1].Char.AsciiChar != ' '))
			{
				// Yes, so clear the buffer to spaces:
				consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
				consoleBuffer[p->Pos()].Attributes = 0;
				p->Left();
				consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
				consoleBuffer[p->Pos()].Attributes = p->Color();  
			}
			break;
		
		case VK_DOWN:
			// Yes, so clear the buffer to spaces:
			if(p->Pos() + HEIGHT > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT].Char.AsciiChar != ' ' )
			{
				p->ChangeState(false);
			}
			else
			{
				consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
				consoleBuffer[p->Pos()].Attributes = 0;
				p->Down();
				consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
				consoleBuffer[p->Pos()].Attributes = p->Color();  
			}
			break;                       
	} 

	if(p->Pos() + HEIGHT > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT].Char.AsciiChar != ' ' )
	{
		p->ChangeState(false);
	}


}

void avanzaPuyo(Puyo *p, int *avanza, CHAR_INFO *consoleBuffer, COORD charBufSize, COORD characterPos, SMALL_RECT *writeArea)
{
	if(p->Pos() + HEIGHT > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT].Char.AsciiChar != ' ' )
		p->ChangeState(false);

	*avanza = *avanza + 1;
	if(*avanza == CONT_AVANZA && p->Check() == true)
	{
		p->Avanza();
		consoleBuffer[p->Pos() - HEIGHT].Char.AsciiChar = ' ';
		consoleBuffer[p->Pos() - HEIGHT].Attributes = 0; 
		consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
		consoleBuffer[p->Pos()].Attributes = p->Color();  
        
		WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, writeArea);
		if(p->Pos() + HEIGHT > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT].Char.AsciiChar != ' ' )
			p->ChangeState(false);
	}
   
	if(*avanza >= CONT_AVANZA)
	   *avanza = 0;

}
