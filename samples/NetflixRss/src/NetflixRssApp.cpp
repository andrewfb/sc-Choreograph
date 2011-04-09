#include "NetflixRssParser.h"

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Xml.h"
#include "cinder/Thread.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NetflixRssApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	vector<NetflixMovie>	mMovies;
};

void NetflixRssApp::setup()
{
	NetflixRssParser parser( Url( "http://rss.netflix.com/Top100RSS" ) );
	mMovies = parser.getMovies();
	
	for( vector<NetflixMovie>::const_iterator movIt = mMovies.begin(); movIt != mMovies.end(); ++movIt )
		console() << *movIt << std::endl;
}

void NetflixRssApp::mouseDown( MouseEvent event )
{
}

void NetflixRssApp::update()
{
}

void NetflixRssApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( NetflixRssApp, RendererGl )
