#include "Symbol_Table.h"

template<std::equality_comparable Key_T, typename Value_T>
Symbol<Key_T, Value_T>::~Symbol() noexcept {
	this->detach();
	while (this->child() != nullptr)
		delete this->child_;
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
	if (this->bias() < BIAS_CAPACITY)
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

template<std::equality_comparable Key_T, typename Value_T>
void Symbol<Key_T, Value_T>::decrease_bias() noexcept {
	if (this->bias() > 0)
		--this->bias_;
	Symbol* curr = this->next_;
	if (curr == nullptr)
		return;
	this->detach();
	do {
		if (curr->bias() == this->bias()) {
			curr = curr->next_;
			continue;
		}
		curr->prepend(*this);
		return;
	} while (curr != nullptr);
	curr->append(*this);
}
