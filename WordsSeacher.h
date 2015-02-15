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
	pair<string, string> do_borders(string _range) const;
	void do_print(ostream &_out_stream, map<string, vector<unsigned>>::const_iterator _b, map<string, vector<unsigned>>::const_iterator _e) const;
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

pair<string, string> Text::do_borders(string _range) const {
	string begin_word, end_word;
	string::const_iterator iter_dash = find(_range.cbegin(), _range.cend(), '-');
	if (iter_dash == _range.cend()) {
		begin_word = end_word = _range;
	}
	else {
		begin_word.assign(_range.cbegin(), iter_dash);
		end_word.assign(++iter_dash, _range.cend());
	}
	return make_pair(begin_word, end_word);
}

void Text::do_print(ostream &_out_stream, map<string, vector<unsigned>>::const_iterator _b, map<string, vector<unsigned>>::const_iterator _e) const {
	if (_b != _e) {
		while (_b != _e) {
			_out_stream << _b->first << " : ";
			_out_stream << _b->second.size();
			_out_stream << ((_b->second.size() == 1) ? " time. " : " times. ");
			for (const unsigned u : _b->second)
				_out_stream << u << " ";
			_out_stream << endl;
			++_b;
		}
	}
	else {
		_out_stream << "empty range.\n";
	}
}

void Text::print_range(ostream &_out_stream, const string &_range) const {
	_out_stream << _range << ":\n\n";
	auto borders = do_borders(_range);
	if (borders.second >= borders.first) {
		// right border if a little more than last word in the range
		++borders.second.back();
		auto begin_iter = words.lower_bound(borders.first);
		auto end_iter = words.lower_bound(borders.second);
		do_print(_out_stream, begin_iter, end_iter);
	}
	else {
		_out_stream << "incorrect range!!!\n";
	}
	_out_stream << "\n\n";
}

#endif
