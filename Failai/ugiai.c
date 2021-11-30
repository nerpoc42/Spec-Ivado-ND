#include <stdio.h>

int main() {
	FILE* in;
	FILE* out;
	size_t n;
	float avg;
	float max, min;
	float ugis;
	char in_s[256];
	char out_s[256];

	puts("Iveskite duomenu failo pavadinima: ");
	do {
		// Prideda \n i in_s
		//fgets(in_s, FILENAME_LENGTH, stdin);
		scanf("%256s", in_s);

		if ((in = fopen(in_s, "r")) == NULL) {
			fputs("Nepavyko atidaryti nurodyto failo, bandykite dar karta:\n", stderr);
		}
	} while (!in);
	
	puts("Iveskite rezultatu failo pavadinima: ");
	do {
		//fgets(out_s, FILENAME_LENGTH, stdout);
		scanf("%256s", out_s);

		if ((out = fopen(out_s, "w")) == NULL) {
			fputs("Nepavyko atidaryti nurodyto failo, bandykite dar karta:\n", stderr);
		}
	} while (!out);

	fscanf(in, "%zu", &n);

	fscanf(in, "%f", &ugis);
	max = ugis;
	min = ugis;
	avg = ugis;

	for (size_t i = 1; i < n; ++i) {
		fscanf(in, "%f", &ugis);

		if (ugis > max) max = ugis;
		else if (ugis < min) min = ugis;
		
		avg += ugis;
	}

	avg /= n;
	fprintf(out, "Auksciausias: %.2f\n", max);
	fprintf(out, "Zemiausias: %.2f\n", min);
	fprintf(out, "Vidurkis: %.2f\n", avg);

	fclose(in);
	fclose(out);

	return 0;
}
