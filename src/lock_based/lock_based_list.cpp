#include "lock_based_list.hpp"

template<class T>
void 
lock_based_list<T>::push_front(T const& value)
{
    std::unique_ptr<node> new_node(new node(value));
    std::lock_guard<std::mutex> lock(head.m);
    new_node->next = std::move(head.next);
    head.next = std::move(new_node);
}

template<class T>
template<class Function>
void 
lock_based_list<T>::for_each(Function f)
{
    lock_based_list<T>::node* current = &head;
    std::unique_lock<std::mutex> lock(head.m);
    while(node* const next = current->next.get()) {
        std::unique_lock<std::mutex> next_lock(next->m);
        lock.unlock();
        f(*next->data);
        current=next;
        lock=std::move(next_lock);
    }
}

template<class T>
template<class Predicate>
std::shared_ptr<T> 
lock_based_list<T>::find_first_if(Predicate p)
{
    node* current = &head;
    std::unique_lock<std::mutex> lock(head.m);
    while(node* const next = current->next.get()) {
        std::unique_lock<std::mutex> next_lock(next->m);
        lock.unlock();
        if(p(*next->data)) {
            return next->data;
        }
        current=next;
        lock=std::move(next_lock);
    }
    return std::shared_ptr<T>();
}

template<class T>
template<class Predicate>
void 
lock_based_list<T>::remove_if(Predicate p)
{
    node* current = &head;
    std::unique_lock<std::mutex> lock(head.m);
    while(node* const next=current->next.get()) {
        std::unique_lock<std::mutex> next_lock(next->m);
        if(p(*next->data)) {
            std::unique_ptr<node> old_next = std::move(current->next);
            current->next = std::move(next->next);
            next_lock.unlock();
        } else {
            lock.unlock();
            current = next;
            lock = std::move(next_lock);
        }
    }
}