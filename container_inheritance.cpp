#include<iostream>

template<class T> class Container {
public:
    virtual ~Container() = 0;
    virtual Container& operator=(Container const& other) = 0;
    virtual Container& operator=(Container&& other) = 0;
    virtual void push_back (T const& value) = 0;
};

template<class T> Container<T>::~Container ()= default;

template<class T> class vector : public Container<T> 
{
private:
    T* data_;
    int sz_;
public:
    vector(int sz = 0, T const& value = T {}) : sz_{sz}, data_{new T[sz]} {
        std::fill(data_, data_ + sz, value);
    }
    
    vector(vector&& other) noexcept : Container<T>(other.sz_), sz_{std::exchange(other.sz_, 0)}, data_{std::exchange(other.data_, nullptr)} {}
    
    ~vector() override {
        delete[] data_;
    }
    
    vector& operator=(vector const& other) override {
        if (this != &other)
        {
            sz_ = other.sz_;
            data_ = new T[sz_];
            std::copy(other.data_, other.data_ + sz_, data_);
        }
        return *this;
    }
    
    vector& operator=(vector&& other) override {
        if (this != &other)
        {
            delete[] data_;
            sz_ = std::exchange(other.sz_,0);
            data_ = std::exchange(other.data_, nullptr);
        }
        return *this;
    }
    
    void push_back (T const& value) override {
        T* new_data = new T[sz_+1];
        std::copy(data_, data_+sz_, new_data);
        delete[] data_;
        *(new_data+sz_) = value;
        data_ = std::exchange(new_data, nullptr);
        ++sz_;
    }

    T& operator[] (int i) const {
        return *(data_+i);
    }
};

int main (){

    vector<int> v1(1);
    v1.push_back(1);
    v1.push_back(2);

    std::cout << v1[0]
}