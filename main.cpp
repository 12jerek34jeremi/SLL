#include <iostream>
#include "SLL.h"

int main() {
    SLL<int, double> my_list;
    my_list.insert_back(0, 1.6);
    my_list.insert_back(1, 2.4);
    my_list.insert_front(2, 0.5);
    my_list.insert_front(3, 1.0);
    my_list.insert_middle(4, 1.5, 0);

    my_list.print_list();
    return 0;
}
