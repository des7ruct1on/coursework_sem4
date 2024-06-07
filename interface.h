#ifndef interface_h
#define interface_h

#include <iostream>
#include "database.h"
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include "logger/logger_builder.h"
#include "logger/client_logger.h"
#include "logger/client_logger_builder.h"
#include "logger/logger.h"
#include <sys/stat.h>
#include <unistd.h>
#include "allocators/allocator_with_fit_mode.h"
#include "allocators/allocator_types.h"
#include "allocators/allocator_guardant.h"
#include "commands/command.h"
#include "commands/command_add_pool.h"
#include "request_handler/request_handler_with_command.h"
#include "request_handler/request_handler_with_command_chain.h"
#include "commands/command_add_scheme.h"
#include "commands/comman_add_collection.h"
#include "commands/command_add_value.h"
#include "commands/command_delete_value.h"
#include "commands/command_find_value.h"
#include "commands/command_update_value.h"
#include "commands/command_delete_collection.h"
#include "commands/command_delete_scheme.h"
#include "commands/command_delete_pool.h"
#include "commands/command_read_set_value.h"
#include "version_singleton.h"
#include <string>
#include "commands/command_find_value_in_time.h"

class Interactive {
    
    logger* _log;
    
    data_base* _db;
    
public:
    
    Interactive(data_base* db, logger* log = nullptr) {
        _log = log;
        _db =  db;
    }
    
    ~Interactive() = default;
    
    Interactive(const Interactive& other) = delete;
    
    Interactive(Interactive&& other) noexcept = delete;
    
    Interactive& operator=(const Interactive& other) = delete;
    
    Interactive& operator=(Interactive&& other) noexcept = delete;
    
private:
    
    void print_menu_mode() const {
        std::cout << std::endl;
        std::cout << "Choose mode type:\n";
        std::cout << "\tin_file_system / in_memory_cache\n";
        std::cout << std::endl;
    }
    
    int get_mode() {
        print_menu_mode();
        std::string choose;
        std::cin >> choose;
        if (choose == "in_file_system") {
            _db->set_mode(true);
        } else if (choose == "in_memory_cache") {
            _db->set_mode(false);
        } else {
            return -1;
        }
        return 1;
    }
    
    void print_menu_command_mode() const {
        std::cout << std::endl;
        std::cout << "Choose mode type:\n";
        std::cout << "\tfrom_file / input\n";
        std::cout << std::endl;
    }
    
    int get_command_mode() {
        print_menu_command_mode();
        std::string choose;
        std::cin >> choose;
        if (choose == "from_file") {
            return 1;
        } else if (choose == "input") {
            return 2;
        }
        return -1;
    }
    
    void print_actions() const {
        std::cout << std::endl;
        std::cout << "INPUT MENU:\n";
        std::cout << "Type cmd\n";
        std::cout << "\t|ADD_POOL pool_name\n";
        std::cout << "\t|ADD_SCHEME pool_name scheme_name\n";
        std::cout << "\t|ADD_COLLECTION pool_name scheme_name collection_name allocator_type  fit_mode\n";
        std::cout << "\t|ADD_VALUE pool_name scheme_name collection_name key person_id nickname balance\n";
        std::cout << "\t|DELETE_POOL pool_name\n";
        std::cout << "\t|DELETE_SCHEME pool_name scheme_name\n";
        std::cout << "\t|DELETE_COLLECTION pool_name scheme_name collection_name\n";
        std::cout << "\t|DELETE_VALUE pool_name scheme_name collection_name key\n";
        std::cout << "\t|IN_TIME pool_name scheme_name collection_name key time_str\n";
        std::cout << "\t|FIND_VALUE pool_name scheme_name collection_name key\n";
        std::cout << "\t|READ_SET pool_name scheme_name collection_name id_min id_max is_inclusive_lower is_inclusive_upper;\n";
        std::cout << "\t|UPDATE_VALUE pool_name scheme_name collection_name id_key id_order first_name rating\n";
        std::cout << "\t|EXIT\n";
        std::cout << std::endl;
    }
    
    void print_filename() const {
        std::cout << "Enter filename: ";
    }
    
    std::string& get_filename_to() {
        std::string s;
        print_filename();
        std::cin >> s;
        return s;
    }
    
    void print_restore_choose() const {
        std::cout << std::endl;
        std::cout << "Do you wanna to restore data from file?";
        std::cout << "Yes/No\n";
        std::cout << std::endl;
    }
    
    int get_restore_choose() {
        std::string s;
        print_restore_choose();
        std::cin >> s;
        if (s == "Yes" || s == "yes" || s == "y" || s == "+") {
            return 1;
        } else if (s == "No" || s == "no" || s == "n" || s == "-") {
            return 0;
        } else {
            return -1;
        }
    }
    
public:
    void interact(request_handler_with_command_chain& ch, const std::string& restore_file) {
        if (_log) _log->trace("Started interacting");
        int _get_mode = -1;
        while (_get_mode < 0) {
            if (_log) _log->trace("getting mode");
            _get_mode = get_mode();
        }
        
        int choose_mode_input = -1;
        while (choose_mode_input < 0) {
            if (_log) _log->trace("getting mode input");
            choose_mode_input = get_command_mode();
        }
        
        int choose_restore = -1;
        while (choose_restore < 0 && _get_mode != 0) {
            if (_log) _log->trace("getting mode restore");
            choose_restore = get_restore_choose();
        }
        
        if (!_db->get_mode() && choose_restore) {
            if (_log) _log->trace("Started restoring");
            std::ifstream input_file;
            input_file.open(restore_file);
            if(!input_file.is_open()) {
                if (_log) _log->error("failed to open file");
                throw std::logic_error("Failed to open file");
            }
            std::string str;
            while(std::getline(input_file, str)) {
                ch.handle(str);
            }
            input_file.close();
            if (_log) _log->trace("Finished restoring");
        }
        while (true) {
            if (choose_mode_input == 1) { // file
                if (_log) _log->trace("Started in_file_system mode");
                std::ifstream input_file;
                std::string file_open = get_filename_to();
                input_file.open(file_open);
                if(!input_file.is_open()) {
                    if (_log) _log->error("failed to open file");
                    throw std::logic_error("Failed to open file");
                }
                std::string str;
                while(std::getline(input_file, str)) {
                    //std::cout << std::endl << str << std::endl << std::endl;
                    ch.handle(str);
                }
                input_file.close();
                if (_log) _log->trace("Finished in_file_system mode");
                break;
            } else { //input
                if (_log) _log->trace("Started input mode");
                std::string str;
                char c;
                while(true) {
                    print_actions();
                    while (std::cin.get(c)) {
                        if (c != '\n' && c != '\0') {
                            str.push_back(c);
                        } else {
                            break;
                        }
                    }
                    
                    if (str == "EXIT" || str == "exit") {
                        break;
                    }
                    std::cout << std::endl << str << std::endl << std::endl;
                    ch.handle(str);
                    str.clear();
                }
                if (_log) _log->trace("Finished input mode");
                if (str == "EXIT" || str == "exit") {
                    break;
                }
            }
        }
        if (_log) _log->trace("Finished interacting");
        
    }
};

#endif
