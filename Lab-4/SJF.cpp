#include<bits/stdc++.h>
using namespace std;

//class for process

class Process{
    public:
    int pid; //process id
    int arrival_time; //arrival time
    int burst_time; //burst time
    int cpu_burst_time; //cpu burst time
    vector<int> cpu_burst; //cpu burst time
    vector<int> io_burst; //io burst time
    int cpu_burst_size; //cpu burst size
    int io_burst_size; //io burst size
    int waiting_time; //waiting time
    int turnaround_time; //turnaround time
    int response_time; //response time
    int completion_time; //completion time
    int remaining_time; //remaining time
    bool is_this_first_time; //is this first time
};

struct comparator {
    bool operator()(pair<int, Process> &a, pair<int, Process> &b){
        if(a.first == b.first) return a.second.pid < b.second.pid;
        return a.first < b.first;
    }
};

int main(int argc, char *argv[]){
    //open the file from input argument 1 in read mode
    FILE *fp = fopen(argv[1], "r");
    //if file is not found
    if(fp == NULL){
        cout<<"File not found"<<endl;
        return 0;
    }
    //vector to store the process
    vector<Process> process;
    //now read the file line by line upto the end of file using while loop
    int pid=1;
    while(!feof(fp)){
        //create a process object for each process
        Process p;
        p.is_this_first_time = true;
        p.pid = pid;
        pid++;
        // first line of each process contains arrival time and consecutive cpu burst and io burst
        //first element is arrival time
        int arrival_time;
        fscanf(fp, "%d", &arrival_time);
        p.arrival_time = arrival_time;
        //now read the cpu burst and io burst
        int cpu_burst, io_burst;
        //end of line is indicated by -1
        while(fscanf(fp, "%d", &cpu_burst) && cpu_burst != -1){
            p.cpu_burst.push_back(cpu_burst);
            fscanf(fp, "%d", &io_burst);
            if(io_burst!=-1) p.io_burst.push_back(io_burst);
            else break;
        }
        // reverse the cpu burst and io burst vector
        reverse(p.cpu_burst.begin(), p.cpu_burst.end());
        reverse(p.io_burst.begin(), p.io_burst.end());
        p.burst_time = accumulate(p.cpu_burst.begin(), p.cpu_burst.end(), 0);
        p.cpu_burst_time = p.burst_time;
        //push the process in the vector
        process.push_back(p);
    }

    // now sort the process according to arrival time
    sort(process.begin(), process.end(), [](Process a, Process b){
        return a.arrival_time < b.arrival_time;
    });

    int n = process.size();
    Process cpu_process, io_process;
    bool cpu_idle = true, io_idle = true,not_finished = true;
    // priority queue of pair of int and process
    // first element of pair is the burst time and second element is the process
    // comparator is used to sort the queue according to burst time
auto cmp = [](pair<int, Process> &a, pair<int, Process> &b) {
    if(a.first == b.first) return a.second.pid > b.second.pid;
    return a.first > b.first;
};

priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(cmp)> cpu_queue(cmp);
priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(cmp)> io_queue(cmp);

    int time = 0;
    //cpu shoudn't be idle
    int i=0;
    while(not_finished){
        while(i<n && process[i].arrival_time <= time){
            cpu_queue.push({process[i].burst_time, process[i]});
            i++;
        }
        if(cpu_idle){
            if(!cpu_queue.empty()){
                cpu_process = cpu_queue.top().second;
                cpu_queue.pop();
                if(cpu_process.is_this_first_time){
                    for(int k=0;k<n;k++){
                        if(process[k].pid == cpu_process.pid){
                            process[k].response_time = time - process[k].arrival_time;
                            cpu_process.is_this_first_time = false;
                            break;
                        }
                    }
                }
                cpu_idle = false;
                continue;
            }
        }else{
            cpu_process.burst_time--;
            cpu_process.cpu_burst[cpu_process.cpu_burst.size()-1]--;
            if(cpu_process.cpu_burst[cpu_process.cpu_burst.size()-1] == 0){
                if(cpu_process.io_burst.size() == 0){
                    for(int k=0;k<n;k++){
                        if(process[k].pid == cpu_process.pid){
                            process[k].completion_time = time+1;
                            break;
                        }
                    }
                }else{
                    cpu_process.cpu_burst.pop_back();
                    io_queue.push({cpu_process.io_burst[cpu_process.io_burst.size()-1], cpu_process});
                    time++;
                    cpu_idle=true;
                    continue;
                }
                cpu_idle = true;
            }
        }
        if(io_idle){
            if(!io_queue.empty()){
                io_process = io_queue.top().second;
                io_queue.pop();
                io_idle = false;
            }
        }else{
            io_process.io_burst[io_process.io_burst.size()-1]--;
            if(io_process.io_burst[io_process.io_burst.size()-1] == 0){
                int x=io_process.cpu_burst.size();
                if(io_process.cpu_burst.size() == 0){
                    //check pid of all process and if it is equal to io_process.pid then set its completion time
                    for(int k=0;k<n;k++){
                        if(process[k].pid == io_process.pid){
                            process[k].completion_time = time+1;
                            break;
                        }
                    }

                }else{
                    io_process.io_burst.pop_back();
                    cpu_queue.push({io_process.burst_time, io_process});
                }
                io_idle = true;
            }
        }
        time++;
       if(cpu_queue.empty() && io_queue.empty() && cpu_idle && io_idle) not_finished = false;
    }
    //print the completion time of each process and its pid
    int avg_wait_time=0, avg_turnaround_time=0, avg_response_time=0;
    for(auto p: process){
        p.waiting_time = p.completion_time - p.arrival_time - p.cpu_burst_time;
        avg_wait_time += p.waiting_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        avg_turnaround_time += p.turnaround_time;
        avg_response_time += p.response_time;
        cout<<"Process "<<p.pid<<" completion time: "<<p.completion_time<<" arrival time: "<<p.arrival_time<<" waiting time: "<<p.waiting_time<<" turnaround time: "<<p.turnaround_time<<" response time: "<<p.response_time<<endl;
    }
    avg_wait_time /= n;
    avg_turnaround_time /= n;
    avg_response_time /= n;

    cout<<"Average waiting time: "<<avg_wait_time<<endl;
    cout<<"Average turnaround time: "<<avg_turnaround_time<<endl;
    cout<<"Average response time: "<<avg_response_time<<endl;
}