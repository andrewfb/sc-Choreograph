
#pragma once

#include "Choreograph.h"
#include "cinder/Color.h"
#include "cinder/Rect.h"
#include "cinder/Area.h"
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include <string>
#include <vector>

using namespace ci;
using namespace gl;
using namespace std;

class AccordionItem {
public:
	
	AccordionItem();
	AccordionItem( Timeline *timeline, float x, float y, float height, float contractedWidth, float expandedHeight, Texture image, string title, string subtitle );
	void update();
	void draw();
	
	bool isPointIn( const Vec2f &pt );
	void animTo ( float newX, float newWidth, bool revealText = false );
	float round( float n );
	
	float mX, mY, mWidth, mHeight, mExpandedWidth, mTextAlpha;
	
	string mTitle, mSubtitle;
	
	Texture mImage, mText;
	Rectf mImageRect, mTextRect;
	Area    mImageArea,  mTextArea;
	
	Timeline		*mTimeline;
	float			mAnimDuration;
	EaseFn			mAnimEase;
};


