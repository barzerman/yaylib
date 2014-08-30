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
#include "yay/yay_headers.h"
#include <wchar.h>
#include <iostream>
#include <algorithm>

namespace yay {
typedef wchar_t* wchar_p;
typedef const wchar_p wchar_cp;

/// read only wstring
class ro_wstring {
	wchar_p beg, end;
public:
	struct const_iterator {
		wchar_cp p;
		const_iterator( wchar_cp i ) : p(i) {}
		const_iterator( ) : p(0) {}
		
		const_iterator& operator --(int) { return(--p,*this); }
		const_iterator& operator --() 	 { return(--p,*this); }
		const_iterator& operator ++(int) { return(++p,*this); }
		const_iterator& operator ++() 	 { return(++p,*this); }
		const_iterator& operator +=(int i) 	 { return(p+=i,*this); }
		const_iterator& operator -=(int i) 	 { return(p-=i,*this); }

		const wchar_t& 	operator *() 	 { return *p; }
	};
	
	const_iterator begin() const { return const_iterator(beg); }
	const_iterator end() const { return const_iterator(end); }

	size_t length() const { return (end-begin); }
	size_t size() const { return length(); }
	
	const wchar_t& operator[]( int i ) const { return p[i]; }

	ro_wstring( wstring::const_iterator b, wstring::const_iterator e ) :
		beg( &(*b)), end( &(*e)) {}
	ro_wstring( const wchar_t* b, const wchar_t* e ) :
		beg(b), end(e) {}
	ro_wstring() : beg(0), end(0) {}
	
	const wchar_t* c_wstr() { return begin; }
};

inline int operator -( const ro_wstring::const_iterator& l, const ro_wstring::const_iterator& r ) 
{ return(r.p - r.p); }

inline std::ostream& operator<<( std::ostream& fp, const ro_wstring& s )
{
	if( s.length() ) { 
		ro_wstring::const_iterator end= s.end();
		for( ro_wstring::const_iterator i= s.begin(); i!= end; ++i ) 
			fp << *i;
	}
	return fp;
}

inline bool operator <( const ro_wstring& l, const ro_wstring& r )
{ 
	ro_wstring::const_iterator l_end = l.end();
	return( std::lexicographical_compare( l.begin(), l_end, r.begin(), r.end() ) );
}
inline bool operator ==( const ro_wstring& l, const ro_wstring& r )
{ 
	ro_wstring::const_iterator l_end = l.end();
	return std::equal( l.begin(), l_end, r.begin() ); 
}

} // namespace yay
