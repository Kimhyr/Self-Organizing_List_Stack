#pragma once

#include <stdexcept>
#include <string_view>

template<typename Symbol_T>
class Symbol_Tree {
public:
	class Node {
		friend class Symbol_Tree;
	
	public:
		Node() = default;
		constexpr Node(char const* identifier, std::uint8_t bias) noexcept
			: identifier_(identifier), bias_(bias) {}
		constexpr Node(char const* identifier, std::uint8_t bias, Symbol_T& symbol) noexcept
			: identifier_(identifier), bias_(bias), symbol_(&symbol) {}
		
		~Node();
	
	public:
		constexpr Node const* prior() const noexcept { return this->prior_; }
		constexpr Node const* next() const noexcept { return this->next_; }
		constexpr std::string_view const& identifier() const noexcept { return this->identifier_; }
		constexpr Node const* parent() const noexcept { return this->parent_; }
		constexpr Node const* child() const noexcept { return this->child_; }
		constexpr std::uint8_t bias() const noexcept { return this->bias_; }
		constexpr Symbol_T* symbol() const noexcept { return this->symbol_; }
	
	private:
		Node* prior_ = nullptr;
		Node* next_ = nullptr;
		std::string_view identifier_ = nullptr;
		Node* parent_ = nullptr;
		Node* child_ = nullptr;
		std::uint8_t bias_ = 0;
		Symbol_T* symbol_ = nullptr;
	
	private:
		void append(Node& next) noexcept;
		void prepend(Node& prior) noexcept;
	};

public:
	Symbol_T* search_from(Node const& parent, std::string_view const& query);

	
	void insert(Node& node, Node& parent, bool is_super = false) noexcept;

public:
	Node* root_ = nullptr;
	Node* current_ = nullptr;
	Node* super_ = nullptr;
};
