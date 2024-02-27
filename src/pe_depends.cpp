#include <pe_tools/pe_file.hpp>

#include <iostream>
#include <string>

using pe_tools::pe_file;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        try {
            std::string const filename = argv[1];
            pe_file f(filename);

            for(auto const & dll: f.get_import_dlls()) {
                std::cout << dll << std::endl;
            }
        }
        catch (std::exception const & ex) {
            std::cerr << "error: " << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        std::cout << "usage: pe-deps <filename>" << std::endl;
    }

    return EXIT_SUCCESS;
}