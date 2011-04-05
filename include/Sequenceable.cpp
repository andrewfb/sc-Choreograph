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

Sequenceable::Sequenceable( void *target, double startTime, double duration )
	: mTarget( target ), mStartTime( startTime ), mDuration( duration ), mReversed( false )
{
	mComplete = false;
}

void Sequenceable::stepTo( double newTime )
{
	if( mComplete )
		return;
	
	double relTime = math<double>::min( (newTime - mStartTime) / mDuration, 1 );
	if( newTime >= mStartTime + mDuration ) {
		mComplete = true;
	}
	
	updateTarget( relTime );	
}
			
} } // namespace cinder::tween