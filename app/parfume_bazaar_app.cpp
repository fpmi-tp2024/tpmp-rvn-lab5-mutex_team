//
// Created by Alexander Douhi on 4/3/2024.
//

#include "parfume_bazaar/parfume_bazaar.h"
#include <iostream>
#include <vector>

using namespace std;

const char* db_path = "./parfume.db";

void who_are_you();
void hello_admin();
void hello_makler();
void makler_menu(parfume_bazaar::Makler&);
void admin_menu(parfume_bazaar::Admin&);

void who_are_you()
{
    int ans;
    cout << "Who are you\n 1.Admin\n 2.Makler\n";
    cin >> ans;
    switch (ans)
    {
    case 1:
        hello_admin();
        break;
    case 2:
        hello_makler();
        break;
    default:
        who_are_you();
        break;
    }
}

void hello_admin()
{
    bool res;
    string login, password;
    cout << "Hello admin!\n login: ";
    cin >> login;
    cout << " password: ";
    cin >> password;
    parfume_bazaar::Admin admin(login);
    res = admin.insert(admin, password, db_path);
    if (!res)
    {
        cout << "Invalid admin\n";
        who_are_you();
    }
    admin_menu(admin);
}

void hello_makler()
{
    bool res;
    string surname, password, address;
    int y, m, d;
    cout << "Hello makler!\n surname: ";
    cin >> surname;
    cout << " birth date (y, m, d): ";
    cin >> y >> m >> d;
    if (!parfume_bazaar::Date::checkDate(y, m, d))
    {
        cout << "Invalid date\n";
        who_are_you();
        return;
    }
    cout << " address: ";
    cin >> address;
    cout << " password: ";
    cin >> password;
    parfume_bazaar::Date date(y, m, d);
    parfume_bazaar::Makler makler(surname, date, address);
    res = makler.insert(makler, password, db_path);
    if (!res)
    {
        cout << "Invalid makler\n";
        who_are_you();
    }
    makler_menu(makler);
}

void makler_menu(parfume_bazaar::Makler& makler)
{
    bool res;
    int ans, amount;
    string old_password, new_password, item_name, item_type, firm_name;
    int y, m, d;
    cout << "Makler menu\n 1.Change password\n 2.Get popular item info\n 3.Get successful makler info\n 4.Get bargains\n 5.Make bargain\n 6.Remove account\n 7.Exit\n";
    cin >> ans;
    pair<string, vector<string> > p;
    pair<parfume_bazaar::Makler, vector<string> > smi;
    vector<parfume_bazaar::Bargain> gb;
    switch (ans)
    {
    case 1:
    {
        cout << "Input\n old password: ";
        cin >> old_password;
        cout << " new password: ";
        cin >> new_password;
        res = makler.update(makler, old_password, new_password, db_path);
        makler_menu(makler);
        break;
    }
    case 2:
    {
        cout << "Input:\n start date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            makler_menu(makler);
            break;
        }
        parfume_bazaar::Date start_date(y, m, d);
        cout << " new date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            makler_menu(makler);
            break;
        }
        parfume_bazaar::Date old_date(y, m, d);
        p = parfume_bazaar::getPopularItemInfo(makler, start_date, old_date, db_path);
        cout << p.first << "\n";
        for (string& info : p.second)
            cout << info << "\n";
        makler_menu(makler);
        break;
    }
    case 3:
    {
        cout << "Input:\n start date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            makler_menu(makler);
            break;
        }
        parfume_bazaar::Date start_date(y, m, d);
        cout << " new date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            makler_menu(makler);
            break;
        }
        parfume_bazaar::Date old_date(y, m, d);
        smi = parfume_bazaar::getSuccessfulMaklerInfo(makler, start_date, old_date, db_path);
        cout << "Makler surname: " << smi.first.getSurname() << "\n";
        for (string& info : smi.second)
            cout << info << "\n";
        makler_menu(makler);
        break;
    }
    case 4:
    {
        cout << "Input:\n date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            makler_menu(makler);
            break;
        }
        parfume_bazaar::Date date(y, m, d);
        gb = parfume_bazaar::getBargains(makler, date, db_path);
        cout << "Bargains for date " << date.toSqliteFormat() << "\n";
        for (parfume_bazaar::Bargain& b : gb)
            cout << b.getItemType() << " " << b.getItemName() << " " << b.getFirmBuyerName() << " " << b.getDate().toSqliteFormat() << " " << b.getAmount() << "\n";
        makler_menu(makler);
        break;
    }
    case 5:
    {
        cout << "Input bargain:\n date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            makler_menu(makler);
            break;
        }
        cout << " item name: ";
        cin >> item_name;
        cout << " item type: ";
        cin >> item_type;
        cout << " firm name: ";
        cin >> firm_name;
        cout << " amount: ";
        cin >> amount;
        parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(y, m, d), item_name, item_type, amount, firm_name);
        parfume_bazaar::Bargain::insert(makler, bargain, db_path);
        break;
    }
    case 6:
        cout << "Input\n password: ";
        cin >> old_password;
        parfume_bazaar::Makler::remove(makler, old_password, db_path);
        who_are_you();
        break;
    case 7:
        return;
    default:
        who_are_you();
        break;
    }
}


void admin_menu(parfume_bazaar::Admin& admin)
{
    int ans;
    int y, m, d;
    cout << "Admin menu\n 1.Change password\n 2.Add item\n 3.Update item amount\n 4.Delete item\n 5.Get item info\n 6.Get firm info about item\n 7.Get firm seller info\n 8.Remove account\n 9.Exit\n";
    cin >> ans;
    switch (ans)
    {
    case 1:
    {
        string old_password, new_password;
        cout << "Input\n old password: ";
        cin >> old_password;
        cout << " new password: ";
        cin >> new_password;
        admin.update(admin, old_password, new_password, db_path);
        admin_menu(admin);
        break;
    }
    case 2:
    {
        string name, type, seller_name;
        double price;
        int amount;
        cout << "Input\n name: ";
        cin >> name;
        cout << " type: ";
        cin >> type;
        cout << " seller name: ";
        cin >> seller_name;
        cout << " price: ";
        cin >> price;
        cout << " amount: ";
        cin >> amount;
        cout << " expiration date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            admin_menu(admin);
            break;
        }
        parfume_bazaar::Item item(name, type, price, seller_name, parfume_bazaar::Date(y, m, d), amount);
        parfume_bazaar::Item::insert(admin, item, db_path);
        admin_menu(admin);
        break;
    }
    case 3:
    {
        string name;
        cout << "Input:\n item name: ";
        cin >> name;
        cout << " amount: ";
        int amount;
        cin >> amount;
        parfume_bazaar::Item::update(admin, name, amount, db_path);
        admin_menu(admin);
        break;
    }
    case 4:
    {
        string name;
        cout << "Input:\n item name: ";
        cin >> name;
        parfume_bazaar::Item::remove(admin, name, db_path);
        admin_menu(admin);
        break;
    }
    case 5:
    {
        cout << "Input:\n start date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            admin_menu(admin);
            break;
        }
        parfume_bazaar::Date start_date(y, m, d);
        cout << " end date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            admin_menu(admin);
            break;
        }
        parfume_bazaar::Date end_date(y, m, d);
        vector<string> infos = parfume_bazaar::getItemInfo(admin, start_date, end_date, db_path);
        cout << "Items info:\n";
        for (string& info : infos)
            cout << info << "\n";
        admin_menu(admin);
        break;
    }
    case 6:
    {
        cout << "Input:\n start date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            admin_menu(admin);
            break;
        }
        parfume_bazaar::Date start_date(y, m, d);
        cout << " end date (y, m, d): ";
        cin >> y >> m >> d;
        if (!parfume_bazaar::Date::checkDate(y, m, d))
        {
            cout << "Invalid date\n";
            admin_menu(admin);
            break;
        }
        parfume_bazaar::Date old_date(y, m, d);
        vector<string> infos = parfume_bazaar::getFirmInfoAboutItem(admin, start_date, old_date, db_path);
        for (string& info : infos)
            cout << info << "\n";
        admin_menu(admin);
        break;
    }
    case 7:
    {
       vector< pair< string, vector<string> > > sellers = parfume_bazaar::getFirmSellerInfo(admin, db_path);
       for (pair< string, vector<string> >& seller : sellers)
       {
           cout << "Seller name: " << seller.first << "\n";
           cout << "Info:\n";
           for (string& info : seller.second)
               cout << info << "\n";
       }
       admin_menu(admin);
       break;
    }
    case 8:
    {
        string password;
        cout << "Input\n password: ";
        cin >> password;
        parfume_bazaar::Admin::remove(admin, password, db_path);
        who_are_you();
        break;
    }
    case 9:
        return;
    default:
        who_are_you();
        break;
    }
}

int main() {
    if (parfume_bazaar::createTables())
    {
        parfume_bazaar::printHelloWorld();
        who_are_you();
    }
    return 0;
}
