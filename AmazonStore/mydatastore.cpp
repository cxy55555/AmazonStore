#include "mydatastore.h"
#include "datastore.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include "product.h"
#include "user.h"
#include "util.h"
#include "msort.h"
#include <utility>


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
	users_vec.push_back(make_pair(u->getName(),u));
	graph.push_back(std::vector<std::pair<std::string, double> >());
	user_index.insert(make_pair(u->getName(),users_vec.size()-1));
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
	ofile<<"</reviews>"<<std::endl;

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
	users_[username]->addreview(review, prodName);
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
	std::stringstream ss;
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
	users_[username]->addreview(newreview, prodName);
	products_[prodName]->addreview(newreview);
	return 1;
}

bool Mydatastore::checkuser(std::string username, std::string password)
{
	if(users_.find(username)==users_.end())
	{
		return false;
	}
	else
	{
		unsigned long long pass = hashpass(password);
		if(users_[username]->getPassword()==pass)
		{
			return true;
		}
	}
	return false;
	
}

unsigned long long Mydatastore::hashpass(std::string password)
{
	//convert password to longlong
	unsigned long long sum=0;
	unsigned long long result;
	for (size_t i=0; i< password.size();i++)
	{
		sum =sum*128+password[i];
	}
	
	//get w1 w2 w3 w4
	unsigned int w[4];
	for(size_t i=0; i<4; i++)
	{
		unsigned long long temp=sum;
		for (size_t j=0; j<i;j++)
		{
			temp=temp/65521;
		}
		w[3-i]=temp%65521;
	}
	result=(45912*w[0] + 35511*w[1] + 65169 * w[2] + 4625 * w[3]) % 65521;
	return result;
}

std::vector<std::pair<std::string, double> > Mydatastore::makeSuggestion(std::string currentUser)
{
	std::vector<std::pair<std::string, double> > result;
	if(users_[currentUser]->getrev().size()==products_.size()) return result;
	std::map<std::string,int> reviewd=users_[currentUser]->getrev();
	basic();
	std::map<std::string, double> similarity=refined(currentUser);
	//loop all products
	for(std::map<std::string, Product*>::iterator it=products_.begin();
		it!=products_.end();it++){
		//check if the user already review the product
		if(reviewd.find(it->first)==reviewd.end()&& 
			it->second->getrev().size()!=0)
		{
			std::vector<Review*> revs= it->second->getrev();
			double rp=0;
			double w=0;
			//loop all review to get rating
			for(size_t i=0; i<revs.size();i++){
				int rbp= revs[i]->rating;
				double sba= similarity[revs[i]->username];
				rp=rp+(1-sba)*rbp;
				w=w+(1-sba);
			}
			if(w==0){
				result.push_back(make_pair(it->first, 0));
			}
			else {
				result.push_back(make_pair(it->first, rp/w));
			}
		}
	}
	return result;
}

void Mydatastore::basic()
{
	//loop all pairs
	for(size_t i=0; i<users_vec.size();i++ ){
		for(size_t j=i+1; j<users_vec.size();j++)
		{
			//find intersection
			std::map<std::string,int>  s1=users_vec[i].second->getrev();
			std::map<std::string,int> s2=users_vec[j].second->getrev();
			double avg=0;
			int num=0;
			for( std::map<std::string,int>::iterator it = s1.begin(); 
				it!=s1.end(); ++it)
			{
				if(s2.find(it->first)!= s2.end())
				{
					//calculate avg
					avg=avg+abs((double)it->second -(double)s2[it->first])/4.0;
					num++;
				}
			}
			//add edge to the graph
			if(num!=0){
				avg=avg/num;
				graph[i].push_back(make_pair(users_vec[j].first,avg));
				graph[j].push_back(make_pair(users_vec[i].first, avg));
			}
			else{
				graph[i].push_back(make_pair(users_vec[j].first,1));
				graph[j].push_back(make_pair(users_vec[i].first,1));
			}
		}
	}
}

std::map<std::string, double> Mydatastore::refined(std::string curr)
{
	std::map<std::string, double> similarity;
	Heap<std::string> pq;
	pq.push(0,curr);
	//similarity.insert(make_pair(curr,0));
	for(size_t i=0; i< users_vec.size();i++){
		if(users_vec[i].first!=curr){
			pq.push(std::numeric_limits<double>::infinity(),users_vec[i].first);
			std::pair<std::string,double> p;
			p.first=users_vec[i].first;
			p.second=std::numeric_limits<double>::infinity();
			similarity.insert(p);
		}
	}
	while (!pq.empty())
	{
		std::string temp = pq.top();
		pq.pop();
		int idx= user_index[temp];
		//check if idx right
		for(size_t i=0; i< graph[idx].size();i++){
			double w = graph[idx][i].second;		
			if(similarity[temp]+w < similarity[graph[idx][i].first])
			{
				similarity[graph[idx][i].first]=similarity[temp]+w;
				pq.decreaseKey(similarity[temp]+w, graph[idx][i].first);
			}
		}
	}
	similarity.erase(curr);
	return similarity;
}