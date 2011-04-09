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
const Vec2f EXPANDED_SIZE( 250, 100 );

class Movie {
  public:
	Movie( const NetflixMovie &info, Vec2f pos, Vec2f size )
		: mInfo( info ), mPos( pos ), mSize( size ), mAlpha( 1 )
	{}
  
	void draw() {
		gl::color( ColorA( 1, 1, 1, mAlpha ) );
		gl::drawSolidRect( Rectf( mPos, mPos + mSize ) );
		gl::color( ColorA( 0.7f, 0.7f, 0.7f, mAlpha ) );
		Rectf inset( Rectf( mPos, mPos + mSize ) );
		gl::drawStrokedRect( Rectf( inset.getUpperLeft() + Vec2f(2,2), inset.getLowerRight() - Vec2f(2,2)) );
	}

	bool isPointIn( const Vec2f &pt ) { return Rectf( mPos, mPos + mSize ).contains( pt ); }
	
	ci::Vec2f		mPos, mSize;
	float			mAlpha;
	NetflixMovie	mInfo;
	
	static Vec2f	sNormalSize, sExpandedSize;
};
Vec2f Movie::sNormalSize, Movie::sExpandedSize;

class NetflixRssApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseMove( MouseEvent event );	
	void mouseDown( MouseEvent event );
	void update();
	void draw();

	// never use a vector with a timeline
	list<Movie>				mMovies;
	list<Movie>::iterator	mCurrentSelection;
	Timeline				mTimeline;
};

void NetflixRssApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
//	settings->setResizable( false );
}

void NetflixRssApp::setup()
{
	NetflixRssParser parser( Url( "http://rss.netflix.com/Top100RSS" ) );

	TimelineRef initialFadeTln = Timeline::create();
	Movie::sNormalSize = Vec2f( WINDOW_WIDTH - MOVIE_PADDING, WINDOW_HEIGHT - MOVIE_PADDING ) / Vec2f( MOVIE_COLS, MOVIE_ROWS ) - Vec2f( MOVIE_PADDING, MOVIE_PADDING );
	Movie::sExpandedSize = Vec2f( 250, 100 );
	const Vec2f tileOffset = Movie::sNormalSize + Vec2f( MOVIE_PADDING, MOVIE_PADDING );
	for( size_t m = 0; m < parser.getMovies().size(); ++m ) {
		int row = m / MOVIE_COLS;
		int col = m % MOVIE_COLS;
		Vec2f pos = Vec2f( MOVIE_PADDING, MOVIE_PADDING ) + Vec2f( col * tileOffset.x, row * tileOffset.y );
		mMovies.push_back( Movie( parser.getMovies()[m], pos, Movie::sNormalSize ) );
console() << parser.getMovies()[m] << std::endl;
		
		mMovies.back().mAlpha = 0;
		initialFadeTln->append( &mMovies.back().mAlpha, 0.0f, 1.0f, 0.5f )->delay( -0.49f );
	}

	mTimeline.stepTo( getElapsedSeconds() );
	mTimeline.add( initialFadeTln );
	
	mCurrentSelection = mMovies.end();
}

void NetflixRssApp::mouseMove( MouseEvent event )
{
	// if the mouse is still in the current selection, there's nothing to see here
	if( mCurrentSelection != mMovies.end() && mCurrentSelection->isPointIn( event.getPos() ) )
		return;
	
	// mouse has left the current selection
	if( mCurrentSelection != mMovies.end() ) {
		mTimeline.replace( &mCurrentSelection->mSize.x, Movie::sNormalSize.x, 0.08f );
		mTimeline.replace( &mCurrentSelection->mSize.y, Movie::sNormalSize.y, 0.08f );
	}
	
	// find the new selection
	mCurrentSelection = mMovies.end();
	for( list<Movie>::iterator movIt = mMovies.begin(); movIt != mMovies.end(); ++movIt ) {
		if( movIt->isPointIn( event.getPos() ) ) {
			mCurrentSelection = movIt;
			mTimeline.add( &mCurrentSelection->mSize.x, Movie::sExpandedSize.x, 0.2f );
			mTimeline.add( &mCurrentSelection->mSize.y, Movie::sExpandedSize.y, 0.2f )->delay( 0.1f );
			break;
		}
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
	
	for( list<Movie>::iterator movIt = mMovies.begin(); movIt != mMovies.end(); ++movIt ) {
		if( movIt != mCurrentSelection )
			movIt->draw();
	}
	
	// always draw the current selection on top
	if( mCurrentSelection != mMovies.end() )
		mCurrentSelection->draw();
}


CINDER_APP_BASIC( NetflixRssApp, RendererGl )
