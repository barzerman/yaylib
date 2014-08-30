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
#include <yay/yay_bitflags.h>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
namespace yay {

struct  dir_iter_t {
public: 
    enum { 
        BIT_DIR_RECURSE,
        BIT_FOLLOW_LINKS,
        BIT_MAX
    };
    yay::bitflags<BIT_MAX> d_bits;
    
    template <typename CB>
    void operator()( CB& cb, boost::filesystem::path path, size_t depth=0 ) 
    {
        if ( boost::filesystem::exists(path) && boost::filesystem::is_directory(path)) {
            boost::filesystem::directory_iterator end_iter;
            for( boost::filesystem::directory_iterator di(path) ; di != end_iter ; ++di) {
                if( !cb(di,depth) ) 
                    return;
                if (boost::filesystem::is_regular_file(di->status()) ) {
                    ;
                } else if (boost::filesystem::is_directory(di->status()) ) {
                    dir_iter_t nextLevel(*this);
                    if( d_bits.check(BIT_DIR_RECURSE) )
                        nextLevel(cb,di->path(),depth+1);
                } 
            }
        }
    }
};

struct dir_iter_print_t {
    std::ostream& fp;
    dir_iter_print_t( std::ostream& f ) : fp(f) {}

    /// returns false if wants recursion terminated 
    bool operator()( boost::filesystem::directory_iterator& di, size_t depth ) {
        for( size_t i=0; i< depth; ++i ) fp << '\t';

        fp << di->path().parent_path().string() << '/' << di->path().filename().string() << ( boost::filesystem::is_directory(di->status()) ? "/" : ""  ) << std::endl; 
        return true;
    }
};
/// this can be used to add regexp filter on filename (only the filename will be searched for the regexp)
template <typename CB>
struct fs_filter_regex {
public:
    CB&              callback;
    boost::regex    pattern;
    
    fs_filter_regex( CB& cb ): callback(cb) {}
    fs_filter_regex( CB& cb, const char* p ) : callback(cb), pattern(p) {}

    bool operator()( boost::filesystem::directory_iterator& di, size_t depth=0 ) {
        if( pattern.empty() || boost::regex_search (di->path().filename().string(), pattern) )
            return callback(di,depth); 
        return true;
    }
};

/// this function is here mainly 
inline void dir_list( std::ostream& fp, const char* path, bool recurse=false, const char* regex=0 )
{
    dir_iter_print_t printer(fp);
    fs_filter_regex<dir_iter_print_t> fs_regex_iter(printer);

    dir_iter_t dip;
    if( recurse ) 
        dip.d_bits.set( dir_iter_t::BIT_DIR_RECURSE );
    if( regex ) 
        fs_regex_iter.pattern= regex;

    dip( fs_regex_iter, std::string(path) );
}

template <typename CB>
inline void dir_regex_iterate( CB& cb, const char* path, const char* regex, bool recurse=false )
{
    fs_filter_regex<CB> fs_regex_iter(cb);

    dir_iter_t dip;
    if( recurse ) 
        dip.d_bits.set( dir_iter_t::BIT_DIR_RECURSE );
    if( regex ) 
        fs_regex_iter.pattern= regex;

    dip( fs_regex_iter, std::string(path) );
}

} // namespace yay
