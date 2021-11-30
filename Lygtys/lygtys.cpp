#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <map>
#include <cstring>

const int MAX_EXPR_LENGTH = 128;
const int SYMBOL_LENGTH = 5;

typedef long double (*expr_func)(long double x);

long double _x = 0;
bool _variables = false;
bool _equals = false;

/* Math parser */
//-------------------------------------------------------------------------------------------------------
char *parse_half(char *expr, long double& n1, char op1);
char *parse(char *expr, long double &n1);

int parse_function(char *&expr, char* symbol, long double &number);
int parse_symbol(char *&expr, long double &number);
bool parse_variable(char* symbol, long double &number);

bool read_num(char *&expr, long double &number);
bool read_op(char *&expr, char &op);
int precedence(char s);
bool exec_op(long double& n1, long double n2, char op);

int fact(int num) {
    if (num <= 1) return 1;
    return num * fact(num-1);
}

bool parse_variable(char* symbol, long double &number) {
	if (*symbol == 'x') {
		number = _x;
		_variables = true;
	}
	else if (*symbol == 'e') number = M_E;
	else if (strcmp(symbol, "pi") == 0) number = M_PI;
	else return false;
	
	return true;
}

// -1 no function, 0 malformed function, 1 valid function
int parse_function(char *&expr, char* symbol, long double &number) {
	expr_func func;
	if (strcmp(symbol, "cos") == 0) func = cosl;
	else if (strcmp(symbol, "sin") == 0) func = sinl;
	else if (strcmp(symbol, "tan") == 0) func = tanl;
	else if (strcmp(symbol, "arctan") == 0) func = atanl;
	else if (strcmp(symbol, "arccos") == 0) func = acosl;
	else if (strcmp(symbol, "log") == 0) func = logl;
	else if (strcmp(symbol, "exp") == 0) func = expl;
	else return -1;

	if (*expr != '(') return 0;
    ++expr;

	long double in_number;
	expr = parse(expr, in_number);
	if (!expr) return false;

	number = func(in_number);
	return (*(expr++) == ')');
}

// Parses symbol, could be a function or variable
// -1 no symbol, 0 malformed symbol, 1 valid symbol (number)
int parse_symbol(char *&expr, long double &number) {
	char symbol[SYMBOL_LENGTH];
	char* sym_ptr = symbol;

	if (!((*expr >= 'a' && *expr <= 'z') || (*expr >= 'A' && *expr <= 'Z')))
		return -1;	
    *(sym_ptr++) = *(expr++);

	while ((*expr >= 'a' && *expr <= 'z') || (*expr >= 'A' && *expr <= 'Z') || (*expr >= '0' && *expr <= '9')) 
		*(sym_ptr++) = *(expr++);
	*sym_ptr = 0;

	int res = parse_function(expr, symbol, number);
	if (res == 0) return 0;
	else if (res == 1) return 1;
	else return parse_variable(symbol, number);
}

// Reads number from the math expression, 
// format: <sign><number><optional factorial sign>, number can be subexpression in parantheses to which parse() is called
bool read_num(char *&expr, long double &number) {
	int res = parse_symbol(expr, number);
	if (res == 1) return true;
    else if (res == 0) return false;
	
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

	number = strtold(expr, &expr);

    while (*expr == ' ') ++expr; // Skip whitespace
    if (*expr == '!') { // Handle factorial
        if (ceill(number) != number) return false;
        number = fact((int)number-1);
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
    case '=':
        return 0;
    default:
        return -1;
    }
}

// Apply operator on two numbers
bool exec_op(long double& n1, long double n2, char op) {
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
        if (ceill(n1) != n1 || ceill(n2) != n2) return false;
        n1 = (int)n1 % (int)n2;
        break;
    case '^':
        n1 = pow(n1, n2);
        break;
	case '=':
		if (_equals) return false;
		n1 -= n2;
		_equals = true;
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
char *parse_half(char *expr, long double& n1, char op1) {
    long double n2;
    char op2;

    if (!read_num(expr, n2)) { // 1 operator 1 number
        if (op1 == '=' && *expr == '\0') {
			return expr; // Optional equals at the end
			_equals = true;
		}
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
			// Equals
			if (!exec_op(n1, n2, op1)) {
                return nullptr; // Invalid operation
            }
            return parse_half(expr, n1, op2);
        }
    }
}

// Parse subexpression: get n1 and op1, and call parse half with n1 and op2
char *parse(char *expr, long double &n1) {
    char op1;

    if (!read_num(expr, n1)) return nullptr; // No number
    if (!read_op(expr, op1)) { // Only number
        if (*expr != 0 && *expr != ')') return nullptr; // Unrecognized sign
        return expr;
    }

    return parse_half(expr, n1, op1);
}

//-------------------------------------------------------------------------------------------------------
/* Math parser */

const long double EPS = 1e-7;

// -1 neteisinga lygtis, 0 sprendiniai neegzistuoja, 1 padaryti skaičiavimai
int get_func_roots(long double x1, long double x2, long double& rez, char* expr) {   
    _x = x1;
	_equals = false;
	if (!parse(expr, rez)) return -1;

    while ((rez > EPS || rez < -EPS) && (x2-x1 >= EPS || x2-x1 <= -EPS)) {
        if (rez >= EPS) 
			x1 = _x;
		else 
			x2 = _x;
		
		_x = (x1+x2)/2;
        std::cout << x1 << " " << x2 << std::endl;

		_equals = false;
		if (!parse(expr, rez)) return 0;
    }
   	
	if (rez <= EPS && rez >= -EPS) {
		rez = _x;
		return true;
	} else 
		return false;
}

int main()
{
	char expr[MAX_EXPR_LENGTH];
	std::cout << "Įveskite lygtį ar bet kokį skaičiavimą: " << std::endl;
	std::cin.getline(expr, MAX_EXPR_LENGTH);

	long double rez;
	if (!parse(expr, rez)) { 
		std::cerr << "Neteisinga lygtis ar skaičiavimas" << std::endl;
        return 1;
    }

	if (!_variables) {
		std::cout << rez << std::endl;
		return 0;
	}	

	long double x1, x2;
	std::cout << "Iveskite funkcijos rėžius: ";
	std::cin >> x1 >> x2;

	if (x2<x1) std::swap(x2,x1);

	int res = get_func_roots(x1, x2, rez, expr);
	if (res == -1) {
		std::cerr << "Neteisinga lygtis ar skaičiavimas" << std::endl;
		return 1;
	} else if (res == 0) {
		std::cerr << "Sprendiniai neegzistuoja" << std::endl;
		return 1;
	} else std::cout << rez << std::endl;

    return 0;
}

