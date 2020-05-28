#pragma once
#include <stdexcept>
#include "Iterator.h"

template <typename  T>
class List 
{
public:
	struct Node
	{
		Node(T data, Node* next = nullptr, Node* prev = nullptr)
		{
			this->data = data;
			this->next = next;
			this->prev = prev;
		};
		~Node()
		{
			next = nullptr;
			delete next;
			prev = nullptr;
			delete prev;
			data = {};
		};
		T data;
		Node* next;
		Node* prev;
	};
	void add_first(T newElem)
	{
		head = new Node(newElem);
		tail = head;
		size = 1;
	}
	void reset_list()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}
	Node* head;
	Node* tail;
	size_t size;
	class ListIterator : public Iterator<T>
	{
	public:
		ListIterator(Node* head)
		{
			current = head;
		};

	private:
		T next() override
		{
			if (!current) return T();
			const T data = current->data;
			current = current->next;
			return data;
		}
		bool has_next() override
		{
			return (current != nullptr);
		}
		Node* current;
	};
public:
	List()
	{
		reset_list();
	}
	~List()
	{
		clear();
	}

	void push_back(T newElem)
	{
		if (size == 0) {
			add_first(newElem);
		}
		else {
			tail->next = new Node(newElem, nullptr, tail);
			tail = tail->next;
			size++;
		}
	}

	void push_front(T newElem)
	{
		if (size == 0) {
			add_first(newElem);
		}
		else {
			head = new Node(newElem, head, nullptr);
			size++;
		}
	}

	void pop_back() //deletes last element
	{
		if (size == 0) return;

		if (size == 1) {
			reset_list();
			delete head;
			delete tail;
		}
		else {
			Node* current = tail->prev;
			current->next = nullptr;
			delete tail;
			tail = current;
			size--;
		}
	}
	void pop_front() // deletes first element
	{
		if (size == 0) return;

		if (size == 1) {
			reset_list();
			delete head;
			delete tail;
		}
		else {
			Node* current = head->next;
			delete head;
			head = current;
			size--;
		}
	}

	void insert(size_t index, T newElem) //ads element by index
	{
		if (index >= size + 1) {
			throw std::out_of_range("Index is greater than list size and more than the index for teoretical last element");
		}
		else {
			if (size == 0 && index == 0)
			{
				add_first(newElem);
				return;
			}
			if (index == 0)
			{
				push_front(newElem);
				return;
			}
			if (index == size)
			{
				push_back(newElem);
				return;
			}
			size_t counter = 0;
			Node* current = head;
			Node* prev = nullptr;
			while (counter != index) {
				prev = current;
				current = current->next;
				counter++;
			}
			current = new Node(newElem, current, prev);
			if (prev != nullptr) {
				prev->next = current;
			}
			else
			{
				head = current;
			}
			size++;
		}
	}

	T at(size_t index) const
	{
		if (index >= size) {
			throw std::out_of_range("Index is greater than list size");
		}
		else {
			size_t counter = 0;
			Node* current = head;
			while (counter != index) {
				current = current->next;
				counter++;
			}
			return current->data;
		}
	}

	void remove(size_t index) // deletes elem by its index
	{
		if (index >= size)
		{
			throw std::out_of_range("Index is greater than list size");
		}
		else {
			if (size == 0)
			{
				return;
			}
			if (index == 0)
			{
				pop_front();
				return;
			}
			if (index == size - 1)
			{
				pop_back();
				return;
			}
			size_t counter = 0;
			Node* current = head;
			while (counter != index) {
				current = current->next;
				counter++;
			}
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
			size--;
		}
	}

	size_t get_size() const
	{
		return size;
	}

	void print_to_console() const // prints elements 
	{
		if (size == 0)
			return;
		auto* list = create_list_iterator();
		std::cout << "[nullptr] <- ";
		while (list->has_next()) {
			std::cout << "[" << list->next() << "] <- ";
		}
		std::cout << "[nullptr]" << std::endl;
	}

	void clear()// destructor of the list
	{
		if (size == 0) return;

		if (size == 1) {
			reset_list();
			delete head;
			delete tail;
		}
		else {
			while (head != nullptr) {
				pop_front();
			}
			reset_list();
			delete head;
			delete tail;
		}
	}

	void set(size_t index, T newElem) const //replaces elem by index to newElem
	{
		if (index >= size) {
			throw std::out_of_range("Index is greater than list size");
		}
		else {
			size_t counter = 0;
			Node* current = head;
			while (counter != index) {
				current = current->next;
				counter++;
			}
			current->data = newElem;
		}
	}
	bool isEmpty() const //checks if the list is empty
	{
		return size == 0;
	}
	bool contains(T data) const
	{
		if (this->size == 0) return true;
		Node* current = head;
		Node* current_find = this->head;
		bool contains = false;
		do {
			if (current->data == current_find->data)
			{
				current_find = current_find->next;
				if (current_find == nullptr)
				{
					contains = true;
					break;
				}
				current = current->next;
			}
			else
			{
				contains = false;
				current_find = this->head;
				current = current->next;
			}
		} while (current != nullptr);
		return contains;
	}
	bool equals(List* list) const
	{
		if (size != list->size)//if size equals
			return false;
		for (int i = 0; i < size; i++)
		{
			if (at(i) != list->at(i))
				return false;
		}
		return true;
	}
	T get_back()
	{

		return tail->data;
	}

	T get_top()
	{
		return head->data;
	}
	Iterator<T>* create_list_iterator() const
	{
		if (this == nullptr && this->head == nullptr) throw std::exception("Does not exist");
		return new ListIterator(this->head);
	}
};