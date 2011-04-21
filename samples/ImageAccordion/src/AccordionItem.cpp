
/*
 *
 * All photos copyright Trey Ratcliff: http://www.stuckincustoms.com/
 *
 */

#include "cinder/gl/Texture.h"
#include "AccordionItem.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"
#include <cmath>

using namespace ci;

AccordionItem::AccordionItem() {}

AccordionItem::AccordionItem( float x, float y, float height, float contractedWidth, float expandedWidth, gl::Texture image ) {
	
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
	//return mRect.contains( pt );
	return mImageArea.contains( pt );
}

void AccordionItem::update() {
	mRect = Rectf(round(mX), round(mY), round(mX + mWidth), round(mY + mHeight));
	mImageArea = Area(round(mX), round(mY), round(mX + mWidth), round(mY + mHeight));
}

void AccordionItem::draw() {
	//gl::color(mColor);
	//gl::drawSolidRect(mRect);
	
	mImage.enableAndBind();
	gl::draw( mImage, mImageArea, mRect );
	//gl::draw( mImage, mImageArea );
	
}

float AccordionItem::round( float n ) {
	return floor(n+0.5f);
}



