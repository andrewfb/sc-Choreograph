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
			//! add an action to the sequence
			void add( SeqRef );
			//! advance time based on target fps
			void step();
			//! advance time a specified amount
			void step( double timestep );
			//! go to a specific time
			void stepTo( double time );
			
			//! add a cue to the Sequence
			SeqRef add( std::function<void ()> action, double atTime )
			{
				mActions.push_back( SeqRef( new Cue( action, atTime ) ) );
				return mActions.back();
			}
			
			//! create a new tween and add it to the list
			template<typename T>
			std::shared_ptr< Tween<T> > add( T *target, T endValue, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut, typename Tween<T>::UpdateFunc updateFunction = &lerp<T> ) {
				mActions.push_back( SeqRef( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, updateFunction ) ) );
				return std::static_pointer_cast< Tween<T> >( mActions.back() );
			}
			
			//! create a new tween
			template<typename T>
			std::shared_ptr< Tween<T> > add( T *target, T startValue, T endValue, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut, typename Tween<T>::UpdateFunc updateFunction = &lerp<T> ) {
				mActions.push_back( SeqRef( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, updateFunction ) ) );
				return std::static_pointer_cast< Tween<T> >( mActions.back() );
			}
			
			//! replace an existing tween
			template<typename T>
			std::shared_ptr< Tween<T> > replace( T *target, T endValue, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut, typename Tween<T>::UpdateFunc updateFunction = &lerp<T> ) {
				SeqRef existingAction = find( target );
				if( existingAction )
					remove( existingAction );
				mActions.push_back( SeqRef( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, updateFunction ) ) );
				return std::static_pointer_cast< Tween<T> >( mActions.back() );
			}
			
			SeqRef		find( void *target );
			void		remove( SeqRef tween );
			
			//! remove all tweens from the Sequence
			void clearSequence();
			//! remove completed tweens from the Sequence
			void clearFinishedTweens();
			//! reset time to zero
			void reset(){ stepTo( 0.0 ); }
virtual void update( double relativeTime) {}			
		private:
			double					mCurrentTime;
			std::vector< SeqRef >	mActions;
		};
	}

}