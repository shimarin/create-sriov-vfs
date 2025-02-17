#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <print>
#include <filesystem>

#include <argparse/argparse.hpp>

void create(const std::string& class_, const std::string& device, int count)
{
    auto device_path = std::filesystem::path("/sys/class") / class_ / device;
    if (!std::filesystem::exists(device_path)) {
        throw std::runtime_error(std::format("Device {} not found with class {}", device, class_));
    }
    //else
    auto sriov_numvfs_path = device_path / "device/sriov_numvfs";
    if (!std::filesystem::exists(sriov_numvfs_path)) {
        throw std::runtime_error(std::format("Device {} does not support SR-IOV", device));
    }
    //else
    int fd = open(sriov_numvfs_path.c_str(), O_WRONLY);
    if (fd < 0) {
        throw std::runtime_error(std::format("Failed to open {}", sriov_numvfs_path.string()));
    }
    //else
    int rst = write(fd, std::to_string(count).c_str(), std::to_string(count).size());
    close(fd);
    if (rst < 0) {
        throw std::runtime_error(std::format("Failed to write to {}: {}", 
            sriov_numvfs_path.string(),
            strerror(errno)));
    }
}

int main(int argc, char** argv) {
    argparse::ArgumentParser program("create-sriov-vfs");
    program.add_argument("class")
        .help("PCI device class");
    program.add_argument("device")
        .help("Device name");
    program.add_argument("count")
        .help("Number of VFs to create")
        .scan<'i', int>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::println("{}", err.what());
        std::cout << program;
        return 1;
    }

    auto class_ = program.get<std::string>("class");
    auto device = program.get<std::string>("device");
    auto count = program.get<int>("count");
    try {
        std::println("Creating {} VFs for device {} with class {}", count, device, class_);
        create(class_, device, count);
    }
    catch (const std::runtime_error& e) {
        std::println(std::cerr, "Error: {}", e.what());
        return 1;
    }

    return 0;
}