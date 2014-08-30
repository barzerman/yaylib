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
#include <string>
#include <map>
#include <boost/unordered_map.hpp>
#include "yay/yay_trie.h"
#include "yay/yay_utf8.h"

namespace yay
{
	typedef std::vector<std::string> StringList_t;

	namespace ASCII
	{
		class NGramModel
		{
			typedef boost::unordered_map<uint32_t, uint64_t> EncDict_t;
			EncDict_t m_encounters;
			uint64_t m_totalSize;
		public:
			NGramModel();

			void addWord(const char *word);
			void addWords(const StringList_t& text);

			double getProb(const char *word) const;
		};
	}

	namespace UTF8
	{
		class NGramModel
		{
			typedef boost::unordered_map<uint64_t, uint64_t> EncDict_t;
			EncDict_t m_encounters;

			uint64_t m_totalSize;
		public:
			NGramModel();

			void addWord(const char *word);
			void addWords(const StringList_t& text);

			double getProb(const char *word) const;
		};
	}

	template<typename Model>
	class BasicTopicModelMgr
	{
		typedef std::map<int, Model> ModelsDict_t;
		ModelsDict_t m_models;
	public:
		typedef Model ModelType_t;

		inline BasicTopicModelMgr()
		{
		}

		inline Model& getModel(int topic)
		{
			typename ModelsDict_t::iterator pos = m_models.find(topic);
			if (pos == m_models.end())
				pos = m_models.insert(std::make_pair(topic, Model())).first;
			return pos->second;
		}

		inline void getAvailableTopics(std::vector<int>& result, const std::vector<int>& filter ) const
        {
			for (typename ModelsDict_t::const_iterator i = m_models.begin(), end = m_models.end(); i != end; ++i) {
                if( std::find( filter.begin(), filter.end(), i->first ) != filter.end() ) {
				    result.push_back(i->first);
                }
            }
        }
		inline void getAvailableTopics(std::vector<int>& result) const
		{
			for (typename ModelsDict_t::const_iterator i = m_models.begin(), end = m_models.end(); i != end; ++i)
				result.push_back(i->first);
		}
		
		inline size_t getNumTopics() const
		{
			return m_models.size();
		}
	};

	typedef BasicTopicModelMgr<UTF8::NGramModel> TopicModelMgr;
	
	typedef BasicTopicModelMgr<UTF8::NGramModel> UTF8TopicModelMgr;
	typedef BasicTopicModelMgr<ASCII::NGramModel> ASCIITopicModelMgr;

	/** If smartAsciiCheck is set to true, then this function first checks if
	 * the string contains non-ASCII characters. If any non-ascii characters,
	 * are found, then no models from ascii manager are queried.
	 */
	void evalAllLangs(UTF8TopicModelMgr *utf8, ASCIITopicModelMgr *ascii,
			const char *str, std::vector<std::pair<int, double> >& probs, bool smartAsciiCheck = false);

	inline void getScores(UTF8::NGramModel& utf8, ASCII::NGramModel& ascii, const char *str, size_t len, double& utf8Score, double& asciiScore)
	{
		const char *ptr = str;
		while (ptr < str + len && *ptr)
			if (*ptr++ > 127)
			{
				utf8Score = 1;
				asciiScore = 0;
				return;
			}

		utf8Score = utf8.getProb(str);
		asciiScore = ascii.getProb(str);
	}
}
