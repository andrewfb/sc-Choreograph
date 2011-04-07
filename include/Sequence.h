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
			
		class Sequence : public Sequenceable {		
		  public:
			Sequence();			

			//! advance time a specified amount
			void step( float timestep );
			//! go to a specific time
			void stepTo( float absoluteTime );
			
			//! add a cue to the Sequence
			SeqRef add( std::function<void ()> action, float atTime )
			{
				mActions.push_back( SeqRef( new Cue( action, atTime ) ) );
				return mActions.back();
			}
			
			//! create a new tween and add it to the list
			template<typename T>
			TweenRef<T> add( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				return add( target, *target, endValue, duration, easeFunction, lerpFunction );
			}
			
			//! create a new tween
			template<typename T>
			TweenRef<T> add( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				mActions.push_back( SeqRef( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) ) );
				return std::static_pointer_cast< Tween<T> >( mActions.back() );
			}

			//! replace an existing tween
			template<typename T>
			TweenRef<T> replace( T *target, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				return replace( target, *target, endValue, duration, easeFunction, lerpFunction );
			}
			
			//! replace an existing tween
			template<typename T>
			TweenRef<T> replace( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = Quadratic::easeInOut, typename Tween<T>::LerpFn lerpFunction = &lerp<T> ) {
				TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
				replace( newTween );
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
virtual void update( float relativeTime) {}
virtual void complete() {}
			size_t getNumTweens() const { return mActions.size(); }
		private:
			float					mCurrentTime;
			std::vector< SeqRef >	mActions;
		};
	}

}