//
//  MatchMaker.hpp
//  Unhinged_App
//
//  Created by Venky on 3/7/22.
//

#ifndef MatchMaker_h
#define MatchMaker_h

#include <stdio.h>
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "provided.h"
#include <unordered_set>

class MatchMaker
{
  public:
    MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
    ~MatchMaker();
    std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
  private:
    const MemberDatabase* memBase;
    const AttributeTranslator* atrTrans;
};

#endif /* MatchMaker_h */
