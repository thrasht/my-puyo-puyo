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
#include <ctime>


HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.

int HEIGHT = 15;
int WIDTH = 20;

int LEFT = -1;
int RIGHT = 1;
int UP = -HEIGHT;
int DOWN = HEIGHT;
int PDOWN = 2;
int TAM_ARR_VISITED = 100;
double TIME_TO_MOVE = 0.5;

using namespace std;

void movePuyo(INPUT_RECORD, CHAR_INFO *, Puyo *, bool *);
void avanzaPuyo(Puyo *, CHAR_INFO *, COORD, COORD, SMALL_RECT *);
int searchOrDestroyRensa(Puyo *, int, int, CHAR_INFO *, int *, bool);
bool searchAndDestroyRensaChain(Puyo *, int, int, CHAR_INFO *, int *);
void visitPosition(int, int *);
bool positionIsNotVisited(int, int *);
void initializingDirVisited(int *);
void fillSpaces(CHAR_INFO *);
void rotatePuyo(Puyo *, CHAR_INFO *);
void checkIfMove(Puyo *, CHAR_INFO *);

int _tmain(int argc, _TCHAR* argv[])
{
	//The count of the search of a rensa
	int countR = 0, countR2 = 0;
	
	std::clock_t start;
	double duration;

	int *dirVisited = (int*)malloc(sizeof(int) * TAM_ARR_VISITED);
    
     // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

    // Change the window title:
    SetConsoleTitle(TEXT("Puyo Puyo"));

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

    // How many events have happened
    DWORD numEvents = 0;

    // How many events have we read from the console
    DWORD numEventsRead = 0;

    // Boolean flag to state whether app is running or not.
    bool appIsRunning = true;
    // If we set appIsRunning to false, the program will end!
	
	//Initializing the clock
	start = std::clock();	
	
	//Initializing the array of two Puyos
	Puyo *pp;
	pp = (Puyo *)malloc(sizeof(Puyo) * 2);

	//This is where all the game begins
    while (appIsRunning) 
	{
		//Creating 2 new random Puyos
		pp = new Puyo();
		pp[1] = *(new Puyo(LEFT));

		//This while remains until the two puyos are down
		while ((pp->Check() == true || (pp + 1)->Check() == true) && appIsRunning)
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
					// Check the event type if it was a key and if it was pressing down
					if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown)
					{
						//You need to check if the puyos can still moving
						if(pp->Check() == true && (pp + 1)->Check() == true)
						{
							//If they can still moving, you need to check if the Puyo that is
							//rotate is in DOWN position because, in that case, you need to first
							//move the down puyo and then the up Puyo, in order to not delete de 
							//down Puyo. You need to check the same when the Puyo that is rotate
							//is on the left and you go to the left or if it is right and you go to
							//the right
							if((pp + 1)->Partner() == DOWN 
								|| ((pp + 1)->Partner() == LEFT && eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_LEFT)
								|| ((pp + 1)->Partner() == RIGHT && eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RIGHT))
							{
								movePuyo(eventBuffer[i], consoleBuffer, pp + 1, &appIsRunning);
								movePuyo(eventBuffer[i], consoleBuffer, pp, &appIsRunning);
							}
							else
							{
								movePuyo(eventBuffer[i], consoleBuffer, pp, &appIsRunning);
								movePuyo(eventBuffer[i], consoleBuffer, pp + 1, &appIsRunning);
							}
						}
						//Then, you can write the new positions of the Puyos
						WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
					}
				}
				// Clean up our event buffer:
				delete[] eventBuffer;
			}

			//Obtains the time that is passed
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//If has been passed more than TIME_TO_MOVE seconds, move the puyos one step forward
			if(duration > TIME_TO_MOVE)
			{
				//If they can still moving, you need to check if the Puyo that is
				//rotate is in DOWN position because, in that case, you need to first
				//move the down puyo and then the up Puyo, in order to not delete de 
				//down Puyo.
				if((pp + 1)->Partner() == DOWN)
				{
					avanzaPuyo(pp + 1, consoleBuffer, charBufSize, characterPos, &writeArea);
					avanzaPuyo(pp, consoleBuffer, charBufSize, characterPos, &writeArea);
				}
				else
				{
					avanzaPuyo(pp, consoleBuffer, charBufSize, characterPos, &writeArea);
					avanzaPuyo(pp + 1, consoleBuffer, charBufSize, characterPos, &writeArea);
				}
				//Reset the clock
				start = std::clock();
			}
		}
		//Initializing the dirVisited Array to -1 to add the directions visited in
		//the next recursive function to search the Rensa. You call the function with
		//the last parameter in false because you need to do a search first
		initializingDirVisited(dirVisited);
		countR = searchOrDestroyRensa(pp, pp->Pos(), 0, consoleBuffer, dirVisited, false);

		//Doing the same fot the second puyo
		initializingDirVisited(dirVisited);
		countR2 = searchOrDestroyRensa(pp + 1, pp[1].Pos(), 0, consoleBuffer, dirVisited, false);

		//If the function returns 4 or more, then there is a Rensa so you need to destroy it
		if(countR >= 4 || countR2 >= 4)
		{
			if(countR >=4)
			{
				//Reset the dirVisited Array and call the searchOrDestroyRensa function
				//with the last parameter in true
				initializingDirVisited(dirVisited);
				searchOrDestroyRensa(pp, pp->Pos(), 0, consoleBuffer, dirVisited, true);
			}
			
			if(countR2 >=4)
			{
				//Doing the same for the second puyo
				initializingDirVisited(dirVisited);
				searchOrDestroyRensa(pp + 1, pp[1].Pos(), 0, consoleBuffer, dirVisited, true);
			}			

			do{
				//Sleep the program 500 ms to see the changes
				Sleep(500);
				WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
				Sleep(500);
				//Fill the empity spaces
				fillSpaces(consoleBuffer);
				WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
				Sleep(500);
				//this do-while continues until there aren`t chains left
			}while(searchAndDestroyRensaChain(pp, pp->Pos(), 0, consoleBuffer, dirVisited));

		}
        
	}

	return 0;
}

//This function do the movements of the puyo
//Left, Right, Down and Rotate
void movePuyo(INPUT_RECORD eventBuffer, CHAR_INFO *consoleBuffer, Puyo *p, bool *appIsRunning)
{

	switch (eventBuffer.Event.KeyEvent.wVirtualKeyCode) 
	{
		case VK_ESCAPE:

			// Yes, it was, so set the appIsRunning to false.
			*appIsRunning = false;
			break;

		case 0x41: //Pressed the 'A' Key. Rotate the puyo
			
			rotatePuyo(p, consoleBuffer);
			
			break;

		case VK_RIGHT:
			if((p->Pos() + 1) % HEIGHT != 0 && !(consoleBuffer[p->Pos() + 1].Char.AsciiChar != ' '))
			{
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
			checkIfMove(p, consoleBuffer);

			if(p->Check() == true)
			{
				consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
				consoleBuffer[p->Pos()].Attributes = 0;
				p->Down();
				consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
				consoleBuffer[p->Pos()].Attributes = p->Color();  
			}
			break;                       
	} 

	checkIfMove(p, consoleBuffer);

}

//This Function changes the position of the puyo that is rotating
void rotatePuyo(Puyo *p, CHAR_INFO *consoleBuffer)
{

	switch(p->Partner())
			{
				case -1:
					consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
					consoleBuffer[p->Pos()].Attributes = 0;
					p->EditPos(p->Pos() + RIGHT + UP);
					p->EditPartner(UP);
					consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
					consoleBuffer[p->Pos()].Attributes = p->Color(); 
					break;

				case -15:
					consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
					consoleBuffer[p->Pos()].Attributes = 0;
					p->EditPos(p->Pos() + RIGHT + DOWN);
					p->EditPartner(RIGHT);
					consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
					consoleBuffer[p->Pos()].Attributes = p->Color(); 
					break;

				case 1:
					consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
					consoleBuffer[p->Pos()].Attributes = 0;
					p->EditPos(p->Pos() + LEFT + DOWN);
					p->EditPartner(DOWN);
					(p - 1)->EditPartner(PDOWN);
					consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
					consoleBuffer[p->Pos()].Attributes = p->Color(); 
					break;

				case 15:
					consoleBuffer[p->Pos()].Char.AsciiChar = ' ';
					consoleBuffer[p->Pos()].Attributes = 0;
					p->EditPos(p->Pos() + LEFT + UP);
					p->EditPartner(LEFT);
					(p - 1)->EditPartner(0);
					consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
					consoleBuffer[p->Pos()].Attributes = p->Color(); 
					break;

			}

}

//Checking if the Puyo can move
void checkIfMove(Puyo *p, CHAR_INFO *consoleBuffer)
{
	if(p->Pos() + HEIGHT > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT].Char.AsciiChar != ' ' )
		if(p->Partner() == UP)
		{
			if(p->Pos() + HEIGHT * 2 > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT * 2].Char.AsciiChar != ' ' )
				p->ChangeState(false);
		}
		else if(p->Partner() == PDOWN)
		{
			if(p->Pos() + HEIGHT * 2 > HEIGHT * WIDTH || consoleBuffer[p->Pos() + HEIGHT * 2].Char.AsciiChar != ' ' )
				p->ChangeState(false);
		}
		else
			p->ChangeState(false);

}

//This function moves the puyo step forward
void avanzaPuyo(Puyo *p, CHAR_INFO *consoleBuffer, COORD charBufSize, COORD characterPos, SMALL_RECT *writeArea)
{
	checkIfMove(p, consoleBuffer);

	if(p->Check() == true)
	{
		p->Avanza();
		consoleBuffer[p->Pos() - HEIGHT].Char.AsciiChar = ' ';
		consoleBuffer[p->Pos() - HEIGHT].Attributes = 0; 
		consoleBuffer[p->Pos()].Char.AsciiChar = p->Figure();
		consoleBuffer[p->Pos()].Attributes = p->Color();  
        
		WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, writeArea);
		checkIfMove(p, consoleBuffer);
	}

}

//To search the linked figures in order to destroy, i made a recursive algorithm
//The dirVisited array is to not repeat any puyo visited before
//The recursive function accumulates each time it founds the same figure and if the result
//is 4 or more, the rensa exists.
//the destroy parameter indicates if you want to make a search or delete a rensa.
int searchOrDestroyRensa(Puyo *p, int pos, int origin, CHAR_INFO *charBuffer, int *dirVisited, bool destroy)
{
	int res = 0;

	visitPosition(pos, dirVisited);

	if(charBuffer[pos].Char.AsciiChar == p->Figure())
	{
		res = 1;
		if(destroy == true)
		{
			charBuffer[pos].Char.AsciiChar = ' ';
			charBuffer[pos].Attributes = 0;
		}
	}

	if(res == 1)
	{
		//Search to the Right side but check before if it was not visited
		if(positionIsNotVisited(pos + RIGHT, dirVisited))
			res += searchOrDestroyRensa(p, pos + RIGHT, LEFT, charBuffer, dirVisited, destroy);

		//Search to the Left side but check before if it was not visited
		if(positionIsNotVisited(pos + LEFT, dirVisited))
			res += searchOrDestroyRensa(p, pos + LEFT, RIGHT, charBuffer, dirVisited, destroy);
		
		//Search to the Upper side but check before if it was not visited
		if(positionIsNotVisited(pos + UP, dirVisited))
			res += searchOrDestroyRensa(p, pos + UP, DOWN, charBuffer, dirVisited, destroy);

		//Search to the Down side but check before if it was not visited 
		if(positionIsNotVisited(pos + DOWN, dirVisited))
			res += searchOrDestroyRensa(p, pos + DOWN, UP, charBuffer, dirVisited, destroy);

	}

	return res;

}

//Reseting the directions Array
void initializingDirVisited(int *visitedPositions)
{
	int i = 0;

	for(i = 0; i < TAM_ARR_VISITED; i++)
		visitedPositions[i] = -1;

}

//Here is where you add the directions visited
void visitPosition(int pos, int *visitedPositions)
{
	int i = 0;

	for(i = 0; i < TAM_ARR_VISITED; i++)
	{
		if(visitedPositions[i] == -1)
		{
			visitedPositions[i] = pos;
			break;
		}
	}
}

//Returns true if the position is not yet visited so you can advance
bool positionIsNotVisited(int pos, int *visitedPositions)
{
	int i = 0;
	bool notVisited = true;

	for(i = 0; i < TAM_ARR_VISITED && visitedPositions[i] != -1; i++)
	{
		if(visitedPositions[i] == pos)
		{
			notVisited = false;
			break;
		}
	}

	return notVisited;
}

//Fill the empity spaces after a Rensa was destroyed
void fillSpaces(CHAR_INFO *consoleBuffer)
{
	int i, j, k;

	for( i = WIDTH * (HEIGHT - 1); i < WIDTH * HEIGHT; i++)
	{
		for( j = i; j > 0; j = j - HEIGHT)
		{
			if(consoleBuffer[j].Char.AsciiChar == ' ')
			{
				for( k = j - HEIGHT; k > 0; k = k - HEIGHT)
				{
					if(consoleBuffer[k].Char.AsciiChar != ' ')
					{
						consoleBuffer[j].Char.AsciiChar = consoleBuffer[k].Char.AsciiChar;
						consoleBuffer[j].Attributes = consoleBuffer[k].Attributes;

						consoleBuffer[k].Char.AsciiChar = ' ';
						consoleBuffer[k].Attributes = 0;

						break;
					}
				}
			}
		}
	}
}

//Runs over all the console searching for new Rensas, if you find some, destroy them
bool searchAndDestroyRensaChain(Puyo *p, int pos, int origin, CHAR_INFO *charBuffer, int *dirVisited)
{
	int i, j, cont = 0;
	bool res = false;

	for( i = WIDTH * (HEIGHT - 1); i < WIDTH * HEIGHT; i++)
	{
		for( j = i; j > 0; j = j - HEIGHT)
		{
			if(charBuffer[j].Char.AsciiChar != ' ')
			{
				p->EditFigure(charBuffer[j].Char.AsciiChar);
				p->EditColor(charBuffer[j].Attributes);
				p->EditPos(j);

				initializingDirVisited(dirVisited);
				cont = searchOrDestroyRensa(p, p->Pos(), 0, charBuffer, dirVisited, false);
				if(cont >= 4)
				{
					initializingDirVisited(dirVisited);
					searchOrDestroyRensa(p, p->Pos(), 0, charBuffer, dirVisited, true);
					Sleep(500);
					res = true;
				}

			}
		}
	}

	return res;
}
