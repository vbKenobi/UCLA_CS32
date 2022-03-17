//
//  PersonProfile.cpp
//  CS32_Unhinged
//
//  Created by Venky on 3/4/22.
//

#include "PersonProfile.h"
#include <string>
#include <unordered_set>

PersonProfile::PersonProfile(std::string name, std::string email)
{
    userName = name;
    userEmail = email;
    numDistinctPairs = 0;
}

PersonProfile::~PersonProfile()
{
    
}

std::string PersonProfile::GetName() const
{
    return userName;
}

std::string PersonProfile::GetEmail() const
{
    return userEmail;
}

void PersonProfile::AddAttValPair(const AttValPair& attVal)
{
    std::unordered_set<std::string>* values = attributesToValue.search(attVal.attribute);
    
    //If the attribute was never found
    if(values == nullptr)
    {
        //Add the attribute to the RadixTree
        std::unordered_set<std::string> tempVals;
        tempVals.insert(attVal.value);
        attributesToValue.insert(attVal.attribute, tempVals);
        numDistinctPairs++;
        vectorAttValPair.push_back(attVal);
        return;
    }
    
    //If we do find the value in the corresponding attribute return;
    if(values->find(attVal.value) != values->end())
    {
        return;
    }
    
    //Otherwise insert the new value
    numDistinctPairs++;
    values->insert(attVal.value);
    vectorAttValPair.push_back(attVal);
}

int PersonProfile::GetNumAttValPairs() const
{
    return numDistinctPairs;
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attVal) const
{
    if(attribute_num < 0 || attribute_num >= GetNumAttValPairs())
        return false;
    
    attVal = vectorAttValPair[attribute_num];
    return true;
}

