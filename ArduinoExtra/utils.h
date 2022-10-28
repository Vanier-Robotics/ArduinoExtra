/**
 * @file utils.h
 * @author Eliot Fondere
 * @brief Utility functions for ArduinoExtra (move semantics, forward, etc.)
 * 
 * @copyright Copyright (c) 2022 Vanier Robotics (MIT License)
 * 
 * Credits:
 * https://stackoverflow.com/questions/37503114/how-to-implement-remove-reference
 * https://stackoverflow.com/questions/7510182/how-does-stdmove-transfer-values-into-rvalues
 * https://www.foonathan.net/2020/09/move-forward/
 */
 
#ifndef _INCLUDE_AEX_UTILS_H_
#define _INCLUDE_AEX_UTILS_H_
 
namespace aex
{
 
/**
 * @brief Return the intial type of a variable
 * 
 * @tparam T type (with or without reference)
 */
template<typename T>
struct remove_reference
{
    typedef T type;
};
 
/**
 * @brief Template specialization for lvalue references
 * 
 * @tparam T lvalue reference
 * 
 * @see remove_reference
 */
template<typename T>
struct remove_reference<T&>
{
    typedef T type;
};
 
/**
 * @brief Template specialization for rvalue references
 * 
 * @tparam T rvalue reference
 * 
 * @see remove_reference
 */
template<typename T>
struct remove_reference<T&&>
{
    typedef T type;
};
 
/**
 * @brief Cast a variable to an rvalue
 * 
 * @tparam T type of the variable
 * @param value value to cast
 * @return remove_reference<T>::type&& resulting rvalue
 */
template<typename T>
inline typename remove_reference<T>::type&& move(T&& value)
{
    return static_cast<typename remove_reference<T>::type&&>(value);
}
 
/**
 * @brief Forward argument(s) (from variadic template)
 * 
 * @tparam T type of the value
 * @param value lvalue to forward
 * @return constexpr T&& forwarded rvalue
 */
template<typename T>
constexpr T&& forward(typename remove_reference<T>::type& value) noexcept
{
    return static_cast<T&&>(value);
}
 
/**
 * @brief Forward argument(s) (from variadic template)
 * 
 * @tparam T type of the value
 * @param value rvalue to forward
 * @return constexpr T&& forwarded rvalue
 */
template<typename T>
constexpr T&& forward(typename remove_reference<T>::type&& value) noexcept
{
    // assert(!is_lvalue_reference<T>::value);
    return static_cast<T&&>(value);
}
 
} // namespace aex
 
#endif // _INCLUDE_AEX_UTILS_H_
