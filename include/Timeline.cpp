/*
 *  Timeline.cpp
 *  BasicTween
 *
 *  Created by David Wicks on 5/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#include "Timeline.h"

#include <vector>

using namespace cinder;
using namespace cinder::tween;
typedef std::vector<TimelineItemRef>::iterator s_iter;
using namespace std;

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

void Timeline::appendPingPong()
{
	vector<TimelineItemRef> toAppend;
	
	float duration = mDuration;
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter ) {
		TimelineItemRef cloned = (*iter)->cloneReverse();
		cloned->mStartTime = duration + ( duration - ( cloned->mStartTime + cloned->mDuration ) );
		toAppend.push_back( cloned );
	}
	
	for( vector<TimelineItemRef>::const_iterator appIt = toAppend.begin(); appIt != toAppend.end(); ++appIt ) {
		mItems.push_back( *appIt );
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

void Timeline::reverse()
{
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter )
		(*iter)->reverse();
}

TimelineItemRef Timeline::cloneReverse() const
{
	Timeline *result = new Timeline( *this );
	for( s_iter iter = result->mItems.begin(); iter != result->mItems.end(); ++iter ) {
		(*iter)->reverse();
		(*iter)->mStartTime = mDuration + ( mDuration - ( (*iter)->mStartTime + (*iter)->mDuration ) );		
	}
	return TimelineItemRef( result );
}

void Timeline::update( float absTime )
{
	absTime = loopTime( absTime );
std::cout << "T: " << absTime << std::endl;
	stepTo( absTime );
}
