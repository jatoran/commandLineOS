# commandLineOS
command line OS

Organized by 1 class per file

To Do:
	get it so we can actually type things that change the state of the screen


Questions:
	Should we rename the Program class to Main or something less ambiguous?



Old Notes from C++ commandLine:
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
//s

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