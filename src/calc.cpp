/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: Simple calculator
 *  File: src/calc.cpp
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple calculator script */
#include <iostream>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

int getPriority(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;
    else
        return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return 0;
    }
}

double calculateExpression(string expression) {
    stack<double> values;
    stack<char> operators;

    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == ' ')
            continue;
        else if (expression[i] == '(')
            operators.push(expression[i]);
        else if (isdigit(expression[i]) || expression[i] == 'P' || expression[i] == 'π') {
            double num = 0;
            // π
            if (expression[i] == 'π' or expression[i] == 'P')
                num = M_PI;
            else {
                while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                    num = num * 10 + (expression[i] - '0');
                    i++;
                }
                i--;
            }
            values.push(num);
        }
        else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.pop(); // remove '(' from stack
        }
        else {
            while (!operators.empty() && getPriority(operators.top()) >= getPriority(expression[i])) {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(expression[i]);
        }
    }

    while (!operators.empty()) {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}

int main() {
    string expression;
    cout << "Enter an expression: ";
    getline(cin, expression);

    double result = calculateExpression(expression);
    cout << "Result: " << result << endl;

    return 0;
}
