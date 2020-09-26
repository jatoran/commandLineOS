//future stuff: kadane's, djikstra's, A*, shunting/prefix/postfix/infix/minmaxGame/hash functions/ hash tables/ cryptography
//median of medians, quickselect, huffman coding, knuth morris pratt algorithm
//creating files that are executable (batch and other stuff)/intellisense/input memory (history of typed commands with up/down arrow keys)
//some system environment variables that are handled in the system object like how many commands are kept track of and executable paths etc.
//make cd work with absolute paths in general instead of just currently viewable folders and the parent (..) folder
//serialization (which is file i/o)
//color possibility: https://repl.it/@PasAdam/Drawing-pro#main.cpp
//errors
//  ex1: try to enter a nonexistent directory: "directory "jfkdlsjfkl" doesn't exist
//  ex2: try to type the conents of a nonexistent file
//  ex3: try to remove a nonexistent file or directory
//multiple files - dont do this cuz going to C#


//TO DO:
//exit command

//no blank line directly below commands

//change error text to the names of  commands, not the function names

//put all constants into a struct called OSParameters that you pass into OS object (in constructor if necessary)

//keep history of typed commands (including commands that resulted in errors) and when the user types the up arrow or down arrow, it cycles through commands. it loops around
	//and keeps track of the last 4 typed commands
	//when hit up arrow - needs to also save current line? no, it is already saved in screen array, so hitting down arrow when inputhistoryIndex = max takes you back to screen array on currRow, and hitting up arrow when inputhistoryIndex = 0 also takes you back to screen array on currRow

//folder path max size? (abbreviates with ... if pass that size?)
	//if more than 20 char, then have it occupy 20 char and have a ... from the start of the string (root)

//screen array bounds checking
	//no width limits - just wraps around but is technically on same row?
		//how to implement this, would i just overflow it into the next screen array row? this would fuck with the scrolling
			//check if too long, then it wraps
				//will make backspacing more complicated
					//so keep a buffer of rows

//make system messages stand out more from user input

#include <iostream>
#include <thread>
#include <conio.h> //_getch
#include <chrono> //this_thread, milliseconds
#include <array>
#include <Windows.h> //STD_OUTPUT_HANDLE for setCursorPosition
#include <string>
#include <vector>
#include <algorithm> //remove_if

using namespace std;

//Constants
static const int displayWidth = 100;
static const int displayHeight = 25;
static const int inputHistoryCapacity = 5;
static const int consoleText = 15; //15 = bright white
static const int consoleBackground = 26; //26 = blue

//Forward Decs
class File;
class Folder;
class Command;
class Program;
class OperatingSystem;

class Command {
public:
	string key;
	void (*fcnPtr)(string input, string key, OperatingSystem* OS);
	string helpSyntax;
	string helpSummary;

	Command(string c_key, void (*C_fncPtr)(string input, string key, OperatingSystem* OS), string c_helpSyntax, string c_helpExample) {
		key = c_key;
		fcnPtr = C_fncPtr;
		helpSyntax = c_helpSyntax;
		helpSummary = c_helpExample;
	}
};

class CommandHandler {
public:
	vector<Command*> commands;

	CommandHandler() {
		initCommands();
	};
	void initCommands();
	void processInput(OperatingSystem* OS);
};

class Folder {
public:
	string title;
	Folder* parent;
	vector<Folder*> subFolders;
	vector<File*> subFiles;
	bool removeable = true;

	Folder(string d_title = "", Folder* d_parent = nullptr, bool d_removeable = true) {
		title = d_title;
		parent = d_parent;
		removeable = d_removeable;
	}
};

class OperatingSystem {
public:
	int currRow = 0;
	int currCol = 0;
	Folder root = { "root:", nullptr, false };
	Folder* currentFolder = &root;
	CommandHandler commandProcessor;
	array<array<char, displayWidth>, displayHeight> screen{ 0 };
	//array<array<char, displayWidth>, displayHeight> screenBuffer{ 0 };
	string folderPath = currentFolder->title;
	vector<string> inputHistory;
	string currentInput;

	void saveInputHistory();
	void getInput(string prompt = "");
	void updateFolderPathString();
	string parseStringFromPos(int position, string str);
	void printError(string errorText = "Undefined Error");
	void insertStringCurrRow(string str, int startPos = 0);
	void nextRow(int count = 1);
	bool checkStringAlphaNum(string title, int substringStartPos);
	void insertCharToScreen(int row, int col, char value);
};

class File {
public:
	string title;
	string contents;
	Folder* parent;

	File(string f_title = "", Folder* f_parent = nullptr, string f_contents = "") {
		title = f_title;
		parent = f_parent;
		contents = f_contents;
	}
};

void showCursor(bool showFlag){
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void setCursorPosition(int x, int y){
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void clear() {
	system("cls");
}

void printTopBorder(){
	cout << char(218);
	for (int k = 0; k < displayWidth + 1; k++) {
		cout << char(196);
	}
	cout << char(191);
	cout << endl;
}

void printLeftBorder() {
	cout << char(179);
}

void printRightBorder(int col, int row, OperatingSystem* OS) {
	setCursorPosition(displayWidth + 2, row);
	cout << char(179);
}

void printBottomBorder(int col, int row, OperatingSystem* OS) {
	cout << endl;
	cout << char(192);
	for (int k = 0; k < displayWidth + 1; k++) {
		cout << char(196);
	}
	cout << char(217);
}

void updateConsole(OperatingSystem* OS) {
		showCursor(false);
		
		if (OS->currRow < displayHeight) {
			for (int i = 0; i < OS->currRow + 1; i++) {
				for (int j = 0; j < displayWidth; j++) {
					//if (OS->screenBuffer[i][j] != OS->screen[i][j]) {
						setCursorPosition(j + 1, i + 2);
						cout << OS->screen[i][j];
					//}
				}
			}
			setCursorPosition(OS->currCol + 1, OS->currRow + 2);
		}

		else {
			for (int i = 0; i < displayHeight; i++) {
				setCursorPosition(1, i + 2);
				for (int j = 0; j < displayWidth; j++) {
					//if (OS->screenBuffer[i][j] != OS->screen[i][j]) {
						cout << OS->screen[(OS->currRow + i + 1) % displayHeight][j];
					//}
				}
			}
			setCursorPosition(OS->currCol + 1, displayHeight + 1);
		}
		showCursor(true);
		//OS->screenBuffer = OS->screen;
}

void setConsoleColor(unsigned short color) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	SetConsoleTextAttribute(hOut, color);
}

void initializeConsole(OperatingSystem* OS) {
	setConsoleColor(consoleText | consoleBackground);
	cout << "CONSOLE";
	
	//code to fill in blank spots with blue background
	for (int i = 0; i < displayWidth - 4; i++) {
		cout << " ";
	}

	cout << endl;
	printTopBorder();
	for (int i = 2; i < displayHeight + 2; i++) {
		setCursorPosition(0, i);
		printLeftBorder();
		printRightBorder(OS->currCol, i, OS);
	}
	printBottomBorder(OS->currCol, displayHeight + 1, OS);

	//code to fill in blank spots with blue background
	for (int i = 2; i < displayHeight + 2; i++) {
		for (int j = 1; j < displayWidth + 2; j++) {
			setCursorPosition(j, i);
			cout << " ";
		}
	}
	setCursorPosition(OS->currCol + 1, OS->currRow + 2);
	OS->updateFolderPathString();
	OS->insertStringCurrRow(OS->folderPath);
	updateConsole(OS);
	setCursorPosition(OS->folderPath.length() + 1, OS->currRow + 2);
	OS->currCol = OS->folderPath.length();
}

int getFolderIndex(string folderTitle, vector<Folder*>& children) {
	for (size_t i = 0; i < children.size(); i++)
		if (folderTitle == children[i]->title) return i;
	return -1;
}

int getFileIndex(string folderTitle, vector<File*>& children) {
	for (size_t i = 0; i < children.size(); i++)
		if (folderTitle == children[i]->title) return i;
	return -1;
}

void printSubFolders(OperatingSystem* OS, Folder* currFolder) {
	string str = "";
	for (size_t i = 0; i < currFolder->subFolders.size(); i++) {
		str.append("[" + currFolder->subFolders[i]->title + "] ");
	}
	if (str.length() > 0) {
		OS->insertStringCurrRow(str);
		OS->nextRow();
	}
}

void printSubFiles(OperatingSystem* OS, Folder* currFolder) {
	string str = "";
	for (size_t i = 0; i < currFolder->subFiles.size(); i++) {
		str.append("\"" + currFolder->subFiles[i]->title + "\" ");
	}
	if (str.length() > 0) {
		OS->insertStringCurrRow(str);
		OS->nextRow();
	}
}

bool OperatingSystem::checkStringAlphaNum(string title, int substringStartPos) {
	int size = title.size();
	if (size <= substringStartPos) return false;
	title = parseStringFromPos(substringStartPos, title);
	title.erase(remove_if(title.begin(), title.end(), [](char c) { return isalnum(c); }), title.end());
	if (title.size() > 0) return false;
	return true;
}

void OperatingSystem::saveInputHistory() {
	int i = 0;
	string newInput;
	while (screen[currRow % displayHeight][i] != NULL) {
		newInput.push_back(screen[currRow % displayHeight][i]);
		i++;
	}
	if (inputHistory.size() == inputHistoryCapacity) {
		inputHistory.erase(inputHistory.begin());
	}
	inputHistory.push_back(newInput);
}

void OperatingSystem::updateFolderPathString() {
	Folder* current = currentFolder;
	string newFolderPath = current->title;

	while (current->parent) {
		current = current->parent;
		newFolderPath.insert(0, "\\");
		newFolderPath.insert(0, current->title);
	}
	newFolderPath.append("\\>");
	folderPath = newFolderPath;
}

void traverse(string input, string key, OperatingSystem* OS) {
	int index;
	
	if (input.length() > 2) {
		input = OS->parseStringFromPos(3, input);
		if (input == ".." && OS->currentFolder->parent) {
			OS->currentFolder = OS->currentFolder->parent;
			OS->nextRow();
		}
		else {
			index = getFolderIndex(input, OS->currentFolder->subFolders);

			if (index != -1)
				OS->currentFolder = OS->currentFolder->subFolders[index];
			else OS->printError("dir error 1");
		}
	}
	else OS->printError("dir error 2");
}

void _deleteFolder(int index, Folder* parentFolder, OperatingSystem* OS) {
	string userInput;
	Folder* currFolder = parentFolder->subFolders[index];
	int fileCount = currFolder->subFiles.size();
	int folderCount = currFolder->subFolders.size();
	string currTitle = currFolder->title;

	if (folderCount == 0 && fileCount == 0) {
		OS->nextRow();
		OS->insertStringCurrRow("Folder [" + currFolder->title + "] deleted.");
		delete currFolder;
		parentFolder->subFolders.erase(parentFolder->subFolders.begin() + index);
	}

	if (folderCount == 0 && fileCount > 0) {
		OS->nextRow();
		OS->insertStringCurrRow("WARNING: Folder [" + currTitle + "] contains " + to_string(fileCount) + " files:");
		OS->nextRow();
		printSubFiles(OS, OS->currentFolder);
		OS->getInput("Confirm delete ? (y / n) ");

		if (OS->currentInput == "y") {
			OS->nextRow();
			OS->insertStringCurrRow("Folder [" + currFolder->title + "] deleted.");
			delete currFolder;
			parentFolder->subFolders.erase(parentFolder->subFolders.begin() + index);
			OS->nextRow();
		}
	}

	if (folderCount > 0) {
		OS->nextRow();
		OS->insertStringCurrRow("WARNING: Folder [" + currTitle + "] contains " + to_string(fileCount) + " files and " + to_string(folderCount) + " folders.");
		OS->getInput("Confirm delete ? (y / n) ");
		if (userInput == "y") {
			for (int i = 0; i < folderCount; i++)
				_deleteFolder(0, currFolder, OS);
			_deleteFolder(index, parentFolder, OS);
		}
	}
}

void deleteFolder(string input, string key, OperatingSystem* OS) {
	if (input.length() > 6) {
		input = OS->parseStringFromPos(6, input);
		int index = getFolderIndex(input, OS->currentFolder->subFolders);
		OS->nextRow();
		if (index != -1)
			_deleteFolder(index, OS->currentFolder, OS);
		else
			OS->printError("Folder does not exist");
	}
	else {
		OS->printError("Invalid syntax, type 'help rmdir' for syntax");
	}
}

void clearScreen(string input, string key, OperatingSystem* OS) {
	for (size_t i = 0; i < displayHeight; i++) {
		for (size_t j = 0; j < displayWidth; j++) {
			OS->insertCharToScreen(i, j, 0);
		}
	}
	OS->currRow = 0;
	OS->currCol = 0;
}

void createFolder(string input, string key, OperatingSystem* OS) {
	string duplicateError = "Folder name already taken";
	int commandLength = key.size();
	int size = input.size();

	if (size > commandLength + 1) {
		if (!OS->checkStringAlphaNum(input, 6)) {
			OS->printError("mkdir syntax error 1");
		}
		else if (input.find(" ") != commandLength) {
			OS->printError("mkdir syntax error 2");
		}
		else {
			input = OS->parseStringFromPos(6, input);
			if (getFolderIndex(input, OS->currentFolder->subFolders) != -1)
				OS->printError(duplicateError);
			else {
				OS->currentFolder->subFolders.push_back(new Folder(input, OS->currentFolder));
				OS->nextRow(2);
				OS->insertStringCurrRow("Folder [" + OS->currentFolder->subFolders.back()->title + "] created");
				OS->nextRow();
			}
		}
	}
	else OS->printError("mkdir syntax error 3");
}

void printFileContents(string input, string key, OperatingSystem* OS) {
	int index;
	input = OS->parseStringFromPos(5, input);
	index = getFileIndex(input, OS->currentFolder->subFiles);
	string error = "File does not exist";

	OS->nextRow(2);
	if (index != -1) {
		OS->insertStringCurrRow("Contents of " + input + ": ");
		OS->nextRow();
		OS->insertStringCurrRow(OS->currentFolder->subFiles[index]->contents);
	}
	else OS->printError(error);

	OS->nextRow();
}

void deleteFile(string input, string key, OperatingSystem* OS) {
	int index;
	string error = "no file with that name";
	
	if (input.length() > 4) {
		input = OS->parseStringFromPos(4, input);
		index = getFileIndex(input, OS->currentFolder->subFiles);
		if (index == -1)
			OS->printError(error);
		else {
			OS->nextRow(2);
			OS->insertStringCurrRow("File \"" + OS->currentFolder->subFiles[index]->title + "\" deleted.");
			delete OS->currentFolder->subFiles[index];
			OS->currentFolder->subFiles.erase(OS->currentFolder->subFiles.begin() + index);
			OS->nextRow();
		}
	}
	else {
		OS->printError("Invalid syntax. type 'help del' for da wei");
	}
}

void createFile(string input, string key, OperatingSystem* OS) {
	int countQuotes = 0;
	int countSpaces = 0;
	string newFileTitle = "";
	string newFileContent = "";
	string duplicateError = "Error. Filename already taken";
	int commandLength = key.size();
	int size = input.size();

	OS->nextRow();

	if (size > commandLength + 1) {
		if (input.find(" ") != commandLength) {
			OS->printError("create syntax error 1");
		}
		else {
			input = OS->parseStringFromPos(7, input);
			int countQuotes = count(input.begin(), input.end(), '\"');
			int countSpaces = count(input.begin(), input.end(), ' ');

			if (countSpaces == 0 && countQuotes == 0) {
				if (!OS->checkStringAlphaNum(input, 0)) {
					OS->printError("create syntax error 2");
				}
				else if (getFileIndex(input, OS->currentFolder->subFiles) != -1) {
					OS->printError("create syntax error 3");
				}
				else {
					OS->getInput("Enter file contents:");
					OS->currentFolder->subFiles.push_back(new File(input, OS->currentFolder, OS->currentInput));
					OS->currentInput = "";

					OS->nextRow();
					OS->insertStringCurrRow("File \"" + OS->currentFolder->subFiles.back()->title + "\" created");
					OS->nextRow();
				}
			}
			else if (countQuotes > 0 && countQuotes != 2) {
				OS->printError("create syntax error 4");
			}
			else {
				int spacePos = input.find(" ");
				int firstQuotePos = input.find("\"");
				if (input.back() != '\"') {
					OS->printError("create syntax error 5");
				}
				else if ((firstQuotePos - 1) != spacePos) {
					OS->printError("create syntax error 6");
				}
				else {
					newFileTitle = input.substr(0, spacePos);
					newFileContent = input.substr(firstQuotePos + 1, ((input.size() - newFileTitle.size()) - 3));

					if (!OS->checkStringAlphaNum(newFileTitle, 0)) {
						OS->printError("create syntax error 7");
					}
					else if (getFileIndex(newFileTitle, OS->currentFolder->subFiles) != -1) {
						OS->printError(duplicateError);
					}
					else {
						OS->currentFolder->subFiles.push_back(new File(newFileTitle, OS->currentFolder, newFileContent));

						OS->nextRow();
						OS->insertStringCurrRow("File \"" + OS->currentFolder->subFiles.back()->title + "\" created");
						OS->nextRow();
					}
				}
			}
		}
	}
	else OS->printError("create syntax error 8");
}

void printFolderContents(string input, string key, OperatingSystem* OS) {
	Folder* currFolder = &OS->root;
	vector<string> subStrings;
	int subFolderIndex;
	int spacePos = 0;
	size_t commandLength = key.size();
	int slashPos = 0;
	string newString = "";
	int slashCount = 0;

	OS->nextRow(2);
	if (input.size() > commandLength) {
		spacePos = input.find(' ');
		int spaceCount = count(input.begin(), input.end(), ' ');

		if (spacePos == commandLength && spaceCount == 1) {
			input = OS->parseStringFromPos(4, input);

			while (input.size() > 0) {
				slashPos = input.find("\\");
				slashCount = count(input.begin(), input.end(), '\\');
				if (slashCount == 0) {
					newString = input;
					input.erase(0, newString.length());
				}
				else {
					newString = input.substr(0, slashPos);
					input.erase(0, newString.length() + 1);
				}
				subStrings.push_back(newString);
			}

			bool exists = false;
			for (size_t i = 0; i < subStrings.size() - 1; i++) {
				if (subStrings[i] == currFolder->title) {
					subFolderIndex = getFolderIndex(subStrings[i + 1], currFolder->subFolders);
					if (subFolderIndex != -1) {
						currFolder = currFolder->subFolders[subFolderIndex];
						exists = true;
					}
					else {
						exists = false;
						break;
					}
				}
				else {
					exists = false;
					break;
				}
			}
			if (!exists) {
				OS->printError("Error: Folder does not exist");
			}
			else {
				newString = "";
				for (size_t i = 0; i < subStrings.size(); i++) {
					newString.append(subStrings[i] + "\\");
				}
				OS->insertStringCurrRow("Contents of: " + newString);
				OS->nextRow();
				printSubFolders(OS, currFolder);
				printSubFiles(OS, currFolder);
			}
		}
		else {
			OS->printError("printFolderContents error 1");
		}
	}
	else {
		OS->updateFolderPathString();

		newString.append("Contents of: " + OS->folderPath);
		OS->insertStringCurrRow(newString);

		OS->nextRow();
		printSubFolders(OS, OS->currentFolder);
		printSubFiles(OS, OS->currentFolder);
	}
}

void printHelp(string input, string key, OperatingSystem* OS) {
	vector<Command*> commands = OS->commandProcessor.commands;

	OS->nextRow(2);
	if (input.size() > 4) {
		input = OS->parseStringFromPos(5, input);
		for (size_t i = 0; i < commands.size(); i++) {
			if (commands[i]->key == input) {
				OS->insertStringCurrRow(commands[i]->helpSyntax + " - " + OS->commandProcessor.commands[i]->helpSummary);
				OS->nextRow();
				break;
			}
		}
	}

	else {
		for (size_t i = 0; i < OS->commandProcessor.commands.size(); i++) {
			OS->insertStringCurrRow(OS->commandProcessor.commands[i]->key + " - " + OS->commandProcessor.commands[i]->helpSummary);
			OS->nextRow();
		}
	}
}

void CommandHandler::initCommands() {
	commands.push_back(new Command("cd", traverse, "'cd [folder name]' OR 'cd ..'", "Change Folder"));
	commands.push_back(new Command("type", printFileContents, "'type [file name]'   eg. 'type blue'", "Print file contents"));
	commands.push_back(new Command("create", createFile, "'create [file name]' OR 'create [file name] \"file contents\"'   eg. 'create red' OR 'create red \"flower\"' - NOTE: File names may only contain numbers, letters, and underscores", "Create New File"));
	commands.push_back(new Command("del", deleteFile, "'del [file name]'   eg. 'del purple'", "Delete File"));
	commands.push_back(new Command("mkdir", createFolder, "'mkdir [folder name]'   eg. 'mkdir orange' - NOTE: Folder names may only contain numbers, letters, and underscores", "Create New Folder"));
	commands.push_back(new Command("rmdir", deleteFolder, "'rmdir [folder name]'   eg. 'rmdir yellow'", "Delete Folder"));
	commands.push_back(new Command("dir", printFolderContents, "'dir'", "Print contents of current folder"));
	commands.push_back(new Command("cls", clearScreen, "'cls'", "Clear Console"));
	commands.push_back(new Command("help", printHelp, "'help [command]'     eg. 'help cd'", "Print summary of list of commands or single command syntax + summary"));
}

void OperatingSystem::insertCharToScreen(int row, int col, char value) {
	if (col < displayWidth) {
		screen[row % displayHeight][col] = value;
	}
	else {
		//add functionality for out of bounds overflowing into next row
	}
}

void OperatingSystem::insertStringCurrRow(string str, int startPos ) { //startPos = 0 default
	for (size_t i = 0; i < str.size(); i++) {
		if (i + startPos > displayWidth) {
			printError("Input overflowed array");
			break;
		}
		else {
			insertCharToScreen(currRow, i + startPos, str[i]);
		}
	}
}

void OperatingSystem::printError(string errorText) {
	nextRow(2);
	insertStringCurrRow(errorText);
	nextRow();
}

void CommandHandler::processInput(OperatingSystem* OS) {
	string input = OS->currentInput;
	size_t index = 0;
	bool found = false;

	while (index < commands.size()) {
		if (input.find(commands[index]->key) == 0) {
			commands[index]->fcnPtr(input, commands[index]->key, OS);
			found = true;
			break;
		}
		else index++;
	}
	if (!found) {
		OS->printError("Command not found, type 'help' for commands.");
	}

	OS->nextRow();
	OS->updateFolderPathString();
	OS->insertStringCurrRow(OS->folderPath);
	OS->currCol = OS->folderPath.length();
	updateConsole(OS);
}

string OperatingSystem::parseStringFromPos(int position, string str) {
	string substring = str.substr(position, str.npos);
	return substring;
}

void OperatingSystem::nextRow(int count) { //default count = 1
	for (int i = 0; i < count; i++) {
		currRow++;
		for (int j = 0; j < displayWidth; j++) {
			insertCharToScreen(currRow, j, 0);
		}
	}	
	currCol = 0;
}

void OperatingSystem::getInput(string prompt) { //default prompt = blank
	//up       2 Inputs: 224 + 72
	//down     2 Inputs: 224 + 80
	//left     2 Inputs: 224 + 75
	//right    2 Inputs: 224 + 77
	//backspace -        8
	//tab -              9
	//enter =            13
	//SPACE =            32
	//!"#$%&'()*+,-./    33-47
	//int 0-9            48-57
	//:;<=>?@            58-64
	//A-Z                65-90
	//[\]^_`             91-96
	//a-z                97-122
	//{|}~               123-126
	if (prompt == "") {
		int input;
		input = _getch();

		char charInput = char(input);
		if (input == 13) { //enter
			saveInputHistory();
			commandProcessor.processInput(this);
			currentInput = "";
		}
		else if (input == 8) { //backspace
			if (size_t(currCol) > folderPath.length()) {
				insertCharToScreen(currRow, currCol - 1, 0);
				currCol--;
				currentInput.pop_back();
				updateConsole(this);
			}
		}
		else if (currCol < displayWidth) {
			insertCharToScreen(currRow, currCol, input);
			currentInput.push_back(input);
			currCol++;
			cout << charInput;
		}
	}
	else {
		currentInput = "";
		bool inputComplete = false;
		nextRow();
		insertStringCurrRow(prompt);
		nextRow();
		updateConsole(this);
		while (!inputComplete) {
			int input;
			input = _getch();

			char charInput = char(input);
			if (input == 13) { //enter
				saveInputHistory();
				inputComplete = true;
			}
			else if (input == 8) { //backspace
				if (size_t(currCol) > folderPath.length()) {
					insertCharToScreen(currRow, currCol - 1, 0);
					currCol--;
					currentInput.pop_back();
					updateConsole(this);
				}
			}
			else if (currCol < displayWidth) {
				insertCharToScreen(currRow, currCol, input);
				currentInput.push_back(input);
				currCol++;
				cout << charInput;
			}
		}
	}
}

int main() {
	OperatingSystem OS;

	initializeConsole(&OS);
	while (1) {
		OS.getInput();
	}

	return 0;
}