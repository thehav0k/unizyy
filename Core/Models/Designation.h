//
// Created by Md. Asif Khan on 8/8/25.
//

#ifndef DESIGNATION_H
#define DESIGNATION_H

enum class AcademicPosition {
    Professor,
    AssociateProfessor,
    AssistantProfessor,
    Lecturer
};

static inline const char* toString(AcademicPosition AP) {
    switch (AP) {
        case AcademicPosition::Professor: return "Professor";
        case AcademicPosition::AssociateProfessor: return "Associate Professor";
        case AcademicPosition::AssistantProfessor: return "Assistant Professor";
        case AcademicPosition::Lecturer: return "Lecturer";
        default: return "Unknown Rank";
    }
}
static inline const char* toEnum(AcademicPosition AP) {
    return toString(AP);
}
#endif //DESIGNATION_H
