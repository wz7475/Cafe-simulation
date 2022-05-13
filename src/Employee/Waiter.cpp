#include "../../includes/model/Employee/Waiter.h"
#include "../../includes/model/Employee/Exceptions.h"

Waiter::Waiter(int id, const string &name, const string &surname, int gender, Price baseSalary,
               unsigned int baseAmountOfShifts, bool canServeAlcohol)
        : Worker(id, name, surname, gender, baseSalary, baseAmountOfShifts),
          can_serve_alcohol(canServeAlcohol) {}

Price Waiter::calculate_salary() const noexcept {
    return (can_serve_alcohol) ? base_salary * 1.1 * 1.05 : base_salary * 1.1;
}

bool Waiter::is_can_serve_alcohol() const {
    return can_serve_alcohol;
}

void Waiter::set_can_serve_alcohol(bool canServeAlcohol) {
    can_serve_alcohol = canServeAlcohol;
}

unsigned int Waiter::calculate_shifts_per_week() const noexcept {
    return (can_serve_alcohol) ? base_amount_of_shifts + 1 : base_amount_of_shifts;
}

std::ostream &Waiter::write(std::ostream &os) const noexcept{
    Worker::write(os);
    os << " " << can_serve_alcohol;
    return os;
}



string Waiter::print() const noexcept {
    return Worker::print() + "whether can serve alcohol: " + std::to_string
            (can_serve_alcohol) + "\n";
}

std::istream &Waiter::read(std::istream &in) {
    Worker::read(in);
    in >> can_serve_alcohol;
    return in;
}

Waiter::Waiter() {

}

bool Waiter::operator==(const Waiter &rhs) const {
    return static_cast<const Worker &>(*this) == static_cast<const Worker &>(rhs) &&
           can_serve_alcohol == rhs.can_serve_alcohol;
}

bool Waiter::operator!=(const Waiter &rhs) const {
    return !(rhs == *this);
}

string Waiter::printState() const{
    string msg = "waiter " + std::to_string(id) + " - ";
    switch (state) {
        case WaiterState::awaiting:
            return msg + "awaiting\n";
        case WaiterState::giveMenu:
            return msg + "giving cards\n";
        case WaiterState::collectOrder:
            return msg + "collecting orders\n";
        case WaiterState::handInOrder:
            return msg + "handing in orders\n";
        case WaiterState::takeReceipt:
            return msg + "taking receipt\n";
        default:
            throw StateException(state);
    }
}

