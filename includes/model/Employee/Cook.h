#ifndef COOK_H
#define COOK_H

#include "Employee.h"
#include "../MenuItem/menuItem.h"
#include <string>

class Cook : public Employee {
public:
//    io methods
    std::istream &read(std::istream &in);
    std::ostream &write(std::ostream &os) const noexcept;

//    printProperties method for db listing
    string printProperties() const noexcept;
//    method for logs in simulation
    string printStateLog() const;

//    cook properties
    enum CookState {
        free = 0, busy
    };
    void setAssignedMenuItem(unique_ptr<MenuItem> newAssignedMenuItem);

//  constructors, operators ...
    Cook();
    Cook(int id, const string &name, const string &surname, int gender, Price baseSalary,
         unsigned baseAmountOfShifts, unsigned int knownCuisines);
    bool operator==(const Cook &rhs) const;
    bool operator!=(const Cook &rhs) const;


// legacy methods
    int get_known_cuisines() const;
    void set_known_cuisines(int knownCuisines);
    Price calculate_salary() const noexcept;
    unsigned int calculate_shifts_per_week() const noexcept;



private:
    unsigned known_cuisines;
    std::unique_ptr<MenuItem> assignedMenuItem;



};


#endif //COOK_H