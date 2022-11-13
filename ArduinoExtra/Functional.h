/**
 * @file Functional.h
 * @author Eliot Fondere
 * @brief Function wrapper for global functions, static member functions, lambdas and member functions
 * 
 * @copyright Copyright (c) 2022 Vanier Robotics (MIT License)
 *
 * @example
 * class ExampleClass
 * {
 * public:
 *     void exampleMethod(float value)
 *     {
 *         Serial.println(value + m_value);
 *     }
 *
 *     static void exampleStaticMethod(float value)
 *     {
 *         Serial.println(value);
 *     }
 *
 * private:
 *     float m_value;
 * };
 *
 * float exampleFunction(float value1, float value2)
 * {
 *     return value1 * value2;
 * }
 *
 * void setup()
 * {
 *     ExampleClass exampleObject;
 *
 *     aex::Function<void(float)> exampleFunction1 =
 *         aex::Function<void(float)>::bind<ExampleClass>(exampleObject, &ExampleClass::exampleMethod);
 *     exampleFunction1 = aex::Function<void(float)>::bind(ExampleClass::exampleStaticMethod);
 *
 *     exampleFunction1(3.0f); // Will execute ExampleClass::exampleStaticMethod(3.0f)
 *
 *     aex::Function<float(float, float)> exampleFunction2 =
 *     	   aex::Function<float(float, float)>::bind(exampleFunction);
 *     Serial.println(exampleFunction2(2.0f, 5.0f)); // Will output "10.00" to the console
 *
 *     aex::Function<void()> exampleFunction2([]() -> void { Serial.println("Lambda Function"); };
 *     exampleFunction2(); // Will write "Lambda Function" to the console
 * }
 */

#ifndef _INCLUDE_AEX_FUNCTIONAL_H_
#define _INCLUDE_AEX_FUNCTIONAL_H_

namespace aex
{

/**
 * @brief Allow for template specialization of the Function class
 */
template<class... Args> class Function;

namespace priv
{

/**
 * @brief Allow for template specialization of the Callable classes
 *
 * @todo: Add a callable for const functions?
 */
template<class...> class Callable;
template<class...> class FunctionCallable;
template<class...> class MemberCallable;

/**
 * @brief Callable parent class. Container for a callable object
 *
 * @tparam R Return type of the callable object
 * @tparam Args Types of arguments passed to the callable object
 */
template<class R, class... Args>
class Callable<R(Args...)>
{
public:
	friend class aex::Function<R(Args...)>; // For Callable* copy()

	/**
	 * @brief Execute the callable object
	 *
	 * @param args Arguments passed when calling the object
	 * @return Return value of the object
	 */
	virtual R operator()(Args... args) = 0;

private:
	/**
	 * @brief Copy a Callable object
	 *
	 * @return A pointer to the new callable object
	 *
	 * @note This function is implemented by the children of this class to allow copying a function
	 * without losing information on the type of callable. Ownership of the memory is transferred to
	 * whatever calls this function.
	 */
	virtual Callable* copy() const = 0;
};

/**
 * @brief Callable type for general functions, static methods and lambdas
 *
 * @tparam R Return type of the function
 * @tparam Args Types of the arguments passed to the function
 */
template<class R, class... Args>
class FunctionCallable<R(Args...)> : public Callable<R(Args...)>
{
public:
	using functionPtr_t = R(*)(Args...); ///< Type of the function pointer

	/**
	 * @brief Create a new FunctionCallable with a function pointer
	 *
	 * @param functionPtr Function pointer to the desired function
	 */
	explicit FunctionCallable(functionPtr_t functionPtr)
	{
		m_functionPtr = functionPtr;
	}

	/**
	 * @brief Copy the FunctionCallable by allocating a new one
	 *
	 * @return Pointer to the new FunctionCallable
	 */
	Callable<R(Args...)>* copy() const override
	{
		return new FunctionCallable<R(Args...)>(m_functionPtr);
	}

	/**
	 * @brief Execute the function
	 *
	 * @param args Arguments passed to the function
	 * @return Return value of the function
	 */
	R operator()(Args... args) override
	{
		return m_functionPtr(args...);
	}

private:
	functionPtr_t m_functionPtr; ///< Pointer to the function
};

/**
 * @brief Callable type for member functions / methods.
 *
 * @tparam C Type of the class containing the method
 * @tparam R Return type of the method
 * @tparam Args Types of arguments passed to the method
 */
template<class C, class R, class... Args>
class MemberCallable<C, R(Args...)> : public Callable<R(Args...)>
{
public:
	using functionPtr_t = R(C::*)(Args...); ///< Type of the function pointer

	/**
	 * @brief Create a new MemberCallable from a reference to an object and a function pointer
	 *
	 * @param objectReference Reference to the object from which the method is called
	 * @param functionPtr Pointer to the function
	 *
	 * @note Make sure to add the reference symbol before the function pointer (i.e.
	 * &ClassName::methodName).
	 */
	MemberCallable(C& objectReference, functionPtr_t functionPtr)
	: m_objectReference(objectReference)
	{
		m_functionPtr = functionPtr;
	}

	/**
	 * @brief Copy the MemberCallable by allocating a new one
	 *
	 * @return Pointer to the new MemberCallable
	 */
	Callable<R(Args...)>* copy() const override
	{
		return new MemberCallable<C, R(Args...)>(m_objectReference, m_functionPtr);
	}

	/**
	 * @brief Execute the method
	 *
	 * @param args Arguments passed to the method
	 * @return Return value of the method
	 */
	R operator()(Args... args) override
	{
		return (m_objectReference.*m_functionPtr)(args...);
	}

private:
	C&            m_objectReference; ///< Reference to the object from which the method is called
	functionPtr_t m_functionPtr;     ///< Pointer to the function
};

} // priv

/**
 * @brief Function wrapper for global functions, lambdas, static methods and instance methods
 *
 * @tparam R Return type of the function
 * @tparam Args Types of the arguments passed to the function
 *
 * @note Write the type like Function<void(int, float)> and not Function<void, int, float>.
 */
template<class R, class... Args>
class Function<R(Args...)>
{
public:
	/**
	 * @brief Create a Function object using a simple pointer to a function
	 *
	 * @param functionPtr Pointer to the function
	 *
	 * @note This constructor is used to make it easier to create Function objects from global
	 * functions, static member functions and lambdas.
	 */
	Function(R(*functionPtr)(Args...))
	{
		m_callable = new priv::FunctionCallable<R(Args...)>(functionPtr);
	}

	/**
	 * @brief Create a Function object by copying another
	 *
	 * @param other Function object to copy
	 */
	Function(Function& other)
	{
		m_callable = other.m_callable->copy();
	}

	/**
	 * @brief Create a Function object by copying another
	 *
	 * @param other Function object to copy
	 */
	Function(const Function& other)
	{
		m_callable = other.m_callable->copy();
	}

	/**
	 * @brief Copy a Function object
	 *
	 * @param other Function object to copy
	 * @return Reference to this object
	 *
	 * @note Safely deletes the previous callable object if necessary
	 */
	Function& operator=(Function& other)
	{
		if (m_callable != nullptr)
		{
			delete m_callable;
		}

		m_callable = other.m_callable->copy();
	}

	/**
	 * @brief Copy a Function object
	 *
	 * @param other Function object to copy
	 * @return Reference to this object
	 *
	 * @note Safely deletes the previous callable object if necessary
	 */
	Function& operator=(const Function& other)
	{
		if (m_callable != nullptr)
		{
			delete m_callable;
		}

		m_callable = other.m_callable->copy();
	}

	/**
	 * @brief Destroy the Function object and delete the callable object
	 */
	~Function()
	{
		delete m_callable;
	}

	/**
	 * @brief Execute the function
	 *
	 * @param args Arguments passed to the function
	 * @return Return value of the function
	 */
	R operator()(Args... args)
	{
		return (*m_callable)(args...);
	}

	/**
	 * @brief Create a new function object for global functions, lambdas and static methods
	 *
	 * @param functionPtr Pointer to the function
	 * @return The new function object
	 */
	static Function<R(Args...)> bind(R(*functionPtr)(Args...))
	{
		return {new priv::FunctionCallable<R(Args...)>(functionPtr)};
	}

	/**
	 * @brief Create a new function object for member functions
	 *
	 * @tparam C Type of the class containing the method
	 * @param objectRef Reference to the object from which the method is called
	 * @param functionPtr Pointer to the function
	 * @return The new function object
	 */
	template<class C>
	static Function<R(Args...)> bind(C& objectRef, R(C::*functionPtr)(Args...))
	{
		return {new priv::MemberCallable<C, R(Args...)>(objectRef, functionPtr)};
	}

private:
	/**
	 * @brief Create a Function using a pointer to a callable
	 *
	 * @param callable Pointer to the heap-allocated callable object
	 *
	 * @see Function::bind()
	 * @note The ownership of the pointer is assumed to be transferred to the new Function object
	 */
	Function(priv::Callable<R(Args...)>* callable)
	: m_callable(callable)
	{
	}

	priv::Callable<R(Args...)>* m_callable = nullptr; ///< Internal callable object
};

} // aex

#endif // _INCLUDE_AEX_FUNCTIONAL_H_
