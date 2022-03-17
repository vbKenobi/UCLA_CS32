//
//  MatchMaker.cpp
//  Unhinged_App
//
//  Created by Venky on 3/7/22.
//

#include "MatchMaker.h"
#include <vector>
#include <unordered_set>
#include <map>
#include <algorithm>

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
{
    memBase = new MemberDatabase;
    memBase = &mdb;
    atrTrans = new AttributeTranslator;
    atrTrans = &at;
}

MatchMaker::~MatchMaker()
{
}

bool cmp(const EmailCount& e1, const EmailCount& e2)
{
    if(e1.count < e2.count)
        return false;
    else if(e1.count > e2.count)
        return true;
    
    if(e1.email < e2.email)
        return true;
    else
        return false;
}
  


std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const
{
    std::vector<EmailCount> emailCounts;
    
    //Get the list of traits for the given email, ensure that there are no duplicates since two values can map to the same thing
    const PersonProfile* person = memBase->GetMemberByEmail(email);
    std::unordered_set<std::string> compatAttr;
    
    AttValPair av;
    for(int i = 0; i < person->GetNumAttValPairs(); i++)
    {
        //Get the first attribute
        person->GetAttVal(i, av);
        //Get the vector of Attributes, for the corresponding attribute
        std::vector<AttValPair> vectorOfAttributes = atrTrans->FindCompatibleAttValPairs(av);
        
        //Add it to the unordered_set to make sure duplicates do not get added
        for(int j = 0; j < vectorOfAttributes.size(); j++)
        {
            compatAttr.insert(vectorOfAttributes[j].attribute+","+vectorOfAttributes[j].value);
        }
    }

    //For each of the related attributes find the corresponding emails
    std::unordered_set<std::string>::iterator it = compatAttr.begin();
    std::map<std::string, int> emailToMatches;
    std::vector<std::string> listOfEmails;
    
    typedef std::map<std::string, int>::iterator MapIterator;

    // Pair of Map Iterator and bool value
    std::pair< MapIterator , bool> result;
    
    while(it != compatAttr.end())
    {
        std::string delimiter = ",";
        std::string att = it->substr(0, it->find(delimiter));
        std::string val = it->substr(it->find(delimiter)+1, it->length());
        
        listOfEmails = memBase->FindMatchingMembers(AttValPair(att, val));
        for(int i = 0; i < listOfEmails.size(); i++)
        {
            result = emailToMatches.insert(std::pair<std::string, int>(listOfEmails[i], 1));
            if(result.second == false)
            {
                auto it2 = emailToMatches.find(listOfEmails[i]);
                it2->second += 1;
            }
        }
        it++;
    }
    
    std::map<std::string, int>::iterator it3 = emailToMatches.begin();
    while(it3 != emailToMatches.end())
    {
        if(it3->first == email)
        {
            it3++;
            continue;
        }
        
        if(it3->second >= threshold)
        {
            emailCounts.push_back(EmailCount(it3->first, it3->second));
        }
        it3++;
    }
    std::sort(emailCounts.begin(), emailCounts.end(), cmp);
  
    return emailCounts;
}
