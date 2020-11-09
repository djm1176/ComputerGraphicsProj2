/*==================================================================================================
 #$(*&$(*&$#(*&%(*#&%(*&$(*&#$(*&#$(*&#$(*&#$(*&#%(@(#*$&$%^&@$^(%#*^$)*%(*&$#&@^*$(*^#*$*&@#&@^#&%^$*$%#*$
 #$(*&$(*&$#(*&%(*#&%(*&$(*&#$(*&#$(*&#$(*&#$(*&#%(@(#*$&$%^&@$^(%#*^$)*%(*&$#&@^*$(*^#*$*&@#&@^#&%^$*$%#*$
 #$(*&$(*&$#(*&%(*#&%(*&$(*&#$(*&#$(*&#$(*&#$(*&#%(@(#*$&$%^&@$^(%#*^$)*%(*&$#&@^*$(*^#*$*&@#&@^#&%^$*$%#*$
 BEFORE SUBMITTING:
	Fix, remove or finish all TODOs
	Review comments and remove any poor language/unnecessary comments
 #$(*&$(*&$#(*&%(*#&%(*&$(*&#$(*&#$(*&#$(*&#$(*&#%(@(#*$&$%^&@$^(%#*^$)*%(*&$#&@^*$(*^#*$*&@#&@^#&%^$*$%#*$
 #$(*&$(*&$#(*&%(*#&%(*&$(*&#$(*&#$(*&#$(*&#$(*&#%(@(#*$&$%^&@$^(%#*^$)*%(*&$#&@^*$(*^#*$*&@#&@^#&%^$*$%#*$
 #$(*&$(*&$#(*&%(*#&%(*&$(*&#$(*&#$(*&#$(*&#$(*&#%(@(#*$&$%^&@$^(%#*^$)*%(*&$#&@^*$(*^#*$*&@#&@^#&%^$*$%#*$
 COURSE:				  CSC 525/625
 ASSIGNMENT:			  Project 2
 PROGRAMMER:			  Eric McCullough
						  Alex Webber
						  Tyler Cardwell
						  Daniel Moore
						  John Meents
						  Ben Jinkerson
						  Dallas Bramel
 LAST MODIFIED DATE:	  10/23/2020
 DESCRIPTION:			  Text editor written in GLUT
 NOTE:					  N/A
 FILES:					  Project2.cpp, (labProj.sln, ...)
 IDE/COMPILER:			  Microsoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
		1.		Double click on labProj.sln	to OPEN the project
		2.		Press Ctrl+F7 to COMPILE
		3.		Press Ctrl+F5 to EXECUTE
==================================================================================================*/
#include <GL/freeglut.h>  // include GLUT library

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>

//********* Prototypes
void myInit();
void myDisplayCallback();
void menuInit();
void keyboardCallback(unsigned char, int, int);
void specialFuncCallback(int, int, int);
void mouseCallback(int, int, int, int);
void motionCallback(int, int);
void reshapeCallback(int, int);
void draw();
void helpDisplayCallback();
void helpKeyboardCallback(unsigned char, int, int);
void drawHelpText(std::string text, int length, int x, int y);
void drawHelp();
void save();

void recalculateDisplayString(int, int);

//********* Types
struct MousePosition {
	float x_coord;
	float y_coord;

	explicit MousePosition(float x_coord, float y_coord) : x_coord{ x_coord },
		y_coord{ y_coord }
	{
		// Intentionally empty due to using MIL
	}
};

class InvalidFileException : public std::runtime_error {
public:
	explicit InvalidFileException(const std::string& message = "")
		: std::runtime_error("Invalid file " + message + " either does not exist or could not be opened.") {
		// Empty due to using MIL
	}
};

//********* Globals

// TODO: Could we give this a more distinctive name since it's a global? Maybe all caps or something like file_text
std::string text = "Hello, world! This is a test with some text on screen. It is really sort of annoyingly long and it doesn't really mean anything, but it demonstrates how word wrapping should work.\nThis text should be on a new line.\n\nThis text is 2 lines below. Would you like a tab?\tThere's a tab.\n\tOh, what about a tab on a new line? Isn't that cool?";
//std::string text = "\n"; //for testing
//const std::string text = "Line 1\nLine 2\t<tab\n\nLine 3 with 1 empty line above\n\tLine 4 with tab at start";

std::vector<std::string> disp_text;

const GLint WINDOW_SIZE[]{ 800, 600 };
const GLint HELP_SIZE[]{ 700, 300 };
const GLint ORIGIN_OFFSET[]{ 32, 24 };

MousePosition mouse_position = MousePosition(0.0, 70.0);
int cursor_position = 0;
int fontChoise = 0;
int colorChoice = 0;
void* GLOBAL_FONT[] = { GLUT_BITMAP_9_BY_15,GLUT_BITMAP_TIMES_ROMAN_10,GLUT_BITMAP_HELVETICA_10 };
bool leftButton = false;
bool rightButton = false;

int mainWindow;
int helpWindow;

//********* Subroutines
void mainMenuHandler(int choice) {
}
void fontMenuHandler(int choice) {
	fontChoise = choice;
	myDisplayCallback();
}
void colorMenuHandler(int choice) {
	colorChoice = choice;
	myDisplayCallback();
}
void helpMenuHandler(int choice) {
	glutSetWindow(helpWindow);
	if (choice == 1) {
		glutShowWindow();
	}
	else if (choice == 0) {
		glutHideWindow();
	}
	
}
void saveMenuHandler(int choice) {
	save();
}
void exitMenuHandler(int choice) {
	exit(choice);
}
int main(int argc, char** argv) {

	glutInit(&argc, argv);  // initialization

	glutInitWindowSize(WINDOW_SIZE[0], WINDOW_SIZE[1]);                // specify a window size
	glutInitWindowPosition(0, 0);              // specify a window position
	mainWindow = glutCreateWindow("GLUT Text Editor");  // create a titled window

	myInit();  // specify some settings

	menuInit();

	glutDisplayFunc(myDisplayCallback);  // register a callback
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialFuncCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutReshapeFunc(reshapeCallback);

	glutInitWindowSize(HELP_SIZE[0], HELP_SIZE[1]);
	glutInitWindowPosition(825, 0);
	helpWindow = glutCreateWindow("GLUT Text Editor Help");
	myInit();

	glutDisplayFunc(helpDisplayCallback);
	glutKeyboardFunc(helpKeyboardCallback);

	glutMainLoop();  // get into an infinite loop

	return 1;  // something wrong happened
}

//***********************************************************************************
void myInit() {
	glClearColor(0.1, 0.12, 0.12, 0);  // specify a background clor: white
	gluOrtho2D(0, WINDOW_SIZE[0], WINDOW_SIZE[1], 0);  // specify a viewing area
}
//***********************************************************************************
void menuInit() {
	int fontSubMenu = glutCreateMenu(fontMenuHandler);
	glutAddMenuEntry("Default", 0);
	glutAddMenuEntry("Times Roman", 1);
	glutAddMenuEntry("Helvetica", 2);
	int colorSubMenu = glutCreateMenu(colorMenuHandler);
	glutAddMenuEntry("Default", 0);
	glutAddMenuEntry("red", 1);
	glutAddMenuEntry("blue", 2);
	int helpSubMenu = glutCreateMenu(helpMenuHandler);
	glutAddMenuEntry("Show Help Window", 1);
	glutAddMenuEntry("Hide Help Window", 0);
	int saveSubMenu = glutCreateMenu(colorMenuHandler);
	int exitAppMenu = glutCreateMenu(exitMenuHandler);
	glutAddMenuEntry("Yes", 1);
	glutAddMenuEntry("No", 0);
	glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Change Font", fontSubMenu);
	glutAddSubMenu("Change Color", colorSubMenu);
	glutAddSubMenu("Help", helpSubMenu);
	glutAddSubMenu("Save", saveSubMenu);
	glutAddSubMenu("Exit", exitAppMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCreateMenu(mainMenuHandler);
}
//***********************************************************************************
void myDisplayCallback() {
	glClear(GL_COLOR_BUFFER_BIT);  // draw the background

	draw();

	glFlush();  // flush out the buffer contents
}
//***********************************************************************************
void helpDisplayCallback() {
	glClear(GL_COLOR_BUFFER_BIT);  // draw the background

	drawHelp();

	glFlush();  // flush out the buffer contents
	glutSwapBuffers();
}

//***********************************************************************************
void keyboardCallback(unsigned char key, int x, int y) {

	//Note: keys 9 and 13 are defined explicitly. That's not necessary if you remove the "key < 31" block, that's for debugging.

	//TODO: Add support for:
		//CTRL+A = (char)1
		//CTRL+C = (char)3
		//CTRL+V = (char)22
		//Possibly an undo stack??!?!?!?!?! CTRL+Z?

	//TODO: This is very strange, but pressing the ctrl key with another key changes the value of
	// the key parameter, so I'm just using ALT for now, since it's the only one that allows 's' to be
	// seen unaltered.
	int mod = glutGetModifiers();
	if (key == 's' && mod == GLUT_ACTIVE_ALT) {
		try {
			save();
		}
		catch (const InvalidFileException& err) {
			// TODO figure out better file handling
			std::cout << err.what() << std::endl;
		}
	}
	else if (key == 8) {
		text = text.substr(0, text.length() - 1);
	}
	else if (key == 13) {
		text += '\n';
	}
	else if (key == 9) {
		text += '\t';
	}
	else if (key < 31) {
		//TODO: this if block is for debugging, remove this block!
		text += "<" + std::to_string(key) + ">";
	}
	else {
		text += key;
	}
	recalculateDisplayString(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	myDisplayCallback();
	glFlush();
}

void specialFuncCallback(int key, int x, int y) {
	text_window.specialFuncCallback(key);
}

//***********************************************************************************
void helpKeyboardCallback(unsigned char key, int x, int y) {
	switch (key) {
	case 81:
	case 113:
		glutHideWindow();
		break;
	}
}

//***********************************************************************************
void mouseCallback(int button, int state, int x, int y) {

}

//***********************************************************************************
void motionCallback(int x, int y) {

}

void reshapeCallback(int w, int h) {
	//Window resized, recalculate display
	recalculateDisplayString(w, h);

}

void recalculateDisplayString(int w, int h) {
	//Make the vector store a new version with word wrapping and new lines

	disp_text.clear(); //Does not guarantee reallocation
	int pos{ 0 };

	//Handle new lines
	disp_text.push_back((char)31 + text);

	//pos becomes next occurrence of \n each loop. If no more are detected, we are done
	while ((pos = disp_text.at(disp_text.size() - 1).find_first_of('\n')) != std::string::npos) {
		//Split the last element of the vector with 2 elements; last becomes up until newline, next becomes everything else
		//First char on each new line is tagged with a hidden control character
		disp_text.push_back((char)31 + disp_text.at(disp_text.size() - 1).substr(pos + 1)); //The last half ( + 1 'skips' the newline)
		disp_text.at(disp_text.size() - 2) = disp_text.at(disp_text.size() - 2).substr(0, pos); //The first half
	}

	//Handle word wrap

	//TODO: The -50 is a random magic num. Make this determined from constants or something
	//How many characters at the current font can be on a row
	//NOTE: 'W' is a pretty wide character; that is the reason for using its width for this calculation
	int MAX_ROW_CHARS = (w - 50) / glutBitmapWidth(GLOBAL_FONT[fontChoise], 'W');

	for (int i = 0; i < disp_text.size(); i++) {
		if (disp_text[i].length() > MAX_ROW_CHARS) {
			//Handle a word wrap
			int split_pos{ MAX_ROW_CHARS };
			//Backtrack from max chars in row until a whitespace
			for (; split_pos > 0 && !std::isspace(disp_text[i][split_pos]); split_pos--);

			disp_text.insert(disp_text.begin() + i + 1, disp_text[i].substr(split_pos + 1));
			disp_text[i] = disp_text[i].substr(0, split_pos);
		}
	}
}

//Draws the interface that surrounds the text display
void renderInterface() {
	//TODO: Render keyboard cursor, possibly add a blinking effect?
	const int _height = glutGet(GLUT_WINDOW_HEIGHT);

	//Render line number border
	glBegin(GL_QUADS);
	glVertex2i(ORIGIN_OFFSET[0] - 2, 0 * glutBitmapHeight(GLOBAL_FONT[fontChoise]) + ORIGIN_OFFSET[1] + (WINDOW_SIZE[1] - _height));
	glVertex2i(ORIGIN_OFFSET[0] - 1, 0 * glutBitmapHeight(GLOBAL_FONT[fontChoise]) + ORIGIN_OFFSET[1] + (WINDOW_SIZE[1] - _height));
	glVertex2i(ORIGIN_OFFSET[0] - 1, 0 * glutBitmapHeight(GLOBAL_FONT[fontChoise]) + ORIGIN_OFFSET[1] + (WINDOW_SIZE[1] - _height) - 15);
	glVertex2i(ORIGIN_OFFSET[0] - 2, 0 * glutBitmapHeight(GLOBAL_FONT[fontChoise]) + ORIGIN_OFFSET[1] + (WINDOW_SIZE[1] - _height) - 15);
	glEnd();
}

//Renders the contents of the string on screen
void renderFileText() {
	const int _height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor3f(0.08, 0.1, 0.1);
	glBegin(GL_QUADS);
	glVertex2i(0, WINDOW_SIZE[1] - _height);
	glVertex2i(ORIGIN_OFFSET[0], WINDOW_SIZE[1] - _height);
	glVertex2i(ORIGIN_OFFSET[0], _height);
	glVertex2i(0, _height);
	glEnd();


	for (int i = 0, line = 0; i < disp_text.size(); i++) {
		if (disp_text[i][0] == (char)31) {
			//This is a true 'newline', label such on the left
			//NOTE: The (WINDOW_SIZE[1] = _height) is because gluOrtho2d doesn't let you "anchor" to the top when changing screen size
			std::string line_num_str = std::to_string(line);
			glColor3f(0.5, 0.5, 0.5); //TODO: Make line number color constant
			glRasterPos2i((ORIGIN_OFFSET[0] / 2) - (line_num_str.length() * glutBitmapWidth(GLOBAL_FONT[fontChoise], 'W') / 2), i * glutBitmapHeight(GLOBAL_FONT) + ORIGIN_OFFSET[1] + (WINDOW_SIZE[1] - _height));
			glutBitmapString(GLOBAL_FONT[fontChoise], (const unsigned char*)line_num_str.c_str());
			line++;
		}
		if (colorChoice == 0) {
			glColor3f(0.8, 0.8, 0.8); //TODO: Make font color constant
		}
		else if (colorChoice == 1) {
			glColor3f(1,0, 0); 
		}
		else if (colorChoice == 2) {
			glColor3f(0, 0, 1);
		}
		
		glRasterPos2i(ORIGIN_OFFSET[0] + 4, i * glutBitmapHeight(GLOBAL_FONT[fontChoise]) + ORIGIN_OFFSET[1] + (WINDOW_SIZE[1] - _height));
		for (int j = 0; j < disp_text[i].length(); j++)
			if (disp_text[i][j] == 9) {
				//Tab character
				for (int k = 0; k < 4; k++) glutBitmapCharacter(GLOBAL_FONT[fontChoise], ' ');
			}
			else if (disp_text[i][j] > 31) {
				glutBitmapCharacter(GLOBAL_FONT[fontChoise], disp_text[i][j]);
			}
	}
}

//***********************************************************************************
void draw() {
	renderInterface();
	renderFileText();
}

//***********************************************************************************
void drawHelp() {

	std::string helpItems[10] = { "Welcome to the GLUT Text Editor, Version 1.0, November 2020",
	"To change properties about the text, right click the editor to view the menu",
	"   Select 'Font' to change the font", "   Select 'Color' to change the text color",
	"   Select 'Help' to return to this help window", "   Select 'Save' to save the text to a file",
	"   Select 'Exit' to leave the program", "Keyboard Shortcuts:", "   CTRL + S to save the file",
	"   CTRL + H for help" }; //

	std::string saveLoc = "The saved file is stored at C:\\Temp\\typed.txt";
	std::string done = "Press Q to terminate the help screen and to return to the editor.";

	for (int i = 0; i < sizeof(helpItems) / sizeof(helpItems[0]); i++)
	{
		drawHelpText(helpItems[i], 0, 10, (i * 35) + 40);
	}

	drawHelpText(saveLoc, 0, 10, 530);
	drawHelpText(done, 0, 10, 575);

}

void drawHelpText(std::string text, int length, int x, int y) {
	glColor3f(0.8, 0.8, 0.8);
	glRasterPos2i(x, y);
	for (auto c : text)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)c);
	}
}

void save() {
	std::string outFileName = "C:\\Temp\\type.txt";
	std::ofstream outfile(outFileName);
	if (!outfile.is_open()) {
		outfile.close();
		throw InvalidFileException(outFileName);
	}
	outfile << text;
	outfile.close();
}