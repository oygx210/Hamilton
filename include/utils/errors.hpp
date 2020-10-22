#pragma once

#include <exception>

/** 
 * @file Custom exceptions
 */

/** 
 * Cannot perform associative operations on array as they 
 * have different sizes
 */
struct HArraySizeMismatch : public std::exception
{
    HArraySizeMismatch(const char* Msg)
    {
        mMsg = Msg;
    }

    const char* what() const noexcept
    {
        return mMsg;
    }

private:
    const char* mMsg;
};
