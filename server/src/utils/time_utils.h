#ifndef TIME_UTILS_HEAD
#define TIME_UTILS_HEAD

#include <chrono>
#include <format>


inline std::string toIso8601(std::filesystem::file_time_type ftime) {
    auto sctp = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
    return std::format("{:%Y-%m-%dT%H:%M:%SZ}", std::chrono::floor<std::chrono::seconds>(sctp));
}


#endif // TIME_UTILS_HEAD