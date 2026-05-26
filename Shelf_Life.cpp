#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <limits>
#include "nlohmann/json.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;


// No.1 Grocery Item Structure

struct GroceryItem {
    string name;
    double price;
    string category;
    string last_purchase_date;
    int days_until_runout;

    // Convert object to JSON
    json to_json() const {
        return json{
            {"name", name},
            {"price", price},
            {"category", category},
            {"last_purchase_date", last_purchase_date},
            {"days_until_runout", days_until_runout}
        };
    }

    // Create object from JSON
    static GroceryItem from_json(const json& j) {
        return GroceryItem{
            j.at("name").get<string>(),
            j.at("price").get<double>(),
            j.at("category").get<string>(),
            j.at("last_purchase_date").get<string>(),
            j.at("days_until_runout").get<int>()
        };
    }
};


//NO.2  Grocery Tracker Class

class GroceryTracker {
private:
    map<string, GroceryItem> items;
   fs::path data_dir = fs::current_path() / "data";
   fs::path data_file = data_dir / "groceries.json";

public:
   GroceryTracker() {
    try {
        data_dir = fs::current_path() / "data";

        if (!fs::exists(data_dir)) {
            fs::create_directories(data_dir);
            cout << "[INFO] Created data directory." << endl;
        }

        load_data();
    }
    catch (const exception& e) {
        cerr << "[ERROR] Filesystem error: " << e.what() << endl;
    }
}

    ~GroceryTracker() {
        save_data();
    }

    
    void add_item() {
        GroceryItem item;

        cout << endl << "---{ Add Grocery Item }---" << endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Name: ";
        getline(cin, item.name);

        if (item.name.empty()) {
            cout << "[ERROR] Name cannot be empty." << endl;
            return;
        }

        cout << "Price: $";
        while (!(cin >> item.price) || item.price < 0) {
            cout << "[ERROR] Invalid price. Enter again: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Category: ";
        getline(cin, item.category);

        cout << "Days until runout: ";
        while (!(cin >> item.days_until_runout) || item.days_until_runout < 0) {
            cout << "[ERROR] Invalid number. Enter again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        item.last_purchase_date = get_current_date();
        items[item.name] = item;

        cout << endl << "[SUCCESS] Added: " << item.name << endl;
        save_data();
    }

  
    void view_items() const {
        if (items.empty()) {
            cout << endl << "[INFO] No items tracked yet." << endl;
            return;
        }

        cout << endl << "=={ Grocery Inventory }==" << endl;
        cout << left
            << setw(20) << "Name"
            << setw(12) << "Price"
            << setw(15) << "Category"
            << setw(15) << "Last Purchase"
            << setw(10) << "Days Left"
            << endl;

        cout << string(72, '-') << endl;

        for (const auto& [name, item] : items) {
            cout << setw(20) << item.name
                << "$" << setw(11) << fixed << setprecision(2) << item.price
                << setw(15) << item.category
                << setw(15) << item.last_purchase_date
                << setw(10) << item.days_until_runout
                << endl;
        }
        cout << string(72, '=') << endl;
    }

    
    void delete_item() {
        if (items.empty()) {
            cout << endl << "[INFO] No items to delete." << endl;
            return;
        }

        cout << endl << "Current items:" << endl;
        for (const auto& [name, item] : items) {
            cout << "  - " << name << endl;
        }

        string name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << endl << "Enter item name to delete: ";
        getline(cin, name);

        if (items.erase(name)) {
            cout << "[SUCCESS] Deleted: " << name << endl;
            save_data();
        }
        else {
            cout << "[ERROR] Item not found: " << name << endl;
        }
    }

    
    void budget_report() const {
        if (items.empty()) {
            cout << endl << "[INFO] No items to report." << endl;
            return;
        }

        double total = 0.0;
        map<string, double> category_totals;

        for (const auto& [name, item] : items) {
            total += item.price;
            category_totals[item.category] += item.price;
        }

        cout << endl << "=={ Budget Report }==" << endl;
        cout << fixed << setprecision(2);
        cout << "Total Spending: $" << total << endl << endl;
        cout << "Breakdown by Category:" << endl;

        for (const auto& [category, amount] : category_totals) {
            double percentage = (amount / total) * 100;
            cout << "  " << setw(15) << left << category
                 << " : $" << setw(8) << right << amount
                 << " (" << setw(5) << percentage << "%)" << endl;
        }
        cout << string(35, '=') << endl;
    }

  
    void predict_runout() const {
        cout << endl << "=={ Items Running Out Soon }==" << endl;
        bool found = false;

        for (const auto& [name, item] : items) {
            if (item.days_until_runout < 7) {
                cout << "[WARNING] " << item.name
                     << " - Only " << item.days_until_runout
                     << " day(s) left!" << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "[OK] All items well-stocked!" << endl;
        }
        cout << string(44, '=') << endl;
    }

private:

    void save_data() {
        try {
            json j = json::array();

            for (const auto& [name, item] : items) {
                j.push_back(item.to_json());
            }

            ofstream file(data_file);
            if (!file) {
                cerr << "[ERROR] Could not save data to file." << endl;
                return;
            }

            file << j.dump(4);
            file.close();

            cout << "[SUCCESS] Data saved successfully to " << data_file << endl;
            cout << "[INFO] Total items saved: " << items.size() << endl;
        }
        catch (const exception& e) {
            cerr << "[ERROR] Failed to save: " << e.what() << endl;
        }
    }

    
    void load_data() {
        if (!fs::exists(data_file)) {
            cout << "[INFO] No existing data file. Starting fresh." << endl;
            return;
        }

        ifstream file(data_file);
        if (!file) {
            cerr << "[ERROR] Could not open data file." << endl;
            return;
        }

        try {
            json j;
            file >> j;

            if (!j.is_array()) {
                cout << "[WARNING] Invalid data format." << endl;
                return;
            }

            for (const auto& item_json : j) {
                GroceryItem item = GroceryItem::from_json(item_json);
                items[item.name] = item;
            }

            cout << "[SUCCESS] Loaded " << items.size() << " item(s) from file." << endl;
        }
        catch (const exception& e) {
            cerr << "[ERROR] JSON parsing failed: " << e.what() << endl;
        }
    }


    string get_current_date() const {
        time_t now = time(nullptr);
        tm* local_time = localtime(&now);

        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", local_time);

        return string(buffer);
    }
};


//No.11  Menu
void show_menu() {
    cout << endl;
    cout << "==={ Grocery expenses Tracker }===" << endl;
    cout << "  1. Add Item" << endl;
    cout << "  2. View All Items" << endl;
    cout << "  3. Delete Item" << endl;
    cout << "  4. Budget Report" << endl;
    cout << "  5. Predict Runouts" << endl;
    cout << "  6. Exit" << endl;

    cout << "Enter your choice: ";
}


// Main
int main() 
{
    cout << endl;
    cout << "************************************************" << endl;
    cout << "*     WELCOME TO GROCERY EXPENSE TRACKER       *" << endl;
    cout << "************************************************" << endl;
    cout << endl;

    GroceryTracker tracker;
    int choice;

    while (true) {
        show_menu();
        
        if (!(cin >> choice)) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl << "[ERROR] Invalid input! Please enter a number (1-6)." << endl;

            cout << endl << "Press Enter to continue...";
            cin.get();
            continue;
        }

        switch (choice)
        {
        case 1:
            tracker.add_item();
            break;

        case 2:
            tracker.view_items();
            break;

        case 3:
            tracker.delete_item();
            break;

        case 4:
            tracker.budget_report();
            break;

        case 5:
            tracker.predict_runout();
            break;

        case 6:
            cout << "Thank you! for Using Glitch's Tracker..." << endl;
            cout << endl << "Press Enter to exit...";
            cin.ignore();
            cin.get();
            return 0;

        default:
            cout << endl << "[ERROR] Invalid choice! Please select 1-6." << endl;
        }

        cout << endl << "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}