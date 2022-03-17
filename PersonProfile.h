//
//  PersonProfile.hpp
//  CS32_Unhinged
//
//  Created by Venky on 3/4/22.
//

#ifndef PersonProfile_h
#define PersonProfile_h

#include "provided.h"
#include "RadixTree.h"
#include <string>
#include <vector>
#include <unordered_set>

class PersonProfile
{
  public:
    PersonProfile(std::string name, std::string email);
    ~PersonProfile();
    std::string GetName() const;
    std::string GetEmail() const;
    void AddAttValPair(const AttValPair& attVal);
    int GetNumAttValPairs() const;
    bool GetAttVal(int attribute_num, AttValPair& attVal) const;
    
  private:
    std::string userName;
    std::string userEmail;
    RadixTree<std::unordered_set<std::string>> attributesToValue;
    std::vector<AttValPair> vectorAttValPair;
    int numDistinctPairs;
    
};

#endif /* PersonProfile_h */
