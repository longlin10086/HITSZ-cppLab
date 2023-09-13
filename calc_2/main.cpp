#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

using namespace std;

template<class type>
class STACK{
private:
    vector<type> _stack;
public:
    void push(type Char){
        _stack.push_back(Char);
    }
    auto top(){
        return _stack[_stack.size()-1];
    }
    void pop(){
        _stack.pop_back();
    }
    bool empty(){
        return _stack.empty();
    }
};

using namespace std;

STACK<double> Stack_num;
STACK<char> Stack_operator;
vector<char> Number = {'0','1','2','3','4','5','6','7','8','9','.'};
vector<pair<char, char>> Bracket = {{'{','}'},{'[',']'},{'(',')'}};
map<char, int> operator_level = {{'+',1},{'-',1},{'*',2},{'/',2}};


class IDetect{
public:

    bool detect_num(char& current){
        bool is_num = false;
        for (int j = 0; j < Number.size(); ++j) {
            if(current == Number[j]){
                is_num = true;
            }
        }
        return is_num;
    }

    bool detect_left_bracket(char& current){
        bool is_left_bracket = false;
        for (int i = 0; i < Bracket.size(); ++i) {
            if(Bracket[i].first == current){
                is_left_bracket = true;
            }
        }
        return is_left_bracket;
    }

    bool detect_right_bracket(char& current){
        bool is_right_bracket = false;
        for (int i = 0; i < Bracket.size(); ++i) {
            if(Bracket[i].second == current){
                is_right_bracket = true;
            }
        }
        return is_right_bracket;
    }

    bool detect_match(char& current_left, char& current_right) {
        for (int j = 0; j < Bracket.size(); ++j) {
            if ((current_left == Bracket[j].first) and (current_right == Bracket[j].second)) return true;
        }
        return false;
    }

    void operate_num(char& top_operator){
        double x = Stack_num.top();
        Stack_num.pop();
        double y = Stack_num.top();
        Stack_num.pop();
        if (top_operator == '+') Stack_num.push(y+x);
        else if(top_operator == '-') Stack_num.push(y-x);
        else if(top_operator == '*') Stack_num.push(y*x);
        else if(top_operator == '/') Stack_num.push(y/x);
    }

};

unique_ptr detect_char = make_unique<IDetect>();

void bounce_stack();

int main() {
    string Input;
    cin >> Input;
    string num = "";
    for (int i = 0; i < Input.size(); ++i) {
        char current_char = Input[i];

        // detect the num
        if(detect_char->detect_num(current_char)) {
            num += current_char;
        }
        // the end num
        if ((i == Input.size() - 1) and (!num.empty())) {
            Stack_num.push(stod(num));
            num = "";
        }
        // detect not the num
        if (!detect_char->detect_num(current_char)) {
            if(!num.empty()){
                Stack_num.push(stod(num));
                num = "";
            }
            // normal operator
            if (!detect_char->detect_right_bracket(current_char) and !detect_char->detect_left_bracket(current_char)) {
                // stack is empty or match the push condition
                if ((Stack_operator.empty()) or (operator_level[current_char] > operator_level[Stack_operator.top()])) {
                    Stack_operator.push(current_char);
                }
                // not match the push condition
                else {
                    bounce_stack();
                    Stack_operator.push(current_char);
                }
            }

            // detect right bracket
            else if (detect_char->detect_right_bracket(current_char)) {
                bool is_match = false;
                while (!Stack_operator.empty()) {
                    char tmp = Stack_operator.top();
                    // detect the left bracket
                    if (detect_char->detect_left_bracket(tmp)) {
                        // match the right
                        if (detect_char->detect_match(tmp, current_char)) {
                            is_match = true;
                            Stack_operator.pop();
                        }
                        // not match the right side
                        break;
                    } else {
                        Stack_operator.pop();
                        // match the push condition
                        if ((operator_level[tmp] <= operator_level[Stack_operator.top()]) and !Stack_operator.empty()) {
                            Stack_operator.push(tmp);
                            break;
                        }
                        detect_char->operate_num(tmp);
                    }
                }
                // not match the right side
                if (!is_match){
                    cout << "There is something wrong!" << endl;
                    return 0;
                }
            }

            // detect left bracket
            else if (detect_char->detect_left_bracket(current_char)) {
                Stack_operator.push(current_char);
            }

        }

    }

    // reading finished
    bounce_stack();
    cout << "The answer is: " << Stack_num.top() << endl;
}

void bounce_stack() {
    while (!Stack_operator.empty()) {
        char tmp = Stack_operator.top();
        Stack_operator.pop();
        // detect the left bracket
        if (detect_char->detect_left_bracket(tmp)) {
            Stack_operator.push(tmp);
            break;
        }
        detect_char->operate_num(tmp);
    }
}



