using System;

public class OperatingSystem {
    public bool running = true;
    
    //initialize in constructor
    public int cursorRow;
    public int cursorCol;
    int displayHeight;
    int displayWidth;
    public char[,] screen;

    public void getInput() {
        ConsoleKeyInfo inputKey = Console.ReadKey(true);
        int asciiKey = (int)inputKey.KeyChar;
        //Console.WriteLine("Char Entered: {0}, ASCII Code: {1}", inputKey.KeyChar, asciiKey);
        //ReadKey does not return modifier keys (alt,shift,ctrl)


        Console.SetCursorPosition(cursorCol, cursorRow);
        Console.Write(inputKey.KeyChar);

        cursorCol++;

        
        Console.SetCursorPosition(1, 0);
        Console.Write(inputKey.KeyChar);

    }

    public void printScreen() {
        Console.Clear();
        for (int i = 0; i < displayHeight; i++) {
            for (int j = 0; j < displayWidth; j++) {
                Console.Write(screen[i, j]);
            }
            Console.WriteLine();
        }
    }

    public OperatingSystem(int _displayHeight, int _displayWidth, int _cursorRow, int _cursorCol) {
        displayHeight = _displayHeight;
        displayWidth = _displayWidth;
        cursorRow = _cursorRow;
        cursorCol = _cursorCol;
        screen = new char[displayWidth, displayHeight];


        for (int i = 0; i < displayHeight; i++) {
            for (int j = 0; j < displayWidth; j++) {
                screen[i, j] = 'd';
            }
        }
    }
}