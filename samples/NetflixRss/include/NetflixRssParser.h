#pragma once

#include "cinder/Xml.h"
#include "cinder/Url.h"

#include <ostream>

struct NetflixMovie {
	std::string		mTitle, mDescription;
	ci::Url			mImgUrl, mLink;
};

class NetflixRssParser {
  public:
	NetflixRssParser( const ci::Url &xmlUrl ) {
		ci::XmlTree xml( ci::loadUrl( "http://rss.netflix.com/Top100RSS" ) );
		for( ci::XmlTree::ConstIter itemIt = xml.begin( "/rss/channel/item" ); itemIt != xml.end(); ++itemIt ) {
			NetflixMovie newMovie;
			std::string titleStr = itemIt->getChild( "title" ).getValue();
			newMovie.mTitle = titleStr.substr( titleStr.find( "- ") + 2, std::string::npos );
			if( parseHtmlDescription( itemIt->getChild( "description" ).getValue(), &newMovie.mLink, &newMovie.mImgUrl, &newMovie.mDescription ) ) {
				mMovies.push_back( newMovie );
			}
		}
	}

	std::vector<NetflixMovie>&	getMovies() { return mMovies; }

	// Converts the HTML of the description into individual pieces of data
	// <a href="[link]"><img src="[imgUrl]"/></a><br>[description]
	bool parseHtmlDescription( const std::string &html, ci::Url *link, ci::Url *imgUrl, std::string *description )
	{
		std::string htmlCopy( html );

		*link = ci::Url( parseHtmlSection( &htmlCopy, "<a href=\"", "\">" ) );
		if( link->str().empty() )
			return false;

		*imgUrl = ci::Url( parseHtmlSection( &htmlCopy, "<img src=\"", "\"/></a>" ) );
		if( imgUrl->str().empty() )
			return false;

		*description = htmlCopy;
		if( description->find( '<' ) == 0 ) // strip the leading <br>
			*description = description->substr( description->find( '>' ) + 1 );
		
		return true;
	}

	// Returns the text in 'html' between the 'prefix' and 'suffix' strings, 
	// and modifies 'html' to point to the first character after 'suffix'
	// returns empty string on failure
	static std::string parseHtmlSection( std::string *html, const std::string &prefix, const std::string &suffix )
	{
		size_t start = html->find( prefix );
		if( start == std::string::npos )
			return std::string();
		else
			start += prefix.length();
		size_t end = html->find( suffix, start );
		if( end == std::string::npos )
			return std::string();
		else {
			std::string result = html->substr( start, end - start );
			*html = html->substr( end + suffix.length() );
			return result;
		}
	}

	std::vector<NetflixMovie>		mMovies;
};


	
std::ostream& operator<<( std::ostream &os, const NetflixMovie &movie ) {
	os << movie.mTitle << ":" << std::endl;
	os << " l: " << movie.mLink << "   i: " << movie.mImgUrl << std::endl;
	os << "  " << movie.mDescription;
	return os;
}