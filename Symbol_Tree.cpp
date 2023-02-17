#include "Symbol_Tree.h"

template<typename T>
void Symbol_Tree<T>::insert(Node& node, Node& parent) {
	Node* current = parent.child_;
	if (!current) {
		parent.child_ = &node;
		return;
	}
	for (; current->next(); current = current->next_) {
		int cmp = current->identifier().compare(node.identifier());
		if (cmp > 0)
			break;
		else if (cmp == 0)
			throw std::invalid_argument("Identifier already exists in the parent node.");
	}
	node.prior_ = current;
	node.next_ = current->next_;
	current->next_ = &node;
}

template<typename T>
T* Symbol_Tree<T>::search_from(Node const& parent, std::string_view const& query) {
	for (Node const* child = parent.child(); child; child = child->next()) {
		if (query.length() != child->identifier().length())
			continue;
		int cmp = child->identifier().compare(query);
		if (cmp > 0)
			return nullptr;
		if (cmp == 0)
			return child->symbol();
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
