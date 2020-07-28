#include <iostream>

template <class T> class Container {
public:
  virtual ~Container() = 0;
  virtual void push_back(T const &value) = 0;
};

template <class T> Container<T>::~Container() = default;

template <class T> class vector : public Container<T> {
  class iterator;

private:
  T *data_;
  int sz_;

public:
  ////////////////////////////////////////////////////////////////////////

  vector(int sz = 0, T const &value = T{}) : sz_{sz}, data_{new T[sz]} {
    std::fill(data_, data_ + sz_, value);
  }

  vector(vector const &other) : sz_{other.sz_}, data_{new T[sz_]} {
    std::copy(other.data_, other.data_ + sz_, data_);
  }

  vector(vector &&other) noexcept
      : sz_{std::exchange(other.sz_, 0)}, data_{std::exchange(other.data_,
                                                              nullptr)} {}

  ~vector() override { delete[] data_; }

  vector &operator=(vector const &other) {
    if (this != &other) {
      delete[] data_;
      sz_ = other.sz_;
      data_ = new T[sz_];
      std::copy(other.data_, other.data_ + sz_, data_);
    }
    return *this;
  }

  vector &operator=(vector &&other) {
    if (this != &other) {
      delete[] data_;
      sz_ = std::exchange(other.sz_, 0);
      data_ = std::exchange(other.data_, nullptr);
    }
    return *this;
  }

  ////////////////////////////////////////////////////////////////////////

  int size() const { return sz_; }

  T &operator[](int i) const { return *(data_ + i); }

  T &back() const { return *(data_ + sz_ - 1); }

  T &front() const { return *(data_); }

  void push_back(T const &value) override {
    T *new_data = new T[sz_ + 1];
    std::copy(data_, data_ + sz_, new_data);
    delete[] data_;
    data_ = std::exchange(new_data, nullptr);
    *(data_ + sz_) = value;
    ++sz_;
  }

  void pop_back() {
    int new_size = sz_ - 1;
    T *new_data = new T[new_size];
    std::copy(data_, data_ + new_size, new_data);
    delete[] data_;
    data_ = std::exchange(new_data, nullptr);
    sz_ = new_size;
  }

  int position(iterator const &it) const {
    int pos = 0;
    for (auto begin = this->begin(); begin != it; ++begin) {
      ++pos;
    }
    return pos;
  }

  iterator begin() const { return iterator(data_); }

  iterator end() const { return iterator(data_ + sz_); }

  void erase(iterator &it) {
    if (sz_ != 0) {
      int new_sz = sz_ - 1;
      T *new_data = new T[new_sz];
      std::copy(data_, it(), new_data);
      std::copy(it() + 1, this->end()(), new_data + position(it));
      delete[] data_;
      data_ = std::exchange(new_data, nullptr);
      sz_ = new_sz;
    }
  }

  void erase(iterator &b, iterator &e) {
    int new_sz = sz_ - this->position(e) + this->position(b);
    T *new_data = new T[new_sz];
    if (new_sz != 0) {
      std::copy(this->begin()(), b(), new_data);
      if (e != this->end())
        std::copy(e() + 1, this->end()(), new_data + position(b) + 1);
    }
    delete[] data_;
    data_ = std::exchange(new_data, nullptr);
    sz_ = new_sz;
  }
};

template <class T> class vector<T>::iterator {
private:
  T *current_;

public:
  iterator(T *current) : current_{current} {}
  ~iterator() = default;
  bool operator==(iterator const &other) {
    return this->current_ == other.current_;
  }
  bool operator!=(iterator const &other) { return !(*this == other); }

  iterator &operator++() {
    ++current_;
    return *this;
  }
  iterator &operator--() {
    --current_;
    return *this;
  }
  iterator &operator++(int i) {
    ++current_;
    return *this;
  }
  iterator &operator--(int i) {
    --current_;
    return *this;
  }
  iterator &operator+=(int i) {
    current_ += i;
    return *this;
  }
  iterator &operator-=(int i) {
    current_ -= i;
    return *this;
  }

  T *operator()() const { return this->current_; }
  T &operator*() { return *current_; }
};

template <class T> class list : public Container<T> {
  struct Node;
  class iterator;

private:
  Node *head_;
  Node *tail_;

public:
  list() {
    head_ = new Node;
    tail_ = new Node;
    head_->next_ = nullptr;
    head_->prev_ = tail_;
    tail_->prev_ = nullptr;
    tail_->next_ = head_;
  }

  ~list() override {
    Node *current = nullptr;
    while (tail_->next_ != head_) {
      current = tail_->next_->next_;
      delete tail_->next_;
      tail_->next_ = current;
      current = nullptr;
    }
    delete tail_;
    delete head_;
  }

  list(list &&other) noexcept {
    head_ = new Node;
    tail_ = new Node;
    tail_->prev_ = nullptr;
    head_->next_ = nullptr;
    head_->prev_ = tail_;
    tail_->next_ = head_;

    Node *other_current = other.tail_->next_;
    while (other_current != other.head_) {
      Node *new_data = new Node;
      new_data->data_ = other_current->data_;
      new_data->next_ = head_;
      new_data->prev_ = head_->prev_;
      head_->prev_->next_ = new_data;
      head_->prev_ = new_data;

      Node *temp = other_current->next_;
      delete other_current;
      other_current = temp;
      temp = nullptr;
    }
    other.tail_->prev_ = nullptr;
    other.tail_->next_ = other.head_;
    other.head_->next_ = nullptr;
    other.head_->prev_ = other.tail_;
  }

  list &operator=(list const &other) {
    if (this->tail_ != other.tail_) {
      while (this->tail_->next_ != this->head_) {
        Node *temp = this->tail_->next_->next_;
        delete this->tail_->next_;
        this->tail_->next_ = temp;
      }
      this->head_->prev_ = tail_;
      this->tail_->next_ = head_;

      Node *other_current = other.tail_->next_;

      while (other_current != other.head_) {
        this->push_back(other_current->data_);
        other_current = other_current->next_;
      }
    }
    return *this;
  }

  list &operator=(list &&other) noexcept {
    // emptying this
    if (this->tail_ != other.tail_) {
      while (this->tail_->next_ != this->head_) {
        Node *temp = this->tail_->next_->next_;
        delete this->tail_->next_;
        this->tail_->next_ = temp;
      }
      this->head_->prev_ = tail_;
      this->tail_->next_ = head_;
      // filling this and emptying other
      Node *other_current = other.tail_->next_;
      while (other_current != other.head_) {
        this->push_back(other_current->data_);

        Node *temp = other_current->next_;
        delete other_current;
        other_current = temp;
        temp = nullptr;
      }
      other.tail_->prev_ = nullptr;
      other.tail_->next_ = other.head_;
      other.head_->next_ = nullptr;
      other.head_->prev_ = other.tail_;
    }
    return *this;
  }

  void push_back(T const &data) override {
    Node *new_ = new Node;
    new_->data_ = data;
    new_->next_ = head_;
    new_->prev_ = head_->prev_;
    head_->prev_->next_ = new_;
    head_->prev_ = new_;
  }

  void push_front(T const &data) {
    Node *new_ = new Node;
    new_->data_ = data;
    new_->prev_ = tail_;
    new_->next_ = tail_->next_;
    tail_->next_->next_ = new_;
    tail_->next_ = new_;
  }

  iterator begin() const { return iterator(tail_->next_); }
  iterator end() const { return iterator(head_); }

  int position(iterator const &it) const {
    int position = 0;
    auto begin = iterator(tail_->next_);
    for (; begin != it; ++begin) {
      ++position;
    }
    return position;
  }

  iterator insert(iterator &it, T const &data) {
    Node *new_ = new Node;
    new_->data_ = data;
    new_->next_ = it();
    new_->prev_ = it()->prev_;
    it()->prev_->next_ = new_;
    it()->prev_ = new_;
    return it;
  }
};

template <class T> struct list<T>::Node {
  T data_;
  Node *next_;
  Node *prev_;
};

template <class T> class list<T>::iterator {
private:
  Node *current_;

public:
  iterator(Node *current) : current_{current} {}
  bool operator==(iterator const &other) const {
    return this->current_ == other.current_;
  }
  bool operator!=(iterator const &other) const { return !(*this == other); }

  iterator &operator++() {
    this->current_ = current_->next_;
    return *this;
  }
  iterator &operator++(int unused) {
    this->current_ = current_->next_;
    return *this;
  }
  iterator &operator--() {
    this->current_ = current_->prev_;
    return *this;
  }
  iterator &operator--(int unused) {
    this->current_ = current_->prev_;
    return *this;
  }

  T &operator*() { return current_->data_; }

  Node *operator()() { return current_; }
};

int main() {

}