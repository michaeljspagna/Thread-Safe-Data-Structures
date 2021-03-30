#include <functional>
#include <utility>
#include <list>
#include<vector>
#include <mutex>

template<class Key, class Value, class Hash=std::hash<Key>>
class lock_based_map
{
private:
    class bucket_type
    {
    private:
        typedef std::pair<Key, Value> bucket_value;
        typedef std::list<bucket_value> bucket_data;
        typedef typename bucket_data::iterator bucket_iterator;
        bucket_data data;
        mutable std::shared_mutex mutex;

        bucket_iterator find_entry_for(Key const& key) const
        {
            return std::find_if(data.begin()
                                ,data.end()
                                ,[&] (bucket_value const& item) 
                                {return item.first==key;});
        }
    public:
        
    };
};
