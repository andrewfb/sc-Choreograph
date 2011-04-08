/*
 *  TimelineItem.h
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
		//! Base interface for anything that can go on a Timeline
		class TimelineItem
		{
		  public:
			TimelineItem( void *target, float startTime, float duration );
			TimelineItem() : mTarget( 0 ), mDuration( 0 ), mAutoRemove( true ) {}
			virtual ~TimelineItem() {}
			
			void* getTarget() const { return mTarget; }
			
			//! go to a specific time
			void stepTo( float time );
			virtual void start() = 0;
			virtual void update( float relativeTime ) = 0;
			virtual void complete() = 0;
			
			float getStartTime() const { return mStartTime; }
			float getEndTime() const { return mStartTime + mDuration; }
			float getDuration() const { return mDuration; }			
			
			//! set the action's start time
			void setStartTime( float time ) { mStartTime = time; }
			void setDuration( float duration ) { mDuration = duration; }
			//! push back the action's start time
			void delay( float amt ) { mStartTime += amt; }
			
			//! Has the item begun?
			bool hasStarted() const { return mHasStarted; }			
			//! Has the item finished?
			virtual bool isComplete() { return mComplete; }
			
			//! Should the item remove itself from the Timeline when it is complete
			bool	isAutoRemove() const { return mAutoRemove; }
			//! Sets whether the item will remove itself from the Timeline when it is complete
			void	setAutoRemove( bool autoRemove = true ) { mAutoRemove = autoRemove; }
			
			bool	isLoop() const { return mLoop; }
			void	setLoop( bool loop = true ) { mLoop = loop; }

			//! Ping-pong overrides 'loop'
			bool	isPingPong() const { return mPingPong; }
			void	setPingPong( bool pingPong = true ) { mPingPong = pingPong; }
			
		  protected:
			void	*mTarget;
			float	mStartTime, mDuration;
			bool	mHasStarted, mComplete;
			bool	mPingPong, mLoop;
			bool	mAutoRemove;
		};
		
		typedef std::shared_ptr<TimelineItem>	TimelineItemRef;
	}
}
