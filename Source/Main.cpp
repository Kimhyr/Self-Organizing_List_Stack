#include "Symbol_Graph.h"

using DefaultGraph = Symbol_Graph<std::string, int, std::uint8_t>;
using DefaultSymbol = Symbol<std::string, int, std::uint8_t>;

bool f(DefaultSymbol const& sym) {
	return sym.key()[0] == '-';
}

int main() {
	int nums[] = { 1, 2, 3, 4};
	DefaultGraph graph(*new DefaultSymbol("", nums[0], 0), f);
	graph.enter_in(*new DefaultSymbol("poop", nums[1], 0), graph.root());
	graph.enter_in(*new DefaultSymbol("poop", nums[1], 0), graph.root());
	graph.enter_in(*new DefaultSymbol("-pee", nums[1], 2), graph.root());
	graph.enter_in(*new DefaultSymbol("pee", nums[1], 3), graph.root());
	return 0;
}
