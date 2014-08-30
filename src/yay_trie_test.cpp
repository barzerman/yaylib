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

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <yay/yay_debug.h>
#include <yay/yay_trie.h>

typedef yay::trie<int, int > Trie;
typedef yay::char_trie_funcs<uint32_t> CharTrie;
struct CB_crap {
	yay::trie_visitor_continue_t operator()( const Trie::Path& p)

	{
		for( Trie::Path::const_iterator i = p.begin(); i != p.end();++i ) {
			std::cerr << (*i)->first << ":" << (*i)->second.data() << "/" ;
		}
		std::cerr << std::endl;
		return yay::TRIE_VISITOR_CONTINUE;
	}
};
struct Uint32CharTrie_fullpath_print_cb {
	yay::trie_visitor_continue_t operator()( const CharTrie::Trie::Path& p)

	{
		for( CharTrie::Trie::Path::const_iterator i = p.begin(); i != p.end();++i ) {
			std::cerr << (*i)->first << ":" << (*i)->second.data() << "/" ;
		}
		std::cerr << std::endl;
		return yay::TRIE_VISITOR_CONTINUE;
	}
};

struct Uint32CharTrie_print_cb {
	yay::trie_visitor_continue_t operator()( const CharTrie::Trie::Path& p)

	{
		if( !p.size() || p.back()->second.data() == 0xffffffff ) 
			return yay::TRIE_VISITOR_CONTINUE;

		for( CharTrie::Trie::Path::const_iterator i = p.begin(); i != p.end();++i ) {
			std::cerr << (*i)->first ;
		}
		std::cerr << ":" << p.back()->second.data() << std::endl;
		return yay::TRIE_VISITOR_CONTINUE;
	}
};

int main( int argc, char* argv[] ) 
{
	//// 
	{
		Uint32CharTrie_print_cb cb;
		CharTrie::Trie ctrie;
		CharTrie::add( ctrie, "he", 333, 0xffffffff );
		CharTrie::add( ctrie, "hello", 150, 0xffffffff );
		CharTrie::add( ctrie, "hell", 250, 0xffffffff );
		yay::trie_visitor< CharTrie::Trie, Uint32CharTrie_print_cb > vis(cb);
		
		vis.visit(ctrie);
		char buf[1024];
		
		while( true ) {
			std::vector< int > intVec;
			int i4=0;
			std::cout << "Enter word:";
			
			std::cin.getline( buf, sizeof(buf) );
				
			typedef std::pair< const CharTrie::Trie*, const char*  > PathPair;
			
			PathPair pp = CharTrie::matchString( ctrie, buf );
			if( pp.first ) {
				std::cerr << "substring matched:" << pp.first->data() << "~"  << std::string( buf, pp.second- buf ) << std::endl;
				std::cerr << "\n";
			} else 
				std::cerr << "substring did not match!\n";
		}
	}

	Trie t;
	
	t.add(10,2).add(11,3);
	t.add(10,2).add(25,3).add(100,18);
	t.add(10,2).add(25,3);
	t.add(10,2).add(25,3);
	t.add(10,2).add(25,3);
	t.add(10,2).add(25,3);
	t.add(10,2).add(25,3);
	t.add(10,2).add(25,3);
	t.add(10,2).add(25,3);
	CB_crap cb;
	yay::trie_visitor< Trie, CB_crap > vis(cb);
	
	vis.visit(t);
	
	char buf[1024];
	while( true ) {
		std::vector< int > intVec;
		int i4=0;
		std::cout << "Enter path:";
		
		std::cin.getline( buf, sizeof(buf) );
		std::istringstream sstr(buf);
		while( sstr >> i4 ) {
			std::cerr << "adding " << i4 << "\n";
			intVec.push_back( i4 );
		}
		
		typedef std::pair< bool, std::vector< int >::const_iterator > PathPair;
		
		PathPair pp = t.getLongestPath( intVec.begin(), intVec.end() );
		if( pp.first ) {
			std::cerr << "path matched:" ;
			for( std::vector< int >::const_iterator i = intVec.begin(); i!= pp.second; ++i )
			{
				std::cerr << *i << "/";
			}
			std::cerr << "\n";
		} else 
			std::cerr << "Path did not match!\n";
	}
		
}
