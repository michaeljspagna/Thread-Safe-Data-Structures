#ifndef LOCK_BASED_LIST_HPP
#define LOCK_BASED_LIST_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
template <class T>
class lock_based_list
{
private:
    struct node
    {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node(): 
            next() 
        {}
        node(T const& value):
            data(std::make_shared<T>(value))
        {}

    };
    node head;
public:
    lock_based_list()
    {}
    ~lock_based_list()
    {
        remove_if([] (node const&) {return true;});
    }
    lock_based_list(lock_based_list const& other) = delete;
    lock_based_list& operator=(lock_based_list const& other) = delete;

    void push_front(T const& value);
    template<class Function>
    void for_each(Function f);
    template<class Predicate>
    std::shared_ptr<T> find_first_if(Predicate p);
    template<class Predicate>
    void remove_if(Predicate p);
};

#endif /* LOCK_BASED_LIST_HPP */