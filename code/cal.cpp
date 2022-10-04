#define debug1 printf("(debug)")
#include"number.cpp"
using namespace std;
struct Number;
Number one;
Number Devide_Powers(string s);
Number Formulas(string s);
bool isDigit(char c)
{
    if (c > '9' || c < '0') return 0;
    return 1;
}
/*
Reduce_String(string s):
to reduce the space at the beginning or the end of the string
and the outer brackets
*/
string Reduce_String(string s)
{
    int len = s.length();
    if (len == 0) return s;
    while (s[0] == ' ' || s[len - 1] == ' ' || (s[0] == '(' && s[len - 1] == ')')) {
        if (s[0] == ' ') s = s.substr(1, len - 1);
        else if (s[len - 1] == ' ') s = s.substr(0, len - 1);
        else s = s.substr(1, len - 2);
        len = s.length();
        if (len == 0) return s;
    }
    return s;
}
Number toNumber(string s)
{
    Number num;
    memset(num.fracPart, 0, sizeof(num.fracPart));
    memset(num.intPart, 0, sizeof(num.intPart));
    num.fracLen = 0;
    int type = 0, ePos = 0, numSign = 1, expSign = 1;
    int haveDot = 0, dotPoint = 0, havNumber = 0;
    for (int i = 0;i < s.length();i++)
    if (isDigit(s[i])) havNumber = 1;
    if (!havNumber) {
        num.errorCode = 2;
        return num;
    }
    while (s[0] == ' ') s = s.substr(1, s.length() - 1);
    while (s[s.length() - 1] == ' ') s = s.substr(0, s.length() - 1);
    while (s[0] == '-') {
        numSign *= -1;
        s = s.substr(1,s.length() - 1);
    }
    if (s[0] =='.') {
        s = '0' + s;
    }
    for (int i = 0;i < s.length(); i++){
        if (s[i] == '.' && type == 0) {
            dotPoint = i;
            type = 1;
        }
        else if (s[i] == 'e' && ePos == 0) {
            ePos = i;
            if (type == 1) haveDot = 1; 
            type = 2;
            if (i == 0) type = 3;
            else if (s[i - 1] == '.') type = 3;
            if (i == s.length() - 1) type = 3;
            else if (s[i+1] != '-' && !isDigit(s[i+1])) type = 3;
            else if (s[i+1] == '-') expSign = -1;
            for (int j = i + 2;j < s.length(); j++)
                if (!isDigit(s[j])) type = 3;
            break;
        }
        else if (!isDigit(s[i])) {
            type = 3;
            break;
        }
    }
    if (type == 3) {
        num.errorCode = 2;
        return num;
    }
    if (dotPoint > maxLen) {
        num.errorCode = 1;
        return num;
    }
    for (int i = 0;i < s.length() && s[i] != '.' && s[i] != 'e'; i++) {
        num.intPart[i] = s[i] - '0';
        num.intLen = i + 1;
    }
    reverse(num.intPart, num.intPart + num.intLen);
    if (dotPoint != 0) {
        int beginPos = min(int(s.length() - 1), ePos - 1);
        if (ePos == 0) beginPos = s.length() - 1;
        for (int i = beginPos;s[i] != '.' && i >= 0; i--) {
            num.fracPart[beginPos - i] = s[i] - '0';
            num.fracLen ++;
        }
        reverse(num.fracPart, num.fracPart + num.fracLen);
    }
    if (type == 2) {
        int mul = 0;// the number after "e"
        int pos1 = ePos + 1;
        if (expSign == -1) pos1 ++;
        for (int i = pos1;i < s.length();i++){
            mul = mul * 10 + s[i] - '0';
        } 
        if (expSign == 1 && mul + num.intLen > maxLen) num.errorCode = 1;
        if (expSign == -1 && mul + ePos - dotPoint - 1 > maxLen) num.errorCode = 1;
        if (num.errorCode == 1) {
            return num;
        }
        if (expSign == 1 && mul > 0) {
            for (int i = num.intLen - 1;i >= 0;i--) {
                num.intPart[i + mul] = num.intPart[i];
                num.intPart[i] = 0;
            }
            num.intLen += mul;
            for (int i = 0;i < num.fracLen;i++) {
                if (i < mul) num.intPart[mul - i - 1] = num.fracPart[i];
                else num.fracPart[i - mul] = num.fracPart[i];
                num.fracPart[i] = 0; 
            }
            num.fracLen = max(num.fracLen - mul, 0);
        }
        if (expSign == -1 && mul > 0) {
            for (int i = num.fracLen - 1;i >= 0;i--) {
                num.fracPart[i + mul] = num.fracPart[i];
                num.fracPart[i] = 0;
            }
            num.fracLen += mul;
            for (int i = 0;i < num.intLen;i++) {
                if (i < mul) num.fracPart[mul - i - 1] = num.intPart[i];
                else num.intPart[i - mul] = num.intPart[i];
                num.intPart[i] = 0;
            }
            num.intLen = max(num.intLen - mul, 0);
        }
    }
    for (int i = num.intLen - 1; i >= 0; i--) {
        if (num.intPart[i] == 0) num.intLen --;
        else break;
    }
    for (int i = num.fracLen - 1; i >= 0; i--) {
        if (num.fracPart[i] == 0) num.fracLen --;
        else break;
    }
    if (num.intLen == 0 && num.fracLen == 0)
    numSign = 1;
    num.numSign = numSign;
    return num;
}
Number Devide_Powers(string s)
{
    Number tmp = toNumber(s);
    if (tmp.errorCode == 0) return tmp;
    Number ans;
    int pre = 0;
    one.intLen = one.intPart[0] = 1;
    one.errorCode = one.fracLen = 0;
    one.numSign = 1;
    ans = one;
    tmp.errorCode = 0;
    int brackets = 0; 
    bool mode = 0; // 0 represents *, 1 represents /
    // record the number of brackets, only deal with the outer ones
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') brackets ++;
        if (s[i] == ')') brackets --;
        if (brackets == 0) {
            if (i == s.length() - 1 || s[i + 1] == '*' || s[i + 1] == '/') {
            //    cout << pre << ' ' << i << ' ' << i - pre + 1 << '\n';
            //    cout << pre << ' ' << i << ' ' << s.substr(pre, i - pre + 1) << '\n';
                Number tmp = Formulas(s.substr(pre, i - pre + 1));
                if (tmp.errorCode > 0) return tmp; 
                pre = i + 2;
                if (mode) ans = ans / tmp;
                else ans = ans * tmp;
            //    PrintlnNumber(ans);
            //    PrintlnNumber(tmp);
                if (s[i + 1] == '*') mode = 0;
                else mode = 1;
            }
        }
    }
    return ans;
}
Number Formulas(string s)
{
    s = Reduce_String(s);
//    cout<<"\n<"<<s<<"> = ";printf("\n");
    Number tmp1 = toNumber(s);
    Number ans;
    int pre = 0;
    ans.intLen = ans.fracLen = 0;
    if (tmp1.errorCode == 0) return tmp1;
    int brackets = 0; 
    bool mode = 0; // 0 represents +, 1 represents -
    // record the number of brackets, only deal with the outer ones
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') brackets ++;
        if (s[i] == ')') brackets --;
        if (brackets == 0) {
            if (i == s.length() - 1 || s[i + 1] == '+' || s[i + 1] == '-') {
            	
		//	cout << pre << ' ' << i << '\n'; 
        //        cout << "????????????"<<pre << ' ' << i << ' ' << s.substr(pre, i - pre + 1) << "u\n";
        //        cout << '{';
                Number tmp;
                tmp.numSign = 1;
                tmp = Devide_Powers(s.substr(pre, i - pre + 1));
            //    cout<<tmp.intLen<<'\n';
                if (tmp.errorCode > 0) return tmp; 
                pre = i + 2;
                if (mode) ans = ans - tmp;
                else ans = ans + tmp;//ERROR!!
            //    PrintlnNumber(ans);
            //    PrintlnNumber(tmp);
            //    printf("&&&&&&&&%d %d %d %d %d %d %d&&&&\n",tmp.numSign, ans.errorCode, ans.intLen, tmp.errorCode, tmp.intLen, tmp.fracLen, tmp.intPart[0]);
                if (s[i + 1] == '+') mode = 0;
                else mode = 1;
            }
        }
    }
    return ans;
}
string Formating(string s)
{
    string ans = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == 0) continue;
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
            ans = ans + " " + s[i] + " ";
        }
        else ans = ans + s[i];
    }
    return ans;
}
int main()
{
    while (true) {
        string formula;
        getline(cin, formula);
        cout << "Solution: " << Formating(formula) << " = ";
        PrintlnNumber(Formulas(formula));
    }
}
/*
int main()
{
    string s1, s2;
    cin >> s1;
    cin >> s2;
    Number num1, num2;
    num1 = toNumber(s1);
    num2 = toNumber(s2);
    Number num = num1 / num2;
    if (num.errorCode == 2) cout<<"The input cannot be interpret as numbers!"<<'\n';
    else if (num.errorCode == 1) cout<<"The number is too large or too precise!\n";
    else {
        PrintNumber(num1);
        cout << " + ";
        PrintNumber(num2);
        cout << " = ";
        PrintNumber(num);
    }
    system("pause");
}*/
