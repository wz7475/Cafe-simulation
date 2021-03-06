//
// Created by bkisl on 27.04.2022.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "../../includes/model/Databases/menuDatabase.h"
#include "../../includes/model/MenuItem/beverage.h"
#include "../../includes/model/MenuItem/dessert.h"
#include "../../includes/model/MenuItem/dish.h"
#include "../../includes/model/helpers/exceptions.h"
#include "../../includes/model/Databases/menuDatabase.h"

TEST_CASE("Initialization")
{
    SECTION("Constructor tests")
    {
        Beverage coffee("Coffee", Price(2, 99), CupType::Cup, 3);
        Dessert cake("Cake", Price(3, 49), 3);
        Dish salad("Salad", Price(9, 99), true, 3);

        vector<shared_ptr<MenuItem>> items;

        CHECK_THROWS_AS(MenuDatabase(items), ArgumentEmptyException);

        items.push_back(make_shared<Beverage>(coffee));
        items.push_back(make_shared<Dessert>(cake));
        items.push_back(make_shared<Dish>(salad));

        REQUIRE_NOTHROW(MenuDatabase(make_shared<Beverage>(coffee)));
        REQUIRE_NOTHROW(MenuDatabase(items));

        MenuDatabase menu1(make_shared<Beverage>(coffee));
        CHECK(menu1.GetItems().size() == 1);

        vector<shared_ptr<MenuItem>> itemsNew;
        itemsNew.push_back(make_shared<Beverage>(coffee));
        itemsNew.push_back(make_shared<Dessert>(cake));
        itemsNew.push_back(make_shared<Dish>(salad));
        MenuDatabase menu2(itemsNew);
        CHECK(menu2.GetItems().size() == 3);
    }
}

TEST_CASE("Collection methods")
{
    Beverage coffee("Coffee", Price(2, 99), CupType::Cup, 4);
    Dessert cake("Cake", Price(3, 49), 3);
    Dish salad("Salad", Price(9, 99), true, 3);

    vector<shared_ptr<MenuItem>> items;
    items.push_back(make_shared<Beverage>(coffee));
    items.push_back(make_shared<Dessert>(cake));

    MenuDatabase menu(items);

    SECTION("Item exists")
    {
        CHECK(menu.ItemExists("Coffee"));
        CHECK_FALSE(menu.ItemExists("Salad"));
    }

    SECTION("Add item")
    {
        Beverage anotherCoffee("Coffee", Price(10, 99), CupType::Mug, 5);
        CHECK_THROWS_AS(menu.AddItem(make_shared<Beverage>(anotherCoffee)), ItemExistenceException);
        REQUIRE_NOTHROW(menu.AddItem(make_shared<Dish>(salad)));
        CHECK(menu.GetItems().size() == 3);
    }

    SECTION("Remove item")
    {
        CHECK_THROWS_AS(menu.RemoveItem(20), IndexOutOfRangeException);
        CHECK_THROWS_AS(menu.RemoveItem("Abcde"), ItemExistenceException);
        REQUIRE_NOTHROW(menu.RemoveItem("Coffee"));

        CHECK(menu.GetItems().size() == 1);
        CHECK_THROWS_AS(menu.RemoveItem("Cake"), ItemExistenceException);
    }
}

TEST_CASE("Stream operations")
{
    stringstream ss;

    SECTION("Output")
    {
        MenuDatabase db(make_shared<Beverage>("Coffee", Price(2, 49), CupType::Cup, 3));
        db.AddItem(make_shared<Dessert>("Cake", Price(5, 99), 2));
        db.AddItem(make_shared<Dish>("Caesar salad", Price(19, 99), true, 5));

        ss << db;

        CHECK(ss.str() == "012\nCoffee\n2 49 3 0\nCake\n5 99 2\nCaesar salad\n19 99 5 1\n");
    }

    SECTION("Input")
    {
        ss = stringstream();
        MenuDatabase db(make_shared<Beverage>("Coffee", Price(2, 49), CupType::Cup, 3));
        db.AddItem(make_shared<Dessert>("Cake", Price(5, 99), 2));
        db.AddItem(make_shared<Dish>("Caesar salad", Price(19, 99), true, 5));

        MenuDatabase newDb(make_shared<Beverage>("Lonely coffee", Price(0, 5), CupType::Cup, 3));

        ss << db;
        ss >> newDb;

        vector<shared_ptr<MenuItem>> newItems = newDb.GetItems();

        CHECK(newItems[0]->GetName() == "Coffee");
        CHECK(newItems[1]->GetName() == "Cake");
        CHECK(newItems[2]->GetName() == "Caesar salad");
    }
}
