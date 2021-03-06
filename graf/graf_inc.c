#define MAX_OPS   MAX_LINE
#define MAX_VALUES  (MAX_LINE/2)
#define MAX_FUNCS (MAX_LINE/2)
#define STACK_LEN (MAX_LINE/2)
char compiled[MAX_OPS];
byte num_steps;
typedef double (*func)(double);
func functions[MAX_FUNCS];
byte num_functions;
double values[MAX_VALUES];
byte num_values;
double stack[STACK_LEN];
double mypow(double x,double y);
struct
{
    char *name;
    func f;
    byte length;
} function_table[]=
{
/* must be ordered so that if one function name is an initial substring
   of another, the longer one comes first: thus, tanh comes before tan */
    {"abs", fabs,3},
    {"floor", floor,5},
    {"ceil", ceil,4},
    {"sqrt", sqrt,4},
    {"sinh", sinh,4},
    {"cosh", cosh,4},
    {"tanh", tanh,4},
    {"sin", sin,3},
    {"cos", cos,3},
    {"tan", tan,3},
    {"arcsin", asin,6},
    {"asin", asin,4},
    {"arccos", acos,6},
    {"acos", acos,4},
    {"arctan", atan,6},
    {"atan", atan,4},
    {"log10", log10,5},
    {"log", log,3},
    {"ln", log,2},
    {"round",_frndint,5},
    {"exp", exp,3}
};

#define FUNCTION_TABLE_SIZE (sizeof(function_table)/sizeof(*function_table))

int yylex(void);
