#pragma once
#include <iostream>
#include "Iterator.h"
#include "List.h"
#define RED 0
#define BLACK 1
using namespace std;

template <class T, class T2 >
class Map
{
private:

	class Node
	{
	public:
		Node(T key, T2 data, Node* left = nullptr, Node* right = nullptr) 
		{
			this->data = data;
			this->left = left;
			this->right = right;
			this->key = key;
			this->parent = nullptr;
			this->color = RED;
		};
		bool color;
		T key; T2 data;
		Node* right;
		Node* left;
		Node* parent;
	};

	Node* root;
	size_t size;

public:
	Map()
	{
		root = NULL;
		size = 0;
	}
	void clear()
	{
		delete_tree(root);
	}

	class Iter : public Iterator<Node*>
	{
	public:
		Iter(Node* root)
		{
			current = root;
		};

	private:
		Node* next() override
		{
			while (current != nullptr) {
				list.push_front(current);
				current = current->left;
			}
			current = list.at(0);
			list.pop_front();
			auto temp = current;
			current = current->right;
			return temp;
		}
		bool has_next() override
		{
			return (list.get_size() || current != nullptr);
		};
		List<Node*> list;
		Node* current;
	};

	Iterator<Node*>* create_iter() const
	{
		if (this == nullptr && this->root == nullptr) throw std::exception("Dictionary does not exist");
		return new Iter(this->root);
	};

private:
	void insertfix(Node* current)      //supporting function for insert
	{
		Node* uncle;
		if (root == current)
		{
			current->color = BLACK;
			return;
		}

		while (current->parent != nullptr && current->parent->color == RED)
		{
			Node* grandparents = current->parent->parent;
			if (grandparents->left == current->parent)
			{
				if (grandparents->right != nullptr)
				{
					uncle = grandparents->right;
					if (uncle->color == RED)
					{
						current->parent->color = BLACK;
						uncle->color = BLACK;
						grandparents->color = RED;
						current = grandparents;
					}
				}
				else
				{
					if (current->parent->right == current)
					{
						current = current->parent;
						leftrotate(current);
					}
					current->parent->color = BLACK;
					grandparents->color = RED;
					rightrotate(grandparents);
				}
			}
			else
			{
				if (grandparents->left != nullptr)
				{
					uncle = grandparents->left;
					if (uncle->color == RED)
					{
						current->parent->color = BLACK;
						uncle->color = BLACK;
						grandparents->color = RED;
						current = grandparents;
					}
				}
				else
				{
					if (current->parent->left == current)
					{
						current = current->parent;
						rightrotate(current);
					}
					current->parent->color = BLACK;
					grandparents->color = RED;
					leftrotate(grandparents);
				}
			}
			root->color = BLACK;
		}
	}
	void leftrotate(Node* current)
	{
		if (current->right == nullptr)
			return;
		else
		{
			Node* y = current->right;
			if (y->left != nullptr)
			{
				current->right = y->left;
				y->left->parent = current;
			}
			else
				current->right = nullptr;
			if (current->parent != nullptr)
				y->parent = current->parent;
			if (current->parent == nullptr)
				root = y;
			else
			{
				if (current == current->parent->left)
					current->parent->left = y;
				else
					current->parent->right = y;
			}
			y->left = current;
			current->parent = y;
		}
	}

	void rightrotate(Node* current)
	{
		if (current->left == nullptr)
			return;
		else
		{
			Node* child = current->left;
			if (child->right != nullptr)
			{
				current->left = child->right;
				child->right->parent = current;
			}
			else
				current->left = nullptr;
			if (current->parent != nullptr)
				child->parent = current->parent;
			if (current->parent == nullptr)
				root = child;
			else
			{
				if (current == current->parent->left)
					current->parent->left = child;
				else
					current->parent->right = child;
			}
			child->right = current;
			current->parent = child;
		}
	}

	Node* get_leaf(Node* current)      //supporting function for remove
	{
		Node* leaf = NULL;
		if (current->left != NULL)
		{
			leaf = current->left;
			while (leaf->right != NULL)
				leaf = leaf->right;
		}
		else
		{
			leaf = current->right;
			while (leaf->left != NULL)
				leaf = leaf->left;
		}
		return leaf;
	}

	void add_first(Node* current)  //adding the first element to the tree
	{
		root = current;
		root->parent = nullptr;
	}

	void delfix(Node* current)  // remove balance
	{
		Node* temp_elem;
		while (current != nullptr && current != root && current->color == BLACK)
		{
			if (current->parent->left == current)
			{
				temp_elem = current->parent->right;
				if (temp_elem->color == RED)
				{
					temp_elem->color = BLACK;
					current->parent->color = RED;
					leftrotate(current->parent);
					temp_elem = current->parent->right;
				}
				if (temp_elem->right->color == BLACK && temp_elem->left->color == BLACK)
				{
					temp_elem->color = RED;
					current = current->parent;
				}
				else
				{
					if (temp_elem->right->color == BLACK)
					{
						temp_elem->left->color = BLACK;
						temp_elem->color = RED;
						rightrotate(temp_elem);
						temp_elem = current->parent->right;
					}
					temp_elem->color = current->parent->color;
					current->parent->color = BLACK;
					temp_elem->right->color = BLACK;
					leftrotate(current->parent);
					current = root;
				}
			}
			else
			{
				temp_elem = current->parent->left;
				if (temp_elem->color == RED)
				{
					temp_elem->color = BLACK;
					current->parent->color = RED;
					rightrotate(current->parent);
					temp_elem = current->parent->left;
				}
				if (temp_elem->left->color == BLACK && temp_elem->right->color == BLACK)
				{
					temp_elem->color = RED;
					current = current->parent;
				}
				else
				{
					if (temp_elem->left->color == BLACK)
					{
						temp_elem->right->color = BLACK;
						temp_elem->color = RED;
						leftrotate(temp_elem);
						temp_elem = current->parent->left;
					}
					temp_elem->color = current->parent->color;
					current->parent->color = BLACK;
					temp_elem->left->color = BLACK;
					rightrotate(current->parent);
					current = root;
				}
			}
			current->color = BLACK;
			root->color = BLACK;
		}
	}

	T2 find(T& key, Node* node)
	{
		if (node != nullptr)
		{
			if (key < node->key)
				return find(key, node->left);
			else if (key > node->key)
				return find(key, node->right);
			else if (key == node->key)
				return node->data;
			else
				throw std::out_of_range("This key is not in the map!");

		}
	}

	Node* contains(T key, Node* Root)
	{
		if (Root)
		{
			if (key < Root->key)
				contains(key, Root->left); //recursive search to the left
			else if (key > Root->key)
				contains(key, Root->right); //recursive search to the right
			else
				return Root;

		}
		else return nullptr; //if element wasn't found
	};

public:
	bool contains(T key) //searches element by its key 
	{
		if (contains(key, root) == nullptr)
			return false;
		else return true;
	};

	void get_keys()
	{
		if (this->root == nullptr)
		{
			throw out_of_range("Root was nullptr!");
		}

		auto it = create_iter();
		while (it->has_next())
		{
			cout << it->next()->key << ' ';
		}
		cout << '\n';
		auto it1 = create_iter();
		while (it1->has_next())
		{
			cout << it1->next()->color << ' ';
		}

		cout << '\n';
	}
	void get_value()
	{
		if (this->root == nullptr)
		{
			throw out_of_range("Root was nullptr!");
		}
		auto it1 = create_iter();
		while (it1->has_next())
			cout << it1->next()->data << ' ';
		cout << '\n';
	}

	void delete_tree(Node*& root)//clearing the tree
	{
		if (root != nullptr)
		{
			delete_tree(root->left);
			delete_tree(root->right);
			delete root;
		}
	}
	void insert(T key, T2 value)//insert element in the tree
	{
		Node* help_elem, * prev;
		Node* current = new Node(key, value);
		help_elem = root;
		prev = nullptr;
		if (root == nullptr)//if the list is empty
		{
			add_first(current);
		}
		else
		{
			while (help_elem != nullptr) //finds a place to add a new element
			{
				prev = help_elem;
				if (help_elem->key < current->key)
					help_elem = help_elem->right;
				else
					if (help_elem->key > current->key)
						help_elem = help_elem->left;
					else
						if (help_elem->key == current->key)
							return;
			}
			current->parent = prev;
			if (prev->key < current->key)
				prev->right = current;
			else
				prev->left = current;
		}

		insertfix(current);
		size++;
	}
	void remove(T key)//removing element by key
	{
		if (this->root == nullptr)                               
		{
			throw out_of_range("The map is empty!");
		}

		Node* current;
		current = root;
		Node* parent_of_leaf = nullptr;
		Node* leaf = nullptr;
		int flag = 0;                                            
		while (current !=nullptr && flag == 0)                     
		{
			if (current->key == key)
				flag = 1;
			if (flag == 0)
			{
				if (current->key < key)
					current = current->right;
				else
					current = current->left;
			}
		}
		if (flag == 0)                                         
		{
			throw out_of_range("Not found");
		}
		else
		{
			if (current->left == nullptr || current->right == nullptr)//elem doesn't have right or left child
				parent_of_leaf = current;
			else
				parent_of_leaf = get_leaf(current);
			if (parent_of_leaf->left != nullptr)
				leaf = parent_of_leaf->left;
			else
			{
				if (parent_of_leaf->right != nullptr)
					leaf = parent_of_leaf->right;
				else
					leaf = nullptr;
			}
			if (leaf != nullptr)
				leaf->parent = parent_of_leaf->parent;
			if (parent_of_leaf->parent == nullptr)
				root = leaf;
			else
			{
				if (parent_of_leaf == parent_of_leaf->parent->left)
					parent_of_leaf->parent->left = leaf;                     
				else
					parent_of_leaf->parent->right = leaf;
			}
			if (parent_of_leaf != current)                                  
			{
				current->color = parent_of_leaf->color;
				current->key = parent_of_leaf->key;
			}
			if (parent_of_leaf->color == BLACK)                       
				delfix(leaf); //balancing colors
		}
		size--;
	}

	size_t get_size()//getting size of the tree
	{
		return size;
	}
	T2 find(T key)
	{
		return find(key, root);
	}
};