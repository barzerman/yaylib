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


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <yay/yay_util_time.h>

namespace yay {
stopwatch::stopwatch( ) : sec(0), msec(0)
{
	gettimeofday(&tmpTv, 0 );
}
stopwatch& stopwatch::start( )
{
	gettimeofday(&tmpTv, 0 );
	return *this;
}
stopwatch& stopwatch::calcTime(bool advance )
{
	struct timeval tv;
	gettimeofday(&tv, 0 );
	//std::cerr << "CRAP: " << tv.tv_sec << ":" << tv.tv_usec << " vs " << 
	//tmpTv.tv_sec << ":" << tmpTv.tv_usec << "\n";
	
	time_t tmpSec = sec;
	suseconds_t tmpMSec = msec;

	if( tv.tv_usec< tmpTv.tv_usec ) {
		msec = 1000000 + tv.tv_usec - tmpTv.tv_usec;
		sec = tv.tv_sec - tmpTv.tv_sec-1;
	} else {
		msec = tv.tv_usec - tmpTv.tv_usec;
		sec = tv.tv_sec - tmpTv.tv_sec;
	}
	if( advance ) {
		sec += tmpSec;
		msec += tmpMSec;
	}
	if( msec >= 1000000 ) {
		++sec;
		msec -= 1000000;
	}
	return *this;
}

stopwatch& stopwatch::restart( )
{
	gettimeofday(&tmpTv, 0 );

	msec = 0;
	sec = 0;
	return *this;
}

} // namespace yay

#ifdef YAY_UTIL_TIME_CPP_TEST
main( int argc, char* argv[] )
{
	yay::stopwatch sw;
	yay::stopwatch sw1;
	int iter = 0;
	while(1) {
		sw.start();
		for( int i=0; i< 6000000; ++i ) ;
		sw.stop();
		std::cerr << "timer 1 " <<  sw<< "\n";
		sleep(1);
		std::cerr << ++iter << "  timer 2 " <<  sw1.calcTime() << "\n";
	}
	getchar();	
	std::cerr << "elapsed sec:microsec " <<  sw.stop()<< "\n";
}
#endif // YAY_UTIL_TIME_CPP_TEST
