

#ifndef COURSE_WORK_DATA_BASE_COMMAN_ADD_COLLECTION_H
#define COURSE_WORK_DATA_BASE_COMMAN_ADD_COLLECTION_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../allocators/allocator_types.h"
#include "../allocators/allocator_with_fit_mode.h"
#include "../version_singleton.h"

class command_add_collection final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    enum allocator_type _type_of_allocator;
    allocator_with_fit_mode::fit_mode _type_of_fit_mode;
    std::string _lctr_tp;
    std::string _ft_md_tp;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "ADD_COLLECTION")
        {
            std::string pool_name, scheme_name, collection_name, allocator_type, fit_mode;
            std::cout << request;
            iss >> pool_name >> scheme_name >> collection_name >> allocator_type >> fit_mode;
            if(allocator_type == "buddies_system")
            {
                _type_of_allocator = allocator_type::buddies_system;
            }
            else if(allocator_type == "sorted_list")
            {
                _type_of_allocator = allocator_type::sorted_list;
            }
            else if(allocator_type == "boundary_tags")
            {
                _type_of_allocator = allocator_type::boundary_tags;
            } else if(allocator_type == "red_black_tree")
            {
                _type_of_allocator = allocator_type::red_black_tree;
            } else if(allocator_type == "global")
            {
                _type_of_allocator = allocator_type::global_heap;
            }
            else 
            {
                throw std::logic_error("type of allocator error");
            }
            if(fit_mode == "best_fit")
            {
                _type_of_fit_mode = allocator_with_fit_mode::fit_mode::the_best_fit;
            }
            else if(fit_mode == "worst_fit")
            {
                _type_of_fit_mode = allocator_with_fit_mode::fit_mode::the_worst_fit;
            }
            else if(fit_mode == "first_fit")
            {
                _type_of_fit_mode = allocator_with_fit_mode::fit_mode::first_fit;
            }
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _lctr_tp = allocator_type;
            _ft_md_tp = fit_mode;
            return true;
        }
        else return false;


    }

    void execute(std::string const& request) final
    {
        version_singleton::get_instance(" ")->write_to_file("ADD_COLLECTION " + _pool_name + " " + _scheme_name + " " + _collection_name +  " " + _lctr_tp  + " " + _ft_md_tp);
        data_base::get_instance()->add_collection(_pool_name, _scheme_name, _collection_name, _type_of_allocator, _type_of_fit_mode);
        if(data_base::get_instance()->get_mode())
        {
            std::string catalog_path  = _pool_name + "/" + _scheme_name + "/" + _collection_name;
            if(mkdir(catalog_path.c_str(), 0777) != 0)
            {
                throw std::logic_error("can`t create catalog");
            }
        }

    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAN_ADD_COLLECTION_H
