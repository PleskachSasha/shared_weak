#pragma once
#include "Shared_ptr.h"
#include "ControlBlock.h"

template<typename T>
class Weak_ptr {
private:
	template<class Y> friend class shared_ptr;
	ControlBlock<T>* cptr = nullptr;
public:
	Weak_ptr() noexcept : cptr(new ControlBlock<T>()) {
		++this->cptr->weak_counter;
	}

	Weak_ptr(const Weak_ptr<T>& ptr) : cptr(ptr.cptr)  {
			++this->cptr->weak_counter;
	}
	Weak_ptr(const Shared_ptr<T>& ptr) : cptr(ptr.cptr) {
		++this->cptr->weak_counter;
	}
	Weak_ptr(Weak_ptr<T>&& ptr) : cptr(ptr.cptr) {
		ptr.cptr = nullptr;
	}
	Weak_ptr& operator=(const Weak_ptr& other) noexcept {
		this->cptr = other.cptr;
		++this->cptr->weak_counter;
		return *this;
	}
	
	Weak_ptr& operator=(Weak_ptr&& other) noexcept {
		this->cptr = other.cptr;
		other.cptr = nullptr;
		return *this;
	}
	~Weak_ptr() {
		if (!this->cptr) {
			return;
		}

		--this->cptr->weak_counter;
		if (this->cptr != nullptr)
		{
			if (this->cptr->shared_counter <= 0 && this->cptr->weak_counter <= 0) {
				delete this->cptr;
				this->cptr = nullptr;
			}
		}
			
	}
	size_t use_count() const {
		return this->cptr->shared_counter;
	}
	Shared_ptr<T> lock() const {
		Shared_ptr<T> sp(this->cptr->object);
		sp.cptr->weak_counter = this->cptr->weak_counter; 
		sp.cptr->shared_counter = this->cptr->shared_counter;
		return sp;
	}

	bool expired() const noexcept {
		return !this->cptr || this->cptr->shared_counter <= 0;
	}

	void reset() {
		this->~Weak_ptr();
		this->cptr = nullptr;
	}
		
};