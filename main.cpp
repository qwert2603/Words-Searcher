#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

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

string make_word(const string &_word) {	// function modifying word to get rid of dots, commas and so on
	string res;	// resulted string
	for(char c : _word)	// check every symbol
		if (islower(c) || isupper(c) || c == '\'')	// if it is a letter or apostrophe
			res.push_back(tolower(c));	// add symbol to resulted string
	return res;	// everything is good, return resulted word
}

// function to print range of items from map to output file
void print_range(map<string, vector<unsigned>> &_words, const string &_range, ostream &_out_stream) {
	_out_stream << _range << ":\n\n";	// print range itself
	string begin_word, end_word;	// words -- bounds of range
	// iterator to '-' symbol or to _range.end() if _range doesn't contain '-'
	string::const_iterator iter_dash = find(_range.cbegin(), _range.cend(), '-');
	if (iter_dash == _range.cend()) {	// if _range doesn't contain '-'. For example, range is "sc"
		begin_word = end_word = _range;	// both bounds of range are like _rande
	}
	else {	// if _range  contain '-'. For example, range is "sc-wi"
		begin_word.assign(_range.cbegin(), iter_dash);	// left border of range. "sc" from "sc-wi"
		end_word.assign(++iter_dash, _range.cend());	// right border of range.
	}
	// right border should be little more, because it is a word that more than the bigges word in the range
	++end_word[end_word.size() - 1];
	if (end_word >= begin_word) {	// if right border is equal or more than left one
		// get iterators to first and past-the-end elements in the range
		auto begin_iter = _words.lower_bound(begin_word);
		auto end_iter = _words.lower_bound(end_word);
		if(begin_iter != end_iter) {
			while (begin_iter != end_iter) {	// for all elements from range
				_out_stream << begin_iter->first << " : ";	// print word itself
				_out_stream << begin_iter->second.size();	// print how many times word was found
				_out_stream << ((begin_iter->second.size() == 1) ? " time. " : " times. ");
				for (const unsigned u : begin_iter->second)
					_out_stream << u << " ";	// print numbers of lines where this word was found
				_out_stream << endl;
				++begin_iter;
			}
		}
		else {
			_out_stream << "empty range.\n";
		}
	}
	else {	// borders are not OK
		_out_stream << "incorrect range!!!\n";	// tell about error
	}
	_out_stream << "\n\n";
}

int _tmain(int argc, _TCHAR* argv[]) {
	cout << "Input ranges: ";
	ifstream text_file("text.txt");	// text file for processing
	ofstream output_file("result.txt");	// text file for result printing
	if (text_file) {	// if opening file was successful
		// map conteining word and vector of lines where this word was found
		map<string, vector<unsigned>> words;
		string cur_line;	// current processed line of text
		unsigned line_number = 0;	// number of current processed line of text
		while (getline(text_file, cur_line)) {	// for every line in text file
			istringstream in_str_stream(cur_line); // for line processing
			string cur_word;	// current processed word
			while (in_str_stream >> cur_word) {	// for every word in line
				cur_word = make_word(cur_word);	// modify word to get rid of dots, commas and so on
				if (!cur_word.empty())	// if it is real word
					// add word to map if it wasn't there before and add line number to appropriate vector
					words[cur_word].push_back(line_number);
			}	// end of current line processing
			++line_number;	// increment number of processed line
		}	// end of input file processing
		text_file.close();	// close input file stream, we don't need it anymore
		string ranges;	// line conteining some ragnes
		getline(cin, ranges);	// get this ranges from user
		istringstream ranges_stream(ranges);	// for ranges processing
		string range;	// line containing one range
		while (ranges_stream >> range)	// for every range
			print_range(words, range, output_file);	// print range to output file
		cout << "\ndone\n\n";
		cin.get();
	}
	else { // if opening file wasn't successful
		// tell about error
		output_file << "\"text.txt\" not found!\n";
		cout << "\"text.txt\" not found!\n";
	}
	return 0; // the end
}
