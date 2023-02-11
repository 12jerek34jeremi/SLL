#include <iostream>
#include "SLL.h"
#include <string>
#include <cstdlib> 
#include <ctime>  

#define SHOW_FUNCTIONALITY

/*
To test if the list implementation is working I created the 'Person' class. I tested list different pairs of key and value
types, including std::string as a key and Person as value.
*/

class Person {
public:

    int weight;
    bool sex; // true for woman, false for man
    int height;
    Person(int weight, bool sex, int height) : weight(weight), sex(sex), height(height) {
    }

    Person() : Person(rand() % 60 + 40, rand() % 2 == 0, rand() % 50 + 150) {
        //creates a ranom person.
    }
    friend std::ostream& operator<<(std::ostream& stream, const Person & my_person){
        stream << "(weight: " << my_person.weight << ", sex: " << my_person.sex << ", height: " << my_person.height << ")";
        return stream;
    }
};
template class SLL<std::string, Person>;



void show_functionality();
void print_people_list(const SLL<std::string, Person>& list);

int main() {
    srand((unsigned)time(0));

    show_functionality();

    return 0;
}


void print_people_list(const SLL<std::string, Person> & list) {
    int i=0;
    for (auto my_iterator = list.begin(); my_iterator != nullptr; ++my_iterator, i++) {
        std::pair<std::string, Person> my_pair = *my_iterator;
        std::cout << i << ": " << my_pair.first << "; " << my_pair.second << std::endl;
    }
    if (i == 0) {
        std::cout << "(Empty List)" << std::endl;
    }
}

void show_functionality() {
    /*This is how you can create an empy list.*/
    std::cout << "Creating an empty list" << std::endl;
    SLL<std::string, Person> list1;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);

    /*You can use method "insert_front" to insert new element to the front of the list. First argument
     of this function is key, second is value.*/
    std::cout << "\n\nInserting one element to list1: " << std::endl;
    list1.insert_front("George", Person());
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


    /*You can use method "insert_back" to insert elements at the end of the list.
    Belowe code adds two elements to the front of the list and the back of the list.*/
    std::cout << "\n\nInserting two elements at the beginning and at the end of list1: " << std::endl;
    list1.insert_front("Angela", Person());
    list1.insert_front("Angela", Person());
    list1.insert_back("Matilda", Person());
    list1.insert_back("Matilda", Person());
    std::cout << "list1:" << std::endl;
    print_people_list(list1);

    /*You can use method "insert_after" to insert new element aftet an element of specified key.
    In the list there can be multiple elements of the same key,
    that's why method "insert_after" has argument "int occurrence".
    The method inserts new node after n-th node which key is same as 'after' argument, where n is 'occurence' argument.
    The default value of "occurence" argument is 1.
    Below code will insert two elements of keys Julia and Scarlett after first and second element of key "Angela".
    */
    std::cout << "\n\nInserting person after first Angela, and after second Angela." << std::endl;
    list1.insert_after("Julia", Person(), "Angela", 1);
    list1.insert_after("Scarlett", Person(), "Angela", 2);
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


    /*
    You can use copy constructor to make a deep copy of the list.
    */
    std::cout << "\n\nMaking an deep copy of a list:\n";
    const SLL<std::string, Person> list2 = list1;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    /*To access the list elements you can use "get_first", "get_last" and "get_value" methods.
    First one returns value of the first element, second method returns value of the last element, 
    third method returns value of the n'th element of specified key. (n i is value of second argument, default 1).*/
    std::cout << "\n\nAccessing elements of an const object." << std::endl;
    Person my_person = list2.get_first();
    std::cout << "first person:"<<my_person << std::endl;
     my_person = list2.get_last();
    std::cout << "last person:" << my_person << std::endl;
     my_person = list2.get_value("Matilda");
    std::cout << "First person got by key 'Matilda':" << my_person << std::endl;
     my_person = list2.get_value("Matilda", 2);
    std::cout << "Second person got by key 'Matilda':" << my_person << std::endl;


    /*You can access values of both const and non-const list, but you can modify only elements of non-const list.
    You can modify keys and values of a non-const list by using methods "set/_value" and "set_key".
    First method set value of the n'th element of specified key, second method sets key of the n'th element of specified key.
    (n i is value of last argument "occurnce", default 1).
    Below code will change the value of first and second element of key "Angela" and change key of second "Matilda"
    to "Hermiona" and first "Julia" to "Monica".
    */
    std::cout << "\n\nModyfying key and values of first list." << std::endl;
    list1.set_value("Angela", Person(75, 1, 175));
    list1.set_value("Angela", Person(80, 1, 185), 2);
    list1.set_key("Matilda", "Hermiona", 2);
    list1.set_key("Julia", "Monica");
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    /*You can use "delete_first" and "delete_last" methods to delete the first and last elements of the list.
    You can also use method "delete_element" to delete element of specified key.
    Method "delete_element" deletes the n-th node of key given by 'key' argument, where n is 'occurence' argument.*/
    std::cout << "\n\nDeleting some elements from first the list\n";
    list1.delete_element("Angela", 2);
    list1.delete_first();
    list1.delete_last();
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    /*I will now copy (deeply) list2 into list1:*/
    std::cout << "\n\nCreatig deep copy of list2 to list1:" << std::endl;
    list1 = list2;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    /*A "begin" method will return the iterator pointing to the first element.
    Calling "begin" on non-const list returns an iterator which enables both to access and modify elements,
    while callling "begin" returns an iterator which enables only to access elements.
    To be more precise calling "begin" on non-const list returns object of class IteratorVar,
    while calling "begin" on const list returns object of class IteratorConst.

    You can increment or decrement the iterator to go to next or previous node.
    Incrementing an iterator pointing to the last element results in nullptr,
    decrementing an iterator pointing to the first element results in nullptr.

    For both classes (IteratorVar and IteratorConst) indirection operators (*) are implemented but they do different things.

    Operator * from IteratorVar returns a pair (std::pair) of references to key and value of node pointed by the itarator.
    This way you can acces and also modify values.

    Operator * from IteratorConst returns a pair (std::pair) of copy of key and value of node pointed by the itarator.
    This way you can only acces key and values, you can't change them.

    Below code shows how to use iterators to print all people heigher than 170.
*/
    std::cout << "\n\nIterating through const list to print only people taller than 170: " << std::endl;
    int i = 0;
    for (auto my_iterator = list2.begin(); my_iterator != nullptr; ++my_iterator, ++i) {
        std::pair<std::string, Person> my_pair = *my_iterator;
        if(my_pair.second.height>170)
            std::cout << i << ": " << my_pair.first << "; " << my_pair.second << std::endl;
    }

    /*
    
    Below code shows how to use iterators to set all keys starting with letter 'A' to "Afrodyta"
    and set height of all people heigher than 170 to 200.
    */
    std::cout << "\n\nIterating through var list and altering some keys and values:" << std::endl;
    for (auto my_iterator = list1.begin(); my_iterator != nullptr; ++my_iterator) {
        std::pair<std::string &, Person &> my_pair = *my_iterator;
        if (my_pair.first[0] == 'A')
            my_pair.first = "Afrodyta";

        if (my_pair.second.height > 175)
            my_pair.second.height = 200;
    }

    std::cout << "list1:" << std::endl;
    print_people_list(list1);

    /*Here is how you can clear the list:*/
    std::cout << "\n\nClearing list1:" << std::endl;
    list1.clear();
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


}