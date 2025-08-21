//
// Created by Md. Asif Khan on 8/8/25.
//

#pragma once

enum class AdminType {
    Transport,
    HallDining,
    PublicRelations,
    Department,
    SystemAdmin
};

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
