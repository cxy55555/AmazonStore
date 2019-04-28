#include <sstream>
#include <iomanip>
#include "clothing.h"

using namespace std;
Clothing::Clothing(const std::string category,
	const std::string name, double price,
		int qty, const std::string size, 
		const std::string brand) :
	Product("clothing", name, price, qty)
{
	size_ = size;
	brand_ = brand;
	keywords_ = parseStringToWords(this->getName());
	set<std::string> temp = parseStringToWords(brand_);
	for(set<std::string>::iterator it = temp.begin();
		it!= temp.end(); ++it)
	{
		keywords_.insert(*it);
	}
}
Clothing::~Clothing()
{

}

std::set<std::string> Clothing::keywords() const
{
		
	return keywords_;
}

bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
	return true;
}

std::string Clothing::displayString() const
{

	string display;
	stringstream ss;
	
	ss <<this->getName()<<"\n";
    ss<< "Size: "<< size_<< " Brand: "<<brand_<<"\n";
	ss<<fixed<<setprecision(2)<< price_<<" ";
	ss<<std::to_string(this->getQty())<<" ";
	ss <<"left.\n";
	ss<<fixed<<setprecision(2)<<"Rating: "<<rating_<<"\n";

	return ss.str();
}

void Clothing::dump(std::ostream& os) const
{
	os << "clothing"<< endl;
	os <<this->getName()<< endl;
	os << fixed<<setprecision(2)<<this->getPrice()<< endl;
	os << this->getQty()<< endl;
	os << size_<< endl;
	os << brand_<< endl;
	return;
}