enum Bool{FALSE, TRUE};

typedef struct variable {
	char name[100];	
	int index;
	int	value;
	int isValidInput;
	int defined;
	struct variable* next;
} variable;

typedef struct gate{
	char type[100];
	int	inputNum;
	int index;
	int* indices;
	struct gate* next;
} gate;

int inputNum;
int outputNum;
int numVariables = 0;
int numGates = 0;
variable* varList = NULL;
gate* gateList = NULL;
variable* trueVar;
variable* falseVar;

void addInitialVariables(char args[], int isInput);
variable* addVariable(char name[]);
void addGate(char gateType[], char args[]);
int translateToGrayCode(int num);
void addCircuitComponent(char line[]);
void initialInputs(int sequenceNum);
void doCircuit();
variable* getVarName(char name[100]);
variable* getVariable(int index);
gate* getGate(int index);
int logBaseTwo(int input) ;
int powerOfTwo(int exponent) ;

void circuitSort();
int getvarDefineAtGate(variable* undefined);
