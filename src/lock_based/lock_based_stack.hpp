#ifndef LOCK_BASED_STACK_H
#define LOCK_BASED_STACK_H
#include <condition_variable>
#include <memory>
#include <mutex>

template<class T>
class lock_based_stack
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_condition;

    node* get_tail();
    std::unique_ptr<node> pop_head();
    std::unique_ptr<node> try_pop_head();
    std::unique_ptr<node> try_pop_head(T& value);
    std::unique_lock<std::mutex> wait_for_data();
    std::unique_ptr<node> wait_pop_head();
    std::unique_ptr<node> wait_pop_head(T& value);

public:
    lock_based_stack():
        head(new node), tail(head.get())
    {}
    lock_based_stack(const lock_based_stack& other) = delete;
    lock_based_stack& operator=(const lock_based_stack& other)=delete;

    std::shared_ptr<T> try_pop();
    bool try_pop(T& value);
    std::shared_ptr<T> wait_and_pop();
    void wait_and_pop(T& value);
    void push(T new_value);
    bool empty();
};
#endif /* LOCK_BASED_STACK_H */