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
#include <boost/signals2.hpp>

namespace cinder
{

	namespace tween
	{
		class Cue : public Sequenceable
  		{
		  public:
			Cue( std::function<void ()> action, double atTime=0.0 );
			~Cue();
			void addReceiver( std::function<void ()> action );
			void update( double relativeTime );
			
 		  private:
			boost::signals2::signal<void ()> mSignal;
		};
		
		typedef boost::shared_ptr<Cue> CueRef;
	}
	
	
}