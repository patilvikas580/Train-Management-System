#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip> // For better formatting

using namespace std;

class TourBus {
    int bid;
    char destination[200];
    char time[50];
    int max_seats;
    int booked;
    int fare;

public:
    TourBus() {
        bid = 0;
        max_seats = 50;
        booked = 0;
        fare = 0;
        strcpy(time, "9:10am");
        strcpy(destination, "");
    }

    void input();
    void show();
    void display();
    int getid() { return bid; }
    void book();
    char* getDestination() { return destination; }
    char* getTime() { return time; }
    int getBooked() { return booked; }
    int getMax() { return max_seats; }
    int getFare() { return fare; }
};

class Ticket {
    char name[50];
    TourBus bus;

public:
    void generate(char cname[], TourBus tb) {
        strcpy(name, cname);
        bus = tb;
    }

    void display() {
        cout << "Customer Name: " << name << endl;
        cout << "Details of Bus: " << endl;
        bus.show();
    }
};



void TourBus::input() {
    cout << "Enter bus id: ";
    cin >> bid;
    cin.ignore(); 
    cout << "Enter bus destination: ";
    cin.getline(destination, 200);
    cout << "Enter time of bus: ";
    cin.getline(time, 50);
    cout << "Enter fare of the bus: ";
    cin >> fare;
}

void TourBus::display() 
{
    cout << left << setw(8) << bid
         << setw(20) << destination
         << setw(10) << time
         << setw(10) << max_seats
         << setw(10) << booked
         << setw(10) << fare
         << endl;
}

void TourBus::show() 
{
    cout << "Bus Id: " << bid << endl;
    cout << "Destination: " << destination << endl;
    cout << "Time: " << time << endl;
    cout << "Seats Available: " << (max_seats - booked) << endl;
    cout << "Fare: " << fare << endl;
}

void TourBus::book() 
{
    if (booked < max_seats) 
    {
        booked++;
    } 
    else 
    {
        cout << "Sorry, no seats available!" << endl;
    }
}
int main() {
    int ch;
    fstream F, G;
    TourBus b;

    do {
        cout << "\n********* Tour Bus Booking System *********" << endl;
        cout << "1. Add a New Tour Bus" << endl;
        cout << "2. Show Selected Bus" << endl;
        cout << "3. Display All Buses" << endl;
        cout << "4. Delete a Bus" << endl;
        cout << "5. Book a Ticket" << endl;
        cout << "6. Display Booked Tickets" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch) {
            case 1:
                F.open("tour.dat", ios::app | ios::binary);
                b.input();
                F.write((char*)&b, sizeof(b));
                F.close();
                cout << "Bus added Successfully!" << endl;
                break;

            case 2: {
                int id, chk = 0;
                cout << "Enter the bus id to be displayed: ";
                cin >> id;
                F.open("tour.dat", ios::in | ios::binary);

                if (F.fail()) {
                    cout << "Can't open file!" << endl;
                } else {
                    while (F.read((char*)&b, sizeof(b))) {
                        if (b.getid() == id) {
                            b.show();
                            chk = 1;
                            break;
                        }
                    }
                    if (chk == 0)
                        cout << "Bus not found!" << endl;
                }
                F.close();
                break;
            }

            case 3:
                F.open("tour.dat", ios::in | ios::binary);
                if (F.fail()) {
                    cout << "Can't open file!" << endl;
                } else {
                    cout << left << setw(8) << "ID"
                         << setw(20) << "Destination"
                         << setw(10) << "Time"
                         << setw(10) << "Seats"
                         << setw(10) << "Booked"
                         << setw(10) << "Fare"
                         << endl;
                    cout << string(68, '-') << endl;
                    while (F.read((char*)&b, sizeof(b)))
                        b.display();
                }
                F.close();
                break;

            case 4: {
                int id, chk = 0;
                cout << "Enter the bus id to be deleted: ";
                cin >> id;
                F.open("tour.dat", ios::in | ios::binary);
                G.open("temp.dat", ios::out | ios::binary);

                if (F.fail()) {
                    cout << "Can't open file!" << endl;
                } else {
                    while (F.read((char*)&b, sizeof(b))) {
                        if (b.getid() != id) {
                            G.write((char*)&b, sizeof(b));
                        } else {
                            b.show();
                            chk = 1;
                        }
                    }
                    if (chk == 0)
                        cout << "Bus not found!" << endl;
                    else
                        cout << "Bus deleted!" << endl;
                }
                F.close();
                G.close();
                remove("tour.dat");
                rename("temp.dat", "tour.dat");
                break;
            }

            case 5: {
                char dest[70], cname[50];
                Ticket t;
                int chk = 0;

                cout << "Enter the destination: ";
                cin.getline(dest, 70);

                F.open("tour.dat", ios::in | ios::out | ios::binary);
                if (F.fail()) {
                    cout << "Can't open file!" << endl;
                } else {
                    while (F.read((char*)&b, sizeof(b))) {
                        if (strcmp(b.getDestination(), dest) == 0) {
                            b.show();
                            chk = 1;

                            cout << "Enter the customer name: ";
                            cin.getline(cname, 50);

                            b.book();
                            t.generate(cname, b);

                            G.open("tickets.dat", ios::app | ios::binary);
                            G.write((char*)&t, sizeof(t));
                            G.close();

                            F.seekp(-sizeof(b), ios::cur);
                            F.write((char*)&b, sizeof(b));

                            cout << "Ticket booked successfully!" << endl;
                            break;
                        }
                    }
                    if (chk == 0)
                        cout << "No bus found!" << endl;
                }
                F.close();
                break;
            }

            case 6:
                cout << "Booked Tickets:\n";
                G.open("tickets.dat", ios::in | ios::binary);
                if (G.fail()) {
                    cout << "Can't open file!" << endl;
                } else {
                    Ticket t;
                    while (G.read((char*)&t, sizeof(t)))
                        t.display();
                }
                G.close();
                break;

            case 7:
                cout << "Exiting program..." << endl;
                break;

            default:
                cout << "Invalid choice! Try again." << endl;
        }
    } while (ch != 7);
}
