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

#include <sstream>
#include <boost/python.hpp>

namespace yay {

namespace python {

struct object_extract {

    template <typename T>
    bool extract( T& t, const boost::python::object& o )
    {
        boost::python::extract<T> e(o);
        if ( e.check() ) 
            return ( t=e, true );
        else
            return false;
    }


    template <typename T>
    bool operator()( T& t, const boost::python::object& o)
        { return extract(t,o); }
    bool operator()( std::string& t, const boost::python::object& o)
    {
        if( extract(t,o) ) 
            return true;
        /// trying to extract 
        { /// int
        int x = 0.0;
        if( extract(x,o) ) {
            std::stringstream sstr;
            sstr<< x;
            return ( t=sstr.str(),true );
        }
        } // end of int 
        { /// double
        double x = 0.0;
        if( extract(x,o) ) {
            std::stringstream sstr;
            sstr<< x;
            return ( t=sstr.str(),true );
        }
        } // end of double
        return false;
    }
};

} // namesapce python

} // namespace yay 
