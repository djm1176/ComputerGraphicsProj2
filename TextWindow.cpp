#include "TextWindow.h"
#include <iostream>
#include <sstream>

TextWindow::TextWindow(int w, int h) {
	resize(w, h);
}

void TextWindow::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2iv(m_windowSize);
	//Render text
	for (int i = 0; i < m_cachedDisplay.size(); i++) {
		//Each line of text

		//Render line number on left-hand side.
		glRasterPos2i(4, i * glutBitmapHeight(m_font) + m_textPadding[1]);
		glColor3ubv(FONT_COLOR_DIM);
		glutBitmapString(m_font, (unsigned char*)std::to_string(i).c_str());

		glColor3ubv(FONT_COLOR_TEXT);
		glRasterPos2i(m_textPadding[0], i * glutBitmapHeight(m_font) + m_textPadding[1]);

		for (int c = 0; c < m_cachedDisplay.at(i).length(); c++) {
			if (m_cachedDisplay.at(i)[c] == 9) {
				for (int k = 0; k < 4; k++) glutBitmapCharacter(m_font, 32); //Render tabs
			} else {
				//Render all other characters
				glutBitmapCharacter(m_font, m_cachedDisplay.at(i)[c]);
			}
		}
	}

	//Render keyboard cursor
	glBegin(GL_QUADS);
	glVertex2i(m_textPadding[0] + m_cursorX, m_textPadding[1] + m_cursorY);
	glVertex2i(m_textPadding[0] + m_cursorX + 2, m_textPadding[1] + m_cursorY);
	glVertex2i(m_textPadding[0] + m_cursorX + 2, m_textPadding[1] + m_cursorY + glutBitmapHeight(m_font));
	glVertex2i(m_textPadding[0] + m_cursorX, m_textPadding[1] + m_cursorY + glutBitmapHeight(m_font));

	glEnd();
}

void TextWindow::resize(GLint w, GLint h) {
	m_windowSize[0] = w;
	m_windowSize[1] = h;
	recalculate();
}

void TextWindow::keyboardCallback(int key) {
	
	std::string& _targetStr = m_cachedDisplay.at(m_cursorRow);
	if (key == 8) {
		//New line
		if (m_cursorCol == 0 && _targetStr.length() == 0)
			m_cachedDisplay.erase(m_cachedDisplay.begin() + m_cursorRow);
		else
			_targetStr.erase(_targetStr.begin() + m_cursorCol);
	}
	else if (key == 13) {
		_targetStr.insert(m_cursorCol, "\n");
	}
	else if (key == 9) {
		_targetStr.insert(m_cursorCol, "\t");
	}
	else if (key < 31) {
		//TODO: this if block is for debugging, remove this block!
		_targetStr.insert(m_cursorCol, "<" + std::to_string(key) + ">");
	}
	else {
		_targetStr.insert(m_cursorCol, 1, (char)key);
	}

	serialize();
	recalculate();
	render();
	glFlush();
}

void TextWindow::specialFuncCallback(int key) {
	switch (key) {
	case GLUT_KEY_LEFT:
	{
		m_cursorCol--;
		if (m_cursorCol < 0) m_cursorCol = 0;
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		m_cursorCol++;
		int len = m_cachedDisplay.at(m_cursorRow).length();
		if (m_cursorCol > len - 1) m_cursorCol = len - 1;
		break;
	}
	case GLUT_KEY_UP:
	{
		m_cursorSubRow--;
		if (m_cursorSubRow < 0) {
			if (m_cursorRow > 0) {
				m_cursorRow--;
				m_cursorSubRow = m_cachedDisplay.at(m_cursorRow).size() - 1;
			} else {
				m_cursorSubRow = 0;
			}
		}
		break;
	}
	case GLUT_KEY_DOWN:
	{
		m_cursorSubRow++;
		int len = m_cachedDisplay.at(m_cursorRow).size();
		if (m_cursorSubRow > len) {
			if (m_cursorRow < m_cachedDisplay.size()) {
				m_cursorRow++;
				m_cursorSubRow = 0;
			} else {
				m_cursorSubRow = len - 1;
			}
		}
		break;
	}
	default:
		return;
	}
	render();
	glFlush();
}

void TextWindow::serialize() {
	std::stringstream ss;
	for (int i = 0; i < m_cachedDisplay.size(); i++) {
			ss << m_cachedDisplay.at(i) << "\n";
	}
	m_text = ss.str();
}

void TextWindow::mouseCallback(int btn, int state, int x, int y) {
	switch (btn) {
	case GLUT_KEY_LEFT:
		m_leftMouseDown = (state == GLUT_KEY_DOWN);
	case GLUT_KEY_RIGHT:
		m_rightMouseDown = (state == GLUT_KEY_DOWN);
		break;
	}
	if (state == GLUT_KEY_UP) return;

	m_mousePos[0] = x;
	m_mousePos[1] = y;

	//Remove padding before calculations
	x -= m_textPadding[0];
	y -= m_textPadding[1];

	//Change y from 'pixel' coords to 'char' coords
	y /= glutBitmapHeight(m_font);
	y++;

	int c{ 0 }, pixels{ 0 };
	bool done = false;

	if (y >= m_cachedDisplay.size()) y = m_cachedDisplay.size() - 1;
	for (int i = 0, m_cursorOffset = 0; i < m_cachedDisplay.size(); i++) {
		m_cursorOffset += m_cachedDisplay.at(i).length();
	}

	for (; c < m_cachedDisplay.at(y).length(); c++) {
		char _char = m_cachedDisplay.at(y)[c];
		int w = glutBitmapWidth(m_font, _char);
		if (_char == 9) w = glutBitmapWidth(m_font, ' ') * 4; //Fixes tabs

		pixels += w;
		if (pixels >= x) {
			pixels -= w;
			break; //c now holds x index of cursor

		}
	}
	std::cout << m_cachedDisplay.at(y)[c] << std::endl;

	y--;
	m_cursorX = pixels;
	m_cursorY = y * glutBitmapHeight(m_font);
	m_cursorRow = y;
	m_cursorCol = c;

}

void TextWindow::motionCallback(int x, int y) {

	recalculate();
}

void TextWindow::setFont(void* font) {
	m_font = font;
	recalculate();
}

void TextWindow::setPadding(int w, int h) {
	m_textPadding[0] = w;
	m_textPadding[1] = h;
	recalculate();
}

void TextWindow::setColor(GLubyte* col) {
	m_fontColor[0] = col[0]; //R
	m_fontColor[1] = col[1]; //G
	m_fontColor[2] = col[2]; //B
	recalculate();
}


void TextWindow::setText(const std::string& text) {
	m_text = text;
	recalculate();
}

//collapse vector into a string
std::string TextWindow::getText() {
	std::stringstream returnString;
	for (unsigned i = 0; i < m_cachedDisplay.size(); i++)
	{
		returnString << m_cachedDisplay.at(i) << "\n";
	}

	return returnString.str();
}

void TextWindow::recalculate() {
	//Make the vector store a new version with word wrapping and new lines
	m_cachedDisplay.clear();
	int pos{ 0 };

	//Handle new lines
	m_cachedDisplay.push_back(std::string{ m_text });

	//pos becomes next occurrence of \n each loop. If no more are detected, we are done
	while ((pos = m_cachedDisplay
		.at(m_cachedDisplay.size() - 1)
		.find_first_of('\n')) != std::string::npos) {
		//Split the last element of the vector with 2 elements; last becomes up until newline, next becomes everything else
		//First char on each new line is tagged with a hidden control character
		m_cachedDisplay.push_back(std::string{ m_cachedDisplay
			.at(m_cachedDisplay.size() - 1)
			.substr(pos + 1) }); // The + 1 skips the newline char
		m_cachedDisplay.at(m_cachedDisplay.size() - 2) = m_cachedDisplay.at(m_cachedDisplay.size() - 2).substr(0, pos); //The first half
	}
	//Handle word wrap

	//How many characters at the current font can be on a row
	//NOTE: 'W' is a reasonably wide character; that is the reason for using its width for this calculation
	int _max_row_chars = (m_windowSize[0] - (m_textPadding[0] * 2)) / glutBitmapWidth(m_font, 'W');

	for (int i = 0; i < m_cachedDisplay.size(); i++) {
		if (m_cachedDisplay.at(i).length() > _max_row_chars) {
			//Handle a word wrap
			int _split_pos{ _max_row_chars };

			//Backtrack from max chars in row until a whitespace
			for (; _split_pos > 0 && !std::isspace(m_cachedDisplay.at(i).at(_split_pos)); _split_pos--);

			m_cachedDisplay.insert(m_cachedDisplay.begin() + i + 1, m_cachedDisplay.at(i).substr(_split_pos + 1));
			m_cachedDisplay.at(i) = m_cachedDisplay.at(i).substr(0, _split_pos);

		}
	}

}
