#ifndef ANY_HXX
#define ANY_HXX

#include <exception>
#include <memory>

#include "optional.hxx"

/* Example implementation of an "any" type in C++14.
 * This implementation differs from the boost one in the fact that here, we do
 * not make use of RTTI, but we use the exception system in some strange way to
 * cast. This has some disadvantages, but allows us to cast any containing some
 * object of dervied type to their base class.
 * Arguably, this is not a very useful improvment, but this technic is rarely implemented,
 * so it was a nice exercise.
 * 
 * The main goal of this implementation is not to provide a fully fledged an usable any
 * type for C++14, but to demonstrate how the optional type can be used instead of throwing
 * an exception or returning a naked pointer. Here, if the cast fail, it do not throw,
 * but returns a disengaged optional of the desired type. The user has then the opportunity
 * to check wether the conversion succeeded or failed before accessing the value.
 */

class invalid_any_access : std::logic_error
{
    using std::logic_error::logic_error;  
};

class base_encapsulator
{
    public:
    
    virtual ~base_encapsulator() = default;
    virtual base_encapsulator* clone() const = 0;
};

template<class T>
class encapsulator : public base_encapsulator
{
    using value_type = T;
    public:
    
    encapsulator(const T& value)
    : value_{value}
    {}
    
    encapsulator(T&& value)
    : value_{std::move(value)}
    {}
    
    virtual ~encapsulator() noexcept
    {
        value_.~T();
    }
    
    void swap(T& other) noexcept
    {
        using std::swap;
        swap(*this, other);
    }
    
    virtual base_encapsulator* clone() const override
    {
        return new encapsulator<T>(value_);
    }
    
    T& get_encapsulated_value()
    {
        return value_;
    }
    
    private:
    value_type value_;
};

class any
{
    public:
    any() = delete; // ?
    
    any(const any& other)
    : ptr_{other.ptr_->clone()},
      thrower_{other.thrower_}
    {}
    any(any&& other)
    : ptr_{std::move(other.ptr_)},
      thrower_{other.thrower_}
    {
        other.ptr_.release();
    }
    
    template<class T>
    any(T&& value)
    : ptr_{new encapsulator<T>{std::forward<T>(value)}},
      thrower_(&throw_stub<T>)
    {}
    
    ~any() = default;
    
    template<class T>
    any& operator=(T&& value)
    {
        ptr_ = new encapsulator<std::decay_t<T>>{std::forward<T>(value)};
        thrower_ = throw_stub<T>;
        
        return *this;
    }
    
    template<class T>
    optional<T> as() noexcept
    {
        optional<T> ret;
        
        try
        {
            thrower_(ptr_.get());
        }
        catch(T& e)
        {
            ret = e;
        }
        catch(...)
        {}
        
        return ret;
    }
    
    void swap(any& other) noexcept
    {
        // Enable correct ADL;
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(thrower_, other.thrower_);
    }
    
    private:
    std::unique_ptr<base_encapsulator> ptr_;
    void(*thrower_)(void*);
    
    template<class T>
    static void throw_stub(void* ptr)
    {
        throw static_cast<T&>(static_cast<encapsulator<T>*>(ptr)->get_encapsulated_value());
    }
};

void swap(any& lhs, any& rhs)
{
    lhs.swap(rhs);
}

#endif // ANY_HXX