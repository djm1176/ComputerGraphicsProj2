#pragma once
#include <GL/freeglut.h>
#include <vector>
#include <string>

class TextWindow {
public:
	TextWindow();

	//Render the TextWindow
	void render();

	//Tell the TextWindow to be resized to the new width and height
	void resize(GLint w, GLint h);

	//Tell the TextWindow that a key was pressed
	void keyboardCallback(int key);

	//Tell the TextWindow that the mouse state changed at pixel coordinates (x, y)
	void mouseCallback(int btn, int state, int x, int y);

	//Tell the TextWindow that the mouse moved with a button pressed to pixel coordinates (x, y)
	void motionCallback(int x, int y);

	//Change the font displayed to a new GL font
	void setFont(const void* font);

	//Change the font color to a new RGB (0-255) color
	void setColor(GLbyte* col);

	//Change the font color to a new RGB color
	void setColor(GLfloat* col);

private:
	//The actual strings that are displayed on the GUI
	std::vector<std::string> m_cachedDisplayString;

	//The displayed text is offset by (x, y). This accounts for borders, padding, etc.
	GLint m_offsetTextPos[2]{ 0, 0 };

	//The size of the TextEditor window
	GLint m_windowSize[2]{ 0, 0 };

	//Recalculates the vector of strings that is displayed on the GUI
	void recalculateCacheString();
};

