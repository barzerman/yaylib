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
// this implements a Schroedinger Object - a wrapper over a type 
// which takes construction argument alongside the boolean telling it whether or not the actual object
// should be constructed on stack
// for example you have a boost mutex lock or some other RAII wrapper, which you want to conditionally 
// do nothing - meaning - you dont want it to actually lock anything 
// schrod<Lock> lock( mutex, false ) would do the job
// the constructor wont actually create Lock(mutex) nor will the destructor destroy it 
namespace yay {

template <typename Actual>
class Schrod {
	char d_buf[ sizeof(Actual) ];
	bool d_exists;

	Schrod& operator=( const Schrod& ) { return *this; }
	Schrod(const Schrod& ) {}
public:
	typedef Actual value_type;
	Schrod( bool ex=true  ) : d_exists(ex) { if( d_exists) new(d_buf) Actual(); }

	template <typename T> Schrod( T& p, bool ex=true  ) : d_exists(ex) { if( d_exists) new(d_buf) Actual(p); }
	template <typename T> Schrod( const T& p, bool ex=true  ) : d_exists(ex) { if( d_exists) new(d_buf) Actual(p); }

	bool exists() const { return d_exists; }
	Actual* actual() { return ( d_exists? (Actual*)d_buf : 0 ); }
	const Actual* actual() const { return ( d_exists? (Actual*)d_buf : 0 ); }

	~Schrod() { if( d_exists ) (static_cast<Actual*>(d_buf))->Actual::~Actual(); }
};

} // namespace yay
