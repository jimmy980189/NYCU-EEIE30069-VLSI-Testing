#include <iostream> 
#include <fstream>
//#include <alg.h>
#include "circuit.h"
#include "GetLongOpt.h"
using namespace std;

extern GetLongOpt option;

void CIRCUIT::FanoutList()
{
    unsigned i = 0, j;
    GATE* gptr;
    for (;i < No_Gate();i++) {
        gptr = Gate(i);
        for (j = 0;j < gptr->No_Fanin();j++) {
            gptr->Fanin(j)->AddOutput_list(gptr);
        }
    }
}

void CIRCUIT::Levelize()
{
    list<GATE*> Queue;
    GATE* gptr;
    GATE* out;
    unsigned j = 0;
    for (unsigned i = 0;i < No_PI();i++) {
        gptr = PIGate(i);
        gptr->SetLevel(0);
        for (j = 0;j < gptr->No_Fanout();j++) {
            out = gptr->Fanout(j);
            if (out->GetFunction() != G_PPI) {
                out->IncCount();
                if (out->GetCount() == out->No_Fanin()) {
                    out->SetLevel(1);
                    Queue.push_back(out);
                }
            }
        }
    }
    for (unsigned i = 0;i < No_PPI();i++) {
        gptr = PPIGate(i);
        gptr->SetLevel(0);
        for (j = 0;j < gptr->No_Fanout();j++) {
            out = gptr->Fanout(j);
            if (out->GetFunction() != G_PPI) {
                out->IncCount();
                if (out->GetCount() ==
                        out->No_Fanin()) {
                    out->SetLevel(1);
                    Queue.push_back(out);
                }
            }
        }
    }
    int l1, l2;
    while (!Queue.empty()) {
        gptr = Queue.front();
        Queue.pop_front();
        l2 = gptr->GetLevel();
        for (j = 0;j < gptr->No_Fanout();j++) {
            out = gptr->Fanout(j);
            if (out->GetFunction() != G_PPI) {
                l1 = out->GetLevel();
                if (l1 <= l2)
                    out->SetLevel(l2 + 1);
                out->IncCount();
                if (out->GetCount() ==
                        out->No_Fanin()) {
                    Queue.push_back(out);
                }
            }
        }
    }
    for (unsigned i = 0;i < No_Gate();i++) {
        Gate(i)->ResetCount();
    }
}

void CIRCUIT::Check_Levelization()
{

    GATE* gptr;
    GATE* in;
    unsigned i, j;
    for (i = 0;i < No_Gate();i++) {
        gptr = Gate(i);
        if (gptr->GetFunction() == G_PI) {
            if (gptr->GetLevel() != 0) {
                cout << "Wrong Level for PI : " <<
                gptr->GetName() << endl;
                exit( -1);
            }
        }
        else if (gptr->GetFunction() == G_PPI) {
            if (gptr->GetLevel() != 0) {
                cout << "Wrong Level for PPI : " <<
                gptr->GetName() << endl;
                exit( -1);
            }
        }
        else {
            for (j = 0;j < gptr->No_Fanin();j++) {
                in = gptr->Fanin(j);
                if (in->GetLevel() >= gptr->GetLevel()) {
                    cout << "Wrong Level for: " <<
                    gptr->GetName() << '\t' <<
                    gptr->GetID() << '\t' <<
                    gptr->GetLevel() <<
                    " with fanin " <<
                    in->GetName() << '\t' <<
                    in->GetID() << '\t' <<
                    in->GetLevel() <<
                    endl;
                }
            }
        }
    }
}

void CIRCUIT::SetMaxLevel()
{
    for (unsigned i = 0;i < No_Gate();i++) {
        if (Gate(i)->GetLevel() > MaxLevel) {
            MaxLevel = Gate(i)->GetLevel();
        }
    }
}

//Setup the Gate ID and Inversion
//Setup the list of PI PPI PO PPO
void CIRCUIT::SetupIO_ID()
{
    unsigned i = 0;
    GATE* gptr;
    vector<GATE*>::iterator Circuit_ite = Netlist.begin();
    for (; Circuit_ite != Netlist.end();Circuit_ite++, i++) {
        gptr = (*Circuit_ite);
        gptr->SetID(i);
        switch (gptr->GetFunction()) {
            case G_PI: PIlist.push_back(gptr);
                break;
            case G_PO: POlist.push_back(gptr);
                break;
            case G_PPI: PPIlist.push_back(gptr);
                break;
            case G_PPO: PPOlist.push_back(gptr);
                break;
            case G_NOT: gptr->SetInversion();
                break;
            case G_NAND: gptr->SetInversion();
                break;
            case G_NOR: gptr->SetInversion();
                break;
            default:
                break;
        }
    }
}

void CIRCUIT::PrintCircuitInfo() {
        int numGates_5type = 0; // only 5 types: AND, OR, NOT, NAND, NOR
        int numSignalNets = 0;
        int numBranchNets = 0;
        int numStemNets = 0;
        int numGateType = 12;
        int numTotalFanout = 0;
        double numAveFanout = 0;

        vector<int> numGateEach(numGateType, 0);
        vector<string> gateFunc {"G_PI", "G_PO", "G_PPI", "G_PPO", "G_NOT", "G_AND", "G_NAND", "G_OR", "G_NOR", "G_DFF", "G_BUF", "G_BAD"};

        for (unsigned int i = 0; i < this->No_Gate(); i++) {
            GATE* tmp = this->Gate(i);

            ++numGateEach[tmp->GetFunction()];
            numTotalFanout += tmp->No_Fanout();

            if (tmp->No_Fanout() > 1) {
                ++numStemNets;
                numBranchNets += tmp->No_Fanout();
            }
        }
        numSignalNets = numTotalFanout + numStemNets;
        numAveFanout = (double) numTotalFanout / this->No_Gate();

        numGates_5type = numGateEach[G_NOT] + numGateEach[G_OR] + numGateEach[G_NOR] + numGateEach[G_AND] + numGateEach[G_NAND];

        cout << "---------------------------------------" << endl;

        cout << "Number of inputs: " << this->No_PI() << endl;
        cout << "Number of outputs: " << this->No_PO() << endl;
        cout << "Total number of gates including inverter, or, nor, and, nand: " << numGates_5type << endl;

        cout << "Number of gates for each type: " << endl;
        for (int i = 0; i < numGateType; i++)
            cout << "\t" << gateFunc[i] << " " << numGateEach[i] << endl;

        cout << "Number of flip-flops: " << numGateEach[G_PPI] << endl;
        cout << "Total number of signal nets: " << numSignalNets << endl;
        cout << "Number of branch nets: " << numBranchNets << endl;
        cout << "Number of stem nets: " << numStemNets << endl;
        cout << "Average number of fanouts of each gate (all types): " << numAveFanout << endl;

        cout << "---------------------------------------" << endl;
}

// Reference Code from: https://www.geeksforgeeks.org/find-paths-given-source-destination/
void CIRCUIT::PrintAllPaths_Recur(GATE* startGate, GATE* endGate, vector<bool>& visitedGate, vector<GATE*>& path, int& cnt) {

    int u = startGate->GetID();
    int d = endGate->GetID();

    visitedGate[u] = true;
    path.push_back(startGate);

    if (startGate->GetLevel() > endGate->GetLevel())
        return;
 
    // If current vertex is same as destination, then print
    // current path[]
    if (u == d) {
        ++cnt;
        for (long unsigned int i = 0; i < path.size(); i++)
            cout << path[i]->GetName() << " ";
        cout << endl;
    }
    else
        // If current vertex is not destination
        // Recur for all the vertices adjacent to current
        // vertex
        for (unsigned int i = 0; i < startGate->No_Fanout(); i++)
            if (!visitedGate[startGate->Fanout(i)->GetID()])
                this->PrintAllPaths_Recur(startGate->Fanout(i), endGate, visitedGate, path, cnt);
    // Remove current vertex from path[] and mark it as
    // unvisitedGate
    path.pop_back();
    visitedGate[u] = false;
}

// Non-recurrsive version
int CIRCUIT::PrintAllPaths_Stack(const char* startPI, const char* endPO) {

    vector<bool> visitedGate(this->No_Gate(), false);
    vector<GATE*> path;
    stack<GATE*> s;
    GATE* tmpGate = NULL;
    GATE* startGate = NULL;
    GATE* endGate = NULL;
    GATE* currentGate = NULL;
    unsigned int cnt = 0;
    unsigned int preLevel = 0;

    for (unsigned int i = 0; i < this->No_PI(); i++) {
        tmpGate = this->PIGate(i);
        if (tmpGate->GetName() == startPI) {
            startGate = tmpGate;
            break;
        }
    }

    for (unsigned int i = 0; i < this->No_PO(); i++) {
        tmpGate = this->POGate(i);
        if (tmpGate->GetName() == endPO) {
            endGate = tmpGate;
            break;
        }
    }

    if (!(startGate && endGate)) {
        cout << "No such start PI gate or end PO gate" << endl;
        exit(-1);
    }

    s.push(startGate);

    // DFS
    while (!s.empty()) {

        currentGate = s.top();
        s.pop();

        // KEY constrain to enhance performance
        // Skip the gate if its level is larger than target PO gate (is deeper than target)
        if (currentGate->GetLevel() > endGate->GetLevel())
            continue;

        while (currentGate->GetLevel() < preLevel) {
            preLevel = path.back()->GetLevel();
            path.pop_back();
        }

        path.push_back(currentGate);
    
        if (currentGate->GetName() == endGate->GetName()) {
            for (unsigned int i = 0; i < path.size(); i++)
                cout << path[i]->GetName() << " ";
            cout << endl;
            ++cnt;
        }
        else
            for (unsigned int i = 0; i < currentGate->No_Fanout(); i++)
                s.push(currentGate->Fanout(i));

        preLevel = currentGate->GetLevel();
    }

    cout << "---------------------------------------" << endl;
    cout << "The paths from " << startPI << " to " << endPO << ": " << cnt << endl;
    cout << "---------------------------------------" << endl;

    return cnt;
}

void CIRCUIT::GenerateRandPattern(const char* output, int num, bool unknown) {

    ofstream f(output);
    
    srand(time(0));

    if(f.fail()) {
        cout << "Failed to generate file (Missing output parameter)" << endl;
        exit(-1);
    }

    for (unsigned int i = 0; i < this->No_PI(); i++)
        f << "PI " << this->PIGate(i)->GetName() << " ";
    f << endl;

    for (int i = 0; i < num; i++) {
        int tmp = 0;
        string patternGernerated;
        for (unsigned int j = 0; j < this->No_PI(); j++) {
            if (!unknown) {
                tmp = (int) (2.0 * rand() / (RAND_MAX + 1.0));
                patternGernerated += to_string(tmp);
            }
            else {
                tmp = (int) (3.0 * rand() / (RAND_MAX + 1.0));
                switch (tmp) {
                    case 0:
                        patternGernerated += '0';
                        break;
                    case 1:
                        patternGernerated += '1';
                        break;
                    case 2:
                        patternGernerated += 'X';
                        break;
                }
            }
        }
        f << patternGernerated << endl;
    }

    cout << "Finish generating random pattern (output file is " << output << ")" << endl;
}

void CIRCUIT::Simulator(const char* output) {

    int start = string(output).find_last_of("/");
    int end = string(output).find_last_of(".");
    string filename = string(output).substr(start + 1, end - start - 1);

    ofstream f(output);

    if(f.fail()) {
        cout << "Failed to generate file" << endl;
        exit(-1);
    }

    // GenOutputFile

    // Initialize
    f << "#include <iostream>" << endl;
    f << "#include <ctime>" << endl;
    f << "#include <bitset>" << endl;
    f << "#include <string>" << endl;
    f << "#include <fstream>" << endl;
    f << endl;

    f << "using namespace std;" << endl;
    f << "const unsigned PatternNum = 16;" << endl;
    f << endl;

    f << "void evaluate();" << endl;
    f << "void printIO(unsigned idx);" << endl;
    f << endl;

    for (unsigned i = 0; i < this->No_Gate(); i++)
        f << "bitset<PatternNum> G_" << this->Gate(i)->GetName() << "[2];" << endl;
    f << "bitset<PatternNum> temp;" << endl;
    
    f << "ofstream fout(\"" << filename << ".out\",ios::out);" << endl;
    f << endl;

    // main
    f << "int main()" << endl;
    f << "{" << endl;
    f << "clock_t time_init, time_end;" << endl;
    f << "time_init = clock();" << endl;

    //FIX divided input pattern in to 16..
    //ex: 24 -> 16 8
    list<GATE*> gQueue;
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
    bool isListCompleted = false;
    while(!Pattern.eof()) { 
        for(pattern_idx=0; pattern_idx<PatternNum; pattern_idx++) {
            if(!Pattern.eof()){ 
                ++pattern_num;
                Pattern.ReadNextPattern(pattern_idx);
            }
            else break;
        }

        // extra
        for (unsigned i = 0; i < this->No_PI(); i++) {
            f << "G_" << this->PIGate(i)->GetName() << "[0] = ";
            f << this->PIGate(i)->GetWireValue(0).to_ulong() << " ;" << endl; //GX_0
            f << "G_" << this->PIGate(i)->GetName() << "[1] = ";
            f << this->PIGate(i)->GetWireValue(1).to_ulong() << " ;" << endl; //GX_1
        }
        f << endl;
        f << "evaluate();" << endl;
        f << "printIO(" << pattern_idx << ");" << endl;
        f << endl;

        ScheduleAllPIs();

        // ParallelLogicSim()
        GATE* gptr;
        if (!isListCompleted)
            for (unsigned i = 0;i <= MaxLevel;i++) {
                while (!Queue[i].empty()) {
                    gptr = Queue[i].front();
                    Queue[i].pop_front();
                    gptr->ResetFlag(SCHEDULED);
                    //ParallelEvaluate(gptr); 

                    ScheduleFanout(gptr);
                    gQueue.push_back(gptr);
                }
            }
        isListCompleted = true;
    }

    f << "time_end = clock();" << endl;
    f << "cout << \"Total CPU Time = \" << double(time_end - time_init)/CLOCKS_PER_SEC << endl;" << endl;
    f << "system(\"ps aux | grep a.out \");" << endl;
    f << "return 0;" << endl;
    f << "}" << endl;

    // evaluate
    f << "void evaluate()" << endl;
    f << "{" << endl;

    // Gate operation
    for (auto gptr : gQueue) {
        switch(gptr->GetFunction()) {
            case G_AND:
            case G_NAND:
                for (unsigned i = 0; i < gptr->No_Fanin(); ++i) {
                    if (i == 0) {
                        f << "G_" << gptr->GetName() << "[0] = G_" << gptr->Fanin(i)->GetName() << "[0] ;";
                        f << endl;
                        f << "G_" << gptr->GetName() << "[1] = G_" << gptr->Fanin(i)->GetName() << "[1] ;";
                        f << endl;
                    }
                    else  {
                        f << "G_" << gptr->GetName() << "[0] &= G_" << gptr->Fanin(i)->GetName() << "[0] ;";
                        f << endl;
                        f << "G_" << gptr->GetName() << "[1] &= G_" << gptr->Fanin(i)->GetName() << "[1] ;";
                        f << endl;
                    }
                }
                break;
            case G_OR:
            case G_NOR:
                for (unsigned i = 0; i < gptr->No_Fanin(); ++i) {
                    if (i == 0) {
                        f << "G_" << gptr->GetName() << "[0] = G_" << gptr->Fanin(i)->GetName() << "[0] ;";
                        f << endl;
                        f << "G_" << gptr->GetName() << "[1] = G_" << gptr->Fanin(i)->GetName() << "[1] ;";
                        f << endl;
                    }
                    else  {
                        f << "G_" << gptr->GetName() << "[0] |= G_" << gptr->Fanin(i)->GetName() << "[0] ;";
                        f << endl;
                        f << "G_" << gptr->GetName() << "[1] |= G_" << gptr->Fanin(i)->GetName() << "[1] ;";
                        f << endl;
                    }
                }
                break;
            case G_NOT:
            case G_PO:
            case G_BUF:
                f << "G_" << gptr->GetName() << "[0] = G_" << gptr->Fanin(0)->GetName() << "[0] ;";
                f << endl;
                f << "G_" << gptr->GetName() << "[1] = G_" << gptr->Fanin(0)->GetName() << "[1] ;";
                f << endl;
                break;
            default: 
                cout << gptr->GetFunction() << endl;
                break;
        } 
        if (gptr->Is_Inversion()) {
            f << "temp = G_" << gptr->GetName() << "[0] ;" << endl;
            f << "G_" << gptr->GetName() << "[0] = ~G_" << gptr->GetName() << "[1] ;" << endl;
            f << "G_" << gptr->GetName() << "[1] = ~temp" << " ;" << endl;
        }
    }

    f << "}" << endl;

    // printIO
    f << "void printIO(unsigned idx)" << endl;
    f << "{" << endl;
    f << "for (unsigned j=0; j<idx; j++)" << endl;
    f << "{" << endl;

    for (int i = 0; i < 1; i++) {
        // PIlist
        for (unsigned i = 0; i < this->No_PI(); i++) {
            f << "    if (G_" << this->PIlist[i]->GetName() << "[0][j]==0)" << endl;
            f << "    {" << endl;
            f << "        if (G_" << this->PIlist[i]->GetName() << "[1][j]==1)" << endl;
            f << "            fout << \"F\";" << endl;
            f << "        else" << endl;
            f << "            fout << \"0\";" << endl;
            f << "    }" << endl;
            f << "    else" << endl;
            f << "    {" << endl;
            f << "        if(G_" << this->PIlist[i]->GetName() << "[1][j] == 1)" << endl;
            f << "            fout << \"1\";" << endl;
            f << "        else" << endl;
            f << "            fout << \"2\";" << endl;
            f << "    }" << endl;
        }

        f << "fout<<\" \";" << endl;

        // POlist
        for (unsigned i = 0; i < this->No_PO(); i++) {
            f << "    if(G_" << this->POlist[i]->GetName() << "[0][j]==0)" << endl;
            f << "    {" << endl;
            f << "        if(G_" << this->POlist[i]->GetName() << "[1][j]==1)" << endl;
            f << "            fout<<\"F\";" << endl;
            f << "        else" << endl;
            f << "            fout<<\"0\";" << endl;
            f << "    }" << endl;
            f << "    else" << endl;
            f << "    {" << endl;
            f << "        if(G_" << this->POlist[i]->GetName() << "[1][j]==1)" << endl;
            f << "            fout<<\"1\";" << endl;
            f << "        else" << endl;
            f << "            fout<<\"2\";" << endl;
            f << "    }" << endl;
        }
    }
    f << "fout << endl;" << endl;
    f << "}" << endl;
    f << "}" << endl;

    f.close();
}
