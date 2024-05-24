#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


char token[10000];
int count=0;

void tokenizer()
{
	FILE * file;
    file = fopen( "input.txt" , "r"); //ENTER THE NAME OF THE TEXT FILE FOR INPUT HERE
    
    char line[100000];
    int i;
    
    while( fgets(line,100000,file) )
    {
    	for(i=0;i<strlen(line);i++)
    	{

    		if(line[i]==32 || line [i]==9 || line[i]==10 || line[i] == 13)
    		{
    		
    			continue;
			}
			else
			{
				token[count]=line[i];
				count++;
			}
		}
	}
    
}


struct node
{
	char name[100];
	int name_count;
	struct node* children[100];
	int no_children;
	int current_child;
	char value_name[100];
	int state;
};

char variable_name[100][100];
char variable_value[100][100];
int variable_index = 0;


// stack

struct node *stack[10000];
int top;




struct node *root;

/*Prototypes of the function*/
struct node* solve_constant(int, int);
struct node* solve_assignment(int, int);
struct node* solve_expression_E3(int, int);
struct node* solve_expression_E2(int, int);
struct node* solve_expression_E1(int, int);
struct node* solve_expression_E(int, int);
struct node* solve_variable_V(int, int);
struct node* solve_variable_V1(int, int);
struct node* solve_variable_Vi(int, int);
struct node* solve_prog(int, int);
struct node* solve_while(int, int);
struct node* solve_statement(int, int);
struct node* solve_print(int, int);
struct node* solve_read(int, int);
void simulate(struct node *);


FILE *fp;

 
// Open file



void print(struct node * r)
{
	

	int i;
	for(i=0;i<r->name_count;i++)
	{
		fprintf(fp, "%c",r->name[i]);		
	}
	if(r->no_children>0)
	{	
		fprintf(fp, "@ ");
		for(i=0;i<r->no_children;i++)
		{
			print(r->children[i]);
		}
		fprintf(fp, "# ");
	}		
}


void makeZero(struct node * r)
{
	
	
	r->current_child=0;
	//r->state=0;
	
	if(r->no_children>0)
	{	
		int i;
		for(i=0;i<r->no_children;i++)
		{
			makeZero(r->children[i]);
		}
	}		
}

int isAlpha(char c)
{
	if((c>=97 && c<=122) || (c>=65 && c<=90))
		return 1;

	return 0;
}

int isNum(char c)
{
	if((c>=48 && c<=57))
		return 1;

	return 0;
}

int B = 8;

struct node* solve_constant(int start, int end)
{
	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'C';
	int dec_point = -1;
	//search decimal point
	for(i=start;i<=end;i++)
	{
		if(token[i] == '.')
		{
			dec_point = i;
			break;
		}
	}
	if(dec_point == -1 || (dec_point!=-1 && end-dec_point<=B))
	{
		assign_root->no_children = 1;

		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		
		for(i=0;i<end-start+1;i++)
		{
			assign_root->children[0]->name[i] = token[start+i];
		}
		assign_root->children[0]->name_count = end-start+1;
		assign_root->children[0]->no_children = 0;
		
	}
	else
	{
		assign_root->no_children = 1;

		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		
		for(i=0;i<dec_point - start + 1 + B;i++)
		{
			
			assign_root->children[0]->name[i] = token[start+i];
		}
		assign_root->children[0]->name_count = dec_point - start + 1 + B;
		assign_root->children[0]->no_children = 0;
	}


return assign_root;

}

struct node* solve_expression_E3(int start, int end)
{
	int flag = 0;
	if(token[start] == '(')
		flag = 1;
	else if(isAlpha(token[start]) == 1)
		flag = 2;
	else if(isNum(token[start]) == 1)
		flag = 3;


	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 2;
	assign_root->name[0] = 'E';
	assign_root->name[1] = '3' ;

	if(flag == 1)
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0]->name[0] = '(';
		assign_root->children[0]->name_count = 1;
		assign_root->children[0]->no_children = 0;

		assign_root->children[1] = solve_expression_E(start+1, end-1);

		assign_root->children[2]->name[0] = ')';
		assign_root->children[2]->name_count = 1;
		assign_root->children[2]->no_children = 0;

		
	}
	else if(flag == 2)
	{
		assign_root->no_children = 1;	

		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));

		assign_root->children[0] = solve_variable_V1(start, end);
	}
	else if(flag == 3)
	{
		assign_root->no_children = 1;	

		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));

		assign_root->children[0] = solve_constant(start, end);
	}

return assign_root;
}


struct node* solve_expression_E2(int start, int end)
{
	struct node *assign_root;
	int i=end;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 2;
	assign_root->name[0] = 'E' ;
	assign_root->name[1] = '2' ;
	int op_ind;	
	int flag = 0;	//flag == 1 ==> * and flag == 2 ==> /
	int sum = 0;
	while(i>=start)
	{
		if(token[i] == ')')
			sum++;
		if(token[i] == '(')
			sum--;
		if(token[i] == '*' && sum == 0)
		{
			op_ind = i;
			flag = 1;
			break;
		}
		if(token[i] == '/' && sum == 0)
		{
			op_ind = i;
			flag = 2;
			break;
		}
		i--;
	}

	if(flag == 0)
	{
		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		assign_root->no_children = 1;
		assign_root->children[0] = solve_expression_E3(start, end);

	}
	else if(flag == 1)	// found *
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0] = solve_expression_E2(start, op_ind-1);

		assign_root->children[1]->name[0] = '*';
		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_expression_E3(op_ind+1, end);
	}
	else if(flag == 2)	// found /
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0] = solve_expression_E2(start, op_ind-1);

		assign_root->children[1]->name[0] = '/';
		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_expression_E3(op_ind+1, end);
	}

	return assign_root;
}


struct node* solve_expression_E1(int start, int end)
{

	struct node *assign_root;
	int i=end;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 2;
	assign_root->name[0] = 'E';
	assign_root->name[1] = '1';
	int op_ind;	
	int flag = 0;	//flag == 1 ==> + and flag == 2 ==> -
	int sum = 0;
	while(i>=start)
	{
		if(token[i] == ')')
			sum++;
		if(token[i] == '(')
			sum--;
		if(token[i] == '+' && sum == 0)
		{
			op_ind = i;
			flag = 1;
			break;
		}
		if(token[i] == '-' && sum == 0)
		{
			op_ind = i;
			flag = 2;
			break;
		}
		i--;
	}

	if(flag == 0)
	{
		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		assign_root->no_children = 1;
		assign_root->children[0] = solve_expression_E2(start, end);

	}
	else if(flag == 1)	// found +
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0] = solve_expression_E1(start, op_ind-1);

		assign_root->children[1]->name[0] = '+';
		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_expression_E2(op_ind+1, end);
	}
	else if(flag == 2)	// found -
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0] = solve_expression_E1(start, op_ind-1);

		assign_root->children[1]->name[0] = '-';
		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_expression_E2(op_ind+1, end);
	}

	return assign_root;

}

struct node* solve_expression_E(int start, int end)
{
	struct node *assign_root;
	int i=end;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'E';
	int op_ind;	
	int flag = 0;	//flag == 1 ==> < and flag == 2 ==> ==
	int sum = 0;
	while(i>=start)
	{
		if(token[i] == ')')
			sum++;
		if(token[i] == '(')
			sum--;
		if(token[i] == '<' && sum == 0)
		{
			op_ind = i;
			flag = 1;
			break;
		}
		if(i>start && token[i] == '=' && token[i-1] == '=' && sum == 0)
		{
			op_ind = i;
			flag = 2;
			break;
		}
		i--;
	}

	if(flag == 0)
	{
		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		assign_root->no_children = 1;
		assign_root->children[0] = solve_expression_E1(start, end);

	}
	else if(flag == 1)	// found <
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0] = solve_expression_E(start, op_ind-1);

		assign_root->children[1]->name[0] = '<';
		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_expression_E1(op_ind+1, end);
	}
	else if(flag == 2)	// found ==
	{
		assign_root->no_children = 3;	
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}
		assign_root->children[0] = solve_expression_E(start, op_ind-2);

		assign_root->children[1]->name[0] = '=';
		assign_root->children[1]->name[1] = '=';
		assign_root->children[1]->name_count = 2;
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_expression_E1(op_ind+1, end);
	}

	return assign_root;

}

struct node* solve_variable_V(int start, int end)
{
	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->no_children = 1;
	assign_root->name_count = 1;
	
	assign_root->name[0] = 'V';
	assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
	for(i=0;i<end-start+1;i++)
	{
		assign_root->children[0]->name[i] = token[start+i];
	}
	assign_root->children[0]->name_count = end-start+1;
	assign_root->children[0]->no_children = 0;

return assign_root;

}

struct node* solve_variable_Vi(int start, int end)
{
	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->no_children = 4;

	for(i=0;i<4;i++)
	{
		assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
	}

	assign_root->name_count = 2;
	assign_root->name[0] = 'V';
	assign_root->name[1] = 'i';
	
	//search for [ 
	int start_sq, end_sq = end;
	for(i=start;i<=end;i++)
	{
		if(token[i] == '[')
		{
			start_sq = i;
			break;
		}
	}

	// assign names of children

	assign_root->children[0] = solve_variable_V(start, start_sq-1);

	assign_root->children[1]->name[0] = '[';
	assign_root->children[1]->name_count = 1;
	assign_root->children[1]->no_children = 0;

	assign_root->children[2] = solve_expression_E(start_sq+1, end_sq-1);

	assign_root->children[3]->name[0] = ']';
	assign_root->children[3]->name_count = 1;
	assign_root->children[3]->no_children = 0;

	return assign_root;
}


struct node* solve_variable_V1(int start, int end)
{
	int i;
	int flag = 0;
	for(i=start;i<=end;i++)
	{
		if(token[i] == '[' || token[i] == ']')
		{
			flag = 1;
			break;
		}
	}

	struct node *assign_root;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name[0] = 'V';
	assign_root->name[1] = '1';
	assign_root-> name_count = 2;


	if(flag == 0) // no square bracket
	{
		assign_root->no_children = 1;
		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		assign_root->children[0] = solve_variable_V(start, end);
	}
	else	// has a bracket
	{
		assign_root->no_children = 1;
		assign_root->children[0] =(struct node *)malloc(sizeof(struct node));
		assign_root->children[0] = solve_variable_Vi(start, end);
		

	}
	return assign_root;
}


struct node* solve_assignment(int start, int end)
{
	struct node *assign_root;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name[0] = 'A';
	assign_root-> name_count = 1;

	assign_root->no_children = 3;
	
	int i, equal;
	
	for(i=0;i<3;i++)
	{
		assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
	}

	// search for =
	
	for(i=start;i<=end;i++)
	{
		if(token[i] == '=')
		{
			equal = i;
			break;
		}
	}


	// first child
	 assign_root->children[0] = solve_variable_V1(start, equal-1);

	//second child
	assign_root->children[1]->name[0] = '=';
	assign_root->children[1]->name_count = 1; 
	assign_root->children[1]->no_children = 0; 


	//third child
	assign_root->children[2] = solve_expression_E(equal+1, end);

	

	return assign_root;
}

struct node* solve_prog(int start, int end)
{

	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'P';
	
	// search for semicolon
	int semi;
	int count_curly = 0;
	for(i=start;i<=end;i++)
	{
		if(token[i] == '{')
			count_curly++;
		if(token[i] == '}')
			count_curly--;
		if(token[i] == ';' && count_curly == 0)
		{
			semi = i;
			break;
		}
	}
	if(semi == end)
	{
		assign_root->no_children = 2;
		for(i=0;i<2;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}

		assign_root->children[0] = solve_statement(start, end-1);

		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->name[0] = ';';
		assign_root->children[1]->no_children = 0;
	}
	else
	{
		assign_root->no_children = 3;
		for(i=0;i<3;i++)
		{
			assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
		}

		assign_root->children[0] = solve_statement(start, semi-1);

		assign_root->children[1]->name_count = 1;
		assign_root->children[1]->name[0] = ';';
		assign_root->children[1]->no_children = 0;

		assign_root->children[2] = solve_prog(semi+1, end);
	}

	return assign_root;
}

struct node* solve_while(int start, int end)
{
	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'W';
	assign_root->no_children = 7;
	
	for(i=0;i<7;i++)
	{
		assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
	}


	// search first curly bracket
	int curly;
	for(i=start;i<=end;i++)
	{
		if(token[i] == '{')
		{
			curly = i;
			break;
		}
	}

	assign_root->children[0]->name_count = 5;
	assign_root->children[0]->name[0] = 'w';
	assign_root->children[0]->name[1] = 'h';
	assign_root->children[0]->name[2] = 'i';
	assign_root->children[0]->name[3] = 'l';
	assign_root->children[0]->name[4] = 'e';
	assign_root->children[0]->no_children = 0;

	assign_root->children[1]->name_count = 1;
	assign_root->children[1]->name[0] = '(';
	assign_root->children[1]->no_children = 0;

	assign_root->children[2] = solve_expression_E(start+6, curly-2);

	assign_root->children[3]->name_count = 1;
	assign_root->children[3]->name[0] = ')';
	assign_root->children[3]->no_children = 0;

	assign_root->children[4]->name_count = 1;
	assign_root->children[4]->name[0] = '{';
	assign_root->children[4]->no_children = 0;

	assign_root->children[5] = solve_prog(curly+1, end-1);

	assign_root->children[6]->name_count = 1;
	assign_root->children[6]->name[0] = '}';
	assign_root->children[6]->no_children = 0;

return assign_root;

}


struct node* solve_read(int start, int end)
{
	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'R';
	assign_root->no_children = 2;
	
	for(i=0;i<2;i++)
	{
		assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
	}


	assign_root->children[0]->name_count = 4;
	assign_root->children[0]->name[0] = 'r';
	assign_root->children[0]->name[1] = 'e';
	assign_root->children[0]->name[2] = 'a';
	assign_root->children[0]->name[3] = 'd';
	assign_root->children[0]->no_children = 0;

	assign_root->children[1] = solve_variable_V1(start+4, end);

return assign_root;

}

struct node* solve_print(int start, int end)
{
	struct node *assign_root;
	int i;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'O';
	assign_root->no_children = 2;
	
	for(i=0;i<2;i++)
	{
		assign_root->children[i] =(struct node *)malloc(sizeof(struct node));
	}


	assign_root->children[0]->name_count = 5;
	assign_root->children[0]->name[0] = 'p';
	assign_root->children[0]->name[1] = 'r';
	assign_root->children[0]->name[2] = 'i';
	assign_root->children[0]->name[3] = 'n';
	assign_root->children[0]->name[4] = 't';
	assign_root->children[0]->no_children = 0;

	int flag;
	if(isAlpha(token[start+5]) == 1)
		flag = 1;
	else	// is a number
		flag = 2;
	if(flag == 1)
		assign_root->children[1] = solve_variable_V1(start+5, end);
	else
		assign_root->children[1] = solve_constant(start+5, end);

return assign_root;

}

struct node* solve_statement(int start, int end)
{
	struct node *assign_root;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root->name_count = 1;
	assign_root->name[0] = 'S';
	assign_root->no_children = 1;
	assign_root->children[0] =(struct node *)malloc(sizeof(struct node));

	if(end-start+1 > 6 && token[start] == 'w' && token[start+1] == 'h' && token[start+2] == 'i' && token[start+3] == 'l' && token[start+4] == 'e' && token[start+5] == '(')
	{
		assign_root->children[0] = solve_while(start, end);
	}
	else if(end-start+1 > 4 && token[start] == 'r' && token[start+1] == 'e' && token[start+2] == 'a' && token[start+3] == 'd')
	{
		assign_root->children[0] = solve_read(start, end);
	}
	else if(end-start+1 > 5 && token[start] == 'p' && token[start+1] == 'r' && token[start+2] == 'i' && token[start+3] == 'n' && token[start+4] == 't')
	{
		assign_root->children[0] = solve_print(start, end);
	}
	else
	{
		assign_root->children[0] = solve_assignment(start, end);
	}

	return assign_root;
}


void solve()
{

	struct node *assign_root;
	assign_root =(struct node *)malloc(sizeof(struct node));
	assign_root = solve_prog(0, strlen(token)-1);
	//print(assign_root);

	simulate(assign_root);
	
	
}

void isPresent(char temp[100], char temp_value[100])
{
	int i, flag = 0;
	for(i=0;i<variable_index;i++)
	{
		if(strcmp(variable_name[i], temp) == 0)
		{
			strcpy(variable_value[i], temp_value);
			variable_index++;
			flag=1;
		}
	}
	if(flag == 0)
	{
		strcpy(variable_value[variable_index], temp_value);
		strcpy(variable_name[variable_index], temp);
		variable_index++;
	}
}

void output(char var_name[100])
{
	int i, flag = 0;
	for(i=0;i<variable_index;i++)
	{
		if(strcmp(variable_name[i], var_name) == 0)
		{
			fprintf(fp, "%s\n", variable_value[i]);
			flag = 1;
		}
	}

	// Variable not found
	if(flag == 0)
		fprintf(fp, "Variable Not Declared \n");
}

char * give_value(char var_name[100])
{
	int i;
	for(i=0;i<variable_index;i++)
	{
		int flag = 0;
		if(strcmp(variable_name[i], var_name) == 0)
		{
			flag = 1;
			return variable_value[i];
			
		}
	}

}

#define MAX_PRECISION	(10)
static const float rounders[MAX_PRECISION + 1] =
{
	0.5, 0.05, 0.005, 0.0005, 0.00005, 0.000005, 0.0000005, 0.00000005,	0.000000005, 0.0000000005, 0.00000000005		
};

char * ftoa(float f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;

	// check precision bounds
	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (precision < 0) 
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	if (precision)
		f += rounders[precision];

	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		p = ptr;

		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		p1 = p;

		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		ptr = p1;
	}

	if (precision)
	{
		*ptr++ = '.';

		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	*ptr = 0;

	return buf;
}


/*A function to simulate the Basic C code*/

void simulate(struct node *assign_root)
{
	top = 0;
	stack[top] = assign_root;
	top++;
	
	while(top>=1)
	{
		struct node *popped = stack[top-1];
		top--;
		//printf("%s\n",popped->name);
		if(strcmp(popped->name,"W")==0)
		{
			stack[top] = popped;
			top++;
			if(stack[top-1]->children[2]->state==0)
			{
				stack[top]=stack[top-1]->children[2];
				stack[top]->current_child=0;
				stack[top]->state=1;
				
				makeZero(stack[top]);
				
				top++;
				
			}
			else
			{
				
				if(atoi(stack[top-1]->children[2]->value_name) != 0)
				{		
						stack[top-1]->children[2]->state=0;
						stack[top] = stack[top-1]->children[5];
						stack[top]->current_child=0;
						
						makeZero(stack[top]);
						
						top++;
				}
				else
				{
					stack[top-1]->children[2]->state=0;
					top--;
				}
				
			}
			
		}
		else if(popped->no_children>0 && popped->current_child < popped->no_children)
		{
			stack[top] = popped;
			top++;
			stack[top] = popped->children[popped->current_child];
			popped->current_child++;
			top++;
		}
		else if(top>0&& popped->no_children == 0 && stack[top-1]->current_child == stack[top-1]->no_children && stack[top-1]->current_child == 1)
		{
			strcpy(stack[top-1]->value_name, popped->name);
		}
		else if(top>0 && stack[top-1]->current_child == stack[top-1]->no_children && stack[top-1]->current_child == 1)
		{
			strcpy(stack[top-1]->value_name, popped->value_name);
		}
		else if(top>0 && stack[top-1]->no_children == stack[top-1]->current_child)
		{
			if(strcmp(stack[top-1]->name, "R") == 0)
			{
				char temp[100];
				scanf("%s", temp);
				strcpy(stack[top-1]->value_name, temp);
				isPresent(popped->value_name, temp);

			}
			else if(strcmp(stack[top-1]->name, "O") == 0)
			{
				if(isAlpha(popped->value_name[0]) == 1)
					output(popped->value_name);
				else
					fprintf(fp, "%s\n", popped->value_name);
			}
			else if(strcmp(stack[top-1]->name, "A") == 0)
			{
				// 3 children for assignment
				if(isAlpha(popped->value_name[0]) == 1)
					isPresent(stack[top-1]->children[0]->value_name,give_value(popped->value_name));
				else
					isPresent(stack[top-1]->children[0]->value_name, popped->value_name);

			}
			else if(strcmp(stack[top-1]->name, "E1") == 0)
			{
				
				if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 0 && isAlpha(popped->value_name[0]) == 0)
				{
					if(stack[top-1]->children[1]->name[0] == '+')
					{
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(popped->value_name);
						
						float res = a + b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '-')
					{
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(popped->value_name);
						
						float res = a - b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}	
				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 0 && isAlpha(popped->value_name[0]) == 1)
				{
					if(stack[top-1]->children[1]->name[0] == '+')
					{	
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(give_value(popped->value_name));
						
						float res = a + b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '-')
					{	
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(give_value(popped->value_name));
						float res = a - b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 1 && isAlpha(popped->value_name[0]) == 0)
				{
					if(stack[top-1]->children[1]->name[0] == '+')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof((popped->value_name));
						
						float res = a + b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '-')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof((popped->value_name));
						float res = a - b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 1 && isAlpha(popped->value_name[0]) == 1)
				{
					if(stack[top-1]->children[1]->name[0] == '+')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof(give_value(popped->value_name));
						float res = a + b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '-')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof(give_value(popped->value_name));
						float res = a - b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
			}
			else if(strcmp(stack[top-1]->name, "E2") == 0)
			{
				if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 0 && isAlpha(popped->value_name[0]) == 0)
				{
					if(stack[top-1]->children[1]->name[0] == '*')
					{
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(popped->value_name);

						float res = a * b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '/')
					{
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(popped->value_name);
						float res = a / b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}	
				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 0 && isAlpha(popped->value_name[0]) == 1)
				{
					if(stack[top-1]->children[1]->name[0] == '*')
					{	
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(give_value(popped->value_name));
						float res = a * b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '/')
					{	
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(give_value(popped->value_name));
						float res = a / b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 1 && isAlpha(popped->value_name[0]) == 0)
				{
					if(stack[top-1]->children[1]->name[0] == '*')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof((popped->value_name));
						float res = a * b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '/')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof((popped->value_name));
						float res = a / b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 1 && isAlpha(popped->value_name[0]) == 1)
				{
					if(stack[top-1]->children[1]->name[0] == '*')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof(give_value(popped->value_name));
						float res = a * b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '/')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof(give_value(popped->value_name));
						float res = a / b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
			}
			else if(strcmp(stack[top-1]->name, "E3") == 0)
			{
				strcpy(stack[top-1]->value_name, stack[top-1]->children[1]->value_name);
			}
			else if(strcmp(stack[top-1]->name, "Vi") == 0)
			{
				
				strcpy(stack[top-1]->value_name,stack[top-1]->children[0]->value_name);
				strcat(stack[top-1]->value_name,"[");
				strcat(stack[top-1]->value_name, stack[top-1]->children[2]->value_name);
				strcat(stack[top-1]->value_name,"]");

			}
			else if(strcmp(stack[top-1]->name, "E") == 0)
			{
				if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 0 && isAlpha(popped->value_name[0]) == 0)
				{
					if(stack[top-1]->children[1]->name[0] == '<')
					{
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(popped->value_name);

						float res = a < b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '=')
					{
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(popped->value_name);
						float res = a == b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}	
				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 0 && isAlpha(popped->value_name[0]) == 1)
				{
					if(stack[top-1]->children[1]->name[0] == '<')
					{	
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(give_value(popped->value_name));
						float res = a < b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '=')
					{	
						float a = atof(stack[top-1]->children[0]->value_name);
						float b = atof(give_value(popped->value_name));
						float res = a == b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 1 && isAlpha(popped->value_name[0]) == 0)
				{
					if(stack[top-1]->children[1]->name[0] == '<')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof((popped->value_name));
						float res = a < b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '=')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof((popped->value_name));
						float res = a == b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
				else if(isAlpha(stack[top-1]->children[0]->value_name[0]) == 1 && isAlpha(popped->value_name[0]) == 1)
				{
					if(stack[top-1]->children[1]->name[0] == '<')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof(give_value(popped->value_name));
						float res = a < b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}
					else if(stack[top-1]->children[1]->name[0] == '=')
					{	
						float a = atof(give_value(stack[top-1]->children[0]->value_name));
						float b = atof(give_value(popped->value_name));
						float res = a == b;
						char buffer[10];
						ftoa(res, buffer, 10);
						strcpy(stack[top-1]->value_name, buffer);
					}

				}
			}	
		}
	}

}


int main()
{
	fp=fopen("output.txt", "w");
	tokenizer();

	//root node
	root =(struct node *)malloc(sizeof(struct node));
	root->name[0] = 'P';
	root-> name_count = 1;

	solve();
	
	
	






	
	return 0;
}
