module;

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <new>
#include <ranges>
#include <type_traits>
#include <utility>

export module small_vec;

export template <typename T, size_t N = 4>
struct SmallVec {
    static_assert(N > 0, "SmallVec requires N > 0");

    using value_type = T;

    SmallVec() noexcept = default;

    SmallVec(std::initializer_list<T> init) {
        reserve(init.size());
        for (const auto& value : init) {
            emplace_back(value);
        }
    }

    SmallVec(const SmallVec& other) {
        reserve(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            emplace_back(other[i]);
        }
    }

    SmallVec(SmallVec&& other) noexcept(std::is_nothrow_move_constructible_v<T>) { move_from(std::move(other)); }

    ~SmallVec() {
        clear();
        deallocate_heap();
    }

    SmallVec& operator=(const SmallVec& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        reserve(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            emplace_back(other[i]);
        }
        return *this;
    }

    SmallVec& operator=(SmallVec&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
        if (this == &other) {
            return *this;
        }
        clear();
        deallocate_heap();
        move_from(std::move(other));
        return *this;
    }

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }

    T* data() noexcept { return storage(); }
    const T* data() const noexcept { return storage(); }

    T* begin() noexcept { return storage(); }
    const T* begin() const noexcept { return storage(); }
    const T* cbegin() const noexcept { return storage(); }
    T* end() noexcept { return storage() + size_; }
    const T* end() const noexcept { return storage() + size_; }
    const T* cend() const noexcept { return storage() + size_; }

    T& operator[](size_t index) noexcept { return storage()[index]; }
    const T& operator[](size_t index) const noexcept { return storage()[index]; }

    template <typename... Args>
    T& emplace_back(Args&&... args) {
        if (size_ == capacity()) {
            reserve(grow_capacity());
        }
        T* slot = storage() + size_;
        ::new (static_cast<void*>(slot)) T(std::forward<Args>(args)...);
        ++size_;
        return *slot;
    }

    void push_back(const T& value) { emplace_back(value); }
    void push_back(T&& value) { emplace_back(std::move(value)); }

    void pop_back() {
        if (size_ == 0) {
            return;
        }
        --size_;
        std::destroy_at(storage() + size_);
    }

    void clear() noexcept {
        destroy_elements();
        size_ = 0;
    }

    void resize(size_t new_size) {
        if (new_size < size_) {
            T* ptr = storage();
            for (size_t i = new_size; i < size_; ++i) {
                std::destroy_at(ptr + i);
            }
            size_ = new_size;
            return;
        }
        if (new_size > size_) {
            reserve(new_size);
            T* ptr = storage();
            for (size_t i = size_; i < new_size; ++i) {
                ::new (static_cast<void*>(ptr + i)) T{};
            }
            size_ = new_size;
        }
    }

    void resize(size_t new_size, const T& value) {
        if (new_size < size_) {
            T* ptr = storage();
            for (size_t i = new_size; i < size_; ++i) {
                std::destroy_at(ptr + i);
            }
            size_ = new_size;
            return;
        }
        if (new_size > size_) {
            reserve(new_size);
            T* ptr = storage();
            for (size_t i = size_; i < new_size; ++i) {
                ::new (static_cast<void*>(ptr + i)) T(value);
            }
            size_ = new_size;
        }
    }

    void reserve(size_t new_cap) {
        if (new_cap <= capacity()) {
            return;
        }
        T* new_storage = allocate(new_cap);
        move_elements(new_storage);
        deallocate_heap();
        heap_storage = new_storage;
        capacity_    = new_cap;
    }

    void shrink_to_fit() {
        if (capacity_ == N) {
            return;
        }
        if (size_ <= N) {
            T* inline_ptr = inline_storage;
            move_elements(inline_ptr);
            deallocate_heap();
            capacity_ = N;
            return;
        }
        if (size_ == capacity_) {
            return;
        }
        T* new_storage = allocate(size_);
        move_elements(new_storage);
        deallocate_heap();
        heap_storage = new_storage;
        capacity_    = size_;
    }

   private:
    size_t size_     = 0;
    size_t capacity_ = N;

    union {
        alignas(T) std::byte inline_storage[sizeof(T) * N];
        T* heap_storage;
    };

    T* storage() noexcept { return capacity_ > N ? heap_storage : std::launder(reinterpret_cast<T*>(inline_storage)); }
    const T* storage() const noexcept {
        return capacity_ > N ? heap_storage : std::launder(reinterpret_cast<const T*>(inline_storage));
    }

    static T* allocate(size_t cap) {
        return static_cast<T*>(::operator new(sizeof(T) * cap, std::align_val_t{alignof(T)}));
    }

    void deallocate_heap() noexcept {
        if (capacity_ == N) {
            return;
        }
        ::operator delete(static_cast<void*>(heap_storage), std::align_val_t{alignof(T)});
    }

    void destroy_elements() noexcept {
        T* ptr = storage();
        for (size_t i = 0; i < size_; ++i) {
            std::destroy_at(ptr + i);
        }
    }

    void move_elements(T* dest) {
        T* src = storage();
        for (size_t i = 0; i < size_; ++i) {
            ::new (static_cast<void*>(dest + i)) T(std::move(src[i]));
            std::destroy_at(src + i);
        }
    }

    size_t grow_capacity() const noexcept {
        const size_t cap = capacity();
        return cap == 0 ? 1 : cap * 2;
    }

    void move_from(SmallVec&& other) {
        if (other.capacity_ > N) {
            heap_storage       = other.heap_storage;
            size_              = other.size_;
            capacity_          = other.capacity_;
            other.heap_storage = nullptr;
            other.size_        = 0;
            other.capacity_    = N;
            return;
        }
        size_     = other.size_;
        capacity_ = N;
        T* src    = other.storage();
        for (size_t i = 0; i < size_; ++i) {
            ::new (static_cast<void*>(storage() + i)) T(std::move(src[i]));
            std::destroy_at(src + i);
        }
        other.size_ = 0;
    }
};

static_assert(std::ranges::range<SmallVec<int, 4>>, "SmallVec should satisfy the range concept");

void test() {
    SmallVec<int, 4> vec = {1, 2, 3};
    vec.push_back(4);
    vec.push_back(5);  // should trigger heap allocation
    auto vec2 = vec | std::views::filter([](int x) { return x % 2 == 0; }) |
                std::views::transform([](int x) { return x * 10; }) | std::ranges::to<SmallVec<int, 4>>();
}