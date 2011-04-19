/*
 Copyright (c) 2011, The Cinder Project, All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Based on the sc-Choreograph CinderBlock by David Wicks: http://sansumbrella.com/

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "Timeline.h"

#include <vector>

using namespace cinder;
using namespace std;

typedef std::vector<TimelineItemRef>::iterator s_iter;

Timeline::Timeline()
	: TimelineItem( 0, 0, 0, 0 ), mDefaultAutoRemove( true ), mCurrentTime( 0 )
{
	mUseAbsoluteTime = true;
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
		if( (**iter).isComplete() && (**iter).getAutoRemove() )
			iter = mItems.erase( iter );
		else
			++iter;
	}
}

void Timeline::clear()
{
	mItems.clear();	
}

void Timeline::clearCompleted()
{
	s_iter iter = mItems.begin();	
	while (iter != mItems.end()) {		
		if( (**iter).isComplete() )
			iter = mItems.erase(iter);
		else
			++iter;
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

void Timeline::apply( TimelineItemRef item )
{
	TimelineItemRef existingAction = find( item->getTarget() );
	if( existingAction )
		remove( existingAction );
	insert( item );
}


void Timeline::add( TimelineItemRef item )
{
	item->mParent = this;
	item->mStartTime = mCurrentTime;
	mItems.push_back( item );
	calculateDuration();
}

void Timeline::insert( TimelineItemRef item )
{
	item->mParent = this;
	mItems.push_back( item );
	calculateDuration();
}

void Timeline::calculateDuration()
{
	float duration = 0;
	for( s_iter iter = mItems.begin(); iter != mItems.end(); ++iter ) {
		duration = std::max( (*iter)->getEndTime(), duration );
	}
	
	setDuration( duration );
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
	stepTo( absTime );
}

void Timeline::timeChanged( TimelineItem *item )
{
	calculateDuration();
}