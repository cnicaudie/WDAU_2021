#pragma once

#define NO_LOG          0
#define ERROR_LEVEL     1
#define INFO_LEVEL      2
#define DEBUG_LEVEL     3

#ifndef LOG_LEVEL
#define LOG_LEVEL DEBUG_LEVEL
#endif // !LOG_LEVEL

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_DEBUG(message)     std::cout << "[DEBUG] " << message << std::endl
#else
#define LOG_DEBUG(message) 
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define LOG_INFO(message)      std::cout << "[INFO] " << message << std::endl
#else
#define LOG_INFO(message) 
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define LOG_ERROR(message)     std::cout << "[ERROR] " << message << std::endl
#else
#define LOG_ERROR(message) 
#endif