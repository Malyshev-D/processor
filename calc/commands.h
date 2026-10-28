#ifndef COMMANDS
#define COMMANDS

void Push            (processor_t* vm);
void Add             (processor_t* vm);
void Sub             (processor_t* vm);
void Mul             (processor_t* vm);
void Div             (processor_t* vm);
void Away            (processor_t* vm);
void Hlt             (processor_t* vm);
void Pop_reg         (processor_t* vm);
void Push_reg        (processor_t* vm);
void Jump            (processor_t* vm);
void JumpIfAbove     (processor_t* vm);
void JumpIfAboveEqual(processor_t* vm);
void JumpIfBelow     (processor_t* vm);
void JumpIfBelowEqual(processor_t* vm);
void JumpIfEqual     (processor_t* vm);
void JumpIfNotEqual  (processor_t* vm);
void Call            (processor_t* vm);
void Ret             (processor_t* vm);
void Dump            (processor_t* vm);
void Scan            (processor_t* vm);
void Sqrt            (processor_t* vm);

#endif
