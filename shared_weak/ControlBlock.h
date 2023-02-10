#pragma once
template<typename T>
struct ControlBlock {
    T* object;
    size_t shared_counter{0};
    size_t weak_counter{0};
};
