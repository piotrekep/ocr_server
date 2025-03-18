#include "setWorkDir.hpp"


namespace fs = std::filesystem;

int setWorkdir(){
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len == -1) {
        std::cerr << "Failed to read /proc/self/exe" << std::endl;
        return 1;
    }
    buffer[len] = '\0';  // Null-terminate the path

    fs::path exePath(buffer);
    fs::path exeDir = exePath.parent_path();

    // Set the working directory to the directory containing the executable
    fs::current_path(exeDir);
    std::cout << "Working directory set to: " << fs::current_path() << std::endl;
    
    return 0;

}