#include "NetflixRssParser.h"
#include "Resources.h"

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Xml.h"
#include "cinder/Text.h"
#include "cinder/ImageIo.h"
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

gl::TextureFontRef  sTitleFont;

Vec2f floor( Vec2f v )
{
	return Vec2f( math<float>::floor( v.x ), math<float>::floor( v.y ) );
}

class Movie {
  public:
	Movie( const NetflixMovie &info, Vec2f pos, Vec2f size )
		: mInfo( info ), mBasePos( pos ), mPos( pos ), mSize( size ), mAlpha( 1 ), mIconAlpha( 0 ), mTitleOffset( sTitleOffset )
	{
	}
  
	void draw() {
		gl::color( ColorA( 1, 1, 1, mAlpha ) );
		gl::drawSolidRect( Rectf( mPos, mPos + mSize ) );
		gl::color( ColorA( 0.7f, 0.7f, 0.7f, mAlpha ) );
		Rectf inset( Rectf( mPos, mPos + mSize ) );
//		gl::drawStrokedRect( Rectf( inset.getUpperLeft() + Vec2f(2,2), inset.getLowerRight() - Vec2f(2,2)) );
		if( mIconAlpha > 0 ) {
			gl::color( ColorA( 1, 1, 1, mIconAlpha ) );
			gl::draw( sGenericIcon, mPos + Vec2f( 4, 4 ) );
		}
		gl::color( ColorA( 0, 0, 0, mAlpha ) );
		sTitleFont->drawString( mInfo.mTitle, Rectf( floor( mPos + mTitleOffset ), ( mPos + mSize - Vec2f( 2, 0 ) ) ) );
	}

	bool isPointIn( const Vec2f &pt ) { return Rectf( mPos, mPos + mSize ).contains( pt ); }
	
	ci::Vec2f		mBasePos, mPos, mSize;
	float			mAlpha, mIconAlpha;
	ci::Vec2f		mTitleOffset;
	NetflixMovie	mInfo;

	static gl::Texture	sGenericIcon;

	static Vec2f	sNormalSize, sExpandedSize;
	static Vec2f	sTitleOffset, sExpandedTitleOffset;
};
gl::Texture	Movie::sGenericIcon;
Vec2f Movie::sNormalSize, Movie::sExpandedSize, Movie::sTitleOffset, Movie::sExpandedTitleOffset;

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

	sTitleFont = gl::TextureFont::create( Font( "Arial", 16 ) );

	Movie::sGenericIcon = gl::Texture( loadImage( loadResource( RES_FILM_ICON ) ) );
	Movie::sNormalSize = Vec2f( WINDOW_WIDTH - MOVIE_PADDING, WINDOW_HEIGHT - MOVIE_PADDING ) / Vec2f( MOVIE_COLS, MOVIE_ROWS ) - Vec2f( MOVIE_PADDING, MOVIE_PADDING );
	Movie::sExpandedSize = Vec2f( 350, 200 );
	Movie::sTitleOffset = Vec2f( 4, sTitleFont->getAscent() );
	Movie::sExpandedTitleOffset = Vec2f( 68, sTitleFont->getAscent() );
	const Vec2f tileOffset = Movie::sNormalSize + Vec2f( MOVIE_PADDING, MOVIE_PADDING );
	
	TimelineRef initialFadeTln = Timeline::create();
	for( size_t m = 0; m < parser.getMovies().size(); ++m ) {
		int row = m / MOVIE_COLS;
		int col = m % MOVIE_COLS;
		Vec2f pos = Vec2f( MOVIE_PADDING, MOVIE_PADDING ) + Vec2f( col * tileOffset.x, row * tileOffset.y );
		mMovies.push_back( Movie( parser.getMovies()[m], pos, Movie::sNormalSize ) );
		
console() << parser.getMovies()[m] << std::endl;
		
		mMovies.back().mAlpha = 0.0;
		TimelineItemRef movTween = initialFadeTln->add( &mMovies.back().mPos, Vec2f( 0, 0 ), pos, pos.y / getWindowHeight() * 0.5 );
		movTween->delay( row / 10.0f );
		movTween = initialFadeTln->append( &mMovies.back().mAlpha, 0.0f, 1.0f, 0.5f );
		movTween->setStartTime( movTween->getStartTime() );
		movTween->delay( -0.02f );
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
		mTimeline.apply( &mCurrentSelection->mSize.x, Movie::sNormalSize.x, 0.08f );
		mTimeline.apply( &mCurrentSelection->mSize.y, Movie::sNormalSize.y, 0.08f );
		mTimeline.apply( &mCurrentSelection->mTitleOffset, Movie::sTitleOffset, 0.08f );
		mTimeline.apply( &mCurrentSelection->mIconAlpha, 0.0f, 0.04f );
		mTimeline.apply( &mCurrentSelection->mPos, mCurrentSelection->mBasePos, 0.2f )->delay( 0.1f );		
	}
	
	// find the new selection
	mCurrentSelection = mMovies.end();
	for( list<Movie>::iterator movIt = mMovies.begin(); movIt != mMovies.end(); ++movIt ) {
		if( movIt->isPointIn( event.getPos() ) ) {
			mCurrentSelection = movIt;
			mTimeline.apply( &mCurrentSelection->mSize.x, Movie::sExpandedSize.x, 0.2f );
			mTimeline.apply( &mCurrentSelection->mSize.y, Movie::sExpandedSize.y, 0.2f )->delay( 0.1f );
			mTimeline.apply( &mCurrentSelection->mTitleOffset, Movie::sExpandedTitleOffset, 0.2f );
			mTimeline.apply( &mCurrentSelection->mIconAlpha, 0.8f, 0.2f )->delay( 0.1f );
			mTimeline.apply( &mCurrentSelection->mPos, mCurrentSelection->mBasePos + Vec2f( -10, -10 ), 0.2f )->delay( 0.1f );
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
	gl::enableAlphaBlending( false );
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
