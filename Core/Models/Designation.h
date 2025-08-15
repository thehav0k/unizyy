//
// Created by Md. Asif Khan on 8/8/25.
//

#ifndef DESIGNATION_H
#define DESIGNATION_H

enum class designation {
    Professor,
    AssociateProfessor,
    AssistantProfessor,
    Lecturer
};

static inline const char* toString(designation AP) {
    switch (AP) {
        case designation::Professor: return "Professor";
        case designation::AssociateProfessor: return "Associate Professor";
        case designation::AssistantProfessor: return "Assistant Professor";
        case designation::Lecturer: return "Lecturer";
        default: return "Unknown Rank";
    }
}
static inline const char* toEnum(designation AP) {
    return toString(AP);
}
#endif //DESIGNATION_H
