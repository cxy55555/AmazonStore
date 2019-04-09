#include <sstream>
#include <iomanip>
#include "movie.h"

using namespace std;
Movie::Movie(const std::string category,
	const std::string name, double price,
		int qty, const std::string genre, 
		const std::string rating) :
	Product("Movie", name, price, qty)
{
	genre_ = genre;
	rating_ = rating;
	keywords_ = parseStringToWords(this->getName());
	set<std::string> temp = parseStringToWords(genre_);
	for(set<std::string>::iterator it = temp.begin();
		it!= temp.end(); ++it)
	{
		keywords_.insert(*it);
	}
}
Movie::~Movie()
{

}

std::set<std::string> Movie::keywords() const
{
	
	return keywords_;
}

bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
	return true;
}

std::string Movie::displayString() const
{
	string display;
	stringstream ss;
	
	ss <<this->getName()<<"\n";
    ss<< "Genre: "<< genre_<< " Rating: "<<rating_<<"\n";
	ss<<fixed<<setprecision(2)<< price_<<" ";
	ss<<std::to_string(this->getQty())<<" ";
	ss <<"left.\n";
	ss<<fixed<<setprecision(2)<<"Rating: "<<this->getRate()<<"\n";
	return ss.str();
}

void Movie::dump(std::ostream& os) const
{
	os << "movie"<< endl;
	os <<this->getName()<< endl;
	os << fixed<<setprecision(2)<<this->getPrice()<< endl;
	os << this->getQty()<< endl;
	os << genre_<< endl;
	os << rating_<< endl;
	return;
}