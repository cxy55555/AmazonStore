#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);
    void heapify(int idx);
  	void trikleUp(int loc);

private:
    /// Add whatever helper functions you need below




    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trikleUp(int loc)
{
  int parent=(loc-1)/m_;
  //going up to find the location for the node
  while(parent>=0 && c_(store_[loc].first,store_[parent].first))
  {
  	//swap content in heap
    std::pair<double, T> temp = store_[loc];
    store_[loc]=store_[parent];
    store_[parent]=temp;
    //update idx
    keyToLocation_[store_[loc].second]= loc;
    keyToLocation_[store_[parent].second]=parent;
    loc = parent;
    parent=(loc-1)/m_;
  }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    store_.push_back(make_pair(priority,item));
    keyToLocation_.insert(make_pair(item, store_.size()-1));
  	trikleUp(store_.size()-1);

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    if (keyToLocation_.find(item) == keyToLocation_.end()) {
        return;
    }
    //update priorty
    store_[keyToLocation_[item]].first=priority;
    //trickle up to find location
    trikleUp(keyToLocation_[item]);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }

    // You complete
    return store_[0].second;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(int idx)
{
  //recurse down to find the location for the node
  //check if it is leaf node
  if((idx * m_+1) > ((int)store_.size()) - 1)
  {
    return;
  }
  int smaller=idx*m_+1;
  int rChild;
  //find the right child
  if(smaller +m_<=(int) store_.size())
  {
    rChild = smaller+m_-1;
  }
  else
  {
    rChild = store_.size()-1;
  }
  //loop all the child from left to right
  for(int i=smaller+1; i <= rChild;i++)
  {
    if(c_(store_[i].first,store_[smaller].first))
    {
      smaller = i;
    }
  }
  //compare the smallest child with the parent
  if(c_(store_[smaller].first,store_[idx].first))
  {
    //update pair
    std::pair<double, T> temp = store_[smaller];
    store_[smaller]=store_[idx];
    store_[idx]=temp;
    //update map
    keyToLocation_[store_[idx].second]= idx;
    keyToLocation_[store_[smaller].second]=smaller;   
    heapify(smaller);
  }
  
}
/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    keyToLocation_.erase(store_[0].second);
    store_[0]=store_.back();
    keyToLocation_[store_[0].second]=0;
  	store_.pop_back();
  	heapify(0);

}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}


#endif

