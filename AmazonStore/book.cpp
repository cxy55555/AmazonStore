#include <sstream>
#include <iomanip>
#include "book.h"

using namespace std;
Book::Book(const std::string category,
	const std::string name, double price,
		int qty, const std::string ISBN, 
		const std::string author) :
	Product("book", name, price, qty)
{
	ISBN_ = ISBN;
	author_ = author;
	// add all keywords to keywords
	keywords_ = parseStringToWords(this->getName());
	keywords_.insert(ISBN_);
	set<std::string> temp = parseStringToWords(author_);
	for(set<std::string>::iterator it = temp.begin();
		it!= temp.end(); ++it)
	{
		keywords_.insert(*it);
	}
}
Book::~Book()
{

}

std::set<std::string> Book::keywords() const
{
	
	return keywords_;
}

bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
	return true;
}

std::string Book::displayString() const
{
	string display;
	stringstream ss;
	//add all info to ss
	ss <<this->getName()<<"\n";
    ss<< "Author: "<< author_<< " ISBN: "<<ISBN_<<"\n";
	ss<<fixed<<setprecision(2)<< price_<<" ";
	ss<<std::to_string(this->getQty())<<" ";
	ss <<"left.\n";
	ss<<fixed<<setprecision(2)<<"Rating: "<<rating_<<"\n";
	return ss.str();
}

void Book::dump(std::ostream& os) const
{
	//os all info
	os << "book"<< endl;
	os <<this->getName()<< endl;
	os  <<fixed<<setprecision(2)<<this->getPrice()<< endl;
	os << this->getQty()<< endl;
	os << ISBN_<< endl;
	os << author_<< endl;
	return;
}
