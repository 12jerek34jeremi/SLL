#ifndef SLL_H
#define SLL_H


template <typename key_type, typename value_type> class ListElement {
	public:
		key_type key;
		value_type value;
		ListElement<key_type, value_type>* next;
		ListElement(key_type key, value_type value);

#ifdef TESTING_MODE
		~ListElement() {
			std::cout << "key is: " << key << ", value is: " << value << ", DELETING!!" << std::endl;
		}
#endif // TESTING_MODE
};

template <typename key_type, typename value_type> ListElement <key_type, value_type> ::
ListElement(key_type key, value_type value) {
	this->key = key;
	this->value = value;
	this->next = nullptr;
}



template <typename key_type, typename value_type> class SLL {
private:
	ListElement<key_type, value_type>* first;
	ListElement<key_type, value_type>* last;
	ListElement<key_type, value_type>* last_but_one;
	int elements_nr;

public:
	SLL();
	int get_element_nr();
	void insert_front(key_type key, value_type value);
	void insert_back(key_type key, value_type value);
	bool insert_middle(key_type key, value_type value, key_type after); //inserting after given key 'after'
	value_type get_first();
	value_type get_last();
	value_type get_element(key_type key);
	void delete_first();
	void delete_last();
	bool delete_element(key_type key);
	void print_list();
	~SLL();


};



#endif // !


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

	ListElement<key_type, value_type>* last_but_one = first;
	while (last_but_one->next != last) {
		last_but_one = last_but_one->next;
	}
	delete last_but_one->next;
	last_but_one->next = nullptr;
	elements_nr--;

}

template<typename key_type, typename value_type> bool SLL<key_type, value_type>::delete_element(key_type key)
{
	ListElement<key_type, value_type>* to_delete = first;
	ListElement<key_type, value_type>* before = nullptr;
	while (to_delete != nullptr && to_delete->key != key) {
		before = to_delete;
		to_delete = to_delete->next;
	}
	if (to_delete == nullptr) {
		return false;
	}
	before->next = to_delete->next;
	delete to_delete;
	return true;

}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::print_list() {

	ListElement<key_type, value_type>* current = first;
	while (current != nullptr) {
		std::cout << "Element " << current->key << " is: " << current->value << std::endl;
		current = current->next;
	}
}

template<typename key_type, typename value_type>  SLL<key_type, value_type>:: SLL()
{
	first = nullptr;
	last = nullptr;
	elements_nr = 0;
}

template<typename key_type, typename value_type> int SLL<key_type, value_type>::get_element_nr()
{
	return elements_nr;
}


template<typename key_type, typename value_type> void SLL<key_type, value_type>::insert_front(key_type key, value_type value)
{
	if (first == nullptr) {
		first = new ListElement<key_type, value_type>(key, value);
		last = first;
	}
	else {
		ListElement<key_type, value_type> * new_first = new ListElement<key_type, value_type>(key, value);
		new_first->next = first;
		first = new_first;
	}
	elements_nr++;
}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::insert_back(key_type key, value_type value)
{
	if (last == nullptr) {
		first = new ListElement<key_type, value_type>(key, value);
		last = first;
	}
	else {
		last->next = new ListElement<key_type, value_type>(key, value);
		last = last->next;
	}
	elements_nr++;
}

template<typename key_type, typename value_type> bool SLL<key_type, value_type>::insert_middle(key_type key, value_type value, key_type after)
{
	ListElement<key_type, value_type>* current = first;
	while (current->key != after && current!=nullptr) {
		current = current->next;
	}
	if (current == nullptr) {
		return false;
	}
	ListElement <key_type, value_type>* new_element = new ListElement<key_type, value_type>(key, value);
	new_element->next = current->next;
	current->next = new_element;
	elements_nr++;
	return true;
}

template<typename key_type, typename value_type> value_type SLL<key_type, value_type>::get_first()
{
	if (first == nullptr) {
		return NULL;
	}
	return first->value;
}

template<typename key_type, typename value_type> value_type SLL<key_type, value_type>::get_last()
{
	if (last == nullptr) {
		return NULL;
	}
	return last->value;
}

template<typename key_type, typename value_type> value_type SLL<key_type, value_type>::get_element(key_type key)
{
	ListElement<key_type, value_type> current = first;
	while (current->key != key && current != nullptr) {
		current = current->next;
	}
	if (current == nullptr) {
		std::cout << "Element of key '" << key << "' wasn't found!"<<std::endl;
		return NULL;
	}
	return current->value;
}

template<typename key_type, typename value_type> void SLL<key_type, value_type>::delete_first()
{
	if (first == nullptr){
		return;
	}
	ListElement<key_type, value_type>* new_first = first->next;
	delete first;
	first = new_first;
	elements_nr--;
}

template<typename key_type, typename value_type> SLL<key_type, value_type>::~SLL()
{
	ListElement<key_type, value_type>* current = first;
	while (current != nullptr) {
		ListElement<key_type, value_type>* temp = current->next;
		delete current;
		current = temp;

	}
}

