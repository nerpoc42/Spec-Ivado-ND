#include <iostream>
#include <fstream>
#include <algorithm>

const char *const IN1_F = "in1.txt";
const char *const IN2_F = "in2.txt";
const char *const OUT_F = "rez.txt";

//////////////////////////////////////////////////////////////////////

template <class T>
struct Matrica {
    size_t eil;
    size_t stulp;
    T *sk;

    Matrica() : eil(0), stulp(0), sk(nullptr) {}

    Matrica(size_t eil, size_t stulp) : eil(eil), stulp(stulp), sk(new T[eil*stulp]) {}

    Matrica(const Matrica<T>& other) {
        eil = other.eil;
        stulp = other.stulp;
        sk = new T[eil * stulp];

        for (size_t i = 0; i != eil * stulp; ++i) {
            sk[i] = other[i];
        }
    }

    Matrica& operator=(const Matrica<T>& other) {
        if(&other != this) {
            delete[] sk;
            eil = other.eil;
            stulp = other.stulp;
            sk = new T[eil * stulp];

            for (size_t i = 0; i != eil * stulp; ++i) {
                sk[i] = other[i];
            }
        }
        return *this;
    }

    T &operator[](size_t index) const { return sk[index]; }

    ~Matrica() { delete[] sk; sk = nullptr; }

    Matrica operator+(const Matrica &other) const {
        if (eil != other.eil || stulp != other.stulp)
            throw std::runtime_error("Negalima sudeti nevienodo dydzio matricu");

        Matrica rez(eil, stulp);

        for (size_t i = 0; i != eil * stulp; ++i)
            rez[i] = sk[i] + other[i];

        return rez;
    }

    Matrica operator-(const Matrica &other) const {
        if (eil != other.eil || stulp != other.stulp)
            throw std::runtime_error("Negalima atimti nevienodo dydzio matricu");

        Matrica rez(eil, stulp);

        for (size_t i = 0; i != eil * stulp; ++i)
            rez[i] = sk[i] - other[i];

        return rez;
    }

    Matrica operator*(const Matrica &other) const {
        if (eil != other.stulp || stulp != other.eil)
            throw std::runtime_error("Pirmos matricos eilutes turi sutikti su antros stulpeliais ir atvirskciai");

        Matrica rez(eil, other.stulp);

        for (size_t y = 0; y != eil; ++y) {
            for (size_t x = 0; x != other.stulp; ++x) {
                T elem = 0;
                for (size_t z = 0; z != stulp; ++z) {
                    elem += (sk[y * stulp + z] * other[z * other.stulp + x]);
                }
                rez[y * rez.stulp + x] = elem;
            }
        }

        return rez;
    }

    Matrica transponuoti() {
        Matrica rez = *this;
        for (size_t y = 0; y < eil / 2; ++y) {
            for (size_t x = 0; x < stulp / 2; ++x) {
                std::swap(rez[y*stulp+x], rez[(eil-y-1)*stulp+(stulp-x-1)]);
            }
        }
        return rez;
    }

    size_t max_elem(size_t* arr) {
       size_t kiek = 0;
       size_t max = 0;

       for (size_t i = 0; i != stulp*eil; ++i) {
           if (sk[i] == sk[max]) {
               arr[(kiek++)] = i;
           } else if (sk[i] > sk[max]) {
               kiek = 0;
               // Nereikia valyti masyvo
               arr[(kiek++)] = i;
           }
       }

       return kiek;
    }
};

template <class T>
std::istream &operator>>(std::istream &is, Matrica<T> &m) {
    is >> m.eil >> m.stulp;

    delete[] m.sk;

    m.sk = new T[m.eil * m.stulp];

    for (size_t i = 0; i < m.eil * m.stulp; ++i) {
        is >> m.sk[i];
    }

    return is;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const Matrica<T> &m) {
    for (size_t y = 0; y < m.eil; ++y) {
        for (size_t x = 0; x < m.stulp; ++x) {
            os << m.sk[y * m.stulp + x] << " ";
        }
        os << std::endl;
    }
    return os;
}

//////////////////////////////////////////////////////////////////////


int main() {
    std::ifstream in1(IN1_F);
    std::ifstream in2(IN2_F);
    std::ofstream out(OUT_F);

    if (!in1) {
        std::cerr << "Nepavyko atidaryti failo " << IN1_F << std::endl;
        return 1;
    }

    if (!in2) {
        std::cerr << "Nepavyko atidaryti failo " << IN2_F << std::endl;
        return 1;
    }

    if (!out) {
        std::cerr << "Nepavyko atidaryti failo " << OUT_F << std::endl;
        return 1;
    }

    Matrica<long double> m1, m2, trans1, trans2, rez;
    in1 >> m1;
    in2 >> m2;

    trans1 = m1.transponuoti();
    trans2 = m2.transponuoti();

    std::cout << "Pirma transponuota matrica = " << std::endl << trans1 << std::endl;
    out << "Pirma transponuota matrica = " << std::endl << trans1 << std::endl;
    std::cout << "Antra transponuotra matrica = " << std::endl << trans2 << std::endl;
    out << "Antra transponuotra matrica = " << std::endl << trans2 << std::endl;

    auto* idx1 = new size_t[m1.eil*m1.stulp];
    auto* idx2 = new size_t[m2.eil*m2.stulp];
    size_t k1, k2;

    k1 = m1.max_elem(idx1);
    k2 = m2.max_elem(idx2);

    // Netikrina ar tuščia matrica
    std::cout << "Max pirmos matricos: " << m1[idx1[0]] << ", indeksai: " << std::endl;
    out << "Max pirmos matricos: " << m1[idx1[0]] << ", indeksai: " << std::endl;
    for (size_t i = 0; i < k1; ++i) {
        std::cout << idx1[i] << " ";
        out << idx1[i] << " ";
    }
    std::cout << std::endl << std::endl;
    out << std::endl << std::endl;

    std::cout << "Max antros matricos: " << m2[idx2[0]] << ", indeksai: " << std::endl;
    out << "Max pirmos matricos: " << m2[idx2[0]] << ", indeksai: " << std::endl;
    for (size_t i = 0; i < k2; ++i) {
        std::cout << idx2[i] << " ";
        out << idx2[i] << " ";
    }
    std::cout << std::endl << std::endl;
    out << std::endl << std::endl;

    std::cout << "Pirma transponuota matrica = " << std::endl << trans1 << std::endl;
    out << "Pirma transponuota matrica = " << std::endl << trans1 << std::endl;
    std::cout << "Antra transponuotra matrica = " << std::endl << trans2 << std::endl;
    out << "Antra transponuotra matrica = " << std::endl << trans2 << std::endl;

    try {
        rez = m1 + m2;
        std::cout << "Sudetis = " << std::endl << rez << std::endl;
        out << "Sudetis = " << std::endl << rez << std::endl;
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        out << exc.what() << std::endl;
    }

    try {
        rez = m1 - m2;
        std::cout << "Atimtis = " << std::endl << rez << std::endl;
        out << "Atimtis = " << std::endl << rez << std::endl;
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        out << exc.what() << std::endl;
    }

    try {
        rez = m1 * m2;
        std::cout << "Daugyba = " << std::endl << rez << std::endl;
        out << "Daugyba = " << std::endl << rez << std::endl;
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        out << exc.what() << std::endl;
    }

    return 0;
}
