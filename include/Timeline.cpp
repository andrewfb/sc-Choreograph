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
typedef std::vector<TimelineItemRef>::iterator s_iter;

Timeline::Timeline()
	: TimelineItem( 0, 0, 0 ), mDefaultAutoRemove( true ), mCurrentTime( 0 )
{
}

void Timeline::step( float timestep )
{
	mCurrentTime += timestep;
	stepTo( mCurrentTime );
}

void Timeline::stepTo( float absoluteTime )
{	
	mCurrentTime = absoluteTime;
	
	for( s_iter iter = mItems.begin(); iter != mItems.end(); )
	{
		(**iter).stepTo( mCurrentTime );
		if( (**iter).isComplete() && (**iter).isAutoRemove() )
			iter = mItems.erase( iter );
		else
			++iter;
	}
}

void Timeline::clearSequence()
{
	mItems.clear();	
}

void Timeline::clearCompletedTweens()
{
	s_iter iter = mItems.begin();
	
	while (iter != mItems.end()) {		
		if( (**iter).isComplete() )
		{
			iter = mItems.erase(iter);
		} else {
			++iter;
		}
	}
	
	calculateDuration();
}

void Timeline::replace( TimelineItemRef item )
{
	TimelineItemRef existingAction = find( item->getTarget() );
	if( existingAction )
		remove( existingAction );
	mItems.push_back( item );
	calculateDuration();
}


void Timeline::add( TimelineItemRef action )
{
	mItems.push_back( action );
	calculateDuration();
}

void Timeline::calculateDuration()
{
	mDuration = 0;
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter ) {
		mDuration = std::max( (*iter)->getEndTime(), mDuration );
	}	
}

TimelineItemRef Timeline::find( void *target )
{
	s_iter iter = mItems.begin();
	while( iter != mItems.end() ) {
		if( (*iter)->getTarget() == target )
			return *iter;
		++iter;
	}
	
	return TimelineItemRef(); // failed returns null tween
}

void Timeline::remove( TimelineItemRef action )
{
	s_iter iter = std::find( mItems.begin(), mItems.end(), action );
	if( iter != mItems.end() )
		mItems.erase( iter );
	calculateDuration();
}

void Timeline::reset( bool unsetStarted )
{
	TimelineItem::reset( unsetStarted );
	
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter )
		(*iter)->reset( unsetStarted );
}


void Timeline::loopStart()
{
	reset( false );
}

void Timeline::update( float absTime )
{
	absTime = loopTime( absTime );
	stepTo( absTime );
}
