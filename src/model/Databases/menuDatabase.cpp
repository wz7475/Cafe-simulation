//
// Created by bkisl on 27.04.2022.
//

#include "../../../includes/model/Databases/menuDatabase.h"
#include "../../../includes/model/helpers/exceptions.h"

using namespace std;

void MenuDatabase::validateItems(const vector<shared_ptr<MenuItem>> &menuItems) {
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

MenuDatabase::MenuDatabase(shared_ptr<MenuItem> initialItem) {
    items.push_back(initialItem);
}

MenuDatabase::MenuDatabase(vector<shared_ptr<MenuItem>> &initialItems) {
    validateItems(initialItems);
    items = initialItems;
}

const vector<shared_ptr<MenuItem>> &MenuDatabase::GetItems() {
    return items;
}

bool MenuDatabase::ItemExists(string itemName) const {
    for(const shared_ptr<MenuItem> &item : items)
    {
        if(item->GetName() == itemName)
            return true;
    }

    return false;
}

void MenuDatabase::AddItem(shared_ptr<MenuItem> item) {
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
                                 [&](const shared_ptr<MenuItem> &item) -> bool
                                 { return item->GetName() == itemName; }) - items.begin();

    RemoveItem(index);
}

ostream &MenuDatabase::Write(ostream &os) {
    os << *this;
    return os;
}

istream &MenuDatabase::Read(istream &is) {
    is >> *this;
    return is;
}

ostream &operator<<(ostream &os, const MenuDatabase &menu) {
    string typesRecord;

    for(const shared_ptr<MenuItem> &item : menu.items)
    {
        if(typeid(*item.get()) == typeid(Beverage&))
            typesRecord += '0';
        else if(typeid(*item.get()) == typeid(Dessert&))
            typesRecord += '1';
        else if(typeid(*item.get()) == typeid(Dish&))
            typesRecord += '2';
    }

    os << typesRecord << endl;

    for(const shared_ptr<MenuItem> &item : menu.items)
    {
        item->Write(os);
        os << endl;
    }

    return os;
}

istream &operator>>(istream &is, MenuDatabase &menu) {
    vector<shared_ptr<MenuItem>> newItems;
    string typesRecord;
    menu.items.clear();

    is >> typesRecord;

    for(char typeId : typesRecord)
    {
        int typeInt = typeId - '0';

        switch (typeInt)
        {
            case 0:
            {
                Beverage bev("x", Price(0, 1), CupType::Cup, 1);
                bev.Read(is);
                menu.AddItem(make_shared<Beverage>(bev));
                break;
            }
            case 1:
            {
                Dessert des("y", Price(0, 2), 1);
                des.Read(is);
                menu.AddItem(make_shared<Dessert>(des));
                break;
            }
            case 2:
            {
                Dish dish("z", Price(0, 3), false, 1);
                dish.Read(is);
                menu.AddItem(make_shared<Dish>(dish));
                break;
            }
            default:
                break;
        }
    }

    return is;
}
