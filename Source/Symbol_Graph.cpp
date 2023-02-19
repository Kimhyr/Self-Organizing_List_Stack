#include "Symbol_Graph.h"

template<typename K, typename V, std::integral B>
void Symbol_Graph<K, V, B>::enter_in(Symbol& entry, Symbol& parent) {
	Symbol* curr = parent.child_;
	if (!curr) {
		parent.child_ = &entry;
		return;
	}
	for (;; curr = curr->next()) {
		if (curr->key() == entry.key())
			throw std::invalid_argument(__FUNCTION__);
		// THOUGHT: Multithread this?
		// 	* We can't move an entry when an entry is locked.
		//		- Unlock when an entry is--no.
		//	* Would the best thing to do be, if the entry is locked, increase the
		//	  requested bias?
		if (this->can_be_biasless(*curr))
			curr->decrease_bias();
		if (!curr->next())
			break;
	}
	for (;; curr = curr->prior_) {
		if (curr->bias() > entry.bias()) {
			curr->append(entry);
			break;
		} else if (curr->bias() <= entry.bias() ||
			   !curr->prior()) {
			curr->prepend(entry);
			break;
		}
	}
}

template<typename K, typename V, std::integral B>
Symbol_Graph<K, V, B>::~Symbol_Graph() {
	delete this->root_;
}

template<typename K, typename V, std::integral B>
V& Symbol_Graph<K, V, B>::get_from(Key_Type const& key, Symbol const& parent) {
	Symbol* curr = parent.child_;
	for (; curr; curr = curr->next_) {
		if (curr->key() == key) {
			curr->increase_bias();
			return curr->symbol();
		}
		if (this->can_be_biasless(*curr))
			curr->decrease_bias();
	}
	return nullptr;
}

template<typename K, typename V, std::integral B>
Symbol_Graph<K, V, B>::Symbol::~Symbol() {
	this->detach();
	while (this->child())
		delete this->child_;
	this->value_.~V();
}

template<typename K, typename V, std::integral B>
void Symbol_Graph<K, V, B>::Symbol::append(This& next) noexcept {
	next.prior_ = this;
	next.next_ = this->next_;
	this->next_ = &next;
}

template<typename K, typename V, std::integral B>
void Symbol_Graph<K, V, B>::Symbol::prepend(This& prior) noexcept {
	if (this == this->parent()->child())
		this->parent_->child_ = &prior;
	prior.next_ = this;
	prior.prior_ = this->prior_;
	this->prior_ = &prior;
}

template<typename K, typename V, std::integral B>
void Symbol_Graph<K, V, B>::Symbol::detach() noexcept {
	if (this->prior())
		this->prior_->next_ = this->next_;
	else if (this->parent() && this == this->parent()->child())
		this->parent_->child_ = this->next_;
	if (this->next())
		this->next_->prior_ = this->prior_;
}

template<typename K, typename V, std::integral B>
void Symbol_Graph<K, V, B>::Symbol::increase_bias() noexcept {
	if (this->bias() < 5)
		++this->bias_;
	This* e = this->prior_;
	if (!e)
		return;
	do {
		if (e->bias() == this->bias()) {
			e = e->prior_;
			continue;
		}
		if (e->next() == this)
			return;
		this->detach();
		e->append(*this);
		return;
	} while (e);
	e->prepend(*this);
}

template<typename K, typename V, std::integral B>
void Symbol_Graph<K, V, B>::Symbol::decrease_bias() noexcept {
	if (this->bias())
		--this->bias_;
	This* e = this->next_;
	if (!e)
		return;
	do {
		if (e->bias() == this->bias()) {
			e = e->next_;
			continue;
		}
		if (e->prior() == this)
			return;
		this->detach();
		e->prepend(*this);
	} while (e);
	e->append(*this);
}
