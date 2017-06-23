#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <elf.h>

enum {
    NOTYPE = 256, or, and, NEQ, EQ, plus, minus, divide, multiply, point, negative, not, regname, left, right, decimal, hexadecimal, variable
    
    /* TODO: Add more token types */
    
};


static struct rule {
    char *regex;
    int token_type;
} rules[] = {
    
    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */
    
    {" +",	NOTYPE},					// spaces
    {"==", EQ},							// equal
    {"\\+", plus},						// plus
    {"-", minus},						// minus
    {"\\*", multiply},					// times
    {"/", divide},						// divide
    {"\\(", left},						// left bracket
    {"\\)", right},						// right bracket
    {"0x[0-9a-fA-F]+", hexadecimal},	// %x
    {"[0-9]+", decimal},				// %d
    {"\\$[a-zA-Z]+", regname},			// register name
    {"!=", NEQ},						// not equal
    {"&&", and},						// and
    {"\\|\\|", or},						// or
    {"!", not},							// not
	{"[a-z0-9A-Z_]+", variable},		// variable
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;
    
    for(i = 0; i < NR_REGEX; i ++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if(ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token {
    int type;
    char str[32];
    int priority;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;
    
    nr_token = 0;
    
    while(e[position] != '\0') {
        /* Try all rules one by one. */
        for(i = 0; i < NR_REGEX; i ++) {
            if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                
                //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
                position += substr_len;
                
                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array ``tokens''. For certain
                 * types of tokens, some extra actions should be performed.
                 */
                switch(rules[i].token_type) {
                    case EQ:
                        tokens[nr_token].priority = 3;
                        tokens[nr_token++].type = EQ;
                        break;
                    case plus:
                        tokens[nr_token].priority = 4;
                        tokens[nr_token++].type = plus;
                        break;
                    case minus:
                        tokens[nr_token].priority = 4;
                        tokens[nr_token++].type = minus;
                        break;
					case negative:
                        tokens[nr_token].priority = 6;
						tokens[nr_token++].type = negative;
						break;
                    case multiply:
                        tokens[nr_token].priority = 5;
                        tokens[nr_token++].type = multiply;
                        break;
					case point:
                        tokens[nr_token].priority = 6;
						tokens[nr_token++].type = point;
						break;
                    case divide:
                        tokens[nr_token].priority = 5;
                        tokens[nr_token++].type = divide;
                        break;
                    case left:
                        tokens[nr_token].priority = 7;
                        tokens[nr_token++].type = left;
                        break;
                    case right:
                        tokens[nr_token].priority = 7;
                        tokens[nr_token++].type = right;
                        break;
                    case regname:
                        tokens[nr_token].priority = 8;
                        tokens[nr_token].type = regname;
                        strncpy (tokens[nr_token++].str, substr_start, substr_len);
                        break;
                    case NEQ:
                        tokens[nr_token].priority = 3;
                        tokens[nr_token++].type = NEQ;
                        break;
                    case and:
                        tokens[nr_token].priority = 2;
                        tokens[nr_token++].type = and;
                        break;
                    case or:
                        tokens[nr_token].priority = 1;
                        tokens[nr_token++].type = or;
                        break;
                    case not:
                        tokens[nr_token].priority = 6;
                        tokens[nr_token++].type = not;
                        break;
                    case hexadecimal:
                        tokens[nr_token].priority = 8;
                        tokens[nr_token].type = hexadecimal;
                        strncpy (tokens[nr_token++].str, substr_start, substr_len + 1);
                        break;
                    case decimal:
                        tokens[nr_token].priority = 8;
                        tokens[nr_token].type = decimal;
                        strncpy (tokens[nr_token++].str, substr_start, substr_len);
                        break;
					case variable:
						tokens[nr_token].priority = 8;
						tokens[nr_token].type = variable;
						strncpy (tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token++].str[substr_len] = '\0'; 
					/*default: panic("please implement me");*/
                }           
                
                break;
             }
         }
        
        if(i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
     }
    return true;
}

bool check_parentheses(int p, int q)
{
    int i;
    int num = 0;
    if (tokens[p].type != left || tokens[q].type != right)
        return false;
    else
    {
        for (i = p; i < q + 1; ++i)
        {
            if (tokens[i].type == left)
                num++;
            if (tokens[i].type == right)
                num--;
        }
        if (num != 0)
        {
            assert(0);
            return false;
        }
        else
        {
            num = 0;
            for (i = p; i < q + 1; ++i)
            {
                if (tokens[i].type == left)
                {
                    num++;
                }
                if (tokens[i].type == right)
                {
                    num--;
                }
                if (num == 0 && i != q)
                {
                    return false;
                    break;
                }
            }
        }
    }
    return true;
}

uint32_t eval(int p, int q)
{
    int i, j;
    int num = 0, cnt = 0, op = 0x7FFFFFFF;
    uint32_t val1, val2, val, ans;
    int op_candidate[32];
    //printf("kan\n");
    if (p > q)
    {
        assert(0);
    }
    else if (p == q)
    {
		if (tokens[p].type == variable)
		{
			return variable_print(tokens[p].str);
		}
        if (tokens[p].type == decimal || tokens[p].type == hexadecimal)
        {
            ans = (uint32_t)(strtol(tokens[p].str, NULL, 0));
            //printf("p=q %d\n", ans);
            return ans;
        }
        if (tokens[p].type == regname)
        {
            if (strcmp(tokens[p].str, "$eax") == 0)
                return cpu.eax;
            if (strcmp(tokens[p].str, "$edx") == 0)
                return cpu.edx;
            if (strcmp(tokens[p].str, "$ecx") == 0)
                return cpu.ecx;
            if (strcmp(tokens[p].str, "$ebx") == 0)
                return cpu.ebx;
            if (strcmp(tokens[p].str, "$ebp") == 0)
                return cpu.ebp;
            if (strcmp(tokens[p].str, "$esi") == 0)
                return cpu.esi;
            if (strcmp(tokens[p].str, "$edi") == 0)
                return cpu.edi;
            if (strcmp(tokens[p].str, "$esp") == 0)
                return cpu.esp;
            if (strcmp(tokens[p].str, "$eip") == 0)
                return cpu.eip;
        }
    }
    else if (check_parentheses(p, q) == true)
    {
        return eval(p + 1, q - 1);
    }
    else
    {
        for (i = p; i < q + 1; ++i)
        {
            if ((tokens[i].type == plus) || (tokens[i].type == minus) || (tokens[i].type == multiply) || (tokens[i].type == divide) || (tokens[i].type == and) || (tokens[i].type == or) || tokens[i].type == EQ || tokens[i].type == NEQ || tokens[i].type == not || tokens[i].type == negative || tokens[i].type == point)
            {
                num = 0;
                //printf("fuck!%d\n",i);
                for (j = i + 1; j < q + 1; ++j)
                {
                    //printf("fuck!\n");
                    if (tokens[j].type == left)
                    {
                        num--;
                    }
                    if (tokens[j].type == right)
                    {
                        num++;
                    }
                }
                if (num == 0)
                {
                    op_candidate[cnt++] = i;
                    //printf("can: %d\n", i);
                }
            }
        }
        if (cnt == 1)
            op = op_candidate[0];
        else
        {
            int min = op_candidate[0];
            for (i = 1; i < cnt; ++i)
            {
				if (tokens[min].type == not || tokens[min].type == negative || tokens[min].type == point)
					op = tokens[min].priority <= tokens[op_candidate[i]].priority ? min : op_candidate[i];
				else
					op = tokens[min].priority < tokens[op_candidate[i]].priority ? min : op_candidate[i];
			}
        }
        //printf("op: %d\n", op);
        int op_type = tokens[op].type;
        if(tokens[op].type == not || tokens[op].type == negative || tokens[op].type == point)
        {
            val = eval(op + 1, q);
            switch (op_type)
            {
                case not:
                    return !val;
                case negative:
                    return -val;
                case point:
                    return swaddr_read(val, 4);
            }
        }
        else
        {
            val1 = eval(p, op - 1);
            val2 = eval(op + 1, q);
            switch (op_type)
            {
                case plus:
                    return val1 + val2;
                case minus:
                    return val1 - val2;
                case multiply:
                    return val1 * val2;
                case divide:
                    return val1 / val2;
                case and:
                    return val1 && val2;
                case or:
                    return val1 || val2;
                case EQ:
                    return val1 == val2;
                case NEQ:
                    return val1 != val2;
                default:
                    assert(0);
            }
        }
    }
    return 0;
}

uint32_t expr(char *e, bool *success) {
    if(!make_token(e)) {
        *success = false;
        return 0;
    }
    //printf("%d\n", nr_token - 1);
    /* TODO: Insert codes to evaluate the expression. */
    int i;
    for (i = 0; i < nr_token; ++i)
    {
        if (tokens[i].type == multiply && (i == 0 || (tokens[i - 1].type != right && tokens[i - 1].type != decimal && tokens[i - 1].type != hexadecimal && tokens[i - 1].type != regname)))
        {
            tokens[i].type = point;
            tokens[i].priority = 6;
        }
        if (tokens[i].type == minus && (i == 0 || (tokens[i - 1].type != right && tokens[i - 1].type != decimal && tokens[i - 1].type != hexadecimal && tokens[i - 1].type != regname)))
        {
            tokens[i].type = negative;
            tokens[i].priority = 6;
        }

	}
    /*for (i = 0; i < nr_token; ++i)
    {
        printf("type %d\n", tokens[i].type);
        printf("type %d\n", tokens[i].priority);
    }*/
    uint32_t ans = eval(0, nr_token - 1);
    /*panic("please implement me");*/
    return ans;
}

