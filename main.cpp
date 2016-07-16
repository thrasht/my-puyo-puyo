#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <Tchar.h>
#include "Puyo.h"

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.
int HEIGHT = 80;
int WIDTH = 50;
int CONT_AVANZA = 250000;

using namespace std;

int main(int argc, char *argv[])
{
    Puyo p;
    int pos = 24;
    int avanza = 0;
    
    //cout << "Hola: " << p.Pos() << "\n";
    
     // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

    // Change the window title:
    SetConsoleTitle(TEXT("Win32 Console Control Demo"));

    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, 79, 49};

    // Change the console window size:
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    // Create a COORD to hold the buffer size:
    COORD bufferSize = {HEIGHT, WIDTH};

    // Change the internal buffer size:
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    // Set up the character buffer:
    CHAR_INFO consoleBuffer[HEIGHT * WIDTH];

    // Clear the CHAR_INFO buffer:
    for (int i=0; i < HEIGHT * WIDTH; ++i) {

        // Fill it with white-backgrounded spaces
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = 0;
    }

    // Set up the positions:
    COORD charBufSize = {HEIGHT,WIDTH};
    COORD characterPos = {0,0};
    SMALL_RECT writeArea = {0,0,79,49}; 

    // Copy to display:
    WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

    // How many events have happened?
    DWORD numEvents = 0;

    // How many events have we read from the console?
    DWORD numEventsRead = 0;

    // Boolean flag to state whether app is running or not.
    bool appIsRunning = true;
    // If we set appIsRunning to false, the program will end!
    while (appIsRunning) {
          
             
         
          
         
        // Find out how many console events have happened:
        GetNumberOfConsoleInputEvents(rHnd, &numEvents);
        
        // If it's not zero (something happened...)
        if (numEvents != 0) {

            // Create a buffer of that size to store the events
            INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

            // Read the console events into that buffer, and save how
            // many events have been read into numEventsRead.
            ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

            // Now, cycle through all the events that have happened:
            //for (DWORD i = 0; i < numEventsRead; ++i) {
                
                
                // Check the event type: was it a key?
                if (eventBuffer[0].EventType == KEY_EVENT) {

                    // Yes! Was the key code the escape key?
                    if (eventBuffer[0].Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE) {

                        // Yes, it was, so set the appIsRunning to false.
                        appIsRunning = false;

                    // Was if the 'c' key?
                    } else if (eventBuffer[0].Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {

                        // Yes, so clear the buffer to spaces:
                        consoleBuffer[p.Pos()].Char.AsciiChar = ' ';
                        consoleBuffer[p.Pos()].Attributes = 0;
                        p.Right();
                        consoleBuffer[p.Pos()].Char.AsciiChar = p.Figure();
                        consoleBuffer[p.Pos()].Attributes = BACKGROUND_GREEN;  
                        WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
                        } else if (eventBuffer[0].Event.KeyEvent.wVirtualKeyCode == VK_LEFT) {

                        // Yes, so clear the buffer to spaces:
                        consoleBuffer[p.Pos()].Char.AsciiChar = ' ';
                        consoleBuffer[p.Pos()].Attributes = 0;
                        p.Left();
                        consoleBuffer[p.Pos()].Char.AsciiChar = p.Figure();
                        consoleBuffer[p.Pos()].Attributes = BACKGROUND_GREEN;  
                        WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
                        } else if (eventBuffer[0].Event.KeyEvent.wVirtualKeyCode == VK_DOWN) {

                        // Yes, so clear the buffer to spaces:
                        consoleBuffer[p.Pos()].Char.AsciiChar = ' ';
                        consoleBuffer[p.Pos()].Attributes = 0;
                        p.Down();
                        consoleBuffer[p.Pos()].Char.AsciiChar = p.Figure();
                        consoleBuffer[p.Pos()].Attributes = BACKGROUND_GREEN;  
                        WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
                        }
                        
                                    
                }
                
            //}

            // Clean up our event buffer:
            delete[] eventBuffer;
        }
        
        avanza++;
        if(avanza == CONT_AVANZA)
        {
            p.Avanza();
            consoleBuffer[p.Pos() - HEIGHT].Char.AsciiChar = ' ';
            consoleBuffer[p.Pos() - HEIGHT].Attributes = 0; 
            consoleBuffer[p.Pos()].Char.AsciiChar = p.Figure();
            consoleBuffer[p.Pos()].Attributes = BACKGROUND_GREEN;  
            
            WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
        }
           
        if(avanza >= CONT_AVANZA)
           avanza = 0;
        
        
        
    }
    //system("pause");
    
    
    
    return EXIT_SUCCESS;
}
