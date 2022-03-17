//
//  RadixTree.hpp
//  CS32_Unhinged
//
//  Created by Venky on 3/4/22.
//

#ifndef RadixTree_h
#define RadixTree_h

#include <string>
#include <iostream>

//RADIX TREE CLASS DEFINITION
template <typename ValueType>
class RadixTree {
  public:
    RadixTree();
    ~RadixTree();
    void insert(std::string key, const ValueType& value);
    ValueType* search(std::string key) const;
    
  private:
    struct Node
    {
      public:
        std::string s;
        Node* arr[128];
        ValueType value;
        bool isEnd;
        //Constructor
        Node(std::string temp, const ValueType& value2)
        {
            s = temp;
            for(int i = 0; i < 128; i++)
            {
                arr[i] = nullptr;
            }
            value = value2;
        }
        Node()
        {
            s = "";
            for(int i = 0; i < 128; i++)
            {
                arr[i] = nullptr;
            }
        }
    };
    
    Node* rootNode;
    
    int containsSubstr(std::string s1, std::string s2) const;
    void helpTraverseAndInsert(Node* current, Node* temp, std::string key, const ValueType& value);
    ValueType* helperSearch(Node* next, std::string key) const;
    void helpDelete(Node* next);
};

/////////////////////RADIX TREE CLASS IMPLEMENTATION:////////////////////////
//Constructor
template <typename ValueType>
RadixTree<ValueType>::RadixTree()
{
    rootNode = new Node;
}

//Destructor
template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
    helpDelete(rootNode);
}

//Destructor
template <typename ValueType>
void RadixTree<ValueType>::helpDelete(Node* next)
{
    if(next == nullptr)
        return;
    
    for(int i = 0; i < 128; i++)
    {
        helpDelete(next->arr[i]);
    }
    delete next;
}



//Insertion function
template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
    //If the substr doesn't start with the first letter, that is already present
    ValueType copyOfValue = value;
    int index = key[0] - 0;
    if(rootNode->arr[index] == nullptr)
    {
        //Create a node with the rest of the string
        Node* tempNode = new Node(key, copyOfValue);
        tempNode->isEnd = true;
        //Add it at the index of the first letter
        rootNode->arr[index] = tempNode;
    }
    else
    {
        //Call a recursive function to continue traversing the tree until no matching substr is found
        helpTraverseAndInsert(rootNode, nullptr, key, copyOfValue);
    }
}

//Search Function
template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const
{
    if(rootNode->arr[key[0]-0] == nullptr)
        return nullptr;
    else
        return helperSearch(rootNode->arr[key[0]-0], key);
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::helperSearch(Node* next, std::string key) const
{
    if(next == nullptr)
        return nullptr;
    
    int index = -1;
    
    if(key.rfind(next->s, 0) == 0)
    {
        key = key.substr(next->s.length(), key.length());
        index = 1;
    }
    else
        return nullptr;
    /*
    int index = containsSubstr(next->s, key);
    key = key.substr(index+1, key.length());
     */
    
    if(key.length() == 0)
    {
        //ValueType* point = new ValueType;
        if(!(next->isEnd))
            return nullptr;
        
        ValueType* point = &next->value;
        //*point = next->value;
        return point;
    }
    
    if(index < 0)
        return nullptr;
    else
        return helperSearch(next->arr[key[0]-0], key);
        
}


//Assume that s1 is the string that is already present in the tree
//Assume that s2 is the string we want to see if s1 is present in
//Returns the final index where the substr ends
template <typename ValueType>
int RadixTree<ValueType>::containsSubstr(std::string s1, std::string s2) const
{
    if(s1.length() > s2.length() || s1[0] != s2[0])
        return -1;
    
    int index = -1;
    for(int i = 0; i < s1.length(); i++)
    {
        if(s1[i] != s2[i])
            break;
        index++;
    }
    
    return index;
}

template <typename ValueType>
void RadixTree<ValueType>::helpTraverseAndInsert(Node* current, Node* previous, std::string key, const ValueType& value)
{
    //Check to see if the key will contain a substr of an exisiting element
    if(current->arr[key[0]-0] == nullptr)
    {
        Node* newKey = new Node(key, value);
        newKey->isEnd = true;
        current->arr[key[0]-0] = newKey;
        return;
    }
    else
    {
        std::string nodeString = current->arr[key[0]-0]->s;
        Node* pointToNext = current->arr[key[0]-0];
        int index = containsSubstr(nodeString, key);
        
        int reverseIndex = containsSubstr(key, nodeString);
         //case for when the key is smaller than the nodeString
        if(reverseIndex != -1 && index < 0)
        {
            Node* newRoot = new Node(key.substr(0, reverseIndex+1), value);
            Node* breakRoot = new Node(nodeString.substr(reverseIndex+1, nodeString.length()), pointToNext->value);
            breakRoot->isEnd = true;
            newRoot->isEnd = true;
            newRoot->arr[breakRoot->s[0]-0] = breakRoot;
            
            if(current->arr[key[0]-0] != nullptr)
                delete current->arr[key[0]-0];
            
            current->arr[key[0]-0] = newRoot;
            
            if(reverseIndex != key.length()-1)
            {
                Node* halfRoot = new Node(key.substr(reverseIndex+1, key.length()), value);
                newRoot->isEnd = false;
                halfRoot->isEnd = true;
                newRoot->arr[halfRoot->s[0]-0] = halfRoot;
            }
            
            return;
        }
        
        //Remove the duplicating part of the key
        key = key.substr(index+1, key.length());
        
        if(key.length() == 0)
        {
            pointToNext->value = value;
            return;
        }
        //If we fouund the entire substring, we must add a brand new node, to the current node
        if(index == nodeString.length()-1 && pointToNext->arr[key[0]-0] == nullptr)
        {
            Node* newKey = new Node(key, value);
            newKey->isEnd = true;
            pointToNext->arr[key[0]-0] = newKey;
            return;
        }
        //If it is not the null pointer, then check this next substr
        else if(index == nodeString.length()-1 && pointToNext->arr[key[0]-0] != nullptr)
        {
            helpTraverseAndInsert(pointToNext, current, key, value);
        }
        //If the entire substring was not found, we must instead split up the current node, and create a new node
        else
        {
            Node* newRoot = new Node( (pointToNext->s).substr(0, index+1), pointToNext->value);
            newRoot->isEnd = false;
            
            Node* newKey = new Node(key, value);
            newKey->isEnd = true;
            
            //Change the current string
            pointToNext->s = pointToNext->s.substr(index+1, pointToNext->s.length());
            //Have the new base root point to the current root
            newRoot->arr[pointToNext->s[0]-0] = pointToNext;
            //Have the previous root, point to the base root
            current->arr[newRoot->s[0]-0] = newRoot;
            //Have the new base root point to the new key
            newRoot->arr[newKey->s[0]-0] = newKey;
        }
    }
}




#endif /* RadixTree_h */

