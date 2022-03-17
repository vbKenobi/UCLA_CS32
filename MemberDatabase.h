//
//  MemberDatabase.hpp
//  CS32_Unhinged
//
//  Created by Venky on 3/4/22.
//

#ifndef MemberDatabase_h
#define MemberDatabase_h

#include <stdio.h>
#include <string>
#include <vector>
#include "RadixTree.h"
#include "PersonProfile.h"

class MemberDatabase
{
  public:
    MemberDatabase();
    ~MemberDatabase();
    bool LoadDatabase(std::string filename);
    std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
    const PersonProfile* GetMemberByEmail(std::string email) const;

  private:
    RadixTree<std::vector<std::string>> attValToEmail;
    RadixTree<PersonProfile*> emailToPerson;
    std::vector<std::string> vectorOfEmails;
    
};

#endif /* MemberDatabase_h */

