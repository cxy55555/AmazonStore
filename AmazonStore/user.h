#ifndef USER_H
#define USER_H
#include <iostream>
#include <map>
#include <string>
#include "review.h"

/**
 * Implements User functionality and information storage
 *  You should not need to derive anything from User at this time
 */
class User {
public:
    User();
    User(std::string name, double balance, int type, 
        unsigned long long password);
    virtual ~User();

    double getBalance() const;
    std::string getName() const;
    void deductAmount(double amt);
    virtual void dump(std::ostream& os);
    unsigned long long getPassword() const;
    void setPassword(unsigned long long password);
    void addreview(Review* review, std::string product);
    std::map<std::string, int> getrev();

private:
    std::string name_;
    double balance_;
    int type_;
    unsigned long long password_;
    std::map<std::string, int> reviews;
};
#endif
