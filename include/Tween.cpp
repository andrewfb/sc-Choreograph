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

#include "Tween.h"

namespace cinder { namespace tween {

TweenBase::TweenBase( void *target, bool copyStartValue, float startTime, float duration, EaseFn easeFunction )
	: TimelineItem( 0, target, startTime, duration ), mCopyStartValue( copyStartValue ), mEaseFunction( easeFunction )
{
}

} } // namespace cinder::tween