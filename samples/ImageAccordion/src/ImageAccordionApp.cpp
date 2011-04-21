
/*
 *
 * All photos copyright Trey Ratcliff: http://www.stuckincustoms.com/
 *
 */


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIO.h"
#include "Resources.h"
#include "cinder/Rand.h"

#include "Choreograph.h"

#include "AccordionItem.h"

#include <list>
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageAccordionApp : public AppBasic {
  public:
	
	void prepareSettings(Settings *settings);
	void setup();
	void mouseMove( MouseEvent event );
	//void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	
	float mStep;
	
	
	int				mTotalItems;
	
	float			mAnimDuration;
	EaseFn			mAnimEase;
	float			mItemExpandedWidth;
	float			mItemRelaxedWidth;
	float			mItemHeight;
	
	vector<gl::Texture>				mImages;
	
	list<AccordionItem>				mItems;
	list<AccordionItem>::iterator	mCurrentSelection;
	
	Timeline						mTimeline;
	
	
};



void ImageAccordionApp::prepareSettings(Settings *settings) {
	settings->setWindowSize(848,564);
	settings->setFrameRate(60);
	settings->setTitle("ImageAccordion");
}


void ImageAccordionApp::setup() {
	
	mStep = 1.0 / 60.0;
	mAnimEase = EaseOutAtan(25);
	mAnimDuration = 0.7f;
	
	mTotalItems = 8;
	mItemExpandedWidth = 500;
	mItemRelaxedWidth = 848/mTotalItems;
	mItemHeight = 564;
	
	
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_1 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_2 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_3 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_4 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_5 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_6 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_7 ) ) ) );
	mImages.push_back( gl::Texture( loadImage( loadResource( IMAGE_8 ) ) ) );
	
	
	float xPos = 0;
	
	for( int m = 0; m < mTotalItems; ++m ) {
		mItems.push_back( AccordionItem(xPos, 0, mItemHeight, mItemRelaxedWidth, mItemExpandedWidth, mImages[m]) );
		xPos += mItemRelaxedWidth;
	}
	
	// similar to mCurrentSelection = null;
	mCurrentSelection = mItems.end();
	
}



void ImageAccordionApp::mouseMove( MouseEvent event ) {
	
	
	list<AccordionItem>::iterator mNewSelection = mItems.end();
	
	for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
		if( itemIt->isPointIn( event.getPos() ) ) {
			mNewSelection = itemIt;
			break;
		}
	}
	
	
	
	if (mNewSelection == mCurrentSelection) {
		//cout << "do nothing" << endl;
	} else {
		//cout << "do something" << endl;
		
		float xPos = 0;
		float contractedWidth = (mTotalItems*mItemRelaxedWidth - mItemExpandedWidth)/float(mTotalItems - 1);
		mCurrentSelection = mNewSelection;
		
		if (mCurrentSelection == mItems.end()) {
			for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
				mTimeline.apply( &itemIt->mX, xPos, mAnimDuration, mAnimEase );
				mTimeline.apply( &itemIt->mWidth, mItemRelaxedWidth, mAnimDuration, mAnimEase );
				xPos += mItemRelaxedWidth;
			}
		} else {
			for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
				if( itemIt == mCurrentSelection ) {
					mTimeline.apply( &itemIt->mX, xPos, mAnimDuration, mAnimEase );
					mTimeline.apply( &itemIt->mWidth, mItemExpandedWidth, mAnimDuration, mAnimEase );
					xPos += mItemExpandedWidth;
				} else {
					mTimeline.apply( &itemIt->mX, xPos, mAnimDuration, mAnimEase );
					mTimeline.apply( &itemIt->mWidth, contractedWidth, mAnimDuration, mAnimEase );
					xPos += contractedWidth;
				}
			}
		}
		
	}
	
	
}


void ImageAccordionApp::update() {
	
	//mTimeline.step(mStep);
	mTimeline.stepTo( getElapsedSeconds() );
	
	for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
		itemIt->update();
	}
	
}


void ImageAccordionApp::draw() {
	
	gl::clear( Color( 1, 1, 1 ) );
	
	for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
		itemIt->draw();
	}
	
}


CINDER_APP_BASIC( ImageAccordionApp, RendererGl )


