#pragma once
#include <iostream>

using namespace std;

constexpr auto black = 0;
constexpr auto red = 1;

template <class T> struct RBNode {

	T data; // holds the key
	RBNode* parent; // pointer to the parent
	RBNode* left; // pointer to left child
	RBNode* right; // pointer to right child
	bool color; // 1 -> Red, 0 -> Black

};

template <class T> class RBTree {

public:

	RBNode<T>* root;
	RBNode<T>* TNULL;

	RBTree();
	RBNode<T>* search(T);
	bool search(T, T, vector<RBNode<T>*>&); // for range search (inclusive)
	RBNode<T>* helperSeacrh(RBNode<T>*, T);
	void rangeSearchHelper(RBNode<T>*, T, T, vector<RBNode<T>*>&); // will return vector of nodes in the argument, in the given range
	void initializeNULLNode(RBNode<T>*, RBNode<T>*);
	bool insert(T);
	bool fixInsert(RBNode<T>*);
	void rightRotate(RBNode<T>*);
	void leftRotate(RBNode<T>*);
	void print();
	void printHelper(RBNode<T>*, string, bool);
	void deleteNode(T data);
	void deleteNodeHelper(RBNode<T>*, T);
	void fixDelete(RBNode<T>*);
	void transplant(RBNode<T>*, RBNode<T>*);
	RBNode<T>* minimum(RBNode<T>*);

};

template <class T> RBTree<T>::RBTree() {

	// empty tree so the root is pointing to the nill node
	TNULL = new RBNode<T>;
	TNULL->color = 0;
	TNULL->left = nullptr;
	TNULL->right = nullptr;
	root = TNULL;

}

template<class T> RBNode<T>* RBTree<T>::search(T key) {

	RBNode<T>* result = nullptr;
	RBNode<T>* current = root;
	while (current != TNULL) {
		// loop until it gets to nill leaf
		if (key == current->data) {
			result = current;
			return result;
		}
		else if (key < current->data)
			current = current->left;
		else current = current->right;
	}

	return result;

}

template<class T> bool RBTree<T>::search(T from, T to, vector<RBNode<T>*>& ans) {

	rangeSearchHelper(root, from, to, ans);

	if (ans.size() == 0) return false;

	return true;

}


template<class T> void RBTree<T>::rangeSearchHelper(RBNode<T>* current, T from, T to, vector<RBNode<T>*>& ans) {

	if (current == TNULL) return;

	if (current->data < from && current->data < to) // for exclusive search: current->data <= from && current->data < to
		rangeSearchHelper(current->right, from, to, ans);
	else if (current->data > from && current->data > to)
		rangeSearchHelper(current->left, from, to, ans); // for exclusive search: current->data > from && current->data >= to
	else {
		// it lies in the range, check for its left and right child and also store it
		rangeSearchHelper(current->left, from, to, ans);
		ans.push_back(current);
		rangeSearchHelper(current->right, from, to, ans);
	}

}


template <class T> void RBTree<T>::initializeNULLNode(RBNode<T>* node, RBNode<T>* parent) {

	// the leaf node
	node->data = 0;
	node->parent = parent;
	node->left = nullptr;
	node->right = nullptr;
	node->color = 0;

}

template <class T> bool RBTree<T>::insert(T key) {

	// insert the key to the tree in its appropriate position
	// Ordinary Binary Search Insertion
	RBNode<T>* node = new RBNode<T>;
	node->parent = nullptr;
	node->data = key;
	node->left = TNULL;
	node->right = TNULL;
	node->color = red; // new node must be red

	bool duplicate = false;
	RBNode<T>* result = search(key);
	if (result != nullptr) {
		duplicate = true;
		return false;
	}

	// its not a duplicate entry, do insertion

	RBNode<T>* prev = nullptr;
	RBNode<T>* current = this->root;

	while (current != TNULL) {
		//loop until it gets to nill leaf
		prev = current;
		if (node->data < current->data) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	// prev is parent of current
	node->parent = prev;
	if (prev == nullptr) {
		root = node;
	}
	else if (node->data < prev->data) {
		prev->left = node;
	}
	else {
		prev->right = node;
	}

	// if new node is a root node, simply return
	if (node->parent == nullptr) {
		node->color = black;
		return true;
	}

	// if the grandparent is null, simply return
	if (node->parent->parent == nullptr) {
		return true;
	}

	// Fix the tree
	fixInsert(node);

	return true;

}

template <class T> void RBTree<T>::leftRotate(RBNode<T>* x) {

	RBNode<T>* y = x->right;
	x->right = y->left;
	if (y->left != TNULL) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == nullptr) {
		this->root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;

}

template <class T> void RBTree<T>::rightRotate(RBNode<T>* x) {

	RBNode<T>* y = x->left;
	x->left = y->right;
	if (y->right != TNULL) {
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == nullptr) {
		this->root = y;
	}
	else if (x == x->parent->right) {
		x->parent->right = y;
	}
	else {
		x->parent->left = y;
	}

	y->right = x;
	x->parent = y;

}

template <class T> bool RBTree<T>::fixInsert(RBNode<T>* violatedNode) {

	RBNode<T>* uncle;

	while (violatedNode->parent->color == red) {
		// while there is a red to red conflict, continue this loop

		if (violatedNode->parent == violatedNode->parent->parent->right) {
			// if node's parent == node's grandparent's right then uncle should be grandparent's left

			uncle = violatedNode->parent->parent->left; // uncle
			// parent is on the right side and uncle is on the left side

			if (uncle->color == red) {
				// case 3.1
				// both parent and uncle red, simply recolour parent, uncle and grandparent

				uncle->color = black;
				violatedNode->parent->color = black;
				violatedNode->parent->parent->color = red; //painting red because the grandparent could not have been black in the first place to avoid red-red conflict
				violatedNode = violatedNode->parent->parent; //now move to grandparent (go up) to recusively check for red-red conflict if it exists 
			}
			else {
				//parent is red but uncle is black 

				if (violatedNode == violatedNode->parent->left) {
					// case 3.2.2
					// parent was on the right side as established above and insertion is done on the left side so RL rotation
					violatedNode = violatedNode->parent;
					// we have first moved the violatedNode, first because rotation was to be done on the parent
					// and SECONDALLY this node would get down after the rotation, and for the next part we need to 
					// do rotation on grandparent in 3.2.1 case as well so this just sits perfectly
					rightRotate(violatedNode);
				}
				// case 3.2.1
				violatedNode->parent->color = black;
				violatedNode->parent->parent->color = red;
				// update the node's colour involved in rotation, i.e the parent and grandparent
				leftRotate(violatedNode->parent->parent);
			}
		}
		else {
			uncle = violatedNode->parent->parent->right; // uncle

			if (uncle->color == red) {
				// mirror case 3.1
				uncle->color = black;
				violatedNode->parent->color = black;
				violatedNode->parent->parent->color = red;
				violatedNode = violatedNode->parent->parent;
			}
			else {
				if (violatedNode == violatedNode->parent->right) {
					// mirror case 3.2.2
					violatedNode = violatedNode->parent;
					leftRotate(violatedNode);
				}
				// mirror case 3.2.1
				violatedNode->parent->color = black;
				violatedNode->parent->parent->color = red;
				rightRotate(violatedNode->parent->parent);
			}
		}
		if (violatedNode == root) {
			// if we have reached the top, then it means all the down part is satisfied so just break from here
			break;
		}
	}

	// the root always need to be black hence the statment
	root->color = black;

	return true;

}

template <class T> void RBTree<T>::print() {

	if (!root) cout << "The Tree is empty." << endl;
	else printHelper(this->root, "", true);

}

template <class T> void RBTree<T>::printHelper(RBNode<T>* root, string indent, bool last) {

	// print the tree structure on the screen
	if (root != TNULL) {
		cout << indent;
		if (last) {
			cout << "R----";
			indent += "     ";
		}
		else {
			cout << "L----";
			indent += "|    ";
		}

		string sColor = root->color ? "RED" : "BLACK";
		cout << root->data << "(" << sColor << ")" << endl;
		printHelper(root->left, indent, false);
		printHelper(root->right, indent, true);
	}

}

template <class T> void RBTree<T>::deleteNodeHelper(RBNode<T>* node, T key) {

	// find the node containing key
	// the parameter node is at the root, so search starts from root
	RBNode<T>* node_tobe_del = TNULL;
	RBNode<T>* replaced_node; //this root can be potential property breaker. hence send this in fix option
	RBNode<T>* inorder_Successor; //in order successor of the node to be deleted, this is used only when the node to be deleted has two childs

	// if the node(root) reached the nill leaf, then it means the node was not found
	while (node != TNULL) {
		if (node->data == key) {
			node_tobe_del = node;
		}

		if (node->data <= key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}

	if (node_tobe_del == TNULL) {
		cout << "Couldn't find key in the tree" << endl;
		return;
	}

	inorder_Successor = node_tobe_del;
	bool inorder_Successor_original_color = inorder_Successor->color;


	if (node_tobe_del->left == TNULL) {
		// it has right child or no child
		replaced_node = node_tobe_del->right;
		transplant(node_tobe_del, node_tobe_del->right);
	}
	else if (node_tobe_del->right == TNULL) {
		// it has right child
		replaced_node = node_tobe_del->left;
		transplant(node_tobe_del, node_tobe_del->left);
	}
	else {
		// it has two child so first find the inorder successor

		inorder_Successor = minimum(node_tobe_del->right);
		inorder_Successor_original_color = inorder_Successor->color;
		replaced_node = inorder_Successor->right;
		if (inorder_Successor->parent == node_tobe_del) {
			replaced_node->parent = inorder_Successor;
		}
		else {
			transplant(inorder_Successor, inorder_Successor->right);
			inorder_Successor->right = node_tobe_del->right;
			inorder_Successor->right->parent = inorder_Successor;
			// we have worked on inorder's right because this node would not have any left child
			// the inorder would now be floating in air, with the node to be deleted right child appointe its right child
			// and the parent of the node to be delted rights' child updated
			// we are working on its left child, when we are finallly making it take the node to be deleted's place in the next step
		}
		transplant(node_tobe_del, inorder_Successor);
		// finally the inorder succes takes the place of the node to be deleted
		inorder_Successor->left = node_tobe_del->left;
		inorder_Successor->left->parent = inorder_Successor;
		inorder_Successor->color = node_tobe_del->color;
	}

	delete node_tobe_del;

	if (inorder_Successor_original_color == black) {
		// we call fix function only when the node to be delted was black because then the number of black node to nill leaf property is changed
		// if a red node was deleted then this this not need to be called
		fixDelete(replaced_node);
	}

}

template <class T> void RBTree<T>::transplant(RBNode<T>* nodetodel, RBNode<T>* replacement) {

	// this function just rips out the connection of the node to be deleted from its parent, the node to be deleted is now like floating
	if (nodetodel->parent == nullptr) {
		root = replacement;
	}
	else if (nodetodel == nodetodel->parent->left) {
		nodetodel->parent->left = replacement;
	}
	else {
		nodetodel->parent->right = replacement;
	}

	replacement->parent = nodetodel->parent;

}

template <class T> void RBTree<T>::deleteNode(T data) { deleteNodeHelper(this->root, data); }

template <class T> RBNode<T>* RBTree<T>::minimum(RBNode<T>* node) {

	while (node->left != TNULL) {
		node = node->left;
	}

	return node;

}

template <class T> void RBTree<T>::fixDelete(RBNode<T>* potential_violator) {

	RBNode<T>* sibling;
	// cases 1 and 2 belong to red tree deletion and it will not lead to any property violation so this function
	// would not be called for them
	while (potential_violator != root && potential_violator->color == black) {
		if (potential_violator == potential_violator->parent->left) {
			sibling = potential_violator->parent->right;
			if (sibling->color == red) {
				// case 3.1
				// when the sibling is red
				sibling->color = black;
				potential_violator->parent->color = red;
				leftRotate(potential_violator->parent);
				sibling = potential_violator->parent->right;
			}

			if (sibling->left->color == black && sibling->right->color == black) {
				// case 3.2
				// sibling black and both its child black
				sibling->color = red;
				potential_violator = potential_violator->parent;
			}
			else {
				if (sibling->right->color == black) {
					// case 3.3
					// sibling is black, sibling's right child is black but left child is red
					sibling->left->color = black;
					sibling->color = red;
					rightRotate(sibling);
					sibling = potential_violator->parent->right;
				}

				// case 3.4
				// sibling is black, sibling's right child is red
				sibling->color = potential_violator->parent->color;
				potential_violator->parent->color = black;
				sibling->right->color = black;
				leftRotate(potential_violator->parent);
				potential_violator = root;
			}
		}
		else {
			sibling = potential_violator->parent->left;
			if (sibling->color == red) {
				// case 3.1
				// when the sibling is red
				sibling->color = black;
				potential_violator->parent->color = red;
				rightRotate(potential_violator->parent);
				sibling = potential_violator->parent->left;
			}

			if (sibling->right->color == black && sibling->right->color == black) {
				// case 3.2
				// sibling black and both its child black
				sibling->color = red;
				potential_violator = potential_violator->parent;
			}
			else {
				if (sibling->left->color == black) {
					// case 3.3
					// sibling is black, sibling's left child is black but right child is red
					sibling->right->color = black;
					sibling->color = red;
					leftRotate(sibling);
					sibling = potential_violator->parent->left;
				}

				// case 3.4
				// sibling is black, sibling's left child is red
				sibling->color = potential_violator->parent->color;
				potential_violator->parent->color = black;
				sibling->left->color = black;
				rightRotate(potential_violator->parent);
				potential_violator = root;
			}
		}
	}

	potential_violator->color = black;

}