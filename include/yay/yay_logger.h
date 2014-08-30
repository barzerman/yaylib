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

#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <yay/yay_headers.h>

#ifndef LOG_DISABLE
#define YAYLOG(l) yay::LogMsg(yay::Logger::l,__FILE__,__LINE__).getStream()
#else
#define YAYLOG(l) StreamWrap()
#endif

#define YAYLOGDEBUG(l) YAYLOG(DEBUG) << #l << " = " << (l)
#define YAYLOGINIT(l) yay::Logger::init(yay::Logger::l);
#define SETLOGLEVEL(l) yay::Logger::LEVEL = yay::Logger::l

namespace yay {

struct StreamWrap {
    std::ostream* os;
    StreamWrap(std::ostream* o=0) : os(o) {}
};
inline StreamWrap operator<<(StreamWrap vs, std::ostream& ( *pf )(std::ostream&)) { 
 if( vs.os ) pf(*(vs.os));
  return vs;
}

inline StreamWrap operator<<(StreamWrap vs, std::ios& ( *pf )(std::ios&)) { 
   if( vs.os ) pf(*(vs.os));
    return vs;
}

inline StreamWrap operator<<(StreamWrap vs, std::ios_base& ( *pf )(std::ios_base&)) { 
 if( vs.os ) pf(*(vs.os));
  return vs;
}
template <typename T>
inline StreamWrap operator<<(StreamWrap vs, const T& t) {
 if( vs.os ) *(vs.os) << t; 
  return vs;
}
inline StreamWrap operator<<(StreamWrap vs, const char t[]) {
 if( vs.os ) *(vs.os) << t; 
  return vs;
}

/*
class VoidStream {};

template<class T>
inline VoidStream& operator<<(VoidStream &vs, const T &v) { return vs; }
*/


class Logger {
public:
	//static VoidStream voidstream;
	static uint8_t LEVEL;

	enum LogLevelEnum {
		DEBUG,
		WARNING,
		ERROR,
		CRITICAL,

		LOG_LEVEL_MAX
	};
	const static std::string LOG_LVL_STR[LOG_LEVEL_MAX];

	static void init(uint8_t l);

	static Logger* getLogger();

	static void setLevel(uint8_t l) { LEVEL = l; }

	std::ostream& logMsg(const uint8_t lvl, const char* filename,
										   const int lineno);

	std::ostream* getStream() { 
        if( stream_ )
            return stream_; 
        else 
            return &std::cerr;
    }


	void setFile(const char* filename);
	void setStream(std::ostream* stream);
	static const char* getLogLvlStr( int );
private:
	static Logger *instance_;
	std::ostream *stream_;
	bool gotfile;
	Logger() : stream_(&std::cerr), gotfile(false) {}
};


class LogMsg {
	uint level_;
	Logger* logger_;
public:
	LogMsg(const uint8_t lvl, const char* filename, const int lineno) :
		level_(lvl), logger_(Logger::getLogger())
	{
		logger_->logMsg(lvl, filename, lineno);
	}
	~LogMsg()
	{
         StreamWrap sw = getStream();
         if( sw.os )
		    (*(sw.os) << "\n").flush();

	}
	StreamWrap getStream();

private:

};

/// if fileName is not an absolute path deduces current directory and prints it
std::ostream& print_absolute_file_path( std::ostream& , const char* fileName );
} // namespace yay

