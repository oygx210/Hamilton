#pragma once

#include <vector>
#include "errors.hpp"

/** 
 * @file harray.hpp 
 */

/** 
 * Hamilton array type. Wrapper around std::array with support for 
 * element wise addition/substraction with another array. Multiplication/Division 
 * with a scalar, and the equivilant in place operations. Requires the underlying
 * type T to support those operations
 */
template <typename T>
class HArray
{
public:    

    /** 
     * Default array of type T of zero length
     */
    Array() { }

    /** 
     * Default initialise array with the given number of elements
     * @param NumberElements Number of elements in array
     */
    Array(size_t NumberElements) : mVector{NumberElements} { }

    /** 
     * Default initialise array with the number of given elements 
     * set to an initial value
     * @param NumberElements Number of elements in array
     * @param Init Initial value of array elements
     */
    Array(size_t NumberElements, T Init) : mVector{NumberElements, Init} { }

    // Array(const Array<T>& Arr) {mVector = Arr.mVector;}

    /** 
     * @return the size of the array
     */
    size_t Size(void) const noexcept {return mVector.size();}

    /** 
     * Emplace a new element on the rear of the array
     * @param Elem New element to emplace
     */
    void EmplaceBack(const T& Elem) {mVector.emplace_back(Elem);}

    /** 
     * Reserves space for the given number of elements in the array
     * @param NumberElements Number of elements to reserve
     */
    void Reserve(size_t NumberElements) {mVector.reserve(NumberElements);}

    /**
     * @return Reference to the first element of the array
     */
    const T& Front(void) const {return mVector.front();}
    T& Front(void) {return mVector.front();}

    /** 
     * @return Reference to the back element of the array
     */
    const T& Back(void) const {return mVector.back();}
    T& Back(void) {return mVector.back();}

    /** 
     * @return Direct reference to the underlying data    
     */
    const T* Data(void) const noexcept {return mVector.data();}
    T* Data(void) noexcept {return mVector.data();}

    /** 
     * @return True if the array is empty
     */
    bool IsEmpty(void) const noexcept {return mVector.empty();}

    /** 
     * Clears the contents of the array and resizes to zero length
     */
    void Clear(void) noexcept {mVector.clear();}

    /** 
     * Resizes array to the given length
     * @param NewSize New array length
     */
    void Resize(void) noexcept {mVector.resize();}

    /** 
     * @return Iterator to the first element
     */
    auto Begin() noexcept {return mVector.begin();}

    /** 
     * @return Iterator to the last element
     */
    auto End() noexcept {return mVector.end();}

    /** 
     * Inserts an element into the array at the specified location
     * @param Iterator Iterator to the location to insert element at
     * @param Elem Element to insert
     * @return Iterator pointing to the inserted value
     */
    auto Insert(auto Iterator, const T& Elem) {return mVector.insert(Elem);}

    /** 
     * @param Index location of the element to retrieve
     * @return Element at the given `Index`
     */
    const T& operator[](size_t Index) const {return mVector[Index];}
    T& operator[](size_t Index) {return mVector[Index];}

    /** 
     * @param Arr array to compare too
     * @return true if arrays are identical
     */
    bool operator==(const HArray<T>& Arr) {return mVector == Arr.mVector;}

    /** 
     * Element wise addition of arrays
     */
    HArray<T> operator+(const HArray<T>& Arr) const
    {
        if (Size() != Arr.Size())
        {
            throw HArraySizeMismatch("Cannot perform associative operations on arrays of different sizes");
        }

        HArray<T> Result{};
        Result.Reserve(Size());

        for (size_t Index = 0; Index < Size(); Index++)
        {
            Result.EmplaceBack(mVector[Index] + Arr[Index]);
        }

        return Result;        
    }

    /** 
     * Element wise subtraction of arrays
     */
    HArray<T> operator-(const HArray<T>& Arr) const
    {
        if (Size() != Arr.Size())
        {
            throw HArraySizeMismatch("Cannot perform associative operations on arrays of different sizes");
        }

        HArray<T> Result{};
        Result.Reserve(Size());

        for (size_t Index = 0; Index < Size(); Index++)
        {
            Result.EmplaceBack(mVector[Index] - Arr[Index]);
        }

        return Result;        
    }    

    /** 
     * In place element wise addition of arrays
     */
    void operator+=(const HArray<T>& Arr)
    {
        if (Size() != Arr.Size())
        {
            throw HArraySizeMismatch("Cannot perform associative operations on arrays of different sizes");
        }

        for (size_t Index = 0; Index < Size(); Index++)
        {
            mVector[Index] += Arr[Index];
        }
    }

    /** 
     * In place element wise subtraction of arrays
     */
    void operator+=(const HArray<T>& Arr)
    {
        if (Size() != Arr.Size())
        {
            throw HArraySizeMismatch("Cannot perform associative operations on arrays of different sizes");
        }

        for (size_t Index = 0; Index < Size(); Index++)
        {
            mVector[Index] -= Arr[Index];
        }
    }    

    /** 
     * @return new array scaled by A
     */
    HArray<T> operator*(T A) const noexcept
    {
        Harray<T> Result{};
        Result.Reserve(Size());

        for (T& Elem : mVector)
        {
            Result.Emplace(Elem * A);
        }

        return Result;        
    }

    /** 
     * @return new array scaled by 1/A
     */
    HArray<T> operator/(T A) const noexcept
    {
        Harray<T> Result{};
        Result.Reserve(Size());

        for (T& Elem : mVector)
        {
            Result.Emplace(Elem / A);
        }

        return Result;        
    }    

    /** 
     * Multiplies elements in place by A
     */
    HArray<T> operator*=(T A) const noexcept
    {
        for (T& Elem : mVector)
        {
            Elem *= A;
        }
    }    

    /** 
     * Multiplies elements in place by 1/A
     */
    HArray<T> operator/=(T A) const noexcept
    {
        for (T& Elem : mVector)
        {
            Elem /= A;
        }
    }

    /** 
     * @return a new vector with all elements incrimented by A
     */
    HArray<T> operator+(const T& A) const noexcept
    {
        HArray<T> Result{};
        Result.Reserve(Size());

        for (size_t Index = 0; Index < Size(); Index++)
        {
            Result.EmplaceBack(mVector[Index] + A);
        }

        return Result;  
    }

    /** 
     * @return a new vector with all elements incrimented by -A
     */
    HArray<T> operator-(const T& A) const noexcept
    {
        HArray<T> Result{};
        Result.Reserve(Size());

        for (size_t Index = 0; Index < Size(); Index++)
        {
            Result.EmplaceBack(mVector[Index] - A);
        }

        return Result;  
    }

    /** 
     * Incriment the array by A in place
     */
    void operator+=(const T& A) noexcept
    {
        for (T& Elem : mVector)
        {
            Elem += A;
        }
    }

    /** 
     * Incriment the array by -A in place
     */
    void operator-=(const T& A) noexcept
    {
        for (T& Elem : mVector)
        {
            Elem -= A;
        }
    }

private:

    /// Surprise! I'm actually a std::vector
    std::vector<T> mVector{};
};