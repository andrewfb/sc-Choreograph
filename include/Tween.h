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

#include "TimelineItem.h"

#include "cinder/Cinder.h"
#include "cinder/CinderMath.h"
#include "cinder/Easing.h"
#include "cinder/Function.h"

namespace cinder {
	namespace tween {
		template<typename T>
		class Tween;
		typedef std::function<float (float)> EaseFn;
		
		template<typename T>
		T tweenLerp( const T &start, const T &end, float time )
		{
			return start * ( 1 - time ) + end * time;
		}
		
		//Our templated tween design
		class TweenBase : public TimelineItem {
		  public:
			TweenBase( void *target, bool copyStartValue, float startTime, float duration, EaseFn easeFunction = easeNone );
			virtual ~TweenBase() {}

			//! change how the tween moves through time
			void	setEaseFn( EaseFn easeFunction ) { mEaseFunction = easeFunction; }
			EaseFn	getEaseFn() const { return mEaseFunction; }
			
		  protected:
			EaseFn		mEaseFunction;
			float		mDuration;
			bool		mCopyStartValue;
		};

		template<typename T>
		class TweenRef : public std::shared_ptr<Tween<T> > {
		  public:
			TweenRef( const std::shared_ptr<Tween<T> > &sp )
				: std::shared_ptr<Tween<T> >( sp )
			{}
			TweenRef( Tween<T> *tween )
				: std::shared_ptr<Tween<T> >( tween )
			{}
		};
				
		template<typename T>
		class Tween : public TweenBase {
		  public:
			typedef std::function<T (const T&, const T&, float)>	LerpFn;
			typedef std::function<void (T*)>						StartFn;
			typedef std::function<void (T*)>						CompletionFn;
			typedef std::function<void (T*)>						UpdateFn;

			// build a tween with a target, target value, duration, and optional ease function
			Tween( T *target, T endValue, float startTime, float duration,
					EaseFn easeFunction = easeNone, LerpFn lerpFunction = &tweenLerp<T> )
				: TweenBase( target, true, startTime, duration, easeFunction ), mStartValue( *target ), mEndValue( endValue ), mLerpFunction( lerpFunction )
			{
			}
			
			Tween( T *target, T startValue, T endValue, float startTime, float duration,
					EaseFn easeFunction = easeNone, LerpFn lerpFunction = &tweenLerp<T> )
				: TweenBase( target, false, startTime, duration, easeFunction ), mStartValue( startValue ), mEndValue( endValue ), mLerpFunction( lerpFunction )
			{
			}
			
			virtual ~Tween() {}
			
			void			setStartFn( StartFn startFunction ) { mStartFunction = startFunction; }
			StartFn			getStartFn() const { return mStartFunction; }
			TweenRef<T>		startFn( StartFn startFunction ) { mStartFunction = startFunction; return getThisRef(); }
			
			void			setUpdateFn( UpdateFn updateFunction ) { mUpdateFunction = updateFunction; }									
			UpdateFn		getUpdateFn() const { return mUpdateFunction; }
			TweenRef<T>		updateFn( UpdateFn updateFunction ) { mUpdateFunction = updateFunction; return getThisRef(); }
																																							
			void			setCompletionFn( CompletionFn completionFunction ) { mCompletionFunction = completionFunction; }
			CompletionFn	getCompletionFn() const { return mCompletionFunction; }
			TweenRef<T>		completionFn( CompletionFn completionFunction ) { mCompletionFunction = completionFn; return getThisRef(); }
			
			//! Returns the starting value for the tween. If the tween will copy its target's value upon starting (isCopyStartValue()) and the tween has not started, this returns the value of its target when the tween was created
			T	getStartValue() const { return mStartValue; }
			T	getEndValue() const { return mEndValue; }			
			T*	getTarget() const { return reinterpret_cast<T*>( mTarget ); }
			
			//! Returns whether the tween will copy its target's value upon starting
			bool	isCopyStartValue() { return mCopyStartValue; }

			// these override their equivalents in TimelineItem so that we can return a TweenRef<T> instead of TimelineItemRef
			//! Pushes back the tween's start time by \a amt. Returns a reference to \a this
			TweenRef<T> startTime( float newTime ) { setStartTime( newTime ); return getThisRef(); }
			//! Pushes back the tween's start time by \a amt. Returns a reference to \a this
			TweenRef<T> delay( float amt ) { setStartTime( mStartTime + amt ); return getThisRef(); }
			//! Sets the tween's duration to \a newDuration. Returns a reference to \a this
			TweenRef<T> duration( float newDuration ) { setDuration( newDuration ); return getThisRef(); }			
			//! Sets whether the item will remove itself from the Timeline when it is complete
			TweenRef<T> autoRemove( bool autoRmv = true ) { setAutoRemove( autoRmv ); return getThisRef(); }
			//! Sets whether the item starts over when it is complete
			TimelineItemRef loop( bool doLoop = true ) { setLoop( doLoop ); return getThisRef(); }
			
			//! Returns a TweenRef<T> to \a this
			TweenRef<T> getThisRef(){ return TweenRef<T>( std::static_pointer_cast<Tween<T> >( shared_from_this() ) ); }

		  protected:
			virtual void reverse()
			{
				std::swap( mStartValue, mEndValue );
			}
			
			virtual TimelineItemRef	cloneReverse() const
			{
				std::shared_ptr<Tween<T> > result( new Tween<T>( *this ) );
				std::swap( result->mStartValue, result->mEndValue );
				result->mCopyStartValue = false;
				return result;
			}
			
			virtual void reset( bool unsetStarted )
			{
				TimelineItem::reset( unsetStarted );
			}
			
			virtual void start()
			{
				if( mCopyStartValue )
					mStartValue = *(reinterpret_cast<T*>( mTarget ) );
				if( mStartFunction )
					mStartFunction( reinterpret_cast<T*>(mTarget) );
			}
			
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

			T	mStartValue, mEndValue;	
			
			StartFn				mStartFunction;
			LerpFn				mLerpFunction;
			CompletionFn		mCompletionFunction;
			UpdateFn			mUpdateFunction;
		};
		
		typedef std::shared_ptr<TweenBase>	TweenBaseRef;
		
	} //tween
} //cinder