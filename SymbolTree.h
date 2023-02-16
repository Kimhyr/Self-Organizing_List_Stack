#pragma once

#include <stdexcept>
#include <string_view>

template<typename Symbol_T>
class SymbolTree {
private:
	class Node {
		friend class SymbolTree;
	
	public:
		Node() = default;

		constexpr Node(char const* identifier) noexcept
			: identifier_(identifier) {}
		constexpr Node(char const* identifier, Symbol_T& symbol) noexcept
			: identifier_(identifier), symbol_(&symbol) {}
		
		~Node();
	public:
		constexpr Node const* prior() const noexcept { return this->prior_; }
		constexpr Node const* next() const noexcept { return this->next_; }
		constexpr std::string_view const& identifier() const noexcept { return this->identifier_; }
		constexpr Node const* parent() const noexcept { return this->parent_; }
		constexpr Node const* child() const noexcept { return this->child_; }
		constexpr Symbol_T* symbol() const noexcept { return this->symbol_; }
	
	private:
		Node* prior_ = nullptr;
		Node* next_ = nullptr;
		std::string_view identifier_ = nullptr;
		Node* parent_ = nullptr;
		Node* child_ = nullptr;
		Symbol_T* symbol_ = nullptr;
	};

public:
	Symbol_T* search_from(Node const& parent, std::string_view const& query);
	
	void insert(Node& node, Node& parent);

public:
	Node* root_;

};

template<typename Symbol_T>
using SymbolTreeNode = typename SymbolTree<Symbol_T>::Node;
