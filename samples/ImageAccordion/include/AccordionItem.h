/*
 *  AccordionItem.h
 *  ImageAccordion
 *
 *  Created by Chris McKenzie on 4/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "cinder/Color.h";
#include "cinder/Rect.h";
#include "cinder/Vector.h"
#include <vector>

using namespace ci;

class AccordionItem {
public:
	
	AccordionItem();
	AccordionItem( float x, float y, float width, float height );
	void update();
	void draw();
	
	float mX, mY, mWidth, mHeight;
	
	Rectf mRect;
	Color mColor;
	
	bool isPointIn( const Vec2f &pt );
	
};


