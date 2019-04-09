#ifndef BOOK_H
#define BOOK_H
#include "product.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "util.h"

class Book : public Product{
public:
	Book(const std::string category, const std::string name, 
		double price,
		int quantity, const std::string ISBN,
		const std::string author);
	~Book();
	//should I use const here?
	std::set<std::string> keywords() const;
	bool isMatch(std::vector<std::string>& searchTerms) const;
	std::string displayString() const;
	void dump(std::ostream& os) const;
private:
	std::string ISBN_;
	std::string author_;
	std::set<std::string> keywords_;
	

};
#endif