/*============================================================================
The MIT License (MIT)

Copyright (c) 2014 Andre Yanpolsky, Max Eronin, Georg Rudoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
============================================================================*/

#include <yay/yay_logger.h>


namespace yay {

Logger *Logger::instance_ = 0;
uint8_t Logger::LEVEL = Logger::WARNING;
namespace {
const char* g_LOG_LVL_STR[] = {"DEBUG", "WARNING","ERROR","CRITICAL"};
}
const char* yay::Logger::getLogLvlStr( int x)
{
	return( x>=0 && (unsigned)(x)< sizeof(g_LOG_LVL_STR)/sizeof(g_LOG_LVL_STR[0]) ? g_LOG_LVL_STR[x]: "");

}


StreamWrap LogMsg::getStream()
{
	// return (level_ >= logger_->LEVEL) ? logger_->getStream() : voidstream;
	if ( level_ >= logger_->LEVEL )
		return StreamWrap(logger_->getStream());
	else
		return StreamWrap();
}

void Logger::init( uint8_t l = WARNING ) 
{ 
    LEVEL = l; 
}

Logger* Logger::getLogger()
{
	if ( !instance_ )
		instance_ = new Logger();
	return instance_;
}

void Logger::setFile( const char* filename )
{
	if ( gotfile )
		delete stream_;
	gotfile = true;
	stream_ = new std::ofstream( filename );
}

void Logger::setStream( std::ostream* os )
{
	if ( gotfile ) {
		delete stream_;
		gotfile = false;
	}
	stream_ = os;
}

std::ostream& Logger::logMsg( const uint8_t lvl, const char* filename,
		const int lineno )
{
	if (lvl >= Logger::LOG_LEVEL_MAX)
		return *stream_; // should probably crash right here
	if ( lvl >= LEVEL ) {
		return (*stream_ << filename << ":" << lineno << ":[" << g_LOG_LVL_STR[lvl]
				<< "] ") ;
	}
	return *stream_;
}

#if (defined (_WINDOWS_) || defined(WIN32) || defined(_MSC_VER))
inline char *getcwd(char *buf, size_t size)
{
    /// maybe some day someone will implement getcwd for windows (YAY 04/15/2012))
    return ( (size? (*buf=0):0 ), "" );
}
#else
#include <unistd.h>
#endif
std::ostream& print_absolute_file_path( std::ostream& fp, const char* fileName )
{
    if( !fileName ) 
        return fp;
    if( *fileName != '/' ) {
        // if we're here fileName is not an absolute path
        char curDir[1024]; 
        curDir[0] = curDir[ sizeof(curDir)-1 ] = 0;
        if( getcwd(curDir, sizeof(curDir)-1) ) 
            fp << curDir << "/";
    }
    return ( fp << fileName );
}

} // namespace yay
