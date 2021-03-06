#pragma once
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include <fstream>

class TextWindow {

public:
	TextWindow() = default;
	GLubyte FONT_COLOR_TEXT[3]{ 204, 204, 204 };
	//Construct a TextWindow with a width and height
	TextWindow(int w, int h, int padW = 0, int padH = 0);

	//Render the TextWindow
	void render();

	//Tell the TextWindow to be resized to the new width and height
	void resize(GLint w, GLint h);

	//Tell the TextWindow that a key was pressed
	void keyboardCallback(int key);

	//Callback for some special keys, such as Function and Numpad keys
	void specialFuncCallback(int key);

	//Tell the TextWindow that the mouse state changed at pixel coordinates (x, y)
	void mouseCallback(int btn, int state, int x, int y);

	//Tell the TextWindow that the mouse moved with a button pressed to pixel coordinates (x, y)
	void motionCallback(int x, int y);

	//Change the font displayed to a new GL font
	void setFont(void* font);

	//Change the padding of the text on all sides by (w, h)
	void setPadding(int w, int h);

	//Change the font color to a new RGB (0-255) color
	void setColor(GLubyte* col);

	//Set the window's text to the provided string
	void setText(const std::string& text);

	//Set the window's text directly, without recalculations
	void setText(std::vector<std::string> text);

	//Returns the text within the text editor
	std::string getText();

	//Save the text
	void save();

private:
	//Precomputed collection of strings that is used for displaying to the user
	std::vector<std::string> m_cachedDisplay;

	//The size of the TextEditor window
	GLint m_windowSize[2];

	//The padding in pixels between top left corner and where text is displayed  
	GLint m_textPadding[2]{ 0, 0 };

	//The font used to render the text
	void* m_font = GLUT_BITMAP_9_BY_15;

	//The font color used to render the text
	GLubyte m_fontColor[3];

	bool m_leftMouseDown, m_rightMouseDown;
	int m_mousePos[2];

	//Represents the location of the keyboard cursor in the cached display text
	int m_cursorRow{ 0 }, m_cursorCol{ 0 }, m_cursorX{ 0 }, m_cursorY{ 0 };

	//Recalculates and updates internal properties that are used to display the text graphics
	//The optional newStr parameter will replace the old contents of the cached displayed text.
	//If newStr is empty, the cached displayed text holds its old contents.
	void recalculate(const std::string& newStr = "");

private:
	GLubyte FONT_COLOR_DIM[3]{ 190, 190, 190 };

	//The first click or keyboard character in the window positions the cursor,
	//filling in rows/cols needed to put the cursor at that position.
	//Once this operation is performed, clicking works as normal.
	bool m_assignedStartPos = false;
};
