#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "rb.h"
long Time = 0;
long excT=-1;
using namespace std;
class Proc;
RB_Tree<int, Proc*> rbTree;
vector<int> jobVec;
vector<Proc*> allJobs;
std::ofstream outFile("output_file.txt",ios::out);
class Proc
{
	public:
		Proc(long start,int id, long total, int type, int job2=-1)
        {
			start_ =start;
			jobId_ = id;
			executed_time_ =0;
			total_time_ =total;
            type_ = type;
            jobId2_ = job2;
            };
		int scheduler(RB_Tree<int, Proc*>& rbTree)
        {
            int runTime = 0;
            switch(type_) {
                case 0:{
		  if (total_time_ - executed_time_>= 5) {
                        if (!allJobs.empty() && (allJobs.back())->getStart() < Time+5) {
                            runTime =(allJobs.back())->getStart() -Time;
                        } else 
                            runTime = 5;
                        executed_time_ += runTime;
                    } else {
                        if (!allJobs.empty() && (allJobs.back())->getStart() < Time+total_time_ - executed_time_) {
                            runTime =(allJobs.back())->getStart()-Time;
                        } else 
                            runTime = total_time_ - executed_time_;
                        executed_time_ +=runTime;
                    }
                    if (total_time_ == executed_time_) {
                        rbTree.Delete(jobId_);
                    }
                    break;
		
                    }
                case 1:{
                    RB_Tree<int, Proc*>::RB_Node* it = rbTree.find(jobId_);
                    Proc* proc = it->data;
                    if (jobId2_ == -1) {

                        if (proc != NULL) {
                            outFile<<"("<<proc->getJobId()<<","<<proc->getExecT()<<","<<proc->getTotalT()<<")";
                        } else {
                            outFile<<"(0,0,0)";
                        }
                    } else {
                        int count = 0;
                        RB_Tree<int, Proc*>::RB_Node* node = rbTree.getNode();
                        for (vector<int>::iterator it = jobVec.begin(); it != jobVec.end(); ++it) {
                            RB_Tree<int, Proc*>::RB_Node* find = rbTree.find(*it);
                            if (find && find->data && find->data->getJobId() <= jobId2_ && find->data->getJobId()>=jobId_) {
                                proc = find->data;
                                if (count != 0)
                                    outFile<<",";
                                
                                outFile<<"("<<proc->getJobId()<<","<<proc->getExecT()<<","<<proc->getTotalT()<<")";
                                count++;
                            }
                        }
                        if (count == 0) {
                            outFile<<"(0,0,0)";
                        }
            
                    }
                    outFile<<endl;
                    break;
                    }
                case 3:{
                    Proc* proc = NULL;
                    RB_Tree<int, Proc*>::RB_Node* find = rbTree.find(jobId_);
                    if (find->left->left->data)
			  proc = find->left->left->data;
		     else 
			proc = find->left->data;
		    /*if (find == rbTree.getNullNode()){
                        RB_Tree<int, Proc*>::RB_Node* preNode = find->parent;
                        if (preNode && preNode->parent) {
                            find = preNode;
                            preNode = preNode->parent;
                            if (preNode->right == find) { //modify
                                proc = preNode->right->data; //modify
                            } else {
                                proc = preNode->left->data;
                            }
                        }
                    } else {
                        if (find->left && find->left->data) {
                            proc = find->left->data;
                        } else if (find->parent && find->parent->right == find) {
                            proc = find->parent->data;
                        }
                    } */
                    if (proc && proc->getJobId() < jobId_)
                        outFile<<"("<<proc->getJobId()<<","<<proc->getExecT()<<","<<proc->getTotalT()<<")"<<endl;
                    else
                        outFile<<"(0,0,0)"<<endl;
		    
                    break;}
                case 2:{
                    Proc* proc = NULL;
                    RB_Tree<int, Proc*>::RB_Node* find = rbTree.find(jobId_);
                    if (find->right->right->data)
			proc=find->right->right->data;
		    else 
			proc=find->right->data;
		    /* if (find == rbTree.getNullNode()){
                        RB_Tree<int, Proc*>::RB_Node* preNode = find->parent;
                        if (preNode && preNode->parent) {
                     
                            if (preNode->left == find) { //modify
                                proc = preNode->right->data;
                            } else {
                                proc = preNode->left->data;
                            }
                        }
                    }else {
                        if (find->right && find->right->data) {
                            proc = find->right->data;
                        } else if (find->parent && find->parent->left == find) {
                            proc = find->parent->data;
                        }
                    } */
                    if (proc && proc->getJobId() > jobId_)
						outFile<<"("<<proc->getJobId()<<","<<proc->getExecT()<<","<<proc->getTotalT()<<")"<<endl;
                    else
                        outFile<<"(0,0,0)"<<endl;
                    break;}
                default:
                    break;
            }
            return runTime;
        }
		int getJobId(){return jobId_;}
        int getType(){return type_;}
		long getExecT(){return executed_time_;}
	void setTime(int time){executed_time_-=time;Time-=time;}
        long getTotalT(){return total_time_;}
        long getStart(){return start_;}
	private:
	      long start_;
		int jobId_;
		long executed_time_;
		long total_time_;
        int type_; // 0 insert; 1 print; 2 next 3 previous
        int jobId2_;
};

template<class out_type,class in_value>
out_type convert(const in_value & t)
{
    std::stringstream stream;
    stream<<t;
    out_type result;
    stream>>result;
    return result;
}

	int CAPACITY = 0;
	Proc* MINHEAP[1500]={NULL};
    void exchangeVal(int i , int j)  // exchange data
	{
		Proc*  temp = MINHEAP[i];
		MINHEAP[i] = MINHEAP[j];
		MINHEAP[j] = temp;
	}
    void judgeMinHeap(int i) // judgement min heap
	{
		int l = 2*i+1;
		int r = 2*i+2;
		int small = i;
		if (l < CAPACITY && MINHEAP[i]->getExecT() > MINHEAP[l]->getExecT()) {
			small = l;  // i > left: small = left
		}
		if (r < CAPACITY && MINHEAP[r]->getExecT() < MINHEAP[small]->getExecT()) {
			small = r; // i < right: small = right
		}
		if (i != small) {
			exchangeVal(i, small); //exchange i with small
			judgeMinHeap(small); // continue judgement min heap
		}
	}
	void pushMinHeap(Proc* data) // save data to min heap
	{
        if (data->getType() != 0) {
            delete data;
            return ;
        } 
        if (data->getTotalT() == data->getExecT()) {
            delete data;
            return ;
        }
		int i = CAPACITY;
		MINHEAP[CAPACITY++] = data;
		while (i>0 && MINHEAP[i]->getExecT() < MINHEAP[(i-1)/2]->getExecT()){
			exchangeVal(i, (i-1)/2);
			i = (i-1)/2;
		}
	}
	
	Proc* popMinHeap() // get data from min heap
	{
        if (CAPACITY == 0)
            return NULL;
		CAPACITY--;
		Proc* r = MINHEAP[0];
        MINHEAP[0] = NULL;
		exchangeVal(CAPACITY, 0);
		judgeMinHeap(0);
		return r;
	}



int  main(int argc, char* argv[])
{
    std::ifstream input(argv[1],ios::in);
    Proc* curProc = NULL;
    std::string commandStr;
  
    while (!input.eof()) { // read file
        std::getline(input, commandStr);
        if (commandStr.empty())
            continue;
        Proc* proc = NULL;
	   excT = 0; //executed_time
	    int jobId = -1; // jobId
	    long totalT = 0; // total time
	    size_t found = commandStr.find_first_of(":");
	    excT = convert<long>(commandStr.substr(0,found));
	    
	    if (commandStr.find("Insert") != string::npos) {  // Insert
	    	found = commandStr.find_first_of(",");
	    	jobId = convert<long>(commandStr.substr(commandStr.find("(")+1,found-commandStr.find("(")));
	    	totalT = convert<long>(commandStr.substr(found+1, commandStr.find(")")- found-1));	
            proc = new Proc(excT,jobId, totalT, 0); // create Proc
           // rbTree.Insert(jobId, proc); //add proc tu rbTree
            //jobVec.push_back(jobId); 
	    } else if (commandStr.find("PrintJob") != string::npos) {//printJob
            int job1 = -1;
            int job2 = -1;
			found = commandStr.find(")");
            size_t comma = commandStr.find(",");
            if (comma == string::npos) { // printJob(job1)
                job1 = convert<long>(commandStr.substr(commandStr.find("(")+1, commandStr.find(",")));
            } else { //printJob(job1, job2)
				job1 = convert<long>(commandStr.substr(commandStr.find("(")+1, commandStr.find(",")));
                job2 = convert<long>(commandStr.substr(commandStr.find(",")+1, commandStr.find(")")));
            }
            commandStr.erase(0, found+1);
            proc = new Proc(excT,job1, totalT, 1, job2); //create proc
        } else if (commandStr.find("NextJob") != string::npos) { //NextJob
            found = commandStr.find_first_of(")");
            int job = convert<long>(commandStr.substr(commandStr.find("(")+1, found-commandStr.find("(")-1));
            proc = new Proc(excT, job, totalT, 2);
        } else if (commandStr.find("PreviousJob") != string::npos) { //previousJob
            found = commandStr.find_first_of(")");
            int job = convert<long>(commandStr.substr(commandStr.find("(")+1, found-commandStr.find("(")-1));
            proc = new Proc(excT,job, totalT, 3);
        }
        allJobs.insert(allJobs.begin(),proc);
	}
	
while (!allJobs.empty() || (curProc=popMinHeap()) != NULL) {  //modify
        if (!allJobs.empty()) {
            Proc* nextCmd = allJobs.back();
            if (Time==nextCmd->getStart()) {
                allJobs.pop_back();
                if (nextCmd->getType() == 0) {
                    rbTree.Insert(nextCmd->getJobId(), nextCmd);
                    jobVec.push_back(nextCmd->getJobId());
                }
                curProc = nextCmd;
            } else {
                curProc = popMinHeap();
            }
        }
        if (curProc) {
            ///cout<<" cmd:"<<curProc->getJobId()<<" "<<curProc->getExecT()<<" "<<curProc->getTotalT()<<endl;
            Time += curProc->scheduler(rbTree);
            pushMinHeap(curProc);
        } else {
            Time++;
        }
    }
	return 0;
}
