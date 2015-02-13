#include "WordsSeacher.h"
using namespace std;

int main() {
	ifstream text_file("text.txt");
	Text text(text_file);
	for (;;) {
		string range;
		cout << "input range or dot to exit: ";
		if (!(cin >> range) || range == ".") break;
		text.print_range(cout, range);
	}
	return 0;
}

