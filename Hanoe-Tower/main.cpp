#include <iostream>

void solve(unsigned n, char from, char to, char cur) {
	if (n == 0) return;

	solve(n-1, from, cur, to);
	std::cout << "Moving disk " << n << " from " << from << " to " << to << std::endl;
	solve(n-1, cur, to, from);
}

int main() {
	unsigned n;
	std::cout << "Enter how many disks should be solved: ";
	std::cin >> n;

	solve(n, 'a', 'c', 'b');
	std::cout << "Solved!" << std::endl;

	return 0;
}
