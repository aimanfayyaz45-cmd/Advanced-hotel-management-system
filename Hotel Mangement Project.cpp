#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

// Colors for the Terminal
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

using namespace std;

// ================= ROOM CLASS =================
class Room {
public:
    int roomId;
    string type;
    double price;
    bool isAvailable;
    string ac;
    string comfort;
    string size;

    Room() {}
    Room(int id, string t, double p, string a, string c, string s) {
        roomId = id; type = t; price = p; ac = a; comfort = c; size = s;
        isAvailable = true;
    }

    void display() {
        cout << left << "Room ID: " << setw(4) << roomId 
             << " | Type: " << setw(10) << type 
             << " | AC: " << setw(8) << ac 
             << " | Price: " << setw(6) << price 
             << " | Status: " << (isAvailable ? GREEN "Available" RESET : RED "Booked" RESET) 
             << endl;
    }
};

// ================= CUSTOMER CLASS =================
class Customer {
public:
    int customerId;
    string name;
    string cnic;
    string phone;
    int roomId;
    int days;
    double roomBill;
    double foodBill;

    void display() {
        cout << fixed << setprecision(0);
        cout << "Cust ID: " << setw(3) << customerId 
             << " | Name: " << setw(15) << name 
             << " | Room: " << setw(3) << roomId 
             << " | Total Bill: Rs." << (roomBill + foodBill) << endl;
    }
};

// ================= FOOD CLASS =================
class Food {
public:
    int id;
    string name;
    double price;

    Food(int i, string n, double p) : id(i), name(n), price(p) {}
};

// ================= MAIN SYSTEM CLASS =================
class HotelSystem {
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Food> menu;
    int nextCustomerId = 1;

public:
    HotelSystem() {
        loadData();
        if (rooms.empty()) initRooms();
        initMenu();
    }

    ~HotelSystem() {
        saveData();
    }

    void initRooms() {
        rooms.push_back(Room(1, "Single", 1000, "Non-AC", "Standard", "Small"));
        rooms.push_back(Room(2, "Single", 1200, "AC", "Standard", "Small"));
        rooms.push_back(Room(3, "Double", 2000, "AC", "Luxury", "Big"));
        rooms.push_back(Room(4, "Suite", 5000, "AC", "Royal", "Big"));
    }

    void initMenu() {
        menu.push_back(Food(1, "Biryani", 300));
        menu.push_back(Food(2, "Burger", 250));
        menu.push_back(Food(3, "Tea", 50));
    }

    // --- SECURITY FUNCTIONS ---
    bool verifyAdmin() {
        string user;
        int pass;
        cout << YELLOW << "\n--- ADMIN LOGIN ---" << RESET << endl;
        cout << "Enter Username: "; cin >> user;
        cout << "Enter Password: "; cin >> pass;

        if ((user == "admin" && pass == 1234) || (user == "staff" && pass == 5621)) {
            cout << GREEN << "Access Granted!\n" << RESET;
            return true;
        }
        cout << RED << "Invalid Admin Credentials!\n" << RESET;
        return false;
    }

    bool verifyUser() {
        int pass;
        cout << CYAN << "\n--- USER ACCESS ---" << RESET << endl;
        cout << "Enter User Password: "; cin >> pass;
        if (pass == 5678) return true;
        cout << RED << "Wrong User Password!\n" << RESET;
        return false;
    }

    // --- FILE HANDLING ---
    void saveData() {
        ofstream rFile("rooms.txt");
        for (auto &r : rooms)
            rFile << r.roomId << " " << r.type << " " << r.price << " " << r.isAvailable << " " 
                  << r.ac << " " << r.comfort << " " << r.size << endl;

        ofstream cFile("customers.txt");
        for (auto &c : customers)
            cFile << c.customerId << " " << c.name << " " << c.cnic << " " << c.phone << " " 
                  << c.roomId << " " << c.days << " " << c.roomBill << " " << c.foodBill << endl;
    }

    void loadData() {
        ifstream rFile("rooms.txt");
        Room r;
        while (rFile >> r.roomId >> r.type >> r.price >> r.isAvailable >> r.ac >> r.comfort >> r.size)
            rooms.push_back(r);

        ifstream cFile("customers.txt");
        Customer c;
        while (cFile >> c.customerId >> c.name >> c.cnic >> c.phone >> c.roomId >> c.days >> c.roomBill >> c.foodBill) {
            customers.push_back(c);
            if (c.customerId >= nextCustomerId) nextCustomerId = c.customerId + 1;
        }
    }

    // --- PANELS ---
    void adminPanel() {
        if (!verifyAdmin()) return;
        int ch;
        do {
            cout << BLUE << "\n--- ADMIN DASHBOARD ---" << RESET << endl;
            cout << "1. View All Rooms\n2. View All Customers\n3. Revenue Report\n4. Exit Admin\nChoice: ";
            cin >> ch;
            if (ch == 1) for (auto &r : rooms) r.display();
            else if (ch == 2) for (auto &c : customers) c.display();
            else if (ch == 3) report();
        } while (ch != 4);
    }

    void userPanel() {
        if (!verifyUser()) return;
        int ch;
        do {
            cout << CYAN << "\n--- USER DASHBOARD ---" << RESET << endl;
            cout << "1. Available Rooms\n2. Check-In\n3. Order Food\n4. Check-Out\n5. Exit User\nChoice: ";
            cin >> ch;
            if (ch == 1) showAvailable();
            else if (ch == 2) checkIn();
            else if (ch == 3) orderFood();
            else if (ch == 4) checkOut();
        } while (ch != 5);
    }

    void showAvailable() {
        for (auto &r : rooms) if (r.isAvailable) r.display();
    }

    void checkIn() {
        int rId;
        showAvailable();
        cout << "Enter Room ID: "; cin >> rId;
        for (auto &r : rooms) {
            if (r.roomId == rId && r.isAvailable) {
                Customer c;
                c.customerId = nextCustomerId++;
                cout << "Customer Name (NoSpaces): "; cin >> c.name;
                cout << "CNIC: "; cin >> c.cnic;
                cout << "Phone: "; cin >> c.phone;
                cout << "Days: "; cin >> c.days;
                c.roomId = rId;
                c.roomBill = r.price * c.days;
                c.foodBill = 0;
                r.isAvailable = false;
                customers.push_back(c);
                cout << GREEN << "Check-In Successful!\n" << RESET;
                saveData();
                return;
            }
        }
        cout << RED << "Room not available!\n" << RESET;
    }

    void orderFood() {
        int cId;
        cout << "Enter Customer ID: "; cin >> cId;
        for (auto &c : customers) {
            if (c.customerId == cId) {
                for (auto &f : menu) cout << f.id << ". " << f.name << " (Rs." << f.price << ")\n";
                int choice, qty;
                cout << "Select Item ID: "; cin >> choice;
                cout << "Quantity: "; cin >> qty;
                for (auto &f : menu) {
                    if (f.id == choice) {
                        c.foodBill += (f.price * qty);
                        cout << GREEN << "Food Ordered!\n" << RESET;
                        return;
                    }
                }
            }
        }
        cout << RED << "Customer not found!\n" << RESET;
    }

    void checkOut() {
        int cId;
        cout << "Enter Customer ID: "; cin >> cId;
        for (int i = 0; i < customers.size(); i++) {
            if (customers[i].customerId == cId) {
                for (auto &r : rooms) if (r.roomId == customers[i].roomId) r.isAvailable = true;
                cout << GREEN << "Final Bill: Rs." << (customers[i].roomBill + customers[i].foodBill) << RESET << endl;
                customers.erase(customers.begin() + i);
                saveData();
                return;
            }
        }
        cout << RED << "Invalid Customer ID!\n" << RESET;
    }

    void report() {
        double total = 0;
        for (auto &c : customers) total += (c.roomBill + c.foodBill);
        cout << GREEN << "Total Current Revenue: Rs." << total << RESET << endl;
    }

    void showMainMenu() {
        cout << YELLOW << "\n=================================" << RESET << endl;
        cout << "     HOTEL MANAGEMENT SYSTEM" << endl;
        cout << YELLOW << "=================================" << RESET << endl;
        cout << "1. Admin Panel\n2. User Panel\n3. Exit\nChoice: ";
    }
};

// ================= MAIN FUNCTION =================
int main() {
    HotelSystem hotel;
    int choice;

    do {
        hotel.showMainMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) hotel.adminPanel();
        else if (choice == 2) hotel.userPanel();

    } while (choice != 3);

    cout << "\n" << GREEN << "=========================================" << RESET;
    cout << "\n" << GREEN << "  THANK YOU FOR USING THE HOTEL SYSTEM!  " << RESET;
    cout << "\n" << GREEN << "=========================================" << RESET << endl;

    return 0;
}