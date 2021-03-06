#include <iostream>
#include <vector>
#include <chrono>
#include <map>


struct Card{
    char id;
    int count = 0; // Default count is 0
};

struct Timer{
    Timer(){
        startTime = std::chrono::system_clock::now();
    }
    double GetTime(){
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - startTime;
        return diff.count();
    }
    ~Timer(){
        std::cout << "Time elapsed: " << GetTime() << "s\n";
    }
    std::chrono::time_point<std::chrono::system_clock> startTime;
};

std::vector<Card> IndexCardList(std::vector<char> in, std::vector<char> const &key){ // Sorts to match key
    std::vector<Card> CardIndex;
    for(char c : key){ // Dynamically generated id's
        CardIndex.push_back(Card {c});
    }
    for(char c : in){
        for(int i = 0; i < static_cast<int> (CardIndex.size()); i++){
            if(c == CardIndex[i].id){
                CardIndex[i].count++;
                break; // Stops after finding correct id
            }
        }
    }
    return CardIndex;
}

std::vector<Card> IndexUsingHashmap(std::vector<char> in, std::vector<char> const &key){ // Likely the most efficient
    std::map<char, int> hashmap;
    std::vector<Card> out;
    for(char c : in){ // Indexing
        hashmap[c]++;
    }
    for(char c : key){ // Sorting based on key
        out.push_back(Card{c, hashmap[c]});
    }
    return out;
}

std::vector<char> ReconstructList(std::vector<Card> const &IndexedList){
    std::vector<char> out;
    for(Card c : IndexedList){
        while(c.count){
            out.push_back(c.id);
            c.count--;
        }
    }
    return out;
}

void PrintCharList(std::vector<char> in){ // Utility
    for(char c : in){
        std::cout << c << ", ";
    }
    std::cout << "\n";
}

std::vector<char> ReorderKey(std::vector<char> InputList, std::vector<char> key){
    std::vector<Card> CardList = IndexCardList(InputList, key);

    key.clear();
    for(Card c : CardList){
        key.push_back(c.id);
    }
    return key;
}

int main()
{
    std::vector<char> const key = {'a', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'j', 'q', 'k'};

    int l = 200; // Setting random inputs to test sort
    std::vector<char> CardInputList;
    for(int i = 0; i < l; i++){
        CardInputList.emplace_back(key[rand() % key.size()]);
    }

    { std::cout << "Time to print char list;\n";
    Timer t;
    PrintCharList(CardInputList); // Unsorted list and print time for reference
    }

    { std::cout << "Time to reconstruct char list:\n";
    std::vector<Card> dummyList = {Card({'d', l})};
    Timer t;
    ReconstructList(dummyList);
    }

    // Sorting;

    { std::cout << "Inc list with ordered key:\n";
    Timer t;
    PrintCharList(ReconstructList(IndexCardList(CardInputList, key))); // You can unroll these if you want -- I'm just saving lines
    }

    { std::cout << "Inc sort with ideal key;\n";
    auto idealKey = ReorderKey(CardInputList, key);
    std::cout << "New key:\n";
    PrintCharList(idealKey);
    Timer t; // Only measuring the time it takes to sort with ideal key, not the time needed to generate that key
    PrintCharList(ReconstructList(IndexCardList(CardInputList, idealKey)));
    }

    { std::cout << "Hashmap sort: ";
    Timer t;
    PrintCharList(ReconstructList(IndexUsingHashmap(CardInputList, key)));
    }
}
