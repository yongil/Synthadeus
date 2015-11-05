#include "BackgroundGrid.h"

BackgroundGrid::BackgroundGrid(Point origin, Point size, Point cellSize, unsigned int lineColor, unsigned int bkgColor)
	: gridOrigin(origin), gridSize(size), gridCellSize(cellSize), gridColor(lineColor), gridBackground(bkgColor)
{
	gridOrigin.x = origin.x;
	gridOrigin.y = origin.y;
	gridSize.x = size.x;
	gridSize.y = size.y;
	gridCellSize.x = cellSize.x;
	gridCellSize.y = cellSize.y;
}

void BackgroundGrid::render(ID2D1HwndRenderTarget * renderTarget, ID2D1SolidColorBrush ** colorPalette)
{
	// fill background
	float p1x, p1y, p2x, p2y;
	p1x = gridOrigin[0];
	p1y = gridOrigin[1];
	p2x = gridOrigin[0] + gridSize[0];
	p2y = gridOrigin[1] + gridSize[1];

	//renderTarget->DrawRectangle(D2D1::RectF(gridOrigin.x, gridOrigin.y, 
	//	gridOrigin.x + gridSize.x, gridOrigin.y + gridSize.y), colorPalette[COLOR_WHITE], 10.f);

	// vertical lines
	for (int i = gridCellSize[0]; i < gridSize[0]; i += gridCellSize[0])
	{
		p1x = gridOrigin[0] + (float)i; p2x = gridOrigin.x + (float)i;
		p1y = gridOrigin[1]; p2y = gridOrigin[1] + gridSize[1];
		renderTarget->DrawLine(D2D1::Point2F(p1x, p1y),
			D2D1::Point2F(p1x, p1y), colorPalette[COLOR_RED], 1.f);
	}

	// horizontal lines
	/*for (int j = gridCellSize[0]; j < gridSize.y; j += gridCellSize.y)
	{
		p1y = (float)j + gridOrigin[1]; p2y = (float)j + gridOrigin[1];
		p1x = gridOrigin[0]; p2x = gridOrigin[0] + gridSize[0];
		renderTarget->DrawLine(D2D1::Point2F(p1x, p1y),
			D2D1::Point2F(p1x, p1y), colorPalette[gridColor], 1.f);
	}*/
}