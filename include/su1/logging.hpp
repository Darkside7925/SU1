#pragma once

#include "su1/core.hpp"

namespace su1 {

// Logging macros for convenience
#define SU1_LOG_DEBUG(message) Logger::debug(message)
#define SU1_LOG_INFO(message) Logger::info(message)
#define SU1_LOG_WARNING(message) Logger::warning(message)
#define SU1_LOG_ERROR(message) Logger::error(message)
#define SU1_LOG_FATAL(message) Logger::fatal(message)

} // namespace su1
