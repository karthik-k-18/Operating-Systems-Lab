#include<bits/stdc++.h>
using namespace std;

int current_pages=0;

class linkedlist{
    public:
    int page;
    linkedlist *next;
    linkedlist(int page){
        this->page = page;
        this->next = NULL;
    }
};

linkedlist *head = NULL;
linkedlist *tail = NULL;

void insert(linkedlist *head, linkedlist *tail, int page){
    if(head == NULL){
        head = new linkedlist(page);
        tail = head;
        return;
    }
    linkedlist *temp = new linkedlist(page);
    tail->next = temp;
    tail = temp;
}

void replace(linkedlist *head, linkedlist *tail, int page, unordered_map<int,int> &hash){
    linkedlist *temp = head;
    head = head->next;
    hash[temp->page] = 0;
    delete temp;
    insert(head,tail,page);
    hash[page] = 1;
}

void interchange(linkedlist *head, linkedlist *tail, int page){
    if(head->page == page){
        head=head->next;
        insert(head,tail,page);
        return;
    }
    linkedlist *prev = head, *curr = head;
    while(curr->page != page){
        prev = curr;
        curr = curr->next;
    }
    if(curr==tail) return;
    prev->next = curr->next;
    insert(head,tail,page);
    //delete curr;
    free (curr);
}

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
        cout << "Number of page faults: " << 0 << endl;
        return 0;
    }
    if(hash.size() > blocks){
        cout << "The number of blocks can't fit into swap space" << endl;
        return 0;
    }
    hash.clear();
    //traverse the input file again
    file.clear();
    file.seekg(0, ios::beg);
    
    while(file>>page){
        if(hash[page] != 1){
            if(current_pages<frames){
                current_pages++;
                hash[page] = 1;
                insert(head,tail,page);
            }
            else{
                replace(head,tail,page,hash);
            }
            page_faults++;
        }else{
            interchange(head,tail,page);
        }
    }
    cout<< "Number of physical frames: " << frames << endl;
    cout << "Number of page faults: " << page_faults << endl;
}