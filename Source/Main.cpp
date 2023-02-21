#include "Symbol_Table.h"

#include <string>
#include <utility>
#include <iostream>

using namespace Klang;
using Table_Type = Symbol_Table<std::string, char const&>;
using Symbol_Type = Table_Type::Symbol_Type;

int main() {
	char values[] = {'p', 'e', 'e'};
	Table_Type table;
	table.enter(*new Symbol_Type("ROOT", values[0]));
	auto sym = new Symbol_Type("CHILD", values[1], table.root());
	table.enter(*sym);
	
	char const& value = table.access("CHILD", table.root());
	std::cout << "RETURNED VALUE: " << value << '\n';
	return 0;
}
