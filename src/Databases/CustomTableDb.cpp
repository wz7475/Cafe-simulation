#include "../../includes/model/Databases/CustomTableDb.h"

void CustomTableDb::AdvanceStateAllTables() {
    for (auto &table : this->items){
        table->AdvanceStateAll();
    }
}

bool CustomTableDb::TryAssignCustomersGroup(CustomersGroup customersGroup) {
    for (auto &table : this->items){
        if (table->TryAddCustomers(customersGroup)){
            return true;
        }
    }
    return false;
}
