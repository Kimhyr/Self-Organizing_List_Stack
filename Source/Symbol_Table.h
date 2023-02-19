#pragma once

#include <concepts>
#include <stdexcept>

template<std::equality_comparable Key_T, typename Value_T>
class Symbol_Table;

template<std::equality_comparable Key_T, typename Value_T>
class Symbol {
	friend class Symbol_Table<Key_T, Value_T>;

public:

	using Key_Type = Key_T;
	using Value_Type = Value_T;
	using Bias_Type = unsigned char;

public:
	Symbol() = delete;
	Symbol(Symbol&&) = delete;
	Symbol(Symbol const&) = delete;
	
	Symbol& operator=(Symbol&&) = delete;
	Symbol& operator=(Symbol const&) = delete;
	
	constexpr Symbol(Key_Type key, Value_Type& value, Bias_Type bias)
		: key_(key), value_(value), bias_(bias) {}
	constexpr Symbol(Key_Type&& key, Value_Type& value, Bias_Type bias)
		: key_(key), value_(value), bias_(bias) {}
	constexpr Symbol(Key_Type const& key, Value_Type& value, Bias_Type bias)
		: key_(key), value_(value), bias_(bias) {}
	
	constexpr Symbol(Key_Type key, Value_Type& value, Bias_Type bias, Symbol& parent)
		: key_(key), value_(value), bias_(bias), parent_(parent) {}
	constexpr Symbol(Key_Type&& key, Value_Type& value, Bias_Type bias, Symbol& parent)
		: key_(key), value_(value), bias_(bias), parent_(parent) {}
	constexpr Symbol(Key_Type const& key, Value_Type& value, Bias_Type bias, Symbol& parent)
		: key_(key), value_(value), bias_(bias), parent_(parent) {}

	~Symbol() noexcept;

public:
	constexpr Key_T const& key() const noexcept { return this->key_; }
	constexpr Value_T& value() const noexcept { return this->value_; }

	constexpr Bias_Type bias() const noexcept { return this->bias_; }
	
	constexpr Symbol const* parent() const noexcept { return this->parent_; }
	constexpr Symbol const* child() const noexcept { return this->child_; }
	
	constexpr Symbol const* next() const noexcept { return this->next_; }
	constexpr Symbol const* prior() const noexcept { return this->prior_; }

public:
	void nullify_biases() noexcept;

private:
	Key_T key_;
	Value_T& value_;
	Bias_Type bias_;
	Symbol* parent_ = nullptr;
	Symbol* child_ = nullptr;
	Symbol* next_ = nullptr;
	Symbol* prior_ = nullptr;

private:
	void detach() noexcept;
	void prepend(Symbol& other) noexcept;
	void append(Symbol& other) noexcept;

	void increase_bias() noexcept;
};

template<std::equality_comparable Key_T, typename Value_T>
class Symbol_Table {
public:
	using Key_Type = Key_T;
	using Value_Type = Key_T;
	using Symbol_Type = Symbol<Key_Type, Value_Type>;

public:
	constexpr Symbol_Table() = default;
	Symbol_Table(Symbol_Type&&) = delete;
	Symbol_Table(Symbol_Type const&) = delete;

	Symbol_Table& operator=(Symbol_Table&&) = delete;
	Symbol_Table& operator=(Symbol_Table const&) = delete;
	
	~Symbol_Table() noexcept;

public:
	constexpr Symbol_Type const& root() const noexcept { return this->root_; }

public:
	void enter_in(Symbol_Type& symbol, Symbol_Type* parent);

	Value_Type& access_from(Key_Type const& key, Symbol_Type const& parent);

private:
	Symbol_Type* root_ = nullptr;
};
