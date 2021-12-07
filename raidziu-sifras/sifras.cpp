#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

const unsigned MAX_EXPR_LENGTH = 1024;

std::map<char, int> symbols;
bool equal_end = false;

// Constants
const int FIRST_DECODE_DIGIT = 0;
#define CHECK_TRIVIAL (true) // Ignores results, where each letter is 0

int pow(int base, int power) { // No negative powers
    int res = 1; // 1 for negative powers
    while (power > 0) {
        --power;
        res *= base;
    }
    return res;
}

void fact(int num, int& acc) {
    if (num <= 1) return;
    acc *= num;
    fact(num - 1, acc);
}


/* Math parser */
char *parse_half(char *expr, int& n1, char op1);
char *parse(char *expr, int &n1);

// Reads number from the math expression, 
// format: <sign><number><optional factorial sign>, number can be subexpression in parantheses to which parse() is called
bool read_num(char *&expr, int &number) {
    number = 0;
    int l = 0; // number length
    int sign = 1;

    while (*expr == ' ') ++expr; // Skip whitespace

    if (*(expr) == '+') { // Handle number sign
        sign = 1;
        ++expr;
    } else if (*(expr) == '-') {
        sign = -1;
        ++expr;
    }

    while (*expr == ' ') ++expr; // Skip whitespace

    if (*(expr) == '(') { // Handle parentheses (subexpression)
        ++expr;
        expr = parse(expr, number);
        if (!expr) {
            return false;
        }
        if (*expr != ')') return false; // No closing parentheses
        number *= sign;
        ++expr;
        return true;
    }

    // Find number length
    while (*(expr + l) >= '0' && *(expr + l) <= '9' || symbols.find(*(expr+l)) != symbols.end()) {
        ++l;
    }

    if (!l) return false; // No number

    while (l) { // Construct number
        // Replace with decoded version if it's not a digit
        int digit = (*expr >= '0' && *expr <= '9' ? (*expr - '0') : symbols[*expr]);
        number += (pow(10, l - 1) * digit);
        --l;
        ++expr;
    }

    while (*expr == ' ') ++expr; // Skip whitespace
    if (*expr == '!') { // Handle factorial
        fact(number-1, number);
        ++expr;
    }

    number *= sign;
    return true;
}

// Reads an operator, operators: +-*/%=^
bool read_op(char *&expr, char &op) {
    while (*expr == ' ') ++expr; // Skip whitespace
    op = *expr;
    if (op != '+' && op != '-' && op != '*' && op != '/' && op != '%' && op != '=' && op != '^') {
        return false;
    }
    ++expr;
    return true;
}

// Operator precedence, higher more priority
int precedence(char s) {
    switch (s) {
    case '=':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 2;
    case '^':
        return 3;
    case '(':
        return 4;
    case '!':
        return 5;
    default:
        return -1;
    }
}

// Apply operator on two numbers
bool exec_op(int& n1, int n2, char op) {
    switch (op) {
    case '+':
        n1 += n2;
        break;
    case '-':
        n1 -= n2;
        break;
    case '*':
        n1 *= n2;
        break;
    case '/':
        if (n2 == 0) return false;
        else n1 /= n2;
        break;
    case '%':
        n1 %= n2;
        break;
    case '^':
        n1 = pow(n1, n2);
        break;
    case '=':
       n1 = (n1 == n2);
       break;
    default:
        return false;
    }
    return true;
}

// Parses other half of the subexpression, n1 and op1 is given
/* Algorithm:
n1 = first number, op1 = first operator, n2 = second number, op2 = second operator
n1, op1 given

if second number not exist end parsing
otherwise check precedence of operators:
	if op2 > op1: parse right half as n2, and apply op1 to n1 and new n2
	else: apply op1 to n1 and n2 (as n1) and parse right half, with new n1 and op2
*/
char *parse_half(char *expr, int& n1, char op1) {
    int n2;
    char op2;

    if (!read_num(expr, n2)) { // 1 operator 1 number
        if (op1 == '=' && *expr == '\0') return expr; // Optional equals at the end
        else return nullptr;
    }
    if (!read_op(expr, op2)) { // 1 operator 2 numbers
        if (*expr != 0 && *expr != ')') {
            return nullptr; // Unrecognized sign
        }
        if (!exec_op(n1, n2, op1)) {
            return nullptr; // Invalid operation
        }
        return expr;
    } else { // 2 operators 2 numbers
        if (precedence(op2) > precedence(op1)) { // 9 + 4 * | 5
            expr = parse_half(expr, n2, op2);
            if (!expr) {
                return nullptr; // Invalid expression
            }
            if (!exec_op(n1, n2, op1)) {
                return nullptr; // Invalid operation
            }
            return expr;
        } else { // 9 * 4 + | 5
            if (!exec_op(n1, n2, op1)) {
                return nullptr; // Invalid operation
            }
            return parse_half(expr, n1, op2);
        }
    }
}

// Parse subexpression: get n1 and op1, and call parse half with n1 and op2
char *parse(char *expr, int &n1) {
    char op1;

    if (!read_num(expr, n1)) return nullptr; // No number
    if (!read_op(expr, op1)) { // Only number
        if (*expr != 0 && *expr != ')') return nullptr; // Unrecognized sign
        return expr;
    }

    return parse_half(expr, n1, op1);
}
/* Math parser */


// Inserts dummy values for each symbol in the expression
void load_symbols(char * expr) {
    while (*expr) {
        if (*expr >= 'A' && *expr <= 'Z' || *expr >= 'a' && *expr <= 'z') {
            symbols.insert(std::pair<char, int>(*expr, 0));
        }
        ++expr;
    }
}

// Tries each digit for each letter
// PROBLEM: reparses the expression for each letter
bool decode_symbols(char *expr, std::map<char, int>::iterator it, void (*func)(char*, int)) {
    if (it == symbols.end()) {
		//if constexpr (CHECK_TRIVIAL) C++17
		#if CHECK_TRIVIAL
		bool found = false;
		for (std::map<char, int>::iterator it2 = symbols.begin(); it2 != it; ++it2) {
			if (it2->second != 0) {
				found = true;
				break;
			}
		}
		if (!found) return true;
		#endif
        int res;
        parse(expr, res); // PROBLEM: Parses expression for each digit
        func(expr, res);
        return res;
    }

    for (it->second = FIRST_DECODE_DIGIT; it->second <= 9; ++(it->second)) {
        if (decode_symbols(expr, std::next(it), func)) break;
    }

    return false;
}

int main() {
    char expr[MAX_EXPR_LENGTH];
    int num;

    std::cout << "Enter expression: " << std::endl;
    std::cin.getline(expr, MAX_EXPR_LENGTH);

    load_symbols(expr);
    bool compares = false;

    for (char* chr = expr; *chr; ++chr) {
        if (*chr == '=') {
            ++chr;
            while (*chr == ' ') ++chr; // Skip whitespace
            if (*chr == '\0') equal_end = true;
            else compares = true;
        }
    }

    if (!parse(expr, num)) { // Performed either way to make sure expression is valid
        std::cerr << "Malformed expression" << std::endl;
        return 1;
    }
    if (compares) {
        decode_symbols(expr, symbols.begin(), [](char* expr, int res) {
            if (!res) return; // Show only valid results

            std::cout << expr;
            if (!equal_end) std::cout << "=";
            std::cout << res << std::endl;
            for (auto & symbol : symbols) {
                std::cout << symbol.first << " = " << symbol.second << std::endl;
            }
        });
    } else {
        std::cout << expr;
        if (!equal_end) std::cout << "=";
        std::cout << num << std::endl;
    }

    return 0;
}
