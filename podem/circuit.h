#ifndef CIRCUIT_H
#define CIRCUIT_H
#include "fault.h"
#include "tfault.h"
#include "bfault.h"
#include "ReadPattern.h"
#include <stdlib.h>
#include <stack>


typedef GATE* GATEPTR;

class CIRCUIT
{
    private:
        string Name;
        PATTERN Pattern;
        vector<GATE*> Netlist;
        vector<GATE*> PIlist; //store the gate indexes of PI
        vector<GATE*> POlist;
        vector<GATE*> PPIlist;
        vector<GATE*> PPOlist;
        list<FAULT*> Flist; //collapsing fault list
        list<FAULT*> UFlist; //undetected fault list
        list<TFAULT*> TFlist; //collapsing fault list
        list<TFAULT*> UTFlist; //undetected fault list
        list<BFAULT*> BFlist; //bridging fault list
        list<BFAULT*> UBFlist; //undetected bridging fault list
        unsigned MaxLevel;
        unsigned BackTrackLimit; //backtrack limit for Podem
        typedef list<GATE*> ListofGate;
        typedef list<GATE*>::iterator ListofGateIte;
        ListofGate* Queue;
        ListofGate GateStack;
        ListofGate PropagateTree;
        ListofGateIte QueueIte;

    public:
        //Initialize netlist
        CIRCUIT(): MaxLevel(0), BackTrackLimit(10000) {
            Netlist.reserve(32768);
            PIlist.reserve(128);
            POlist.reserve(512);
            PPIlist.reserve(2048);
            PPOlist.reserve(2048);
        }
        CIRCUIT(unsigned NO_GATE, unsigned NO_PI = 128, unsigned NO_PO = 512,
                unsigned NO_PPI = 2048, unsigned NO_PPO = 2048) {
            Netlist.reserve(NO_GATE);
            PIlist.reserve(NO_PI);
            POlist.reserve(NO_PO);
            PPIlist.reserve(NO_PPI);
            PPOlist.reserve(NO_PPO);
        }
        ~CIRCUIT() {
            for (unsigned i = 0;i<Netlist.size();++i) { delete Netlist[i]; }
            list<FAULT*>::iterator fite;
            for (fite = Flist.begin();fite!=Flist.end();++fite) { delete *fite; }
            /*
             *list<BFAULT*>::iterator fite2;
             *for (fite2 = BFlist.begin();fite2!=BFlist.end();++fite2) { delete *fite2; }
             */
        }

        void AddGate(GATE* gptr) { Netlist.push_back(gptr); }
        void SetName(string n){ Name = n;}
        string GetName(){ return Name;}
        int GetMaxLevel(){ return MaxLevel;}
        void SetBackTrackLimit(unsigned bt) { BackTrackLimit = bt; }

        //Access the gates by indexes
        GATE* Gate(unsigned index) { return Netlist[index]; }
        GATE* PIGate(unsigned index) { return PIlist[index]; }
        GATE* POGate(unsigned index) { return POlist[index]; }
        GATE* PPIGate(unsigned index) { return PPIlist[index]; }
        GATE* PPOGate(unsigned index) { return PPOlist[index]; }
        unsigned No_Gate() { return Netlist.size(); }
        unsigned No_PI() { return PIlist.size(); }
        unsigned No_PO() { return POlist.size(); }
        unsigned No_PPI() { return PPIlist.size(); }
        unsigned No_PPO() { return PPOlist.size(); }

        void InitPattern(const char *pattern) {
            Pattern.Initialize(const_cast<char *>(pattern), PIlist.size(), "PI");
        }

        void Schedule(GATE* gptr) {
            if (!gptr->GetFlag(SCHEDULED)) {
                gptr->SetFlag(SCHEDULED);
                Queue[gptr->GetLevel()].push_back(gptr);
            }
        }

        //defined in circuit.cc
        void Levelize();
        void FanoutList();
        void Check_Levelization();
        void SetMaxLevel();
        void SetupIO_ID();

        //defined in sim.cc
        void SetPPIZero(); //Initialize PPI state
        void InitializeQueue();
        void ScheduleFanout(GATE*);
        void SchedulePI();
        void SchedulePPI();
        void LogicSimVectors();
        void LogicSim();
        void PrintIO();
        VALUE Evaluate(GATEPTR gptr);

        //defined in atpg.cc
        void GenerateAllFaultList();
        void GenerateCheckPointFaultList();
        void GenerateFaultList();
        int No_Flist() { return Flist.size(); }
        int No_UFlist() { return UFlist.size(); }
        void Atpg();
        void SortFaninByLevel();
        bool CheckTest();
        bool TraceUnknownPath(GATEPTR gptr);
        bool FaultEvaluate(FAULT* fptr);
        ATPG_STATUS Podem(FAULT* fptr, unsigned &total_backtrack_num);
        ATPG_STATUS SetUniqueImpliedValue(FAULT* fptr);
        ATPG_STATUS BackwardImply(GATEPTR gptr, VALUE value);
        GATEPTR FindPropagateGate();
        GATEPTR FindHardestControl(GATEPTR gptr);
        GATEPTR FindEasiestControl(GATEPTR gptr);
        GATEPTR FindPIAssignment(GATEPTR gptr, VALUE value);
        GATEPTR TestPossible(FAULT* fptr);
        void TraceDetectedStemFault(GATEPTR gptr, VALUE val);

        //defined in fsim.cc
        void MarkOutputGate();
        void MarkPropagateTree(GATEPTR gptr);
        void FaultSimVectors();
        void FaultSim();
        void FaultSimEvaluate(GATE* gptr);
        bool CheckFaultyGate(FAULT* fptr);
        void InjectFaultValue(GATEPTR gptr, unsigned idx,VALUE value);

	//defined in psim.cc for parallel logic simulation
	void ParallelLogicSimVectors();
	int ParallelLogicSim(); //modified to return number of gate evaluations
	void ParallelEvaluate(GATEPTR gptr);
	void PrintParallelIOs(unsigned idx);
	void ScheduleAllPIs();

	//defined in stfsim.cc for single pattern single transition-fault simulation
	void GenerateAllTFaultList();
	void TFaultSimVectors();
	void TFaultSim_t();
	void TFaultSim();
	bool CheckTFaultyGate(TFAULT* fptr);
	bool CheckTFaultyGate_t(TFAULT* fptr);
	VALUE Evaluate_t(GATEPTR gptr);
	void LogicSim_t();
        void PrintTransition();
        void PrintTransition_t();
        void PrintIO_t();

	//defined in tfatpg.cc for transition fault ATPG
	void TFAtpg();
	ATPG_STATUS Initialization(GATEPTR gptr, VALUE target, unsigned &total_backtrack_num);
	ATPG_STATUS BackwardImply_t(GATEPTR gptr, VALUE value);
	GATEPTR FindPIAssignment_t(GATEPTR gptr, VALUE value);
	GATEPTR FindEasiestControl_t(GATEPTR gptr);
	GATEPTR FindHardestControl_t(GATEPTR gptr);

    // -------------------------------------------------------------------------------- //
    
    //defined in circuit.cc for VLSI Testing - Assignment0
    void PrintCircuitInfo();

    //defined in circuit.cc for VLSI Testing - Assignment1
    int PrintAllPaths_Stack(const char* startPI, const char* endPO);
    void PrintAllPaths_Recur(GATE* startGate, GATE* endGate, vector<bool>& visitedGate, vector<GATE*>& path, int& cnt);

    //defined in circuit.cc for VLSI Testing - Assignment2
    void GenerateRandPattern(const char* output, int num, bool unknown);
    //defined in sim.cc for VLSI Testing - Assignment2
    void MLogicSimVectors();
    void MLogicSim();
    VALUE MEvaluate(GATEPTR gptr);
    VALUE IntToValue(bitset<2> i);
    int ValueToInt(VALUE v);

    //defined in circuit.cc for VLSI Testing - Assignment3
    void Simulator(const char* output);
    //defined in psim.cc for VLSI Testing - Assignment3
    void Parallel(ofstream &f);

    //defined in bfsim.cc for VLIS Testing - Assignment4
    void GenerateAllBFaultList(const char* output);
    void BFaultSimVectors();
    void BFaultSim();
    void BFaultSimEvaluate(GATE* gptr);
    bool CheckBFaultyGate(BFAULT* fptr);
};
#endif
