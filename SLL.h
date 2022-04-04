#ifndef SLL_H
#define SLL_H
#include <stdexcept>
#include <utility>
#include <iostream>

template <typename key_type, typename value_type> class SLL {
public:
	class IteratorBase; class IteratorConst; class IteratorVar;

private:
class ListElement {
	friend class SLL; friend class IteratorBase;
	friend class IteratorConst; friend class IteratorVar;
	public:
		ListElement(key_type key, value_type value, ListElement* next=nullptr);
		~ListElement();
	private:
		key_type key;
		value_type value;
		ListElement* next;
	};

public:
class IteratorBase {
	public:
		IteratorBase(ListElement* current);

		IteratorBase& operator++();
		IteratorBase operator++(int);

		friend bool operator == (const IteratorBase& a, const IteratorBase& b) { return a.current == b.current; }
		friend bool operator != (const IteratorBase& a, const IteratorBase& b) { return a.current != b.current; }

	protected:
		ListElement* current;
	};
class IteratorVar : public IteratorBase {
	public:
		IteratorVar(ListElement * current);
		std::pair<key_type &, value_type &> operator*();
};
class IteratorConst : public IteratorBase {
	public:
		IteratorConst(ListElement* current);
		std::pair<key_type, value_type> operator*() const;

};


private:
	ListElement* first;
	ListElement* last;
	int elements_nr;
	ListElement* find(const key_type key, int occurence = 1) const;

public:
	//constructors, destructor, and assignment operator
	SLL();
	~SLL();
	SLL(const SLL& source); //copy constructor, it creates a DEEP copy of an object
	SLL(SLL&& source); //move constructor, creates a SHALLOW copy and prevents destructor from deleting all elements.
	SLL& operator=(const SLL& source); //delete all nodes and creates a deep copy

	//Functions for inserting new elements to the lists.
	void insert_front(const key_type key, const value_type value); //inserts new element of key 'key' and value 'value' at the beginning of the list
	void insert_back(const key_type key, const value_type value);//inserts new element of key 'key' and value 'value' at end of the list
	bool insert_middle(const key_type key, const value_type value, const key_type after, int occurence=1); //inserts new element after
	//element which key is 'after'. Key  

	//Funcitons for accessing list elements.
	value_type get_first() const;
	value_type get_last()const;
	value_type get_value(const key_type key, int occurence=1) const;

	//Functions for deleting list elements
	void delete_first();
	void delete_last();
	bool delete_element(const key_type key, int occurence=1);
	void clear();

	//Functions for modyfying elements
	bool set_key(const key_type key, const key_type new_key, int occurence=1);
	bool set_value(const key_type key, const value_type value, int occurence = 1);

	// Other functions:
	bool is_in(key_type key, int occurence) const;
	IteratorConst begin() const;
	IteratorVar begin();
	int get_element_nr();
};

/// ----- START OF SLL CLASS METHODS IMPLEMENTATIONS -----------

// START OF CONSTRUCTORS IMPLEMENTATIONS
template<typename key_type, typename value_type>
SLL<key_type, value_type>::SLL()
{
	first = nullptr;
	last = nullptr;
	elements_nr = 0;
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>::~SLL()
{
	ListElement* current = first;
	while (current != nullptr) {
		ListElement* temp = current->next;
		delete current;
		current = temp;
	}
	first = nullptr;
	last = nullptr;
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>::
SLL(SLL&& source) : first(source.first), last(source.last), elements_nr(source.elements_nr) {
	
	source.first = nullptr;
	source.last = nullptr;
	source.elements_nr = 0;
	
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>& SLL<key_type, value_type>:: operator=(const SLL & source){
	this->~SLL();
	for (ListElement* current = source.first; current != nullptr; current = current->next) {
		this->insert_back(current->key, current->value);
	}
	return *this;
}

template<typename key_type, typename value_type> SLL<key_type, value_type>::
SLL(const SLL& source) : first(nullptr), last(nullptr), elements_nr(0) {
	std::cout << "START OF COPY CONSTRUCTOR" << std::endl;
	for (ListElement* current = source.first; current != nullptr; current = current->next) {
		this->insert_back(current->key, current->value);
	}
	std::cout << "END OF COPY CONSTRUCTOR" << std::endl;
}
//END OF CONSTRUCTORS IMPLEMENTATIONS


//START OF INSERTING ELEMENTS FUNCTIONS IMPLEMENTATIONS
template<typename key_type, typename value_type>
void SLL<key_type, value_type>::insert_front(const key_type key, const value_type value)
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

template<typename key_type, typename value_type>
void SLL<key_type, value_type>::insert_back(const key_type key, const value_type value)
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

template<typename key_type, typename value_type>
bool SLL<key_type, value_type>::insert_middle(const key_type key, const value_type value, const key_type after, int occurence)
{
	ListElement* after_element = find(after, occurence);
	if (after_element == nullptr)
		return false;

	after_element->next = new ListElement(key, value, after_element->next);
	if (after_element == last)
		last = after_element->next;
}
// END OF INSERTING ELEMENTS FUNCTIONS IMPLEMENTATIONS


//START OF ACCESSING ELEMENTS FUNCTIONS IMPLEMENTATIONS
template<typename key_type, typename value_type>
value_type SLL<key_type, value_type>::get_value(const key_type key, int occurence) const
{
	ListElement* element = find(key, occurence);
	if(element == nullptr)
		throw std::out_of_range("The element of given key is not in this list!");

	return element->value;
}

template<typename key_type, typename value_type>
value_type SLL<key_type, value_type>::get_first() const
{
	if (first == nullptr)
		throw std::out_of_range("This list is empty, there is no first element!");

	return first->value;
}

template<typename key_type, typename value_type>
value_type SLL<key_type, value_type>::get_last() const
{
	if (last == nullptr) {
		throw std::out_of_range("This list is empty, there is no last element!");
	}
	return last->value;
}
// END OF ACCESSING ELEMENTS FUNCTIONS IMPLEMENTATIONS


//START OF DELETING ELEMENTS FUNCTIONS IMPLEMENTATIONS
template<typename key_type, typename value_type>
void SLL<key_type, value_type>::delete_last()
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
	for (; last_but_one->next != last; last_but_one = last_but_one->next);
	delete last_but_one->next;
	last_but_one->next = nullptr;
	last = last_but_one;
	elements_nr--;
}

template<typename key_type, typename value_type>
bool SLL<key_type, value_type>::delete_element(const key_type key, int occurence)
{
	ListElement* before = nullptr;
	ListElement* to_delete = first;

	for (int count = 0; to_delete != nullptr; before = to_delete, to_delete = to_delete->next)
	{
		if (to_delete->key == key) {
			count++;
			if (count == occurence) {
				if (before == nullptr) {
					first = first->next;
				}
				else {
					before->next = to_delete->next;
				}
				delete to_delete;
				elements_nr--;
				return true;
			}
			
		}
	}

	return false;

}

template<typename key_type, typename value_type>
void SLL<key_type, value_type>::clear()
{
	this->~SLL();
}

template<typename key_type, typename value_type>
void SLL<key_type, value_type>::delete_first()
{
	if (first == nullptr) {
		return;
	}
	ListElement* new_first = first->next;
	delete first;
	first = new_first;
	elements_nr--;
}
// END OF DELETING ELEMENTS FUNCTIONS IMPLEMENTATIONS


//START OF MODYFYING  ELEMENTS FUNCTIONS IMPLEMENTATIONS
template<typename key_type, typename value_type>
inline bool SLL<key_type, value_type>::set_value(const key_type key, const value_type value, int occurence)
{
	ListElement* element = find(key, occurence);
	if (element == nullptr)
		return false;
	element->value = value;
	return true;
}

template<typename key_type, typename value_type>
inline bool SLL<key_type, value_type>::set_key(const key_type key, const key_type new_key, int occurence)
{
	ListElement* element = find(key, occurence);
	if (element == nullptr)
		return false;
	element->key = new_key;
	return true;
	
}
// END OF MODYFYING ELEMENTS FUNCTIONS IMPLEMENTATIONS


//START OF OTHER FUNCTIONS IMPLEMENTATIONS
template<typename key_type, typename value_type>
bool SLL<key_type, value_type>::is_in(const key_type key, int occurence) const
{
	ListElement* current = first;
	for (int count = 0; current != nullptr; current = current->next) {
		if (current->key == key) {
			count++;
			if (count == occurence) {
				return true;
			}
		}
	}
	return false;
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>::IteratorConst SLL<key_type, value_type>::begin() const
{
	return IteratorConst(first);
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>::IteratorVar SLL<key_type, value_type>::begin()
{
	return IteratorVar(first);
}

template<typename key_type, typename value_type>
int SLL<key_type, value_type>::get_element_nr()
{
	return elements_nr;
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>::ListElement* SLL<key_type, value_type>::find(const key_type key, int occurence) const
{
	ListElement* current = first;
	for (int count = 0; current != nullptr; current = current->next) {
		if (current->key == key) {
			count++;
			if (count == occurence)
				return current;
		}
	}
	return nullptr;
}
// END OF OTHER FUNCTIONS IMPLEMENTATIONS

/// ----- END OF SLL CLASS METHODS IMPLEMENTATIONS -----------


//START OF ITERATOR IMPLEMENTATION

template<typename key_type, typename value_type>
SLL<key_type, value_type>::IteratorBase::IteratorBase(ListElement* current) : current(current) {
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>:: IteratorBase& SLL<key_type, value_type>::IteratorBase::operator++() {
	if (current == nullptr)
		throw std::out_of_range("End of list already reached!");

	current = current->next;
	return *this;
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>::IteratorBase SLL<key_type, value_type>::IteratorBase::operator++(int) {
	if (current == nullptr)
		throw std::out_of_range("There is no other element!");
	IteratorBase old = *this;
	current = current->next;
	return *this;
}


template<typename key_type, typename value_type>
SLL<key_type, value_type>::IteratorVar::IteratorVar(ListElement* current) : IteratorBase(current) {
}
template<typename key_type, typename value_type>
SLL<key_type, value_type>::IteratorConst::IteratorConst(ListElement* current) : IteratorBase(current) {
}

template<typename key_type, typename value_type>
std::pair<key_type &, value_type &> SLL<key_type, value_type>::IteratorVar::operator*() {
	return std::pair<key_type &, value_type &>(this->current->key, this->current->value);
}

template<typename key_type, typename value_type>
std::pair<key_type, value_type> SLL<key_type, value_type>::IteratorConst::operator*() const{
	return std::pair<key_type, value_type>(this->current->key, this->current->value);
}

//END OF ITERATOR IMPLEMENTATION


//START OF LIST_ELEMENT IMPLEMENTATION
template<typename key_type, typename value_type>
SLL<key_type, value_type>::ListElement::ListElement(key_type key, value_type value, ListElement * next)
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
//END OF LIST ELEMENT IMPLEMENTATION

#endif