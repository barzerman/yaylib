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

#include <yay/yay_snowball.h>
extern "C" {
#include "snowball/libstemmer_c/include/libstemmer.h"
}

namespace yay {


int StemWrapper::getLangFromString( const char* langStr )
{
	switch(tolower(langStr[0])) {
	case 'f': 
		if( tolower(langStr[1]) == 'r' ) 
			return LG_FRENCH;
		break;
    case 'e': 
	{
		char sec = tolower(langStr[1]);
		if (sec == 's')
			return LG_SPANISH;
		else if (sec == 'n')
			return LG_ENGLISH;
		break;
	}
	}
	return LG_INVALID;
}

bool StemWrapper::isUnicodeLang (int lang)
{
	return lang != LG_ENGLISH;
}

const char* StemWrapper::getValidLangString( int lang ) 
{
    if( lang == LG_FRENCH ) 
        return "fr";
    else if( lang == LG_SPANISH ) 
        return "es";
	else if (lang == LG_ENGLISH)
		return "en";
    else
        return 0;
}
void StemWrapper::freeSnowballStemmer( sb_stemmer* sb )
{
    if( sb )
	    sb_stemmer_delete( sb );
}
sb_stemmer* StemWrapper::mkSnowballStemmer( int lang ) 
{
    const char* lg = getValidLangString(lang);
    return( lg ?  sb_stemmer_new(lg, 0) : 0 );
}

bool StemWrapper::stem (const char* in, size_t len, std::string& out) const
{
	const sb_symbol *sbs = reinterpret_cast<const sb_symbol*> (in);
	const char *result = reinterpret_cast<const char*> (sb_stemmer_stem(m_stemmer, sbs, len));
	if (!result)
		return false;

	out.assign(result, sb_stemmer_length(m_stemmer));
	return strcmp(in, result);
}

void MultilangStem::addLang( int lang )
{
	if (m_stemmers.find(lang) != m_stemmers.end())
		return;

    sb_stemmer* sb = StemWrapper::mkSnowballStemmer(lang);
    if( sb ) 
        m_stemmers.insert(std::make_pair(lang, StemWrapper(sb)));
}

bool MultilangStem::stem(int lang, const char *in, size_t length, std::string& out, bool fallback) const
{
	stemmers_t::const_iterator pos = m_stemmers.find(lang);
	if (pos != m_stemmers.end() && pos->second.stem(in, length, out))
		return true;

	if (fallback)
	{
		for (stemmers_t::const_iterator i = m_stemmers.begin(), end = m_stemmers.end(); i != end; ++i)
		{
			if (i->first == lang)
				continue;

			if (i->second.stem(in, length, out))
				return true;
		}
	}

	return false;
}

void MultilangStem::clear()
{
	for(stemmers_t::iterator i = m_stemmers.begin(); i!= m_stemmers.end(); ++i ) {
		StemWrapper::freeSnowballStemmer( i->second.snowball() );
	}
	m_stemmers.clear();
}

} // namespace yay
