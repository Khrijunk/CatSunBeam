#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "../includes.h"

class Textbox
{
public:

	//The constructor takes the DirectX device, the font size, and the position of the 
	//textbox on the screen
	Textbox(LPDIRECT3DDEVICE9 d3ddev, int size, RECT coords);

	//The destructor removes the font object from memory
	~Textbox();

	//Handles the drawing of the text to the screen
	void Draw();

	//Erases the current text and replaces it with the given text
	void SetString(string input);

	//Concatinate's the given input with the current string with a newline at the beginning
	void AppendToString(string input);

	//clears out the string
	void ClearString();

	//Conctatinate the given input with the current string without a newline
	void AddToString(string input);

private:

	// the pointer to the font object
	LPD3DXFONT dxfont;

	//The dimentions of the textbox on the screen
	RECT textbox;

	//the string that is displayed in the textbox
	string text;
};

#endif