#include "context_logger.hpp"

namespace games {
std::unordered_map<std::string, ContextLogger> ContextLoggerFactory::contexts;
ContextLoggerFactory ContextLoggerFactory::instance;
}  // namespace games
