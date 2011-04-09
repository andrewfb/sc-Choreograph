#include "NetflixRssParser.h"

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Xml.h"
#include "cinder/Thread.h"
#include "Timeline.h"

using namespace ci;
using namespace ci::app;
using namespace ci::tween;
using namespace std;

const int	WINDOW_WIDTH = 800;
const int	WINDOW_HEIGHT = 600;
const float	MOVIE_PADDING = 3;
const int	MOVIE_COLS = 4;
const int	MOVIE_ROWS = 25;
const int	TOTAL_MOVIES = 100;

class Movie {
  public:
	Movie( const NetflixMovie &info, Vec2f pos, Vec2f size )
		: mInfo( info ), mPos( pos ), mSize( size ), mAlpha( 1 )
	{}
  
	void draw() {
		gl::color( ColorA( 1, 1, 1, mAlpha ) );
		gl::drawSolidRect( Rectf( mPos, mPos + mSize ) );
	}
	
	ci::Vec2f		mPos, mSize;
	float			mAlpha;
	NetflixMovie	mInfo;
};

class NetflixRssApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	list<Movie>		mMovies;
	Timeline		mTimeline;
};

void NetflixRssApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
//	settings->setResizable( false );
}

void NetflixRssApp::setup()
{
	NetflixRssParser parser( Url( "http://rss.netflix.com/Top100RSS" ) );

	const Vec2f tileSize = Vec2f( WINDOW_WIDTH - MOVIE_PADDING, WINDOW_HEIGHT - MOVIE_PADDING ) / Vec2f( MOVIE_COLS, MOVIE_ROWS ) - Vec2f( MOVIE_PADDING, MOVIE_PADDING );
	const Vec2f tileOffset = tileSize + Vec2f( MOVIE_PADDING, MOVIE_PADDING );
	for( size_t m = 0; m < parser.getMovies().size(); ++m ) {
		int row = m / MOVIE_COLS;
		int col = m % MOVIE_COLS;
		Vec2f pos = Vec2f( MOVIE_PADDING, MOVIE_PADDING ) + Vec2f( col * tileOffset.x, row * tileOffset.y );
		mMovies.push_back( Movie( parser.getMovies()[m], pos, tileSize ) );
console() << parser.getMovies()[m] << std::endl;
		
		mMovies.back().mAlpha = 0;
		mTimeline.append( &mMovies.back().mAlpha, 0.0f, 1.0f, 2.2f )->delay( -2 );
	}
}

void NetflixRssApp::mouseDown( MouseEvent event )
{
}

void NetflixRssApp::update()
{
	mTimeline.stepTo( getElapsedSeconds() );
}

void NetflixRssApp::draw()
{
	// clear out the window with black
	gl::enableAlphaBlending();
	gl::clear( ColorA8u( 185, 9, 11, 255 ) );
	
	for( list<Movie>::iterator movIt = mMovies.begin(); movIt != mMovies.end(); ++movIt )
		movIt->draw();
}


CINDER_APP_BASIC( NetflixRssApp, RendererGl )
