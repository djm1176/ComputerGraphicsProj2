#include "TextWindow.h"

TextWindow::TextWindow(int w, int h) {
	resize(w, h);
}

void TextWindow::render() {
	//Render background color
	glColor3ubv(BACKGROUND_COLOR);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(m_windowSize[0], 0);
	glVertex2i(m_windowSize[0], m_windowSize[1]);
	glVertex2i(0, m_windowSize[1]);
	glEnd();

	//Render text
	for (int i = 0; i < m_cachedDisplay.size(); i++) {
		//Each line number...

		//TODO: Render line number on left-hand side.
		//glColor3f...

		glColor3ubv(FONT_COLOR_TEXT);
		for (int j = 0; j < m_cachedDisplay.at(i).size(); j++) {
			//Each "word-wrap line"...
			for (int c = 0; c < m_cachedDisplay.at(i).at(j).length(); c++) {
				if (m_cachedDisplay.at(i).at(j)[c] == 9) {
					for (int k = 0; k < 4; k++) glutBitmapCharacter(m_font, ' '); //Render tabs
				} else {
					//Render all other characters
					glutBitmapCharacter(m_font, m_cachedDisplay.at(i).at(j)[c]);
				}
			}

		}

	}

}

void TextWindow::resize(GLint w, GLint h) {
	m_windowSize[0] = w;
	m_windowSize[1] = h;
	recalculate();
}

void TextWindow::keyboardCallback(int key) {
	if (key == 8) {
		m_text = m_text.substr(0, m_text.length() - 1);
	} else if (key == 13) {
		m_text += '\n';
	} else if (key == 9) {
		m_text += '\t';
	} else if (key < 31) {
		//TODO: this if block is for debugging, remove this block!
		m_text += "<" + std::to_string(key) + ">";
	} else {
		m_text += key;
	}
	recalculate();
}

void TextWindow::mouseCallback(int btn, int state, int x, int y) {
	switch (btn) {
	case GLUT_KEY_LEFT:
		m_leftMouseDown = (state == GLUT_KEY_DOWN);
	case GLUT_KEY_RIGHT:
		m_rightMouseDown = (state == GLUT_KEY_DOWN);
		break;
	}
	m_mousePos[0] = x;
	m_mousePos[1] = y;
}

void TextWindow::motionCallback(int x, int y) {

	recalculate();
}

void TextWindow::setFont(void* font) {
	m_font = font;
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

void TextWindow::recalculate() {
	//Make the vector store a new version with word wrapping and new lines

	m_cachedDisplay.clear();
	int pos{ 0 };

	//Handle new lines
	m_cachedDisplay.push_back(std::vector<std::string>{m_text});

	//pos becomes next occurrence of \n each loop. If no more are detected, we are done
	//.at(0) corresponds to the first element in the nested vector, which is always the only element in the vector at this point
	while ((pos = m_cachedDisplay
		.at(m_cachedDisplay.size() - 1)
		.at(0)
		.find_first_of('\n')) != std::string::npos) {
		//Split the last element of the vector with 2 elements; last becomes up until newline, next becomes everything else
		//First char on each new line is tagged with a hidden control character
		m_cachedDisplay.push_back(std::vector<std::string>{m_cachedDisplay
			.at(m_cachedDisplay.size() - 1)
			.at(0)
			.substr(pos + 1)}); // The + 1 skips the newline char
		m_cachedDisplay.at(m_cachedDisplay.size() - 2).at(0) = m_cachedDisplay.at(m_cachedDisplay.size() - 2).at(0).substr(0, pos); //The first half
	}

	//Handle word wrap

	//How many characters at the current font can be on a row
	//NOTE: 'W' is a reasonably wide character; that is the reason for using its width for this calculation
	int _max_row_chars = m_windowSize[0] / glutBitmapWidth(m_font, 'W');

	for (int i = 0; i < m_cachedDisplay.size(); i++) {
		for (int j = 0; j < m_cachedDisplay.at(i).size(); j++) {
			if (m_cachedDisplay.at(i).at(j).length() > _max_row_chars) {
				//Handle a word wrap
				int _split_pos{ _max_row_chars };

				//Backtrack from max chars in row until a whitespace
				for (; _split_pos > 0 && !std::isspace(m_cachedDisplay.at(i).at(j).at(_split_pos)); _split_pos--);
				m_cachedDisplay.at(i).insert(m_cachedDisplay.at(i).begin() + j + 1, m_cachedDisplay.at(i).at(j).substr(_split_pos + 1));
				m_cachedDisplay.at(i).at(j) = m_cachedDisplay.at(i).at(j).substr(0, _split_pos);
			}

		}
	}

}
