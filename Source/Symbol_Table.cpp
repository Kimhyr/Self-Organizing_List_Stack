#include "Symbol_Table.h"

template<std::equality_comparable Key_T, typename Value_T>
Symbol_Table<Key_T, Value_T>::~Symbol_Table() noexcept {
	delete this->root_;
}

template<std::equality_comparable Key_T, typename Value_T>
void Symbol_Table<Key_T, Value_T>::enter_in(Symbol_Type& entry, Symbol_Type* parent) {
	if (!parent) {
		this->root_ = &entry;
		return;
	}
	Symbol_Type* child = parent->child_;
	if (!child) {
		parent->child_ = &entry;
		return;
	}
	for (;;) {
		if (child->key() == entry.key())
			throw std::invalid_argument(__FUNCTION__);
		if (!child->next())
			break;
		child = child->next_;
	}
	do {
		if (child->bias() <= entry.bias()) {
			child = child->prior_;
			continue;
		}
		child->append(entry);
		return;
	} while (child->prior());
	child->prepend(entry);
}

template<std::equality_comparable Key_T, typename Value_T> auto Symbol_Table<Key_T, Value_T>::
access_from(Key_Type const& key, Symbol_Type const& parent) -> Value_Type& {
	Symbol_Type* curr = parent.child_; 
	for (; curr; curr = curr->next_) {
		if (curr->key() != key)
			continue;
		return curr->value();
	}
	throw std::invalid_argument(__FUNCTION__);
}

template<std::equality_comparable Key_T, typename Value_T>
Symbol<Key_T, Value_T>::~Symbol() noexcept {
	this->detach();
	while (this->child() != nullptr)
		delete this->child_;
}

template<std::equality_comparable Key_T, typename Value_T>
void Symbol<Key_T, Value_T>::nullify_biases() noexcept {
	for (Symbol* curr = this->child_; curr; curr = curr->next_)
		curr->bias_ = 0;
}

template<std::equality_comparable Key_T, typename Value_T>
void Symbol<Key_T, Value_T>::detach() noexcept {
	if (this->prior())
		this->prior_->next_ = this->next_;
	else if (this->parent())
		this->parent_->child_ = this->next_;
	if (this->next())
		this->next_->prior_ = this->prior_;
}

template<std::equality_comparable Key_T, typename Value_T>
void Symbol<Key_T, Value_T>::prepend(Symbol& other) noexcept {
	other.prior_ = this->prior_;
	other.next_ = this;
	if (this->prior())
		this->prior_->next_ = &other;
	else this->parent_->child_ = &other;
	this->prior_ = &other;
} 

template<std::equality_comparable Key_T, typename Value_T>
void Symbol<Key_T, Value_T>::append(Symbol& other) noexcept {
	other.prior_ = this;
	other.next_ = this->next_;
	if (this->next())
		this->next_->prior_ = &other;
	this->next_ = &other;
}

template<std::equality_comparable Key_T, typename Value_T>
void Symbol<Key_T, Value_T>::increase_bias() noexcept {
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
