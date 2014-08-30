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

#pragma  once
#include <string>
#include <iostream>
#include <vector>
#include <yay/yay_util.h>
/// char string utilities 
namespace yay {
/// ensures that stream receives properly escaped XML
struct XMLStream {
    std::ostream& os;

    std::ostream& escape(const char *s );
    std::ostream& escape(const char *s, size_t s_sz );
	std::ostream& escape(const std::string&);
	
    XMLStream( std::ostream& o ) : os(o) {}
    template <typename T>
    XMLStream& print( const T& x)
        { return (os<< x, *this ); }
    
    XMLStream& print( const std::string& x)
        { return (escape(x.c_str()), *this); }

    typedef std::ios_base& ( *IOS_BASE_PF )(std::ios_base&);
    typedef std::ios& ( *IOS_PF )(std::ios&);
    typedef std::ostream& ( *IOS_OSTREAM )(std::ostream&);
};

template <typename T>
inline XMLStream& operator<<( XMLStream& s, const T& x )
    { return s.print(x); }
inline XMLStream& operator <<(XMLStream& vs, XMLStream::IOS_OSTREAM pf )
    { return( pf((vs.os)), vs ); }

inline XMLStream& operator<<(XMLStream& vs, XMLStream::IOS_PF pf )
    { return( pf((vs.os)), vs ); }

inline XMLStream& operator<<(XMLStream& vs, XMLStream::IOS_BASE_PF pf ) 
{ return (pf((vs.os)),vs); }

struct tag_raii {
	std::ostream &os;
	std::vector<const char*> tags;

	tag_raii(std::ostream &s) : os(s) {}
	tag_raii(std::ostream &s, const char *tag, const char* attr = 0) : os(s) 
        { push(tag,attr); }
	operator std::ostream&() { return os; }

	tag_raii& push(const char *tag, const char* attr=0 ) {
		os << "<" << tag << ( attr ? attr : "" ) << ">";
		tags.push_back(tag);
        return *this;
	}
    size_t pop()
    {
		if( tags.size() ) {
			os << "</" << tags.back() << ">";
			tags.pop_back();
		}
        return tags.size();
    }
    tag_raii& text( const char* s, const char* tag=0, const char* attr=0 ) 
    {
        if( tag )
            push(tag,attr);
        XMLStream(os).print(s);
        if( tag )
            pop();
        return *this;
    }
    tag_raii& text( const std::string& s, const char* tag=0, const char* attr=0  )
        { return text( s.c_str(),tag,attr ); }

	~tag_raii() 
        { do {} while( pop() ); }
};

struct json_raii {
    std::ostream& d_fp;
    size_t d_count, d_depth;

    bool d_isArray; /// when false this is an object

    bool isArray() const { return d_isArray; }
    bool isObject() const { return !d_isArray; }
    std::ostream& getFP() { return d_fp; }

    json_raii( std::ostream& fp, bool arr, size_t depth ) : 
        d_fp(fp), d_count(0), d_depth(depth),d_isArray(arr)  
        { d_fp << ( isArray() ? "[" : "{" ); }
    
    ~json_raii() 
        { d_fp << ( isArray() ? "]" : "}" ); }
    
    std::ostream& indent(std::ostream& fp) 
    {
        for( size_t i=0; i< d_depth; ++i ) fp << "    ";
        return fp;
    }
    std::ostream& startFieldNoindent( const char* f="" ) 
    {
        d_fp << (d_count++ ? ", ": "");
        return ( isArray() ? d_fp : yay::jsonEscape( f, d_fp<< "\"") << "\": " );
    }
    std::ostream& startField( const char* f, bool noNewLine=false ) 
    {
        if( !d_count && !noNewLine)
            d_fp << "\n";
        indent( d_fp << (d_count++ ? (noNewLine ? ",":",\n"):"") );
        return ( isArray() ? d_fp : yay::jsonEscape( f, d_fp<< "\"") << "\": " );
    }

    std::ostream& addKeyVal( const char* k, const char* v ) 
        { return startField(k) << "\"" << v << "\""; }
    std::ostream& addKeyValNoIndent( const char* k, const char* v ) 
        { return startFieldNoindent(k) << "\"" << v << "\""; }
    size_t getDepth() const { return d_depth; }
};

namespace html {
void unescape_in_place( std::string& s );
}
} // yay namespace ends 
