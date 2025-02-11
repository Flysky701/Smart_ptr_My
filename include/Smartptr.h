#include <utility> 
#include <atomic> 

template <typename T>
class shared_ptr{
    public:
        explicit shared_ptr(T* ptr_ = nullptr) noexcept
            : ptr_(ptr), count_(ptr ? new std::atomic<size_t> (1) : nullptr)
        {}
        
        shared_ptr(const shared_ptr& other) noexcept
            : ptr_(other.ptr_), count_(other.count_)
        {
            if(count_){
                count -> fetch_add(1, std::memor_order_relaxed);
            }
        }

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
                    count_ -> fetch_add(1);
                }
            }
            return *this;
        }
        
        shared_ptr& operator =(const shared_ptr&& other) noexcept{
            if(this != &other){
                release();
                ptr_ = other.ptr_;
                count_ = other.count_;
                other.count_ = nullptr;
                other.ptr_ = nullptr;
            }
        }
        
        T& operator* ()const noexcept{
            return *ptr_;
        }

        T& operator-> ()const noexcept{
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