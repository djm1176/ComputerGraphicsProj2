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
#include <GL/freeglut.h> // include GLUT library

#include <string>
#include <iostream>
#include <vector>

#include "TextWindow.h"

#include "TextWindow.h"

//********* Prototypes
void mainWindowInit();
void helpWindowInit();
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
void* GLOBAL_FONT[] = { GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10, GLUT_BITMAP_HELVETICA_10 };
bool leftButton = false;
bool rightButton = false;

int mainWindow;
int helpWindow;

//********* Subroutines

void mainMenuHandler(int choice) {
	switch (choice) {
	case 0:
		text_window.save();
		break;
	case 1:
		glutDestroyWindow(helpWindow);
		glutDestroyWindow(mainWindow);
	default:
		break;
	}

}
void themeMenuHander(int choice) {
	//colorChoice = choice;
	if (choice == 1) {
		//light
		GLubyte textColor[]{ 255,0,0 };
		glClearColor(1.0, 1.0, 1.0, 0);  // specify a background clor: white
		text_window.setColor(textColor);
	} else if (choice == 2) {
		//dark
		GLubyte textColor[]{ 0,255,0 };
		glClearColor(0.0, 0.0, 0.0, 0);  // specify a background clor: black
		text_window.setColor(textColor);
	} else if (choice == 3) {
		//blue
		GLubyte textColor[]{ 153,50,204 };
		glClearColor(0.0, 0.0, 1, 0);  // specify a background clor: blue
		text_window.setColor(textColor);
	}

	myDisplayCallback();

}
void fontMenuHandler(int choice) {
	fontChoise = choice;
	text_window.setFont(GLOBAL_FONT[choice]);
	myDisplayCallback();
}

void colorMenuHandler(int choice) {
	if (choice == 1)//default
	{
		GLubyte textColor[]{ 204,204,204 };
		text_window.setColor(textColor);
	} else if (choice == 2)//red
	{
		GLubyte textColor[]{ 255,0,0 };
		text_window.setColor(textColor);
	} else if (choice == 3)//blue
	{
		GLubyte textColor[]{ 0,0,255 };
		text_window.setColor(textColor);
	}

	myDisplayCallback();
}

void helpMenuHandler(int choice) {
	glutSetWindow(helpWindow);
	if (choice == 1) {
		glutShowWindow();
	} else if (choice == 0) {
		glutHideWindow();
	}
}


int main(int argc, char** argv) {

	glutInit(&argc, argv); // initialization

	glutInitWindowSize(WINDOW_SIZE[0], WINDOW_SIZE[1]); // specify a window size
	glutInitWindowPosition(0, 0);						// specify a window position
	mainWindow = glutCreateWindow("GLUT Text Editor");	// create a titled window
	mainWindowInit(); // specify some settings
	menuInit();

	glutDisplayFunc(myDisplayCallback); // register a callback
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialFuncCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutReshapeFunc(reshapeCallback);

	glutInitWindowSize(HELP_SIZE[0], HELP_SIZE[1]);
	glutInitWindowPosition(825, 0);
	helpWindow = glutCreateWindow("GLUT Text Editor Help");
	helpWindowInit(); // specify some settings

	glutDisplayFunc(helpDisplayCallback);
	glutKeyboardFunc(helpKeyboardCallback);
	
	glutMainLoop(); // get into an infinite loop

	return 1; // something wrong happened
}

//***********************************************************************************
void mainWindowInit() {
	glClearColor(0.1, 0.12, 0.12, 0);  // specify a background clor: white
	gluOrtho2D(0, WINDOW_SIZE[0], WINDOW_SIZE[1], 0);  // specify a viewing area

	text_window = TextWindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 24, 24);

}

void helpWindowInit() {
	glClearColor(0.1, 0.12, 0.12, 0);  // specify a background clor: white
	gluOrtho2D(0, WINDOW_SIZE[0], WINDOW_SIZE[1], 0);  // specify a viewing area
}

//***********************************************************************************
void menuInit() {
	int themeSubMenu = glutCreateMenu(themeMenuHander);
	glutAddMenuEntry("Light", 1);
	glutAddMenuEntry("Dark", 2);
	glutAddMenuEntry("Blue", 3);
	int fontSubMenu = glutCreateMenu(fontMenuHandler);
	glutAddMenuEntry("Default", 0);
	glutAddMenuEntry("Times Roman", 1);
	glutAddMenuEntry("Helvetica", 2);
	int colorSubMenu = glutCreateMenu(colorMenuHandler);
	glutAddMenuEntry("Default", 1);
	glutAddMenuEntry("red", 2);
	glutAddMenuEntry("blue", 3);
	int helpSubMenu = glutCreateMenu(helpMenuHandler);
	glutAddMenuEntry("Show Help Window", 1);
	glutAddMenuEntry("Hide Help Window", 0);
	glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Change Theme", themeSubMenu);
	glutAddSubMenu("Change Font", fontSubMenu);
	glutAddSubMenu("Change Color", colorSubMenu);
	glutAddSubMenu("Help", helpSubMenu);
	glutAddMenuEntry("Save", 0);
	glutAddMenuEntry("Exit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCreateMenu(mainMenuHandler);
}
//***********************************************************************************
void myDisplayCallback() {
	glClear(GL_COLOR_BUFFER_BIT); // draw the background

	draw();

	glFlush(); // flush out the buffer contents
}
//***********************************************************************************
void helpDisplayCallback() {
	glClear(GL_COLOR_BUFFER_BIT); // draw the background

	drawHelp();

	glFlush(); // flush out the buffer contents
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

	std::string helpItems[12] = { "Welcome to the GLUT Text Editor, Version 1.0, November 2020", " ",
								 "To change properties about the text, right click the editor to view the menu",
								 "   Select 'Change theme' to change the editor color theme", "   Select 'Change font' to change the text font",
								 "   Select 'Change color' to change the font color", "   Select 'Help -> Show or Hide' to show or hide these instructions", "   Select 'Save' to save the text to a file",
								 "   Select 'Exit' to leave the program", " ", "Keyboard Shortcuts:", "   ALT + s to save the file" };

	std::string saveLoc = "The saved file is stored at C:\\Temp\\typed.txt";
	std::string done = "Press Q to terminate the help screen and to return to the editor.";

	for (int i = 0; i < sizeof(helpItems) / sizeof(helpItems[0]); i++) {
		drawHelpText(helpItems[i], 0, 10, (i * 35) + 40);
	}

	drawHelpText(saveLoc, 0, 10, 530);
	drawHelpText(done, 0, 10, 575);
}

void drawHelpText(std::string text, int length, int x, int y) {
	glColor3f(0.8, 0.8, 0.8);
	glRasterPos2i(x, y);
	for (auto c : text) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)c);
	}
}

