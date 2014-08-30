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

#include <yay/yay_xml_util.h>

/// char string utilities 
namespace yay {

namespace {
inline std::ostream& singleCharPut( std::ostream& os, char c )
{
    switch (c) {
    case '&': return os << "&amp;";
    case '<': return os << "&lt;";
    case '>': return os << "&gt;";
    case '"': return os << "&quot;";
    case '\'': return os << "&apos;";
    default: return( os.put(c), os );
    }
}

}

std::ostream& XMLStream::escape(const char *s)
{
	for(;*s; ++s) singleCharPut(os,*s);
	return os;
}
std::ostream& XMLStream::escape(const char *s, size_t s_sz )
{
    const char* s_end = s+s_sz;
	for(;s< s_end; ++s) singleCharPut(os,*s);
	return os;
}

std::ostream& XMLStream::escape(const std::string& str)
{
	return escape(str.c_str(), str.size());
}

namespace html {
void unescape( std::string& out, const char* str, size_t sz )
{
#define PUSH_BACK_IF( x, c ) if( !strncmp((s+1),x,sizeof(x)-1) ) { s+=sizeof(x); out.append( c ); }
    for( const char* s = str, *s_end = str+sz; s< s_end; ++s ) {
        if( *s == '&' ) {
             PUSH_BACK_IF("nbsp", " ")
             else PUSH_BACK_IF("lt", "<")
             else PUSH_BACK_IF("gt", ">")
             else PUSH_BACK_IF("amp", "&")
             else PUSH_BACK_IF("cent", "¢")
             else PUSH_BACK_IF("pound", "£")
             else PUSH_BACK_IF("yen", "¥")
             else PUSH_BACK_IF("euro", "€")
             else PUSH_BACK_IF("sect", "§")
             else PUSH_BACK_IF("copy", "©")
             else PUSH_BACK_IF("reg", "®")
             else PUSH_BACK_IF("trade", "™")
             else PUSH_BACK_IF("apos", "'")
             else PUSH_BACK_IF("ndash", "-")
             else PUSH_BACK_IF("quot", "\"")
             else if( s[1] == '#' ) {
                const bool isHex = (s[2] == 'x' );
                char *end = 0;
                const uint8_t val = strtol(s+ (isHex ? 3 : 2), &end, isHex ? 16 : 10);
                if (val)
                    out.push_back( static_cast<char>(val) ) ; 
                if( end ) 
                    s= end;
             }
        } else {
            out.push_back(*s);
        }
    }
}
void unescape_in_place( std::string& str )
{
    if( !strchr( str.c_str(), '&' ) ) 
        return ;
    std::string newStr;
    unescape( newStr, str.c_str(), str.length() );
    str.swap( newStr );
}
void unescape_in_place_old( std::string& str )
{
    size_t pos = 0;
    
    auto knownFixer = [&pos, &str](const std::string& entity, const std::string& after) -> bool
    {
        if (pos + entity.size() + 1 >= str.size())
            return false;
        
        for (size_t i = 0; i < entity.size(); ++i)
        {
            if (entity[i] != str[pos + i + 1])
                return false;
        }
        
        str.replace(pos, entity.size() + 1, after);
			pos += after.size();
        return true;
    };
		
    while ((pos = str.find('&', pos)) != std::string::npos)
    {
        if (pos + 2 >= str.size())
            break;
        
        if (str[pos + 1] == '#')
        {
            const auto isHex = str[pos + 2] == 'x';
            char *end = 0;
            const auto val = strtol(str.c_str() + pos + (isHex ? 3 : 2), &end, isHex ? 16 : 10);
            if (val)
                str.replace(pos, end - (str.c_str() + pos) + 1, 1, static_cast<char>(val));
        }
			else if (
                 knownFixer("nbsp;", " ") ||
                 knownFixer("lt;", "<") ||
                 knownFixer("gt;", ">") ||
                 knownFixer("amp;", "&") ||
                 knownFixer("cent;", "¢") ||
                 knownFixer("pound;", "£") ||
                 knownFixer("yen;", "¥") ||
                 knownFixer("euro;", "€") ||
                 knownFixer("sect;", "§") ||
                 knownFixer("copy;", "©") ||
                 knownFixer("reg;", "®") ||
                 knownFixer("trade;", "™") ||
                 knownFixer("apos;", "'") ||
                 knownFixer("ndash;", "-") ||
                 knownFixer("quot;", "\""))
            ;
        else
            ++pos;
    }
}

} // html namespace ends
} // yay namespace ends
