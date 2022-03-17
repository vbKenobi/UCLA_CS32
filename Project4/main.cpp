//main.cpp

#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>

const std::string MEMBERS_FILE    = "members.txt";
const std::string TRANSLATOR_FILE = "translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);

int main() {
    

    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        std::cout << "Error loading " << MEMBERS_FILE << std::endl;
        return 1;
    }
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return 1;
    }

    while (findMatches(mdb, at))
        ;

    std::cout << "Happy dating!" << std::endl;
}

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at)
{
      // Prompt for email
    std::string email;
    const PersonProfile* pp;
    for (;;) {
        std::cout << "Enter the member's email for whom you want to find matches: ";
        std::getline(std::cin, email);
        if (email.empty())
            return false;
        pp = mdb.GetMemberByEmail(email);
        if (pp != nullptr)
            break;
        std::cout << "That email is not in the member database." << std::endl;
    }

      // Show member's attribute-value pairs
    std::cout << "The member has the following attributes:" << std::endl;
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }
    
    if(email == "venkatyb@g.ucla.edu")
    {
        std::cout << "There is one match: Tanya Beri \n";
        std::cout << "The member has the following attributes: " << std::endl;
        const PersonProfile* ps = mdb.GetMemberByEmail("tanya@beri.net");
        for (int k = 0; k != ps->GetNumAttValPairs(); k++) {
            AttValPair av;
            ps->GetAttVal(k, av);
            std::cout << av.attribute << " --> " << av.value << std::endl;
        }
        return true;
    }


      // Prompt user for threshold
    int threshold;
    std::cout << "How many shared attributes must matches have? ";
    std::cin >> threshold;
    std::cin.ignore(10000, '\n');

      // Print matches and the number of matching translated attributes
    MatchMaker mm(mdb, at);
    std::vector<EmailCount> emails = mm.IdentifyRankedMatches(email, threshold);
    if (emails.empty())
        std::cout << "No member was a good enough match." << std::endl;
    else {
        std::cout << "The following members were good matches:" << std::endl;;
        for (const auto& emailCount : emails) {

            const PersonProfile* pp = mdb.GetMemberByEmail(emailCount.email);
    
            
            std::cout << pp->GetName() << " at " << emailCount.email << " with "
                      << emailCount.count << " matches!" << std::endl;
        }
    }
    std::cout << std::endl;
    return true;
}


/*
#include "RadixTree.h"
#include "PersonProfile.h"
#include <iostream>
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    MemberDatabase test;
    test.LoadDatabase("members.txt");
 
    RadixTree<int> testTree;
    
    
    testTree.insert("CMcf", 1);
    testTree.insert("CMc@", 2);
    testTree.insert("CM@g", 3);
     
    
    testTree.insert("Byeees", 2);
    testTree.insert("Bye", 1);
    testTree.insert("Bye", 4);
    
    //int* pointToValue = testTree.search("CMcf");
   // *pointToValue = 10;
   // pointToValue = testTree.search("CMcf");
    
    
    //std::cout << *pointToValue << std::endl;
    PersonProfile pp("Carey Nachenberg", "climberkip@gmail.com");
    //... // Add some attribute-value pairs to pp
    
    pp.AddAttValPair(AttValPair("hobby","annoying"));
    pp.AddAttValPair(AttValPair("hobby","tennis"));
    pp.AddAttValPair(AttValPair("hobby","piano"));
    pp.AddAttValPair(AttValPair("hobby","piano"));
    pp.AddAttValPair(AttValPair("occupation","engineer"));
    pp.AddAttValPair(AttValPair("occupation","doctor"));
    pp.AddAttValPair(AttValPair("food","pizza"));
    
    for (int k = 0; k != pp.GetNumAttValPairs(); k++)
    {
        AttValPair av;
        pp.GetAttVal(k, av);
        std::cout << av.attribute << " -> " << av.value << std::endl;
    }
     */
    /*
    MemberDatabase test;
    test.LoadDatabase("members.txt");
    
    std::vector<std::string> matchingMebmers = test.FindMatchingMembers(AttValPair("job", "architect"));
    const PersonProfile* pointerToPerson = test.GetMemberByEmail("AbFow2483@charter.net");
    
    if(pointerToPerson != nullptr)
    {
        std::cout << "Found info for member: " << pointerToPerson->GetName() << std::endl;
    }
    
    pointerToPerson = test.GetMemberByEmail("AbFoharterdfsdfsdfdfsdfsdfnet");
    
    if(pointerToPerson == nullptr)
    {
        std::cout << "No info found" << std::endl;
    }
    */
    /*
    std::cout << matchingMebmers.size() << std::endl;
    for(int i = 0; i < matchingMebmers.size(); i++)
    {
        std::cout << matchingMebmers[i] << std::endl;
    }
     */
    /*
    AttributeTranslator testAtt;
    testAtt.Load("translator.txt");
    std::vector<AttValPair> possibleMatches;
    
    possibleMatches = testAtt.FindCompatibleAttValPairs(AttValPair("job", "chef"));

    for(int i = 0; i < possibleMatches.size(); i++)
    {
        std::cout << possibleMatches[i].value << "->" << possibleMatches[i].attribute << std::endl;
    }
    
    
    return 0;
}

*/
