#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"

#include "Choreograph.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::tween;

// Simple class to demonstrate custom lerping
struct Box {
	Box() : mColor( Color( 1, 0.5f, 0.25f ) ), mPos( 320, 240 ), mSize( 10, 10 ) {}
	Box( Color color, Vec2f pos, Vec2f size )
		: mColor( color ), mPos( pos ), mSize( size )
	{}
	
	void	draw() {
		gl::color( mColor );
		gl::drawSolidRect( Rectf( mPos, mPos + mSize ) );
	}
	
	Color	mColor;
	Vec2f	mPos, mSize;
};

Box boxLerp( const Box &start, const Box &end, float t )
{
	return Box( lerp( start.mColor, end.mColor, t ), lerp( start.mPos, end.mPos, t), lerp( start.mSize, end.mSize, t ) );
}


class BasicTweenApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	
	void setup();
	void update();
	void draw();
	
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );
	
	void playRandomTween();
	void tweenToMouse();
	
	void mouseDown( MouseEvent event );
	
	void resize( int width, int height );
	
	void respond();
	
  private:	
	float mX, mY;
	float mStep;
	Vec2f mPos;
	ColorA mColor;
	Sequence mSequence;
	Box		mBox;
};

void BasicTweenApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(600,400);
	settings->setTitle("BasicTween");
}

void BasicTweenApp::setup()
{	
	mX = getWindowWidth()/2;
	mY = getWindowHeight()/2;
	mPos = Vec2f(0,0);
	
	mStep = 1.0 / 60.0;
	
	mColor = ColorA( 0.5, 0.55, 0.52, 1.0 );
	playRandomTween();
	
//	mSequence.add( boost::bind( &BasicTweenApp::respond, this ), 2.0 );
	
}

void BasicTweenApp::update()
{
	// step our animation forward
	mSequence.step( mStep );
	// step() also works, it uses 1.0/app::getFrameRate()
//	TweenManager::instance().step();
}

void BasicTweenApp::draw()
{
	gl::clear(mColor);
	
	gl::color(Color::black());
	gl::drawSolidCircle( Vec2i( mX, mY ), 20.0f );
	
	gl::color(Color::white());
	gl::drawSolidCircle( mPos, 15.0f );

	mBox.draw();
}

void BasicTweenApp::respond()
{
	console() << "Tween completed." << endl;
}

void BasicTweenApp::resize(int width, int height)
{
	
}

//MouseEvents
void BasicTweenApp::mouseDown( MouseEvent event )
{
	tweenToMouse();
}

void BasicTweenApp::playRandomTween()
{
	// Reset the timeline to zero
	mSequence.reset();
	
	// Tween a Vec2f
	Vec2f randomPos( Rand::randFloat(getWindowWidth()), Rand::randFloat(getWindowHeight()) );
	
	// Create our tween
	mSequence.add( &mPos, randomPos, 2.0 );
	
	// Tween our floats
	randomPos = Vec2f( Rand::randFloat(getWindowWidth()), Rand::randFloat(getWindowHeight()) );
	mSequence.add( &mX, randomPos.x, 2.0 );
	mSequence.add( &mY, randomPos.y, 2.0 );
}

Box randomBox()
{
	return Box( Color( Rand::randFloat(), Rand::randFloat(), Rand::randFloat() ), Vec2f( Rand::randFloat( getWindowWidth() ), Rand::randFloat( getWindowHeight() ) ),
		Vec2f( Rand::randFloat( 40 ), Rand::randFloat( 40 ) ) );
}

void printBox( Box *box )
{
	console() << box->mPos;
}

void boxDone( Box *box )
{
	console() << std::endl << "Box is done." << std::endl;
}

void BasicTweenApp::tweenToMouse()
{
console() << "entering: " << mSequence.getNumTweens() << "tweens" << std::endl;
	Vec2f mousePos = getMousePos();
	TweenRef<Vec2f> posTween = mSequence.replace( &mPos, mousePos, 1.25, Back::easeOut );
	posTween->delay( 0.5f );

	// Tween our floats
	mSequence.replace( &mX, mousePos.x, 2.0, Quadratic::easeInOut );
	mSequence.replace( &mY, mousePos.y, 1.5, Quadratic::easeInOut );
	
	// make a new random box and tween to that
	TweenRef<Box> boxTween = mSequence.replace( &mBox, randomBox(), 3.0, Quadratic::easeInOut, boxLerp );
	boxTween->setUpdateFn( printBox );
	boxTween->setCompletionFn( boxDone );
	boxTween->setAutoRemove();
	
console() << mSequence.getNumTweens() << "tweens" << std::endl;
}

void cb( float *f )
{
}

void benchmark()
{
	Timer tm;
	Sequence tln;
	const int totalFloats = 200;
	float tempFloat[totalFloats];
	for( size_t f = 0; f < totalFloats; ++f ) {
		tempFloat[f] = 0;
		TweenRef<float> twn = tln.add( &tempFloat[f], 10.0f, 1 );
		twn->setUpdateFn( cb );
	}
	tm.start();
		for( size_t t = 0; t < 1000000; ++t ) {
			tln.step( 1 / (float)1000000 );
		}
	tm.stop();
	console() << "Total time: " << tm.getSeconds() << std::endl;
}

//KeyEvents
void BasicTweenApp::keyDown( KeyEvent event )
{
	switch( event.getChar() ){
		case 'r':
			mSequence.reset();
		break;
		case 't':
			mStep *= -1;
		break;
		case 'b':
			benchmark();
		break;
		default:
			playRandomTween();
		break;
	}
}

void BasicTweenApp::keyUp( KeyEvent event )
{
	switch( event.getChar() ){
		default:
		break;
	}
}


// This line tells Cinder to actually create the application
CINDER_APP_BASIC( BasicTweenApp, RendererGl )
