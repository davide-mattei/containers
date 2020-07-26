
#include<iostream>
#include<stdexcept>
#include<algorithm>
#include<numeric>

struct error {
    int i;
};

template<class T>
class vector {
    
private:
    int size_;
    T* values_;

public:

    vector(int size = 0, T value = T {}) : size_{size}, values_{new T[size]} {
        std::fill(values_, values_ + size_, value);
    }

    vector(vector const& other) : size_{other.size_}, values_{new T[size_]} { //copy constructor
        std::copy(other.values_, other.values_ + size_, values_);
    }

    explicit vector (vector&& other) noexcept : size_{std::exchange(other.size_, 0)}, values_{std::exchange(other.values_, nullptr)} {} // move constructor
    
    ~vector(){
        delete[] values_;
    }

    vector& operator=(vector const& other) {  //copy assignment - anche void funzia
    if ( this != &other){
        delete [] values_;
        size_ = other.size_;
        values_ = new T[size_];
        std::copy(other.values_, other.values_ + size_, values_);
    }
    return *this;
    }

    vector& operator=(vector const&& other) noexcept { //move assignment
      if (this != &other){
          delete [] values_;
          values_ = std::exchange(other.values_, nullptr); // values_ = other.values_; other.values_ = nullptr;
          size_ = std::exchange(other.size_, 0); //  size_ = other.size_; other.size_ = 0;
      }
      return *this;
    }

    class iterator {

private:
   T* current_it_;
    
public:
  iterator (T* initial_location) : current_it_{initial_location} {}

  T* operator() () {
      return this->current_it_;
  }

  bool operator==(iterator const& other){
      return this->current_it_ == other.current_it_;
  }

  bool operator!=(iterator const& other){
      return ! (*this == other);
      //return this->current_it_ != other.current_it_;
  }

  iterator& operator++() { //only prefix //so it can be modified
      ++current_it_;
      return *this;
  }

  iterator& operator++(int unused) { //only suffix
      ++current_it_;
      return *this;
  }

  iterator& operator+=(int i) {
      current_it_ += i;
      return *this;
  }

  iterator& operator+(int i) {
      return *(this+i);
  }

  iterator& operator-(int i) {
      return *(this-i);
  }

  T& operator* () {
      return *current_it_;
  }

};
    
    T& operator[] (int i){
    if (i<0 || i>= size_){
        throw error{i};
    }
    return *(values_ + i);
    }

    const T&  operator[] (int i) const {
    return *(values_ + i);
}

    void push_back (T const& input_value){
    T* new_values = new T[size_ +1];
    std::copy(values_, values_ + size_, new_values);
    *(new_values + size_) = input_value;
    delete [] values_;
    values_ = new_values;
    new_values = nullptr;
    ++size_;
}

    T& back () {
    return *(values_+size_-1);
}

    int size () const
    {
    return size_;
}
    
    iterator begin() const{ //local objects needs a return-value
    return iterator(values_);
}
    
    iterator end() const{
    return iterator(values_ + size_);
}
    
    int position (iterator it) const { // return the input_position -> starts from 0
    int position = 0;
    for(auto begin = this->begin(); begin != it; ++begin){
        ++position;
    }
    return position;
}

    vector& erase (iterator it){
    if(size_ != 0)
    {
    int new_size = size_ -1;
    T* new_values = new T[new_size];
    std::copy(values_, it(), new_values);
    std::copy(it()+1, this->end()(), new_values + position(it));
    delete [] values_;
    values_ = new_values;
    size_=new_size;
    }
    return *this;
}
    
};

int main(){
    vector<int> v1 (5,1);
    
    std::cout << "v1: ";
    for (auto v : v1){
        std::cout << v << ' ';
    }
    std::cout << '\n';

    vector<int> v2 (std::move(v1));

    std::cout << "v1: ";
    for (auto v : v1){
        std::cout << v << ' ';
    }
    std::cout << '\n';

     std::cout << "v2: ";
    for (auto v : v2){
        std::cout << v << ' ';
    }
    std::cout << '\n';

    v2.push_back(5);
    v2.erase(v2.begin());

    std::cout << "v2: ";
    for (auto v : v2){
        std::cout << v << ' ';
    }
    std::cout << '\n';
}