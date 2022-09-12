#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>

using namespace std;

class Item {
private:
    int cost;
    string unique_id;

public:
    Item() {}

    Item(string unique_id, int cost): unique_id(unique_id), cost(cost) {

    }

    string getUniqueId() {
        return this->unique_id;
    }

    int getCost() {
        return this->cost;
    }
};

unordered_map<string, Item> ITEM_MAP = {
    {"apple", Item("apple", 10)},
    {"banana", Item("banana", 5)}
};

class Offer {
public:
    virtual int getDiscount(unordered_map<string, int> contents) = 0;
};


class ShoppingCart {
private:
    unordered_map<string, int> contents;
    vector<Offer*> offers;

public:
    void addItem(string item_id) {
        contents[item_id]++;
    } 

    void addOffer(Offer *offer) {
        this->offers.push_back(offer);
    }

    int getTotal() {
        int total = 0;
        for (auto &entry: this->contents) {
            total += (ITEM_MAP[entry.first].getCost() * entry.second);
        }
        for (auto &offer: this->offers) {
            total -= offer->getDiscount(this->contents);
        }
        return total;
    }
};

class Offer_BuyOneGetOneFree : public Offer {
private:
    string item_id;
public:
    Offer_BuyOneGetOneFree(string item_id): item_id(item_id) {}

    int getDiscount(unordered_map<string, int> contents) {
        int number_of_pairs = contents[this->item_id] / 2;
        return ITEM_MAP[this->item_id].getCost() * number_of_pairs;
    }
};


void test_add_apples() {
    ShoppingCart cart;
    cart.addItem("apple");
    cart.addItem("apple");

    assert(cart.getTotal() == 20);
}

void test_add_bananas() {
    ShoppingCart cart;
    cart.addItem("apple");
    cart.addItem("banana");

    assert(cart.getTotal() == 15);
}

void test_add_offers() {
    ShoppingCart cart;
    Offer_BuyOneGetOneFree* offer = new Offer_BuyOneGetOneFree("apple");
    cart.addItem("apple");
    cart.addItem("apple");
    cart.addOffer(offer);

    assert(cart.getTotal() == 10);
}


int main() {

    test_add_apples(); 
    test_add_bananas(); 
    test_add_offers(); 

    cout << "Tests succeeded" << endl;
}

