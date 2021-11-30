#include <iostream>
#include <climits>

// 1 versija: skaičiuota nuo n. Labai lėta, skaičiuota tuos pačius skaičius daugybę kartų. 
/*
fib(50) be IO išvedimų suskaičiavo per 49 sekundes, su IO išvedimais ir sync_with_stdio(false) 82 minutes
Jei į failą įrašytumėm fib(x) kaskart kai fib funkcija iškviečiame, gautume 268 GB failą

fib(53) be IO išvedimų suskaičiavo per 3 minutes 35 sekundes
-----------------------------------------------------------------
Kiek kartų paleido rekursines funkcijas nuo fib(50)
12586269025 fib(1)
 7778742049 fib(0)
 7778742049 fib(2)
 4807526976 fib(3)
 2971215073 fib(4)
 1836311903 fib(5)
 1134903170 fib(6)
  701408733 fib(7)
  433494437 fib(8)
  267914296 fib(9)
  165580141 fib(10)
  102334155 fib(11)
   63245986 fib(12)
   39088169 fib(13)
   24157817 fib(14)
   14930352 fib(15)
    9227465 fib(16)
    5702887 fib(17)
    3524578 fib(18)
    2178309 fib(19)
    1346269 fib(20)
     832040 fib(21)
     514229 fib(22)
     317811 fib(23)
     196418 fib(24)
     121393 fib(25)
      75025 fib(26)
      46368 fib(27)
      28657 fib(28)
      17711 fib(29)
      10946 fib(30)
       6765 fib(31)
       4181 fib(32)
       2584 fib(33)
       1597 fib(34)
        987 fib(35)
        610 fib(36)
        377 fib(37)
        233 fib(38)
        144 fib(39)
         89 fib(40)
         55 fib(41)
         34 fib(42)
         21 fib(43)
         13 fib(44)
          8 fib(45)
          5 fib(46)
          3 fib(47)
          2 fib(48)
          1 fib(49)
          1 fib(50)
*/
long long fib1(unsigned long n) {
    if (n == 0 || n == 1) return n; 
	else return fib1(n - 1) + fib1(n - 2);
}

// 2 versija A: skaičiuoja nuo 0 iki n, nesikartoja skaičiavimai.
// Tarpiniai kintamieji, rekursija nereikalinga.
// su kiekvienu n funkcija paleidžiama tik vieną kartą
// fib(1000) 10000000 kartų suskaičiuoja per 47s
long long fib2(unsigned long n, long long a=0, long long b=1) {
	if (n <= 2) {
		if (n == 2) return a+b;
		else return n;
	}
	return fib2(n-1, b, a+b);
}

// 2 versija B: tokia pat kaip A, išskyrus, kad nereiktų skaičiuoti atskirai kelių fibonacci skaičių
// įvedamas kintamasis kuris atsimena paskutinijį fibonačį skaičių masyve, nuo kurio yra skaičiuojamas naujas.
// Naudojama papildoma atmintis. Masyvo pirmi du skaičiai turi būti 0 ir 1. Rekursija bereikalinga.
// fib(1000) 10000000 kartų suskaičiuoja per 0.030-0.070s
long long fib2(unsigned long n, long long* fibs, unsigned long &last_index) {
	if (last_index >= n) {
		return fibs[n]; 
	}
	++last_index;
	fibs[last_index] = fibs[last_index-1] + fibs[last_index-2];
	return fib2(n, fibs, last_index);
}

// 2 versija A perrašyta per ciklus
long long fib3(unsigned long n) {
	if (n < 2) {
		return n;
	}

	long long a, b, temp;
	for (a = 0, b = 1; n != 1; --n) {
		temp = a;
		a = b;
		b = temp+b;
	}

	return b;
}

// 2 versija B perrašyta per ciklus
long long fib3(unsigned long n, long long* fibs, unsigned long &last_index) {
	while (last_index < n) {
		++last_index;
		fibs[last_index] = fibs[last_index-1] + fibs[last_index-2];
	}
	return fibs[n];
}

int main() {
    unsigned long n;
	//unsigned long last_index = 2;

	std::ios::sync_with_stdio(false);	
    std::cin >> n;

	//long long fibs[n+1] = {0,1,1};
    //std::cout << "fib1(" << n << ")= " << fib1(n) << std::endl;
    // std::cout << "fib2(" << n << ")= " << fib2(n) << std::endl;
    //std::cout << "fib2b(" << n << ")= " << fib2(n, fibs, last_index) << std::endl;
    std::cout << "fib3(" << n << ")= " << fib3(n) << std::endl;
    //std::cout << "fib3b(" << n << ")= " << fib3(n, fibs, last_index) << std::endl;
	
    return 0;
}
