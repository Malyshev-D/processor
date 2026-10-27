#ifndef STRUCTARR
#define STRUCTARR

enum cmd {
    HLT,
    PUSH,
    ADD,
    SUB,
    DIV,
    AWAY,
    MUL,
    POPr,
    PUSHr,
    JMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE,
    CALL,
    RET,
    DUMP,
    SCAN,
    SQRT,
    AX = 1,
    BX,
    CX,
    DX
};

#ifdef PROCESSOR
#define VM_FUNCTIONS(...) __VA_ARGS__
#else
#define VM_FUNCTIONS(...)
#endif

struct commands_str {
    int         id;
    const char* word;
    VM_FUNCTIONS(
    void        (*Function)(processor_t*);
    int         arg_number;)
};

const int CMD_AMOUNT = 20;
const int REG_AMOUNT = 4;

#ifdef CMDARRAY// TODO: universal function
commands_str cmd_array[] = {
{HLT,   "Hlt"   VM_FUNCTIONS(, Hlt,              0)},
{PUSH,  "PUSH"  VM_FUNCTIONS(, Push,             1)},
{ADD,   "ADD"   VM_FUNCTIONS(, Add,              0)},
{SUB,   "SUB"   VM_FUNCTIONS(, Sub,              0)},
{DIV,   "DIV"   VM_FUNCTIONS(, Div,              0)},
{AWAY,  "AWAY"  VM_FUNCTIONS(, Away,             0)},
{MUL,   "MUL"   VM_FUNCTIONS(, Mul,              0)},
{POPr,  "POPr"  VM_FUNCTIONS(, Pop_reg,          1)},
{PUSHr, "PUSHr" VM_FUNCTIONS(, Push_reg,         1)},
{JMP,   "JMP"   VM_FUNCTIONS(, Jump,             1)},
{JA,    "JA"    VM_FUNCTIONS(, JumpIfAbove,      1)},
{JAE,   "JAE"   VM_FUNCTIONS(, JumpIfAboveEqual, 1)},
{JB,    "JB"    VM_FUNCTIONS(, JumpIfBelow,      1)},
{JBE,   "JBE"   VM_FUNCTIONS(, JumpIfBelowEqual, 1)},
{JE,    "JE"    VM_FUNCTIONS(, JumpIfEqual,      1)},
{JNE,   "JNE"   VM_FUNCTIONS(, JumpIfNotEqual,   1)},
{CALL,  "CALL"  VM_FUNCTIONS(, Call,             1)},
{RET,   "RET"   VM_FUNCTIONS(, Ret,              0)},
{DUMP,  "DUMP"  VM_FUNCTIONS(, Dump,             0)},
{SCAN,    "SCAN"    VM_FUNCTIONS(, Input,               0)},
{SQRT,  "SQRT"  VM_FUNCTIONS(, Sqrt,             0)}
};
#endif

#ifdef REGISTER_ARRAY
commands_str reg_array[] = {
{AX,    "AX"},
{BX,    "BX"},
{CX,    "CX"},
{DX,    "DX"}
};
#endif

#endif
