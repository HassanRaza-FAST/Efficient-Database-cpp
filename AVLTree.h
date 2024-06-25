#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;

template <class T> class AVLNode {

public:

	T data;
	int height;
	AVLNode<T>* left;
	AVLNode<T>* right;

	AVLNode(T);

};

template <class T> AVLNode<T>::AVLNode(T data) {

	this->data = data;
	height = 0;
	left = nullptr;
	right = nullptr;

}

template <class T> class AVLTree {

public:

	AVLNode<T>* root;

	AVLTree();
	bool insert(T);
	void remove(T);
	void print();
	AVLNode<T>* search(T);
	bool search(T, T, vector<AVLNode<T>*>&); // for range search (inclusive)
	AVLNode<T>* helperSeacrh(AVLNode<T>*, T);
	void rangeSearchHelper(AVLNode<T>*, T, T, vector<AVLNode<T>*>&); // will return vector of nodes in the argument, in the given range

};

template <class T> AVLTree<T>::AVLTree() { root = nullptr; }

template <class T> bool AVLTree<T>::insert(T num) {

	if (!root) {
		AVLNode<T>* temp = new AVLNode<T>(num);
		root = temp;
		root->height = calc_height(root);
		return true;
	}

	bool duplicate = false;

	root = insertRecursive(root, num, duplicate);

	if (duplicate) return false;
	else return true;

}

template <class T> void AVLTree<T>::remove(T num) {

	if (!root) {
		cout << "No root can be deleted as the tree is empty. " << endl;
		return;
	}

	root = removeRecursive(root, num);

}

template <class T> void AVLTree<T>::print() {

	string s;
	printAVL_prettyprint(s, root, false);

}

template <class T> AVLNode<T>* right_rotate(AVLNode<T>* root) {

	AVLNode<T>* tobereturned = root->left;
	AVLNode<T>* tobereturned_rightchild = nullptr;

	if (root->left) tobereturned_rightchild = root->left->right;

	if (tobereturned) tobereturned->right = root;

	root->left = tobereturned_rightchild;

	return tobereturned;

}

template <class T> AVLNode<T>* left_rotate(AVLNode<T>* root) {

	AVLNode<T>* tobereturned = root->right;
	AVLNode<T>* tobereturned_leftchild = nullptr;

	if (root->right) tobereturned_leftchild = root->right->left;

	if (tobereturned) tobereturned->left = root;

	root->right = tobereturned_leftchild;

	return tobereturned;

}

template <class T> int calc_height(AVLNode<T>* root) {

	if (!root) return -1;

	int left = calc_height(root->left);
	int right = calc_height(root->right);

	return(max(left, right) + 1);

}

template <class T> AVLNode<T>* insertRecursive(AVLNode<T>* root, T val, bool& duplicate) {

	if (root == nullptr) {
		AVLNode<T>* temp = new AVLNode<T>(val);
		return temp;
	}

	if (root->data == val) {
		duplicate = true;
		return root;
	}
	else if (root->data > val) root->left = insertRecursive(root->left, val, duplicate);
	else root->right = insertRecursive(root->right, val, duplicate);

	int leftheight = calc_height(root->left);
	int rightheight = calc_height(root->right);

	if (leftheight - rightheight == 2 && val < root->left->data) {
		AVLNode<T>* newNode = right_rotate(root);
		root = newNode;
	}
	else if (leftheight - rightheight == -2 && val > root->right->data) {
		AVLNode<T>* newNode = left_rotate(root);
		root = newNode;
	}
	else if (leftheight - rightheight == 2 && val > root->left->data) {
		root->left = left_rotate(root->left);
		AVLNode<T>* newNode = right_rotate(root);
		root = newNode;
	}
	else if (leftheight - rightheight == -2 && val < root->right->data) {
		root->right = right_rotate(root->right);
		AVLNode<T>* newNode = left_rotate(root);
		root = newNode;
	}

	root->height = calc_height(root);

	return root;

}

template <class T> AVLNode<T>* inorderSuccessor(AVLNode<T>* root) {

	while (root->left) root = root->left;

	return root;

}

template <class T> AVLNode<T>* inorderPrecessor(AVLNode<T>* root) {

	while (root->right) root = root->right;

	return root;

}

template <class T> AVLNode<T>* removeRecursive(AVLNode<T>* root, T val) {

	if (!root) return root;

	if (val < root->data) root->left = removeRecursive(root->left, val);
	else if (val > root->data) root->right = removeRecursive(root->right, val);
	else {
		if (!root->left) {
			AVLNode<T>* temp = root->right;
			delete root;
			return temp;
		}
		else if (!root->right) {
			AVLNode<T>* temp = root->left;
			delete root;
			return temp;
		}
		else {
			AVLNode<T>* temp = inorderPrecessor(root->left);
			root->data = temp->data;
			root->left = removeRecursive(root->left, temp->data);
		}
	}

	int leftheight = calc_height(root->left);
	int rightheight = calc_height(root->right);

	int leftsubtreeheight_left;
	int leftsubtreeheight_right;

	int bfLeft = -1;
	int bfRight = -1;

	if (root->left) {
		leftsubtreeheight_left = calc_height(root->left->left);
		leftsubtreeheight_right = calc_height(root->left->right);
		bfLeft = leftsubtreeheight_left - leftsubtreeheight_right;
	}
	if (root->right) {
		int rightsubtreeheight_left = calc_height(root->right->left);
		int rightsubtreeheight_right = calc_height(root->right->right);
		bfRight = rightsubtreeheight_left - rightsubtreeheight_right;
	}

	if (leftheight - rightheight == 2 && bfLeft >= 0) {
		AVLNode<T>* newNode = right_rotate(root);
		root = newNode;
	}
	else if (leftheight - rightheight == -2 && bfRight <= 0) {
		AVLNode<T>* newNode = left_rotate(root);
		root = newNode;
	}
	else if (leftheight - rightheight == 2 && bfLeft == -1) {
		root->left = left_rotate(root->left);
		AVLNode<T>* newNode = right_rotate(root);
		root = newNode;
	}
	else if (leftheight - rightheight == -2 && bfRight == 1) {
		root->right = right_rotate(root->right);
		AVLNode<T>* newNode = left_rotate(root);
		root = newNode;
	}

	root->height = calc_height(root);

	return root;

}

template <class T> void LevelOrder(AVLNode<T>* root) {

	queue<AVLNode<T>*> q;
	q.push(root);

	while (!q.empty()) {

		int size = q.size();

		for (int i = 0; i < size; i++) {
			AVLNode<T>* temp = q.front();
			q.pop();
			cout << temp->data << " ";
			if (temp->left)
				q.push(temp->left);
			if (temp->right)
				q.push(temp->right);
		}

		cout << endl;

	}

}

template <class T> void printAVL_prettyprint(string prefix, AVLNode<T>* node, bool isLeft) {

	if (node != nullptr) {

		cout << prefix;

		cout << (isLeft ? "L--" : "R--");

		cout << node->data << endl;

		string t1 = "|   ";
		string t2 = "    ";

		if (isLeft) {
			prefix += t1;
		}
		else {
			prefix += t2;
		}

		printAVL_prettyprint(prefix, node->left, true);
		printAVL_prettyprint(prefix, node->right, false);

	}
}

template<class T> AVLNode<T>* AVLTree<T>::search(T key) {
	AVLNode<T>* result = helperSeacrh(root, key);
	return result;
}

template<class T> AVLNode<T>* AVLTree<T>::helperSeacrh(AVLNode<T>* current, T key) {
	if (current == nullptr || current->data == key)
		return current;

	if (key > current->data)
		return helperSeacrh(current->right, key);
	else
		return helperSeacrh(current->left, key);
}

template<class T> bool AVLTree<T>::search(T from, T to, vector<AVLNode<T>*>& ans) {
	rangeSearchHelper(root, from, to, ans);
	if (ans.size() == 0) return false;
	return true;
}

template<class T> void AVLTree<T>::rangeSearchHelper(AVLNode<T>* current, T from, T to, vector<AVLNode<T>*>& ans) {
	if (!current) return;
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