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
			
			//! add a cue to the Sequence
			CueRef add( std::function<void ()> action, float atTime )
			{
				CueRef newCue( new Cue( action, atTime ) );
				newCue->setAutoRemove( mDefaultAutoRemove );
				insert( newCue );
				return newCue;
			}
			
			//! create a new tween and add it to the list
			template<typename T>
			TweenRef<T> add( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}
			
			//! create a new tween
			template<typename T>
			TweenRef<T> add( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}

			//! replace an existing tween
			template<typename T>
			TweenRef<T> replace( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				replace( newTween );
				return newTween;
			}
			
			//! replace an existing tween
			template<typename T>
			TweenRef<T> replace( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				replace( newTween );
				return newTween;
			}

			template<typename T>
			TweenRef<T> append( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, endValue, getDuration(), duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}

			template<typename T>
			TweenRef<T> append( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, getDuration(), duration, easeFunction, lerpFunction ) );
				newTween->setAutoRemove( mDefaultAutoRemove );
				insert( newTween );
				return newTween;
			}

			//! Appends to the end of the timeline mirror images of all items
			void appendPingPong();

			void replace( TimelineItemRef item );
			
			//! add an item to the timeline at the current time
			void add( TimelineItemRef item );
			//! adds an item to the timeline. Its start time is not modified
			void insert( TimelineItemRef item );
			//! adds an item to the timeline, setting its startTime to be at \a atTime
			void insert( TimelineItemRef item, float atTime ) { item->mStartTime = atTime; insert( item ); }

			size_t				getNumItems() const { return mItems.size(); }			
			TimelineItemRef		find( void *target );
			void				remove( TimelineItemRef tween );
			
			//! remove all tweens from the Sequence
			void clearSequence();
			//! remove completed tweens from the Sequence
			void clearCompletedTweens();
			//! Sets the time to zero, all tweens as not completed, and if \a unsetStarted, marks the tweens has not started
			void reset( bool unsetStarted = false );

			void	setDefaultAutoRemove( bool defaultAutoRemove ) { mDefaultAutoRemove = defaultAutoRemove; }
			bool	getDefaultAutoRemove() const { return mDefaultAutoRemove; }

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