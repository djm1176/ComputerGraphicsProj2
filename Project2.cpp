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

#include "TextWindow.h"

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

TextWindow text_window;

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

	text_window = TextWindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 24, 24);

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
	text_window.keyboardCallback(key);

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
	text_window.mouseCallback(button, state, x, y);

}

//***********************************************************************************
void motionCallback(int x, int y) {
	text_window.motionCallback(x, y);

}

void reshapeCallback(int w, int h) {
	text_window.resize(w, h);

}

//***********************************************************************************
void draw() {
	text_window.render();

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
	outfile << text_window.getText();
	outfile.close();
}