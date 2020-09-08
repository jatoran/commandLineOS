using System;

namespace JimmyShaker {
    class OperatingSystem {
        public bool running = true;

        public int cursorRow;
        public int cursorCol;

        public void getInput() {
            ConsoleKeyInfo idiot = Console.ReadKey(true);
            Console.WriteLine(idiot.GetHashCode());
        }
    };

    class Program {
        static void Main(string[] args) { 
            OperatingSystem OS = new OperatingSystem();

            while (OS.running) {
                OS.getInput();
            }
        }
    }
}