/*******************************************************************************
 Copyright (C) 2017 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#ifndef UTIL_STRINGUTIL_H_
#define UTIL_STRINGUTIL_H_

#include <sstream>
#include <string>
#include <vector>
#include <functional>

namespace huesdk_lib {

    /**
     Convert value to string
     @param  value The value to be converted to a string
     @return The string value
     */
    template <typename Type>
    std::string to_string(const Type& value) {
        std::ostringstream string_stream;
        string_stream << value;
        
        return string_stream.str();
    }
    
    /**
     Make string upper case (only characters a-z are supported)
     @param value The value to make upper case
     @return The string in upper case. If a certain character could not
             be converted, it will keep the old value
     */
    std::string to_upper_case(std::string value);
    
    /**
     Generate a random alpha-numeric string
     @param size The size of the random string
     @return The generated string
     */
    std::string random_alphanumeric_string(size_t size);
    
    /**
     Spit string by the symbol
     @param s The input string
     @param c The spitted symbol
     @result the vector with tokens
     */
    std::vector<std::string> split(const std::string& s, char c);
    
    /**
     Spit string by array of separators
     @param s The input string
     @param separators The Vector of separators
     @result the vector with tokens
     */
    std::vector<std::string> split(const std::string& s, const std::vector<std::string>& separators);
    
    std::string::size_type find_separator(const std::string& s, std::string::size_type start_position, const std::vector<std::string>& separators, std::string::size_type* separator_length);
    
    std::string replace_all(std::string subject, const std::string& search, const std::string& replace);
    
    std::string trim(std::string str, const std::string whitespace = " \t\r\n");

    std::string join(const std::vector<std::string>& elements, const std::string& delimiter);

    std::vector<std::string> map_vector(const std::vector<std::string>& elements, std::function<std::string(const std::string&)> mapping);

}  // namespace huesdk_lib

#endif  // UTIL_STRINGUTIL_H_