#include <iostream>
#include <ctime>
#include <bitset>
#include <string>
#include <fstream>

using namespace std;
const unsigned PatternNum = 16;

void evaluate();
void printIO(unsigned idx);

bitset<PatternNum> G_G1[2];
bitset<PatternNum> G_G2[2];
bitset<PatternNum> G_G3[2];
bitset<PatternNum> G_G4[2];
bitset<PatternNum> G_G5[2];
bitset<PatternNum> G_PO_G16[2];
bitset<PatternNum> G_G16[2];
bitset<PatternNum> G_PO_G17[2];
bitset<PatternNum> G_G17[2];
bitset<PatternNum> G_n60[2];
bitset<PatternNum> G_net14[2];
bitset<PatternNum> G_net17[2];
bitset<PatternNum> G_net25[2];
bitset<PatternNum> G_net18[2];
bitset<PatternNum> temp;
ofstream fout("c17_unknown_100.out",ios::out);

int main()
{
clock_t time_init, time_end;
time_init = clock();
G_G1[0] = 54858 ;
G_G1[1] = 17922 ;
G_G2[0] = 30558 ;
G_G2[1] = 848 ;
G_G3[0] = 46847 ;
G_G3[1] = 41490 ;
G_G4[0] = 29661 ;
G_G4[1] = 17113 ;
G_G5[0] = 59700 ;
G_G5[1] = 10548 ;

evaluate();
printIO(16);

G_G1[0] = 53055 ;
G_G1[1] = 19727 ;
G_G2[0] = 52095 ;
G_G2[1] = 51999 ;
G_G3[0] = 13039 ;
G_G3[1] = 621 ;
G_G4[0] = 56655 ;
G_G4[1] = 32837 ;
G_G5[0] = 64391 ;
G_G5[1] = 25094 ;

evaluate();
printIO(16);

G_G1[0] = 48937 ;
G_G1[1] = 35105 ;
G_G2[0] = 26116 ;
G_G2[1] = 17924 ;
G_G3[0] = 65147 ;
G_G3[1] = 61034 ;
G_G4[0] = 24491 ;
G_G4[1] = 21282 ;
G_G5[0] = 44285 ;
G_G5[1] = 42028 ;

evaluate();
printIO(16);

G_G1[0] = 5623 ;
G_G1[1] = 374 ;
G_G2[0] = 1703 ;
G_G2[1] = 32 ;
G_G3[0] = 63478 ;
G_G3[1] = 46800 ;
G_G4[0] = 44507 ;
G_G4[1] = 1473 ;
G_G5[0] = 65527 ;
G_G5[1] = 39733 ;

evaluate();
printIO(16);

G_G1[0] = 32723 ;
G_G1[1] = 4304 ;
G_G2[0] = 49139 ;
G_G2[1] = 45795 ;
G_G3[0] = 65502 ;
G_G3[1] = 27100 ;
G_G4[0] = 50925 ;
G_G4[1] = 18153 ;
G_G5[0] = 25045 ;
G_G5[1] = 16772 ;

evaluate();
printIO(16);

G_G1[0] = 44981 ;
G_G1[1] = 35076 ;
G_G2[0] = 56051 ;
G_G2[1] = 39072 ;
G_G3[0] = 35768 ;
G_G3[1] = 33680 ;
G_G4[0] = 65451 ;
G_G4[1] = 32512 ;
G_G5[0] = 65359 ;
G_G5[1] = 20237 ;

evaluate();
printIO(16);

G_G1[0] = 44987 ;
G_G1[1] = 35080 ;
G_G2[0] = 56055 ;
G_G2[1] = 39072 ;
G_G3[0] = 35774 ;
G_G3[1] = 33688 ;
G_G4[0] = 65454 ;
G_G4[1] = 32516 ;
G_G5[0] = 65353 ;
G_G5[1] = 20225 ;

evaluate();
printIO(4);

time_end = clock();
cout << "Total CPU Time = " << double(time_end - time_init)/CLOCKS_PER_SEC << endl;
system("ps aux | grep a.out ");
return 0;
}
void evaluate()
{
G_net17[0] = G_G1[0] ;
G_net17[1] = G_G1[1] ;
G_net17[0] &= G_G3[0] ;
G_net17[1] &= G_G3[1] ;
temp = G_net17[0] ;
G_net17[0] = ~G_net17[1] ;
G_net17[1] = ~temp ;
G_n60[0] = G_G2[0] ;
G_n60[1] = G_G2[1] ;
G_n60[0] |= G_G5[0] ;
G_n60[1] |= G_G5[1] ;
temp = G_n60[0] ;
G_n60[0] = ~G_n60[1] ;
G_n60[1] = ~temp ;
G_net14[0] = G_G3[0] ;
G_net14[1] = G_G3[1] ;
G_net14[0] &= G_G4[0] ;
G_net14[1] &= G_G4[1] ;
temp = G_net14[0] ;
G_net14[0] = ~G_net14[1] ;
G_net14[1] = ~temp ;
G_net18[0] = G_net14[0] ;
G_net18[1] = G_net14[1] ;
G_net18[0] &= G_G2[0] ;
G_net18[1] &= G_G2[1] ;
temp = G_net18[0] ;
G_net18[0] = ~G_net18[1] ;
G_net18[1] = ~temp ;
G_net25[0] = G_net14[0] ;
G_net25[1] = G_net14[1] ;
temp = G_net25[0] ;
G_net25[0] = ~G_net25[1] ;
G_net25[1] = ~temp ;
G_G16[0] = G_net17[0] ;
G_G16[1] = G_net17[1] ;
G_G16[0] &= G_net18[0] ;
G_G16[1] &= G_net18[1] ;
temp = G_G16[0] ;
G_G16[0] = ~G_G16[1] ;
G_G16[1] = ~temp ;
G_G17[0] = G_n60[0] ;
G_G17[1] = G_n60[1] ;
G_G17[0] |= G_net25[0] ;
G_G17[1] |= G_net25[1] ;
temp = G_G17[0] ;
G_G17[0] = ~G_G17[1] ;
G_G17[1] = ~temp ;
G_PO_G16[0] = G_G16[0] ;
G_PO_G16[1] = G_G16[1] ;
G_PO_G17[0] = G_G17[0] ;
G_PO_G17[1] = G_G17[1] ;
}
void printIO(unsigned idx)
{
for (unsigned j=0; j<idx; j++)
{
    if (G_G1[0][j]==0)
    {
        if (G_G1[1][j]==1)
            fout << "F";
        else
            fout << "0";
    }
    else
    {
        if(G_G1[1][j] == 1)
            fout << "1";
        else
            fout << "2";
    }
    if (G_G2[0][j]==0)
    {
        if (G_G2[1][j]==1)
            fout << "F";
        else
            fout << "0";
    }
    else
    {
        if(G_G2[1][j] == 1)
            fout << "1";
        else
            fout << "2";
    }
    if (G_G3[0][j]==0)
    {
        if (G_G3[1][j]==1)
            fout << "F";
        else
            fout << "0";
    }
    else
    {
        if(G_G3[1][j] == 1)
            fout << "1";
        else
            fout << "2";
    }
    if (G_G4[0][j]==0)
    {
        if (G_G4[1][j]==1)
            fout << "F";
        else
            fout << "0";
    }
    else
    {
        if(G_G4[1][j] == 1)
            fout << "1";
        else
            fout << "2";
    }
    if (G_G5[0][j]==0)
    {
        if (G_G5[1][j]==1)
            fout << "F";
        else
            fout << "0";
    }
    else
    {
        if(G_G5[1][j] == 1)
            fout << "1";
        else
            fout << "2";
    }
fout<<" ";
    if(G_PO_G16[0][j]==0)
    {
        if(G_PO_G16[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G_PO_G16[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
    if(G_PO_G17[0][j]==0)
    {
        if(G_PO_G17[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G_PO_G17[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
fout << endl;
}
}
