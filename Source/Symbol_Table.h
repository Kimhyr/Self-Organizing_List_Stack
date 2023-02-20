#pragma once

#include <concepts>
#include <stdexcept>

#include <iostream>

namespace Klang {

template<std::equality_comparable Key_T, typename Value_T>
class Symbol_Table {
public:
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
		
		constexpr Symbol(Key_Type const& key, Value_Type& value, Bias_Type bias = 0)
			: key_(key), value_(value), bias_(bias), parent_(nullptr),
			  child_(nullptr), prior_(nullptr), next_(nullptr) {}
	
		~Symbol() noexcept {
			this->detach();
			std::cout << "NEXT CHILD: " << this->child()->next() << '\n';
			while (this->child() != nullptr)
				delete this->child_;
		}
	
	public:
		constexpr Key_T const& key() const noexcept { return this->key_; }
		constexpr Value_T& value() noexcept { return this->value_; }
	
		constexpr Bias_Type bias() const noexcept { return this->bias_; }
		
		constexpr Symbol const* parent() const noexcept { return this->parent_; }
		constexpr Symbol const* child() const noexcept { return this->child_; }
		
		constexpr Symbol const* prior() const noexcept { return this->prior_; }
		constexpr Symbol const* next() const noexcept { return this->next_; }
	
	public:
		void nullify_biases() noexcept {
			for (Symbol* curr = this->child_; curr; curr = curr->next_)
				curr->bias_ = 0;
		}
	
	private:
		Key_Type key_;
		Value_Type& value_;
		Bias_Type bias_;
		Symbol* parent_ = nullptr;
		Symbol* child_ = nullptr;
		Symbol* prior_ = nullptr;
		Symbol* next_ = nullptr;
	
	private:
		void detach() noexcept {
			if (this->prior() != nullptr)
				this->prior_->next_ = this->next_;
			else if (this->parent() != nullptr)
				this->parent_->child_ = this->next_;
			if (this->next() != nullptr)
				this->next_->prior_ = this->prior_;
		}
		
		void prepend(Symbol& other) noexcept {
			other.prior_ = this->prior_;
			other.next_ = this;
			if (this->prior() != nullptr)
				this->prior_->next_ = &other;
			else this->parent_->child_ = &other;
			this->prior_ = &other;
		}
		
		void append(Symbol& other) noexcept {
			other.prior_ = this;
			other.next_ = this->next_;
			if (this->next() != nullptr)
				this->next_->prior_ = &other;
			this->next_ = &other;
		}
	
		void increase_bias() noexcept {
			++this->bias_;
			Symbol* curr = this->prior_;
			if (curr == nullptr)
				return;
			this->detach();
			do {
				if (curr->bias() == this->bias()) {
					curr = curr->prior_;
					continue;
				}
				curr->append(*this);
				return;
			} while (curr != nullptr);
			curr->prepend(*this);
		}
	};

public:
	using Key_Type = Key_T;
	using Value_Type = Value_T;
	using Symbol_Type = Symbol;

public:
	constexpr Symbol_Table() = default;
	Symbol_Table(Symbol&&) = delete;
	Symbol_Table(Symbol const&) = delete;

	Symbol_Table& operator=(Symbol_Table&&) = delete;
	Symbol_Table& operator=(Symbol_Table const&) = delete;
	
	~Symbol_Table() noexcept {
		delete this->root_;
	}

public:
	constexpr Symbol* root() noexcept { return this->root_; }

public:
	void enter(Symbol& entry, Symbol* parent = nullptr) {
		if (!parent) {
			this->root_ = &entry;
			return;
		}
		Symbol* child = parent->child_;
		if (!child) {
			parent->child_ = &entry;
			return;
		}
		for (;; child = child->next_) {
			if (child->key() == entry.key())
				throw std::invalid_argument(__FUNCTION__);
			if (!child->next())
				break;
		}
		// We are now at the last child.
		// Reverse to find biased position.
		entry.parent_ = parent;
		std::cout << "parent: " << parent << '\n';
		for (;; child = child->prior_) {
			if (child->bias() <= entry.bias()) {
				if (!child->prior())
					break;
				continue;
			}
			child->append(entry);
			return;
		}
		child->prepend(entry);
	}

	Value_Type& access_from(Key_Type const& key, Symbol const& parent) {
		Symbol* curr = parent.child_; 
		for (; curr; curr = curr->next_) {
			if (curr->key() != key)
				continue;
			return curr->value();
		}
		throw std::invalid_argument(__FUNCTION__);
	}

private:
	Symbol* root_ = nullptr;
};

template<std::equality_comparable Key_T, typename Value_T>
using Symbol = typename Symbol_Table<Key_T, Value_T>::Symbol;

}
