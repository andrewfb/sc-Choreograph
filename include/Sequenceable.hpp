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
			Sequenceable() : mTarget( 0 ), mDuration( 0 ), mAutoRemove( false ) {}
			virtual ~Sequenceable() {}
			
			void *getTarget() const { return mTarget; }
			
			//! go to a specific time
			void stepTo( double time );
			virtual void update( double relativeTime ) = 0;
			virtual void complete() = 0;
			
			double getStartTime() const { return mStartTime; }
			double getEndTime() const { return mStartTime + mDuration; }
			double getDuration() const { return mDuration; }			
			
			//! set the action's start time
			void setStartTime( double time ) { mStartTime = time; }
			void setDuration( double duration ) { mDuration = duration; }
			//! push back the action's start time
			void delay( double amt ) { mStartTime += amt; }
			
			//! change how time behaves
			void reverse( bool isReversed = true ){ mReversed = isReversed; }
			//! loop infinitely:-1, count:1-n
			//void loop(){}
			//! pinpong infinitely:-1, count:1–n
			//void pingpong(){}
			
			//! is the sequenceable action finished?
			virtual bool isComplete() { return mComplete; }
			
			bool	isAutoRemove() const { return mAutoRemove; }
			bool	setAutoRemove( bool autoRemove = true ) { mAutoRemove = autoRemove; }
			
		  protected:
			void	*mTarget;
			double	mStartTime, mDuration;
			bool	mComplete;
			bool	mReversed;
			bool	mAutoRemove;
		};
		
		typedef std::shared_ptr<Sequenceable> SeqRef;
	}
}
