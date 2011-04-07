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

#include "Easing.h"
#include "Sequenceable.hpp"

#include "cinder/Cinder.h"
#include "cinder/CinderMath.h"
#include "cinder/Function.h"

//#include <boost/signals2.hpp>

namespace cinder {
	namespace tween {
		typedef float(*EaseFn)( float );
		
		template<typename T>
		T lerp( const T &start, const T &end, float time )
		{
			return start * ( 1 - time ) + end * time;
		}
		
		//Our templated tween design
		class TweenBase : public Sequenceable {
		  public:
			TweenBase( void *target, float startTime, float duration, EaseFn easeFunction = Quadratic::easeInOut );
			virtual ~TweenBase() {}

			//! change how the tween moves through time
			void	setEaseFn( EaseFn easeFunction ) { mEaseFunction = easeFunction; }
			EaseFn	getEaseFn() const { return mEaseFunction; }
			
		  protected:
			EaseFn		mEaseFunction;
			float		mDuration;
		};
				
		template<typename T>
		class Tween : public TweenBase {
		  public:
			typedef std::function<T (const T&, const T&, float)>	LerpFn;
			typedef std::function<void (T*)>						CompletionFn;
			//typedef boost::signals2::signal<void (T*)>				UpdateSignal;
			typedef std::function<void (T*)>						UpdateFn;

			// build a tween with a target, target value, duration, and optional ease function
			Tween( T *target, T endValue, float startTime, float duration,
					EaseFn easeFunction = Quadratic::easeInOut, LerpFn lerpFunction = &lerp<T> )
				: TweenBase( target, startTime, duration, easeFunction ), mStartValue( *target ), mEndValue( endValue ), mLerpFunction( lerpFunction )
			{
			}
			
			Tween( T *target, T startValue, T endValue, float startTime, float duration,
					EaseFn easeFunction = Quadratic::easeInOut, LerpFn lerpFunction = &lerp<T> )
				: TweenBase( target, startTime, duration, easeFunction ), mStartValue( startValue ), mEndValue( endValue ), mLerpFunction( lerpFunction )
			{
			}
			
			virtual ~Tween() {}
			
			void setUpdateFn( std::function<void (T*)> updateFn ) {
				mUpdateFunction = updateFn;
			}
						
			// this could be modified in the future to allow for a PathTween
			virtual void update( float relativeTime )
			{
				*reinterpret_cast<T*>(mTarget) = mLerpFunction( mStartValue, mEndValue, mEaseFunction( relativeTime ) );
				if( mUpdateFunction )
					mUpdateFunction( reinterpret_cast<T*>(mTarget) );
			}

			virtual void complete()
			{
				if( mCompletionFunction )
					mCompletionFunction( reinterpret_cast<T*>(mTarget) );
			}

			void			setCompletionFn( CompletionFn completionFunction ) { mCompletionFunction = completionFunction; }
			CompletionFn	getCompletionFn () const { return mCompletionFunction; }
			
			T	getStartValue() const { return mStartValue; }
			T	getEndValue() const { return mEndValue; }			
			T*	getTarget() const { return reinterpret_cast<T*>( mTarget ); }
			
		  protected:
			T	mStartValue, mEndValue;	
			
			LerpFn				mLerpFunction;
			CompletionFn		mCompletionFunction;
			UpdateFn			mUpdateFunction;
//			UpdateSignal		mUpdateSignal;
		};
		
		template<typename T = void*>
		class TweenRef : public std::shared_ptr<Tween<T> > {
		  public:
			TweenRef( const std::shared_ptr< Tween<T> > &sp )
				: std::shared_ptr<Tween<T> >( sp )
			{}
		};

		
	} //tween
} //cinder