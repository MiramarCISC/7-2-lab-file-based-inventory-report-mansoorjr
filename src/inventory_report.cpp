#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
    return quantity >= 0;
}

bool isValidPrice(double price) {
    return price >= 0.0;
}

double calculateItemValue(const InventoryItem& item) {
    if (!isValidQuantity(item.quantity) || !isValidPrice(item.price)) {
        return 0.0;
    }
    return item.quantity * item.price;
}

// Reads sku, name, quantity, and price from each line until the file ends or maxItems is reached
int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    ifstream in(filename);
    if (!in.is_open()) {
        return 0;
    }

    int count = 0;
    string sku, name;
    int quantity;
    double price;

    while (count < maxItems && (in >> sku >> name >> quantity >> price)) {
        items[count].sku = sku;
        items[count].name = name;
        items[count].quantity = quantity;
        items[count].price = price;
        count++;
    }

    in.close();
    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        out << items[i].sku << " " << items[i].name << " "
            << calculateItemValue(items[i]) << endl;
    }

    out << "Total inventory value: " << calculateTotalInventoryValue(items, count) << endl;

    out.close();
    return true;
}

double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return 0.0;
    }

    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total += calculateItemValue(items[i]);
    }
    return total;
}

int findItemBySku(const InventoryItem items[], int count, string sku) {
    if (items == nullptr) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (items[i].sku == sku) {
            return i;
        }
    }
    return -1;
}

// Starts by assuming the first item is highest, then compares each item's value to find the true highest
int findHighestValueItemIndex(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return -1;
    }

    int highestIndex = 0;
    double highestValue = calculateItemValue(items[0]);

    for (int i = 1; i < count; i++) {
        double value = calculateItemValue(items[i]);
        if (value > highestValue) {
            highestValue = value;
            highestIndex = i;
        }
    }
    return highestIndex;
}