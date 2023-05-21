#include <optional>
#include <utility>
#include <string>
#include <cstring>

namespace util {

// Gets a template string, parse it and cover all the conditions
std::optional<std::pair<int16_t, int16_t>> ParseStringToSeq(const std::string& str, 
                                                            std::string parse_str);
}  // namespace util