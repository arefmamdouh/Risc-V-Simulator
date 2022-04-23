#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

//Reads file of risc-v instructions
vector<string> readingRiscCode(string filePath) //function to read commands of given risc-v code
{
    vector<string> code;
    string line;
    ifstream MyReadFile(filePath);
    string path;
    while (!MyReadFile) {
        cout << "ERROR!\nPlease enter the path of the file you want read: ";
        cin >> path;
        MyReadFile.open(path);
    }

    while (getline(MyReadFile, line)) {
        //        for (int i = 0; i<line.size(); i++) {
        //            line[i]=tolower(line[i]);
        //        }
        //        cout<<"Lowercase: "<<line<<endl;
        code.push_back(line);

    }

    MyReadFile.close();

    return code;
}

//divides instructions to operands and commands
void divideCode(vector<string>code, vector<string>& commands, vector<string>& operands, map<string, long>& labels) // dividing command longo (add) and (a1,a2,a3)
{
    string lineOfCode;// to pass on the extracted line from the file and divide it longo command and operands
    string command; //(and)
    string operand; // (a1,a2,a3)
    map<string, long>::iterator itr;

    for (long i = 0; i < code.size(); i++) {
        for (long j = 0; j < code[i].size(); j++) {
            if (code[i].at(j) == ':') {
                if (code[i].size() > j + 1) {
                    labels.insert(pair<string, long>(code[i].substr(0, j), i));
                    code[i] = code[i].substr(j + 2, code[i].size());
                }
                else {
                    labels.insert(pair<string, long>(code[i].substr(0, j), i));
                }
                if (code[i].at(0) == ' ') {
                    code[i] = code[i].substr(1, code[i].size());
                }
            }
        }

    }

    for (long i = 0; i < code.size(); i++) {
        lineOfCode = code[i];
        for (long j = 0; j < lineOfCode.size(); j++) {
            if (lineOfCode[j] != 32) //extracting command
                command = command + lineOfCode[j]; // extracting the command assuming there is no space before the command. not " add" but "add" with no space before

            else {
                commands.push_back(command); // pushing the command longo commands vector

                operand = lineOfCode.substr(j, lineOfCode.size());
                operand.erase(std::remove(operand.begin(), operand.end(), ' '), operand.end()); // removing white spaces from
                operands.push_back(operand); // pushing operands to operands vector
                operand = ""; //emptying the command string to be ready for the next operand
                command = ""; //emptying the command string to be ready for the next command
                break;

            }
        }
    }
}

//conversions
long binaryToDecimal(string n)
{
    long long num = stoll(n);
    long dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    long base = 1;

    long temp = num;
    while (temp) {
        long last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}
string decimalToBinary(long n) {
    long long binary = 0, i = 1;
    string binaryEquivalanet;

    while (n > 0) {
        n = n / 2;
        binary = binary + ((n % 2) * i);
        i *= 10;
    }
    binaryEquivalanet = to_string(binary);
    while (binaryEquivalanet.size() < 32) {
        binaryEquivalanet = '0' + binaryEquivalanet;
    }

    return binaryEquivalanet;
}

//returns first, second and third registers in a command
string firstreg(string command) {
    string reg1;
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] != ',')
            reg1 = reg1 + command[i];
        else
            break;
    }



    if (reg1[0] == 'x')
        return reg1;
    else
    {
        if (isdigit(reg1[0])) {
            return reg1.substr(2, reg1.find(')') - 2);
        }
        else
            return reg1;



    }




    return reg1;
}
string secondreg(string command) {
    string reg2 = "";
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    command = command.substr(i + 1, command.size()); // cutting extracted part from the command

    for (i = 0; i < command.size(); i++) {
        if (command[i] != ',')
            reg2 = reg2 + command[i];
        else
            break;
    }

    if (reg2[0] == 'x')
        return reg2;
    else
    {
        if (isdigit(reg2[0])) {
            return reg2.substr(2, reg2.find(')') - 2);
        }
        else
            return reg2;



    }
}
string thirdreg(string command) {
    string reg3 = "";
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    command = command.substr(i + 1, command.size()); // cutting extracted part from the command

    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    reg3 = command.substr(i + 1, command.size()); // cutting extracted part from the command

    if (reg3[0] == 'x')
        return reg3;
    else
    {
        if (isdigit(reg3[0])) {
            return reg3.substr(2, reg3.find(')') - 2);
        }
        else
            return reg3;



    }
}

//returns a value if there is an offset
long firstOffset(string command) {
    string reg1;
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] != ',')
            reg1 = reg1 + command[i];
        else
            break;
    }



    if (reg1[0] == 'x')
        return -1;
    else
    {
        long num = stoi(reg1.substr(0, reg1.find('(')));

        if (num % 4 == 0) {
            return num / 4;
        }
        else {
            return -1; //
        }

    }




}
long secondOffset(string command) {
    string reg2 = "";
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    command = command.substr(i + 1, command.size()); // cutting extracted part from the command

    for (i = 0; i < command.size(); i++) {
        if (command[i] != ',')
            reg2 = reg2 + command[i];
        else
            break;
    }

    if (reg2[0] == 'x')
        return -1;
    else
    {
        long num = stoi(reg2.substr(0, reg2.find('(')));

        if (num % 4 == 0) {
            return num / 4;
        }
        else {
            return -1; //
        }

    }

}
long thirdOffset(string command)
{
    string reg3 = "";
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    command = command.substr(i + 1, command.size()); // cutting extracted part from the command

    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    reg3 = command.substr(i + 1, command.size()); // cutting extracted part from the command

    if (reg3[0] == 'x')
        return -1;
    else
    {
        long num = stoi(reg3.substr(0, reg3.find('(')));

        if (num % 4 == 0) {
            return num / 4;
        }
        else {
            return -1; // number can not be divided by 4 for indexing in array
        }

    }



}

//returns a value if there is a number
long secondimm(string command) {
    string reg2 = "";
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    reg2 = command.substr(i + 1, command.size()); // cutting extracted part from the command

    return stol(reg2);
}
long thirdimm(string command) {
    string reg3 = "";
    long i = 0;
    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    command = command.substr(i + 1, command.size()); // cutting extracted part from the command

    for (i = 0; i < command.size(); i++) {
        if (command[i] == ',')
            break;
    }

    reg3 = command.substr(i + 1, command.size()); // cutting extracted part from the command

    return stol(reg3);
}

//RiscV32 functions
void add(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 + num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 + num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void sub(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 - num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 - num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void addi(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 + rs2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 + rs2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void li(string rd, long rs1, map<string, vector<long>>& m, long f1) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr3 = m.find(rd);

    if (f1 != -1) {
        itr3->second[f1] = rs1;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = rs1;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }

}
void And(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 & num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 & num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void Or(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 | num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 | num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void Xor(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 ^ num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 ^ num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void sll(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 << num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 << num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void srl(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 >> num2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 >> num2;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void slt(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 < num2 ? 1 : 0;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1 < num2 ? 1 : 0;
        //        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void sltu(string rd, string rs1, string rs2, map<string, vector<long>>& m, long f1, long f2, long f3)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = m.find(rd);

    long num1;
    long num2;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f3 != -1) {
        num2 = itr2->second[f3];
    }
    else {
        num2 = itr2->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = abs(num1) < abs(num2) ? 1 : 0;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = abs(num1) < abs(num2) ? 1 : 0;

    }
}
void Andi(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 & rs2;
        //        cout << itr3->first << ":" << itr3->second[f1] << endl;

    }
    else {
        itr3->second[0] = num1 & rs2;
    }
}
void Ori(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 | rs2;
    }
    else {
        itr3->second[0] = num1 | rs2;
    }
}
void Xori(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 ^ rs2;
    }
    else {
        itr3->second[0] = num1 ^ rs2;
    }
}
void slli(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 << rs2;
    }
    else {
        itr3->second[0] = num1 << rs2;
    }
}
void srli(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 >> rs2;
    }
    else {
        itr3->second[0] = num1 >> rs2;
    }
}
void slti(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1 < rs2 ? 1 : 0;
    }
    else {
        itr3->second[0] = num1 < rs2 ? 1 : 0;
    }
}
void sltiu(string rd, string rs1, long rs2, map<string, vector<long>>& m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = abs(num1) < abs(rs2) ? 1 : 0;
    }
    else {
        itr3->second[0] = abs(num1) < abs(rs2) ? 1 : 0;
    }
}
void lw(string rd, string rs1, map<string, vector<long>> m, long f1, long f2)
{
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1) {
        num1 = itr1->second[f2];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f1 != -1) {
        itr3->second[f1] = num1;
    }
    else {
        itr3->second[0] = num1;
    }

}
void lh(string rd, string rs1, map<string, vector<long>> m, long f1, long f2)
{
    string temp;
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1)
    {
        temp = decimalToBinary(itr1->second[f2]);
        temp = temp.substr(24, 32);
        itr1->second[f2] = binaryToDecimal(temp);
        num1 = itr1->second[f2];
    }
    else {

        temp = decimalToBinary(itr1->second[0]);
        temp = temp.substr(24, 32);
        itr1->second[0] = binaryToDecimal(temp);
        num1 = itr1->second[0];
    }

    if (f1 != -1) {
        itr3->second[f1] = num1;
    }
    else {
        itr3->second[0] = num1;
    }

}
void lb(string rd, string rs1, map<string, vector<long>> m, long f1, long f2)
{
    string temp;
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1)
    {
        temp = decimalToBinary(itr1->second[f2]);
        temp = temp.substr(24, 32);
        itr1->second[f2] = binaryToDecimal(temp);
        num1 = itr1->second[f2];
    }
    else {

        temp = decimalToBinary(itr1->second[0]);
        temp = temp.substr(24, 32);
        itr1->second[0] = binaryToDecimal(temp);
        num1 = itr1->second[0];
    }

    if (f1 != -1) {
        itr3->second[f1] = num1;
    }
    else {
        itr3->second[0] = num1;
    }


}
void lbu(string rd, string rs1, map<string, vector<long>> m, long f1, long f2)
{
    string temp;
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;


    if (f2 != -1)
    {
        itr1->second[f2] = abs(itr1->second[f2]);
        temp = decimalToBinary(itr1->second[f2]);
        temp = temp.substr(24, 32);
        itr1->second[f2] = binaryToDecimal(temp);
        num1 = itr1->second[f2];
    }
    else {
        itr1->second[f2] = abs(itr1->second[0]);
        temp = decimalToBinary(itr1->second[0]);
        temp = temp.substr(24, 32);
        itr1->second[0] = binaryToDecimal(temp);
        num1 = itr1->second[0];
    }

    if (f1 != -1) {
        itr3->second[f1] = num1;
    }
    else {
        itr3->second[0] = num1;
    }

}
void lhu(string rd, string rs1, map<string, vector<long>> m, long f1, long f2)
{
    string temp;
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num1;

    if (f2 != -1)
    {
        itr1->second[f2] = abs(itr1->second[f2]);
        temp = decimalToBinary(itr1->second[f2]);
        temp = temp.substr(16, 32);
        itr1->second[f2] = binaryToDecimal(temp);
        num1 = itr1->second[f2];
    }
    else {
        itr1->second[f2] = abs(itr1->second[0]);
        temp = decimalToBinary(itr1->second[0]);
        temp = temp.substr(16, 32);
        itr1->second[0] = binaryToDecimal(temp);
        num1 = itr1->second[0];
    }

    if (f1 != -1) {
        itr3->second[f1] = num1;
    }
    else {
        itr3->second[0] = num1;
    }

}
void sw(string rd, string rs1, map<string, vector<long>>& m, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr3 = m.find(rd);
    itr1 = m.find(rs1);

    long num1;

    if (f1 != -1) {
        num1 = itr3->second[f1];
    }
    else {
        num1 = itr3->second[0];
    }
    if (f2 != -1) {
        itr1->second[f2] = num1;
    }
    else {
        itr1->second[0] = num1;
    }

}
void sh(string rd, string rs1, map<string, vector<long>>& m, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr3 = m.find(rd);
    itr1 = m.find(rs1);
    //    cout <<"Before saving half word:"<< itr1->first << "=" << decimalToBinary(itr1->second) << endl;
    string temp;
    long num1;

    if (f1 != -1) {
        temp = decimalToBinary(itr3->second[f1]);
        temp = temp.substr(16, temp.size());
        itr3->second[f1] = binaryToDecimal(temp);
        num1 = itr3->second[f1];
    }
    else {
        temp = decimalToBinary(itr3->second[0]);
        temp = temp.substr(16, temp.size());
        itr3->second[0] = binaryToDecimal(temp);
        num1 = itr3->second[0];
    }
    if (f2 != -1) {
        itr1->second[f2] = num1;
    }

    else {
        itr1->second[0] = num1;
    }


}
void sb(string rd, string rs1, map<string, vector<long>>& m, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr3 = m.find(rd);
    itr1 = m.find(rs1);


    string temp;
    long num1;


    if (f1 != -1) {
        temp = decimalToBinary(itr3->second[f1]);
        temp = temp.substr(24, temp.size());
        itr3->second[f1] = binaryToDecimal(temp);
        num1 = itr3->second[f1];
    }
    else {
        temp = decimalToBinary(itr3->second[0]);
        temp = temp.substr(24, temp.size());
        itr3->second[0] = binaryToDecimal(temp);
        num1 = itr3->second[0];
    }

    if (f2 != -1) {
        itr1->second[f2] = num1;
    }
    else {
        itr1->second[0] = num1;
    }


}
long beq(string rs1, string rs2, string rd, map<string, vector<long>>& m, map<string, long> labels, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = labels.find(rd);

    long num1;
    long num2;

    if (f1 != -1) {
        num1 = itr1->second[f1];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f2 != -1) {
        num2 = itr2->second[f2];
    }
    else {
        num2 = itr2->second[0];
    }
    if (num1 == num2) {
        return itr3->second;
    }
    else {
        return -1;
    }
}
long bne(string rs1, string rs2, string rd, map<string, vector<long>>& m, map<string, long> labels, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = labels.find(rd);

    long num1;
    long num2;

    if (f1 != -1) {
        num1 = itr1->second[f1];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f2 != -1) {
        num2 = itr2->second[f2];
    }
    else {
        num2 = itr2->second[0];
    }
    if (num1 != num2) {
        return itr3->second;
    }
    else {
        return -1;
    }
}
long blt(string rs1, string rs2, string rd, map<string, vector<long>>& m, map<string, long> labels, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = labels.find(rd);

    long num1;
    long num2;

    if (f1 != -1) {
        num1 = itr1->second[f1];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f2 != -1) {
        num2 = itr2->second[f2];
    }
    else {
        num2 = itr2->second[0];
    }
    if (num1 < num2) {
        return itr3->second;
    }
    else {
        return -1;
    }
}
long bge(string rs1, string rs2, string rd, map<string, vector<long>>& m, map<string, long> labels, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = labels.find(rd);

    long num1;
    long num2;

    if (f1 != -1) {
        num1 = itr1->second[f1];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f2 != -1) {
        num2 = itr2->second[f2];
    }
    else {
        num2 = itr2->second[0];
    }
    if (num1 >= num2) {
        return itr3->second;
    }
    else {
        return -1;
    }
}
long bltu(string rs1, string rs2, string rd, map<string, vector<long>>& m, map<string, long> labels, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = labels.find(rd);

    long num1;
    long num2;

    if (f1 != -1) {
        num1 = itr1->second[f1];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f2 != -1) {
        num2 = itr2->second[f2];
    }
    else {
        num2 = itr2->second[0];
    }
    if (abs(num1) < abs(num2)) {
        return itr3->second;
    }
    else {
        return -1;
    }
}
long bgeu(string rs1, string rs2, string rd, map<string, vector<long>>& m, map<string, long> labels, long f1, long f2) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr2;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr2 = m.find(rs2);
    itr3 = labels.find(rd);

    long num1;
    long num2;

    if (f1 != -1) {
        num1 = itr1->second[f1];
    }
    else {
        num1 = itr1->second[0];
    }
    if (f2 != -1) {
        num2 = itr2->second[f2];
    }
    else {
        num2 = itr2->second[0];
    }
    if (abs(num1) >= abs(num2)) {
        return itr3->second;
    }
    else {
        return -1;
    }
}
long jal(string rs1, string rd, map<string, vector<long>>& m, map<string, long> labels, long i) {
    map<string, vector<long>>::iterator itr1;
    map<string, long>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = labels.find(rd);


    itr1->second[0] = i + 1;

    return itr3->second;
}
long jalr(string rs1, string rd, map<string, vector<long>>& m, map<string, long> labels) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr1 = m.find(rs1);
    itr3 = m.find(rd);

    long num2;

    num2 = itr3->second[0];

    return num2;
}
void lui(string rd, long rs1, map<string, vector<long>>& m, long f1) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr3 = m.find(rd);
    long num1;
    string temp;
    string temp2 = "000000000000";

    temp = decimalToBinary(rs1);
    temp = temp + temp2;
    num1 = binaryToDecimal(temp);
    if (f1 != -1) {

        itr3->second[f1] = num1;
        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = num1;
        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}
void auipc(string rd, long rs1, map<string, vector<long>>& m, long i, long f1) {
    map<string, vector<long>>::iterator itr1;
    map<string, vector<long>>::iterator itr3;
    itr3 = m.find(rd);
    long num1;

    string temp;
    string temp2 = "000000000000";

    temp = decimalToBinary(rs1);
    temp = temp + temp2;
    num1 = binaryToDecimal(temp);
    if (f1 != -1) {

        itr3->second[f1] = i + num1;
        cout << itr3->first << ":" << itr3->second[f1] << endl;
    }
    else {
        itr3->second[0] = i + num1;
        cout << itr3->first << ":" << itr3->second[0] << endl;
    }
}

//run through instructions and identifies what each instrucion is
void identify(vector<string> operand, vector<string> command, map<string, vector<long>>& m, map<string, long> labels) {
    map<string, vector<long>>::iterator itr1;
    long i = 0;
    long f1, f2, f3;
    while (i < operand.size()) {

        if (command[i] == "add") { //1
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            add(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "sub") {//2
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            sub(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "addi") {//3
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            addi(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "li") {//4
            f1 = firstOffset(operand[i]);
            li(firstreg(operand[i]), secondimm(operand[i]), m, f1);
        }
        else if (command[i] == "and") {//5
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            And(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "or") {//5
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            Or(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "xor") {//6
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            Xor(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "sll") {//7
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            sll(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "srl") {//8
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            srl(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "sla") {// negative sign is taken longo consideration so the same sll() function is used //9
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            sll(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "sra") { // negative sign is taken longo consideration so the same srl() function is used //10
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            srl(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "slt") {//11
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            slt(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "sltu") {//12
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            f3 = thirdOffset(operand[i]);
            sltu(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, f1, f2, f3);
        }
        else if (command[i] == "andi") {//13
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            Andi(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "ori") {//14
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            Ori(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "xori") {//15
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            Xori(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "slli") {//16
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            slli(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "srli") {//17
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            srli(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "slai") {// negative sign is taken longo consideration so the same sll() function is used //18
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            slli(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "srai") { // negative sign is taken longo consideration so the same srl() function is used //19
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            srli(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "slti") {//20
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            slti(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "sltiu") {//21
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            sltiu(firstreg(operand[i]), secondreg(operand[i]), thirdimm(operand[i]), m, f1, f2);
        }
        else if (command[i] == "lw") {//22
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            lw(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "lh") {//23
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            lh(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "lb") {//24
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            lb(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "lbu") {//25
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            lbu(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "lhu") {//26
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            lhu(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "sw") {//27
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            sw(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "sh") {//28
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            sh(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "sb") {//29
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            sb(firstreg(operand[i]), secondreg(operand[i]), m, f1, f2);
        }
        else if (command[i] == "beq") {//30
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);

            if (beq(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2) != -1) {

                long j = beq(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2);
                i = j - 1;
            }
        }
        else if (command[i] == "bne") {//31
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            if (bne(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2) != -1) {
                long j = bne(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2);
                i = j - 1;
            }
        }
        else if (command[i] == "blt") {//32
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            if (blt(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2) != -1) {
                long j = blt(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2);
                i = j - 1;
            }
        }
        else if (command[i] == "bge") {//33
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            if (bge(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2) != -1) {
                long j = bge(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2);
                i = j - 1;
            }
        }
        else if (command[i] == "bltu") {//34
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            if (bltu(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2) != -1) {
                long j = bltu(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2);
                i = j - 1;
            }
        }
        else if (command[i] == "bgeu") {//35
            f1 = firstOffset(operand[i]);
            f2 = secondOffset(operand[i]);
            if (bgeu(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2) != -1) {
                long j = bgeu(firstreg(operand[i]), secondreg(operand[i]), thirdreg(operand[i]), m, labels, f1, f2);
                i = j - 1;
            }
        }
        else if ((command[i] == "FENCE") || (command[i] == "ECALL") || (command[i] == "EBREAK")) {
            cout << "Ending program execution..." << endl;
            break;
        }
        else if (command[i] == "jal") {
            long j = jal(firstreg(operand[i]), secondreg(operand[i]), m, labels, i);
            i = j - 1;
        }
        else if (command[i] == "jalr") {
            long j = jalr(firstreg(operand[i]), secondreg(operand[i]), m, labels);
            i = j - 1;
        }
        else if (command[i] == "lui") {
            lui(firstreg(operand[i]), secondimm(operand[i]), m, f1);
        }
        else if (command[i] == "auipc") {
            auipc(firstreg(operand[i]), secondimm(operand[i]), m, i, f1);
        }

        itr1 = m.find("x0");
        for (int k = 0; k < 32; k++) {
            cout << itr1->first << ": " << itr1->second[0] << endl;
            itr1++;
        }
        cout << endl;
        cout << "------------------------INSTRUCTION DONE-----------------------" << endl;
        i++;
    }
}

int main() {
    map<string, vector<long>> m;
    map<string, vector<long>>::iterator itr;

    vector<long> z[32];

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 8; j++) {
            z[i].push_back(0);
        }
    }
    m.insert(pair<string, vector<long>>("x0", z[0]));
    m.insert(pair<string, vector<long>>("x1", z[1]));
    m.insert(pair<string, vector<long>>("x2", z[2]));
    m.insert(pair<string, vector<long>>("x3", z[3]));
    m.insert(pair<string, vector<long>>("x4", z[4]));
    m.insert(pair<string, vector<long>>("x5", z[5]));
    m.insert(pair<string, vector<long>>("x6", z[6]));
    m.insert(pair<string, vector<long>>("x7", z[7]));
    m.insert(pair<string, vector<long>>("x8", z[8]));
    m.insert(pair<string, vector<long>>("x9", z[9]));
    m.insert(pair<string, vector<long>>("x10", z[10]));
    m.insert(pair<string, vector<long>>("x11", z[11]));
    m.insert(pair<string, vector<long>>("x12", z[12]));
    m.insert(pair<string, vector<long>>("x13", z[13]));
    m.insert(pair<string, vector<long>>("x14", z[14]));
    m.insert(pair<string, vector<long>>("x15", z[15]));
    m.insert(pair<string, vector<long>>("x16", z[16]));
    m.insert(pair<string, vector<long>>("x17", z[17]));
    m.insert(pair<string, vector<long>>("x18", z[18]));
    m.insert(pair<string, vector<long>>("x19", z[19]));
    m.insert(pair<string, vector<long>>("x20", z[20]));
    m.insert(pair<string, vector<long>>("x21", z[21]));
    m.insert(pair<string, vector<long>>("x22", z[22]));
    m.insert(pair<string, vector<long>>("x23", z[23]));
    m.insert(pair<string, vector<long>>("x24", z[24]));
    m.insert(pair<string, vector<long>>("x25", z[25]));
    m.insert(pair<string, vector<long>>("x26", z[26]));
    m.insert(pair<string, vector<long>>("x27", z[27]));
    m.insert(pair<string, vector<long>>("x28", z[28]));
    m.insert(pair<string, vector<long>>("x29", z[29]));
    m.insert(pair<string, vector<long>>("x30", z[30]));
    m.insert(pair<string, vector<long>>("x31", z[31]));

    vector<string> code;
    vector<string> commands;
    vector<string> operands;
    map<string, long> labels;
    string path;
    cout << "Please enter the path of the file you want read: ";
    cin >> path;
    code = readingRiscCode(path);


    divideCode(code, commands, operands, labels);
    identify(operands, commands, m, labels);
}