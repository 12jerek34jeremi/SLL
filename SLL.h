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
	/*
	This class represents just one node from a list.
	*/

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

/*
			----- ITERATORS DESCRIPTION ------
	To be able to both iterate through constant list and iterate through normal (variable) list this SLL class has two itarators,
	namely ItaratorVar and IteratorConst. They both inherit from ItaratorBase class.
	For both Iterators (from IteratorBase class):
		There is implemented postfix and prefix incrementation operator. It moves Iterator one node up. If the iterator already
		points on the last element of the list the incrementation will result in pointing on nothing (a nullptr).
		Incrementing an Iterator poining on nothing raises an 'out of range' exception.
		
		Operators != and == are also implemented. They check if two iterators points on the exactly the same node.
		If you want to check if you can increment an iterator, simply do: if(my_iterator != nullptr){ ++ my_iterator;}
		If my_iterator==nullptr is true, it means that your iterator already points on nothing and you can't increment it;

	For both iterators (var and const) indirection operator (*) are implemented but they do different things.
		Operator * from IteratorVar returns a pair (std::pair) of references to key and value of node
		pointed by the itarator. This way you can acces and also modify values.

		Operator * from IteratorConst returns a pair (std::pair) of copy of key and value of node
		pointed by the itarator. This way you can only acces key and values, you can't change them.

	Calling begin() on normal (variable) SLL object will return IteratorVar object pointing on the first node of a list.
	If list is empty it will point on nothing.

	Callinng begin on constant SLL object will return IteratorConst object pointing on the first node of the list.
	If list is empty it will point on nothing.
	
	Tou can see how to use this two operators in the last part of show_functionality function from main.cpp file.
	Two for loops were created there just to show how to use those iterators.
*/
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
	//constructors, destructor, and assignment operator (more description in functions definitions)
	SLL();
	~SLL();
	SLL(const SLL& source); //copy constructor, it creates a DEEP copy of an object
	SLL(SLL&& source); //move constructor, creates a SHALLOW copy and prevents destructor from deleting all elements.
	SLL& operator=(const SLL& source); //delete all nodes and creates a deep copy

	//Functions for inserting new elements to the lists.  (more description in functions definitions)
	void insert_front(const key_type key, const value_type value);
	void insert_back(const key_type key, const value_type value);
	bool insert_after(const key_type key, const value_type value, const key_type after, int occurence=1); //inserts new element after
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
	IteratorConst begin() const; //see on ITERATORS DESCRIPTION for info about this function
	IteratorVar begin();//see on ITERATORS DESCRIPTION for info about this function
	int get_element_nr(); //returns a length of a list
};

/// ----- START OF SLL CLASS METHODS IMPLEMENTATIONS -----------

// START OF CONSTRUCTORS IMPLEMENTATIONS
template<typename key_type, typename value_type>
SLL<key_type, value_type>::SLL()
{
	/*
		This constructor just creates an empty list.
	*/
	first = nullptr;
	last = nullptr;
	elements_nr = 0;
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>::~SLL()
{
	/*
	Destructor frees memory alocated for each node and makes sure that pointer on first
	element of the list and last element of this list is nullptr.
	*/

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
	/*
		This is move construcotor, so there is no need to create  deep copy while in some function
		returning list created within this funcions. It creates a shallow copy of a list and set
		pointers on first and last elements in source list to nullptr to make sure that destructor
		do not clear that list.
	*/

	source.first = nullptr;
	source.last = nullptr;
	source.elements_nr = 0;
	
}

template<typename key_type, typename value_type>
SLL<key_type, value_type>& SLL<key_type, value_type>:: operator=(const SLL & source){
	/*
	Free memory allocated for all current nodes and makes a deep copy of source SLL.
	*/

	this->~SLL();
	for (ListElement* current = source.first; current != nullptr; current = current->next) {
		this->insert_back(current->key, current->value);
	}
	return *this;
}

template<typename key_type, typename value_type> SLL<key_type, value_type>::
SLL(const SLL& source) : first(nullptr), last(nullptr), elements_nr(0) {
	/*
		Creates a deep copy of a given list.
	*/


	for (ListElement* current = source.first; current != nullptr; current = current->next) {
		this->insert_back(current->key, current->value);
	}
}
//END OF CONSTRUCTORS IMPLEMENTATIONS


//START OF INSERTING ELEMENTS FUNCTIONS IMPLEMENTATIONS
template<typename key_type, typename value_type>
void SLL<key_type, value_type>::insert_front(const key_type key, const value_type value)
{
	/*
		It inserts new node in the front of the list. Key for new node is 'key' argument
		and value of new node is 'value' argument.
	*/
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
	/*
		It inserts new node in the end of the list. Key for new node is 'key' argument
		and value of new node is 'value' argument.
	*/
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
bool SLL<key_type, value_type>::insert_after(const key_type key, const value_type value, const key_type after, int occurence)
{
	/*
		It inserts new node after n-th node which key is same as 'after' argument, where n is 'occurence' argument.
		Key for new node is 'key' argument and value of new node is 'value' argument.
	*/
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
	/*
		Returns value of n-th node of key given by 'key' argument, where n is 'occurence' argument.
		Throws an 'out_of_range' exception when there are less than n nodes with given key.
	*/
	ListElement* element = find(key, occurence);
	if(element == nullptr)
		throw std::out_of_range("The element of given key is not in this list!");

	return element->value;
}

template<typename key_type, typename value_type>
value_type SLL<key_type, value_type>::get_first() const
{
	/*
		Returns value of a first node of list (as copy).
		Throws an 'out_of_range' exception if list is empty.
	*/
	if (first == nullptr)
		throw std::out_of_range("This list is empty, there is no first element!");

	return first->value;
}

template<typename key_type, typename value_type>
value_type SLL<key_type, value_type>::get_last() const
{
	/*
		Returns value of a last node of list (as copy).
		Throws an 'out_of_range' exception if list is empty.
	*/
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
	/*
		Deletes last node from the list. Do nothing if list is empty.
	*/

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
	/*
	Deletes the n-th node of key given by 'key' argument, where n is 'occurence' argument.
		Throws an 'out_of_range' exception when there are less than n nodes with given key.
	*/
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
	/*
	Deletes all elements (nodes) from the list.
	*/
	this->~SLL();
}

template<typename key_type, typename value_type>
void SLL<key_type, value_type>::delete_first()
{
	/*
		Deletes last node from the list. Do nothing if list is empty.
	*/
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
bool SLL<key_type, value_type>::set_value(const key_type key, const value_type new_value, int occurence)
{
	/*
	 Change value of n-th node of key given by 'key' argument, where n is 'occurence' argument.
	 Value will be changed to new value given by 'new_value' argument.

	 If there are less than n nodes with given key it returns false (list isn't altered then).
	 Otherwise returns true.
	*/
	ListElement* element = find(key, occurence);
	if (element == nullptr)
		return false;
	element->value = new_value;
	return true;
}

template<typename key_type, typename value_type>
bool SLL<key_type, value_type>::set_key(const key_type key, const key_type new_key, int occurence)
{
	/*
	 Change key of n-th node of key given by 'key' argument, where n is 'occurence' argument.
	 Key will be changed to new key given by 'new_key' argument.

	 If there are less than n nodes with given key it returns false (list isn't altered then).
	 Otherwise returns true.
	*/
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
	/*
	Returns true if there are at lest n nodes of key given by 'key' argument, where n is 'occurence' argument".
	Otherwise returns false.
	*/ListElement* current = first;
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
	/*
		Returns an IteratorConst object pointig on the firs element of the list. This iterator enable
		only to acces without modyfing list elements. See ITERATORS DESCRIPTION at the top of
		SLL class definition for more info.
	*/
	return IteratorConst(first);
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>::IteratorVar SLL<key_type, value_type>::begin()
{
	/*
		Returns an IteratorVar object pointig on the firs element of the list. This iterator enable
		to both acces and modify list elements. See ITERATORS DESCRIPTION at the top of
		SLL class definition for more info.
	*/
	return IteratorVar(first);
}

template<typename key_type, typename value_type>
int SLL<key_type, value_type>::get_element_nr()
{
	/*
	Returns the number of elements that are currently in the lists (the current length of the list)
	*/
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
	/*
		It moves Iterator one node up. If the iterator already points on the last element
		of the list the incrementation will result in an iterator pointing on nothing (a nullptr).
		Incrementing an Iterator poining on nothing raises an 'out of range' exception.

		(First increments and  then return).
	*/

	if (current == nullptr)
		throw std::out_of_range("End of list already reached!");

	current = current->next;
	return *this;
}

template<typename key_type, typename value_type>
typename SLL<key_type, value_type>::IteratorBase SLL<key_type, value_type>::IteratorBase::operator++(int) {
	/*
	It moves Iterator one node up. If the iterator already points on the last element
	of the list the incrementation will result in an iterator pointing on nothing (a nullptr).
	Incrementing an Iterator poining on nothing raises an 'out of range' exception.

		(First saves copy of itself, then increments and  then return the copy.
			So it return's itself before incrementation.).
	*/

	if (current == nullptr)
		throw std::out_of_range("End of list already reached!");
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
	/*
		Returns a pair (std::pair) of references of key and value of node pointed by this iterator.
		Raise an 'out_of_range' exception when operator points on nothing.
		(See ITERATORS DESCRIPTION for more info).
	*/
	if (IteratorBase::current == nullptr)
		throw std::out_of_range("This iterator points on nothing.\n"
			"Propably you alredy iterated through whole list, the list is empty or list, from which this iterator was created, was cleard or delete");
	return std::pair<key_type &, value_type &>(this->current->key, this->current->value);
}

template<typename key_type, typename value_type>
std::pair<key_type, value_type> SLL<key_type, value_type>::IteratorConst::operator*() const{
	/*
		Returns a pair (std::pair) of copies of key and value of node pointed by this iterator.
		Raise an 'out_of_range' exception when operator points on nothing.
		(See ITERATORS DESCRIPTION for more info).
	*/
	if (IteratorBase::current == nullptr)
		throw std::out_of_range("This iterator points on nothing.\n"
			"Propably you alredy iterated through whole list, the list is empty or list, from which this iterator was created, was cleard or delete");
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