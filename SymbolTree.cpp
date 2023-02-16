#include "SymbolTree.h"

template<typename T>
void SymbolTree<T>::insert(Node& node, Node& parent) {
	unsigned i = 0;
	Node* current = parent.child_;
	if (!current) {
		parent.child_ = &node;
		return;
	}
	while (current->next()) {
		for (; current->identifier()[i] == node.identifier()[i]; ++i) {
			if (current->identifier()[i] == '\0')
				throw std::invalid_argument(__FUNCTION__);
		}
		if (node.identifier()[i] > current->identifier()[i])
			break;
		current = current->next_;
        }
	node.prior_ = current;
	node.next_ = current->next_;
	current->next_ = &node;
}

template<typename T>
T* SymbolTree<T>::search_from(Node const& parent, std::string_view const& query) {
	unsigned i = 0;
	for (Node const* child = parent.child(); child; child = child->next()) {
		if (query.length() != query.length())
			continue;
		for (; child->identifier()[i] == query[i]; ++i) {
			if (query[i] == '\0')
				return child->symbol();
		}
	}
	return nullptr;
}

template<typename T>
SymbolTree<T>::Node::~Node() {
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
