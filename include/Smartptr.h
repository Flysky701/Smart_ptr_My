#include <utility> 
#include <atomic> 

template <typename T>
class shared_ptr{
    public:
        explicit shared_ptr(T* ptr = nullptr) noexcept
            : ptr_(ptr), count_(ptr ? new std::atomic<size_t> (1) : nullptr)
        {}
        
        // 复制
        shared_ptr(const shared_ptr& other) noexcept
            : ptr_(other.ptr_), count_(other.count_)
        {
            if(count_){
                count_ -> fetch_add(1, std::memory_order_relaxed);
            }
        }
        
        // 移动
        shared_ptr(shared_ptr&& other) noexcept
            : ptr_(other.ptr_), count_(other.count_)
        {
            other.ptr_ = nullptr;
            other.count_ = nullptr;
        }
        
        ~shared_ptr(){
            release();
        }
        shared_ptr& operator =(const shared_ptr& other) noexcept{
            if(this != &other){
                release();
                ptr_ = other.ptr_;
                count_ = other.count_;
                if(count_){
                    count_ -> fetch_add(1, std::memory_order_relaxed);
                }
            }
            return *this;
        }

        shared_ptr& operator =(shared_ptr&& other) noexcept{
            if(this != &other){
                release();
                ptr_ = other.ptr_;
                count_ = other.count_;
                other.ptr_ = nullptr;
                other.count_ = nullptr;
            }
        }
        
        T& operator* ()const noexcept{
            return *ptr_;
        }

        T* operator-> ()const noexcept{
            return ptr_;
        }

        T* get(){
            return ptr_;
        }

        size_t usecount() const noexcept{
            return count_ ? count_-> load(std::memory_order_relaxed): 0;
        }
        

    private:
        T* ptr_;
        std::atomic<size_t>* count_;

        void release() noexcept{
            if(count_){
                if(count_ -> fetch_sub(1) == 1){
                    delete ptr_;
                    delete count_;
                }
                ptr_ = nullptr;
                count_ = nullptr;
            }
        }
};

template <typename T>
class unique_ptr{
    public:
        explicit unique_ptr(T* ptr = nullptr) noexcept
            :ptr_(ptr){}
        
        unique_ptr(const unique_ptr &) noexcept = delete;

        unique_ptr(unique_ptr &&other) noexcept 
            :ptr_(other.ptr_){
                other.ptr_ = nullptr;
            }
        
        ~unique_ptr(){
            delete ptr_;
        }

        unique_ptr& operator=(const unique_ptr& other) = delete;

        unique_ptr& operator=(unique_ptr&& other) noexcept{
            if(this != &other){
                delete ptr_;
                this -> ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        T* operator-> ()const noexcept{
            return ptr_;
        }

        T& operator* ()const noexcept{
            return *ptr_;
        }

        T* get()const noexcept{
            return ptr_;
        }
        T* release() noexcept{
            T* old = ptr_;
            ptr_ = nullptr;
            return old;
        }

        void reset(T* new_re = nullptr){
            delete ptr_;
            ptr_ = new_re;
        }

        void swap(unique_ptr &other){
            std::swap(ptr_,  other.ptr_);
        }
    private:
        T* ptr_;
};