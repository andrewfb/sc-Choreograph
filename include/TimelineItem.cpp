/*
 *  TimelineItem.cpp
 *  BasicTween
 *
 *  Created by Andrew Bell on 4/5/11.
 *  Copyright 2011 The Mill. All rights reserved.
 *
 */

#include "TimelineItem.h"
#include "cinder/CinderMath.h"

namespace cinder { namespace tween { 

TimelineItem::TimelineItem( void *target, float startTime, float duration )
	: mTarget( target ), mStartTime( startTime ), mDuration( duration ), mHasStarted( false ),
		mComplete( false ), mAutoRemove( true ), mLoop( false ), mPingPong( false ), mLastLoopIteration( 0 )
{
}

void TimelineItem::stepTo( float newTime )
{
	if( mComplete )
		return;
	
	float invDuration = ( mDuration <= 0 ) ? 1 : ( 1 / mDuration );
	
	if( newTime >= mStartTime ) {
		float relTime;
		if( mPingPong ) {
			relTime = math<float>::fmod( (newTime - mStartTime) * invDuration, 2 );
			if( relTime > 1 )
				relTime = 2 - relTime;
		}
		else if( mLoop ) {
			relTime = math<float>::fmod( (newTime - mStartTime) * invDuration, 1 );
		}
		else
			relTime = math<float>::min( (newTime - mStartTime) * invDuration, 1 );
		
		if( ! mHasStarted ) {
			mHasStarted = true;
			start();
		}
		
		if( mLoop || mPingPong ) {
			int32_t loopIteration = static_cast<int32_t>( ( newTime - mStartTime ) * invDuration );
			if( updateAtLoopStart() ) {
				if( loopIteration != mLastLoopIteration ) {
					mLastLoopIteration = loopIteration;
					update( relTime );
				}
			}
			else
				update( relTime );
		}
		else
			update( relTime );
	}
	if( newTime >= mStartTime + mDuration && ( ! mLoop ) && ( ! mPingPong ) ) {
		mComplete = true;
		complete();
	}
}
			
} } // namespace cinder::tween