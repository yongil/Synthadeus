////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Rounded Rectangle renderable object									  //
//   Justin Ross                                                              //
//   11-4-15                                                                  //
//                                                                            //
//   A rectangle with x and y radius to round the corners                  	  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Renderable.h"
#include "Rectangle2.h"

class RoundedRectangle : public Rectangle2
{
private:

	// radiuses for rounding the rectangle
	float xRadius;
	float yRadius;

public:

	// runtime type information
	RTTI_MACRO(RoundedRectangle);

	// construct a rounded rectangle
	RoundedRectangle(Point rectOrigin, Point rectSize, unsigned int lineColor, unsigned int bkgColor, float XRadius, float YRadius);

	// the ID2D commands
	virtual void render(Render2D* render2d, ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush** colorPalette, IDWriteTextFormat** fontPalette);
	
	// get the origin
	virtual Point getOrigin();
};

