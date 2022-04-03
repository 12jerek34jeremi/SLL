#include <iostream>
#include "SLL.h"
#include <string>
#include <cstdlib> 
#include <ctime>  

using namespace std;

typedef struct {
    int weight;
    bool sex; // true for woman, false for man
    int height;
}Person;
template class SLL<std::string, Person>;

void print_people_list(const SLL<std::string, Person> & list);
template<typename key_type, typename value_type> SLL<key_type, value_type> 
shuffle(const SLL<key_type, value_type> & list1, int len1, const SLL<key_type, value_type> & list2, int len2, int repeat);

int main() {
    srand((unsigned)time(0));

    //Start CREATING TWO LISTS
    SLL<std::string, Person>list1;
    SLL<std::string, Person> list2;
    std::string names[32] = { "Michalina", "John", "Julia", "Andrew", "Michael", "Simon", "Angela", "Michael",
    "Thomas", "George", "George", "Ryan", "Gary", "Eric", "Jonathan", "Brandon", "Brandon", "Aaron", "Kyle",
        "Christian", "Terry", "Arthur", "Jordan", "Logan", "Vincent", "Bobby", "Terry", "Jeremy", "Jose", "Patrick", "Jack", "Mathew"};
    for (int i = 0; i < 32; i+=2) {
        list1.insert_back(names[i], Person{ rand() % 60 + 40, rand() % 2 == 0,rand() % 50 + 150 });
        list2.insert_back(names[i+1], Person{ rand() % 60 + 40, rand() % 2 == 0,rand() % 50 + 150 });
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

 

    return 0;
}

void print_people_list(const SLL<std::string, Person> & list) {
    int i=0;
    for (auto my_iterator = list.begin(); my_iterator != nullptr; ++my_iterator, i++) {
        std::pair<std::string, Person> my_pair = *my_iterator;
        std::cout << i << ": " << my_pair.first << "; weight: " << my_pair.second.weight << ", sex: "
            << my_pair.second.sex << ", height: " << my_pair.second.height << std::endl;
    }
}



template<typename key_type, typename value_type>  SLL<key_type, value_type> 
shuffle(const SLL<key_type, value_type> & list1, int len1, const SLL<key_type, value_type> & list2, int len2, int repeat) {
    int step = 1;
    SLL<key_type, value_type> result_list;

    for (typename SLL<key_type, value_type>::Iterator iterator1 = list1.begin(), iterator2 = list2.begin();
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