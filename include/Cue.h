/*
 *  Cue.h
 *  BasicTween
 *
 *  Created by David Wicks on 2/24/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#pragma once

#include "Sequenceable.hpp"
#include "cinder/Function.h"

namespace cinder
{

	namespace tween
	{
		class Cue : public Sequenceable
  		{
		  public:
			Cue( std::function<void ()> action, float atTime=0.0 );
			~Cue();
			void addReceiver( std::function<void ()> action );
			virtual void update( float relativeTime );
			
			virtual void complete() {} // completion is a no-op for Cues
			
 		  private:
//			boost::signals2::signal<void ()> mSignal;
		};
		
		typedef boost::shared_ptr<Cue> CueRef;
	}
	
	
}