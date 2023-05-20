#include <util/util.h>

namespace util {

std::optional<std::pair<int16_t, int16_t>> ParseStringToSeq(const std::string& str, 
                                                            std::string parse_str) {
    std::string seq_name;
    short first_digit{0};
    short interval{0};

    std::string parce_str = parse_str;   
    char delim = ' '; 
    size_t pos = 0;
    uint8_t parce_str_size = parce_str.size();
    uint8_t delim_size{1};
    std::string temp;
    while (pos < parce_str_size) {
        temp = temp.assign(parce_str, pos, parce_str.find(delim, pos) - pos);
        pos += temp.size() + delim_size;
        if (temp.size() <= 0) { continue; }
        if(seq_name == "") { seq_name = temp; continue; }
        try {
            if(first_digit == 0) { first_digit = std::stoi(temp); continue; }
            if(interval == 0) { interval = std::stoi(temp); continue; }
        } catch(std::exception& ex) {
            return std::nullopt;
        }
    }

    if (seq_name != str) { return std::nullopt; }
    if (first_digit < -9999 || first_digit > 9999 || first_digit == 0) { 
        return std::nullopt; }
    if (interval < -9999 || interval > 9999 || interval == 0) { 
        return std::nullopt; }

    return std::make_pair<int16_t, int16_t>(std::move(first_digit), 
                                            std::move(interval));
}
}  // namespace util
