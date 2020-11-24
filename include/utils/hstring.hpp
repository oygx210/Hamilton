#pragma once

#include <string>
#include "harray.hpp"
#include "errors.hpp"

/** 
 * @file hstring.hpp 
 */

/** 
 * Hamilton string type. Compared to std::string cannot be modified in place
 * 
 */
class HString
{
public:    

    /** 
     * Default empty string
     */
    HString() noexcept { } 

    /** 
     * Construct string from C-string
     * @param Str c string to constrcut from
     */
    HString(const char* Str) : mString{Str}, mSize{mString.size()} { }

    
    /** 
     * Construct directly from C++ string
     * @param Str C++ string to construct from
     */
    HString(const std::string& Str) : mString{Str}, mSize{mString.size()} { }

    /** 
     * String assignment
     * @param Str String value to assign to
     */
    HString(const HString& Str) : mString{Str.mString}, mSize{mString.size()} { }

    /** 
     * Create string from any time which can be converted to a c++ string using to_string  
     */
    HString(const auto Val) : mString{std::to_string(Val)}, mSize{mString.size()} { }

    HString(HString&& Str) = default;
    HString& operator=(const HString&) = default;
    HString& operator=(HString&&) = default;    

    /** 
     * @return An iterator to the beginning of the string.
     */
    const auto Begin(void) const {return mString.begin();}
    auto Begin(void) {return mString.begin();}

    const auto begin(void) const {return Begin();}
    auto begin(void) {return Begin();}

    /** 
     * @return An iterator to the end of the string.
     */
    const auto End(void) const noexcept {return mString.end();}
    auto End(void) {return mString.end();}

    const auto end(void) const {return End();}
    auto end(void) {return End();}

    /** 
     * @return Number of bytes in the string
     */
    size_t Size(void) const noexcept {return mSize;}

    /** 
     * Retrieve a character at the given position. If `Pos` is greater than or equal to 
     * the string length, will return a null character.
     * @param Position of character to return
     * @return Character at the specified `Pos`
     */
    const char& At(size_t Pos) const noexcept
    {
        if (Pos >= Size())
        {
            return NULL_CHAR;    
        }

        return mString[Pos];
    }

    /** 
     * @return A reference to the last character in the string.
     */
    const char& Back(void) const noexcept
    {
        size_t StrSize = Size();    
        if (StrSize == 0)
        {
            return NULL_CHAR;    
        }

        return mString[StrSize - 1];
    }

    /** 
     * @return A reference to the first character in the string.
     */
    const char& Front(void) const noexcept
    {
        if (Size() == 0)
        {
            return NULL_CHAR;    
        }

        return mString[0];
    }

    /** 
     * Returns a newly constructed string from the concatenation of this string and `Str`
     * @param Str Secondary string to concatenate
     * @return New string consisting of the concatenation of both
     */
    HString operator+(const HString& Str) const {return HString{mString + Str.mString};}

    /** 
     * Returns a newly constructed string from the concatenation of this string and `Str`
     * @param Str Secondary string to concatenate
     * @return New string consisting of the concatenation of both
     */    
    HString operator+(const char* Str) const {return HString{mString + Str};}

    /**
     * @return `true` if strings are equal
     */
    bool operator==(const HString& Str) const noexcept {return mString == Str.mString;}

    /** 
     * @return `true` if strings are not equal
     */
    bool operator!=(const HString& Str) const noexcept {return mString != Str.mString;}  

    bool operator<(const HString& Str) const noexcept {return mString < Str.mString;}
    bool operator<=(const HString& Str) const noexcept {return mString <= Str.mString;}
    bool operator>(const HString& Str) const noexcept {return mString > Str.mString;}
    bool operator>=(const HString& Str) const noexcept {return mString >= Str.mString;}

    /** 
     * Checks if this string contains the given substring
     * @param Substring Sub string to search for
     * @param StartPos Starting position to begin search for substring within this string
     * @return Index of the start of the found string, or `Size()` if the string was not found
     */
    size_t Find(const HString& SubString, size_t StartPos = 0) const noexcept {return mString.find(SubString.mString, StartPos);}

    /** 
     * Checks if this string contains the given substring
     * @param Substring Sub string to search for
     * @param StartPos Starting position to begin search for substring within this string
     * @return Index of the start of the found string, or `Size()` if the string was not found
     */
    size_t Find(const char* SubString, size_t StartPos = 0) const noexcept {return mString.find(SubString, StartPos);}

    /** 
     * @return String as a c string
     */
    const char* Data(void) const noexcept {return mString.data();}

    /** 
     * @return String as a c++ string
     */
    const std::string& CppString(void) const noexcept {return mString;}

    /** 
     * Reset to an empty string
     */
    void Clear() noexcept
    {
        mString.clear();
        mSize = 0;
    }    

    /** 
     * Returns a substring of the current string
     * @param StartPos Start position in the current string to begin the substring. If this is greater than or 
     * equal to the string length, returns an empty string
     * @param Length Number of characters in the string to copy to the substring, sequentially from `StartPos`. If 
     * this exceeds the length of the original string, as many characters as possible are used
     * @return A substring
     */
    HString Substring(size_t StartPos, size_t Length) const
    {
        const size_t StrSize = Size();

        if (StartPos >= StrSize)
        {
            return HString{};
        }

        // Copy entire remainder of string
        size_t TrueLength = (Length > StrSize - StartPos) ? StrSize - StartPos : Length;
        std::string Temp{};
        Temp.resize(TrueLength);

        for (size_t Index = 0; Index < TrueLength; Index++)
        {
            Temp[Index] = mString[StartPos + Index];
        }

        return HString{std::move(Temp)};
    }

    /** 
     * @return `true` if the string begins with the given substring
     */
    bool StartsWith(const HString& Str) const noexcept {return mString.starts_with(Str.mString);}

    /** 
     * @return `true` if the string ends with the given substring
     */
    bool EndsWith(const HString& Str) const noexcept {return mString.ends_with(Str.mString);}

    /** 
     * @return `true` if the string containts the given substring
     */
    bool Contains(const HString& Str) const noexcept {return (Find(Str) < Size());}

    /** 
     * Splits the string into an array of substrings, separeted by `Token`
     * @param Token substring to split by
     * @return Array of substrings
     */
    HArray<HString> Split(const HString& Separator) const
    {
        size_t LastPos = 0;    
        size_t Pos = Find(Separator);

        HArray<HString> Result{ };        

        while (Pos < mSize)
        {
            Result.AppendBack(Substring(LastPos, Pos - LastPos));

            LastPos = Pos + Separator.Size();
            Pos = Find(Separator, LastPos);
        }

        Result.AppendBack(Substring(LastPos, Pos - LastPos));

        return Result;        
    }

    /** 
     * Return a new string with all specified tokens removed from the old
     * @param Tokens Array of substrings to remove all instances of
     */
    // HString Strip(const HArray<HString>& Tokens) const
    // {
        // Sort Tokens by size
        // Copy String data into backwards array
        // Loop through each token
            // Loop through copy of backwards string data removing every instance of Token
        // Return new array from remaining data (reversed again)
    // }

    /** 
     * Return a new string where the first instance of `SearchStr` is replaced by `ReplaceStr`
     * @param SearchStr Original substring to replace
     * @param ReplaceStr Replacement string
     * @param StartPos Starting position in the string to start searching
     * @return A new string with all instances of the original SearchStr replaced 
     */
    HString Replace(const HString& SearchStr, const HString& ReplaceStr, size_t StartPos = 0) const
    {
        size_t Pos = Find(SearchStr, StartPos);

        if (Pos >= Size())
        {
            return *this;    
        }

        return HString{mString.substr(0, Pos) + ReplaceStr.mString + mString.substr(Pos + SearchStr.Size(), Size() - (Pos + SearchStr.Size()))};
    }

    /** 
     * Return a new string where every instance of `SearchStr` is replaced by `ReplaceStr`
     * @param SearchStr Original substring to replace
     * @param ReplaceStr Replacement string
     * @return A new string with all instances of the original SearchStr replaced
     */
    HString ReplaceAll(const HString& SearchStr, const HString& ReplaceStr) const
    {
        size_t LastPos = 0;    
        size_t Pos = Find(SearchStr);

        if (Pos >= Size())
        {
            return *this;    
        }

        std::string Result{};

        while (Pos < mSize)
        {
            Result.append(mString.substr(LastPos, Pos - LastPos));
            Result.append(ReplaceStr.mString);

            LastPos = Pos + SearchStr.Size(); 
            Pos = Find(SearchStr, LastPos);
        }

        Result.append(mString.substr(LastPos, Size()));

        return HString(std::move(Result));
    }

    /** 
     * Joins an array of substrings into a super string 
     * @param Separator Separator string between each substring
     * @param Substrings Sub-strings to join
     * @return Super string
     */
    static HString Join(HString Separator, const HArray<HString>& Substrings)
    {
        // Not joining any strings    
        if (Substrings.Size() == 0)
        {
            return HString{};
        }
        else if (Substrings.Size() == 1)
        {
            return Substrings.Front();    
        }

        // Count up the total character size for a single allocation
        size_t CharacterLength = (Substrings.Size() - 1) * Separator.Size();
        for (const HString& Str : Substrings)
        {
            CharacterLength += Str.Size();
        }

        // Allocate
        std::string Data{};
        Data.reserve(CharacterLength);

        // Populate        
        Data += Substrings.Front().CppString();
        for (size_t Index = 1; Index < Substrings.Size(); Index++)
        {
            Data += Separator.CppString();
            Data += Substrings[Index].CppString();
        }

        return HString{std::move(Data)};
    }

    static size_t Npos(void) noexcept {return std::string::npos;}

private:
    /// Underlying C++ string type
    std::string mString{};
    size_t mSize = 0;

    // Null character
    constexpr static char NULL_CHAR = '\0';
};

/** 
 * String concatenation (left)
 */
inline HString operator+(const char* CStr, const HString& Str) 
{
    return Str + CStr;
}