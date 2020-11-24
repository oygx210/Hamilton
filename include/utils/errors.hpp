#pragma once

#include <exception>
#include <string>

/** 
 * @file errors.hpp Custom exceptions
 */

namespace Error
{
    /** 
     * Base class for a generic Hamilton exception    
     */
    class GenericException : public std::exception
    {
    public:    

        /** 
         * Message with a file and line
         */
        GenericException(const char* File, size_t Line, const char* Msg)  :
            mMsg{std::string(File) + std::string("(") + std::to_string(Line) + 
                std::string("): ") + std::string(Msg)}
        {

        }

        /** 
         * Completely user defined message
         */
        GenericException(const char* Msg) : mMsg{std::string(Msg)} { }
        GenericException(const std::string& Msg) : mMsg{Msg} { }

        /** 
         * @return Exception message as string
         */
        const std::string& What(void) const noexcept {return mMsg;}

        /** 
         * @return Exception message as pointer to a const char
         */
        const char* what() const noexcept
        {
            return mMsg.c_str();
        }        

    protected:
        const std::string mMsg;
    };

    /** 
     * Cannot perform associative operations on array as they 
     * have different sizes
     */
    class HArraySizeMismatch : public GenericException
    {
    public:
        /// Default message implementation    
        HArraySizeMismatch(const char* File, size_t Line) :
            GenericException(File, Line, "Cannot perform associative operations on arrays of different sizes")
        {
            
        }

        /// Custom message implementation
        HArraySizeMismatch(const char* Msg) : GenericException(Msg) { }
    };

    /** 
     * Attempt to access out of bounds array element
     */
    class OutOfBoundsException : public GenericException
    {
    public:
        /// Default Message Implementation
        OutOfBoundsException(const char* File, size_t Line, size_t Index, size_t ArraySize) :
            GenericException{std::string(File) + std::string("(") + std::to_string(Line) + 
                std::string("): Attempt to access out of bounds element ") + 
                std::to_string(Index) + std::string(" in array of size ") + std::to_string(ArraySize)}
        {

        }

        /// Custom Message Implementation
        OutOfBoundsException(const char* Msg) : GenericException(Msg) { }
    };
}