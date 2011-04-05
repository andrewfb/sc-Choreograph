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
#include "Easing.h"
#include "TimeBasis.h"
#include "Sequenceable.hpp"

namespace cinder {
	namespace tween {
		
		//Non-templated base class to allow us to have a list containing all types of Tween
		class Tweenable : public Sequenceable {
		public:
			Tweenable( void *data ) 
				: mTargetVoid( data ), mComplete( false ), mNumRepeats( 0 ), mPingPong( false ), mLoop( false )
			{}
			virtual ~Tweenable(){};
			
			//! change how the tween moves through time
			void setEaseFunction( double (*easeFunction)(double t) ) { mEaseFunction = easeFunction; }
			
			//! change how the tween thinks about time
//			void reverse() { setTimeFunction(TimeBasis::reverse); }
			void setLoop( bool loop = true ) { mLoop = loop; }
			void setPingPong( bool pingPong = true ) { mPingPong = pingPong; }
			
			virtual void	restart() { mComplete = false; }
			
			//! push back the tween's start time
			void delay( double amt ){ mStartTime += amt; }
			//! set the tween's start time
			void setStartTime( double time ){ mStartTime = time; }
			double getStartTime() const { return mStartTime; }
			bool isComplete() const { return mComplete; }
			
			virtual double getDuration() const { return mDuration; }
			//! change the duration of the tween
			void setDuration( double duration ){ mDuration = duration; }
			virtual double getCurrentTime() const { return mStartTime + mT * mDuration; }
			
			const void	*getTargetVoid() const { return mTargetVoid; }
			
		protected:
			double	mDuration;
			double	mStartTime;
			void	*mTargetVoid;
			double	mT;	// normalized time
			bool	mComplete;
			int		mNumRepeats;
			bool	mPingPong, mLoop;
			// how we move between points in time
			double (*mEaseFunction)(double t);
		};
		
		typedef std::shared_ptr<Tweenable> TweenRef;
		
		//Our templated tween design
		template<typename T>
		class Tween : public Tweenable{
		  public:
			// build a tween with a target, target value, duration, and optional ease function
			Tween<T>( T *target, T targetValue, double startTime, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut )
				: Tweenable( target )
			{
				mTarget = target;
				mStartValue = *target;
				mEndValue = targetValue;
				mValueDelta = mEndValue - mStartValue;
				
				mStartTime = startTime;
				mDuration = duration;
				mT = 0.0;
				mEaseFunction = easeFunction;
			}
			
			Tween<T>( T *target, T startValue, T targetValue, double startTime, double duration, double (*easeFunction)(double t)=Quadratic::easeInOut )
				: Tweenable( target )
			{
				mTarget = target;
				mStartValue = startValue;
				mEndValue = targetValue;
				mValueDelta = mEndValue - mStartValue;
				
				mStartTime = startTime;
				mDuration = duration;
				mT = 0.0;
				mEaseFunction = easeFunction;
			}
			
			~Tween<T>() {}
			
			// this could be modified in the future to allow for a PathTween
			virtual void stepTo( double newTime )
			{
				if( mComplete )
					return;

				double delta = newTime - mStartTime;
				if( delta <= 0 )
					return;
				else if( delta < mDuration ) {
					mT = delta / mDuration;
					*mTarget = mStartValue + mValueDelta * mEaseFunction( mT );
				}
				else { // mT >= 1
					if( mLoop ) {
						mT = math<double>::fmod( delta, mDuration ) / mDuration;
					}
					else if( mPingPong ) {
						double t2 = fmod( delta, mDuration * 2 );
						if( t2 > mDuration )
							mT = ((mDuration*2) - t2) / mDuration;
						else
							mT = t2 / mDuration;
					}
					else {
						mT = 1; // all done
						mComplete = true;
					}
					*mTarget = mStartValue + mValueDelta * mEaseFunction( mT );
				}
//				updateTarget();	
			}
			
			virtual void updateTarget()
			{
				if( ! mComplete || mT < 1.0 )
				{
					if( mT > 0.0 && mT < 1.0 )
					{
						*mTarget = mStartValue + mValueDelta * mEaseFunction( mT );
						mComplete = false;
					} else if ( mT == 1.0 )
					{	// at the completion point, set to target value
						*mTarget = mEndValue;
						mComplete = true;
					} else
					{
						
					}
				}
			}
			
			T* getTarget(){ return mTarget; }
			
		private:			
			T* mTarget;
			T mStartValue, mValueDelta, mEndValue;			
		};

		/*
		 * Debating whether to add some predefined tween types
		 *
		typedef Tween<float> Tweenf;
		typedef Tween<Vec2f> Tween2f; 
		//*/
	} //tween
} //cinder