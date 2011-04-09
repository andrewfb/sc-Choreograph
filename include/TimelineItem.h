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
		typedef std::shared_ptr<class TimelineItem>	TimelineItemRef;

		//! Base interface for anything that can go on a Timeline
		class TimelineItem
		{
		  public:
			TimelineItem( void *target, float startTime, float duration );
			TimelineItem() : mTarget( 0 ), mDuration( 0 ), mAutoRemove( true ) {}
			virtual ~TimelineItem() {}
			
			void* getTarget() const { return mTarget; }

			//! Creates a cloned item which runs in reverse relative to a timeline of duration \a timelineDuration
			virtual void				reverse() = 0;
			virtual TimelineItemRef		cloneReverse() const = 0;
			
			//! go to a specific time
			void stepTo( float time );
			
			float getStartTime() const { return mStartTime; }
			float getEndTime() const { return mStartTime + mDuration; }
			float getDuration() const { return mDuration; }			
			
			//! set the action's start time
			void setStartTime( float time ) { mStartTime = time; }
			void setDuration( float duration ) { mDuration = duration; }
			//! push back the action's start time
			void delay( float amt ) { mStartTime += amt; }
			//! Sets the time to zero, not completed, and if \a unsetStarted, marks the tweens has not started
			virtual void reset( bool unsetStarted = false ) { if( unsetStarted ) mHasStarted = false; mComplete = false; }
			
			//! Has the item begun?
			bool hasStarted() const { return mHasStarted; }			
			//! Has the item finished?
			bool isComplete() { return mComplete; }
			
			//! Should the item remove itself from the Timeline when it is complete
			bool	isAutoRemove() const { return mAutoRemove; }
			//! Sets whether the item will remove itself from the Timeline when it is complete
			void	setAutoRemove( bool autoRemove = true ) { mAutoRemove = autoRemove; }
			
			bool	isLoop() const { return mLoop; }
			void	setLoop( bool loop = true ) { mLoop = loop; }

			virtual void start() = 0;
			virtual void loopStart() {}
			virtual void update( float relativeTime ) = 0;
			virtual void complete() = 0;
			//! Call update() only at the beginning of each loop (for example Cues exhibit require this behavior)
			virtual bool updateAtLoopStart() { return false; }
			//! Whether update() receives time ranging from [0,1] (when false) or [0,duration] (when true)
			virtual bool wantsAbsoluteTime() { return false; }
			
		  protected:
			//! Converts time from absolute to absolute based on item's looping attributes
			float	loopTime( float absTime );

			void	*mTarget;
			float	mStartTime, mDuration;
			bool	mHasStarted, mComplete;
			bool	mLoop;
			bool	mAutoRemove;
			int32_t	mLastLoopIteration;
			
			friend class Timeline;
		};
	}
}
