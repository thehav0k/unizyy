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