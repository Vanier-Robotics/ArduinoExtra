/**
 * @file Array.h
 * @author Eliot Fondere
 * @brief Simple Fixed-Size Array Container
 * 
 * @copyright Copyright (c) 2022 Vanier Robotics (MIT License)
 * 
 */
#ifndef _INCLUDE_AEX_ARRAY_H_
#define _INCLUDE_AEX_ARRAY_H_

namespace aex
{

/**
 * @brief STL-like array class
 * 
 * @tparam T type of data contained in the array
 * @tparam S size of the array
 */
template<typename T, size_t S>
class Array
{
public:
    /**
     * @brief Get the size of the array
     * 
     * @return const size_t size of the array
     * 
     * Note the array is always full so the size is also the maximum capacity
     */
    const size_t getSize()
    {
        return S;
    }

    /**
     * @brief Get the first element of the array
     * 
     * @return const T& const reference to the first element (read only)
     */
    const T& front() const
    {
        return data[0];
    }

    /**
     * @brief Get the first element of the array
     * 
     * @return T& reference to the first element (write access)
     */
    T& front()
    {
        return data[0];
    }

    /**
     * @brief Get the last element of the array
     * 
     * @return const T& const reference to the last element (read only)
     */
    const T& back() const
    {
        return data[S - 1];
    }
    
    /**
     * @brief Get the last element of the array
     * 
     * @return T& reference to the last element (write access)
     */
    T& back()
    {
        return data[S - 1];
    }

    /**
     * @brief Get the element at a given index in the array
     * 
     * @param index index or position of the element
     * @return const T& const reference to the element at the given position (read only)
     * 
     * Note: using at() instead of the [] operator checks if the index is out of bounds. However,
     * in this implementation, there is no proper way to check for errors so this is not fully implemented
     */
    const T& at(size_t index) const
    {
        if (index >= S)
        {
            // error
        }

        return data[index];
    }

    /**
     * @brief Get the element at a given index in the array
     * 
     * @param index index or position of the element
     * @return T& reference to the element at a given position (write access)
     * 
     * Note: using at() instead of the [] operator checks if the index is out of bounds. However,
     * in this implementation, there is no proper way to check for errors so this is not fully implemented
     */
    T& at(size_t index)
    {
        if (index >= S)
        {
            // error
        }

        return data[index];
    }

    /**
     * @brief Get the element at a given index in the array
     * 
     * @param index index or position of the element
     * @return const T& const reference to the element at the given poisition (read only)
     * 
     * Note: no checks are perforemed to see if the index is out of bounds
     */
    const T& operator[](size_t index) const
    {
        return data[index];
    }

    /**
     * @brief Get the element at a given index in the array
     * 
     * @param index index or position of the element
     * @return T& reference to the element at a given position (write access)
     * 
     * Note: no checks are perforemed to see if the index is out of bounds
     */
    T& operator[](size_t index)
    {
        return data[index];
    }

    T data[S];  ///< actual data contained in the array
};

}

#endif // _INCLUDE_AEX_ARRAY_H_
