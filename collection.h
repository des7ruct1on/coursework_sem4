
#ifndef COURSE_WORK_DATA_BASE_COLLECTION_H
#define COURSE_WORK_DATA_BASE_COLLECTION_H

#include "value.h"
#include "include/search_tree.h"
#include "include/b_tree.h"
#include "comparer.h"
#include "iostream"
#include "second_key_comparer.h"
#include "allocators/allocator_buddies_system.h"
#include "allocators/allocator_with_fit_mode.h"
#include "allocators/allocator_types.h"
#include "allocators/allocator.h"
#include "allocators/allocator_guardant.h"
#include "allocators/allocator_sorted_list.h"
#include "allocators/allocator_global_heap.h"
#include "database.h"
#include "flyweight/string_flyweight_factory.h"

class collection
{
public:

    associative_container<Key, Value*>* _data;

    allocator* _allocator_instance_collection;

    allocator_type _type_of_allocator_collection;

    allocator_with_fit_mode::fit_mode _allocator_fit_mode_collection;

public:

    collection(allocator* allocator_instance, allocator_type type, allocator_with_fit_mode::fit_mode fit_mode, const size_t& t = 8) : _type_of_allocator_collection(type), _allocator_fit_mode_collection(fit_mode), _allocator_instance_collection(allocator_instance)
    {
        _data = new b_tree<Key, Value*>(t, second_key_comparer());
    }

public:

    void add_value(Key& id, Value* value)
    {
        _data->insert(id, value);
    }
    void add_value(unsigned int id, long position, long str_size)
    {
        File_value* value = reinterpret_cast<File_value*>(_allocator_instance_collection->allocate(sizeof(File_value), 1));
        value->_position = position;
        value->_string_size = str_size;
        _data->insert(Key(id), dynamic_cast<class Value*>(value));
    }

    void add_value(unsigned int id, Value* value)
    {
        _data->insert(Key(id), value);
    }

    void add_value(unsigned int key, unsigned int id, std::string nickname, double salary)
    {
        Mem_value* mvalue = reinterpret_cast<Mem_value*>(_allocator_instance_collection->allocate(sizeof(Mem_value), 1));
        mvalue->_id = id;
        mvalue->_nickname = new char[nickname.size() + 1];
        mvalue->_nickname = string_flyweight_factory::get_instance().get_string_flyweight(nickname)->get_value();
        mvalue->_salary = salary;

        _data->insert(Key(key), dynamic_cast<Value*>(mvalue));
    }

    void update_value(unsigned int key, unsigned int id, std::string nickname, double salary)
    {
        Mem_value* mvalue = reinterpret_cast<Mem_value*>(_allocator_instance_collection->allocate(sizeof(Mem_value), 1));
        mvalue->_id = id;
        mvalue->_nickname = new char[nickname.size() + 1];
        mvalue->_nickname = string_flyweight_factory::get_instance().get_string_flyweight(nickname)->get_value();
        mvalue->_salary = salary;

        _data->dispose(Key(key));
        _data->insert(Key(key), dynamic_cast<Value*>(mvalue));
    }

    void update_value(unsigned int id, long position, long str_size)
    {
        File_value* fvalue = reinterpret_cast<File_value*>(_allocator_instance_collection->allocate(sizeof(File_value), 1));
        fvalue->_position = position;
        fvalue->_string_size = str_size;

        _data->dispose(Key(id));
        _data->insert(Key(id), dynamic_cast<Value*>(fvalue));
    }

public:

    void remove_value(Key const &key)
    {
        _data->dispose(key);
    }

public:

    const Value* get_value(Key const& data_key)
    {
        return _data->obtain(data_key);
    }

    std::vector<Value*> obtain_between_value(const Key& minbound, const Key& maxbound, bool is_inclusive_lower, bool is_inclusive_upper)
    {
        b_tree<Key, Value*>* tree = reinterpret_cast<b_tree<Key, Value*>*>(_data);
        std::vector<associative_container<Key, Value*>::key_value_pair> result = tree->obtain_between(minbound, maxbound, is_inclusive_lower, is_inclusive_upper);
        std::vector<Value*> result_for_return;
        for (auto it : result) {
            result_for_return.push_back(it.value);
        }
        return result_for_return;
    }

public:

    collection(collection const& other) : _allocator_instance_collection(other._allocator_instance_collection)
    {
        _data = new b_tree<Key, Value*>(*reinterpret_cast<b_tree<Key, Value*>*>(other._data));

    }

    collection& operator=(collection const& other)
    {
        if(this != &other)
        {
            delete _data;
            _allocator_instance_collection = other._allocator_instance_collection;
            _data = new b_tree<Key, Value*>(*reinterpret_cast<b_tree<Key, Value*>*>(other._data));
        }
        return *this;
    }

    collection(collection&& other) noexcept : _data(other._data)
    {
        other._data = nullptr;
        _allocator_instance_collection = std::move(other._allocator_instance_collection);
        other._allocator_instance_collection = nullptr;
    }

    collection& operator=(collection && other) noexcept
    {
        if(this != &other)
        {

            delete _data;
            _data = std::move(other._data);
            other._data = nullptr;

            _allocator_instance_collection = other._allocator_instance_collection;
            other._allocator_instance_collection = nullptr;
        }
        return *this;
    }

    ~collection()
    {
        delete _data;
    }
};

#endif //COURSE_WORK_DATA_BASE_COLLECTION_H
