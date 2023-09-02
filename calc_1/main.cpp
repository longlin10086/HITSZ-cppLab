#include <iostream>
#include <string>
#include <vector>

class STACK{
private:
    std::string _stack;
public:
    void push(char Char){
        _stack.push_back(Char);
    }
    auto top(){
        return _stack[_stack.size()-1];
    }
    void pop(){
        _stack.pop_back();
    }
    bool empty(){
        if(_stack[_stack.size()-1]) return true;
        return false;
    }
};

using namespace std;

STACK Stack;
vector<char> Number = {'0','1','2','3','4','5','6','7','8','9','.'};
vector<pair<char,string>> Operator = {{'+',"add"},{'-',"sub"},{'*',"mul"},{'/',"div"}};
vector<pair<char,char>> Bracket = {{'{','}'},{'[',']'},{'(',')'}};

int main() {
    string Input;
    cin >> Input;
    string str = "";
    for (int i = 0; i < Input.length(); ++i) {
        char tmp = Input[i];
        bool is_number = false;
        bool is_operator = false;

        for (int j = 0; j < Number.size(); ++j) {
            if (tmp == Number[j]) {
                str += tmp;
                is_number = true;
                break;
            }
        }
        if (i == Input.length()-1 and !str.empty()) {
            cout << "Number: " << str << endl;
            str = "";
        }
//        cout << tmp << endl;
        if (!is_number) {
            if (!str.empty()) {
                cout << "Number: " << str << endl;
                str = "";
            }
            for (int j = 0; j < Operator.size(); ++j) {
                if (tmp == Operator[j].first) {
                    cout << "Operator: " << Operator[j].second << endl;
                    is_operator = true;
                    break;
                }
            }
            if (!is_operator) {
                bool is_left_bracket = false;
                for (auto j = Bracket.begin(); j != Bracket.end(); ++j) {
                    if (tmp == (*j).first) {
                        Stack.push(tmp);
                        is_left_bracket = true;
                        cout << "Operator: " << tmp << endl;
                        break;
                    }
                }
                if (!is_left_bracket) {
                    char last_bracket = Stack.top();
                    bool is_match = false;
                    for (auto k = Bracket.begin(); k != Bracket.end(); ++k) {
                        if (last_bracket == (*k).first && tmp == (*k).second) {
                            Stack.pop();
                            cout << "Operator: " << tmp << endl;
                            is_match = true;
                            break;
                        }
                    }
                    if (!is_match) {
                        cout << is_match << endl;
                        if(i == Input.length()-1 and !Stack.empty()){
                            cout << "Oops! There is a mistake!" << endl;
                            break;
                        }
                        cout << "Operator: " << tmp << endl;
                        cout << "Oops! There is a mistake!" << endl;
                        break;
                    }
                }
            }
        }
    }

}



