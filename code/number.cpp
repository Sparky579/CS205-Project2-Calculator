#include<cstdio>
#include<iostream>
#include<string>
#include<algorithm>
#include<cstring>
#include<map>
const int maxLen = 1000;
using std::max;
using std::min;
using std::cout;
using std::string;
struct Number;
void PrintNumber(Number number);
void PrintlnNumber(Number number);
Number toNumber(string s);
struct Number{
    int errorCode = 0;//0: no error 1: length error 2: type error
    // intPart: higher digit has higher index
    // fracPart: higher digit has lower index
    // this is in order to process digital carrying easier
    // 12.45: intPart:{2,1} fracPart:{4,5} 
    int intPart[maxLen + 10], fracPart[maxLen + 10], numSign = 1;
    int intLen = 0, fracLen = 0;
    friend Number operator *(Number num1, Number num2) {
    //    PrintlnNumber(num1);
    //    PrintlnNumber(num2);
        Number number;
        memset(number.intPart, 0, sizeof(number.intPart));
        memset(number.fracPart, 0, sizeof(number.fracPart));
        if (num1.errorCode >= 1 || num2.errorCode >= 1 ) {
            number.errorCode = max(num1.errorCode, num2.errorCode);
            return number;
        }
        if (num1.fracLen + num2.fracLen > maxLen || num1.intLen + num2.intLen > maxLen) {
            number.errorCode = 1;
            return number;
        }
        for (int i = 0;i < num1.intLen; i++)
        for (int j = 0;j < num2.intLen; j++) {
            number.intPart[i + j] += num1.intPart[i] * num2.intPart[j];
        }
        // i fracdigit * j fracdigit: i+j fracdigit
        for (int i = 0;i < num1.fracLen; i++)
        for (int j = 0;j < num2.fracLen; j++) {
            number.fracPart[i + j + 1] += num1.fracPart[i] * num2.fracPart[j];
        }
        for (int i = 0;i < num1.intLen; i++)
        for (int j = 0;j < num2.fracLen; j++) {
            if (i > j) number.intPart[i - j - 1] += num1.intPart[i] * num2.fracPart[j];
            else number.fracPart[j - i] += num1.intPart[i] * num2.fracPart[j];
        }
        for (int i = 0;i < num1.fracLen; i++)
        for (int j = 0;j < num2.intLen; j++) {
            if (j > i) number.intPart[j - i - 1] += num1.fracPart[i] * num2.intPart[j];
            else number.fracPart[i - j] += num1.fracPart[i] * num2.intPart[j];
        }
        for (int i = num1.fracLen + num2.fracLen + 1;i >= 0;i--) {
            if (i > 0) {
                number.fracPart[i - 1] += number.fracPart[i] / 10;
            }
            else {
                number.intPart[0] += number.fracPart[i] / 10;
            }
            number.fracPart[i] %= 10;
        }
        number.intLen = 0;
        for (int i = 0; i < num1.intLen + num2.intLen - 1 || number.intPart[i] > 0; i++) {
            number.intLen = i + 1;
            number.intPart[i + 1] += number.intPart[i] / 10;
            number.intPart[i] %= 10;
        }
        number.fracLen = num1.fracLen + num2.fracLen + 1;
        for (int i = num1.fracLen + num2.fracLen; i >= 0; i--){
            if (number.fracPart[i] > 0) break;
            number.fracLen = i;
        }
        for (int i = number.intLen - 1; i >= 0; i--) {
        if (number.intPart[i] == 0) number.intLen --;
        else break;
    }
        for (int i = number.fracLen - 1; i >= 0; i--) {
            if (number.fracPart[i] == 0) number.fracLen --;
            else break;
    }
        if (number.intLen > maxLen || number.fracLen > maxLen) number.errorCode = 1;
        number.numSign = num1.numSign * num2.numSign;
        return number;
}
    friend bool operator <(Number num1, Number num2) {
        if (num1.errorCode >= 1 || num2.errorCode >= 1) return 0;
        if (num1.numSign != num2.numSign) return num1.numSign < num2.numSign;
        bool ans = 0, flag = 0;
        if (num1.intLen != num2.intLen) {
            ans = num1.intLen <= num2.intLen;
            if (num1.numSign == -1) ans ^= 1;
            return ans;
        }
        for (int i = num1.intLen - 1; i >= 0; i--) {
            if (num1.intPart[i] != num2.intPart[i]) {
                ans = num1.intPart[i] < num2.intPart[i];
                flag = 1;
                break;
            }
        }
        if (!flag)
        for (int i = 0; i < min(num1.fracLen, num2.fracLen); i++){
            if (num1.fracPart[i] != num2.fracPart[i]) {
                ans = num1.fracPart[i] <= num2.fracPart[i];
                flag = 1;
                break;
            }
        }
        if (!flag) {
            ans = num1.fracLen <= num2.fracLen;
        }
        if (num1.numSign == -1) ans ^= 1;
        return ans;

    }
    friend Number operator +(Number num1, Number num2){
        Number number;
        number.numSign = 1;
        for (int i = 0; i <= max(num1.intLen, num2.intLen) + 1; i++)
        number.intPart[i] = 0;
        for (int i = 0; i <= max(num1.fracLen, num2.fracLen) + 1; i++)
        number.fracPart[i] = 0;
    //    memset(number.intPart, 0, sizeof(number.intPart));
    //    memset(number.fracPart, 0, sizeof(number.fracPart));
        //need memset?
        if (num1.errorCode >= 1 || num2.errorCode >= 1) {
            number.errorCode = max(num1.errorCode, num2.errorCode);
            return number;
        }
        if (num1.numSign == -1 && num2.numSign == -1) {
            number.numSign = -1;
            num1.numSign = 1;
            num2.numSign = 1;
        }
        else if (num1.numSign == -1) {
            num1.numSign = 1;
            number.numSign = (num2 < num1)? -1 : 1;
            num1.numSign = -1;
        }
        else if (num2.numSign == -1) {
            num2.numSign = 1;
            number.numSign = (num1 < num2) ? -1 : 1;
            num2.numSign = -1;
        }
        for (int i = 0; i < num1.intLen; i++){
            num1.intPart[i] *= num1.numSign;
        }
        for (int i = 0; i < num1.fracLen; i++){
            num1.fracPart[i] *= num1.numSign;
        }
        for (int i = 0; i < num2.intLen; i++){
            num2.intPart[i] *= num2.numSign;
        }
        for (int i = 0; i < num2.fracLen; i++){
            num2.fracPart[i] *= num2.numSign;
        }
    //    printf("?%d %d?",num1.intLen, num2.intLen);
        for (int i = 0; i < max(num1.intLen, num2.intLen); i++) {
            number.intPart[i] = 0;
            if (i < num1.intLen) number.intPart[i] += num1.intPart[i];
            if (i < num2.intLen) number.intPart[i] += num2.intPart[i];
            if (number.numSign == -1 && num1.numSign + num2.numSign != 2) number.intPart[i] *= -1;
        //    printf("(%d %d %d %d)\n", i, number.intPart[i], num1.intPart[i], num2.intPart[i]);
        }
        for (int i = 0; i < max(num1.fracLen, num2.fracLen); i++) {
            number.fracPart[i] = 0;
            if (i < num1.fracLen) number.fracPart[i] += num1.fracPart[i];
            if (i < num2.fracLen) number.fracPart[i] += num2.fracPart[i];
            if (number.numSign == -1) number.fracPart[i] *= -1;
        //    printf("(%d %d)\n", i, number.fracPart[i]);
        }
        number.fracLen = 0;
        for (int i = max(num1.fracLen, num2.fracLen) - 1;i >= 0;i--) {
            if (i > 0) {
                while (number.fracPart[i] < 0) {
                    number.fracPart[i] += 10;
                    number.fracPart[i - 1] -= 1;
                }
                number.fracPart[i - 1] += number.fracPart[i] / 10;
            }
            else {
                while (number.fracPart[i] < 0) {
                    number.fracPart[i] += 10;
                    number.intPart[0] -= 1;
                }
                number.intPart[0] += number.fracPart[i] / 10;
            }
            number.fracPart[i] %= 10;
            if (number.fracLen == 0 && number.fracPart[i] != 0) number.fracLen = i + 1;
        }
        number.intLen = 0;
        for (int i = 0; i < max(num1.intLen, num2.intLen) || number.intPart[i] > 0; i++) {
            while (number.intPart[i] < 0) {
                number.intPart[i] += 10;
                number.intPart[i + 1] -= 1;
            }
            number.intLen = i + 1;
            number.intPart[i + 1] += number.intPart[i] / 10;
            number.intPart[i] %= 10;
        }
        while (number.intLen > 0 && number.intPart[number.intLen - 1] == 0)
            number.intLen --;
        if (number.intLen == 0 && number.fracLen == 0) number.numSign = 1;   
        if (number.intLen > maxLen - 1) number.errorCode = 1;
        return number;
    }
    friend Number operator -(Number num1, Number num2) {
        Number num, num3 = num2;
        num3.numSign *= -1;
        num = num1 + num3;
        return num;
    }
    friend Number operator /(Number num1, Number num2) {
        Number now, ten1, ans;
        int tmpSign = 1;//use this variable to save the sign of answer
        //make num1 and num2 positive, so that easier to handle
        //ten1 is a constant number 0.1
        /* principle:
        from a number large enough and in (1000...00) form, 
        each time use num1 minus this number until num1 < the number(now),
        then let the number be divided by 10(*0.1)
        until the set precision
        */
        ten1.errorCode = now.errorCode = ans.errorCode 
        = ten1.intLen = ans.intLen = ans.fracLen = 0;
        ten1.fracLen = ten1.numSign = ans.numSign = now.numSign = 1;
        ten1.fracPart[0] = 1;
        memset(now.fracPart, 0, sizeof (now.fracPart));
        memset(now.intPart, 0, sizeof (now.intPart));
        memset(ans.fracPart, 0, sizeof (ans.fracPart));
        memset(ans.intPart, 0, sizeof (ans.intPart));
        //need memset?
        now.fracLen = 0;
        now.intLen = max(num1.intLen - num2.intLen + 1, 1);
        now.intPart[now.intLen - 1] = 1;
        if (num1.numSign < 0) tmpSign *= -1;
        if (num2.numSign < 0) tmpSign *= -1;
        num1.numSign = num2.numSign = 1;
        int prec = min(maxLen - 1, max(num1.fracLen + num1.intLen, num2.fracLen + num2.intLen) + 1);
        //set precision
        bool deal = 0;
        for (int i = 0; prec >= 0 ; i++) {
            while (now * num2 < num1) {
                deal = 1;
                num1 = num1 - (now * num2);
                ans = ans + now;
            }
            prec -= deal;
            now = now * ten1;
        }
        ans.numSign = tmpSign;
    //    PrintlnNumber(ans);
        return ans;
    }
};
void PrintNumber(Number number)
{
    if (number.numSign == -1) cout<< '-';
    if (number.intLen == 0) {
        cout << 0;
        if (number.fracLen == 0) number.numSign = 1;
    }
    for (int i = number.intLen - 1; i >= 0; i--) {
        cout << number.intPart[i];
    }
    if (number.fracLen > 0) cout << '.';
    for (int i = 0;i < number.fracLen; i++) 
        cout << number.fracPart[i];
}
void PrintlnNumber(Number number)
{
    PrintNumber(number);
    cout << '\n';
}
Number sqrtNumber(Number number)
{
    Number left = toNumber("0");
    Number right;
    Number half = toNumber("0.5");
    Number one = toNumber("1");
    Number eps;
    if (number < left && !(number.intLen == 0 && number.fracLen == 0)) {
        left.errorCode = 2;
        return left;
    }
    right.intLen = number.intLen / 2 + 2;
    right.numSign = eps.numSign = 1;
    right.intPart[number.intLen / 2 + 1] = 1;
    eps.fracLen = number.fracLen + 9;
    eps.fracPart[number.fracLen + 8] = 1;
    while (left < right) {
        Number mid = (left + right) * half;
        Number square = mid * mid;
        Number prec = square - number;
        int tmpSign = prec.numSign;
        prec.numSign = 1;
        if (prec < eps && tmpSign == 1) {
            mid.fracLen = number.fracLen + 2;
            return mid * one;
        }
        else if (square < number) left = mid;
        else right = mid;
    /*    printf("$");
        PrintlnNumber(left);
        PrintlnNumber(right);
        PrintlnNumber(square);
        PrintlnNumber(number);
        printf("\n");*/
    }
}
Number absNumber(Number number)
{
    Number tmp = number;
    tmp.numSign = 1;
    return tmp;
}
