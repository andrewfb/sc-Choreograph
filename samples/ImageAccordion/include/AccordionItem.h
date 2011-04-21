
#pragma once

#include "Choreograph.h"
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
	AccordionItem( Timeline &timeline, float x, float y, float height, float contractedWidth, float expandedHeight, Texture image );
	void update();
	void draw();
	void animTo ( float newX, float newWidth );
	bool isPointIn( const Vec2f &pt );
	float round( float n );
	
	float mX, mY, mWidth, mHeight, mExpandedWidth;
	
	Rectf mRect;
	Color mColor;
	
	Texture mImage;
	Area    mImageArea;
	
	Timeline *mTimeline;
	
	float			mAnimDuration;
	EaseFn			mAnimEase;
	
};


