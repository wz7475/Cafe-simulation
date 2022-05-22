//
// Created by bkisl on 27.04.2022.
//

#include "../../includes/model/Databases/menuDatabase.h"
#include "../../includes/model/MenuItem/exceptions.h."

using namespace std;

void MenuDatabase::validateItems(const vector<unique_ptr<MenuItem>> &menuItems) {
    if(menuItems.empty()) throw ArgumentEmptyException("Menu cannot be empty.");
    map<string, int> namesToOccurrences;

    for(auto &menuItem : menuItems)
    {
        string itemName = menuItem->GetName();

        if(!namesToOccurrences.contains(itemName))
        {
            namesToOccurrences.insert(pair{ itemName, 0 });
        }

        namesToOccurrences[itemName]++;
    }

    for(const auto& [key, value] : namesToOccurrences)
    {
        if(value > 1) throw ItemExistenceException("Menu item names must be unique inside database.");
    }
}

MenuDatabase::MenuDatabase(unique_ptr<MenuItem> initialItem) {
    items.push_back(move(initialItem));
}

MenuDatabase::MenuDatabase(vector<unique_ptr<MenuItem>> &initialItems) {
    validateItems(initialItems);
    items = move(initialItems);
}

const vector<unique_ptr<MenuItem>> &MenuDatabase::GetItems() {
    return items;
}

bool MenuDatabase::ItemExists(string itemName) const {
    for(const unique_ptr<MenuItem> &item : items)
    {
        if(item->GetName() == itemName)
            return true;
    }

    return false;
}

void MenuDatabase::AddItem(unique_ptr<MenuItem> item) {
    if(ItemExists(item->GetName()))
        throw ItemExistenceException("Cannot add an existing item.");

    items.push_back(move(item));
}

void MenuDatabase::RemoveItem(unsigned int itemIndex) {
    if(items.size() == 1)
        throw ItemExistenceException("Cannot remove a position from single-element menu");

    if(itemIndex >= items.size())
        throw IndexOutOfRangeException("Index of element to remove is out of range.");

    items.erase(items.begin() + itemIndex);
}

void MenuDatabase::RemoveItem(string itemName) {
    if(!ItemExists(itemName)) throw ItemExistenceException("Item with given name doesn't exist.");

    unsigned int index = find_if(items.begin(), items.end(),
                                 [&](const unique_ptr<MenuItem> &item) -> bool
                                 { return item->GetName() == itemName; }) - items.begin();

    RemoveItem(index);
}

ostream &MenuDatabase::Write(ostream &os) {
    return os;  // TODO: implement writing menu database to stream
}

istream &MenuDatabase::Read(istream &is) {
    return is;  // TODO: implement reading menu database from stream
}

ostream &operator<<(ostream &os, const MenuDatabase &menu) {
    return os;
}

istream &operator>>(istream &is, const MenuDatabase &menu) {
    return is;
}