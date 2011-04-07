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
typedef std::vector< SeqRef >::iterator s_iter;

Timeline::Timeline()
{
	mCurrentTime = 0;
}

void Timeline::step( float timestep )
{
	mCurrentTime += timestep;
	stepTo( mCurrentTime );
}

void Timeline::stepTo( float absoluteTime )
{	
	mCurrentTime = absoluteTime;
	
	for( s_iter iter = mActions.begin(); iter != mActions.end(); )
	{
		(**iter).stepTo( mCurrentTime );
		if( (**iter).isComplete() && (**iter).isAutoRemove() )
			iter = mActions.erase( iter );
		else
			++iter;
	}
}

void Timeline::clearSequence()
{
	mActions.clear();	
}

void Timeline::clearCompletedTweens()
{
	s_iter iter = mActions.begin();
	
	while (iter != mActions.end()) {		
		if( (**iter).isComplete() )
		{
			iter = mActions.erase(iter);
		} else {
			++iter;
		}
	}
	
	calculateDuration();
}

void Timeline::replace( SeqRef item )
{
	SeqRef existingAction = find( item->getTarget() );
	if( existingAction )
		remove( existingAction );
	mActions.push_back( item );
	calculateDuration();
}


void Timeline::add( SeqRef action )
{
	mActions.push_back( action );
	calculateDuration();
}

void Timeline::calculateDuration()
{
	mDuration = 0;
	for( s_iter iter = mActions.begin(); iter != mActions.end(); ++iter ) {
		mDuration = std::max( (*iter)->getEndTime(), mDuration );
	}	
}

SeqRef Timeline::find( void *target )
{
	s_iter iter = mActions.begin();
	while( iter != mActions.end() ) {
		if( (*iter)->getTarget() == target )
			return *iter;
		++iter;
	}
	
	return SeqRef(); // failed returns null tween
}

void Timeline::remove( SeqRef action )
{
	s_iter iter = std::find( mActions.begin(), mActions.end(), action );
	if( iter != mActions.end() )
		mActions.erase( iter );
	calculateDuration();
}


