/*
 *  TimeBasis.h
 *  Clearing
 *
 *  Created by David Wicks on 6/21/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *	
 *	TimeBasis functions return a normalized time
 *	Given the start of a cycle and the duration of one cycle,
 *	They return a value in the range [0,1.0]
 *	
 *	They enable looping, ping-ponging, and reversing of tweens
 *  N.B. These are no longer used by the Choreograph library
 */

#pragma once

#include "cinder/CinderMath.h"

namespace cinder {
	namespace tween {
		struct TimeBasis {
			static float linear( float delta, float duration )
			{	
				return math<float>::min( delta / duration, 1 );
			}
			
			static float linearByFrame( float delta, float duration )
			{
				return math<float>::min( delta / duration, 1 );
			}
			
			static float pingpong( float delta, float duration )
			{
				float t2 = fmod( delta, duration * 2 );
				if( t2 > duration ){
					return ((duration*2) - t2)/duration;
				} else {
					return t2/duration;
				}
			}
			
			static float repeat( float delta, float duration )
			{
				float t = fmod( delta, duration );
				return t / duration;
			}
			
			static float reverse( float delta, float duration )
			{
				return math<float>::max( 1 - ( delta / duration ), 0 );
			}
		};
	}
}
