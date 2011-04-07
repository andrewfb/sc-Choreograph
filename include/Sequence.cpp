/*
 *  Sequence.cpp
 *  BasicTween
 *
 *  Created by David Wicks on 5/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#include "Sequence.h"

using namespace cinder;
using namespace cinder::tween;
typedef std::vector< SeqRef >::iterator s_iter;

Sequence::Sequence()
{
	mCurrentTime = 0;
}

void Sequence::step( float timestep )
{
	mCurrentTime += timestep;
	stepTo( mCurrentTime );
}

void Sequence::stepTo( float absoluteTime )
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

void Sequence::clearSequence()
{
	mActions.clear();	
}

void Sequence::clearCompletedTweens()
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

void Sequence::replace( SeqRef item )
{
	SeqRef existingAction = find( item->getTarget() );
	if( existingAction )
		remove( existingAction );
	mActions.push_back( item );
	calculateDuration();
}


void Sequence::add( SeqRef action )
{
	mActions.push_back( action );
	calculateDuration();
}

void Sequence::calculateDuration()
{
	mDuration = 0;
	for( s_iter iter = mActions.begin(); iter != mActions.end(); ++iter ) {
		mDuration = std::max( (*iter)->getEndTime(), mDuration );
	}	
}

SeqRef Sequence::find( void *target )
{
	s_iter iter = mActions.begin();
	while( iter != mActions.end() ) {
		if( (*iter)->getTarget() == target )
			return *iter;
		++iter;
	}
	
	return SeqRef(); // failed returns null tween
}

void Sequence::remove( SeqRef action )
{
	s_iter iter = std::find( mActions.begin(), mActions.end(), action );
	if( iter != mActions.end() )
		mActions.erase( iter );
	calculateDuration();
}


