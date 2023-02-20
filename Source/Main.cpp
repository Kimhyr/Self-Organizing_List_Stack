#include "Symbol_Table.h"

#include <string>
#include <utility>
#include <iostream>

using namespace Klang;
using Table_Type = Symbol_Table<int, char>;
using Symbol_Type = Table_Type::Symbol_Type;

int main() {
	char* values = new char[32];
	values[0] = 'p';
	values[1] = 'e';
	values[2] = 'e';
	
	Table_Type table;
	
	// Set the root.
	table.enter(*new Symbol_Type(0, values[0]));
	
	// This entry should be a child of the root with no next and prior.
	table.enter(*new Symbol_Type(21, values[0]), table.root());

	// table.enter(*new Symbol_Type(2, values[1]), table.root());
	// table.enter(*new Symbol_Type(3, values[2]), table.root());
	char& value = table.access_from(21, *table.root());
	std::cout << "RETURNED VALUE: " << value << '\n';
	if (value == values[0])
		return 0;
	return 1;
}
