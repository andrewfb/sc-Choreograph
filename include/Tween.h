/*
 *  Tween.h
 *  BasicTween
 *
 *  Created by David Wicks on 5/26/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 *	Tweens any type that supports operators + * /
 *	Tween<type>( &object, toValue, overTime, withEasing );
 *
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/CinderMath.h"
#include "Easing.h"
#include "Sequenceable.hpp"

namespace cinder {
	namespace tween {
		//Our templated tween design
		class TweenBase : public Sequenceable {
		  public:
			TweenBase( void *target, double startTime, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut );
			virtual ~TweenBase() {}
			
		  protected:
			double	(*mEaseFunction)(double t);
			double	mDuration;
			bool	mComplete;
		};
		
		template<typename T>
		class Tween : public TweenBase {
		  public:
			// build a tween with a target, target value, duration, and optional ease function
			Tween( T *target, T endValue, double startTime, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut )
				: TweenBase( target, startTime, duration, easeFunction ), mStartValue( *target ), mEndValue( endValue )
			{
			}
			
			Tween( T *target, T startValue, T endValue, double startTime, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut )
				: TweenBase( target, startTime, duration, easeFunction ), mStartValue( startValue ), mEndValue( endValue )
			{
				mStartTime = startTime;
				mDuration = duration;
				mComplete = false;
				
				mEaseFunction = easeFunction;
			}
			
			~Tween() {}
			
			// this could be modified in the future to allow for a PathTween
			virtual void updateTarget( double relativeTime )
			{
				if( relativeTime > 0 && relativeTime < 1 ) {
					*reinterpret_cast<T*>(mTarget) = mStartValue + ( mEndValue - mStartValue ) * mEaseFunction( relativeTime );
				}
				else if( relativeTime == 1 ) {	// at the completion point, set to target value
					*reinterpret_cast<T*>(mTarget) = mEndValue;
					mComplete = true;
				}
			}
			
			//! change how the tween moves through time
			void setEaseFunction( double (*easeFunction)(double t) ) { mEaseFunction = easeFunction; }
			//! change the duration of the tween
			void setDuration( double duration ){ mDuration = duration; }
			//! returns the duration of the sequenceable item
			double getDuration(){ return mDuration; }
			
			T*		getTarget() const { return mTarget; }
			double	getStartTime() const { return mStartTime; }
			double	getEndValue() const { return mEndValue; }			
			bool	isComplete() const { return mComplete; }
			
		private:
			T	mStartValue, mEndValue;	
		};
	} //tween
} //cinder