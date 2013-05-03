#include "Textbox.h"

Textbox::Textbox(LPDIRECT3DDEVICE9 d3ddev, int size, RECT coords)
{
	D3DXCreateFont(d3ddev,    // the D3D Device
                   size,    // font height of 30
				   0,    // default font width
                   FW_NORMAL,    // font weight
                   1,    // not using MipLevels
                   true,    // italic font
                   DEFAULT_CHARSET,    // default character set
                   OUT_DEFAULT_PRECIS,    // default OutputPrecision,
                   DEFAULT_QUALITY,    // default Quality
                   DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
                   "Arial",    // use Facename Arial
                   &dxfont);    // the font object

	//set up the coordinates of the textbox
	textbox = coords;

	text = "";
}

Textbox::~Textbox()
{
	dxfont->Release();
}

void Textbox::Draw()
{
    // draw the Hello World text
    dxfont->DrawTextA(NULL,
					  text.c_str(),
                      text.length(),
                      &textbox,
                      DT_LEFT | DT_TOP,
                      D3DCOLOR_ARGB(255, 255, 255, 255));
}

void Textbox::SetString(string input)
{
	text = input;
}

void Textbox::AppendToString(string input)
{
	text += "\n" + input;
}

void Textbox::ClearString()
{
	text = "";
}

void Textbox::AddToString(string input)
{
	text += input;
}