////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Rectangle renderable object										      //
//   Justin Ross                                                              //
//   11-4-15                                                                  //
//                                                                            //
//   Stores the rectangle as 2 points                                     	  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector2D.h"
#include "Renderable.h"

// the name is 'Rectangle2' to avoid conflict with Windows API function of the same name
class Rectangle2 : public Renderable
{
protected:
	// data for the rectangle
	Point origin;
	Point size;
	unsigned int lineColor;
	unsigned int bkgColor;

public:
	RTTI_MACRO(Rectangle2);

	// set up a rectangle
	Rectangle2(Point rectOrigin, Point rectSize, unsigned int lineCol, unsigned int bkgCol);

	// overrides from renderable
	virtual void render(Render2D* render2d, ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush** colorPalette, IDWriteTextFormat** fontPalette);
	virtual Point getOrigin();
};
