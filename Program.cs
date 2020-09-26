using System;
using System.Collections.Specialized;

//get it so we can actually type things that change the state of the screen

namespace commandLineOS {
    public class OperatingSystem {
        public bool running = true;

        //initialize in constructor
        public int cursorRow;
        public int cursorCol;
        int displayHeight;
        int displayWidth;
        public char[,] screen = new char[10, 10];

        public void getInput() {
            ConsoleKeyInfo idiot = Console.ReadKey(true);
            Console.WriteLine(idiot.GetHashCode());
        }

        public void printScreen() {
            for (int i = 0; i < displayHeight; i++) {
                for (int j = 0; j < displayWidth; j++) {
                    Console.Write(screen[i, j]);
                }
                Console.WriteLine();
            }
        }

        public OperatingSystem(int displayHeight, int displayWidth, int cursorRow, int cursorCol) {
            this.displayHeight = displayHeight;
            this.displayWidth = displayWidth;
            this.cursorRow = cursorRow;
            this.cursorCol = cursorCol;

            for (int i = 0; i < displayHeight; i++) {
                for (int j = 0; j < displayWidth; j++) {
                    screen[i, j] = 'd';
                }
            }
        }
    }

    class Program {
        static void Main(string[] args) {
            OperatingSystem OS = new OperatingSystem(10,10,0,0);

            while (OS.running) {
                OS.printScreen();
                OS.getInput();
            }
        }
    }
}