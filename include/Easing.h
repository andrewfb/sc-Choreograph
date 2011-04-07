/*
 *  Easing.h
 *  BasicTween
 *
 *  Created by David Wicks on 5/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 *  Easing functions Derived from Robert Penner's work
 *  http://www.robertpenner.com/easing/
 *
 *  Ease functions take a start value, the final change in value, and the current normalized timestep
 *	They return the value of the animation curve at the provided point in time
 *	
 *	For each easing function fn,
 *	fn( 0.0 ) will return 0.0, and fn( 1.0 ) will return 1.0
 *	in between, they may return any value, including those outside [0,1.0]
 * 
 */

#pragma once
#include "cinder/CinderMath.h"

namespace cinder {

	namespace tween {
		
		
		struct Linear
		{
			static float noEase( float t )
			{
				return t;
			}
		};
		
		struct Quadratic
		{
			static float easeIn( float t )
			{
				return t * t;
			}
			static float easeOut( float t )
			{ 
				return - t * (t-2);
			}
			static float easeInOut( float t )
			{
				t *= 2;
				if (t < 1) return 0.5f * t * t;
				
				t -= 1;
				return -0.5f * ((t)*(t-2) -1);
			}
		};
		
		// Cubic
		struct Cubic
		{
			static float easeIn(float t)
			{
				return pow(t, 3.0);
			}
			static float easeOut(float t)
			{
				return pow(t - 1.0, 3.0) + 1.0;
			}
			static float easeInOut(float t)
			{
				t /= 0.5;
				if (t < 1.0)
					return 0.5 * pow(t, 3.0);
				return 0.5 * (pow(t - 2.0, 3.0) + 2.0); 	
			}
		};
		
		// Quartic
		struct Quart
		{
			static float easeIn(float t)
			{
				return pow(t, 4.0);
			}
			static float easeOut(float t)
			{
				return -pow(t - 1.0, 4.0) - 1.0;
			}
			static float easeInOut(float t)
			{
				t /= 0.5;
				if (t < 1.0)
					return 0.5 * pow(t, 4.0);
				t -= 2.0;
				return -0.5 * (t * pow(t, 3.0) - 2.0); 
			}
		};
		
		// Quintic
		struct Quint
		{
			static float easeIn(float t)
			{
				return pow(t, 5.0);
			}
			static float easeOut(float t)
			{
				return pow(t - 1.0, 5.0) + 1.0;
			}
			static float easeInOut(float t)
			{
				t /= 5.0;
				if (t < 1.0)
					return 0.5 * pow(t, 5.0);
				return 0.5 * (pow(t - 2.0, 5.0) + 2.0); 
			}
		};
		
		// Sinusoidal
		struct Sine
		{
			static float easeIn(float t)
			{
				return -cos(t * (M_PI / 2.0)) + 1.0;
			}
			static float easeOut(float t)
			{
				return sin(t * (M_PI / 2.0));
			}
			static float easeInOut(float t)
			{
				return -0.5 * (cos(M_PI * t) - 1.0); 
			}
		};
		
		// Exponential
		struct Expo
		{
			static float easeIn(float t)
			{
				return t == 0.0 ? 0.0 : pow(2.0, 10.0 * (t - 1.0));
			}
			static float easeOut(float t)
			{
				return t == 1.0 ? 1.0 : -pow(2.0, -10.0 * t) + 1.0;
			}
			static float easeInOut(float t)
			{
				if (t == 0.0 || t == 1.0) 
					return t;
				t /= 0.5;
				if (t < 1.0) 
					return 0.5 * pow(2.0, 10.0 * (t - 1.0));
				--t;
				return 0.5 * (-pow(2.0, -10.0 * t) + 2.0);
			}
		};
		
		// Circular
		struct Circ
		{
			static float easeIn(float t)
			{
				return -(sqrt(1.0 - pow(t, 2.0)) - 1);
			}
			static float easeOut(float t)
			{
				return sqrt(1.0 - pow(t - 1.0, 2.0));
			}
			static float easeInOut(float t)
			{
				t /= 0.5;
				if (t < 1.0)
					return -0.5 * (sqrt(1.0 - pow(t, 2.0)) - 1.0);
				t -= 2.0;
				return 0.5 * (sqrt(1.0 - pow(t, 2.0)) + 1.0);	
			}
		};
		
		// Bounce
		struct Bounce
		{
			static float easeIn(float t)
			{
				return 1.0;
			}
			static float easeOut(float t)
			{
				if (t < 1.0 / 2.75)
				{
					return (7.5625 * t * t);
				}
				else if (t < 2.0 / 2.75)
				{
					t -= 1.5 / 2.75;
					return 7.5625 * t * t + 0.75;
				} 
				else if (t < 2.5 / 2.75)
				{
					t -= 2.25 / 2.75;
					return 7.5625 * t * t + 0.9375;
				} 
				else 
				{
					t -= 2.625 / 2.75;
					return 7.5625 * t *t + 0.984375;
				}
			}
			static float easeInOut(float t)
			{
				return 1.0;
			}
		};
		
		struct Back
		{			
			static float easeIn( float t )
			{
				float ks = 1.70158;
				return t * t * ((ks+1)*t - ks);
			}
			static float easeOut( float t )
			{ 
				float ks = 1.70158;
				t -= 1;
				return (t*t*((ks+1)*t + ks) + 1);
			}
			static float easeInOut( float t )
			{
				float ks = 1.70158;
				t *= 2;
				float s = ks * 1.525;
				
				if (t < 1) return 0.5*(t*t*(((s)+1)*t - s));
				return 0.5*((t-=2)*t*(((s)+1)*t + s) + 2);
				
			}
		};
		
		// Elastic
		struct Elastic
		{
			static float easeIn(float t)
			{
				return 1.0;
			}
			static float easeOut(float t)
			{
				return 1.0;
			}
			static float easeInOut(float t)
			{
				return 1.0;
			}
		};
		
	}
}