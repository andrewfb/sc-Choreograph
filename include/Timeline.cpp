/*
 *  Timeline.cpp
 *  BasicTween
 *
 *  Created by David Wicks on 5/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#include "Timeline.h"

using namespace cinder;
using namespace cinder::tween;
typedef std::vector< TweenRef >::iterator t_iter;


Timeline::Timeline()
{
	mCurrentTime = 0;
}

void Timeline::stepTo( double time )
{	
	mCurrentTime = time;
	
	for( t_iter iter = mTweens.begin(); iter != mTweens.end(); ++iter )
	{
		(**iter).stepTo( time );
	}
}

void Timeline::clearTimeline()
{
	mTweens.clear();	
}

void Timeline::clearFinishedTweens()
{
	t_iter iter = mTweens.begin();
	
	while (iter != mTweens.end()) {		
		if( (**iter).isComplete() )
		{
			iter = mTweens.erase(iter);
		} else {
			++iter;
		}
	}
}

void Timeline::addTween( TweenRef tween)
{
	mTweens.push_back( tween );
}

TweenRef Timeline::findTween( void *target )
{
	t_iter iter = mTweens.begin();
	while( iter != mTweens.end() ) {
		if( (*iter)->getTargetVoid() == target )
			return *iter;
		++iter;
	}
	
	return TweenRef(); // failed returns null tween
}

void Timeline::removeTween( TweenRef tween )
{
	t_iter iter = std::find( mTweens.begin(), mTweens.end(), tween );
	if( iter != mTweens.end() )
		mTweens.erase( iter );
}


