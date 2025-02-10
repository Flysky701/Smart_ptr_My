#include <utility> 
#include <atomic> 

template <typename T>
class my_smart_ptr{
    public:
        shared_ptr(T* ptr_ = nullptr);
        explicit shared_ptr()
    
    private:
        T* ptr_;
        std::atomic<size_t>* count_;

        void relese() noexcept{
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