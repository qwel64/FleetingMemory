#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iomanip>
#include <chrono>
#include <thread>

struct ProcessInfo {
    pid_t pid;
    std::string name;
    long memoryUsage;
    bool valid;
};

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

void logProcessInfo(const ProcessInfo& info) {
    std::cout << "PID: " << info.pid
              << ", Name: " << info.name
              << ", Memory: " << info.memoryUsage << " KB"
              << std::endl;
}

std::vector<pid_t> getProcessList() {
    std::vector<pid_t> processes;
    DIR* dir = opendir("/proc");
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                char* endptr;
                long pid = strtol(entry->d_name, &endptr, 10);
                if (*endptr == '\0') {
                    processes.push_back(static_cast<pid_t>(pid));
                }
            }
        }
        closedir(dir);
    }
    return processes;
}

ProcessInfo getProcessInfo(pid_t pid) {
    ProcessInfo info;
    info.pid = pid;
    info.valid = false;

    std::string statm_path = "/proc/" + std::to_string(pid) + "/statm";
    std::ifstream statm_file(statm_path);
    if (statm_file.is_open()) {
        long size, resident, shared, text, lib, data, dt;
        statm_file >> size >> resident >> shared >> text >> lib >> data >> dt;
        info.memoryUsage = resident * 4;
        info.valid = true;
    }

    std::string comm_path = "/proc/" + std::to_string(pid) + "/comm";
    std::ifstream comm_file(comm_path);
    if (comm_file.is_open()) {
        std::getline(comm_file, info.name);
    }

    return info;
}

void analyzeUserSpace() {
    std::vector<pid_t> processes = getProcessList();

    for (const auto& pid : processes) {
        std::string statm_path = "/proc/" + std::to_string(pid) + "/statm";
        std::ifstream statm_file(statm_path);
        if (statm_file.is_open()) {
            long size, resident, shared, text, lib, data, dt;
            statm_file >> size >> resident >> shared >> text >> lib >> data >> dt;
            
            if (data > 1000000) {
                std::cout << "Potential memory leak detected in process " << pid << std::endl;
                std::cout << "  Data segment size: " << data * 4096 / 1024 / 1024 << " MB" << std::endl;
            }
        }
    }

    std::cout << "User space memory leak analysis completed." << std::endl;
}

void analyzeAndLogRealTime() {
    while (true) {
        auto start = std::chrono::steady_clock::now();
        
        std::vector<pid_t> processes = getProcessList();
        std::cout << "Timestamp: " << getCurrentTimestamp() << std::endl;
        std::cout << "Number of processes: " << processes.size() << std::endl;
        
        for (const auto& pid : processes) {
            ProcessInfo info = getProcessInfo(pid);
            if (info.valid) {
                logProcessInfo(info);
            }
        }
        
        std::cout << std::string(50, '-') << std::endl;
        
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        if (elapsed < std::chrono::seconds(1)) {
            std::this_thread::sleep_for(std::chrono::seconds(1) - elapsed);
        }
    }
}

int main() {
    analyzeUserSpace();
    analyzeAndLogRealTime();

    return 0;
}
