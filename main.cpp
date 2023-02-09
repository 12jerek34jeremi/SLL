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
    std::cout << "Creating an empty list" << std::endl;
    SLL<std::string, Person> list1;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


    std::cout << "\n\nInserting one element to list1: " << std::endl;
    list1.insert_front("George", Person());
    std::cout << "list1:" << std::endl;
    print_people_list(list1);

    std::cout << "\n\nInserting two elements at the beginning and at the end of list1: " << std::endl;
    list1.insert_front("Angela", Person());
    list1.insert_front("Angela", Person());
    list1.insert_back("Matilda", Person());
    list1.insert_back("Matilda", Person());
    std::cout << "list1:" << std::endl;
    print_people_list(list1);

    std::cout << "\n\nInserting person after first Angela, and after second Angela." << std::endl;
    list1.insert_after("Julia", Person(), "Angela", 1);
    list1.insert_after("Scarlett", Person(), "Angela", 2);
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


    std::cout << "\n\nMaking an deep copy of a list:\n";
    const SLL<std::string, Person> list2 = list1;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    std::cout << "\n\nAccessing elements of an const object." << std::endl;
    Person my_person = list2.get_first();
    std::cout << "first person:"<<my_person << std::endl;
     my_person = list2.get_last();
    std::cout << "last person:" << my_person << std::endl;
     my_person = list2.get_value("Matilda");
    std::cout << "First person got by key 'Matilda':" << my_person << std::endl;
     my_person = list2.get_value("Matilda", 2);
    std::cout << "Second person got by key 'Matilda':" << my_person << std::endl;


    std::cout << "\n\nModyfying key and values of first list." << std::endl;
    list1.set_value("Angela", Person(75, 1, 175));
    list1.set_value("Angela", Person(80, 1, 185), 2);
    list1.set_key("Matilda", "Hermiona", 2);
    list1.set_key("Julia", "Monica");
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);

    std::cout << "\n\nDeleting some elements from first the list\n";
    list1.delete_element("Angela", 2);
    list1.delete_first();
    list1.delete_last();
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);

    std::cout << "\n\nCreatig deep copy of list2 to list1:" << std::endl;
    list1 = list2;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    std::cout << "\n\nIterating through const list to print only people taller than 170: " << std::endl;
    int i = 0;
    for (auto my_iterator = list2.begin(); my_iterator != nullptr; ++my_iterator, ++i) {
        std::pair<std::string, Person> my_pair = *my_iterator;
        if(my_pair.second.height>170)
            std::cout << i << ": " << my_pair.first << "; " << my_pair.second << std::endl;
    }

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

    std::cout << "\n\nClearing list1:" << std::endl;
    list1.clear();
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


}