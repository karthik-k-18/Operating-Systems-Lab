#include<bits/stdc++.h>
using namespace std;

int main(){
    //open req2.dat file in write mode
    ofstream file;
    file.open("req5.dat");
    // generate 60 random numbers between 1 and 60 with seed 0
    srand(3);
    for(int i=0;i<60;i++){
        int rand_num = rand()%80 + 1;
        file << rand_num << " ";
    }
    file.close();
}