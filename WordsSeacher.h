#ifndef WORDSSEACHER
#define WORDSSEACHER

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using std::istream;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istringstream;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::copy_if;
using std::for_each;


class Text {
public:
	explicit Text(istream &_input);
	// function to print range of items from map to output file
	void print_range(ostream &_out_stream, const string &_range) const;
private:
	// map conteining word and vector of lines where this word was found
	map<string, vector<unsigned>> words;
};

Text::Text(istream &_input) {
	string cur_line;
	unsigned line_number = 0;
	while (getline(_input, cur_line)) {
		istringstream in_str_stream(cur_line);
		string cur_word;
		while (in_str_stream >> cur_word) {
			string processed_word;
			copy_if(cur_word.begin(), cur_word.end(), back_inserter(processed_word), isalpha);
			for_each(processed_word.begin(), processed_word.end(), [](char &_c){_c = tolower(_c); });
			if (!cur_word.empty())
				words[processed_word].push_back(line_number);
		}
		++line_number;
	}
}

void Text::print_range(ostream &_out_stream, const string &_range) const {
	_out_stream << _range << ":\n\n";
	string begin_word, end_word;
	string::const_iterator iter_dash = find(_range.cbegin(), _range.cend(), '-');
	if (iter_dash == _range.cend()) {
		begin_word = end_word = _range;
	}
	else {
		begin_word.assign(_range.cbegin(), iter_dash);
		end_word.assign(++iter_dash, _range.cend());
	}
	++end_word[end_word.size() - 1];
	if (end_word >= begin_word) {
		auto begin_iter = words.lower_bound(begin_word);
		auto end_iter = words.lower_bound(end_word);
		if (begin_iter != end_iter) {
			while (begin_iter != end_iter) {
				_out_stream << begin_iter->first << " : ";
				_out_stream << begin_iter->second.size();
				_out_stream << ((begin_iter->second.size() == 1) ? " time. " : " times. ");
				for (const unsigned u : begin_iter->second)
					_out_stream << u << " ";
				_out_stream << endl;
				++begin_iter;
			}
		}
		else {
			_out_stream << "empty range.\n";
		}
	}
	else {
		_out_stream << "incorrect range!!!\n";
	}
	_out_stream << "\n\n";
}

#endif
