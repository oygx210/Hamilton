#pragma once

#include <vector>
#include "errors.hpp"

/** 
 * @file harray.hpp 
 */

/** 
 * Hamilton array type. Wrapper around std::vector with support for 
 * element wise addition/substraction with another array. Multiplication/Division 
 * with a scalar, and the equivilant in place operations. Requires the underlying
 * type T to support those operations
 */
template <typename T = double>
class HArray
{
public:    

    /** 
     * Default array of type T of zero length
     */
    HArray() { }

    /** 
     * Default initialise array with the given number of elements
     * @param NumberElements Number of elements in array
     */
    static HArray<T> OfSize(size_t NumberElements)
    {
        return HArray<T>(std::vector<T>(NumberElements));
    }

    /** 
     * Default initialise array with the number of given elements 
     * set to an initial value
     * @param NumberElements Number of elements in array
     * @param Init Initial value of array elements
     */
    static HArray<T> OfSizeWithInit(size_t NumberElements, T Init)
    {
        return HArray<T>(std::vector<T>(NumberElements, Init));
    }

    /** 
     * Initialise from a set of elements
     * @param Args Initial array elements
     */
    template <typename... Elements>
    HArray(Elements... Args) : mVector{Args...} { }

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
    void Resize(size_t NewSize) noexcept {mVector.resize(NewSize);}

    /** 
     * @return Iterator to the first element
     */
    const auto Begin() const {return mVector.begin();}
    auto Begin() {return mVector.begin();}    

    const auto begin() const {return Begin();}    
    auto begin() {return Begin();}

    /** 
     * @return Iterator to the last element
     */
    const auto End() const {return mVector.end();}
    auto End() {return mVector.end();}

    const auto end() const {return End();}
    auto end() {return End();}

    /** 
     * @return Constant iterator to the first element
     */
    const auto CBegin() const noexcept {return mVector.cbegin();}

    /** 
     * @return Constant iterator to the last element
     */
    const auto CEnd() const noexcept {return mVector.cend();}    

    /** 
     * Inserts an element into the array at the specified location
     * @param Iterator Iterator to the location to insert element at
     * @param Elem Element to insert
     * @return Iterator pointing to the inserted value
     */
    auto Insert(const auto Iterator, const T& Elem) {return mVector.insert(Iterator, Elem);}

    /** 
     * Inserts a range into the array at the specified location
     * @param Position in the array where the new elements are inserted.
     * @param First
     * @param Last Iterators specifying a range of elements. Copies of the elements 
     * in the range [First,Last) are inserted at position (in the same order).
     * @return An iterator that points to the first of the newly inserted elements.
     */
    auto Insert(const auto Position, const auto First, const auto Last) {return mVector.insert(Position, First, Last);}

    /** 
     * Erase an element at the given location
     * @param Position Iterator to the position to be removed
     * @return An iterator pointing to the new location of the 
     * element that followed the last element erased by the function call. 
     * This is the container end if the operation erased the last element in the sequence.
     */
    auto Erase(const auto Position) {return mVector.erase(Position);}

    /** 
     * @param First
     * @param Last Iterators specifying a range within the array] to be removed: 
     * [First, Last). i.e., the range includes all the elements between first and last, 
     * including the element pointed by first but not the one pointed by last. 
     * Member types iterator and const_iterator are random access iterator types that 
     * point to elements.
     * @return An iterator pointing to the new location of the 
     * element that followed the last element erased by the function call. 
     * This is the container end if the operation erased the last element in the sequence. 
     */
    auto Erase(auto First, auto Last) {return mVector.erase(First, Last);}

    /** 
     * Appends a copy of the specified array to the back of this array
     * @param Arr Array to be copied to back
     */
    void AppendBack(const HArray<T>& Arr) {Insert(CEnd(), Arr.CBegin(), Arr.CEnd());}

    /** 
     * @return standard library vector
     */
    const std::vector<T>& CppVector(void) const noexcept {return mVector;}

    /** 
     * @param Index location of the element to retrieve
     * @return Element at the given `Index`
     */
    const T& operator[](size_t Index) const {return mVector[Index];}
    T& operator[](size_t Index) {return mVector[Index];}

    /** 
     * Get element by index, throws if out of bounds
     * @param Index location of the element to retrieve
     * @return Element at the given `Index`
     */
    const T& IndexSafe(size_t Index) const
    {
        if (Index >= Size())
        {
            throw Error::OutOfBoundsException(__FILE__, __LINE__, Index, Size());
        }

        return mVector[Index];
    }

    /** 
     * Get element by index, throws if out of bounds
     * @param Index location of the element to retrieve
     * @return Element at the given `Index`
     */
    T& IndexSafe(size_t Index)
    {
        if (Index >= Size())
        {
            throw Error::OutOfBoundsException(__FILE__, __LINE__, Index, Size());
        }

        return mVector[Index];
    }    

    /** 
     * @param Arr array to compare too
     * @return true if arrays are identical
     */
    bool operator==(const HArray<T>& Arr) const noexcept {return mVector == Arr.mVector;}

    /** 
     * Element wise addition of arrays
     */
    HArray<T> operator+(const HArray<T>& Arr) const
    {
        if (Size() != Arr.Size())
        {
            throw Error::HArraySizeMismatch(__FILE__, __LINE__);
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
            throw Error::HArraySizeMismatch(__FILE__, __LINE__);
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
            throw Error::HArraySizeMismatch(__FILE__, __LINE__);
        }

        for (size_t Index = 0; Index < Size(); Index++)
        {
            mVector[Index] += Arr[Index];
        }
    }

    /** 
     * In place element wise subtraction of arrays
     */
    void operator-=(const HArray<T>& Arr)
    {
        if (Size() != Arr.Size())
        {
            throw Error::HArraySizeMismatch(__FILE__, __LINE__);
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
        HArray<T> Result{};
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
        HArray<T> Result{};
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
    void operator*=(T A) noexcept
    {
        for (T& Elem : mVector)
        {
            Elem *= A;
        }
    }    

    /** 
     * Multiplies elements in place by 1/A
     */
    void operator/=(T A) noexcept
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

    /// Instatiate from a c++ vector
    HArray(std::vector<T> Vec) : mVector{Vec} { }
};