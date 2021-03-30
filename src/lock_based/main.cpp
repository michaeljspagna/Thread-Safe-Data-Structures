//#include "lock_based_stack.cpp"
//#include "lock_based_queue.cpp"
#include "lock_based_list.cpp"
#include <iostream>

// void stack()
// {
//     lock_based_stack<int> s;
//     s.push(7);
//     s.push(14);
//     s.push(21);
//     int x;
//     s.wait_and_pop(x);
//     std::cout << x << std::endl;
//     s.wait_and_pop(x);
//     std::cout << x << std::endl;
//     int z = 0;
//     bool y;
//     y = s.try_pop(z);
//     std::cout << z << std::endl;
//     std::cout << y << std::endl;
// }

// void queue()
// {
//     lock_based_queue<int> q;
//     q.push(7);
//     q.push(14);
//     int x;
//     q.wait_and_pop(x);
//     std::cout << x << std::endl;
//     q.wait_and_pop(x);
//     std::cout << x << std::endl;
// }

void prnt(int x)
{
    std::cout << x << std::endl;
}

void list()
{
    lock_based_list<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    l.for_each(prnt);
}



int main(int argc, char const *argv[])
{
    //stack();
    //queue();
    list();
    return 0;
}
