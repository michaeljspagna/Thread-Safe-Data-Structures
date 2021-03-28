#include "impl/lock_based_queue.cpp"
#include "impl/lock_based_stack.cpp"
#include <iostream>
// int main(int argc, char const *argv[])
// {
//     lock_based_queue<int> q;
//     q.push(7);
//     q.push(14);
//     int x;
//     q.wait_and_pop(x);
//     std::cout << x << std::endl;
//     q.wait_and_pop(x);
//     std::cout << x << std::endl;
//     return 0;
// }

int main(int argc, char const *argv[])
{
    lock_based_stack<int> s;
    s.push(7);
    s.push(14);
    int x;
    s.wait_and_pop(x);
    std::cout << x << std::endl;
    s.wait_and_pop(x);
    std::cout << x << std::endl;
    int z = 0;
    bool y;
    y = s.try_pop(z);
    std::cout << z << std::endl;
    std::fputs(x ? "true" : "false", stdout);
    return 0;
}

