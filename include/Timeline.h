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
			
		class Timeline : public TimelineItem {		
		  public:
			Timeline();			

			//! advance time a specified amount
			void step( float timestep );
			//! go to a specific time
			void stepTo( float absoluteTime );
			
			//! add a cue to the Sequence
			CueRef add( std::function<void ()> action, float atTime )
			{
				CueRef newCue( new Cue( action, atTime ) );
				add( newCue );
				return newCue;
			}
			
			//! create a new tween and add it to the list
			template<typename T>
			TweenRef<T> add( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				add( newTween );
				return newTween;
			}
			
			//! create a new tween
			template<typename T>
			TweenRef<T> add( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				add( newTween );
				return newTween;
			}

			//! replace an existing tween
			template<typename T>
			TweenRef<T> replace( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> )
			{
				TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				replace( newTween );
				return newTween;
			}
			
			//! replace an existing tween
			template<typename T>
			TweenRef<T> replace( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				replace( newTween );
				return newTween;
			}

			template<typename T>
			TweenRef<T> append( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, endValue, getDuration(), duration, easeFunction, lerpFunction ) );
				add( newTween );
				return newTween;
			}

			template<typename T>
			TweenRef<T> append( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, getDuration(), duration, easeFunction, lerpFunction ) );
				add( newTween );
				return newTween;
			}

			void replace( SeqRef item );
			
			//! add an action to the sequence
			void add( SeqRef item );

			
			SeqRef		find( void *target );
			void		remove( SeqRef tween );
			
			//! remove all tweens from the Sequence
			void clearSequence();
			//! remove completed tweens from the Sequence
			void clearCompletedTweens();
			//! reset time to zero
			void reset(){ stepTo( 0.0 ); }
virtual void start() {}
virtual void update( float relativeTime) {}
virtual void complete() {}
			size_t getNumTweens() const { return mActions.size(); }
		private:
			void					calculateDuration();
		
			float					mCurrentTime;
			std::vector< SeqRef >	mActions;
		};
	}

}