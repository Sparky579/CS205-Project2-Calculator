#define debug1 printf("(debug)")
#include"number.cpp"
using std::map;
using std::cin;
using std::reverse;
struct Number;
Number one;
Number Devide_Powers(string s);
Number Formulas(string s);
map<string, Number> vars;
bool isDigit(char c)
{
    if (c > '9' || c < '0') return 0;
    return 1;
}
int is_equation(string s)
{
    /*
        if it has one "=", return the position(is an equation)
        if it has 0 "=", return -1
        if it has more than 1, return -10000(WRONG equation)
    */
    int equat = 0, pos = -1;
    for (int i = s.length() - 1;i >= 0; i--) {
        if (s[i] == '=') {
            equat++;
            pos = i;
        }
    }
    if (equat > 1 || pos == 0) return -10000; // the first char can not be "=" 
    else if (equat == 1) return pos;
    else return -1;
}
bool Can_Be_Var(string s)
{
    if (isDigit(s[0])) return 0;
    for (int i = 0; i < s.length(); i++){
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') return 0;
        if (s[i] == '=') return 0;
    }
    return 1;
}
/*
    to examine whether the formula is function(something) form
    we can't check it only by brackets,
    for example, sqrt(5)*sqrt(5) is not a single function, 
    although there are brackets at both the front and the end of the formula,
    but "5)*sqrt(5" is not a correct formula.
    so we need to count the quantity of brackets.
*/
bool Is_Function(string s, bool strict)
{
    int brackets = 0;
    bool counting = 0;
    if (s[s.length() - 1] != ')') return 0;
    if (s[0] == '(' && strict) return 0;
    for (int i = 0; i < s.length() - 1; i++){
        if (counting == 0 && (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'))
            return 0;
        if (s[i] == '(') {
            counting = 1;
            brackets ++;
        } 
        if (s[i] == ')') brackets --;
        if (counting && brackets < 1) return 0;
    }
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
    bool isFunc = Is_Function(s, 0);
    while (s[0] == ' ' || s[len - 1] == ' ' || (s[0] == '(' && s[len - 1] == ')' && isFunc)) {
        if (s[0] == ' ') s = s.substr(1, len - 1);
        else if (s[len - 1] == ' ') s = s.substr(0, len - 1);
        else s = s.substr(1, len - 2);
        len = s.length();
        if (len == 0) return s;
    }
    return s;
}
/*
    To examine weather the brackets are correct
*/
bool Correct_Brackets(string s)
{
    int brackets = 0;
    for (int i = 0; i < s.length(); i++){
        if (s[i] == '(') brackets ++;
        if (s[i] == ')') brackets --;
        if (brackets < 0) return 0;
        if (i > 0 && s[i - 1] == '(' && s[i] == ')') return 0;
    }
    return brackets == 0;
}
/*
    to convert a string to a struct Number
*/
Number toNumber(string s)
{
    s = Reduce_String(s);
    if (vars.find(s) != vars.end()) return vars[s];
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
Number Function_Process(string s)
{
    if (s.substr(0, 4) == "sqrt" ) {
        s = s.substr(4, s.length() - 4);
        return sqrtNumber(Formulas(s));
    }   
    else if (s.substr(0, 3) == "abs") {
        s = s.substr(3, s.length() - 3);
        return absNumber(Formulas(s));
    }
    return toNumber("quit");
}
Number Devide_Powers(string s)
{
    s = Reduce_String(s);
    if (Is_Function(s, 1) && s[s.length() - 1] == ')') 
        return Function_Process(s);
    Number tmp = toNumber(s);
    if (tmp.errorCode == 0) return tmp;
    Number ans;
    int pre = 0;
    bool haveSign = 0;//to throw an error if no sign
    one.intLen = one.intPart[0] = 1;
    one.errorCode = one.fracLen = 0;
    one.numSign = 1;
    ans = one;
    tmp.errorCode = 0;
    int brackets = 0; 
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') brackets ++;
        if (s[i] == ')') brackets --;
        if (brackets == 0 && (s[i] == '+' || s[i] == '-')) return Formulas(s);
    }
    brackets = 0;
    bool mode = 0; // 0 represents *, 1 represents /
    // record the number of brackets, only deal with the outer ones
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') brackets ++;
        if (s[i] == ')') brackets --;
        if (brackets == 0) {
            if (i != 0 && (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')) haveSign = 1;
            if (i == s.length() - 1 || s[i + 1] == '*' || s[i + 1] == '/') {
                if (pre == 0 && i == s.length() - 1 && !haveSign) {
                    ans.errorCode = 2;
                    return ans;
                }
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
    Number tmp1 = toNumber(s);
    if (s == "-" || s == "+" || s == "*" || s == "/" || ! Correct_Brackets(s) || s == "()") {
        tmp1.errorCode = 2;return tmp1;
    }
    Number ans;
    s = Reduce_String(s);
    if (Is_Function(s, 1)) 
        return Function_Process(s);
    for (int i = 1; i < s.length(); i++) {
        if (s[i] == '-' && s[i - 1] == '(' && s[i - 1] != 'e' && !isDigit(s[i - 1]))
            s.insert(i, "0");
        else if (s[i] == '-' && ! isDigit(s[i - 1]) && s[i - 1] != ')' && s[i - 1] != 'e'){
            tmp1.errorCode = 2;
            return tmp1;
        }
    }
//    cout<<"\n<"<<s<<"> = ";printf("\n");
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
                if (ans.errorCode > 0) return ans;
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
    for (auto i = s.begin(); i != s.end(); i++){
        if (*i == ' ') {s.erase(i); i--;}
    }
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
        if (formula.length() == 0) continue;
        if (formula == "quit") return 0;
        int equat = is_equation(formula);
        if (equat >= 0) {
            Number num = Formulas(formula.substr(equat + 1, formula.length() - equat));
            string varName = Reduce_String(formula.substr(0, equat));
            if (!Can_Be_Var(varName) || varName == "quit") cout << "This var name is illegel!\n";
            else if (num.errorCode != 0 || varName == "quit" || varName == "=" ) {
                cout << "Can not assign this value!" << '\n';
            } 
            else {
                cout << "Assigned Successfully!" << "\n";
                // cout << Reduce_String(formula.substr(0, equat)) << " " << formula.substr(equat + 1, formula.length() - equat) << "$\n";
                vars[Reduce_String(formula.substr(0, equat))] 
                = Formulas(formula.substr(equat + 1, formula.length() - equat));
            }
        }
        else if (equat == -1)
        {
            Number ans = Formulas(formula);
            if (ans.errorCode == 0) {
                cout << "Solution: " << Formating(formula) << " = ";
                PrintlnNumber(Formulas(formula));
            }
            else {
                cout << "Error occurred! Please examine your typing" << '\n';
            }
        }
        else {
            cout << "You are typing a wrong equation!" << '\n';
        }
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
