#include<bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[]){
    if(argc != 5){
        cout << "Usage: ./FIFO address_space_size memoery_size swap_space <input_file>" << endl;
        return 0;
    }
    unordered_map<int,int>hash;
    int pages = atoi(argv[1]);
    int frames = atoi(argv[2]);
    int blocks = atoi(argv[3]);
    int page_faults = 0;
    //traverse the input file
    ifstream file(argv[4]);
    //traverse the input file seperated by space of integers store it in hash
    int page;
    while(file >> page){
        hash[page] = 1;
    }
    cout<< "Number of active pages: " << hash.size() << endl;
    if(frames>=pages){
        cout<< "Number of physical frames: " << frames << endl;
        cout << "Number of page faults: " << hash.size() << endl;
        return 0;
    }
    if(hash.size() > blocks+frames){
        cout << "The number of blocks can't fit into swap space" << endl;
        return 0;
    }
    vector<int>vec;
    //clear the hash
    hash.clear();
    //traverse the input file again
    file.clear();
    file.seekg(0, ios::beg);
    while(file>>page){
        if(hash[page] != 1){
            if(vec.size()<frames){
                vec.push_back(page);
                hash[page] = 1;
            }
            else{
                int temp = vec.front();
                vec.erase(vec.begin());
                hash[temp] = 0;
                vec.push_back(page);
                hash[page] = 1;
            }
            page_faults++;
        }else{
            vec.erase(find(vec.begin(),vec.end(),page));
            vec.push_back(page);
        }
    }
    cout<< "Number of physical frames: " << frames << endl;
    cout << "Number of page faults: " << page_faults << endl;
}