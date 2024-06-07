#include "interface.h"

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();

    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }

    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }

    logger *built_logger = builder->build();

    delete builder;

    return built_logger;
}

void create_all_cmd(request_handler_with_command_chain& chain) {
    chain.add_handler(new command_add_pool())
        .add_handler(new command_add_scheme())
        .add_handler(new command_add_collection())
        .add_handler(new command_add_value())
        .add_handler(new command_delete_value())
        .add_handler(new command_find_value())
        .add_handler(new command_update_value())
        .add_handler(new command_delete_collection())
        .add_handler(new command_delete_scheme())
        .add_handler(new command_delete_pool())
        .add_handler(new command_read_set_value())
        .add_handler(new command_find_value_in_time());
}

int main(int argc, char* argv[]) {
    
    
    logger* logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                   {
        {
            "log.txt",
            logger::severity::trace
        },
        {
            "log.txt",
            logger::severity::debug
        },
        {
            "log.txt",
            logger::severity::information
        },
        {
            "log.txt",
            logger::severity::warning
        },
        {
            "log.txt",
            logger::severity::error
        },
        {
            "log.txt",
            logger::severity::critical
        },
    });
    
    data_base* data_base = data_base::get_instance(8, logger);
    version_singleton* instance =  version_singleton::get_instance("restore.txt");
    
    request_handler_with_command_chain chain;
    try {
        create_all_cmd(chain);
    } catch (const std::exception& e) {
        std::cerr << "Error creating commands: " << e.what() << std::endl;
        return -1;
    }
    Interactive* interact_input = nullptr;
    try {
        interact_input = new Interactive(data_base, logger);
    } catch (const std::bad_alloc& exc) {
        std::cerr << exc.what() << std::endl;
        return -2;
    }
    try {
        interact_input->interact(chain, "restore.txt");
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return -3;
    } catch (const std::logic_error& le) {
        std::cerr << le.what() << std::endl;
        return -4;
    } 
    delete data_base;
    delete interact_input;
    delete logger;
    return 0;
}

