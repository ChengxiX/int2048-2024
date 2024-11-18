#include "include/int2048.h"

namespace sjtu {

int2048::int2048() {
    signal = true;
}

int2048::int2048(long long num) {
    digits.resize(5);
    if (num < 0) {
        signal = false;
        num = -num;
    } else {
        signal = true;
    }
    for (int i = 0; i < 6; i++) {
        digits[i] = num % 10000;
        num /= 10000;
    }
}

int2048::int2048(const std::string & num) {
    read(num);
}

void int2048::read(const std::string & num) {
    if (num[0] == '-') {
        signal = false;
    } else {
        signal = true;
    }
    int p = num.size() - 1;
    digits.resize((num.size() / 4) + 1);
    for (int i = 0; i < (num.size() / 4) + 1; i++) {
        digits[i] = 0;
        for (int j = 0; j < DIGIT_NUM; j++, p--) {
            if (p < 0 || (p == 0 && num[p] == '-')) {
                return;
            }
            digits[i] += (num[p] - '0') * pow(10, j);
        }
    }
    if (digits.size()==1||digits[0]==0) {
        signal = true;
    }
}

int2048::int2048(const int2048 & num) {
    digits = num.digits;
    signal = num.signal;
}

void int2048::print() {
    for (int i = digits.size()-1; i >= 0; i--) {
        if (digits[i] != 0) {
            if (!signal) {
                std::cout << '-';
            }
            std::cout << digits[i];
            for (int j = i-1; j >= 0; j--) {
                if (digits[j] == 0) {
                    std::cout << "0000";
                } else {
                    for (int k = 3; k >= 0; k--) {
                        if (digits[j] < pow(10, k)) {
                            std::cout << '0';
                        } else {
                            std::cout << digits[j];
                            break;
                        }
                    }
                }
            }
            return;
        }
    }
    std::cout << '0';
}

int2048 & int2048::add(const int2048 &num) {
    if (signal != num.signal) {
        return minus(num);
    }
    int i = 0;
    int adv = 0;
    int d = std::max(digits.size(), num.digits.size());
    digits.resize(d);
    for (; i < d; i++) {
        if (i < num.digits.size()) {    
            digits[i] += num.digits[i];
        }
        digits[i] += adv;
        if (digits[i] >= 10000) {
            digits[i] -= 10000;
            adv = 1;
        }
        else {
            adv = 0;
        }
    }
    if (adv) {
        digits.push_back(1);
    }
    return *this;
}

int2048 add(int2048 num1, const int2048 &num2) {
    int2048 res = num1;
    res.add(num2);
    return res;
}

int2048 & int2048::minus(const int2048 &num) {
    if (signal != num.signal) {
        bool sig = signal;
        signal = num.signal;
        add(num);
        signal = sig;
        return *this;
    }
    int i = 0;
    int d = std::max(digits.size(), num.digits.size());
    digits.resize(d);
    for (; i < num.digits.size(); i++) {
        digits[i] -= num.digits[i];
    }
    int flag = 0;
    for (int i = d - 1; i >= 0; i--) {
        if (digits[i] > 0) {
            flag = 1;
            break;
        }
        if (digits[i] < 0) {
            flag = -1;
            break;
        }
    }
    if (flag == 0) {
        signal = true;
    }
    else if (flag == -1) {
        signal = !signal;
        for (int i = 0; i < d - 1; i++) {
            if (digits[i] > 0) {
                digits[i] = 10000 - digits[i];
                digits[i+1]++;
            }
            else {
                digits[i] = -digits[i];
            }
        }
    }
    else {
        for (int i = 0; i < d - 1; i++) {
            if (digits[i] < 0) {
                digits[i] += 10000;
                digits[i+1]--;
            }
        }
    }
    return *this;
}

int2048 minus(int2048 num1, const int2048 &num2) {
    int2048 res = num1;
    res.minus(num2);
    return res;
}

} // namespace sjtu

int main() {
    sjtu::int2048 a(321);
    sjtu::int2048 b(134);
    b.minus(a);
    b.print();
}