#ifndef ADMINTYPE_H
#define ADMINTYPE_H

// AdminType(used in enum defining) different admin role gula represent korte help kore
enum class AdminType {
    Transport,
    HallDining,
    PublicRelations,
    Department,
    SystemAdmin
};

// AdminType ke string e convert korar helper function
static inline const char* toString(AdminType type) {
    switch (type) {
        case AdminType::Transport: return "Transport";
        case AdminType::HallDining: return "Hall Dining";
        case AdminType::PublicRelations: return "Public Relations";
        case AdminType::Department: return "Department";
        case AdminType::SystemAdmin: return "System Admin";
        default: return "Unknown Type";
    }
}

#endif //ADMINTYPE_H
