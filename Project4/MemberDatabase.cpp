//
//  MemberDatabase.cpp
//  CS32_Unhinged
//
//  Created by Venky on 3/4/22.
//

#include "MemberDatabase.h"
#include "provided.h"
#include <iostream>
#include <fstream>
#include <string>

MemberDatabase::MemberDatabase()
{}

MemberDatabase::~MemberDatabase()
{
    //delete attValToEmail;
    //delete emailToPerson;
    for(int i = 0; i < vectorOfEmails.size(); i++)
    {
        if(emailToPerson.search(vectorOfEmails[i]) != nullptr)
        {
            PersonProfile** temp = emailToPerson.search(vectorOfEmails[i]);
            PersonProfile** temp2 = temp;
            temp = nullptr;
            delete *temp2;
        }
    }
}

bool MemberDatabase::LoadDatabase(std::string fileName)
{
    std::ifstream myfile;
    myfile.open(fileName);
    std::string email;
    std::string name;
    std::string numAttributes;
    PersonProfile* tempPerson;
    std::string atValPair;
    int iterate;
    
    if(myfile.is_open())
    {
        while(myfile)
        {
            std::getline(myfile, name);
            std::getline(myfile, email);
            tempPerson = new PersonProfile(name, email);
            
            std::getline(myfile, numAttributes);
            iterate = std::stoi(numAttributes);
            
            for(int i = 0; i < iterate; i++)
            {
                std::getline(myfile, atValPair);
                std::string delimiter = ",";
                std::string att = atValPair.substr(0, atValPair.find(delimiter));
                std::string val = atValPair.substr(atValPair.find(delimiter)+1, atValPair.length());
                
                tempPerson->AddAttValPair(AttValPair(att, val));
            }
            
            int numIterations = tempPerson->GetNumAttValPairs();
            for(int i = 0; i < numIterations; i++)
            {
                AttValPair tempPair;
                tempPerson->GetAttVal(i, tempPair);
                std::vector<std::string>* pointToEmail = attValToEmail.search(tempPair.attribute+","+tempPair.value);
                
                if(pointToEmail != nullptr)
                {
                    pointToEmail->push_back(email);
                }
                else if(pointToEmail == nullptr)
                {
                    std::vector<std::string> tempEmails;
                    tempEmails.push_back(email);
                    attValToEmail.insert(tempPair.attribute+","+tempPair.value, tempEmails);
                }
            }
            
            
            //Radix tree of mapping email to a person
            PersonProfile** pointToPerson = emailToPerson.search(email);
            
            if(pointToPerson !=  nullptr)
                return false;
            else
                emailToPerson.insert(email, tempPerson);

            vectorOfEmails.push_back(email);
            
            std::getline(myfile, email);
        }
    }
    
    return true;
}


std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
    std::string pair = input.attribute + "," + input.value;
    std::vector<std::string>* vectorEmails  = attValToEmail.search(pair);
    std::vector<std::string> copyEmails = *vectorEmails;
    return copyEmails;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
    PersonProfile** pointToProfile = emailToPerson.search(email);
    
    if(pointToProfile == nullptr)
        return nullptr;
    
    PersonProfile* returnProfile = *pointToProfile;
    return returnProfile;
}

