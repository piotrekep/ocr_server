#include "setWorkDir.hpp"


namespace fs = std::filesystem;

int setWorkdir(){
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len == -1) {
        std::cerr << "Failed to read /proc/self/exe" << std::endl;
        return 1;
    }
    buffer[len] = '\0'; 

    fs::path exePath(buffer);
    fs::path exeDir = exePath.parent_path();
    fs::current_path(exeDir);
    
    return 0;

}