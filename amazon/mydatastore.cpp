#include "mydatastore.h"
#include "datastore.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "product.h"
#include "user.h"
#include "util.h"
#include "msort.h"


Mydatastore::~Mydatastore()
{
	for (std::map<std::string, Product*>::iterator it = products_.begin();
	 it != products_.end(); it++){
		it->second->deletereview();
		delete it->second;
	}

	for (std::map<std::string, User*>::iterator it = users_.begin();
	 it != users_.end(); it++){
		
		delete it->second;
	}
	
}

void Mydatastore::addProduct(Product* p)
{

	products_.insert(make_pair(p->getName(), p));	
	
	//loop keywords of p
	std::set<std::string> temp = p->keywords();

	for(std::set<std::string>::iterator it = temp.begin();
		it!=temp.end();it++)
	{
		
		//if p's keyword is not in keywords map
		//construct a product vector
		//add keyword to map
		if(keywords_.find(*it)== keywords_.end())
		{
			
			std::set<Product*> products;
			products.insert(p);
			keywords_.insert(make_pair(*it, products));
		}
		//if keyword in map
		//add new product
		else
		{
			
			keywords_[*it].insert(p);
		}
	}
}

void Mydatastore::addUser(User* u)
{
	users_.insert(make_pair(u->getName(), u));
	
}

std::vector<Product*> Mydatastore::search(std::vector<std::string>& terms, int type)
{

	std::vector<std::string> validKeyword;
	std::vector<Product*> result;
	if(terms.size()>0)
	{	
		//loop to find keyword we already have in map
		
		for(size_t i=0; i<terms.size();++i)
		{
			if(keywords_.find(terms[i])!=keywords_.end())
			{
				
				validKeyword.push_back(terms[i]);
			}
		}
		//check if size = 0;
		if(validKeyword.size()==0)
		{
			return result;
		}	
		//and serach
		if(type==0)
		{	
			if(validKeyword.size()!=terms.size())
			{
				return result;
			}		
			std::set<Product*> intersect = keywords_[validKeyword[0]];
			//loop through valid keyword to find intersection		
			for(size_t i=1; i< validKeyword.size();++i)
			{
				intersect = setIntersection(intersect, keywords_[validKeyword[i]]);
			}		
			//push all in vector to set
			for(std::set<Product*>::iterator it=intersect.begin();
			 it!= intersect.end(); ++it)
			{
				result.push_back(*it);
			}
		}
		//or search
		else if(type ==1)
		{
			
			std::set<Product*> unionSet = keywords_[validKeyword[0]];
			//loop through valid keyword to find union
		
			for(size_t i=1; i< validKeyword.size();++i)
			{
		
				unionSet = setUnion(unionSet, keywords_[validKeyword[i]]);
			}
			//push all in vector to set	
			for(std::set<Product*>::iterator it=unionSet.begin();
			 it!= unionSet.end(); ++it)
			{
				result.push_back(*it);
			}
		}
	}
	
	return result;
}

void Mydatastore::dump(std::ostream& ofile)
{
	//output products
	ofile<<"<products>"<<std::endl;
	for (std::map<std::string, Product*>::iterator it = products_.begin();
	 it != products_.end(); it++){
		it->second->dump(ofile);
	}
	ofile<<"</products>"<< std::endl;
	//output users
	ofile<<"<users>"<< std::endl;
	for (std::map<std::string, User*>::iterator it = users_.begin();
	 it != users_.end(); it++){
		it->second->dump(ofile);
	}
	ofile<<"</users>"<< std::endl;
	ofile<<"<reviews>"<<std::endl;
	for (std::map<std::string, Product*>::iterator it = products_.begin();
	 it != products_.end(); it++){
		it->second->dumpreview(ofile);
	}
	ofile<<"</review>"<<std::endl;

}

int Mydatastore::addcart(std::string name, size_t index,std::vector<Product*>& hits)
{

	//check if index is valid
	if(index-1>=hits.size())
	{
		return -1;
	}
	//if user is not in cart yet
	if(carts.find(name)==carts.end())
	{
		std::vector<Product*> buythings;
		buythings.push_back(hits[index-1]);
		carts.insert(make_pair(name, buythings));
	}
	else
	{		
		carts[name].push_back(hits[index-1]);
	}
	return 1;
}

void Mydatastore::viewcart(std::string name)
{		
	//loop through all iterm in cart	
	for(size_t i=0; i< carts[name].size();++i)
	{	
	    std::cout <<"Item "<< i+1<< "\n";		
		std::string temp =carts[name][i]->displayString();
		std::cout << temp<<"\n";
	}		
}

void Mydatastore::buycart(std::string name)
{
		
	//loop each product in cart
	std::vector<Product*> cart=carts[name];
	size_t sizet = carts[name].size();
	int num =0;
	for(size_t i=0;i<sizet;i++)
	{
		
		//check if it is in stock
		Product* buything = carts[name][i];
		if(buything->getQty()>0)
		{
			
			//check if money is enough

			if(users_[name]->getBalance() >= buything->getPrice())
			{
				
				//qty -1
				buything->subtractQty(1);
				//bebit deduction
				users_[name]->deductAmount(buything->getPrice());
				//remove from cart
				cart.erase(cart.begin()+i-num);
				num++;
			}

		}
		
		
	}
	carts[name]=cart;
}

std::vector<Product*> Mydatastore::dumpcart(std::string name)
{
	return carts[name];
}

void Mydatastore::addReview(const std::string& prodName,
                          int rating,
                          const std::string& username,
                          const std::string& date,
                          const std::string& review_text)
{
	//check if their is such product in our product list
	if(products_.find(prodName)==products_.end())
	{
		return;
	}
	//check if user in user list
	if(users_.find(username)==users_.end())
	{
		return;
	}
	Review* review = new Review(rating, username,date, review_text);
	products_[prodName]->addreview(review);

}

int Mydatastore::newReview(std::string prodName, int rating,
	std::string username, std::string date, std::string review)
{
	std::string temp = date;
	if (temp.size()!=10)
	{
		return -1;
	}
	//wrong rating
	if(rating<0 ||rating >5)
	{
		return 0;
	}
	//malformed date
	for (size_t i=0; i< 10;i++)
	{
		if(i==4||i==7)
		{
			if(temp[i]!=45)
			{
				return -1;
			}
			else
			{
				temp[i]=32;
			}
		}
		else
		{
			if(temp[i]>57||temp[i]<48)
			{
				return -1;
			}
		}
	}
	int year, mm, dd;
	stringstream ss;
	ss<<temp;
	ss>>year>>mm>>dd;
	//feb
	if(mm==2)
	{
	
		if(dd>=28||dd<1)
		{
			return -1;
		}
	}
    //31days
	else if(mm==1||mm ==3||mm==5||mm==7||mm==8
		||mm==10||mm==12)
	{	
		if(dd>31||dd==0)
		{
			return -1;
		}
	}
	//30days
	else if(mm==4||mm==6||mm==9||mm==11)
	{
		if(dd>30||dd==0)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	Review* newreview = new Review(rating, username,date, review);

	products_[prodName]->addreview(newreview);
	return 1;
}

bool Mydatastore::checkuser(std::string username)
{
	if(users_.find(username)==users_.end())
	{
		return false;
	}
	else
	{
		return true;
	}
	
}