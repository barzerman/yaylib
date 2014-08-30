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
#include <yay/yay_headers.h>

#include <iostream>
#include <iomanip>
namespace yay {

/// stopwatch is a cumulative timer - wrapper of gettimeofday
/// starts on construction
/// standard usage:
/// yay::stopwatch sw;
///  std::cout << sw.calcTime() << "\n";
///  or if you need to pause the watch 
/// sw.stop() ... do something else 
/// sw.start() 
struct stopwatch 
{
	time_t sec; // total elapsed seconds`
	suseconds_t msec; // total elapsed microseconds

	struct timeval tmpTv;

	stopwatch();
	/// if advance == true advances the times 
	/// advance true is equivalent to stop()
	stopwatch& calcTime(bool advance= false);
	stopwatch& stop()
		{ calcTime(true); return *this; }
	stopwatch& start();
	/// 0s everything and restarts
	stopwatch& restart();

	inline std::ostream& print( std::ostream& fp )  const
	{
		return fp << sec << "." << std::setfill('0') << std::setw(6) << msec;
	}
	double getTimeAsDouble() const
		{ return (double)sec + msec/1000000.0; }
	double calcTimeAsDouble(bool advance=false) 
		{ calcTime(advance); return (double)sec + msec/1000000.0; }
};
inline std::ostream& operator <<( std::ostream& fp, const stopwatch& sw )
{
	return sw.print(fp);
}

} // namesapce yay
