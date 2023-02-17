#include "Symbol_Tree.h"

template<typename T>
void Symbol_Tree<T>::insert(Node& node, Node& parent, bool is_super) noexcept {
	for (Node* current = parent.child_; current; current = current->next_) {
		if (node.bias() >= current->bias()) {
			current->prepend(node);
			goto Finish;
		}
		if (!current->next()) {
			current->append(node);
			goto Finish;
		}
	}
	parent.child_ = &node;
}

template<typename T>
T* Symbol_Tree<T>::search_from(Node const& parent, std::string_view const& query) {
	for (Node const* child = parent.child(); child; child = child->next()) {
		if (query.length() != child->identifier().length())
			continue;
		if (child->identifier().compare(query)) {
			++child->bias_;
			return child->symbol();
		}
	}
	return nullptr;
}

template<typename T>
Symbol_Tree<T>::Node::~Node() {
	if (this->prior)
		this->prior->next = this->next;
	else if (this->parent_)
		this->parent_->child_ = this->next_;
	if (this->next)
		this->next->prior = this->prior;
	while (this->child_)
		delete this->child_;
	delete this->symbol_;
}

template<typename T>
void Symbol_Tree<T>::Node::append(Node& next) noexcept {
	next.prior_ = this;
	next.next_ = this->next_;
	this->next_ = &next;
}

template<typename T>
void Symbol_Tree<T>::Node::prepend(Node& prior) noexcept {
	prior.next_ = this;
	prior.prior_ = this->prior_;
	this->prior_ = &prior;
}
