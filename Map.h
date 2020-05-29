#pragma once
#include <iostream>
#include <exception>
#include <list>
#include "Iterator.h"
#include "List.h"
#define RED true
#define BLACK false
#define PRINT 0 
#define KEYS 1 
#define VALUES 2
template <class KEY, class T>
class Map
{
private:
	struct TreeNode
	{
		T data;  //data stored in node 
		KEY key;
		TreeNode* left;
		TreeNode* right;
		TreeNode* parent;
		bool color;
		TreeNode(KEY key, T data, TreeNode* left = nullptr, TreeNode* right = nullptr)
		{
			this->data = data;
			this->left = left;
			this->right = right;
			this->key = key;
			this->parent = nullptr;
			this->color = RED;
		};

		TreeNode()
		{
			this->parent = nullptr;
			this->color = RED;
		}

	};

	size_t size;
	TreeNode* Root;

	void RotateLeft(TreeNode* current)
	{
		TreeNode* r = current->right;
		r->parent = current->parent; 
		// establish r->parent link 
		if (current->parent) {
			if (current->parent->left == current)
				current->parent->left = r;
			else
				current->parent->right = r;
		}
		else
			Root = r;

		current->right = r->left;
		if (r->left)
			r->left->parent = current;

		//link current and r
		current->parent = r;
		r->left = current;
	};
	void RotateRight(TreeNode* current)
	{
		TreeNode* l = current->left;
		// establish l->parent link 
		l->parent = current->parent;
		if (current->parent)
		{
			if (current == current->parent->left)
				current->parent->left = l;
			else
				current->parent->right = l;
		}
		else
			Root = l;

		// establish current->right link 
		current->left = l->right;
		if (l->right)
			l->right->parent = current;

		//link current and l
		current->parent = l;
		l->right = current;
	};

	void deleteTree(TreeNode*& Root)
	{
		if (Root != nullptr)
		{
			deleteTree(Root->left);
			deleteTree(Root->right);
			delete Root;
		}
	};

	TreeNode* grandparent(TreeNode* current) //finds grandfather of current element
	{
		if ((current != NULL) && (current->parent != NULL))
			return current->parent->parent;
		else
			return NULL;
	};

	TreeNode* uncle(TreeNode* current) //finds current element's uncle
	{
		TreeNode* g = grandparent(current);

		if (g == NULL)
			return NULL;  //no grandfather means no uncle
		if (current->parent == g->right)
			return g->left;
		else
			return g->right;
	};

	TreeNode* sibling(TreeNode* current) //finds current element's brother
	{
		if (current == current->parent->left)
			return current->parent->right;
		else 
			return current->parent->left;
	}

	TreeNode* contains(int key, TreeNode* Root)
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
	TreeNode* Local_Max(TreeNode* Root)
	{
		while (Root->right)
			Root = Root->right;
		return Root;
	};
	//returns pointer to the node with the key
	TreeNode* findTreeNode(KEY& key, TreeNode* node)
	{
		if (node != nullptr)
		{
			if (key < node->key)
				return findTreeNode(key, node->left);
			else if (key > node->key)
				return findTreeNode(key, node->right);
			else if (key == node->key)
				return node;
			else
				throw std::out_of_range("This key is not in the map!");

		}
	}
	T find(KEY& key, TreeNode* node)
	{
		if (node != nullptr)
		{
			if (key < node->key)
				return find(key, node->left);
			else if (key > node->key)
				return find(key, node->right);
			else if (key == node->key)
				return node->data;
		}
	}
	void remove(TreeNode* current);
	void balance_insert(TreeNode* current, KEY key, T value);
	void balance_delete(TreeNode* current);
	void print_Tree(TreeNode* current, int level);
	template <class Returnable>
	void in_order_traverse(TreeNode* start, std::list<Returnable>& List, int selector)
	{
		if (start)
		{
			in_order_traverse(start->left, List, selector);
			switch (selector)
			{
			case PRINT: std::cout << "[" << start->key << " : " << start->data << "]"<< "; "; break;
			case KEYS: List.push_back(start->key); break;
			case VALUES: List.push_back(start->data); break;
			default: throw std::invalid_argument("Wrong selector");

				List.push_back(start->key);
			}

			in_order_traverse(start->right, List, selector);
		}
	};



public:
	Map()
	{
		Root = NULL;
		size = 0;
	};
	void clear() // clears associative array (map)
	{
		deleteTree(Root);
	};
	////////////////////////ITERATOR///////////////////////
	class Iter : public Iterator<TreeNode*>
	{
	public:
		Iter(TreeNode* root)
		{
			current = root;
		};

	private:
		TreeNode* next() override
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
		List<TreeNode*> list;
		TreeNode* current;
	};

	Iterator<TreeNode*>* create_iter() const
	{
		if (this == nullptr && this->Root == nullptr) throw std::exception("Dictionary does not exist");
		return new Iter(this->rRot);
	};
	/////////////////////////////////////////////////////////////////////


	void insert(KEY key, T value); // ads elements with key and value
	void remove(KEY key); // deletes element by its key

	bool contains(KEY key) //searches element by its key 
	{
		if (contains(key, Root) == nullptr)
			return false;
		else return true;
	};
	void update(KEY key, T new_value)
	{
		TreeNode* change = findTreeNode(key, Root);
		change->data = new_value;
	};
	T find(KEY key)
	{
		return find(key, Root);
	}
	
	size_t get_size()
	{
		return size;
	}
	std::list<KEY> get_keys(); // returns a key list 
	std::list<T> get_values(); //rerurns a value list
	void print(); // prints elements to the console 
	void my_print();
};





template <typename KEY, class T>
void Map<KEY, T>::insert(KEY key, T value) // ads elements with key and value
{
	TreeNode* last_inserted, * current, * prev = nullptr;
	last_inserted = new TreeNode;
	last_inserted->right = last_inserted->left = last_inserted->parent = nullptr;
	last_inserted->color = RED;
	current = Root;
	if (Root == nullptr) //if tree is empty
	{
		last_inserted->key = key;
		last_inserted->data = value;
		Root = last_inserted;
		balance_insert(last_inserted, key, value);
		size++;
		return;
	}
	//tree is not empty
	while (current)
	{
		prev = current;                //future parent of a new element of the tree
		if (key < current->key)
			current = current->left;
		else
			current = current->right;
	}
	if (key < prev->key) //building a connection between current an prev elements
		prev->left = last_inserted;
	else
		prev->right = last_inserted;

	last_inserted->parent = prev;
	last_inserted->key = key;
	last_inserted->data = value;
	size++;
	balance_insert(last_inserted, key, value);
};


template <typename KEY, class T>
void Map<KEY, T>::balance_insert(TreeNode* current, KEY key, T value)
{
	if (current->parent == nullptr) //the tree is empty
	{
		current->color = BLACK; //root is always black
		return;
	}
	if (current->parent->color == BLACK) //the tree is still valid
		return;

	//case when parent and uncle are RED
	TreeNode* Uncle = uncle(current), * Grandad = grandparent(current);
	if ((Uncle != NULL) && (Uncle->color == RED)) // && (n->parent->color == RED) - parent is RED, it's been alredy checked above
	{
		current->parent->color = BLACK;
		Uncle->color = BLACK;
		Grandad->color = RED;
		balance_insert(Grandad, key, value); //recursive balaning of grandparent
	}

	//case when parent is red but uncle is black
	else
	{
		//current is on the right and parent is on the left
		if ((current == current->parent->right) && (current->parent == Grandad->left))
		{
			RotateLeft(current->parent);
			current = current->left;
		}
		//current is on the left and parent is on the right
		else if ((current == current->parent->left) && (current->parent == Grandad->right))
		{
			RotateRight(current->parent);
			current = current->right;
		}
		current->parent->color = BLACK;
		Grandad->color = RED;
		if ((current == current->parent->left) && (current->parent == Grandad->left)) {
			RotateRight(Grandad);
		}
		else  /* (current == current->parent->right) && (current->parent == Grandad->right) */
			RotateLeft(Grandad);
	}
};

template <typename KEY, class T>
void Map<KEY, T>::remove(KEY key)
{
	TreeNode* current; //deleting element
	current = contains(key, Root);
	if (current == nullptr)
	{
		throw std::out_of_range("Error!This element doesn't exist!");
		return;
	}
	size--;
	remove(current);
	
};


template <typename KEY, class T>
void Map<KEY, T>::remove(TreeNode* current)
{
	if (current->left == nullptr && current->right == nullptr)  //element has no children
	{
		if (current == Root)
			current = nullptr;
		else if (current == current->parent->right) //current is right child
			current->parent->right = nullptr;
		else
			current->parent->left = nullptr; //current is left child
	}
	else if (current->left != nullptr && current->right != nullptr) //current has both children
	{
		TreeNode* localMax = Local_Max(current->left);//finds the greatest value on the left subtree
		current->key = localMax->key;
		current->data = localMax->data;
		remove(localMax);
		return;
	}
	else if (current->left) //current has only left child
	{
		if (current->parent->left == current)
			current->parent->left = current->left; //current's child takes current's place
		else
			current->parent->right = current->left;
		if (current->color == BLACK)
		{
			if (current->left->color == RED) //no need to balance tree
				current->left->color = BLACK;
			else
				balance_delete(current->left);
		}
	}
	else if (current->right)//current has only right one
	{
		if (current->parent->right == current)
			current->parent->right = current->right; //current's child takes current's place
		else
			current->parent->left = current->right;
		if (current->color == BLACK)
		{
			if (current->right->color == RED)
				current->right->color = BLACK;
			else
				balance_delete(current->right);
		}

	}
	delete current;
}
template <typename KEY, class T>
void Map<KEY, T>::balance_delete(TreeNode* current)
{
	if (current->parent == nullptr) //current is the new root, no balance need
		return;

	TreeNode* brother = sibling(current);
	//case when current's sibling is red
	if (brother->color == RED)
	{
		current->parent->color = RED;
		brother->color = BLACK;
		if (current == current->parent->left)
			RotateLeft(current->parent);
		else
			RotateRight(current->parent);
	}
	//case when current's parent, sibling and sibling's children are black
	//as a result all paths going trough sibling have more black elements than other paths
	if ((current->parent->color == BLACK) && (brother->color == BLACK) &&
		(brother->left->color == BLACK) && (brother->right->color == BLACK))
	{
			brother->color = RED;
			balance_delete(current->parent);
	}
	//case when current's parent is red but sibling and sibling's children are black
	if ((current->parent->color == RED) && (brother->color == BLACK) &&
		(brother->left->color == BLACK) && (brother->right->color == BLACK))
		{
			brother->color = RED;
			current->parent->color = BLACK;
		}
	//case when sibling is black, his children have different color
	if (brother->color == BLACK)
	{
		//current is on the left 
		if ((current == current->parent->left) && (brother->right->color == BLACK) && (brother->left->color == RED))
		{
			brother->color = RED;
			brother->left->color = BLACK;
			RotateRight(brother);
		}
		//current is on the right
		else if ((current == current->parent->right) && (brother->left->color == BLACK) && (brother->right->color == RED))
		{
			brother->color = RED;
			brother->right->color = BLACK;
			RotateLeft(brother);
		}
	}
	brother->color = current->parent->color;
	current->parent->color = BLACK;

	if (current == current->parent->left) 
	{
		brother->right->color = BLACK;
		RotateLeft(current->parent);
	}
	else 
	{
		brother->left->color = BLACK;
		RotateRight(current->parent);
	}
 
}



template <typename KEY, class T>
std::list<KEY> Map<KEY, T>::get_keys()
{
	std::list<KEY> List;
	in_order_traverse(Root, List, KEYS);
	return  List;
	
}
template <typename KEY, class T>
std::list<T> Map<KEY, T>::get_values()
{
	std::list<T> List;
	in_order_traverse(Root, List, VALUES);
	return  List;
}
template <typename KEY, class T>
void Map<KEY, T>::print()
{
	if (Root)
	{
		std::list<T> List;
		in_order_traverse(Root, List, PRINT);
	}
	else throw std::invalid_argument("The tree is empty!");
}

template <typename KEY, class T>
void Map<KEY, T>::my_print()
{
	int level = 0;
	if (Root)
		print_Tree(Root, level);
	else throw std::out_of_range("Error!The tree is empty!");
}
template <class KEY, class T>
void Map<KEY, T>::print_Tree(TreeNode* current, int level)
{
	if (current)
	{
		print_Tree(current->right, level + 1);
		for (int i = 0; i < level; i++) std::cout << "        ";
		std::cout << "[" << current->key <<":"<<current->data<<"]" << std::endl;
		print_Tree(current->left, level + 1);
	}
}