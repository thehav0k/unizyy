#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

enum class Halls {
    Al_Beruni_Hall,
    Meer_Mosharraf_Hossain_Hall,
    Shaheed_Salam_Barkat_Hall,
    AFM_Kamaluddin_Hall,
    Moulana_Bhasani_Hall,
    Bangabondhu_Sheikh_Majibur_Rahman_Hall,
    Jatiya_Kabi_Kazi_Nazrul_Islam_Hall,
    Rabindra_Nath_Tagore_Hall,
    Shahid_Tajuddin_Ahmed_Hall,
    Shahid_Sheikh_Russel_Hall,
    Shaheed_Rafiq_Jabbar_Hall,
    Nawab_Faizunnesa_Hall,
    Fazilatunnesa_Hall,
    Jahanara_Imam_Hall,
    Preetilata_Hall,
    Begum_Khaleda_Zia_Hall,
    Sheikh_Hasina_Hall,
    Bir_Pratik_Taramon_Bibi_Hall
};

// Convert Halls enum to human-readable string
inline string hallToString(Halls hall) {
    switch (hall) {
        case Halls::Al_Beruni_Hall: return "Al Beruni Hall";
        case Halls::Meer_Mosharraf_Hossain_Hall: return "Meer Mosharraf Hossain Hall";
        case Halls::Shaheed_Salam_Barkat_Hall: return "Shaheed Salam Barkat Hall";
        case Halls::AFM_Kamaluddin_Hall: return "AFM Kamaluddin Hall";
        case Halls::Moulana_Bhasani_Hall: return "Moulana Bhasani Hall";
        case Halls::Bangabondhu_Sheikh_Majibur_Rahman_Hall: return "Bangabondhu Sheikh Majibur Rahman Hall";
        case Halls::Jatiya_Kabi_Kazi_Nazrul_Islam_Hall: return "Jatiya Kabi Kazi Nazrul Islam Hall";
        case Halls::Rabindra_Nath_Tagore_Hall: return "Rabindra Nath Tagore Hall";
        case Halls::Shahid_Tajuddin_Ahmed_Hall: return "Shahid Tajuddin Ahmed Hall";
        case Halls::Shahid_Sheikh_Russel_Hall: return "Shahid Sheikh Russel Hall";
        case Halls::Shaheed_Rafiq_Jabbar_Hall: return "Shaheed Rafiq Jabbar Hall";
        case Halls::Nawab_Faizunnesa_Hall: return "Nawab Faizunnesa Hall";
        case Halls::Fazilatunnesa_Hall: return "Fazilatunnesa Hall";
        case Halls::Jahanara_Imam_Hall: return "Jahanara Imam Hall";
        case Halls::Preetilata_Hall: return "Preetilata Hall";
        case Halls::Begum_Khaleda_Zia_Hall: return "Begum Khaleda Zia Hall";
        case Halls::Sheikh_Hasina_Hall: return "Sheikh Hasina Hall";
        case Halls::Bir_Pratik_Taramon_Bibi_Hall: return "Bir Pratik Taramon Bibi Hall";
        default: return "Unknown Hall";
    }
}

// Convert string to Halls enum
inline Halls stringToHalls(const string& hallStr) {
    string normalized = hallStr;
    replace(normalized.begin(), normalized.end(), '_', ' ');
    // Allow both exact with spaces and underscore version
    if (normalized == "Al Beruni Hall") return Halls::Al_Beruni_Hall;
    if (normalized == "Meer Mosharraf Hossain Hall") return Halls::Meer_Mosharraf_Hossain_Hall;
    if (normalized == "Shaheed Salam Barkat Hall") return Halls::Shaheed_Salam_Barkat_Hall;
    if (normalized == "AFM Kamaluddin Hall") return Halls::AFM_Kamaluddin_Hall;
    if (normalized == "Moulana Bhasani Hall") return Halls::Moulana_Bhasani_Hall;
    if (normalized == "Bangabondhu Sheikh Majibur Rahman Hall") return Halls::Bangabondhu_Sheikh_Majibur_Rahman_Hall;
    if (normalized == "Jatiya Kabi Kazi Nazrul Islam Hall") return Halls::Jatiya_Kabi_Kazi_Nazrul_Islam_Hall;
    if (normalized == "Rabindra Nath Tagore Hall") return Halls::Rabindra_Nath_Tagore_Hall;
    if (normalized == "Shahid Tajuddin Ahmed Hall") return Halls::Shahid_Tajuddin_Ahmed_Hall;
    if (normalized == "Shahid Sheikh Russel Hall") return Halls::Shahid_Sheikh_Russel_Hall;
    if (normalized == "Shaheed Rafiq Jabbar Hall") return Halls::Shaheed_Rafiq_Jabbar_Hall;
    if (normalized == "Nawab Faizunnesa Hall") return Halls::Nawab_Faizunnesa_Hall;
    if (normalized == "Fazilatunnesa Hall") return Halls::Fazilatunnesa_Hall;
    if (normalized == "Jahanara Imam Hall") return Halls::Jahanara_Imam_Hall;
    if (normalized == "Preetilata Hall") return Halls::Preetilata_Hall;
    if (normalized == "Begum Khaleda Zia Hall") return Halls::Begum_Khaleda_Zia_Hall;
    if (normalized == "Sheikh Hasina Hall") return Halls::Sheikh_Hasina_Hall;
    if (normalized == "Bir Pratik Taramon Bibi Hall") return Halls::Bir_Pratik_Taramon_Bibi_Hall;
    return Halls::Al_Beruni_Hall; // Default
}

// Check if a hall is for male students (1 for male, 0 for female)
inline int getHallGender(Halls hall) {
    switch (hall) {
        case Halls::Al_Beruni_Hall:
        case Halls::Meer_Mosharraf_Hossain_Hall:
        case Halls::Shaheed_Salam_Barkat_Hall:
        case Halls::AFM_Kamaluddin_Hall:
        case Halls::Moulana_Bhasani_Hall:
        case Halls::Bangabondhu_Sheikh_Majibur_Rahman_Hall:
        case Halls::Jatiya_Kabi_Kazi_Nazrul_Islam_Hall:
        case Halls::Rabindra_Nath_Tagore_Hall:
        case Halls::Shahid_Tajuddin_Ahmed_Hall:
        case Halls::Shahid_Sheikh_Russel_Hall:
        case Halls::Shaheed_Rafiq_Jabbar_Hall:
            return 1; // Male halls
        case Halls::Nawab_Faizunnesa_Hall:
        case Halls::Fazilatunnesa_Hall:
        case Halls::Jahanara_Imam_Hall:
        case Halls::Preetilata_Hall:
        case Halls::Begum_Khaleda_Zia_Hall:
        case Halls::Sheikh_Hasina_Hall:
        case Halls::Bir_Pratik_Taramon_Bibi_Hall:
            return 0; // Female halls
        default:
            return 1; // Default to male
    }
}