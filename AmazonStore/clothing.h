#ifndef CLOTHING_H
#define CLOTHING_H
#include "product.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "util.h"

class Clothing : public Product{
public:
	Clothing(const std::string category,
		const std::string name, double price,
		int quantity, const std::string size,
		const std::string brand);
	~Clothing();
	//should I use const here?
	std::set<std::string> keywords() const;
	bool isMatch(std::vector<std::string>& searchTerms) const;
	std::string displayString() const;
	void dump(std::ostream& os) const;
private:
	std::string size_;
	std::string brand_;
	std::set<std::string> keywords_;
};
#endif