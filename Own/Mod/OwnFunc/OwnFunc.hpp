//
// Created by Administrator on 25-5-14.
//

#pragma once
#include <array>

// 嵌入式环境中避免使用标准库头文件，定义简易概念
template<typename T>
concept CallableVoid = requires(T t) {
    { t() } -> std::same_as<void>;
};

class MyFunction {
private:
    static constexpr size_t STORAGE_SIZE = 8; // 足够存储 this 指针（4 字节 + 对齐）
    static constexpr size_t STORAGE_ALIGN = 4; // 32 位系统对齐
    std::array<char, STORAGE_SIZE> storage_;
    using InvokeFn = void (*)(const char*);
    using DestroyFn = void (*)(char*);
    InvokeFn invoke_ = nullptr;
    DestroyFn destroy_ = nullptr;

    template<typename T>
    struct CallableWrapper {
        static void invoke(const char* storage) {
            const T& callable = *reinterpret_cast<const T*>(storage);
            callable();
        }
        static void destroy(char* storage) {
            reinterpret_cast<T*>(storage)->~T();
        }
    };

public:
    // 构造函数
    template<typename T>
    requires CallableVoid<T> && (sizeof(T) <= STORAGE_SIZE)
    MyFunction(T c) {
        static_assert(alignof(T) <= STORAGE_ALIGN, "Alignment too large");
        new (storage_.data()) T(c);
        invoke_ = &CallableWrapper<T>::invoke;
        destroy_ = &CallableWrapper<T>::destroy;
    }

    // 默认构造函数
    MyFunction() = default;

    // 析构函数
    ~MyFunction() {
        if (destroy_) {
            destroy_(storage_.data());
        }
    }

    // 拷贝赋值
    MyFunction& operator=(const MyFunction& other) {
        if (this != &other) {
            if (destroy_) {
                destroy_(storage_.data());
            }
            if (other.invoke_) {
                std::memcpy(storage_.data(), other.storage_.data(), STORAGE_SIZE);
                invoke_ = other.invoke_;
                destroy_ = other.destroy_;
            } else {
                invoke_ = nullptr;
                destroy_ = nullptr;
            }
        }
        return *this;
    }

    // 移动赋值
    MyFunction& operator=(MyFunction&& other) noexcept {
        if (this != &other) {
            if (destroy_) {
                destroy_(storage_.data());
            }
            std::memcpy(storage_.data(), other.storage_.data(), STORAGE_SIZE);
            invoke_ = other.invoke_;
            destroy_ = other.destroy_;
            other.invoke_ = nullptr;
            other.destroy_ = nullptr;
        }
        return *this;
    }

    // 调用
    void operator()() const {
        if (invoke_) {
            invoke_(storage_.data());
        }
    }

    // 比较 != nullptr
    bool operator!=(std::nullptr_t) const noexcept {
        return invoke_ != nullptr;
    }

    // 比较 == nullptr
    bool operator==(std::nullptr_t) const noexcept {
        return invoke_ == nullptr;
    }
};

