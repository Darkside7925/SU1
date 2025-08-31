#pragma once

#include "su1/core.hpp"
#include <fmt/format.h>

namespace su1 {

// Logging macros for convenience with format string support
#define SU1_LOG_DEBUG(message, ...) Logger::debug(fmt::format(message, ##__VA_ARGS__))
#define SU1_LOG_INFO(message, ...) Logger::info(fmt::format(message, ##__VA_ARGS__))
#define SU1_LOG_WARNING(message, ...) Logger::warning(fmt::format(message, ##__VA_ARGS__))
#define SU1_LOG_ERROR(message, ...) Logger::error(fmt::format(message, ##__VA_ARGS__))
#define SU1_LOG_FATAL(message, ...) Logger::fatal(fmt::format(message, ##__VA_ARGS__))

} // namespace su1
