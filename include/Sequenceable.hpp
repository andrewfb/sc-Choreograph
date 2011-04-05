/*
 *  Sequenceable.h
 *  sc-Choreograph
 *
 *  Created by David Wicks on 2/14/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#pragma once
#include "cinder/Cinder.h"

namespace cinder
{
	namespace tween
	{
		//! Base interface for anything that can go on a Sequence
		class Sequenceable
		{
		public:
			Sequenceable( void *target, double startTime, double duration );
			Sequenceable() : mTarget( 0 ), mDuration( 0 ) {}
			virtual ~Sequenceable() {}
			
			const void	*getTarget() const { return mTarget; }
			
			//! go to a specific time
			void stepTo( double time );
			virtual void updateTarget( double relativeTime ) = 0;
			
			//! push back the action's start time
			void delay( double amt ) { mStartTime += amt; }
			//! set the action's start time
			void setStartTime( double time ) { mStartTime = time; }
			
			//! change how time behaves
			void reverse( bool isReversed = true ){ mReversed = isReversed; }
			//! loop infinitely:-1, count:1-n
			//void loop(){}
			//! pinpong infinitely:-1, count:1–n
			//void pingpong(){}
			
			//! is the sequenceable action finished?
			virtual bool isComplete() { return mComplete; }
			
		  protected:
			void	*mTarget;
			double	mStartTime, mDuration;
			bool	mComplete;
			bool	mReversed;			
		};
		
		typedef std::shared_ptr<Sequenceable> SeqRef;
	}
}
