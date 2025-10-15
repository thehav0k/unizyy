#ifndef DESIGNATION_H
#define DESIGNATION_H

enum class designation {
    Professor,
    AssociateProfessor,
    AssistantProfessor,
    Lecturer
};

static inline const char* toString(designation des) {
    switch (des) {
        case designation::Professor: return "Professor";
        case designation::AssociateProfessor: return "Associate Professor";
        case designation::AssistantProfessor: return "Assistant Professor";
        case designation::Lecturer: return "Lecturer";
        default: return "Unknown Rank";
    }
}
static inline const char* toEnum(designation des) {
    return toString(des);
}
#endif //DESIGNATION_H
