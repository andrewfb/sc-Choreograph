
#pragma once

#include "cinder/Color.h"
#include "cinder/Rect.h"
#include "cinder/Area.h"
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include <vector>

using namespace ci;
using namespace gl;

class AccordionItem {
public:
	
	AccordionItem();
	AccordionItem( float x, float y, float height, float contractedWidth, float expandedHeight, Texture image );
	void update();
	void draw();
	bool isPointIn( const Vec2f &pt );
	float round( float n );
	
	float mX, mY, mWidth, mHeight, mExpandedWidth;
	
	Rectf mRect;
	Color mColor;
	
	Texture mImage;
	Area    mImageArea;
	
};


