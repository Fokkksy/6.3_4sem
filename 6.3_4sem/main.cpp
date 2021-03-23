#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std::chrono;

template <typename TP>
std::time_t to_time_t(TP tp)
{
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}


auto last_write(std::filesystem::path p)
{
    auto ftime = std::filesystem::last_write_time(p);
    std::time_t cftime = to_time_t(ftime);
    return std::asctime(std::localtime(&cftime));
}


void info(const std::filesystem::path path)
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "This path does not exist. Try again..." << std::endl;
    }

    std::filesystem::directory_iterator begin(path);
    std::filesystem::directory_iterator end;

    std::vector<std::filesystem::path> subdirs;
    std::copy_if(begin, end, std::back_inserter(subdirs), [](const std::filesystem::path& path) {
        return std::filesystem::is_directory(path) | std::filesystem::is_regular_file(path);
        });

    std::cout << std::left << std::setw(40) << "filename" <<
        std::setw(10) << "Type" << std::setw(10) << "file size" <<
        std::setw(10) << "last write in file" << std::endl;
    for (auto c : subdirs)
    {
        if (std::filesystem::is_directory(c))
        {
            std::cout << std::left << std::setw(40) << c.filename() <<
                std::setw(10) << "d" << std::setw(10) << "-" <<
                std::setw(10) << last_write(c) << std::endl;
        }
        else
        {
            std::cout << std::left << std::setw(40) << c.filename() <<
                std::setw(10) << "f" << std::setw(10) <<
                std::filesystem::file_size(c) <<
                std::setw(10) << last_write(c) << std::endl;
        }
    }
}

int main(int argc, const char** argv)
{
    std::filesystem::path path{ argv[1] };
    info(path);


    return 0;
}
