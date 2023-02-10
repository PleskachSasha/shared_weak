#pragma once
#include <iostream>
#include "ControlBlock.h"

template<typename T>
class Shared_ptr {
public:
    Shared_ptr(T* value) : m_value(value), cptr(new ControlBlock<T>())
    {
        cptr->object = value;
        ++this->cptr->shared_counter; 
        ++this->cptr->weak_counter;
    }

    Shared_ptr(const Shared_ptr& other) : m_value(other.m_value), cptr(other.cptr) {
        if (other.m_value != nullptr) {
            ++(this->cptr->shared_counter);
            ++this->cptr->weak_counter;
        }
            
    }
    Shared_ptr& operator=(const Shared_ptr& other) {

        this->cptr = other.cptr;
        this->m_value = other.m_value;

        if (other.m_value != nullptr) {
            ++this->cptr->weak_counter;
            ++(this->cptr->shared_counter);
        }
            

        return *this;
    }
    Shared_ptr(Shared_ptr&& other) noexcept : m_value(other.m_value), cptr(other.cptr) {
        other.m_value = nullptr;
        other.cptr = nullptr;
    }

    Shared_ptr& operator=(Shared_ptr&& other) {
        this->cptr = other.cptr;
        this->m_value = other.m_value;

        other.m_value = nullptr;
        other.cptr = nullptr;
        
        return *this;
    }
    ~Shared_ptr() {
        if (cptr != nullptr)
        {
            --(cptr->shared_counter);
        }
        if (cptr != nullptr) {
            if (cptr->shared_counter == 0) {
                delete m_value;
                m_value = nullptr;
                if (cptr->weak_counter == 0)
                {
                    delete cptr;
                    cptr = nullptr;
                }
            }
        }
    }

    void reset() {
        this->~Shared_ptr();
        m_value = nullptr;
        cptr = nullptr;
    }
    void reset(T* ptr) {
        this->~Shared_ptr();
        m_value = ptr;
        cptr = new ControlBlock<T>();
    }

    T* get() const {
        return m_value;
    }
    explicit operator bool() const {
        return m_value != nullptr;
    }
    size_t use_count() const {
        return cptr->shared_counter;
    }
    T& operator *() const {
        return *m_value;
    }
    T* operator ->() const {
        return m_value;
    }

private:
    T* m_value;
    ControlBlock<T>* cptr = nullptr;

    template<class Y> friend class shared_ptr;
    template<class Y> friend class Weak_ptr;
};


