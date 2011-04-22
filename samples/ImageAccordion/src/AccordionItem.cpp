
/*
 *
 * All photos copyright Trey Ratcliff
 * under Creative Commons Non-Commercial license
 *
 * http://www.stuckincustoms.com/
 * http://creativecommons.org/licenses/by-nc/2.5/
 * 
 */

#include "Choreograph.h"
#include "cinder/gl/Texture.h"
#include "AccordionItem.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/Text.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"
#include <string>
#include <cmath>

using namespace ci;
using namespace std;

AccordionItem::AccordionItem() {}

AccordionItem::AccordionItem( Timeline *timeline, float x, float y, float height, float contractedWidth, float expandedWidth, gl::Texture image, string title, string subtitle ) 
	: mTimeline(timeline), mX(x), mY(y), mWidth(contractedWidth), mHeight(height), mExpandedWidth(expandedWidth), mImage(image), mTitle(title), mSubtitle(subtitle)
{
#if defined( CINDER_COCOA_TOUCH )
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial-BoldMT" );
#else
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial Bold" );
#endif
	
	mAnimEase = EaseOutAtan(25);
	mAnimDuration = 0.7f;
	
	mTextAlpha = 0.0f;
	
	TextLayout layout;
	layout.clear( ColorA( 0.6f, 0.6f, 0.6f, 0.0f ) );
	layout.setFont( Font( boldFont, 26 ) );
	layout.setColor( Color( 1, 1, 1 ) );
	layout.addLine( mTitle );
	layout.setFont( Font( normalFont, 16 ) );
	layout.addLine( mSubtitle );
	layout.setBorder(11, 6);
	mText = gl::Texture( layout.render( true ) );
	
	mImageArea = mImage.getBounds();
	mTextArea = Area(0, round(mY), round(mWidth), round(mY + mHeight));
	
	mImageRect = Rectf(mX, mY, mX + mWidth, mY + mHeight);
	mTextRect = Rectf(mX, mY, mX + mWidth, mY + mHeight);
}


bool AccordionItem::isPointIn( const Vec2f &pt )
{
	return mImageRect.contains( pt );
}


void AccordionItem::animTo ( float newX, float newWidth, bool revealText )
{
	mTimeline->apply( &mX, newX, mAnimDuration, mAnimEase );
	mTimeline->apply( &mWidth, newWidth, mAnimDuration, mAnimEase );
	
	if (revealText) mTimeline->apply( &mTextAlpha, 1.0f, mAnimDuration*0.3f, EaseNone() );
	else mTimeline->apply( &mTextAlpha, 0.0f, mAnimDuration*0.3f, EaseNone() );
}


void AccordionItem::update()
{
	// rectangle to render image texture
	mImageRect = Rectf(round(mX), round(mY), round(mX + mWidth), round(mY + mHeight));
	// sample area of image texture to render
	mImageArea = Area(round(mX), round(mY), round(mX + mWidth), round(mY + mHeight));
	
	// rectangle to render text texture
	mTextRect = Rectf(round(mX), round(mY), round( mX + math<float>::min( mWidth, mText.getWidth() ) ), round( mY + math<float>::min( mHeight, mText.getHeight() ) ) );
	// sample area of text texture to render
	mTextArea = Area(0, 0, mTextRect.getWidth(), mTextRect.getHeight() );
}


void AccordionItem::draw()
{
	gl::color( Color(1,1,1) );
	gl::draw( mImage, mImageArea, mImageRect );
	gl::color( ColorA(1,1,1,mTextAlpha) );
	gl::draw( mText, mTextArea, mTextRect );
}


float AccordionItem::round( float n )
{
	return floor(n+0.5f);
}



