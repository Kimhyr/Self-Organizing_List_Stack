#include "Symbol_Tree.h"

template<typename T>
void Symbol_Tree<T>::insert(Entry& entry, Entry& parent, bool is_super) {
	Entry* curr = parent.child_;
	if (!curr) {
		parent.child_ = &entry;
		goto Finish;
	}
	for (;; curr = curr->next()) {
		if (curr->identifier() == entry.identifier())
			throw std::invalid_argument(__FUNCTION__);
		// NOTE: Multithread this? Maybe a task manager on a separate thread that
		// executes procedures on objects if the object is not mutex locked.
		// No. This could be a fun project.
		if (this->entry_can_decrease_bias(*curr))
			curr->decrease_bias();
		if (!curr->next())
			break;
	}
	for (;; curr = curr->prior_) {
		if (curr->bias() > entry.bias()) {
			curr->append(entry);
			break;
		} else if (curr->bias() <= entry.bias()) {
			curr->prepend(entry);
			break;
		}
	}
Finish:
	if (is_super)
		this->super_ = &entry;
}

template<typename T>
T* Symbol_Tree<T>::search_from(Entry const& parent, std::string_view const& query) {
	Entry* curr = parent.child_;
	for (; curr; curr = curr->next_) {
		if (curr->identifier == query) {
			curr->increase_bias();
			return curr->symbol();
		}
		if (this->entry_can_decrease_bias(*curr))
			curr->decrease_bias();
	}
	return nullptr;
}

template<typename T>
Symbol_Tree<T>::Entry::~Entry() {
	this->detach();
	while (this->child())
		delete this->child_;
	delete this->symbol_;
}

template<typename T>
void Symbol_Tree<T>::Entry::append(Entry& next) noexcept {
	next.prior_ = this;
	next.next_ = this->next_;
	this->next_ = &next;
}

template<typename T>
void Symbol_Tree<T>::Entry::prepend(Entry& prior) noexcept {
	if (this == this->parent()->child())
		this->parent_->child_ = &prior;
	prior.next_ = this;
	prior.prior_ = this->prior_;
	this->prior_ = &prior;
}

template<typename T>
void Symbol_Tree<T>::Entry::detach() noexcept {
	if (this->prior())
		this->prior_->next_ = this->next_;
	else if (this->parent() && this == this->parent()->child())
		this->parent_->child_ = this->next_;
	if (this->next())
		this->next_->prior_ = this->prior_;
}

template<typename T>
void Symbol_Tree<T>::Entry::increase_bias() noexcept {
	if (this->bias() < 5)
		++this->bias_;
	Entry* e = this->prior_;
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

template<typename T>
void Symbol_Tree<T>::Entry::decrease_bias() noexcept {
	if (this->bias())
		--this->bias_;
	Entry* e = this->next_;
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
