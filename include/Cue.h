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

#pragma once

#include "TimelineItem.h"
#include "cinder/Function.h"

namespace cinder {

class Cue : public TimelineItem
{
  public:
	Cue( std::function<void ()> fn, float atTime = 0 );

	void					setFn( std::function<void ()> fn ) { mFunction = fn; }
	std::function<void ()>	getFn() const { return mFunction; }
	
  protected:
	virtual void				reverse() { /* no-op */ }
	virtual TimelineItemRef	cloneReverse() const;

	virtual void start() {} // starting is a no-op for Cues
	virtual void loopStart();
	virtual void update( float relativeTime ) {} // update is a no-op for Cues
	virtual void complete() {} // completion is a no-op for Cues
	virtual bool updateAtLoopStart() { return true; }
  
	std::function<void ()>		mFunction;
};

typedef std::shared_ptr<Cue>	CueRef;

} // namespace cinder