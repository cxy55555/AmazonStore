#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include "datastore.h"
#include <iostream>
#include <utility>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "product.h"
#include "user.h"
#include "util.h"
#include "heap.h"

class Mydatastore: public DataStore{
public:
	
	~Mydatastore();
	void addProduct(Product* p);
	void addUser(User* u);
	std::vector<Product*> search(std::vector<std::string>& terms, int type);
	void dump(std::ostream& ofile);
	int addcart(std::string name, size_t index,std::vector<Product*>& hits);
	void viewcart(std::string name);
	void buycart(std::string name);
	std::vector<Product*> dumpcart(std::string name);
	void addReview(const std::string& prodName,
                          int rating,
                          const std::string& username,
                          const std::string& date,
                          const std::string& review_text);
	bool checkuser(std::string username,std::string password);
	int newReview(std::string prodName, int rating,
	std::string username, std::string date, std::string review);
	unsigned long long hashpass(std::string password);
	std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);
	void basic();
	std::map<std::string, double> refined(std::string curr);

private:
	std::map<std::string, Product*> products_;
	std::map<std::string, User*> users_;
	std::map<std::string, std::set<Product*> > keywords_;
	std::map<std::string, std::vector<Product*> > carts;	
	std::vector<std::pair<std::string, User*> > users_vec;
	std::vector<std::vector<std::pair<std::string , double> > > graph;
	std::map<std::string, int> user_index;
};


#endif