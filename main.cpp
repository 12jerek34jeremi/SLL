#include <iostream>
#include "SLL.h"
#include <string>
#include <cstdlib> 
#include <ctime>  

#define SHOW_FUNCTIONALITY


class Person {
public:

    int weight;
    bool sex; // true for woman, false for man
    int height;
    Person(int weight, bool sex, int height) : weight(weight), sex(sex), height(height) {
    }

    Person() : Person(rand() % 60 + 40, rand() % 2 == 0, rand() % 50 + 150) {

    }
    friend std::ostream& operator<<(std::ostream& stream, const Person & my_person){
        stream << "(weight: " << my_person.weight << ", sex: " << my_person.sex << ", height: " << my_person.height << ")";
        return stream;
    }
};

template class SLL<std::string, Person>;

void print_people_list(const SLL<std::string, Person> & list);
template<typename key_type, typename value_type> SLL<key_type, value_type> 
shuffle(const SLL<key_type, value_type> & list1, int len1, const SLL<key_type, value_type> & list2, int len2, int repeat);

void show_functionality();

int main() {
    srand((unsigned)time(0));

    //Start CREATING TWO LISTS
    SLL<std::string, Person>list1;
    SLL<std::string, Person> list2;
    std::string names[32] = { "Michalina", "John", "Julia", "Andrew", "Michael", "Simon", "Angela", "Michael",
    "Thomas", "George", "George", "Ryan", "Gary", "Eric", "Jonathan", "Brandon", "Brandon", "Aaron", "Kyle",
        "Christian", "Terry", "Arthur", "Jordan", "Logan", "Vincent", "Bobby", "Terry", "Jeremy", "Jose", "Patrick", "Jack", "Mathew"};
    for (int i = 0; i < 32; i+=2) {
        list1.insert_back(names[i], Person());
        list2.insert_back(names[i+1], Person());
    }
    //END CREATING TWO LISTS

    std::cout << "This is the first list: \n";
    print_people_list(list1);
    std::cout << "\n\nThis is the second list: \n";
    print_people_list(list2);
    std::cout << "\n\n\n Starting shuffling. \n";

    SLL<std::string, Person> shuffled_list = shuffle<std::string, Person>(list1, 3, list2, 1, 5);
    std::cout << "This is the shuffled list: \n";
    print_people_list(shuffled_list);

#ifdef SHOW_FUNCTIONALITY
    show_functionality();
#endif // SHOW_FUNCTIONALITY


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



template<typename key_type, typename value_type>  SLL<key_type, value_type> 
shuffle(const SLL<key_type, value_type> & list1, int len1, const SLL<key_type, value_type> & list2, int len2, int repeat) {
    int step = 1;
    SLL<key_type, value_type> result_list;

    for (auto iterator1 = list1.begin(), iterator2 = list2.begin();
        step <= repeat && (iterator1 != nullptr || iterator2 != nullptr); step++) {

        for (int count = 1; iterator1 != nullptr && count<=len1; ++iterator1, count++) {
            std::pair<key_type, value_type> my_pair = *iterator1;
            result_list.insert_back(my_pair.first, my_pair.second);
        }
        for (int count = 1; iterator2 != nullptr && count <= len2; ++iterator2, count++) {
            std::pair<key_type, value_type> my_pair = *iterator2;
            result_list.insert_back(my_pair.first, my_pair.second);
        }

    }

    return result_list;
}

void show_functionality() {
    std::cout << "Creating an empty list" << std::endl;
    SLL<std::string, Person> list1;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


    std::cout << "\n\nInserting one element to list1: " << std::endl;
    list1.insert_front("George", Person{ rand() % 60 + 40, rand() % 2 == 0,rand() % 50 + 150 });
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
    list1.insert_middle("Julia", Person(), "Angela", 1);
    list1.insert_middle("Scarlett", Person(), "Angela", 2);
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


    std::cout << "\n\nMaking an deep copy of a list:";
    const SLL<std::string, Person> list2 = list1;
    std::cout << "list1:" << std::endl;
    print_people_list(list1);
    std::cout << "list2:" << std::endl;
    print_people_list(list2);


    std::cout << "\n\nAccessing elements of an const object." << std::endl;
    Person my_person = list1.get_first();
    std::cout << "first person:"<<my_person << std::endl;
     my_person = list1.get_last();
    std::cout << "last person:" << my_person << std::endl;
     my_person = list1.get_value("Matilda");
    std::cout << "First person got by key 'Matilda':" << my_person << std::endl;
     my_person = list1.get_value("Matilda", 2);
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

    std::cout << "\n\nDeleting some elements from first ist";
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


    std::cout << "\n\nIterating through const list to print only names starting with 'A': " << std::endl;
    int i = 0;
    for (auto my_iterator = list2.begin(); my_iterator != nullptr; ++my_iterator) {
        std::pair<std::string, Person> my_pair = *my_iterator;
        if(my_pair.second.height>170)
            std::cout << i << ": " << my_pair.first << "; " << my_pair.second << std::endl;
    }

    std::cout << "\n\nIterating through var list:" << std::endl;
    for (auto my_iterator = list1.begin(); my_iterator != nullptr; ++my_iterator) {
        std::pair<std::string &, Person &> my_pair = *my_iterator;
        if (my_pair.first[0] == 'A')
            my_pair.first = "Afrodyta";

        if (my_pair.second.height > 175)
            my_pair.second.height = 200;
    }

    std::cout << "list1:" << std::endl;
    print_people_list(list1);

    std::cout << "Clearing list1:" << std::endl;
    list1.clear();
    std::cout << "list1:" << std::endl;
    print_people_list(list1);


}