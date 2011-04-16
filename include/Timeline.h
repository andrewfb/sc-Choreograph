/*
 *  Sequence.h
 *  BasicTween
 *
 *  Created by David Wicks on 5/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 *	Runs all Tween objects provided to it
 *	Presents factory methods for creating Tweens (the recommended approach)
 *
 */

#pragma once
#include "Tween.h"
#include "Cue.h"
#include "cinder/Cinder.h"
#include <vector>
#include "cinder/Color.h"
#include "cinder/Vector.h"

namespace cinder {
	namespace tween {

		typedef std::shared_ptr<class Timeline>		TimelineRef;
			
		class Timeline : public TimelineItem {		
		  public:
			Timeline();			

			static TimelineRef	create() { return std::shared_ptr<Timeline>( new Timeline() ); }

			//! advance time a specified amount
			void	step( float timestep );
			//! go to a specific time
			void	stepTo( float absoluteTime );
			
			float	getCurrentTime() const { return mCurrentTime; }
			
			//! add a cue to the Timeline add the start-time \a atTime
			CueRef add( std::function<void ()> action, float atTime )
			{
				CueRef newCue( new Cue( action, atTime ) );
				newCue->setAutoRemove( mDefaultAutoRemove );
				insert( newCue );
				return newCue;
			}
			
			//! Create a new tween and adds it to the timeline's current time
			template<typename T>
			TweenRef<T> add( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}
			
			//! Create a new tween and adds it to the timeline's current time
			template<typename T>
			TweenRef<T> add( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}

			//! Replaces any existing tweens on the \a target with a new tween at the timeline's current time
			template<typename T>
			TweenRef<T> apply( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				apply( newTween );
				return newTween;
			}
			
			//! Replaces any existing tweens on the \a target with a new tween at the timeline's current time
			template<typename T>
			TweenRef<T> apply( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				apply( newTween );
				return newTween;
			}

			//! Creates a new tween and adds it to the end of a timeline, settings its start time to the timeline's duration
			template<typename T>
			TweenRef<T> append( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, endValue, getDuration(), duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}
			
			//! Creates a new tween and adds it to the end of a timeline, settings its start time to the timeline's duration
			template<typename T>
			TweenRef<T> append( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, getDuration(), duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}

			//! Appends to the end of the timeline mirror images of all items
			void appendPingPong();

			//! Replaces any existing TimelineItems that match \a item's target and adds \a item to the timeline
			void apply( TimelineItemRef item );
			
			//! add an item to the timeline at the current time
			void add( TimelineItemRef item );
			//! adds an item to the timeline. Its start time is not modified
			void insert( TimelineItemRef item );
			//! adds an item to the timeline, setting its startTime to be at \a atTime
			void insert( TimelineItemRef item, float atTime ) { item->mStartTime = atTime; insert( item ); }

			//! Returns the number of items in the Timeline
			size_t				getNumItems() const { return mItems.size(); }
			//! Returns the first item in the timeline the target of which matches \a target
			TimelineItemRef		find( void *target );
			//! Removes the TimelineItem \a item from the Timeline
			void				remove( TimelineItemRef item );
			
			//! Remove all tweens from the Timeline
			void clear();
			//! Remove completed items from the Timeline
			void clearCompleted();
			//! Sets the time to zero, all tweens as not completed, and if \a unsetStarted, marks the tweens as not started
			void reset( bool unsetStarted = false );

			//! Sets the default \a autoRemove value for all future TimelineItems added to the Timeline
			void	setDefaultAutoRemove( bool defaultAutoRemove ) { mDefaultAutoRemove = defaultAutoRemove; }
			//! Returns the default \a autoRemove value for all future TimelineItems added to the Timeline
			bool	getDefaultAutoRemove() const { return mDefaultAutoRemove; }

			//! Call this to notify the Timeline if the \a item's start-time or duration has changed
			void	timeChanged( TimelineItem *item );

		  protected:
			virtual void reverse();
			virtual TimelineItemRef cloneReverse() const;
			virtual void start() {}
			virtual void loopStart();
			virtual void update( float absTime );
			virtual void complete() {}
			virtual bool wantsAbsoluteTime() { return true; }

			void							calculateDuration();
		
			bool							mDefaultAutoRemove;
			float							mCurrentTime;
			std::vector<TimelineItemRef>	mItems;
		};
	}

}