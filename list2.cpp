#include <iostream>

template <class T> class list {

    class Node;
    class iterator;

private:
    
  Node* head_; //
  Node* tail_; //

public:
  list() {
    head_ = new Node;
    tail_ = new Node;
    head_->next_ = nullptr;
    head_->prev_ = tail_;
    tail_->next_ = head_;
    tail_->prev_ = nullptr;

  }

  ~list()
  {
    while (head_->prev_ == tail_) {
      Node* it = head_->prev_->prev_;
      delete head_->prev_;
      head_->prev_ = it;
    }
    delete head_;
    delete tail_;
  }

  void push_back(T const &value) 
  {
    Node* N = new Node;
    N->data_ = value;
    N->next_ = head_;
    N->prev_ = head_->prev_;
    head_->prev_->next_ = N;
    head_->prev_ = N;
  }

  void push_front(T const &value)
  {
    Node* N = new Node;
    N->data_ = value;
    N->next_ = tail_->next_;
    N->prev_ = tail_;
    tail_->next_->prev_ = N;
    tail_->next_ = N;
  }

  iterator begin() const {
      return iterator(tail_->next_);
  }
  
  iterator end() const {
      return iterator(head_);
  }

  iterator insert(iterator it, T const& value) {
      Node* N = new Node;
      N -> data_ = value;
      N -> next_ = it();
      N -> prev_ = it() ->prev_;
      /*it.current_node_*/ it() -> prev_ -> next_ = N;
      /*it.current_node_*/ it() -> prev_ = N;
      return it;
  }

  int position (iterator const& other) {
      int position = 0;
      for (auto beg = this->begin(); beg != other; ++beg){
          position++;
      }
      return position;
  }

};

template<class T> 
struct list<T>::Node {
    T data_;
    Node *prev_;
    Node *next_;
};

  template<class T> 
class list<T>::iterator {
    private:
    Node* current_node_;
    public:
    iterator(Node* current) : current_node_{current} {}

    iterator& operator++(){
        current_node_ = current_node_-> next_;
        return *this;
    }

    iterator& operator++(int n){
        current_node_ = current_node_-> next_;
        return *this;
    }

    iterator& operator--(){
        current_node_ = current_node_-> prev_;
        return *this;
    }

    T& operator*(){
        return current_node_-> data_;
    }

    bool operator==(iterator const& other) {
        return this->current_node_ == other.current_node_;
    }
    bool operator!=(iterator const& other) {
        return !(*this == other);
    }

    Node* operator() () { return this->current_node_; }



  };



int main() {
  list<int> lista{};
  lista.push_back(3);
  lista.push_front(4);

  auto it = lista.begin();
  lista.insert(it, 5);
  //std::cout << *it << '\n';
  it = lista.begin();
  for (auto end = lista.end(); it != end; ++it)
    std::cout << *it << ' ';

  std::cout << '\n';
  
}