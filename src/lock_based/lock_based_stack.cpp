#include "lock_based_stack.hpp"

template<class T>
typename lock_based_stack<T>::node* 
lock_based_stack<T>::get_tail()
{
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
}

template<class T>
std::unique_ptr<typename lock_based_stack<T>::node> 
lock_based_stack<T>::pop_head()
{
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
}

template<class T>
std::unique_ptr<typename lock_based_stack<T>::node> 
lock_based_stack<T>::try_pop_head()
{
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if(head.get()==get_tail())
    {
        return std::unique_ptr<node>();
    }
    return pop_head();
}

template<class T>
std::unique_ptr<typename lock_based_stack<T>::node>  
lock_based_stack<T>::try_pop_head(T& value)
{
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if(head.get()==get_tail())
    {
        return std::unique_ptr<node>();
    }
    value=std::move(*head->data);
    return pop_head();
}

template<class T>
std::unique_lock<std::mutex> 
lock_based_stack<T>::wait_for_data()
{
    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_condition.wait(
        head_lock, [&]{return head.get() != get_tail();}
    );
    return std::move(head_lock);
}

template<class T>
std::unique_ptr<typename lock_based_stack<T>::node> 
lock_based_stack<T>::wait_pop_head()
{
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
}

template<class T>
std::unique_ptr<typename lock_based_stack<T>::node> 
lock_based_stack<T>::wait_pop_head(T& value)
{
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    value = std::move(*head->data);
    return pop_head();
}

template<class T>
std::shared_ptr<T> 
lock_based_stack<T>::try_pop()
{
    std::unique_ptr<node> const old_head=try_pop_head();
    return old_head?old_head->data:std::shared_ptr<T>();
}

template<class T>
bool 
lock_based_stack<T>::try_pop(T& value)
{
    std::unique_ptr<node> const old_head=try_pop_head(value);
    return old_head != nullptr;
}

template<class T>
std::shared_ptr<T> 
lock_based_stack<T>::wait_and_pop()
{
    std::unique_ptr<node> const old_head = wait_pop_head();
    return old_head->data;
}

template<class T>
void 
lock_based_stack<T>::wait_and_pop(T& value)
{
    std::unique_ptr<node> const old_head = wait_pop_head(value);
}

template<class T>
void 
lock_based_stack<T>::push(T new_value)
{
    std::shared_ptr<T> new_data( 
        std::make_shared<T>(std::move(new_value))
    );
    std::unique_ptr<node> ptr(new node);
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        node* const new_head = ptr.get();
        new_head->data = new_data;
        new_head->next = std::move(head);
        head = std::move(ptr);
    }
    data_condition.notify_one();
}

template<class T>
bool 
lock_based_stack<T>::empty()
{
    std::lock_guard<std::mutex> head_lock(head_mutex);
    return (head.get() == get_tail());
}