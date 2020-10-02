using System;
using System.Collections.Specialized;

//namespace commandLineOS { //do we need this? program works without it
class Program {
    static void Main(string[] args) {
        OperatingSystem OS = new OperatingSystem(10,10,0,0);

        while (OS.running) {
            OS.printScreen();
            OS.getInput();
        }
    }
}