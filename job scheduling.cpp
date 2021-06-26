#include <iostream>
#include <string>

using namespace std;
int Timer = 0; // the global clock timer for running the simulation
int Time_to_Run = 150; // the total time length to emulate the job scheduling process
int time_slice_period = 10; // set the length of a time_slice

class JobNode
{
public:
    JobNode(int birth, int complete, string namein)
    {
        birth_time = birth;
        complete_time = complete;
        remained_time  = complete;
        name = namein;
        prev = NULL;
        next = NULL;
        //cout << birth_time << ": " << remained_time << ":  " << name << ":  " << endl;
    }
    int birth_time; // the time the job appears
    int remained_time; // the remaining time to finish the job
    int complete_time; // the total time needed to complete the job
    string name; // the name of the job
    JobNode* prev; // pointer to next node
    JobNode* next; // pointer to previous node
};
enum Order   // two sorting orders of jobs in the waiting list
{
    By_Birth_Time,
    By_Remaining_Time
};

template<class T>
class DList
{
public:
    DList(); // constructor
    void insert(T& node, int order); // insert the node T into the list according to the specified order (either By_Birth_Time or By_Remaining_Time)
    void insert(T& node, T* at); // insert the node T before the specified node (at)
    bool del(T* nodep); // delete the specified node from the list
    void clear(); // clear the whole list
    int length(); // return the number of jobs in the list
    T* first; // pointer to the 1st node of the list
    T* last; // pointer to the last node of the list
};

class Manager
{
public:
    Manager(int n_serv); // constructor to prepare the specified number (n_serv) of waiting lists for the CPUs
    int dispatch(JobNode* j, int order); // dispatch the given job (j) according to the chosen policy and the specified sorting order of waiting jobs
    void process(int cpu_id, int time_slice); // emulate the serving of jobs on the waiting list of the specified CPU (cpu_id) with the given length for a time_slice
    void showLists();
    int n_servers; // the number of waiting lists
    DList<JobNode> *servers; // an array of waiting lists for all CPUs
};

template<class T>
DList<T>::DList()
{
    first =NULL;
    last =NULL;
}
template<class T>
void DList<T>::insert(T &node, int order)
{
    if (first==NULL)
    {
        first = &node;//if the list in empty, set
        last = &node;

    }
    else
    {
        T *temp = first;//create a temp pointer start from the first
        if(order==0){//by birth time
            while (temp!=NULL){//when the temp pointer has not reach to the end
                if(temp ->birth_time > node.birth_time){//if the node's birth time is smaller then the temp pointer's next, then insert the before temp pointer's next
                    insert(node, temp);
                    break;
                }
                temp = temp->next;//temp pointer point to the next one
            }
            if (temp ==NULL){//when the temp pointer pointer has point to the end, then the node insert to the last
                last->next = &node;
                node.prev = last;
                last = last -> next;
            }
        }
        else{
            while (temp!=NULL){//when the temp pointer has not reach to the end
                if(temp->remained_time > node.remained_time){//if the node's remained time is smaller then the temp pointer's next, then insert the before temp pointer's next
                    insert(node,temp);
                    break;
                }
                temp = temp->next;//temp pointer point to the next one
            }
            if (temp ==NULL){//when the temp pointer pointer has point to the end, then the node insert to the last
                last->next = &node;
                node.prev = last;
                last = last -> next;
            }
        }
    }
}



template <class T>
void DList<T>::insert(T &node, T *at)
{

    if (at == first)//if the node insert at the first
    {
        first = &node;
        at -> prev = &node;
        node.prev = NULL;
        node.next = at;
    }
    //we don't have to consider if the node insert at the last because we already did it in the previous insert function
    else//the node insert in the middle
    {
        node.prev = at -> prev;
        node.next = at;
        at -> prev -> next = &node;
        at -> prev = &node;
    }

}

template<class T>
bool DList<T>::del(T* nodep)
{
    if(first == NULL)//if the first is null that means there is 0 node int the list
        return 0;
    if (nodep == last){//if the node we are going to delete is the last, the last pointer has to point forward to the previous node
        last = last -> prev;
    }
    if (nodep->next!=NULL){//if the node is in the list, connect the nodeprev
        nodep -> next -> prev = nodep -> prev;
    }
    if (nodep == first){//if is the first node, first pointer point to the second node in the list
        first = first -> next;
    }
    if (nodep->prev!=NULL){//if the node is in the list, connect the nodenext
        nodep -> prev -> next = nodep -> next;
    }
    nodep->next = NULL;//set the node next and prev pointer to null;
    nodep->prev = NULL;
    return 1;
}

template<class T>
int DList<T>::length()
{
    JobNode *temp = first;//create a temp pointer start from the begin
    int count=0;
    while(temp != NULL)//when  the pointer has not point to the end
    {
        count++;
        temp = temp -> next;//moving to the next node and add one time, so we know how many node are in the list
    }
    return count;
}

template<class T>
void DList<T>::clear()
{
    while(first!=NULL)//start from the first, delete to the end
    {
        del(first);
    }
}
Manager::Manager(int n_serv)
{
    n_servers = n_serv;
    servers = new DList<JobNode>[n_servers];

}

int Manager::dispatch(JobNode* j, int order)//we used order = remained time as example
{
    int waitinglst = 1000,index = 0,comparesum = 0;
    for (int i=0; i<n_servers; i++){//start from the begining of the list
        servers[i].insert(*j,order);//insert the j node in to the list
        JobNode *temp = servers[i].first;//create a temp pointer point at the first node in the list
        while(temp != j){

            comparesum = temp->remained_time +comparesum;//adding all the remained time up
            temp = temp -> next;// point to the next node and keep adding the remained time up
        }
        if (comparesum < waitinglst){//if the sum of the list's remained time is less then the smallest sum remained of all list ,then the current list should be the list that we are going insert the node j
            waitinglst = comparesum;
            index = i;//mark the list
        }
        comparesum =0;//sum = 0 because we are going to calculate another list
        servers[i].del(j);//delete the insert node, since we have the index, we just have to add j node in to the list when all the comparison is done
    }
    servers[index].insert(*j,order);//insert the node into the index list
    return index;
}

void  Manager::process(int cpu_id, int time_slice)
{
    int temp;
    if (servers[cpu_id].first -> remained_time == time_slice){// if the timeslice = the remained, then  just delete the first remauned time
        servers[cpu_id].del(servers[cpu_id].first);//delete the first remained time
    }
    else if (servers[cpu_id].first -> remained_time > time_slice){//if the timeslice is smaller then the first remained time, then the first remained time become the first remained time - timeslice
        servers[cpu_id].first -> remained_time = servers[cpu_id].first -> remained_time - time_slice;
    }
    else if (servers[cpu_id].first -> remained_time < time_slice){//if the timeslice is lagger then the first remained time, then we will delete the first remained time and the the next nodes remained time will become the the remained time - the timeslice left
        temp = servers[cpu_id].first -> remained_time - time_slice;
        servers[cpu_id].del(servers[cpu_id].first);
        servers[cpu_id].first -> remained_time = temp + servers[cpu_id].first -> remained_time;
    }
}
void Manager::showLists()
{
    for (int i=0; i<n_servers; i++)
    {
        cout << "List " << i+1 << ":";//number of list
        JobNode *temp = servers[i].first;//create a temp pointer start from the first
        while(temp!=NULL)//while the temp pointer hasnt point to the end
        {
            cout << "-[" << temp->name << "," << temp ->birth_time << "," << temp->remained_time << "]";//print out the name ,birth time and the remained time
            temp = temp -> next;//point to the next node
        }
        cout << endl;
    }
    cout << endl;

}
int main()
{
        JobNode* jn;
    Manager* mgr = new Manager(3);
    jn = new JobNode(10, 50, "t1");
    // job dispatching stage
    int order = By_Remaining_Time;
    mgr->servers[0].insert(*(new JobNode(10, 50, "t1")), order);
    mgr->servers[1].insert(*(new JobNode(20, 70, "t2")), order);
    mgr->servers[2].insert(*(new JobNode(30, 40, "t3")), order);
    mgr->showLists();
    mgr->dispatch(new JobNode(40, 50, "t4"), order);
    mgr->showLists();
    mgr->dispatch(new JobNode(35, 60, "t5"), order);
    mgr->showLists();
    mgr->dispatch(new JobNode(45, 30, "t6"), order);
    mgr->showLists();
    mgr->dispatch(new JobNode(25, 70, "t7"), order);
    mgr->showLists();
    mgr->dispatch(new JobNode(55, 60, "t8"), order);
    mgr->showLists();
    // CPU serving stage
    int timeslice = 40;
    for (int i=0; i<3; i++)
        mgr->process(i, timeslice);
    mgr->showLists();
}


