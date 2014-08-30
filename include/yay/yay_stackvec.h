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

#include <vector>
#include <iostream>

namespace yay
{
	template<typename T, template<typename TT, typename Alloc> class Cont = std::vector, typename Alloc = std::allocator<T>>
	class StackVec
	{
		typedef Cont<T, Alloc> ContClass;
		enum
		{
			ObjSize = sizeof(ContClass),
			SSize = ObjSize / sizeof(T)
		};

		size_t m_size;
		union U
		{
			char m_rawData[ObjSize];
			T m_objs[SSize];
			
			U() {}
		} m_u;
	protected:
		inline const ContClass* cont() const { return reinterpret_cast<const ContClass*>(m_u.m_rawData); }
		inline ContClass* cont() { return reinterpret_cast<ContClass*>(m_u.m_rawData); }
		
		inline void dealloc() { cont()->~ContClass(); }

		inline bool isEx() const { return m_size > SSize; }
	public:
		StackVec()
		: m_size(0)
		{
		}

		~StackVec()
		{
			if (isEx())
				dealloc();
		}

		StackVec(const StackVec& sv)
		: m_size(0)
		{
			*this = sv;
		}

		StackVec& operator=(const StackVec& sv)
		{
			if (isEx())
				dealloc();

			m_size = sv.m_size;
			if (isEx())
				new (m_u.m_rawData) ContClass(*sv.cont());
			else
				std::copy(sv.m_u.m_objs, sv.m_u.m_objs + m_size, m_u.m_objs);

			return *this;
		}

		void push_back(const T& t)
		{
			if (m_size < SSize)
				m_u.m_objs[m_size] = t;
			else if (m_size > SSize)
				cont()->push_back(t);
			else
			{
				ContClass c;
				std::copy(m_u.m_objs, m_u.m_objs + SSize, std::back_inserter(c));
				new (m_u.m_rawData) ContClass(c);
				cont()->push_back(t);
			}

			++m_size;
		}

		void pop_back()
		{
			if (m_size > SSize + 1)
				cont()->pop_back();
			else if (m_size == SSize + 1)
			{
				cont()->pop_back();
				const ContClass c = *cont();
				dealloc();
				std::copy(c.begin(), c.end(), m_u.m_objs);
			}

			--m_size;
		}

		inline size_t size() const { return m_size; }

		inline T& operator[](size_t pos)
		{
			return isEx() ? (*cont())[pos] : m_u.m_objs[pos];
		}

		inline const T& operator[](size_t pos) const
		{
			return isEx() ? (*cont())[pos] : m_u.m_objs[pos];
		}

		// I hate myself for doing this.
		inline const T* getRawBuf() const { return isEx() ? &(*cont())[0] : m_u.m_objs; }
		inline T* getRawBuf() { return isEx() ? &(*cont())[0] : m_u.m_objs; }
	};
} // namespace yay
