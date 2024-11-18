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
    cut();
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
    digits.clear();
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
    cut();
}

int2048::int2048(const int2048 & num) {
    digits = num.digits;
    signal = num.signal;
}

void int2048::print() {
    std::cout << *this;
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

bool operator!=(const int2048 &a, const int2048 &b) {
    return !(a==b);
}

bool operator==(const int2048 &a, const int2048 &b) {
    if (a.digits.size()==1&&a.digits[0]==0&&b.digits.size()==1&&b.digits[0]==0) {
        return true;
    }
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
        int2048 d;
        int2048 res;
        int cmp = absCmp(a, b);
        if (cmp < 0) {
            res = b;
            d = a;
        }
        else if (cmp > 0) {
            res = a;
            d = b;
        }
        else {
            return int2048(0);
        }
        res.cut();
        d.cut();
        int borrow = 0;
        for (int i = 0; i < res.digits.size(); i++) {
            if (i >= d.digits.size()) {
                if (!borrow) {
                    break;
                }
                res.digits[i] -= borrow;
            }
            else {
                res.digits[i] -= d.digits[i] + borrow;
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
        for (int i = 0; i < res.digits.size(); i++) {
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
        res.cut();
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

int2048 & int2048::operator*=(const int2048 & a) {
    *this = (*this) * a;
    return *this;
}

std::istream &operator>>(std::istream &is, int2048 &s) {
    std::string str;
    is >> str;
    s.read(str);
    return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &n) {
    for (int i = n.digits.size()-1; i >= 0; i--) {
        if (n.digits[i] != 0) {
            if (!n.signal) {
                os << '-';
            }
            os << n.digits[i];
            for (int j = i-1; j >= 0; j--) {
                if (n.digits[j] == 0) {
                    os << "0000";
                }
                else if (n.digits[j]<10) {
                    os << "000" << n.digits[j];
                }
                else if (n.digits[j]<100) {
                    os << "00" << n.digits[j];
                }
                else if (n.digits[j]<1000) {
                    os << "0" << n.digits[j];
                }
                else {
                    os << n.digits[j];
                }
            }
            return os;
        }
    }
    os << '0';
    return os;
}

int2048 operator*(int2048 a, const int2048 & b) {
    int2048 res;
    res.signal = a.signal == b.signal;
    res.digits.resize(a.digits.size() + b.digits.size());
    for (int i = 0; i < b.digits.size(); i++) {
        for (int j = 0; j < a.digits.size(); j++) {
            res.digits[i+j] += a.digits[j] * b.digits[i];
        }
        int carry = 0;
        for (int j = 0; j < res.digits.size(); j++) {
            res.digits[j] += carry;
            if (res.digits[j] >= 10000) {
                carry = res.digits[j] / 10000;
                res.digits[j] %= 10000;
            }
            else {
                carry = 0;
            }
        }
        if (carry) {
            res.digits.push_back(carry);
        }
    }
    
    res.cut();
    return res;
}

int2048 & int2048::operator/=(const int2048 & b) {
    (*this) = (*this) / b;
    return *this;
}

int2048 operator/(int2048 a, const int2048 & b_) {
    bool signal;
    if (a.signal == b_.signal) {
        signal = true;
    }
    else {
        signal = false;
    }
    int2048 b(b_.digits, true);
    a.signal = true;
    int cmp = absCmp(b, int2048(10000));
    if (cmp == 0) {
        a.digits.erase(a.digits.begin());
        if (a.digits.empty()) {
            a.digits.push_back(0);
        }
        a.signal = signal;
        return a;
    }
    else if (cmp < 0) {
        for (int i = a.digits.size()-1; i >= 0; i--) {
            if (i > 0) {
                a.digits[i-1] += a.digits[i] % b.digits[0] * 10000;
            }
            a.digits[i] /= b.digits[0];
        }
        a.signal = signal;
        return a;
    }
    int2048 l(0);
    int2048 r = a / int2048(10000);
    int2048 mid, ans = l;
    while (l <= r) {
        mid = (l + r) / int2048(2);
        if (mid * b > a) {
            r = mid - 1;
        }
        else {
            l = mid + 1;
            ans = mid;
        }
    }
    ans.signal = signal;
    return ans;
}

int2048 & int2048::operator%=(const int2048 & b) {
    *this = *this - (*this / b) * b;
    return *this;   
}

int2048 operator%(int2048 a, const int2048 & b) {
    a %= b;
    return a;
}

} // namespace sjtu

int main() {
    sjtu::int2048 a("-695428898082480522332164088268129677800793802871414506171749557459180009155183212043517794276");
    sjtu::int2048 b("-495508229187833544681199276240365769370959345897378468521329396734317523064960616872159548113");
    std::cout << a * b << "\n\n";
    std::cout << (a *= b) << "\n\n";
    a *= b;
    a *= a;
    a *= b;
    std::cout << (b *= a) << '\n';
}
