#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Choreograph.h"

#include "AccordionItem.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ci::tween;

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
	tween::EaseFn	mAnimEase;
	float			mItemExpandedHeight;
	float			mItemRelaxedHeight;
	float			mItemWidth;
	
	list<AccordionItem>				mItems;
	list<AccordionItem>::iterator	mCurrentSelection;
	
	Timeline						mTimeline;
	
};



void ImageAccordionApp::prepareSettings(Settings *settings) {
	settings->setWindowSize(600,600);
	settings->setTitle("ImageAccordion");
}


void ImageAccordionApp::setup() {
	
	mStep = 1.0 / 60.0;
	mAnimEase = EaseOutAtan(25);
	mAnimDuration = 0.7f;
	
	mTotalItems = 8;
	mItemExpandedHeight = 200;
	mItemRelaxedHeight = 60;
	mItemWidth = 300;
	
	
	float yPos = 60;
	
	for( int m = 0; m < mTotalItems; ++m ) {
		mItems.push_back( AccordionItem(150, yPos, mItemWidth, mItemRelaxedHeight) );
		yPos += mItemRelaxedHeight;
	}
	
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
		
		float yPos = 60;
		float contractedHeight = (mTotalItems*mItemRelaxedHeight - mItemExpandedHeight)/float(mTotalItems - 1);
		mCurrentSelection = mNewSelection;
		
		if (mCurrentSelection == mItems.end()) {
			for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
				mTimeline.apply( &itemIt->mY, yPos, mAnimDuration, mAnimEase );
				mTimeline.apply( &itemIt->mHeight, mItemRelaxedHeight, mAnimDuration, mAnimEase );
				yPos += mItemRelaxedHeight;
			}
		} else {
			for( list<AccordionItem>::iterator itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt ) {
				if( itemIt == mCurrentSelection ) {
					mTimeline.apply( &itemIt->mY, yPos, mAnimDuration, mAnimEase );
					mTimeline.apply( &itemIt->mHeight, mItemExpandedHeight, mAnimDuration, mAnimEase );
					yPos += mItemExpandedHeight;
				} else {
					mTimeline.apply( &itemIt->mY, yPos, mAnimDuration, mAnimEase );
					mTimeline.apply( &itemIt->mHeight, contractedHeight, mAnimDuration, mAnimEase );
					yPos += contractedHeight;
				}
			}
		}
		
	}
	
	
}


void ImageAccordionApp::update() {
	
	mTimeline.step(mStep);
	
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


