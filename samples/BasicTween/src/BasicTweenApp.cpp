#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"

#include "Choreograph.h"

using namespace std;
using namespace ci;
using namespace ci::app;

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
	
	void cueExample();
	
  private:	
	float mX, mY;
	float mStep;
	Vec2f mPos;
	ColorA mColor;
	Timeline mSequence;
	Box		mBox;
	
	// subtimeline
	TimelineRef		mSubtimeline;
	Box				mSubBoxes[3];
};

void BasicTweenApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(600,400);
	settings->setTitle("BasicTween");
}

void printFloat( float *v )
{
std::cout << "v: " << *v;
}

void BasicTweenApp::setup()
{	
	mX = getWindowWidth()/2;
	mY = getWindowHeight()/2;
	mPos = Vec2f(0,0);
	
	mStep = 1.0 / 60.0;
	
	mColor = ColorA( 0.5, 0.55, 0.52, 1.0 );
	playRandomTween();
	
/*	TimelineItemRef cue = mSequence.add( std::bind( &BasicTweenApp::cueExample, this ), 2.0 );
	cue->setDuration( 1 );
	cue->setLoop();*/
	
	// create subtimeline
	mSubBoxes[0] = Box( Color( 0.2, 0.8, 0 ), Vec2f( 50, getWindowHeight() - 50 ), Vec2f( 30, 30 ) );
	mSubBoxes[1] = Box( Color( 0.3, 0.7, 0.15 ), Vec2f( 150, getWindowHeight() - 60 ), Vec2f( 30, 30 ) );
	mSubBoxes[2] = Box( Color( 0.4, 0.6, 0.3 ), Vec2f( 250, getWindowHeight() - 70 ), Vec2f( 30, 30 ) );	
	
	mSubtimeline = Timeline::create();
	mSubtimeline->setDefaultAutoRemove( false );
	mSubtimeline->append<float>( &mSubBoxes[0].mPos.y, 50.0f, 1 );
TweenRef<float> temp = mSubtimeline->append<float>( &mSubBoxes[1].mPos.y, 50.0f, 1 )->delay( 0.5f );
	mSubtimeline->append<float>( &mSubBoxes[2].mPos.y, 50.0f, 1 );
	mSubtimeline->appendPingPong();
	mSubtimeline->setLoop();
	
	mSequence.add( mSubtimeline );
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
	
	for( int i = 0; i < 3; ++i )
		mSubBoxes[i].draw();
}

void BasicTweenApp::cueExample()
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

void boxStart()
{
	console() << std::endl << "Box is starting." << std::endl;
}

void boxPrint( Box *box )
{
	console() << "Box is at" << box->mPos << std::endl;
}

void boxDone()
{
	console() << "Box is done." << std::endl;
}

void BasicTweenApp::tweenToMouse()
{
console() << "entering: " << mSequence.getNumItems() << "tweens" << std::endl;
	Vec2f mousePos = getMousePos();
	TweenRef<Vec2f> posTween = mSequence.apply( &mPos, mousePos, 1.25, EaseOutBack() );
	posTween->delay( 0.5f );

	// Tween our floats
	mSequence.apply( &mX, mousePos.x, 2.0f, EaseInOutQuad() );
	mSequence.apply( &mY, mousePos.y, 1.5f, EaseInOutQuad() );
	
	// make a new random box and tween to that
	Box newBox = randomBox();
	newBox.mPos = mousePos;
	TweenRef<Box> boxTween = mSequence.appendTarget( &mBox, newBox, 3.0, EaseNone(), boxLerp );
	boxTween->setStartFn( boxStart );
	boxTween->setUpdateFn( std::bind( boxPrint, &mBox ) );
	boxTween->setCompletionFn( boxDone );
//	boxTween->setAutoRemove();
//	boxTween->setPingPong();
	
console() << mSequence.getNumItems() << "tweens lasting " << mSequence.getDuration() << std::endl;
}

void cb( float *f )
{
}

void benchmark()
{
	Timer tm;
	Timeline tln;
	const int totalFloats = 10000;
	float tempFloat[totalFloats];
	for( size_t f = 0; f < totalFloats; ++f ) {
		tempFloat[f] = 0;
		TweenRef<float> twn = tln.add( &tempFloat[f], 10.0f, 1 );
	}
	tm.start();
		for( size_t t = 0; t < 60000; ++t ) {
			tln.step( 1 / (float)60000 );
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
