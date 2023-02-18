#pragma once

#include <stdexcept>
#include <string_view>
#include <functional>

template<typename Symbol_T>
class Symbol_Tree {
public:
	class Entry {
		friend class Symbol_Tree;
	
	public:
		Entry() = default;
		constexpr Entry(char const* identifier, std::uint8_t bias) noexcept
			: identifier_(identifier), bias_(bias) {}
		constexpr Entry(char const* identifier, std::uint8_t bias, Symbol_T& symbol) noexcept
			: identifier_(identifier), bias_(bias), symbol_(&symbol) {}
		
		~Entry();
	
	public:
		constexpr Entry const* prior() const noexcept { return this->prior_; }
		constexpr Entry const* next() const noexcept { return this->next_; }
		constexpr std::string_view const& identifier() const noexcept { return this->identifier_; }
		constexpr Entry const* parent() const noexcept { return this->parent_; }
		constexpr Entry const* child() const noexcept { return this->child_; }
		constexpr std::uint8_t bias() const noexcept { return this->bias_; }
		constexpr Symbol_T* symbol() const noexcept { return this->symbol_; }
	
	private:
		Entry* prior_ = nullptr;
		Entry* next_ = nullptr;
		std::string_view identifier_ = nullptr;
		Entry* parent_ = nullptr;
		Entry* child_ = nullptr;
		std::uint8_t bias_ = 0;
		Symbol_T* symbol_ = nullptr;
	
	private:
		void append(Entry& next) noexcept;
		void prepend(Entry& prior) noexcept;
		void detach() noexcept;

		void increase_bias() noexcept;

		void decrease_bias() noexcept;
	};

public:
	void insert(Entry& node, Entry& parent, bool is_super = false);
	
	Symbol_T* search_from(Entry const& parent, std::string_view const& query);

public:
	Entry* root_ = nullptr;
	Entry* current_ = nullptr;
	Entry* super_ = nullptr;
	std::function<bool(Entry const&)> const* entry_can_decrease_bias;
};
