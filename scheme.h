
#ifndef COURSE_WORK_DATA_BASE_SCHEME_H
#define COURSE_WORK_DATA_BASE_SCHEME_H

#include "database.h"
#include "collection.h"
#include "include/associative_container.h"
#include "include/b_tree.h"
#include "comparer.h"
#include "allocators/allocator_buddies_system.h"
#include "allocators/allocator_with_fit_mode.h"
#include "allocators/allocator_types.h"
#include "allocators/allocator.h"
#include "allocators/allocator_guardant.h"
#include "allocators/allocator_sorted_list.h"
#include "allocators/allocator_global_heap.h"

class scheme
{

public:

    associative_container<std::string, collection> *_scheme;


public:

    scheme(const size_t& t = 8) {
        _scheme = new b_tree<std::string, collection>(t, key_comparer());
    }


public:

    void add_collection(std::string const& name_collection, allocator* allocator_instance, allocator_type type, allocator_with_fit_mode::fit_mode fit_mode)
    {
        _scheme->insert(name_collection, collection(allocator_instance, type, fit_mode));
    }

    void remove_collection(std::string const& name_collection)
    {
        _scheme->dispose(name_collection);
    }

public:

    const collection& obtain_collection(std::string const& name_collection)
    {
        return _scheme->obtain(name_collection);
    }

public:
//TODO:
    scheme(scheme const &other)
    {
        _scheme = new b_tree<std::string, collection>(*dynamic_cast<b_tree<std::string, collection>*>(other._scheme));
    }

    scheme &operator=(scheme const &other)
    {
        if (this != &other)
        {
            delete this->_scheme;
            _scheme =  new b_tree<std::string, collection>(*dynamic_cast<b_tree<std::string, collection>*>(other._scheme));
        }
        return *this;
    }

    scheme(scheme &&other)  noexcept: _scheme(std::move(other._scheme))
    {
        other._scheme = nullptr;
    }

    scheme &operator=(scheme &&other) noexcept
    {
        if(this != &other)
        {

            delete this->_scheme;
            _scheme = std::move(other._scheme);
            other._scheme = nullptr;
        }
        return *this;
    }

    ~scheme()
    {
        //delete this->_scheme;
    }
};


#endif //COURSE_WORK_DATA_BASE_SCHEME_H