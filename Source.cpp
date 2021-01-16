//////////////////// HEADER FILES ////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stack>
#include <sstream>
using namespace std;

//////////////////// Struct Declaration && SYMBOL table //////////////////////////////
struct Token {
	string type=" ";
	vector <string> value;
	int count= 0;
};
struct Token_Temp {
	string type = " ";
	string value = " ";
	int count = 0;
};
struct Var_val
{
	string ident="";
	int ident_value=0;
};
string keyword[35] = { "auto", "break", "case", "char", "const", "continue","cout","default",
					"do", "double", "else","elseif", "enum", "extern", "float", "for", "goto",
					"if", "int", "long","main","register", "return", "short", "signed",
					"sizeof", "static", "struct", "switch", "typedef", "union",
					"unsigned", "void", "volatile", "while" };
vector <Var_val> vtable;
Token Keyword, Identifier, Seprator, Number, Operator, Comment, Output;
char seprator[8] = { ',',';','{','}','[',']','(',')' }; //symbol table 1	Reason for 2 symbol tables is because the array 
char OOperator[8] = { '+','-','=','*','/','%','<','>' };	//symbol table 2	will be searched much faster depending on the token
vector <string> Treeval;
//////////////////// HELPING FUNCTIONS & File Handling //////////////////////////////
string converttostring(char ch[], int size)
{
	string tmp;
	for (int i = 0; i < size; i++)
	{
		if (isalpha(ch[i]))
		{
			tmp = tmp + ch[i];
		}
	}
	return tmp;
}		
string convertstring(char ch)
{
	string tmp = "";
	tmp = ch;
	return tmp;
}
void initalize(char arg[], int size)
{
	for (int i = 0; i < size; i++)
	{
		arg[i] = ' ';
	}
}
bool not_present(Token_Temp arg)
{
	if (arg.type == "Identifier")
	{
		for (int i = 0; i < Identifier.value.size(); i++)
		{
			if (arg.value == Identifier.value[i])
			{
				return false;
			}
		}
	}
	return true;
}
ifstream ftl,fl,*cfl;		//Variable to access files
int inx = 0; // value table index
char arg,tmparg;  //imp dont delete takes the next char from file 
string s = "";



///////////////////////////// Forward prototyping & helping lexi analyzer //////////////////////////////////////
void parser(Token_Temp);
void Insert_Token(Token_Temp);
Token_Temp get_Tokentmp()
{
	Token_Temp Found_Token;
	string tmp = " ";
	int size = 0, comment = 0, ind = 0, indx = 0, cindx = 0, check;

	if (!cfl->is_open())
	{
		cerr << "File not open!" << endl;
	}
	else
	{
		char key[10] = { '\0' }, line[50] = { '\0' }, com[50] = { '\0' };
		ind = indx = cindx = check = 0;
		if (tmparg == '\n') // newline check
		{
			Found_Token.type = "Seprator";
			Found_Token.value = "\n";
			Found_Token.count++;
			//break;
		}
		else if (tmparg == '\t')  //tab check
		{
			Found_Token.type = "Seprator";
			Found_Token.value = char(92) + "t";
			Found_Token.count++;
			//break;
		}
		if (tmparg == '\"')
		{
			cfl->get(tmparg);
			while (tmparg != '"')		///this is imp make sure quotation are ended
			{
				line[ind] = tmparg;
				cfl->get(tmparg);
				ind++;
			}
			tmp = converttostring(line, ind);
			Found_Token.type = "Output";
			Found_Token.value = tmp;
			Found_Token.count++;
			//break;
		}
		if (tmparg == ' ')	//space check
		{												//ads space according to the code in file
			Found_Token.type = "Seprator";
			Found_Token.value = "' '";
			Found_Token.count++;
			//break;
		}
		if (tmparg == '&')
		{
			cfl->get(tmparg);
			if (tmparg == '&')
			{
				Found_Token.type = "Operator";
				Found_Token.value = "&&";
				Found_Token.count++;
			}
			else
			{
				Found_Token.type = "Operator";
				Found_Token.value = "&";
				Found_Token.count++;
			}
		}
		if (tmparg == '|')
		{
			cfl->get(tmparg);
			if (arg == '|')
			{
				Found_Token.type = "Operator";
				Found_Token.value = "||";
				Found_Token.count++;
			}
		}
		if (isalpha(tmparg))			// checks keywords
		{
			while (tmparg != ' ')
			{
				key[indx] = tmparg;
				cfl->get(tmparg);
				indx++;
			}
			tmp = converttostring(key, 10);
			for (int i = 0; i < 35; i++)
			{
				if (tmp == keyword[i])
				{
					Found_Token.type = "Keyword";
					Found_Token.value = keyword[i];
					Found_Token.count++;
					check = 1;
					break;
				}
			}
			if (check == 0)
			{
				Found_Token.type = "Identifier";
				Found_Token.value = tmp;
				Found_Token.count++;
				//break;
			}
		}
		else if (isdigit(tmparg))  //checks number
		{
			Found_Token.type = "Number";
			Found_Token.value = tmparg;
			Found_Token.count++;
			//break;
		}
		if (tmparg == ',' || tmparg == ';' || tmparg == '{' || tmparg == '}' || tmparg == '[' || tmparg == ']' || tmparg == '(' || tmparg == ')')
		{
			for (int j = 0; j < 8; j++)
			{
				if (tmparg == seprator[j])
				{
					Found_Token.type = "Seprator";
					Found_Token.value = seprator[j];
					Found_Token.count++;
					break;
				}
			}

		}
		else if (tmparg == '+' || tmparg == '-' || tmparg == '=' || tmparg == '*' || tmparg == '/' || tmparg == '%' || tmparg == '<' || tmparg == '>')
		{
			for (int k = 0; k < 8; k++)
			{
				if (tmparg == OOperator[k])
				{
					if (comment == 1 && tmparg == '/')	//checks comment
					{
						Found_Token.type = "Comment";
						Found_Token.value = "//";
						Found_Token.count++;
						break;
					}
					else if (tmparg == '=') //checks ==
					{
						cfl->get(tmparg);
						if (tmparg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "==";
							Found_Token.count++;
							check = 1;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (tmparg == '+') //checks  +=
					{
						cfl->get(tmparg);
						if (tmparg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "+=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (tmparg == '+') //checks ++
						{
							Found_Token.type = "Operator";
							Found_Token.value = "++";
							Found_Token.count++;
							break;
						}
						else if (tmparg == '-') //checks +-
						{
							Found_Token.type = "Operator";
							Found_Token.value = "+-";
							Found_Token.count++;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (tmparg == '-') //checks -=
					{
						cfl->get(tmparg);
						if (tmparg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "-=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (tmparg == '-')  //checks --
						{
							Found_Token.type = "Operator";
							Found_Token.value = "--";
							Found_Token.count++;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (tmparg == '*')
					{
						cfl->get(tmparg);
						if (arg == '=')		//checks *=
						{
							Found_Token.type = "Operator";
							Found_Token.value = "*=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (tmparg == '*') //checks **
						{
							Found_Token.type = "Operator";
							Found_Token.value = "**";
							Found_Token.count++;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (tmparg == '/')	//check /=
					{
						cfl->get(tmparg);
						if (tmparg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "/=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (tmparg == '/') //checks //
						{
							Found_Token.type = "Comment";
							Found_Token.value = "//";
							Found_Token.count++;
							while (tmparg != '\n')
							{
								fl.get(tmparg);
								com[cindx] = tmparg;
								cindx++;
							}
							tmp = converttostring(com, cindx);
							Found_Token.value += tmp;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (tmparg == '<')	//check <<
					{
						cfl->get(tmparg);
						if (tmparg == '<')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "<<";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (tmparg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "<=";
							Found_Token.count++;
							check = 1;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (tmparg == '>')	//check >>
					{
						cfl->get(tmparg);
						if (tmparg == '>')
						{
							Found_Token.type = "Operator";
							Found_Token.value = ">>";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (tmparg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = ">=";
							Found_Token.count++;
							check = 1;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else
					{
						Found_Token.type = "Operator";
						Found_Token.value = OOperator[k];
						Found_Token.count++;
						break;
					}
				}
			}

		}
		if (tmparg == '!')
		{
			cfl->get(tmparg);
			if (tmparg == '=')
			{
				Found_Token.type = "Operator";
				Found_Token.value = "!=";
				Found_Token.count++;
			}
		}
		Insert_Token(Found_Token);
	}
	return Found_Token;
}
//////////////////// Token FUNCTIONS //////////////////////////////
void value_table(Token_Temp arg)
{
	cfl = &fl;
	Token_Temp ttmp;
	tmparg = ' ';
	if (arg.type == "Identifier")
	{
		vtable.push_back(Var_val());
		vtable[inx].ident = arg.value;
	}
	while (!isdigit(tmparg) || tmparg != '\n' || tmparg != ';')
	{
		cfl->get(tmparg);
		ttmp = get_Tokentmp();
		parser(ttmp);
		if (isdigit(tmparg))
		{
			vtable[inx].ident_value= tmparg - '0';
			break;
		}
		if (tmparg == ';')
		{
			vtable[inx].ident_value = 0;
			break;
		}
	}
	inx++;
}
void Display_Token()
{
	if (Keyword.count >= 1)
	{
		std::cout << "Token Type = " << Keyword.type << endl;
		std::cout << "Token Occurences = " << Keyword.count << endl;

		for (size_t i = 0; i < Keyword.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Keyword.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "No Keyword Token found! :(" << endl << endl;
	}
	if (Identifier.count >= 1)
	{
		std::cout << "Token Type = " << Identifier.type << endl;
		std::cout << "Token Occurences = " << Identifier.count << endl;

		for (size_t i = 0; i < Identifier.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Identifier.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "No Identifier Tokens found! :(" << endl << endl;
	}
	if (Seprator.count >= 1)
	{
		std::cout << "Token Type = " << Seprator.type << endl;
		std::cout << "Token Occurences = " << Seprator.count << endl;

		for (size_t i = 0; i < Seprator.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Seprator.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "No Seprator Token found! :(" << endl << endl;
	}
	if (Number.count >= 1)
	{
		std::cout << "Token Type = " << Number.type << endl;
		std::cout << "Token Occurences = " << Number.count << endl;

		for (size_t i = 0; i < Number.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Number.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "No Number Token found! :(" << endl << endl;
	}
	if (Operator.count >= 1)
	{
		std::cout << "Token Type = " << Operator.type << endl;
		std::cout << "Token Occurences = " << Operator.count << endl;

		for (size_t i = 0; i < Operator.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Operator.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "No Operator Token found! :(" << endl << endl;
	}
	if (Comment.count >= 1)
	{
		std::cout << "Token Type = " << Comment.type << endl;
		std::cout << "Token Occurences = " << Comment.count << endl;

		for (size_t i = 0; i < Comment.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Comment.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "No Comment Token found! :(" << endl << endl;
	}
	if (Output.count >= 1)
	{
		std::cout << "Token Type = " << Output.type << endl;
		std::cout << "Token Occurences = " << Output.count << endl;

		for (size_t i = 0; i < Output.value.size(); i++)		//size_t is like a data type but for vector size
		{
			std::cout << "Token Value = " << Output.value.at(i) << endl;
		}
		std::cout << endl;
	}
	else
	{
		std::cout << "User did not std::cout anything! :(" << endl << endl;
	}
}
void Insert_Token(Token_Temp arg)
{
	if (arg.type == "Operator")
	{
		Operator.type = arg.type;
		Operator.value.push_back(arg.value);
		Operator.count++;
	}
	else if (arg.type == "Seprator")
	{
		Seprator.type = arg.type;
		Seprator.value.push_back(arg.value);
		Seprator.count++;
	}
	else if (arg.type == "Keyword")
	{
		Keyword.type = arg.type;
		Keyword.value.push_back(arg.value);
		Keyword.count++;
	}
	else if (arg.type == "Comment")
	{
		Comment.type = arg.type;
		Comment.value.push_back(arg.value);
		Comment.count++;
	}
	else if (arg.type == "Number")
	{
		Number.type = arg.type;
		Number.value.push_back(arg.value);
		Number.count++;
	}
	else if (arg.type == "Identifier")
	{
		if (not_present(arg))
		{
			Identifier.type = arg.type;
			Identifier.value.push_back(arg.value);
			Identifier.count++;
			value_table(arg);
		}
	}
	else if (arg.type == "Output")
	{
		Output.type = arg.type;
		Output.value.push_back(arg.value);
		Output.count++;
	}
}
Token_Temp get_Token()
{
	Token_Temp Found_Token;
	string tmp = " ";
	int size=0, comment = 0,ind=0,indx=0,cindx= 0,check;
	
	if (!fl.is_open())
	{
		cerr << "File not open!" << endl;
	}
	else
	{

		char key[10] = { '\0' }, line[50] = { '\0' }, com[50] = { '\0' };
		ind = indx = cindx = check = 0;


		if (arg == '\n') // newline check
		{
			Found_Token.type = "Seprator";
			Found_Token.value = "\\n";
			Found_Token.count++;
			//break;
		}
		else if (arg == '\t')  //tab check
		{
			Found_Token.type = "Seprator";
			Found_Token.value = char(92) + "t";
			Found_Token.count++;
			//break;
		}
		if (arg == '\"')
		{
			fl.get(arg);
			while (arg != '"')		///this is imp make sure quotation are ended
			{
				line[ind] = arg;
				fl.get(arg);
				ind++;
			}
			tmp = converttostring(line, ind);
			Found_Token.type = "Output";
			Found_Token.value = tmp;
			Found_Token.count++;
			//break;
		}
		if (arg == '&')
		{
			fl.get(arg);
			if (arg == '&')
			{
				Found_Token.type = "Operator";
				Found_Token.value = "&&";
				Found_Token.count++;
			}
			else
			{
				Found_Token.type = "Operator";
				Found_Token.value = "&";
				Found_Token.count++;
			}
		}
		if (arg == '|')
		{
			fl.get(arg);
			if (arg == '|')
			{
				Found_Token.type = "Operator";
				Found_Token.value = "||";
				Found_Token.count++;
			}
		}
		if (arg == ' ')	//space check
		{												//ads space according to the code in file
			Found_Token.type = "Seprator";
			Found_Token.value = "' '";
			Found_Token.count++;
			//break;
		}
		if (isalpha(arg))			// checks keywords
		{
			while (arg != ' ')
			{
				key[indx] = arg;
				fl.get(arg);
				indx++;
			}
			tmp = converttostring(key, 10);
			for (int i = 0; i < 34; i++)
			{
				if (tmp == keyword[i])
				{
					Found_Token.type = "Keyword";
					Found_Token.value = keyword[i];
					Found_Token.count++;
					check = 1;
					break;
				}
			}
			if (tmp == "begin" || tmp == "Begin" || tmp == "BEGIN")
			{
				Found_Token.type = "System";
				Found_Token.value = "begin";
				return Found_Token;
			}
			else if (tmp == "end" || tmp == "End" || tmp == "END")
			{
				Found_Token.type = "System";
				Found_Token.value = "end";
				return Found_Token;
			}
			if (check == 0)
			{
				Found_Token.type = "Identifier";
				Found_Token.value = tmp;
				Found_Token.count++;
				//break;
			}
		}
		else if (isdigit(arg))  //checks number
		{
			Found_Token.type = "Number";
			Found_Token.value = arg;
			Found_Token.count++;
			//break;
		}
		if (arg == ',' || arg == ';' || arg == '{' || arg == '}' || arg == '[' || arg == ']' || arg == '(' || arg == ')')
		{
			for (int j = 0; j < 8; j++)
			{
				if (arg == seprator[j])
				{
					Found_Token.type = "Seprator";
					Found_Token.value = seprator[j];
					Found_Token.count++;
					break;
				}
			}

		}
		else if (arg == '+' || arg == '-' || arg == '=' || arg == '*' || arg == '/' || arg == '%' || arg == '<' || arg == '>')
		{
			for (int k = 0; k < 8; k++)
			{
				if (arg == OOperator[k])
				{
					if (comment == 1 && arg == '/')	//checks comment
					{
						Found_Token.type = "Comment";
						Found_Token.value = "//";
						Found_Token.count++;
						break;
					}
					else if (arg == '=') //checks ==
					{
						fl.get(arg);
						if (arg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "==";
							Found_Token.count++;
							check = 1;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (arg == '+') //checks  +=
					{
						fl.get(arg);
						if (arg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "+=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (arg == '+') //checks ++
						{
							Found_Token.type = "Operator";
							Found_Token.value = "++";
							Found_Token.count++;
							break;
						}
						else if (arg == '-') //checks +-
						{
							Found_Token.type = "Operator";
							Found_Token.value = "+-";
							Found_Token.count++;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (arg == '-') //checks -=
					{
						fl.get(arg);
						if (arg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "-=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (arg == '-')  //checks --
						{
							Found_Token.type = "Operator";
							Found_Token.value = "--";
							Found_Token.count++;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (arg == '*')
					{
						fl.get(arg);
						if (arg == '=')		//checks *=
						{
							Found_Token.type = "Operator";
							Found_Token.value = "*=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (arg == '*') //checks **
						{
							Found_Token.type = "Operator";
							Found_Token.value = "**";
							Found_Token.count++;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (arg == '/')	//check /=
					{
						fl.get(arg);
						if (arg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "/=";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (arg == '/') //checks //
						{
							Found_Token.type = "Comment";
							Found_Token.value = "//";
							Found_Token.count++;
							while (arg != '\n')
							{
								fl.get(arg);
								com[cindx] = arg;
								cindx++;
							}
							tmp = converttostring(com, cindx);
							Found_Token.value += tmp;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (arg == '<')	//check <<
					{
						fl.get(arg);
						if (arg == '<')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "<<";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (arg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = "<=";
							Found_Token.count++;
							check = 1;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else if (arg == '>')	//check >>
					{
						fl.get(arg);
						if (arg == '>')
						{
							Found_Token.type = "Operator";
							Found_Token.value = ">>";
							Found_Token.count++;
							check = 1;
							break;
						}
						else if (arg == '=')
						{
							Found_Token.type = "Operator";
							Found_Token.value = ">=";
							Found_Token.count++;
							check = 1;
							break;
						}
						if (check == 0)
						{
							Found_Token.type = "Operator";
							Found_Token.value = OOperator[k];
							Found_Token.count++;
							break;
						}
					}
					else
					{
						Found_Token.type = "Operator";
						Found_Token.value = OOperator[k];
						Found_Token.count++;
						break;
					}
				}
			}

		}
		if (arg == '!')
		{
			fl.get(arg);
			if (arg == '=')
			{
				Found_Token.type = "Operator";
				Found_Token.value = "!=";
				Found_Token.count++;
			}
		}
		parser(Found_Token);
		Insert_Token(Found_Token);

	}

			return Found_Token;
}

////////////////// CALCULATOR /////////////////////////////////////
void Add()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " <<vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}

	std::cout << vtable[0].ident << " + " << vtable[1].ident << " = " << vtable[0].ident_value + vtable[1].ident_value << endl;
}
void Sub()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}

	std::cout << vtable[0].ident << " - " << vtable[1].ident << " = " << vtable[0].ident_value - vtable[1].ident_value << endl;
}
void Mul()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}

	std::cout << vtable[0].ident << " x " << vtable[1].ident << " = " << vtable[0].ident_value * vtable[1].ident_value << endl;
}
void Div()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	std::cout << vtable[0].ident << " / " << vtable[1].ident << " = " << (float)vtable[0].ident_value / vtable[1].ident_value << endl;
}
void Mod()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	std::cout << vtable[0].ident << " % " << vtable[1].ident << " = " << vtable[0].ident_value % vtable[1].ident_value << endl;
}
void Less()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	if (vtable[0].ident_value < vtable[1].ident_value)
	{
		std::cout << vtable[0].ident << " < "<< vtable[1].ident << endl;
	}	
	else
	{
		std::cout << "No, variable = " << vtable[0].ident << " is not less than " << vtable[1].ident << endl;
	}
}
void Lessequl()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	if (vtable[0].ident_value <= vtable[1].ident_value)
	{
		std::cout << vtable[0].ident << " <= " << vtable[1].ident << endl;
	}
	else
	{
		std::cout << "No, variable = " << vtable[0].ident << " is not lesser than or equal to " << vtable[1].ident << endl;
	}
}
void More()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	if (vtable[0].ident_value > vtable[1].ident_value)
	{
		std::cout << vtable[0].ident << " > " << vtable[1].ident << endl;
	}
	else
	{
		std::cout << "No, variable = " << vtable[0].ident << " is not greater than " << vtable[1].ident << endl;
	}
}
void Moreequl()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	if (vtable[0].ident_value >= vtable[1].ident_value)
	{
		std::cout << vtable[0].ident << " >= " << vtable[1].ident << endl;
	}
	else
	{
		std::cout << "No, variable = " << vtable[0].ident << " is not greater than or equal to " << vtable[1].ident << endl;
	}
}
void Equal()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	if (vtable[0].ident_value == vtable[1].ident_value)
	{
		std::cout << vtable[0].ident << " == " << vtable[1].ident << endl;
	}
	else
	{
		std::cout << "No, variable = " << vtable[0].ident << " is not equal to " << vtable[1].ident << endl;
	}
}
bool equal()
{
	if (vtable[0].ident_value == vtable[1].ident_value)
	{
		return true;
	}
	return false;
}
void Notequal()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	if (vtable[0].ident_value != vtable[1].ident_value)
	{
		std::cout << vtable[0].ident << " != " << vtable[1].ident << endl;
		
	}
	else
	{
		std::cout << "No, variable = " << vtable[0].ident << " is equal to " << vtable[1].ident << endl;
	}
}
void Leftshift()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	std::cout << "After Left Shift = \n";
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << (vtable[i].ident_value << 1) << endl;
	}
}
void Rightshift()
{
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << vtable[i].ident_value << endl;
	}
	std::cout << "After Right Shift = \n";
	for (int i = 0; i < vtable.size(); i++)
	{
		std::cout << "Identifer" << i << " = " << vtable[i].ident << "\nvalue " << " = " << (vtable[i].ident_value >> 1) << endl;
	}
}
void And()
{
	if (equal())
	{
		std::cout << "True AND" << endl;
	}
	std::cout << "False AND" << endl;
}
void Or()
{
	if (equal())
	{
		std::cout << "True OR" << endl;
	}
	std::cout << "False OR" << endl;
}
void calculator()
{
	int tcal = 0;
	for (int i = 0; i < Operator.value.size(); i++)
	{
		if (Operator.value[i] == "+")
		{
			Add();
		}
		else if (Operator.value[i] == "-")
		{
			Sub();
		}
		else if (Operator.value[i] == "*")
		{
			Mul();
		}
		else if (Operator.value[i] == "/")
		{
			Div();
		}
		else if (Operator.value[i] == "%")
		{
			Mod();
		}
		else if (Operator.value[i] == "<")
		{
			Less();
		}
		else if (Operator.value[i] == ">")
		{
			More();
		}
		else if (Operator.value[i] == "<=")
		{
			Lessequl();
		}
		else if (Operator.value[i] == ">=")
		{
			Moreequl();
		}
		else if (Operator.value[i] == "==")
		{
			Equal();
		}
		else if (Operator.value[i] == "!=")
		{
			Notequal();
		}
		else if (Operator.value[i] == "&&")
		{
			And();
		}
		else if (Operator.value[i] == "||")
		{
			Or();
		}
		else if (Operator.value[i] == ">>")
		{
			Rightshift();
			tcal = 1;
			cout << Operator.value[i] << endl;
		}
		else if (Operator.value[i] == "<<")
		{
			if (tcal == 1)
			{
				Leftshift();
				cout << Operator.value[i] << endl;
			}
		}
		std::cout << endl;
	}
}

/////////////////////////// PARSER //////////////////////////////////////
int timp = 0;
void parser(Token_Temp arg)
{
	
	if (arg.type == "Identifier")
	{
		for (int j = 0; j < vtable.size(); j++)
		{
			if (arg.value == vtable[j].ident)
			{
				cout << "<Identifier" << j << ">";
				timp = 1;
				break;
			}
		}
		if (timp == 0)
		{
			cout << "<Identifier0>";
			timp = 2;
		}
		else if (timp == 2)
		{
			cout << "<Identifier1>";
			timp = 3;
		}
		else if (timp == 3)
		{
			cout << "<Identifier2>";
		}
	}
	if (arg.type == "Number")
	{
		cout << "<Number>";
	}
	if (arg.type == "Operator")
	{
		for (int i = 0; i < 8; i++)
		{
			if (arg.value == convertstring(OOperator[i]))
			{
				if (arg.value == "/")	//checks comment
				{
					cout << "<Divide-opr>";
					break;
				}
				else if (arg.value == "=") //checks =
				{
					cout << "<Equal-opr>";
					break;
				}
				else if (arg.value == "+") //checks  +
				{
					cout << "<Plus-opr>";
					break;
				}
				else if (arg.value == "-") //checks -
				{
					cout << "<Minus-opr>";
					break;
				}
				else if (arg.value == "*")
				{
					cout << "<Multiply-opr>";
					break;
				}
				else if (arg.value == "%")	//check %
				{
					cout << "<Modulas-opr>";
					break;
				}
				else if (arg.value == "<")	//check <
				{
					cout << "<Less-than-opr>";
					break;
				}
				else if (arg.value == ">")	//check >
				{
					cout << "<Greater-than-opr>";
					break;
				}				
			}
			else if (arg.value == "++")	//check ++
			{
				cout << "<Increment-opr>";
				break;
			}
			else if (arg.value == "--")	//check --
			{
				cout << "<Decrement-opr>";
				break;
			}

		}
		if (arg.value == "<<")
		{
			cout << "<Insertion-opr>";
		}
		else if (arg.value == ">=")
		{
			cout << "<Greater-than-equal-to-opr>";
		}
		else if (arg.value == "<=")
		{
			cout << "<Less-than-equal-to-opr>";
		}
		else if (arg.value == "==")
		{
			cout << "<Compare-equal-to-opr>";
		}
		else if (arg.value == "!=")
		{
			cout << "<Not-equal-to-opr>";
		}
		else if (arg.value == ">=")
		{
			cout << "<Greater-than-equal-to-opr>";
		}
		else if (arg.value == ">>")
		{
			cout << "<Extraction-opr>";
		}
		else if (arg.value == "&&")
		{
			cout << "<AND-opr>";
		}
		else if (arg.value == "||")
		{
			cout << "<OR-opr>";
		}
		else if (arg.value == "+=")
		{
			cout << "<Plus-equal-to-opr>";
		}
		else if (arg.value == "-=")
		{
			cout << "<Minus-equal-to-opr>";
		}
		else if (arg.value == "*=")
		{
			cout << "<Multiply-equal-to-opr>";
		}
		else if (arg.value == "/=")
		{
			cout << "<Divide-equal-to-opr>";
		}
	}
	if (arg.type == "Seprator" && arg.value == ";")
	{
		cout << "<Seprator>";
		cout << endl;
	}
	if (arg.type == "Seprator" && arg.type == "\\n")
	{
		cout << endl;
	}
	if (arg.type == "Seprator" && arg.type == "\t")
	{
		cout << "\t";
	}
	
	if (arg.type == "Keyword")
	{
		cout << "<Keyword>";
	}

}

//////////////////////////////////////// Ignore this tree method evaluation does not work ////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tree Structure
//typedef struct node
//{
//	char data;
//	struct node* left, * right;
//} *nptr;
//
//// Function to create new node
//nptr newNode(char c)
//{
//	nptr n = new node;
//	n->data = c;
//	n->left = n->right = nullptr;
//	return n;
//}
//
//// Function to build Expression Tree
//nptr build(string& s)
//{
//
//	// Stack to hold nodes
//	stack<nptr> stN;
//
//	// Stack to hold chars
//	stack<char> stC;
//	nptr t, t1, t2;
//
//	// Prioritising the operators
//	int p[123] = { 0 };
//	p['+'] = p['-'] = 1, p['/'] = p['*'] = 2, p['^'] = 3,
//		p[')'] = 0;
//
//	for (int i = 0; i < s.length(); i++)
//	{
//		if (s[i] == '(') {
//
//			// Push '(' in char stack
//			stC.push(s[i]);
//		}
//
//		// Push the operands in node stack
//		else if (isalpha(s[i]))
//		{
//			t = newNode(s[i]);
//			stN.push(t);
//		}
//		else if (p[s[i]] > 0)
//		{
//			// If an operator with lower or
//			// same associativity appears
//			while (
//				!stC.empty() && stC.top() != '('
//				&& ((s[i] != '^' && p[stC.top()] >= p[s[i]])
//					|| (s[i] == '^'
//						&& p[stC.top()] > p[s[i]])))
//			{
//
//				// Get and remove the top element
//				// from the character stack
//				t = newNode(stC.top());
//				stC.pop();
//
//				// Get and remove the top element
//				// from the node stack
//				t1 = stN.top();
//				stN.pop();
//
//				// Get and remove the currently top
//				// element from the node stack
//				t2 = stN.top();
//				stN.pop();
//
//				// Update the tree
//				t->left = t2;
//				t->right = t1;
//
//				// Push the node to the node stack
//				stN.push(t);
//			}
//
//			// Push s[i] to char stack
//			stC.push(s[i]);
//		}
//		else if (s[i] == ')') {
//			while (!stC.empty() && stC.top() != '(')
//			{
//				t = newNode(stC.top());
//				stC.pop();
//				t1 = stN.top();
//				stN.pop();
//				t2 = stN.top();
//				stN.pop();
//				t->left = t2;
//				t->right = t1;
//				stN.push(t);
//			}
//			stC.pop();
//		}
//	}
//	t = stN.top();
//	return t;
//}
//
//// Function to print the post order
//// traversal of the tree
//void inorder(nptr root)
//{
//	if (root)
//	{
//		inorder(root->left);
//		cout << root->data;
//		inorder(root->right);
//		
//	}
//}
////////////////////////////////////////////////// Tree implementation :) ///////////////////////////////////////////////////////////////////////////
const int MAX_ITEMS = 20;
class FullStack
	//   stack when full.
{};

class EmptyStack
	// stack when emtpy.
{};

template<class ItemType>
class StackType
{
public:

	StackType();
	bool IsFull() const;
	bool IsEmpty() const;
	void Push(ItemType item);
	void Pop();
	ItemType Top();
private:
	int top;
	ItemType  items[MAX_ITEMS];
};

template<class ItemType>
StackType<ItemType>::StackType()
{
	top = -1;
}

template<class ItemType>
bool StackType<ItemType>::IsEmpty() const
{
	return (top == -1);
}

template<class ItemType>
bool StackType<ItemType>::IsFull() const
{
	return (top == MAX_ITEMS - 1);
}

template<class ItemType>
void StackType<ItemType>::Push(ItemType newItem)
{
	if (IsFull())
		throw FullStack();
	top++;
	items[top] = newItem;
}

template<class ItemType>
void StackType<ItemType>::Pop()
{
	if (IsEmpty())
		throw EmptyStack();
	top--;
}

template<class ItemType>
ItemType StackType<ItemType>::Top()
{
	if (IsEmpty())
		throw EmptyStack();
	return items[top];
}

struct node;
int priority(char);
node* makeNode(char);
void attachOperator(StackType<node*>&, StackType<node*>&); //Pops operator off of the operators stack,
int evaluateTree(node*);  //evaluates tree

struct node 
{
	char info;
	node* left;
	node* right;
};

//Determines the priority of an operator
int priority(char op) 
{
	if ((op == '+') || (op == '-'))
		return 1;
	if ((op == '/') || (op == '*'))
		return 2;
}

node* makeNode(char info) 
{
	node* childnode;
	childnode = new node;
	childnode->info = info;
	childnode->left = NULL;
	childnode->right = NULL;
	return childnode;
}

void attachOperator(StackType<node*>& treenodes, StackType<node*>& operators)
{
	node* operatornode = operators.Top();
	operators.Pop();
	operatornode->left = treenodes.Top();
	treenodes.Pop();
	operatornode->right = treenodes.Top();
	treenodes.Pop();
	treenodes.Push(operatornode);
}

//Using a recursive function, the value of the expression is Calculated
int evaluateTree(node* treenode) {
	int x, y, z;
	if ((treenode->info) == '+' || (treenode->info) == '-' || (treenode->info) == '*' || (treenode->info) == '/') 
	{
		x = evaluateTree(treenode->left);
		y = evaluateTree(treenode->right);
		if (treenode->info == '+')
			z = x + y;
		else if (treenode->info == '-')
			z = x - y;
		else if (treenode->info == '*')
			z = x * y;
		else if (treenode->info == '/')
			z = x / y;
		return z;
	}
	else return treenode->info - '0';
}
int xo = 0;
void getdata()	
{
	ftl.open("abc.txt");
	while(getline(ftl,s))
	{
		Treeval.push_back(s);
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
#include <iostream>													// viusal studio spitting errors 
																// to lazy to type in iostream:: before every error
/////////////////////////////////////////////////////

////////////////////////////  MAIN /////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	Token_Temp objtmp;
	//nptr root=NULL;
	fl.open("abcd.txt");
	fl.get(arg);
	objtmp = get_Token();
	if (objtmp.value == "begin" || objtmp.value == "Begin" || objtmp.value == "BEGIN")
	{
		cout << "Begin keyword found!" << endl;
		while (true)
		{
			fl.get(arg);
			objtmp = get_Token();
						 
			if (objtmp.value == "end")
			{
				//cout << endl;
				cout << "End keyword found!" << endl;
				break;
			}
			if (fl.eof())
			{
				cerr << "No End keyword found!" << endl;
				break;
			}
		}
	}
	else
	{
		cerr << "No Begin keyword found!" << endl;
	}
	
	//s = "(" + s; 	s += ")"; root =	build(s); inorder(root); // previous tree code ignore this does not work (period)
	string infix; 
	StackType<char> input; // stack for input string
	StackType<node*> operators; // stack that operator pointer addresses
	StackType<node*> treenodes;  // stack output node pointer addresses
	char temp;
	int	again = 0;
	cout << endl;
	//////////////////////////////////////////////////////////////// Critical part regarding trees ////////////////////////////////////////////////////////////////
	getdata();
	do
	{
		infix = Treeval.at(again);
		//Pushes the contents of the string into the input stack
		for (int i = 0; i < infix.length(); i++) {
			input.Push(infix[i]);
		}
		while (!input.IsEmpty()) {
			temp = input.Top();
			input.Pop();
			if (isdigit(temp))
				treenodes.Push(makeNode(temp));
			if (temp == ')')
				operators.Push(makeNode(temp));
			//If it is an operator, then
			if ((temp == '+') || (temp == '/') || (temp == '-') || (temp == '*')) {
				bool pushed = false;
				while (!pushed) {
					//If stack is empty, make node and push operator on stack.
					if (operators.IsEmpty()) {
						operators.Push(makeNode(temp));
						pushed = true;
					}
					//If the top of stack is closing parenthesis, make node and push operator on stack.
					else if (operators.Top()->info == ')') {
						operators.Push(makeNode(temp));
						pushed = true;
					}
					//If it has same or higher priority than the top of stack, make node and push operator on stack.
					else if ((priority(temp) > priority(operators.Top()->info)) || (priority(temp) == priority(operators.Top()->info))) {
						operators.Push(makeNode(temp));
						pushed = true;
					}
					//Else pop the operator from the stack, perform attachOperator and add it to treenodes. repeat step 5.
					else {
						attachOperator(treenodes, operators);
					}
				}
			}
			//If it is a opening parenthesis, pop operators from stack and perform attachOperator 
			//until a closing parenthesis is encountered. Pop and discard the closing parenthesis.
			if (temp == '(') {
				while (operators.Top()->info != ')')
				{
					attachOperator(treenodes, operators);
				}
				operators.Pop(); // ')' is popped and discarded
			}
		}
		//If there is no more input, unstack the remaining operators and perform attachOperator
		while (!operators.IsEmpty()) {
			attachOperator(treenodes, operators);
		}

		int answer = evaluateTree(treenodes.Top());
		cout << "Evaluation after Tree traversing: " << answer << endl;
		//cout << "(y/n) do you want to change the expression in file";
		//cin >> again;
		again++;
	} while (again != Treeval.size());

	cout << endl;
	//////////////////////////////////////////////////////////////////////////////// END of Expression Tree /////////////////////////////////////////////////////////////////////////////////////////////////

	std::system("pause");
	return 0;
}