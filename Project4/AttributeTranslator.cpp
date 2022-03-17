//
//  AttributeTranslator.cpp
//  Unhinged_App
//
//  Created by Venky on 3/6/22.
//

#include "AttributeTranslator.h"
#include <string>
#include <fstream>
#include <vector>
#include <unordered_set>

AttributeTranslator::AttributeTranslator()
{
    
}

AttributeTranslator::~AttributeTranslator()
{
    for(int i = 0; i < vectorOfAttributes.size(); i++)
    {
        std::vector<AttValPair*>* pointToVector = relatedAttributes.search(vectorOfAttributes[i]);
        
        if(pointToVector != nullptr)
        {
            for(int j = 0; j < pointToVector->size(); j++)
            {
                AttValPair* temp = pointToVector->at(j);
                pointToVector->at(j) = nullptr;
                delete temp;
            }
        }
    }
}

bool AttributeTranslator::Load(std::string fileName)
{
    std::ifstream myfile;
    myfile.open(fileName);
    std::string pairs;
    
    if(myfile.is_open())
    {
        while(myfile)
        {
            std::getline(myfile, pairs);
            
            if(pairs == "\n")
                continue;
            
             
            //Get the attribute and value pairs
            std::string delimiter = ",";
            std::string att1 = pairs.substr(0, pairs.find(delimiter));
            std::string val1 = pairs.substr(pairs.find(delimiter)+1, pairs.length());
            std::string att2 = val1.substr(val1.find(delimiter)+1, val1.length());
            val1 = val1.substr(0, val1.find(delimiter));
            std::string val2 = att2.substr(att2.find(delimiter)+1, att2.length());
            att2 = att2.substr(0, att2.find(delimiter));

            std::string correlate = att1+","+val1;
            vectorOfAttributes.push_back(correlate);
            //std::unordered_set<AttValPair*>* pointToSet = relatedAttributesSet.search(correlate);
            std::vector<AttValPair*>* pointToCorrelation = relatedAttributes.search(correlate);
            AttValPair* newPair = new AttValPair(att2, val2);
            
            //Add the values to the tree
            if(pointToCorrelation == nullptr)
            {
                //std::unordered_set<AttValPair*> temp;
                //temp.insert(newPair);
                //relatedAttributesSet.insert(correlate, temp);
                
                std::vector<AttValPair*> temp2;
                temp2.push_back(newPair);
                relatedAttributes.insert(correlate, temp2);
            }
            else
            {
                //if(pointToCorrelation->find(newPair) == pointToCorrelation->end())
                //{
                   // pointToSet->insert(newPair);
                    pointToCorrelation->push_back(newPair);
               // }
            }
        }
    }
    
    return true;
}


std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs( const AttValPair& source) const
{
    std::vector<AttValPair> returnVector;
    std::vector<AttValPair*>* returnPointer;
    
    returnPointer = relatedAttributes.search(source.attribute+","+source.value);
    
    if(returnPointer == nullptr)
        return returnVector;
    
    for(int i = 0; i < returnPointer->size(); i++)
    {
        std::string s1 = returnPointer->at(i)->attribute;
        std::string s2 = returnPointer->at(i)->value;
    
        returnVector.push_back(AttValPair(s1, s2));
    }
    
    return returnVector;
}
