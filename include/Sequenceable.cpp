/*
 *  Sequenceable.cpp
 *  BasicTween
 *
 *  Created by Andrew Bell on 4/5/11.
 *  Copyright 2011 The Mill. All rights reserved.
 *
 */

#include "Sequenceable.hpp"
#include "cinder/CinderMath.h"

namespace cinder { namespace tween { 

Sequenceable::Sequenceable( void *target, float startTime, float duration )
	: mTarget( target ), mStartTime( startTime ), mDuration( duration ), mReversed( false ), mHasStarted( false ), mComplete( false ), mAutoRemove( true )
{
}

void Sequenceable::stepTo( float newTime )
{
	if( mComplete )
		return;
	
	float invDuration = ( mDuration <= 0 ) ? 1 : ( 1 / mDuration );
	
	float relTime = math<float>::min( (newTime - mStartTime) * invDuration, 1 );
	if( newTime >= mStartTime ) {
		if( ! mHasStarted ) {
			mHasStarted = true;
			start();
		}
		update( relTime );		
	}
	if( newTime >= mStartTime + mDuration ) {
		mComplete = true;
		complete();
	}
}
			
} } // namespace cinder::tween