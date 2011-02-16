/*
 *  Sequenceable.h
 *  BasicTween
 *
 *  Created by David Wicks on 2/14/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#pragma once

namespace cinder
{
	namespace tween
	{
		//! Base interface for anything that can go on a timeline
		class Sequenceable
		{
		  public:
			virtual ~Sequenceable() {};

			// increment time by \a timeDelta
			void step( double timeDelta ) { stepTo( getCurrentTime() + timeDelta ); }
			
			//! go to a specific time
			virtual void stepTo( double time ) = 0;
			
			//! is the animation finished?
			virtual bool isComplete() const { return false; }
			
			//! returns the duration of the sequenceable item
			virtual double getDuration() const = 0;
			
			virtual double getCurrentTime() const = 0;
		};
	}
}

