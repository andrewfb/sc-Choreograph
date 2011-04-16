/*
 *  AccordionItem.cpp
 *  ImageAccordion
 *
 *  Created by Chris McKenzie on 4/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "AccordionItem.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

AccordionItem::AccordionItem() {}

AccordionItem::AccordionItem( float x, float y, float width, float height ) {
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;
	mRect = Rectf(x, y, x + width, y + height);
	mColor = Color(Rand::randFloat(1), Rand::randFloat(1), Rand::randFloat(1));
	
}

bool AccordionItem::isPointIn( const Vec2f &pt ) {
	return mRect.contains( pt );
}

void AccordionItem::update() {	
	mRect = Rectf(mX, mY, mX + mWidth, mY + mHeight);
}

void AccordionItem::draw() {
	gl::color(mColor);
	gl::drawSolidRect(mRect);
}



