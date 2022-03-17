//
//  AttributeTranslator.hpp
//  Unhinged_App
//
//  Created by Venky on 3/6/22.
//

#ifndef AttributeTranslator_hpp
#define AttributeTranslator_hpp

#include <stdio.h>
#include <string>
#include <unordered_set>
#include <vector>
#include "provided.h"
#include "RadixTree.h"

class AttributeTranslator
{
  public:
    AttributeTranslator();
    ~AttributeTranslator();
    bool Load(std::string filename);
    std::vector<AttValPair> FindCompatibleAttValPairs( const AttValPair& source) const;
    
 private:
    RadixTree<std::vector<AttValPair*>> relatedAttributes;
    //RadixTree<std::unordered_set<AttValPair*>> relatedAttributesSet;
    RadixTree<std::vector<AttValPair>> relatedAttributesTest;
    std::vector<std::string> vectorOfAttributes;
};



#endif /* AttributeTranslator_hpp */
