#include <iostream>
using namespace std;

struct Data {
    int met;
    int men;
    int dien;
};

constexpr bool leap(int met) {
    return met % 400 == 0 || met % 4 == 0 && met % 100 != 0;
}

constexpr int men_dien(int men, bool leap_year) {
    int r = 0;
    for (int m = 1; m < men; ++m) {
        switch (m) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            r += 31;
            break;
        case 2:
            r += 28 + leap_year;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            r += 30;
            break;
        default:
            return 0;
        }
    }
    return r;
}

long data_skrt_d(const Data d1, const Data d2) {
    long r = 0;
// 1 variantas su ciklais:
   for (int met = d1.met; met < d2.met; ++met) {
      r += 365 + leap(met);
  }
  r += men_dien(d2.men, leap(d2.met));
  r -= men_dien(d1.men, leap(d1.met));
  r += (d2.dien - d1.dien);

// 2 variantas per matematinius skaičiavimus:
//    int met_skirt = d2.met - d1.met;
//    long met_skirt_d = 365 * met_skirt + (303 + 97 * (met_skirt) - (97 * d1.met) % 400) / 400;
//    r = met_skirt_d + men_dien(d2.men, leap(d2.met)) - men_dien(d1.men, leap(d1.met)) + d2.dien - d1.dien;
    return r;
}

int main() {
    Data d1{}, d2{};
    cout << "Iveskite pirmaja data: " << endl;
    cin >> d1.met >> d1.men >> d1.dien;
    cout << "Iveskite antraja data: " << endl;
    cin >> d2.met >> d2.men >> d2.dien;
    cout << "Dienu skirtumas tarp datu: " << data_skrt_d(d1, d2) << endl;
    return 0;
}
