#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>

class RPNCalculator {
private:
    std::stack<double> dataStack;
    std::vector<std::string> history;

public:
    // 压入数值
    void push(double value) {
        dataStack.push(value);
    }

    // 弹出数值
    double pop() {
        if (dataStack.empty()) {
            throw std::runtime_error("错误: 栈为空");
        }
        double value = dataStack.top();
        dataStack.pop();
        return value;
    }

    // 获取栈顶元素但不弹出
    double peek() {
        if (dataStack.empty()) {
            throw std::runtime_error("错误: 栈为空");
        }
        return dataStack.top();
    }

    // 清空栈
    void clear() {
        while (!dataStack.empty()) {
            dataStack.pop();
        }
        std::cout << "栈已清空" << std::endl;
    }

    // 显示当前栈内容
    void display() {
        if (dataStack.empty()) {
            std::cout << "栈为空" << std::endl;
            return;
        }

        std::stack<double> temp = dataStack;
        std::vector<double> elements;
        
        while (!temp.empty()) {
            elements.push_back(temp.top());
            temp.pop();
        }

        std::cout << "当前栈内容 (栈顶 -> 栈底): ";
        for (const auto& elem : elements) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    // 计算斐波那契数列第n项
    double fibonacci(int n) {
        if (n < 0) {
            throw std::runtime_error("错误: 斐波那契数列索引必须非负");
        }
        if (n == 0) return 0;
        if (n == 1) return 1;
        
        double a = 0, b = 1;
        for (int i = 2; i <= n; i++) {
            double temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }

    // 计算杨辉三角指定位置的值 C(n, k)
    double pascal(int n, int k) {
        if (n < 0 || k < 0 || k > n) {
            throw std::runtime_error("错误: 杨辉三角参数无效");
        }
        
        // 优化：C(n, k) = C(n, n-k)
        if (k > n - k) {
            k = n - k;
        }
        
        double result = 1;
        for (int i = 0; i < k; i++) {
            result = result * (n - i) / (i + 1);
        }
        return result;
    }

    // 执行计算操作
    void calculate(const std::string& operation) {
        if (operation == "+") {
            double b = pop();
            double a = pop();
            push(a + b);
        }
        else if (operation == "-") {
            double b = pop();
            double a = pop();
            push(a - b);
        }
        else if (operation == "*") {
            double b = pop();
            double a = pop();
            push(a * b);
        }
        else if (operation == "/") {
            double b = pop();
            if (b == 0) {
                throw std::runtime_error("错误: 除数不能为零");
            }
            double a = pop();
            push(a / b);
        }
        else if (operation == "sqrt") {
            double a = pop();
            if (a < 0) {
                throw std::runtime_error("错误: 负数没有实数平方根");
            }
            push(sqrt(a));
        }
        else if (operation == "pow") {
            double b = pop();  // 指数
            double a = pop();  // 底数
            push(pow(a, b));
        }
        else if (operation == "sin") {
            double a = pop();
            push(sin(a));
        }
        else if (operation == "cos") {
            double a = pop();
            push(cos(a));
        }
        else if (operation == "tan") {
            double a = pop();
            push(tan(a));
        }
        else if (operation == "fib") {
            double a = pop();
            int n = static_cast<int>(a);
            push(fibonacci(n));
        }
        else if (operation == "pascal") {
            double k = pop();  // 第二个参数
            double n = pop();  // 第一个参数
            push(pascal(static_cast<int>(n), static_cast<int>(k)));
        }
        else {
            throw std::runtime_error("错误: 未知操作符 '" + operation + "'");
        }
    }

    // 处理RPN表达式
    double evaluate(const std::string& expression) {
        std::istringstream iss(expression);
        std::string token;

        while (iss >> token) {
            // 尝试将token解析为数字
            try {
                size_t pos;
                double value = std::stod(token, &pos);
                if (pos == token.length()) {
                    push(value);
                    continue;
                }
            } catch (...) {
                // 不是数字，继续处理为操作符
            }

            // 处理操作符
            if (token == "clear") {
                clear();
            }
            else if (token == "display") {
                display();
            }
            else {
                calculate(token);
            }
        }

        if (dataStack.empty()) {
            throw std::runtime_error("错误: 表达式计算后栈为空");
        }

        double result = peek();
        history.push_back(expression + " = " + std::to_string(result));
        return result;
    }

    // 显示历史记录
    void showHistory() {
        if (history.empty()) {
            std::cout << "暂无历史记录" << std::endl;
            return;
        }

        std::cout << "\n=== 计算历史 ===" << std::endl;
        for (size_t i = 0; i < history.size(); i++) {
            std::cout << i + 1 << ". " << history[i] << std::endl;
        }
    }

    // 获取栈大小
    size_t size() {
        return dataStack.size();
    }
};

void printHelp() {
    std::cout << "\n=== RPN 计算器帮助 ===" << std::endl;
    std::cout << "基本操作符: + - * / sqrt pow" << std::endl;
    std::cout << "三角函数: sin cos tan" << std::endl;
    std::cout << "特殊操作符:" << std::endl;
    std::cout << "  fib    - 计算斐波那契数列第n项 (用法: n fib)" << std::endl;
    std::cout << "  pascal - 计算杨辉三角C(n,k) (用法: n k pascal)" << std::endl;
    std::cout << "栈操作: clear display" << std::endl;
    std::cout << "其他命令: history help q(退出)" << std::endl;
    std::cout << "示例: 5 5 + (结果: 10)" << std::endl;
    std::cout << "      3 4 pow (结果: 81)" << std::endl;
    std::cout << "      10 fib (结果: 55)" << std::endl;
    std::cout << "      5 2 pascal (结果: 10)" << std::endl;
}

int main() {
    RPNCalculator calculator;
    std::string input;

    std::cout << "==================================" << std::endl;
    std::cout << "    C++ RPN 计算器 v1.0" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "输入表达式 (例: '5 5 +'), 或 'help' 查看帮助, 'q' 退出." << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        // 去除首尾空格
        size_t start = input.find_first_not_of(" \t\n\r");
        size_t end = input.find_last_not_of(" \t\n\r");
        
        if (start == std::string::npos) {
            continue;
        }
        
        input = input.substr(start, end - start + 1);

        if (input == "q" || input == "quit" || input == "exit") {
            std::cout << "感谢使用！再见！" << std::endl;
            break;
        }

        if (input == "help" || input == "h") {
            printHelp();
            continue;
        }

        if (input == "history") {
            calculator.showHistory();
            continue;
        }

        if (input.empty()) {
            continue;
        }

        try {
            double result = calculator.evaluate(input);
            std::cout << "结果: " << std::fixed << std::setprecision(6) << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
