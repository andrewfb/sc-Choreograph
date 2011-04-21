
/*
 *
 * All photos copyright Trey Ratcliff: http://www.stuckincustoms.com/
 *
 */


#include "Choreograph.h"
#include "cinder/gl/Texture.h"
#include "AccordionItem.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/Text.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"
#include <cmath>

using namespace ci;

AccordionItem::AccordionItem() {}

AccordionItem::AccordionItem( Timeline &timeline, float x, float y, float height, float contractedWidth, float expandedWidth, gl::Texture image ) {
	
#if defined( CINDER_COCOA_TOUCH )
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial-BoldMT" );
#else
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial Bold" );
#endif
	
	mTimeline = &timeline;
	mAnimEase = EaseOutAtan(25);
	mAnimDuration = 0.7f;
	
	mX = x;
	mY = y;
	mHeight = height;
	mWidth = contractedWidth;
	mExpandedWidth = expandedWidth;
	
	mColor = Color(Rand::randFloat(1), Rand::randFloat(1), Rand::randFloat(1));
	
	mImage = image;
	mImageArea = mImage.getBounds();
	
	mRect = Rectf(mX, mY, mX + mWidth, mY + mHeight);
	
}

bool AccordionItem::isPointIn( const Vec2f &pt ) {
	return mImageArea.contains( pt );
}


void AccordionItem::animTo ( float newX, float newWidth ) {
	mTimeline->apply( &mX, newX, mAnimDuration, mAnimEase );
	mTimeline->apply( &mWidth, newWidth, mAnimDuration, mAnimEase );
}


void AccordionItem::update() {
	mRect = Rectf(round(mX), round(mY), round(mX + mWidth), round(mY + mHeight));
	mImageArea = Area(round(mX), round(mY), round(mX + mWidth), round(mY + mHeight));
}

void AccordionItem::draw() {
	mImage.enableAndBind();
	gl::draw( mImage, mImageArea, mRect );
}

float AccordionItem::round( float n ) {
	return floor(n+0.5f);
}



