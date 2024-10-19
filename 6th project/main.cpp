#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool error = false;

void writeFile(ifstream & File){
    string line;
    cout << "file content:\n";
    while(getline(File, line)){
        cout << line << endl;
    }
}
string removeWhiteSpace(string str){
    string newStr;
    for(int i = 0;i < str.length();i++){
        if(str[i] != ' '){
            newStr+=str[i];
        }
    }
    return newStr;
}
string DecToBinary(int num){
    if(num == 0){
        return "0";
    }
	string bin = "";
	while (num > 0){	
		bin = (num % 2== 0 ? "0" : "1") + bin;
		num/=2;
	}
	return bin;
}
string shiftToRight(string str){
    string shiftedStr = "";
    shiftedStr.resize(str.size());
    char last = str[str.size()-1];

    for(int i = 0;i< str.size();i++){
        shiftedStr[i+1] = str[i];
    }

    shiftedStr[0] = last;
    return shiftedStr;
}
bool containsSymbol(char symbol, string str){
    for(int i = 0;i < str.size();i++){
        if(str[i] == symbol){
            return true;
        }
    }
    return false;
}
void Ainstruction(int currLine, vector <string> & instructions, vector <string> & binaryInstructions){
    
    string binaryInstructionTemplate = "0000000000000000";
    instructions[currLine][0] = ' ';
    string binInstruction = DecToBinary(stoi(instructions[currLine]));

    for(int i = binInstruction.size()-1; i >= 0;i--){
        binaryInstructionTemplate[i] = binInstruction[i];
    }
    int inscSize = 16-binInstruction.size();
    while (inscSize>0)
    {
        binaryInstructionTemplate = shiftToRight(binaryInstructionTemplate);
        inscSize--;
    }
    
    binaryInstructions[currLine] = binaryInstructionTemplate;
    //cout << "A instruction:"<< binaryInstructionTemplate << endl;
}
string jmpToBit(string jmp){
    jmp[3] = ' ';
    if(jmp == "JMP"){
        return "111";
    }
    else if(jmp == "JGT"){
        return "001";
    }
    else if(jmp == "JEQ"){
        return "010";
    }
    else if(jmp == "JGE"){
        return "011";
    }
    else if(jmp == "JLT"){
        return "100";
    }
    else if(jmp == "JNE"){
        return "101";
    }
    else if(jmp == "JLE"){
        return "110";
    }
    else{
        return "000";
    }
}
string cmpToBit(string cmp){
    if(cmp == "0"){
        return "0101010";
    }
    else if(cmp == "1"){
        return "0111111";
    }
    else if(cmp == "-1"){
        return "0111010";
    }
    else if(cmp == "D"){
        return "0001100";
    }
    else if(cmp == "A"){
        return "0110000";
    }
    else if(cmp == "M"){
        return "1110000";
    }
    else if(cmp == "!D"){
        return "0001101";
    }
    else if(cmp == "!A"){
        return "0110001";
    }
    else if(cmp == "!M"){
        return "1110001";
    }
    else if(cmp == "-D"){
        return "0001111";
    }
    else if(cmp == "-A"){
        return "0110011";
    }
    else if(cmp == "-M"){
        return "10110011";
    }
    else if(cmp == "D+1"){
        return "0011111";
    }
    else if(cmp == "A+1"){
        return "0110111";
    }
    else if(cmp == "M+1"){
        return "1110111";
    }
    else if(cmp == "D-1"){
        return "0001110";
    }
    else if(cmp == "A-1"){
        return "0110010";
    }
    else if(cmp == "M-1"){
        return "1110010";
    }
    else if(cmp == "D+A"){
        return "0000010";
    }
    else if(cmp == "D+M"){
        return "1000010";
    }
    else if(cmp == "D-A"){
        return "0010011";
    }
    else if(cmp == "D-M"){
        return "1010011";
    }
    else if(cmp == "A-D"){
        return "0000111";
    }
    else if(cmp == "M-D"){
        return "1000111";
    }
    else if(cmp == "D&A"){
        return "0000000";
    }
    else if(cmp == "D&M"){
        return "1000000";
    }
    else if(cmp == "D|A"){
        return "0010101";
    }
    else if(cmp == "D|M"){
        return "1010101";
    }
    else{
        return "000000";
        error = true;
    }

}
void Cinstruction(int currLine, vector <string> & instructions, vector <string> & binaryInstructions){
    string binaryInstructionTemplate = "1110000000000000";
    string formula = "";
    string instruction = instructions[currLine]; 

    if(containsSymbol(';',instruction)){
        string jmpBit = jmpToBit(instruction.substr(instruction.find(';')+1,instruction.size()-instruction.find(';')-2));
        binaryInstructionTemplate[14] = jmpBit[1];
        binaryInstructionTemplate[15] = jmpBit[2];
        binaryInstructionTemplate[13] = jmpBit[0];
        formula = instruction.substr(0,instruction.find(';'));
    }
    if(containsSymbol('=', instruction)){
        string left = instruction.substr(0,instruction.find('='));
        if(containsSymbol('A', left)){
            binaryInstructionTemplate[10] = '1';
        }
        if(containsSymbol('D', left)){
            binaryInstructionTemplate[11] = '1';
        }
        if(containsSymbol('M', left)){
            binaryInstructionTemplate[12] = '1';
        }   
        formula = instruction.substr(instruction.find('=')+1,instruction.size()-instruction.find('=')-2);
    }
    if(containsSymbol(';',instruction) && containsSymbol('=',instruction)){
        formula = instruction.substr(instruction.find('=')+1,instruction.find(';')-instruction.find('=')-1);
    }

    formula = cmpToBit(formula);
    
    for(int i =0;i< formula.size();i++){
        binaryInstructionTemplate[i+3] = formula[i];
    }

    binaryInstructions[currLine] = binaryInstructionTemplate;
}

int main(int argc, char* argv[])
{	
    if(argc>2){
        cout << "Dont enter more than one file at a time!" << endl;
        return 1;
    }

    string asmFileName = argv[1];
    bool asmFileExists = false;

	ifstream asmFile(asmFileName, ios::in);
    if(!asmFile.good()){
        cout << "File doesnt exist!";
        return 1;
    }
    ofstream hackFile(asmFileName.substr(0,asmFileName.size()-4)+".hack", ios::out);

	int lineCount = 0;
	string currInstructionRaw;
	vector <string> instructions;
	vector <string> binaryInstructions;
	while(getline(asmFile, currInstructionRaw)){
        string currInstruction = (currInstructionRaw);
		if(!((currInstruction[0] == '/' && currInstruction[1] == '/')) && currInstruction.length()!=1){
			lineCount++;
			instructions.emplace_back(currInstruction);
		}       
	}
	
	binaryInstructions.resize(lineCount+1);
    
	for(int i = 0;i < lineCount;i++){ 
        if((instructions[i][0] == '(')){
            
        }
        else if(instructions[i][0] == '@'){
            if((instructions[i][1] >= '0' && instructions[i][1] <= '9')){
			    Ainstruction(i, instructions, binaryInstructions);
            }
            else{

            }
		}
        else{
            Cinstruction(i, instructions, binaryInstructions);
        }
    }

    for(int i = 0;i < lineCount;i++){hackFile << binaryInstructions[i] <<  endl;}

}