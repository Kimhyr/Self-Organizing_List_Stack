#pragma once

#include <concepts>

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
	static constexpr Bias_Type const& BIAS_CAPACITY = 5;

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

private:
	Key_T key_;
	Value_T value_;
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
	void decrease_bias() noexcept;
};

template<std::equality_comparable Key_T, typename Value_T>
class Symbol_Table {
private:
};
