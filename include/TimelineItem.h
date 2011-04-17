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
		class TimelineItem : public std::enable_shared_from_this<TimelineItem>
		{
		  public:
			TimelineItem( class Timeline *parent = 0 );
			TimelineItem( class Timeline *parent, void *target, float startTime, float duration );
			virtual ~TimelineItem() {}
			
			//! Returns the item's target pointer
			void* getTarget() const { return mTarget; }

			//! Returns the item's start time
			float			getStartTime() const { return mStartTime; }
			//! Set the items's start time to \a newTime
			void			setStartTime( float newTime );
			//! Set the items's start time to \a newTime. Returns a reference to \a this
			TimelineItemRef startTime( float newTime ) { setStartTime( newTime ); return shared_from_this(); }

			//! Pushes back the item's start time by \a amt. Returns a reference to \a this
			TimelineItemRef delay( float amt ) { setStartTime( mStartTime + amt ); return shared_from_this(); }

			//! Returns the item's duration
			float			getDuration() const { return mDuration; }			
			//! Sets the item's duration to \a newDuration.
			void			setDuration( float newDuration );
			//! Sets the item's duration to \a newDuration. Returns a reference to \a this
			TimelineItemRef duration( float newDuration ) { setDuration( newDuration ); return shared_from_this(); }

			//! Returns whether the item starts over when it is complete
			bool			getLoop() const { return mLoop; }
			//! Sets whether the item starts over when it is complete
			void			setLoop( bool doLoop = true ) { mLoop = doLoop; }
			//! Sets whether the item starts over when it is complete. Returns a reference to \a this
			TimelineItemRef loop( bool doLoop = true ) { setLoop( doLoop ); return shared_from_this(); }

			//! Returns the time of the item's competion, equivalent to getStartTime() + getDuration().
			float			getEndTime() const { return mStartTime + mDuration; }

			//! Marks the item as not completed, and if \a unsetStarted, marks the item as not started
			virtual void reset( bool unsetStarted = false ) { if( unsetStarted ) mHasStarted = false; mComplete = false; }
			
			//! Returns whether the item has started
			bool hasStarted() const { return mHasStarted; }			
			//! Returns whether the item has completed
			bool isComplete() { return mComplete; }
			
			//! Should the item remove itself from the Timeline when it is complete
			bool	getAutoRemove() const { return mAutoRemove; }
			//! Sets whether the item will remove itself from the Timeline when it is complete
			void	setAutoRemove( bool autoRemove = true ) { mAutoRemove = autoRemove; }
			//! Sets whether the item will remove itself from the Timeline when it is complete
			TimelineItemRef autoRemove( bool autoRmv = true ) { mAutoRemove = autoRmv; return shared_from_this(); }
			
			virtual void start() = 0;
			virtual void loopStart() {}
			virtual void update( float relativeTime ) = 0;
			virtual void complete() = 0;
			//! Call update() only at the beginning of each loop (for example Cues exhibit require this behavior)
			virtual bool updateAtLoopStart() { return false; }
			virtual void				reverse() = 0;
			//! Creates a cloned item which runs in reverse relative to a timeline of duration \a timelineDuration
			virtual TimelineItemRef		cloneReverse() const = 0;
			//! go to a specific time, generally called by the parent Timeline only.
			void stepTo( float time );
			
		  protected:
			//! Converts time from absolute to absolute based on item's looping attributes
			float	loopTime( float absTime );

			class Timeline	*mParent;
			
			void	*mTarget;
			float	mStartTime;
			bool	mHasStarted, mComplete;
			bool	mLoop;
			bool	mUseAbsoluteTime;
			bool	mAutoRemove;
			int32_t	mLastLoopIteration;
			
			friend class Timeline;
		  private:
			float	mDuration, mInvDuration;
		};
	}
}
