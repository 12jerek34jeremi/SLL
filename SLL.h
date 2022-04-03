#ifndef SLL_H
#define SLL_H
#include <stdexcept>
#include <utility>

template <typename key_type, typename value_type> class SLL {
public:
class ListElement {
	public:
		key_type key;
		value_type value;
		ListElement* next;
		ListElement(key_type key, value_type value, ListElement* next=nullptr);
		~ListElement();
	};

class Iterator {
	public:
		ListElement* current;
		Iterator(ListElement* current) {
			this->current = current;
		}
		Iterator& operator++() {
			if (current == nullptr) 
				throw std::out_of_range("End of list already reached!");

			current = current->next;
			return *this;
		}
		Iterator operator++(int) {
			if (current == nullptr)
				throw std::out_of_range("There is no other element!");
			Iterator old = *this;
			current = current->next;
			return *this;
		}

		friend bool operator == (const Iterator& a, const Iterator& b) { return a.current == b.current; }
		friend bool operator != (const Iterator& a, const Iterator& b) { return a.current != b.current; }

		std::pair<key_type, value_type> operator*() {
			return std::pair<key_type, value_type>(current->key, current->value);
		}
	};


public:
	ListElement* first;
	ListElement* last;
	int elements_nr;

public:
	SLL();
	~SLL();
	SLL(const SLL& source);
	SLL(SLL&& source);
	int get_element_nr();

	void insert_front(key_type key, value_type value);
	void insert_back(key_type key, value_type value);
	bool insert_middle(key_type key, value_type value, key_type after); //inserting after given key 'after'

	value_type get_first();
	value_type get_last();
	value_type& operator[](key_type key);
	value_type operator[](key_type) const;
	bool is_in(key_type key);

	void delete_first();
	void delete_last();
	bool delete_element(key_type key);

	Iterator begin()const{ return Iterator(first); }
};

template<typename key_type, typename value_type>
value_type SLL<key_type, value_type>::operator[](key_type key) const
{
	ListElement* current = first;
	while (current->key != key && current != nullptr) {
		current = current->next;
	}
	if (current == nullptr) {
		throw std::out_of_range("The element of given key wasn't found!");
	}
	return current->value;
}
template<typename key_type, typename value_type>
inline bool SLL<key_type, value_type>::is_in(key_type key)
{
	return false;
}

template<typename key_type, typename value_type>
value_type& SLL<key_type, value_type>::operator[](key_type key)
{
	ListElement* current = first;
	while (current->key != key && current != nullptr) {
		current = current->next;
	}
	if (current == nullptr) {
		throw std::out_of_range("The element of given key wasn't found!");
	}
	return current->value;
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>::ListElement::ListElement(key_type key, value_type value, ListElement* next)
{
	this->key = key;
	this->value = value;
	this->next = next;
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>::ListElement::~ListElement()
{
	next = nullptr;
}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::delete_last()
{	
	if (elements_nr == 0) {
		return;
	}
	
	if (elements_nr == 1) {
		delete first;
		first = nullptr;
		last = nullptr;
		elements_nr = 0;
		return;
	}

	ListElement* last_but_one = first;
	while (last_but_one->next != last) {
		last_but_one = last_but_one->next;
	}
	delete last_but_one->next;
	last_but_one->next = nullptr;
	last = last_but_one;
	elements_nr--;

}

template<typename key_type, typename value_type> bool SLL<key_type, value_type>::delete_element(key_type key)
{
	ListElement* to_delete = first;
	ListElement* before = nullptr;
	while (to_delete != nullptr && to_delete->key != key) {
		before = to_delete;
		to_delete = to_delete->next;
	}

	if (to_delete == nullptr) {
		return false;
	}
	if (before == nullptr) {
		first = to_delete->next;
		if(to_delete == last)
			last = nullptr;
	}
	else
		before->next = to_delete->next;
	
	elements_nr--;
	delete to_delete;
	return true;

}

template<typename key_type, typename value_type> int SLL<key_type, value_type>::get_element_nr()
{
	return elements_nr;
}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::insert_front(key_type key, value_type value)
{
	if (first == nullptr) {
		first = new ListElement(key, value);
		last = first;
	}
	else {
		first = new ListElement(key, value, first);
	}
	elements_nr++;
}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::insert_back(key_type key, value_type value)
{
	if (last == nullptr) {
		first = new ListElement(key, value);
		last = first;
	}
	else {
		last->next = new ListElement(key, value);
		last = last->next;
	}
	elements_nr++;
}

template<typename key_type, typename value_type> bool SLL<key_type, value_type>::insert_middle(key_type key, value_type value, key_type after)
{
	ListElement* current = first;
	while (current->key != after && current!=nullptr) {
		current = current->next;
	}
	if (current == nullptr) {
		return false;
	}
	ListElement * new_element = new ListElement(key, value);
	new_element->next = current->next;
	current->next = new_element;
	elements_nr++;
	return true;
}

template<typename key_type, typename value_type> value_type SLL<key_type, value_type>::get_first()
{
	if (first == nullptr) {

		throw std::out_of_range("This list is empty, there is no first element!");
	}
	return first->value;
}

template<typename key_type, typename value_type> value_type SLL<key_type, value_type>::get_last()
{
	if (last == nullptr) {
		throw std::out_of_range("This list is empty, there is no last element!");
	}
	return last->value;
}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::delete_first()
{
	if (first == nullptr){
		return;
	}
	ListElement* new_first = first->next;
	delete first;
	first = new_first;
	elements_nr--;
}


template<typename key_type, typename value_type>  SLL<key_type, value_type>::SLL()
{
	std::cout << "START OF CONSTRUCTOR (NORMAL)" << std::endl;
	first = nullptr;
	last = nullptr;
	elements_nr = 0;
	std::cout << "END OF CONSTRUCTOR (NORMAL)" << std::endl;
}

template<typename key_type, typename value_type> SLL<key_type, value_type>::~SLL()
{
	std::cout << "START OF DECONSTRUCTOR" << std::endl;
	ListElement* current = first;
	while (current != nullptr) {
		ListElement* temp = current->next;
		delete current;
		current = temp;
	}
	this->first = nullptr;
	std::cout << "END OF DECONSTRUCTOR" << std::endl;
}

template<typename key_type, typename value_type> SLL<key_type, value_type>::
SLL(SLL&& source) : first(source.first), last(source.last), elements_nr(source.elements_nr) {
	std::cout << "START OF MOVE CONSTRUCTOR !!" << std::endl;
	source.first = nullptr;
	source.last = nullptr;
	std::cout << "END OF MOVE CONSTRUCTOR !!" << std::endl;
}

template<typename key_type, typename value_type> SLL<key_type, value_type>::
SLL(const SLL& source) : first(nullptr), last(nullptr), elements_nr(0) {
	std::cout << "START OF COPY CONSTRUCTOR" << std::endl;
	for (ListElement* current = source.first; current != nullptr; current = current->next) {
		this->insert_back(current->key, current->value);
	}
	std::cout << "END OF COPY CONSTRUCTOR" << std::endl;
}

#endif
