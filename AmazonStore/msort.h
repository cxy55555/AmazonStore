#include <iostream>
#include <vector>
#include <string>
#include <sstream>


template <class T, class Comparator>
  void mergeSort (std::vector<T>& myArray, Comparator comp)
  {
  	
  	
  	msort(myArray, 0, myArray.size(), comp);
  }

//recursive sort
template <class T, class Comparator>
  void msort (std::vector<T>& mylist,
  	int start, int end, Comparator comp)
  {
  	if(start+1>=end)
  	{
  		return;
  	}
  	else
  	{
  		
  		int mid = (start+end)/2;  		
  		msort(mylist, start, mid, comp);
  		msort(mylist,  mid, end, comp);
  		merge(mylist,  start, mid, end, comp);
  	}
  }

template <class T, class Comparator>
  void merge (std::vector<T>& mylist, 
  	int start,int mid, int end, Comparator comp)
  {
  	int left=0;
  	int right=0;
  	int whole=0;
  	int ll = mid-start;
  	int lr = end-mid;
  	T arrayl[ll];
  	T arrayr[lr];
    //loop left part
  	for(int i=0; i<ll;i++)
  	{
  		arrayl[i]= mylist[i+start];
  	}
    //loop right part
  	for(int i=0; i<lr; i++)
  	{
  		arrayr[i]=mylist[i+mid];
  	}
    //compare the number in large and small part
  	while(left<ll && right <lr)
  	{
  		if(comp(arrayl[left],arrayr[right]))
  		{
  			mylist[start+whole]=arrayl[left];
  			left++;
  		}
  		else
  		{
  		
  			mylist[start+whole]=arrayr[right];
  			right++;
  		}
  		whole++;
  	}
    //add the left member into list
  	while(left<ll)
  	{
  		mylist[start+whole]=arrayl[left];
  		left++;
  		whole++;
  	}
  	while(right<lr)
  	{
  		mylist[start+whole]=arrayr[right];
  		right++;
  		whole++;
  	}
  	return;
  }
