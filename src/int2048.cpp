#include "include/int2048.h"
#include <algorithm>
#include <vector>

namespace sjtu {

int2048::int2048() {
    signal = true;
}

int2048::int2048(long long num) {
    digits.resize(6);
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

int2048::int2048(const std::vector<int> &digits, bool signal=true) {
    this->digits = digits;
    this->signal = signal;
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
                }
                else if (digits[j]<10) {
                    std::cout << "000" << digits[j];
                }
                else if (digits[j]<100) {
                    std::cout << "00" << digits[j];
                }
                else if (digits[j]<1000) {
                    std::cout << "0" << digits[j];
                }
                else {
                    std::cout << digits[j];
                }
            }
            return;
        }
    }
    std::cout << '0';
}

void int2048::cut() {
    for (int i = digits.size()-1; i >= 0; i--) {
        if (digits[i] != 0) {
            digits.resize(i+1);
            return;
        }
    }
    digits.resize(1);
    signal = true;
}

int2048 int2048::operator+() const {
    return int2048(*this);
}

int2048 int2048::operator-() const {
    return int2048(this->digits, !this->signal);
}

int int2048::len() const {
  for (int i = digits.size()-1; i>=0; i--) {
    if (digits[i]!=0) {
      if (digits[i] > 999) {
        return DIGIT_NUM * i + 4;
      }
      else if (digits[i] > 99) {
        return DIGIT_NUM * i + 3;
      }
      else if (digits[i] > 9) {
        return DIGIT_NUM * i + 2;
      }
      else {
        return DIGIT_NUM * i + 1;
      }
    }
  }
  return 0;
}

bool operator<(const int2048 &a, const int2048 &b) {
    if (!a.signal&&b.signal) {
        return true;
    }
    else if (a.signal&&!b.signal) {
        return false;
    }
    else {
        int cmp = absCmp(a, b);
        if (cmp < 0) {
            return a.signal;
        }
        else if (cmp > 0) {
            return !a.signal;
        }
        else {
            return false;
        }
    }
}

bool operator>(const int2048 &a, const int2048 &b) {
    if (!a.signal&&b.signal) {
        return false;
    }
    else if (a.signal&&!b.signal) {
        return true;
    }
    else {
        int cmp = absCmp(a, b);
        if (cmp < 0) {
            return !a.signal;
        }
        else if (cmp > 0) {
            return a.signal;
        }
        else {
            return false;
        }
    }
}

int absCmp(const int2048 &a, const int2048 &b) {
    int la = a.len();
    int lb = b.len();
    if (la > lb) {
        return 1;
    }
    else if (la < lb) {
        return -1;
    }
    for (int i = (la - 1) / 4; i >= 0; i--) {
        if (a.digits[i] > b.digits[i]) {
            return 1;
        }
        else if (a.digits[i] < b.digits[i]) {
            return -1;
        }
    }
    return 0;
}

bool operator!=(int2048 &a, int2048 &b) {
    return !(a==b);
}

bool operator==(const int2048 &a, const int2048 &b) {
    return (a.signal == b.signal) && (absCmp(a, b) == 0);
}

bool operator>=(int2048 &a, int2048 &b) {
    return !(a < b);
}

bool operator<=(int2048 &a, int2048 &b) {
    return !(a > b);
}

int2048 & int2048::operator=(const int2048 &a) {
    digits = a.digits;
    signal = a.signal;
    return *this;
}

int2048 operator+(const int2048 &a, const int2048 &b) {
    if (a.signal != b.signal) {
        const int2048 *d;
        int2048 res;
        int cmp = absCmp(a, b);
        if (cmp < 0) {
            res = b;
            d = &a;
        }
        else if (cmp > 0) {
            res = a;
            d = &b;
        }
        else {
            return int2048(0);
        }
        int borrow = 0;
        for (int i = 0; i < std::max(res.digits.size(), d->digits.size()); i++) {
            if (i >= d->digits.size()) {
                if (!borrow) {
                    break;
                }
                res.digits[i] -= borrow;
            }
            else {
                res.digits[i] -= d->digits[i] + borrow;
            }
            if (res.digits[i] < 0) {
                res.digits[i] += 10000;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
        }
        return res;
    }
    else {
        int2048 res;
        res = a;
        int carry = 0;
        res.digits.resize(std::max(a.digits.size(), b.digits.size()));
        for (int i = 0; i < std::max(a.digits.size(), b.digits.size()); i++) {
            if (i < b.digits.size()) {
                res.digits[i] += b.digits[i];
            }
            if (i >= b.digits.size() && carry == 0) {
                break;
            }
            res.digits[i] += carry;
            if (res.digits[i] >= 10000) {
                res.digits[i] -= 10000;
                carry = 1;
            }
            else {
                carry = 0;
            }
        }
        if (carry) {
            res.digits.push_back(1);
        }
        return res;
    }
}

int2048 operator-(const int2048 &a, const int2048 &b) {
    int2048 bn = -b;
    return a + bn;
}

int2048 & int2048::operator+=(const int2048 & a) {
    *this = (*this) + a;
    return *this;
}

int2048 & int2048::operator-=(const int2048 & a) {
    *this = (*this) - a;
    return *this;
}

int2048 &int2048::add(const int2048 & a) {
    return (*this) += a;
}

int2048 add(int2048 a, const int2048 & b) {
    return a + b;
}

int2048 minus(int2048 a, const int2048 & b) {
    return a - b;
}

int2048 & int2048::minus(const int2048 & a) {
    return (*this) -= a;
}

} // namespace sjtu

int main() {
    sjtu::int2048 a, b;
    std::string s1, s2;

    s1 = "-40540403261018770859812117263782292077235318825033122274297327781036232";
    s2 = "52795355647846676860229953609448700736796461284858206682265310505567013";
    // std::cin >> s1 >> s2;
    a.read(s1); b.read(s2);
    std::cout << std::endl;
    add(a, b).print(); puts("");
    std::cout << std::endl;

    minus(a, b).print(); puts("");
    std::cout << std::endl;
    sjtu::int2048 t = a;
    a.minus(b).print(); puts("");
    std::cout << std::endl;

    a.add(b).print(); puts("");
    std::cout << std::endl;

    std::cout << (t == a);

    (add(a, b)).minus(minus(a, b)).print(); puts("");
    std::cout << std::endl;

    b.add(a).add(b).minus(add(a, minus(a, b))).print(); puts("");
}
