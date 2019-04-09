#include <sstream>
#include <iomanip>
#include "product.h"

using namespace std;

Product::Product(const std::string category, const std::string name, double price, 
    int qty) :
    name_(name),
    price_(price),
    qty_(qty),
    category_(category)
{
    rating_=0;
}

Product::~Product()
{

}


double Product::getPrice() const
{
    return price_;
}

std::string Product::getName() const
{
    return name_;
}

void Product::subtractQty(int num)
{
    qty_ -= num;
}

int Product::getQty() const
{
    return qty_;
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Product::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Product::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
}

void Product::deletereview()
{
    for (size_t i =0; i< reviews.size();++i)
    {
        delete reviews[i];
    }
}

void Product::addreview(Review* review)
{
    reviews.push_back(review);
    rating_=(rating_+review->rating)/reviews.size();
}

double Product::getRate() const
{
    return rating_;
}

void Product::dumpreview(std::ostream& os) const
{
    if(reviews.size()==0)
    {
        return;
    }
    //dump each review
    for(size_t i=0; i< reviews.size();i++)
    {
        os<<name_<< endl;
        os<<reviews[i]->rating<<" "<<reviews[i]->username;
        os<<" "<<reviews[i]->date<<" "<<reviews[i]->reviewText<<endl;
    }
}

std::vector<Review*> Product::getrev() 
{
    return reviews;
}


