#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Forward declaration of parseExpression to handle parentheses in parseFactor
double parseExpression(const string& expr, int& i, bool& isDouble);

// Parses a number from the expression, handling negative numbers and decimals
double parseNumber(const string& expr, int& i, bool& isDouble) {
    bool isNegative = false;

    // Check if the number is negative
    if (expr[i] == '-') {
        isNegative = true;
        i++;
    }

    double result = 0.0;
    while (i < expr.size() && (isdigit(expr[i]) || expr[i] == '.')) {
        if (expr[i] == '.') {
            isDouble = true;
            i++;
            double fraction = 1;
            while (i < expr.size() && isdigit(expr[i])) {
                fraction /= 10;
                result += (expr[i++] - '0') * fraction;
            }
            break;
        }
        result = result * 10 + (expr[i++] - '0');
    }

    return isNegative ? -result : result;
}

// Parses factors, handling parentheses and exponentiation
double parseFactor(const string& expr, int& i, bool& isDouble) {
    double result;

    if (expr[i] == '(') {  // Handle parentheses
        i++;  // Skip '('
        result = parseExpression(expr, i, isDouble);
        i++;  // Skip ')'
    } else {
        result = parseNumber(expr, i, isDouble);
    }

    // Handle exponentiation
    while (i < expr.size() && expr[i] == '^') {
        i++;
        result = pow(result, parseFactor(expr, i, isDouble));
    }

    return result;
}

// Parses terms, handling multiplication and division
double parseTerm(const string& expr, int& i, bool& isDouble) {
    double result = parseFactor(expr, i, isDouble);

    while (i < expr.size() && (expr[i] == '*' || expr[i] == '/')) {
        char op = expr[i++];
        double nextFactor = parseFactor(expr, i, isDouble);
        if (op == '*') {
            result *= nextFactor;
        } else {
            result /= nextFactor;
            isDouble = true;  // Set to double when performing division
        }
    }

    return result;
}

// Parses the full expression, handling addition and subtraction
double parseExpression(const string& expr, int& i, bool& isDouble) {
    double result = parseTerm(expr, i, isDouble);

    while (i < expr.size() && (expr[i] == '+' || expr[i] == '-')) {
        char op = expr[i++];
        double nextTerm = parseTerm(expr, i, isDouble);
        if (op == '+') {
            result += nextTerm;
        } else {
            result -= nextTerm;
        }
    }

    return result;
}

// Evaluates the entire expression string and returns the result
double evaluateExpression(const string& expression) {
    int i = 0;
    bool isDouble = false;
    double result = parseExpression(expression, i, isDouble);
    return isDouble ? result : static_cast<int>(result);
}

int main() {
    string input;
    cout << "Enter the expression (type '=' to calculate): ";

    // Take continuous input until '=' is entered
    while (true) {
        string part;
        cin >> part;
        if (part == "=") {
            // Evaluate and display the result
            double result = evaluateExpression(input);
            cout << "Result: " << result << endl;
            break;
        }
        input += part;
    }

    return 0;
}
