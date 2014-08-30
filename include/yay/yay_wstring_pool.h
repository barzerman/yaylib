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
#include <vector>
#include <hash_map>
#include "yay/yay_ro_wstring.h"
namespace yay {

class WStringPool {
	std::vector<ro_wstring> poolVec;
	enum { DFLT_POOL_SZ = 256*256 }; // size of single buffer in 
								     // wchar_t

	ro_wstring::const_iterator curPoolBeg, curPoolEnd;
	size_t poolSz;
	
	size_t curLen() const { return ( curPoolEnd - curPoolBeg ); }

	wchar_t* curPtrBeg() { return &(*curPooBeg); }
	wchar_t* curPtrEnd() { return &(*curPooEnd); }

	void addPool( )
	{
		wchar_t * buf = new wchar_t[ poolSz ];
		poolVec.push_back( ro_wstring( buf, buf+poolSz ) );
		curPoolBeg = poolVec.back().begin();
		curPoolEnd = poolVec.back().end();
	}
	void hasEnoughSpace( size_t sz ) const
		{ return( sz <= curLen() ); }
	
	std::map< ro_wstring, size_t > theMap;
	std::vector< ro_wstring > theVec;
public:
	const ro_wstring* decode( size_t id ) const
		{ return ( id< theVec.size() ? &(theVec[id]) : 0 ); }

	size_t decode( const std::wstring& ws )  const
	{
		std::set< ro_wstring >::const_iterator i = theMap.find( 
			ro_wstring( 
				&(ws[0]),
				&(ws[ws.length()])
			)
		) ;
		if( i!= theMap.end() ) 
			return i->second;
		else
			return ~0;
	}

	size_t appendWStr( const std::wstring& ws ) 
	{
		std::set< ro_wstring >::const_iterator i = theMap.find( 
			ro_wstring( 
				&(ws[0]),
				&(ws[ws.length()])
			)
		) ;
		if( i!= theMap.end() ) 
			return i->second;

		if( ws.length() < curLen() ) {
			memcpy( curPtrBeg(), &(ws[0]), sizeof(wchar_t)*ws.length() );
			ro_wstring::const_iterator oldBeg = curPoolBeg;
			curPoolBeg+= ws.length();

			std::map< ro_wstring, int >::value_type newVal( ro_wstring ( oldBeg, curPoolBeg ), theVec.size() );
			theVec.push_back( newVal.first );
			i = theMap.insert( newVal ).first;
			return i->second;
		} else {
			addPool();
			return appendWStr( ws );
		}
	}
	WStringPool( size_t psz  = DFLT_POOL_SZ ) : 
		poolSz(psz) 
		{ addPool(); }
	~WStringPool()
	{
		for( std::vector<ro_wstring>::const_iterator i = poolVec.begin(); i!= poolVec.end(); ++i )
			delete (wchar_t*)( i->c_wstr() ) ; 
	}
	
}; // WStringPool ends

} // a namespace ends
