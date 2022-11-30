#ifndef BFAULT_H
#define BFAULT_H
#include "gate.h"

// Bridging fault
class BFAULT
{
    private:
        VALUE Value;
        GATE* Gate1;
        GATE* Gate2; 

        GATE* Input;
        GATE* Output;

        BFAULT_TYPE Type;

        bool Branch; //fault is on branch
        unsigned EqvFaultNum; //equivalent fault number (includes itself)
        FAULT_STATUS Status;
    public:
        BFAULT(GATE* gptr1, GATE* gptr2, BFAULT_TYPE type, VALUE value): Value(value), Type(type), 
        Gate1(gptr1), Gate2(gptr2), Branch(false), EqvFaultNum(1), Status(UNKNOWN) {}
        ~BFAULT() {}
        VALUE GetValue() { return Value; }
        BFAULT_TYPE GetType() { return Type; }
        GATE* GetGate1() { return Gate1; }
        GATE* GetGate2() { return Gate2; }
        GATE* GetInputGate() { return Input; }
        GATE* GetOutputGate() { return Output; }
        void SetInput(GATE* gptr) { Input = gptr; } 
        void SetOutput(GATE* gptr) { Output = gptr; }
        void SetBranch(bool b) { Branch = b; }
        bool Is_Branch() { return Branch; }
        void SetEqvFaultNum(unsigned n) { EqvFaultNum = n; }
        void IncEqvFaultNum() { ++EqvFaultNum; }
        unsigned GetEqvFaultNum() { return EqvFaultNum; }
        void SetStatus(FAULT_STATUS status) { Status = status; }
        FAULT_STATUS GetStatus() { return Status; }
};

#endif
