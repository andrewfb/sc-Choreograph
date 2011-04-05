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
#include "cinder/Function.h"
#include "Easing.h"
#include "Sequenceable.hpp"

namespace cinder {
	namespace tween {
		template<typename T>
		T lerp( const T &start, const T &end, double time )
		{
			return start * ( 1 - time ) + end * time;
		}
		
		//Our templated tween design
		class TweenBase : public Sequenceable {
		  public:
			TweenBase( void *target, double startTime, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut );
			virtual ~TweenBase() {}
			
		  protected:
			double	(*mEaseFunction)(double t);
			double	mDuration;
		};
		
		template<typename T>
		class Tween : public TweenBase {
		  public:
			typedef std::function<T (const T&, const T&, double)>	UpdateFunc;
		  
			// build a tween with a target, target value, duration, and optional ease function
			Tween( T *target, T endValue, double startTime, double duration,
					double (*easeFunction)(double t)=Quadratic::easeInOut, UpdateFunc updateFunction = &lerp<T> )
				: TweenBase( target, startTime, duration, easeFunction ), mStartValue( *target ), mEndValue( endValue ), mUpdateFunction( updateFunction )
			{
			}
			
			Tween( T *target, T startValue, T endValue, double startTime, double duration,
					double (*easeFunction)(double t)=Quadratic::easeInOut, UpdateFunc updateFunction = &lerp<T> )
				: TweenBase( target, startTime, duration, easeFunction ), mStartValue( startValue ), mEndValue( endValue ), mUpdateFunction( updateFunction )
			{
			}
			
			~Tween() {}
			
			// this could be modified in the future to allow for a PathTween
			virtual void update( double relativeTime )
			{
				*reinterpret_cast<T*>(mTarget) = mUpdateFunction( mStartValue, mEndValue, mEaseFunction( relativeTime ) );
			}
			
			//! change how the tween moves through time
			void setEaseFunction( double (*easeFunction)(double t) ) { mEaseFunction = easeFunction; }

			T	getStartValue() const { return mStartValue; }
			T	getEndValue() const { return mEndValue; }			
			T*	getTarget() const { return reinterpret_cast<T*>( mTarget ); }
			
		  protected:
			T	mStartValue, mEndValue;	
			
			UpdateFunc	mUpdateFunction;
		};
	} //tween
} //cinder