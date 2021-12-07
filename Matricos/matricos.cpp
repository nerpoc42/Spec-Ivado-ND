#include <iostream>
#include <fstream>
#include <stdexcept>


//////////////////////////////////////////////////////////////////////

template <class T>
struct Matrica {
    size_t eil;
    size_t stulp;
    T *sk;

    Matrica() : eil(0), stulp(0), sk(nullptr) {}

    Matrica(size_t eil, size_t stulp) : eil(eil), stulp(stulp), sk(new T[eil*stulp]) {}

    T &operator[](size_t index) const { return this->sk[index]; }

    ~Matrica() { delete[] this->sk; }

    Matrica& operator=(const Matrica& other) {
        if (&other != this) {
            delete[] this->sk;
            this->eil = other.eil;
            this->stulp = other.stulp;
            this->sk = new T[this->eil * this->stulp];

            for (size_t i = 0; i != this->eil * this->stulp; ++i) {
                this->sk[i] = other[i];
            }
        }
        return *this;
    }

    Matrica operator+(const Matrica &other) const {
        if (this->eil != other.eil || this->stulp != other.stulp)
            throw std::runtime_error("Negalima sudeti nevienodo dydzio matricu");

        Matrica rez(other.eil, other.stulp);

        for (size_t i = 0; i != this->eil * this->stulp; ++i)
            rez[i] = this->sk[i] + other[i];

        return rez;
    }

    Matrica operator-(const Matrica &other) const {
        if (this->eil != other.eil || this->stulp != other.stulp)
            throw std::runtime_error("Negalima atimti nevienodo dydzio matricu");

        Matrica rez(other.eil, other.stulp);

        for (size_t i = 0; i != this->eil * this->stulp; ++i)
            rez[i] = this->sk[i] - other[i];

        return rez;
    }

    Matrica operator*(const Matrica &other) const {
        if (this->eil != other.stulp || this->stulp != other.eil)
            throw std::runtime_error("Pirmos matricos eilutes turi sutikti su antros stulpeliais ir atvirskciai");

        Matrica rez(eil, other.stulp);

        for (size_t y = 0; y != this->eil; ++y) {
            for (size_t x = 0; x != other.stulp; ++x) {
                T elem = 0;
                for (size_t z = 0; z != this->stulp; ++z) {
                    elem += (this->sk[y * this->stulp + z] * other[z * other.stulp + x]);
                }
                rez[y * rez.stulp + x] = elem;
            }
        }

        return rez;
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

const char *const IN1_F = "in1.txt";
const char *const IN2_F = "in2.txt";
const char *const OUT_F = "rez.txt";

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

    Matrica<long double> m1, m2, rez;
    in1 >> m1;
    in2 >> m2;

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
