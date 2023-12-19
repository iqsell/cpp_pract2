#include <iostream>
#include <limits>
#include <string>
#include "vector"
#include "algorithm"

class Array {
protected:
    std::string integerPart;
    std::string fractionalPart;

public:
    Array() = default;

    Array(const std::string &intPart, const std::string &fracPart) : integerPart(intPart), fractionalPart(fracPart) {}

    virtual ~Array() {}

    virtual Array *add(const Array &other) const {
        const Array &otherArray = static_cast<const Array &>(other);
        std::string resultIntegerPart;
        std::string resultFractionalPart;


        int maxFracSize = std::max(fractionalPart.size(), otherArray.fractionalPart.size());
        int carry = 0;
        for (int i = maxFracSize - 1; i >= 0; i--) {
            int sum = carry;
            if (i < fractionalPart.size()) {
                sum += fractionalPart[i] - '0';
            }
            if (i < otherArray.fractionalPart.size()) {
                sum += otherArray.fractionalPart[i] - '0';
            }
            carry = sum / 10;
            resultFractionalPart = std::to_string(sum % 10) + resultFractionalPart;
        }


        int maxIntSize = std::max(integerPart.size(), otherArray.integerPart.size());
        carry += (maxFracSize > 0) ? (carry / 10) : 0;
        for (int i = maxIntSize - 1; i >= 0; i--) {
            int sum = carry;
            if (i < integerPart.size()) {
                sum += integerPart[i] - '0';
            }
            if (i < otherArray.integerPart.size()) {
                sum += otherArray.integerPart[i] - '0';
            }
            carry = sum / 10;
            resultIntegerPart = std::to_string(sum % 10) + resultIntegerPart;
        }


        if (carry > 0) {
            resultIntegerPart = std::to_string(carry) + resultIntegerPart;
        }

        return new Array(resultIntegerPart, resultFractionalPart);
    }

    void print() const {
        std::cout << integerPart << "." << fractionalPart << std::endl;
    }
};

class Fraction : public Array {
private:
    long integerPart;
    unsigned short fractionalPart;

public:
    Fraction(long intPart, unsigned short fracPart) : integerPart(intPart), fractionalPart(fracPart) {}

    Array *add(const Array &other) const override {
        const Fraction &otherFraction = static_cast<const Fraction &>(other);
        long resultInteger = integerPart + otherFraction.integerPart;
        unsigned short resultFractional = fractionalPart + otherFraction.fractionalPart;
        if (resultFractional > 255) {
            resultInteger++;
            resultFractional -= 256;
        }
        return new Fraction(resultInteger, resultFractional);
    }

    long getIntegerPart() const {
        return integerPart;
    }

    unsigned short getFractionalPart() const {
        return fractionalPart;
    }
};

class BitString : public Array {
private:
    std::string value;

public:
    BitString(const std::string &val) : value(val) {}

    Array *add(const Array &other) const override {
        const BitString &otherBitString = static_cast<const BitString &>(other);
        std::string resultValue;
        int resultSize = std::max(value.size(), otherBitString.value.size());
        int carry = 0; // Перенос
        for (int i = 0; i < resultSize; i++) {
            int bit1 = (i < value.size()) ? value[value.size() - i - 1] - '0' : 0; // Бит из первой строки
            int bit2 = (i < otherBitString.value.size()) ? otherBitString.value[otherBitString.value.size() - i - 1] -
                                                           '0' : 0; // Бит из второй строки
            int sum = bit1 + bit2 + carry; // Сумма битов и переноса
            int resultBit = sum % 2; // Бит результата
            carry = sum / 2; // Обновляем перенос для следующего бита
            resultValue = std::to_string(resultBit) + resultValue; // Добавляем бит к результату
        }
        if (carry > 0) {
            resultValue = std::to_string(carry) + resultValue; // Добавляем оставшийся перенос, если есть
        }
        return new BitString(resultValue);
    }

    void input() {
        bool validInput = false;
        while (!validInput) {
            std::cout << "Введите битовую строку: ";
            std::cin >> value;
            validInput = true;
            for (char bit: value) {
                if (bit != '0' && bit != '1') {
                    validInput = false;
                    std::cout << "Неверный ввод. Пожалуйста, используйте только бинарные цифры (0 и 1).\n";
                    break;
                }
            }
        }
    }

    void output() const {
        std::cout << "Битовая строка: " << value << std::endl;
    }
};

int main() {
    char choice;
    bool isRunning = true;
    while (isRunning) {
        std::cout << "Выберите действие:" << std::endl;
        std::cout << "1. Выполнить операцию с дробью" << std::endl;
        std::cout << "2. Выполнить операцию со строкой бит" << std::endl;
        std::cout << "3. Выполнить операцию с массивами" << std::endl;
        std::cout << "4. Выйти" << std::endl;
        std::cin >> choice;

        switch (choice) {
            case '1': {
                long intPart1, intPart2;
                unsigned short fracPart1, fracPart2;
                std::cout << "Введите значения для первой дроби (целая и дробная части):" << std::endl;
                if (!(std::cin >> intPart1 >> fracPart1) || fracPart1 < 0) {
                    std::cerr
                            << "Ошибка ввода. Пожалуйста, введите целое число и неотрицательное значение для дробной части числа."
                            << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                std::cout << "Введите значения для второй дроби (целая и дробная части):" << std::endl;
                if (!(std::cin >> intPart2 >> fracPart2) || fracPart2 < 0) {
                    std::cerr
                            << "Ошибка ввода. Пожалуйста, введите целое число и неотрицательное значение для дробной части числа."
                            << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                Fraction fraction1(intPart1, fracPart1);
                Fraction fraction2(intPart2, fracPart2);
                Array *result = fraction1.add(fraction2);
                std::cout << "Результат сложения дробей: " << static_cast<Fraction *>(result)->getIntegerPart() << "."
                          << static_cast<Fraction *>(result)->getFractionalPart() << std::endl;
                delete result;
                break;
            }
            case '2': {
                BitString bs1("101");
                BitString bs2("111");
                bs1.input();
                bs2.input();
                Array *result = bs1.add(bs2);
                static_cast<BitString *>(result)->output();
                delete result;
                break;
            }
            case '3': {
                std::string input1, input2;
                std::cout << "Введите первое дробное число: ";
                std::cin >> input1;
                std::cout << "Введите второе дробное число: ";
                std::cin >> input2;

                // Разделение на целую и дробную части
                size_t dotPos1 = input1.find('.');
                std::string intPart1 = input1.substr(0, dotPos1);
                std::string fracPart1 = (dotPos1 != std::string::npos) ? input1.substr(dotPos1 + 1) : "";

                size_t dotPos2 = input2.find('.');
                std::string intPart2 = input2.substr(0, dotPos2);
                std::string fracPart2 = (dotPos2 != std::string::npos) ? input2.substr(dotPos2 + 1) : "";

                Array array1(intPart1, fracPart1);
                Array array2(intPart2, fracPart2);
                Array *result = array1.add(array2);
                result->print();
                delete result;

                break;

            }
            case '4': {
                isRunning = false;
                break;
            }
            default: {
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
            }
        }
    }

    return 0;
}

