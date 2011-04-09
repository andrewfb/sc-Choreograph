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
		Cue::Cue( std::function<void ()> fn, float atTime ):
			TimelineItem( 0, 0, atTime, 0 ), mFunction( fn )
		{
		}
		
		void Cue::loopStart()
		{
			if( mFunction )
				mFunction();
		}
		
		TimelineItemRef Cue::cloneReverse() const
		{
			return TimelineItemRef( new Cue( *this ) );
		}
	}

}