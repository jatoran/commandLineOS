using System;
using System.Collections.Specialized;

//namespace commandLineOS { //do we need this? program works without it
class Program {
    static void Main(string[] args) {
        OperatingSystem OS = new OperatingSystem(20,20,0,0);
        Console.CursorVisible = false;

        OS.printScreen();

        while (OS.running) {
            OS.getInput();
            OS.printScreen();
            System.Threading.Thread.Sleep(100);
        }
    }
}