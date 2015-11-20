////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   A Text Renderable                                                        //
//   Everett Moser                                                            //
//   11-9-15                                                                  //
//                                                                            //
//   A simple way to create renderable text elements                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Renderable.h"
#include "Vector2D.h"

// maximum length of renderable string
#define TEXT_MAX_STRING_LENGTH 256

class Text : public Renderable
{
private:
	// store the string, and rendering color, font, and rectangle
	char buffer[TEXT_MAX_STRING_LENGTH];
	int font;
	int color;
	Point origin, size;

public:
	RTTI_MACRO(Text);

	// set up the text renderable
	Text(char* string, Point textOrigin, Point textSize, int fontType, unsigned int fontColor);

	// overrides from renderable
	virtual void render(Render2D* render2d, ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush** colorPalette, IDWriteTextFormat** fontPalette);
	virtual Point getOrigin();
};
