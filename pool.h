
#ifndef COURSE_WORK_DATA_BASE_POOL_H
#define COURSE_WORK_DATA_BASE_POOL_H

#include "database.h"
#include "scheme.h"
#include "comparer.h"
#include "allocators/allocator_global_heap.h"

class pool
{

public:
    associative_container<std::string, scheme>* _pool;
    
public:

    pool(const size_t& t = 8)
    {
        _pool = new b_tree<std::string, scheme>(t, key_comparer());
    }

public:

    void add_scheme(std::string const& scheme_name)
    {
        _pool->insert(scheme_name, scheme(8));
    }

    void remove_scheme(std::string const& scheme_name)
    {
        _pool->dispose(scheme_name);
    }

    const scheme& obtain_scheme(std::string const& scheme_name)
    {
        return _pool->obtain(scheme_name);
    }

public:

    pool(pool const &other)
    {
        _pool = new b_tree<std::string, scheme>(*dynamic_cast<b_tree<std::string, scheme>*>(other._pool));
    }


    pool &operator=(pool const &other)
    {
        if (this != &other)
        {
            delete _pool;
            _pool = new b_tree<std::string, scheme>(*dynamic_cast<b_tree<std::string, scheme>*>(other._pool));
        }

        return *this;
    }

    pool(pool&& other) noexcept
    {
        this->_pool = other._pool;
        other._pool = nullptr;
    }

    pool &operator=(pool &&other) noexcept
    {
        if(this != &other)
        {
            delete this->_pool;
            _pool = other._pool;
            other._pool= nullptr;
        }
        return *this;

    }

    ~pool()
    {
        delete _pool;
    }

};

#endif //COURSE_WORK_DATA_BASE_POOL_H
