
#ifndef COURSE_WORK_DATA_BASE_DATABASE_H
#define COURSE_WORK_DATA_BASE_DATABASE_H

#include "pool.h"
#include "include/b_tree.h"
#include "include/associative_container.h"
#include "comparer.h"
#include "collection.h"
#include "allocators/allocator_buddies_system.h"
#include "allocators/allocator_sorted_list.h"
#include "allocators/allocator_with_fit_mode.h"
#include "allocators/allocator_types.h"
#include "allocators/allocator.h"
#include "allocators/allocator_guardant.h"
#include "allocators/allocater_boundary_tags.h"
#include "allocators/allocator_global_heap.h"
#include "allocators/allocator_red_black_tree.h"



class data_base
{
private:

    static data_base* _instance;

    bool _mode;

    allocator* _allocator_instance;

    logger* _log;

public:

    void set_mode(bool mode) {
        this->_mode = mode;
    }

    bool get_mode() {
        return this->_mode;
    }

    logger* get_logger() {
        return _log;
    }


    static data_base* get_instance(const size_t& t = 8, logger* log = nullptr) {
        if(_instance == nullptr)
        {
            _instance = new data_base(t, log);
            if (log) _instance->_log->trace("DB created");
        }
        
        return _instance;
    }


public:

    associative_container<std::string, pool>* _database_entrypoint;

public:

    data_base(const size_t t = 8, logger* log = nullptr)
    {
        _log = log;
        _database_entrypoint = new b_tree<std::string, pool>(t, key_comparer());
    }

    ~data_base() {
        delete _database_entrypoint;
    }

    data_base(data_base const &other) = delete;

    data_base(data_base &&other) =  delete;
     
    data_base &operator=(data_base &other) = delete;
     
    data_base &operator=(data_base &&other) = delete;

public:

    void add_pool(std::string const& pool_name) {
        if (_log) _log->trace("adding pool: " + pool_name);
        _database_entrypoint->insert(pool_name, pool(8));
    }

    void remove_pool(std::string const& pool_name) {
        if (_log) _log->trace("removing pool: " + pool_name);
        _database_entrypoint->dispose(pool_name);
    }

    const pool& obtain_pool(const std::string & pool_name) {
        if (_log) _log->trace("obtaining pool: " + pool_name);
        return _database_entrypoint->obtain(pool_name);
    }


    void add_scheme(std::string const& pool_name, std::string const& name_scheme) {
        if (_log) _log->trace("adding schema: " + pool_name + "/" + name_scheme);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        pl.add_scheme(name_scheme);

    }

    void remove_scheme(std::string const& pool_name, std::string const& scheme_name) {
        if (_log) _log->trace("removing schema: " + pool_name + "/" + scheme_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        pl.remove_scheme(scheme_name);
    }

    void add_collection(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, allocator_type type, allocator_with_fit_mode::fit_mode fit_mode) {
        if (_log) _log->trace("adding collection: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        switch(type) {
            case allocator_type::buddies_system:
                _allocator_instance = new allocator_buddies_system(32, nullptr, nullptr, fit_mode);
                break;
            case allocator_type::sorted_list:
                _allocator_instance = new allocator_sorted_list(1000, nullptr, nullptr, fit_mode);
                break;
            case  allocator_type::boundary_tags:
                _allocator_instance = new allocator_boundary_tags(1000, nullptr, nullptr, fit_mode);
                break;
            case  allocator_type::red_black_tree:
                _allocator_instance = new allocator_red_black_tree(1000, nullptr, nullptr, fit_mode);
                break;
            case  allocator_type::global_heap:
                _allocator_instance = new allocator_global_heap();
                break;

        }

        sc.add_collection(collection_name, _allocator_instance, type, fit_mode);
    }

    void remove_collection(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name) {
        if (_log) _log->trace("removing collection: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        sc.remove_collection(collection_name);
    }

    const Value* obtain_value(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, Key key) {
        if (_log) _log->trace("obtaining value by key = " + std::to_string(key._id_person) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));
        return cl.get_value(key);
    }

    void add_value(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, unsigned int key, unsigned int id, std::string const& nick_name, double salary) {
        if (_log) _log->trace("adding value by key = " + std::to_string(key) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));
        cl.add_value(key, id, nick_name,  salary);
    }

    void update_value(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, unsigned int key, unsigned int id, std::string const& nick_name, double salary) {
        if (_log) _log->trace("updating value by key = " + std::to_string(key) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));
        cl.update_value(key, id, nick_name,  salary);
    }

    void remove_value(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, Key key) {
        if (_log) _log->trace("removing value by key = " + std::to_string(key._id_person) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));
        cl.remove_value(key);
    }

    void update_value(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, unsigned int key, long position, long str_size) {
        if (_log) _log->trace("updating value by key = " + std::to_string(key) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));
        cl.update_value(key, position, str_size);
    }

    void add_value(std::string const& pool_name, std::string const& scheme_name, std::string const& collection_name, unsigned int key, long position, long str_size) {
        if (_log) _log->trace("adding value by key = " + std::to_string(key) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));
        cl.add_value(key, position, str_size);
    }

    std::vector<Value*> obtain_between_value(std::string const &pool_name, std::string const& scheme_name, std::string const& collection_name,
                                          const Key& minbound, const Key& maxbound, bool is_inclusive_lower, bool is_inclusive_upper)
    {
        if (_log) _log->trace("obtaining between {" + std::to_string(minbound._id_person) + ":" + std::to_string(maxbound._id_person) + "from: " + pool_name + "/" + scheme_name + "/" + collection_name);
        pool& pl = const_cast<pool&>(obtain_pool(pool_name));
        scheme& sc = const_cast<scheme&>(pl.obtain_scheme(scheme_name));
        collection& cl = const_cast<collection&>(sc.obtain_collection(collection_name));

        return cl.obtain_between_value(minbound, maxbound, is_inclusive_lower, is_inclusive_upper);

    }

};

data_base* data_base::_instance = nullptr;

#endif //COURSE_WORK_DATA_BASE_DATABASE_H
