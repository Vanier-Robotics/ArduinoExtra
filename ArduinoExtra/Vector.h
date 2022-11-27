/**
 * @file Vector.h
 * @author Eliot Fondere
 * @brief Simple Dynamic Array Container
 * 
 * @copyright Copyright (c) 2022 Vanier Robotics (MIT License)
 * 
 * From:
 * https://www.youtube.com/watch?v=ryRf4Jh_YC0
 */
 
#ifndef _INCLUDE_AEX_VECTOR_H_
#define _INCULDE_AEX_VECTOR_H_
 
#include "utils.h"
 
namespace aex
{
 
/**
 * @brief STL-like vector class
 * 
 * @tparam T type of data contained in the array
 */
template<typename T>
class Vector
{
public:
    /**
     * @brief Initialize an empty vector with capacity of 0
     * 
     */
    Vector()
    {
    }

    /**
     * @brief Initialize an empty vector with a given capacity
     * 
     * @param initialCapacity intial capacity of the vector
     */
    Vector(size_t initialCapacity)
    {
        reAllocate(initialCapacity);
    }
 
    /**
     * @brief Empty the vector and free memory
     * 
     */
    ~Vector()
    {
        clear();
        //::operator delete(m_data, m_capacity * sizeof(T));
        delete[] m_data;
    }
 
    /**
     * @brief Add an element to the back of the vector
     * 
     * @param value new element (to be copied)
     */
    void pushBack(const T& value)
    {
        // re-allocate memory if required
        if (m_size >= m_capacity)
        {
            reAllocate(m_capacity + m_capacity / 2 + 2);
        }
 
        // copy data and increase the size
        m_data[m_size] = value;
        m_size ++;
    }
 
    /**
     * @brief Add an element to the back of the vector
     * 
     * @param value rvalue reference to the new element
     * 
     * This is useful when creating a new object directly in the vector
     * (avoids copying the object around and allocating extra memory)
     */
    void pushBack(T&& value)
    {
        // re-allocate memory if required
        if (m_size >= m_capacity)
        {
            reAllocate(m_capacity + m_capacity / 2 + 2);
        }
 
        // copy data and increase the size
        m_data[m_size] = move(value);
        m_size ++;
    }
 
    /**
     * @brief Constructs an element and adds it to the array
     * 
     * @tparam Args types of the arguments to pass to the constructor (no need to manually type)
     * @param args arguments to pass to the constructor
     * @return T& reference to the new object
     */
    /*template<typename... Args>
    T& emplaceBack(Args... args)
    {
        if (m_size >= m_capacity)
        {
            reAllocate(m_capacity + m_capacity / 2 + 2);
        }
 
        // add the data
        new(&m_data[m_size]) T(forward<Args>(args)...); // create in place
        return m_data[m_size++];
    }*/
 
    /**
     * @brief Remove the last element from the vector
     * 
     */
    void popBack()
    {
        if (m_size > 0)
        {
            m_size--;
            m_data[m_size].~T();
        }
    }
 
    /**
     * @brief Empty the vector
     * 
     */
    void clear()
    {
        for (size_t i = 0; i < m_size; i++)
        {
            m_data[m_size].~T();
        }
 
        m_size = 0;
    }
 
    /**
     * @brief Access data contained in the vector
     * 
     * @param index index of where the data is stored
     * @return const T& const reference to the data
     */
    const T& operator[](size_t index) const
    {
        if (index >= m_size)
        {
            // error
        }
 
        return m_data[index];
    }
 
    /**
     * @brief Access data contained in the vector
     * 
     * @param index index of where the data is stored
     * @return T& reference to the data
     * 
     * This version of the [] operator allows you to modify the value
     */
    T& operator[](size_t index)
    {
        if (index >= m_size)
        {
            // error
        }
 
        return m_data[index];
    }
 
    /**
     * @brief Get the amount of element stored in the vector
     * 
     * @return size_t size of the vector
     */
    size_t getSize() const
    {
        return m_size;
    }
 
    /**
     * @brief Check if the vector is empty
     * 
     * @return true vector is empty
     * @return false vector is not empty
     */
    const bool isEmpty() const
    {
        return (m_size == 0);
    }
 
private:
    /**
     * @brief Re-allocate a new block of memory and move all current elements
     * 
     * @param newCapacity size of array to allocate
     */
    void reAllocate(size_t newCapacity)
    {
        // allocate new block of memory
        T* newBlock = (T*)::operator new(newCapacity * sizeof(T));
 
        if (newCapacity < m_size)
        {
            m_size = newCapacity;
            // warning
        }
 
        // move old elements
        for (size_t i = 0; i < m_size; i++)
        {
            newBlock[i] = move(m_data[i]);
        }
 
        for (size_t i = 0; i < m_size; i++)
        {
            m_data[i].~T();
        }
 
        // delete old elements
        //::operator delete(m_data, m_capacity * sizeof(T));
        delete[] m_data;
        m_data     = newBlock;
        m_capacity = newCapacity;
    }
 
private:
    T* m_data = nullptr;   ///< pointer to the heap-allocated array
 
    size_t m_size     = 0; ///< number of elements in the vector
    size_t m_capacity = 0; ///< max amount of elements the vector can store with currently allocated memory
};
 
} // namespace aex
 
#endif // _INCLUDE_AEX_VECTOR_H_