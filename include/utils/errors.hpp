#pragma once

#include <exception>
#include <string>

/** 
 * @file Custom exceptions
 */

namespace Error
{
    /** 
     * Cannot perform associative operations on array as they 
     * have different sizes
     */
    struct HArraySizeMismatch : public std::exception
    {
        /// Default message implementation    
        HArraySizeMismatch(const char* File, size_t Line) :
            mMsg{std::string(File) + std::string("(") + std::to_string(Line) + 
                std::string("): Cannot perform associative operations on arrays of different sizes")}
        {
            
        }

        /// Custom message implementation
        HArraySizeMismatch(const char* Msg) :
            mMsg{Msg}
        {
            
        }

        const char* what() const noexcept
        {
            return mMsg.c_str();
        }

    private:
        const std::string mMsg;
    };
}