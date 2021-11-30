#include <iostream>
#include <fstream>
#include <cmath>

struct Pos {
	float x;
	float y;
	float d;
};

int main() {
	std::ios::sync_with_stdio(false);

	std::ifstream in;
	std::ofstream out;

	size_t n;

	char in_s[256];
	char out_s[256];

	puts("Iveskite duomenu failo pavadinima: ");
	do {
		// Prideda \n i in_s
		//fgets(in_s, FILENAME_LENGTH, stdin);
		std::cin >> in_s;
		in.open(in_s);

		if (!in) {
			fputs("Nepavyko atidaryti nurodyto failo, bandykite dar karta:\n", stderr);
		}
	} while (!in);
	
	puts("Iveskite rezultatu failo pavadinima: ");
	do {
		//fgets(out_s, FILENAME_LENGTH, stdout);
		std::cin >> out_s;
		out.open(out_s);

		if (!out) {
			fputs("Nepavyko atidaryti nurodyto failo, bandykite dar karta:\n", stderr);
		}
	} while (!out);

	in >> n;

	size_t max_i;
	Pos max_p{0,0,0};

	for (size_t i = 0; i < n; ++i) {
		Pos p;
		in >> p.x >> p.y;
		p.d = std::abs(std::sqrt(p.x*p.x+p.y*p.y));
		if (p.d > max_p.d) {
			max_i = i;
			max_p = p;
		}
	}

	out << "Toliausias taskas yra nr. " << max_i+1 << "(" << max_p.x << "," << max_p.y << ")" << std::endl;
	out << "Nuo nulio atitoteles yra per " << max_p.d << " vienetu" << std::endl;
	return 0;
}
