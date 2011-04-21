
#pragma once

#include "Choreograph.h"
#include "cinder/Color.h"
#include "cinder/Rect.h"
#include "cinder/Area.h"
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include <vector>

using namespace ci;
using namespace gl;

class AccordionItem {
public:
	
	AccordionItem();
	AccordionItem( Timeline *timeline, float x, float y, float height, float contractedWidth, float expandedHeight, Texture image );
	void update();
	void draw();
	
	bool isPointIn( const Vec2f &pt );
	void animExpand ( float newX, float newWidth );
	void animContract ( float newX, float newWidth );
	void animRelax ( float newX, float newWidth );
	void animTo ( float newX, float newWidth );
	float round( float n );
	
	float mX, mY, mWidth, mHeight, mExpandedWidth, mTextAlpha;
	
	Rectf mImageRect;
	Rectf mTextRect;
	
	Texture mImage;
	Texture mText;
	
	Area    mImageArea;
	Area    mTextArea;
	
	Timeline *mTimeline;
	
	float			mAnimDuration;
	EaseFn			mAnimEase;
};


