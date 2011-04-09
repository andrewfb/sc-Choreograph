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
		mComplete( false ), mAutoRemove( true ), mLoop( false ), mLastLoopIteration( -1 )
{
}

void TimelineItem::stepTo( float newTime )
{
	if( mComplete )
		return;
	
	float absTime = newTime - mStartTime;
	float invDuration = ( mDuration <= 0 ) ? 1 : ( 1 / mDuration );
	
	if( newTime >= mStartTime ) {
		float relTime;
		if( mLoop ) {
			relTime = math<float>::fmod( absTime * invDuration, 1 );
		}
		else
			relTime = math<float>::min( absTime * invDuration, 1 );
		
		if( ! mHasStarted ) {
			mHasStarted = true;
			start();
		}
		
		float time = ( wantsAbsoluteTime() ) ? absTime : relTime;
		
		if( mLoop ) {
			int32_t loopIteration = static_cast<int32_t>( ( newTime - mStartTime ) * invDuration );
			if( loopIteration != mLastLoopIteration ) {
				mLastLoopIteration = loopIteration;
				loopStart();
				update( time );
			}
			else
				update( time );
		}
		else
			update( time );
	}
	if( newTime >= mStartTime + mDuration && ( ! mLoop ) ) {
		mComplete = true;
		complete();
	}
}

float TimelineItem::loopTime( float absTime )
{
	float result = absTime;

	float invDuration = ( mDuration <= 0 ) ? 1 : ( 1 / mDuration );
	
	if( mLoop ) {
		result = math<float>::fmod( result * invDuration, 1 );
		result *= mDuration;
	}

	return result;
}

} } // namespace cinder::tween