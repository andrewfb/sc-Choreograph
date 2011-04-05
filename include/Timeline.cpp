/*
 *  Timeline.cpp
 *  BasicTween
 *
 *  Created by David Wicks on 5/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#include <algorithm>
#include "Timeline.h"

using namespace std;

namespace cinder { namespace tween {

typedef std::vector<TweenRef>::iterator t_iter;
typedef std::vector<SequenceableRef>::iterator s_iter;

Timeline::Timeline()
{
	mCurrentTime = 0;
}

void Timeline::stepTo( double time )
{	
	mCurrentTime = time;
	
	// iterate tweens
	for( t_iter iter = mTweens.begin(); iter != mTweens.end(); ++iter ) {
		(**iter).stepTo( time );
	}
	
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter ) {
		(*iter)->stepTo( time );
	}
}

void Timeline::clearTimeline()
{
	mTweens.clear();
	mItems.clear();	
}

void Timeline::clearComplete()
{
	mTweens.erase( remove_if( mTweens.begin(), mTweens.end(), Sequenceable::isSeqComplete ), mTweens.end() );
	mItems.erase( remove_if( mItems.begin(), mItems.end(), Sequenceable::isSeqComplete ), mItems.end() );
}

void Timeline::restart()
{
	mCurrentTime = 0;
	for( t_iter iter = mTweens.begin(); iter != mTweens.end(); ++iter )
		(*iter)->restart();
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter )
		(*iter)->restart();	
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

} } // namespace cinder::tween
