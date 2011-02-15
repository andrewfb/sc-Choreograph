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
};

// Returns the text in 'html' between the 'prefix' and 'suffix' strings, 
// and modifies 'html' to point to the first character after 'suffix'
// returns empty string on failure
string parseHtmlSection( string *html, const string &prefix, const string &suffix )
{
	size_t start = html->find( prefix );
	if( start == string::npos )
		return string();
	else
		start += prefix.length();
	size_t end = html->find( suffix, start );
	if( end == string::npos )
		return string();
	else {
		string result = html->substr( start, end - start );
		*html = html->substr( end + suffix.length() );
		return result;
	}
}

// Converts the HTML of the description into individual pieces of data
// <a href="[link]"><img src="[imgUrl]"/></a><br>[description]
bool parseHtmlDescription( const string &html )
{
	string htmlCopy( html );

	string link = parseHtmlSection( &htmlCopy, "<a href=\"", "\">" );
	if( link.empty() )
		return false;

	string imgUrl = parseHtmlSection( &htmlCopy, "<img src=\"", "\"/></a>" );
	if( imgUrl.empty() )
		return false;

	string description = htmlCopy;
	if( description.find( '<' ) == 0 ) // strip the leading <br>
		description = description.substr( description.find( '>' ) + 1 );

	console() << "Link: " << link << " | " << imgUrl << " | " << std::endl << description << std::endl;
	
	return true;
}

void NetflixRssApp::setup()
{
	XmlTree xml( loadUrl( "http://rss.netflix.com/Top100RSS" ) );
	for( XmlTree::ConstIter itemIt = xml.begin( "/rss/channel/item" ); itemIt != xml.end(); ++itemIt ) {
		parseHtmlDescription( itemIt->getChild( "description" ).getValue() );
	}
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
