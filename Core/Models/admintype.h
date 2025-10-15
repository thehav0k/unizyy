#ifndef ADMINTYPE_H
#define ADMINTYPE_H

//Eta ekta enum class ja diye fixed admin type gulo define kora hoy.(11)
enum class AdminType {
    Transport,
    HallDining,
    PublicRelations,
    SystemAdmin
};

// AdminType ke string e convert korar helper function
static inline const char* toString(AdminType type) {
    switch (type) {
        case AdminType::Transport: return "Transport";
        case AdminType::HallDining: return "Hall Dining";
        case AdminType::PublicRelations: return "Public Relations";
        case AdminType::SystemAdmin: return "System Admin";
        default: return "Unknown Type";
    }
}

#endif //ADMINTYPE_H
