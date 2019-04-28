#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"
#include "msort.h"
#include "review.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());

    }
};
struct Compr
{
    bool operator()(Product* lhs, Product* rhs)
    {
        return lhs->getRate() > rhs->getRate();
    }
};
struct Compt
{
	bool operator()(Review* lhs, Review* rhs)
	{
		return lhs->date > rhs->date;
	}
};

struct CompRev{
    bool operator()(pair<string,double> p1, pair<string,double> p2)
    {
        if(p1.second!=p2.second){
            return p1.second>p2.second;
        }
        return p1.first<p2.first;
    }
};

void displayProducts(vector<Product*>& hits);
void displayReview(vector<Review*>& rev);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    //DataStore ds;
    Mydatastore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  LOGIN username password            " << endl;
    cout << "  LOGOUT                             " << endl;
    cout << "  AND r/n term term ...              " << endl;
    cout << "  OR r/n term term ...               " << endl;
    cout << "  ADD search_hit_number              " << endl;
    cout << "  VIEWCART                           " << endl;
    cout << "  BUYCART                            " << endl;
    cout << "  ADDREV seach_hit_number rating date review_text" << endl;
    cout << "  VIEWREV seach_hit_number           " << endl;
    cout << "  REC                                " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    string username="";
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                if(ss >>cmd){
                    if(cmd=="r" ||cmd =="n")
                    {
                        string term;
                        vector<string> terms;
                        while(ss >> term) {
                            term = convToLower(term);
                            terms.push_back(term);
                        }
                        hits = ds.search(terms, 0);
                        if(cmd=="r")
                        {

                            Compr compr;
                            mergeSort(hits, compr);
                        }
                        else if(cmd=="n")
                        {
                            ProdNameSorter compn;
                            mergeSort(hits, compn);
                        }
                        displayProducts(hits);
                    }
                }                      
            }
            else if ( cmd == "OR" ) {
                if(ss >>cmd){
                    if(cmd=="r" ||cmd =="n")
                    {
                        string term;
                        vector<string> terms;
                        while(ss >> term) {
                            term = convToLower(term);
                            terms.push_back(term);
                        }
                        hits = ds.search(terms, 1);
                        if(cmd=="r")
                        {
                            Compr compr;
                            mergeSort(hits, compr);
                        }
                        else if(cmd=="n")
                        {
                            ProdNameSorter compn;
                            mergeSort(hits, compn);
                        }
                        displayProducts(hits);
                    }
                }               
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            //if add command
            else if(cmd=="REC")
            {
                string name;
                if(username=="")
                {
                    cout<<"No current user"<<endl;
                }   
                else{            
                    vector<std::pair<std::string, double> > result;
                    result=ds.makeSuggestion(username);
                    stable_sort(result.begin(),result.end(),CompRev());
                    if(result.size()==0) {
                        cout<<"No recommendations available"<<endl;
                    }
                    else{
                        ofstream ofile("rec.txt");
                        cout<<username<<endl;
                        ofile<<username<<endl;
                        for(size_t i=0; i< result.size();i++){
                            cout<<fixed<<setprecision(2)<<result[i].second;
                            cout<<" "<<result[i].first<<endl;
                            ofile<<fixed<<setprecision(2)<<result[i].second;
                            ofile<<" "<<result[i].first<<endl;
                        }
                        ofile.close();
                    }
                }
            }
            else if(cmd=="ADDREV")
            {
            	size_t num;
            	int result; 
                //check if user login
            	if(username=="")
            	{
            		cout <<"No current user"<< endl;
            	}
            	else
            	{
	            	if(ss>>num)
	            	{
	            		int rating;
	            		if(ss>>rating && num <=hits.size())
	            		{
		            		string date;
		            		if(ss>>date)
		            		{
		            			string review;
		            			getline(ss,review);		
		            			result=ds.newReview(hits[num-1]->getName(),
		            			rating,username, date, review);		            			
		            		}
	            		}
	            	}
	            	else cout << "Invalid request"<< endl;
                    if(result!=1) cout <<"invalid request"<<endl;
            	}           	
            }
            else if(cmd=="VIEWREV")
            {
            	size_t index;
            	
            		if(ss>>index&& index<=hits.size())
            		{

            			Compt compt;
            			vector<Review*> rev = hits[index-1]->getrev();
                        //check if there is review
            			if(rev.size()>0)
            			{
            				mergeSort(rev, compt);
            				displayReview(rev);
            			}         			          			
            		}
            		else cout<<"Invalid request"<< endl;
            	
            }
            else if(cmd=="LOGIN")
            {
                string temp;
                string password;
                if(ss>>temp)
                {
                    if(ss>>password)
                    {
                        if(ds.checkuser(temp,password))
                        {
                            username=temp;
                            cout<<"User logged in"<<endl;
                        }
                        else
                        {
                            cout <<"Invalid login credentials"<< endl;
                        }
                    }
                    else cout <<"Invalid login credentials"<< endl;
                }
                else cout <<"Invalid login credentials"<< endl;
            }
            else if(cmd=="LOGOUT")
            {
                username="";
            }
            else if(cmd=="ADD")
            {
                string name;
                size_t index;
                int result;                
            	//name invalid
                if(username=="")
                {
                    cout <<"No current user"<< endl;
                }
                else
                {                       
                    ss>>index;
                    //index invalid
                    if(ss.fail())
                    {
                        cout<<"Invalid request"<< endl;
                    }
                    else
                    {
                       //get resut from carts
                        result = ds.addcart(username,index,hits);
                        //if invalid name or index
                        if(result==-1)
                        {
                            cout <<"Invalid request"<< endl;
                        }
                    }
                }                
            }
            else if(cmd=="VIEWCART")
            {                
                //name invalid                
                if(username=="") cout << "No current user"<< endl; 
                else ds.viewcart(username);               
            }
            else if(cmd=="BUYCART")
            {               
                //name invalid                               
                if(username=="")
                {
                    cout <<"No current user"<< endl;
                }  
                else {ds.buycart(username);}                                  
            }
            else {cout << "Unknown command" << endl;}
        }
    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}

void displayReview(vector<Review*>& rev)
{
	for(size_t i=0;i<rev.size();i++)
	{
		rev[i]->display();
	}
	return;
}


