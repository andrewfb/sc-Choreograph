/*
 *  TimelineItem.cpp
 *  BasicTween
 *
 *  Created by Andrew Bell on 4/5/11.
 *  Copyright 2011 The Mill. All rights reserved.
 *
 */

#include "TimelineItem.h"
#include "Timeline.h"

#include "cinder/CinderMath.h"

namespace cinder { namespace tween { 

TimelineItem::TimelineItem( class Timeline *parent )
	: mParent( parent ), mTarget( 0 ), mStartTime( 0 ), mDuration( 0 ), mInvDuration( 0 ), mHasStarted( false ),
		mComplete( false ), mAutoRemove( true ), mLoop( false ), mLastLoopIteration( -1 ), mUseAbsoluteTime( false )
{
}

TimelineItem::TimelineItem( Timeline *parent, void *target, float startTime, float duration )
	: mParent( parent ), mTarget( target ), mStartTime( startTime ), mDuration( duration ), mInvDuration( duration == 0 ? 0 : (1 / duration) ), mHasStarted( false ),
		mComplete( false ), mAutoRemove( true ), mLoop( false ), mLastLoopIteration( -1 ), mUseAbsoluteTime( false )
{
}

void TimelineItem::stepTo( float newTime )
{
	if( mComplete )
		return;
	
	float absTime = newTime - mStartTime;
	
	if( newTime >= mStartTime ) {
		float relTime;
		if( mLoop ) {
			relTime = math<float>::fmod( absTime * mInvDuration, 1 );
		}
		else
			relTime = math<float>::min( absTime * mInvDuration, 1 );
		
		if( ! mHasStarted ) {
			mHasStarted = true;
			start();
		}
		
		float time = ( mUseAbsoluteTime ) ? absTime : relTime;
		
		if( mLoop ) {
			int32_t loopIteration = static_cast<int32_t>( ( newTime - mStartTime ) * mInvDuration );
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

void TimelineItem::setStartTime( float time )
{
	mStartTime = time;
	if( mParent )
		mParent->timeChanged( this );
}

void TimelineItem::setDuration( float duration )
{
	mDuration = duration;
	mInvDuration = duration == 0 ? 1 : ( 1 / duration );
	if( mParent )
		mParent->timeChanged( this );
}

float TimelineItem::loopTime( float absTime )
{
	float result = absTime;
	
	if( mLoop ) {
		result = math<float>::fmod( result * mInvDuration, 1 );
		result *= mDuration;
	}

	return result;
}

} } // namespace cinder::tween