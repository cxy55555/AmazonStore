#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <cctype>
#include <sstream>

using namespace std;


struct Comp2 {
  bool operator()(std::string a, std::string b)
  {
    // Complete the code
    return a>b;
    
  }
};
struct Comp3 {
  bool operator()(std::string a, std::string b)
  {
    // Complete the code
    //convert large to small case
    for (size_t i=0; i< a.size();i++)
    {
    	if(a[i]<97)
    	{
    		a[i]=a[i]+32;
    	}
    }
    for (size_t i=0; i< b.size();i++)
    {
    	if(b[i]<97)
    	{
    		b[i]=b[i]+32;
    	}
    }
    return a<b;
    
  }
};
struct Comp4 {
  bool operator()(std::string a, std::string b)
  {
    // Complete the code
    //use stringstream to get rid of tab
    stringstream ssa;
    ssa<<a;
    string tempa;
    ssa>>tempa;
    stringstream ssb;
    ssb<<b;
    string tempb;
    ssb>>tempb;
    return tempa<tempb;

  }
};

void outputWords(std::ostream& ostr, const std::vector<std::string>& words)
{
  for(unsigned i = 0; i < words.size()-1; i++){
    ostr << words[i] << ":";
  }
  if(words.size() > 0){
    ostr << words.back();
  }
  ostr << endl;
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    cout << "Provide an output file name." << endl;
    return 1;
  }
  ofstream ofile(argv[1]);
  if(ofile.fail()) {
    cout << "Cannot open file for writing." << endl;
    return 1;
  }
  vector<string> mywords =
    { "abc", "Acc", "aBc", "Zxy", "zyx", "  efg" };

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z).  This should still be case sensitive.
  vector<string> order1 = mywords;
  std::sort(order1.begin(), order1.end());
  outputWords(ofile, order1);

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order but descending from largest to smallest
  // (i.e. z to a).  This should still be case sensitive
  vector<string> order2 = mywords;
  Comp2 c2;
  std::sort(order2.begin(), order2.end(), c2);
  outputWords(ofile, order2);


  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z) but case INSENSITIVE (i.e. abc < Abd)
  vector<string> order3 = mywords;
  Comp3 c3;
  std::sort(order3.begin(), order3.end(), c3);
  outputWords(ofile, order3);

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z), case sensitive and ignoring
  // spaces (only ' ') before the word.
  vector<string> order4 = mywords;
  Comp4 c4;
  std::sort(order4.begin(), order4.end(), c4);
  outputWords(ofile, order4);

  ofile.close();
  return 0;
}


