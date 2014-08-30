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

#include <iostream>
#include <yay/yay_utf8.h>
#include <yay/yay_keymaps.h>

namespace yay
{
namespace km
{
	namespace
	{
		const char qwerty[] = "qwertyuiop[]asdfghjkl;'zxcvbnm,.QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>";
		const StrUTF8 ycuken ("йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ");

		const struct TableGenerator
		{
			enum { TableSize = 127 };

			typedef char RuLetter_t[2];
			RuLetter_t m_table[TableSize];

			TableGenerator ()
			: m_table{}
			{
				for (unsigned char i = 0; i < TableSize; ++i)
				{
					const char *pos = strchr(qwerty, i);

					if (pos)
						ycuken[pos - qwerty].copyToBufNoNull(m_table[i]);
					else
						m_table[i][0] = m_table[i][1] = 0;
				}
			}
		} TableGen;
	}

	bool engToRus(const char *eng, size_t size, std::string& rus)
	{
		bool converted = false;
		rus.reserve(size * 2);
		for (size_t i = 0; i < size; ++i)
		{
			const char ch = eng[i];
			const auto letter = TableGen.m_table[static_cast<size_t>(ch < 127 && ch > 0 ? ch : 0)];
			if (!*reinterpret_cast<const uint16_t*>(letter))
				continue;

			rus.append(letter, 2);
			converted = true;
		}

		return converted;
	}
}
}
