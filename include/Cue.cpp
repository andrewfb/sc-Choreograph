/*
 *  Cue.cpp
 *  BasicTween
 *
 *  Created by David Wicks on 2/24/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#include "Cue.h"

namespace cinder
{

	namespace tween
	{
		Cue::Cue( std::function<void ()> action, double atTime ):
		Sequenceable( &action ),
		mSignal(),
		mTime( atTime ),
		mLastTime( 0 )
		{
			mSignal.connect( action );
		}
		
		Cue::~Cue()
		{
			
		}
		
		void Cue::addReceiver( std::function<void ()> action )
		{
			mSignal.connect( action );
		}
		
		void Cue::stepTo( double time )
		{
			if ( ( time > mTime && mLastTime < mTime ) || ( mReversed && mLastTime > mTime && time < mTime ) )
			{
				mSignal();
			}
			
			mLastTime = time;
		}
	}

}