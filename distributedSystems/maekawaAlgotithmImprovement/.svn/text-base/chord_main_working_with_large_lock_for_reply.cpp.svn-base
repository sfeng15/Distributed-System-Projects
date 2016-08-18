//#include "chord_node.h"
#include "spliter.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <queue>

#define TICK_USEC      1000000 // tick length in microseconds currently 0.1s     

#define TICK_MSEC      1000 // tick length in microseconds currently 0.1s          


using namespace std;
using std::chrono::system_clock;
std::chrono::microseconds ts( 2000000 );

//std::this_thread::sleep_for(std::chrono::seconds(3));

//input time
int cs_int ;
int next_req ;
int tot_exec_time ;
//initial time variable
chrono::monotonic_clock::time_point ini_time;

//request class
class request{
	public: 
	int sender_id;
    std::chrono::microseconds timestamp;
  
    bool operator<(const request& a) const
    {
        return timestamp.count() >= a.timestamp.count();
    }
};

//fsm class
enum state { INIT,REQUEST,RELEASE,HELD };
class node_info{
	public: 
	node_info(state a,bool b){

		status=a;
		voted=b;
	}
	state status;
   	bool voted;
  
};
//record each node's status 
vector<node_info> node_status;

int thread_flag=0;
int flag=2;
bool flag1=false;
bool flag2=false;
bool no_deadlock=false;
int node2_counter=0;
int node3_counter=0;
int counter=0;
int count1=0;
int count2=0;
long long i=0;
long long j=0;
//send signal to 5 other nodes in voting sets
//used when multicast requests to other processes
//there are 9 nodes in total
int notified_target[9][5];
//0 target 0,1
//1 target 0,1

//decide whether the current node receive a request
bool notified[9];


//tell how many reply I have received, 9 nodes in total 
bool received_reply[9][5];
//@option substitute?vector< vector<bool> > receive_ack;



//mutex to protect CS for request queue
mutex mtx; 
mutex send_request_mtx; 
mutex t01_lock;
mutex array_lock;
//queue to store request from 5 nodes in voting set
priority_queue<request> t0;
priority_queue<request> t1;
//@?vector< queue<request> > received_request;

request current_request;
request t0_min;
request t1_min;



//vector to contain thread ids
vector<thread> node_threads;
void node_func(int new_id);			
//RPC system
//

//void RPC_send(int node_id_array[5],request request_msg);
void RPC_send(int node_id,request request_msg,state type);


void node_func(int thread_id){


cout<<"thread_id_is:"<<thread_id<<endl;
request request_msg;
request_msg.sender_id=thread_id;

while(1){
	switch(thread_id){
	case 0://node 0 give request
		{	
			
			node_status[0].status=REQUEST;
			while(1){
				if (node_status[0].status==REQUEST)
				{
					
					//get the timestamp and send request to 5 nodes in its voting set
					
					chrono::monotonic_clock::time_point today0;
					today0 = system_clock::now();
					std::chrono::microseconds t0_diff = today0 - ini_time;
					std::cout << "node_zero_time_in_microseconds" << t0_diff.count() <<endl;
					request_msg.timestamp=t0_diff;
					
					/*
					std::chrono::seconds t0( 100 );
					request_msg.timestamp=t0;
					std::cout <<"node_zero_second"<< t0.count()<< std::endl;
					*/
					no_deadlock=false;

			
					

					thread_flag+=1;
					//while(thread_flag<2);
					//send_request_mtx.lock();
						RPC_send(0,request_msg,REQUEST);
					//send_request_mtx.unlock();	
						//cout<<"node_0_send_finish"<<endl;
						chrono::monotonic_clock::time_point now0;
					while(received_reply[request_msg.sender_id][2]==false||received_reply[request_msg.sender_id][3]==false);
					/*{
						if(flag2!=1){
							now0 = system_clock::now();
							std::chrono::microseconds ti0_diff = now0- today0;
							if (ti0_diff.count()>=ts.count())
							{
								flag2=1;
								break;
							
							}
						}	
					}
					*/

					//usleep(0.00001*TICK_USEC);
					//while(received_reply[request_msg.sender_id][2]==false||received_reply[request_msg.sender_id][3]==false);
					no_deadlock=true;
					array_lock.lock();
					received_reply[request_msg.sender_id][2]==false;
					received_reply[request_msg.sender_id][3]==false;
					array_lock.unlock();
					count1=0;
					count2=0;
					cout<<"node_zero_enter_cs"<<endl;
					node_status[0].status=HELD;
				}
				if (node_status[0].status==HELD)
				{
					usleep(cs_int*TICK_MSEC);
					node_status[0].status=RELEASE;
				}
				if (node_status[0].status==RELEASE)
				{
						cout<<"node_zero_start_releasing"<<endl;
					RPC_send(0,request_msg,RELEASE);
					usleep(cs_int*TICK_MSEC);
					node_status[0].status=REQUEST;
				}
			}
		}
		break;
	case 1://node 1 give request
		{	
			node_status[1].status=REQUEST;
			while(1){
				
				if (node_status[1].status==REQUEST)
				{
					///*
					chrono::monotonic_clock::time_point today1;
					today1 = system_clock::now();	//t1.push_back(request_msg);
					std::chrono::microseconds t1_diff = today1 - ini_time;
					std::cout <<"node_one_time_in_microseconds"<< t1_diff.count()<< std::endl;
					request_msg.timestamp=t1_diff;
					//*/

					/*
					std::chrono::seconds t1( 60 );
					request_msg.timestamp=t1;
					std::cout <<"node_one_second"<< t1.count()<< std::endl;
					*/

					no_deadlock=false;




					//current_request=request_msg;
				thread_flag+=1;
				//while(thread_flag<2);
					//send_request_mtx.lock();
					RPC_send(1,request_msg,REQUEST);
					//send_request_mtx.lock();
					//cout<<"node_1_send_finish"<<endl;
					chrono::monotonic_clock::time_point now1;
					while(received_reply[request_msg.sender_id][2]==false||received_reply[request_msg.sender_id][3]==false);
					/*{
						if(flag2!=1){
							now1 = system_clock::now();
							std::chrono::microseconds ti1_diff = now1 - today1;
							if (ti1_diff.count()>=ts.count())
							{
								flag1=1;
								break;
							}
						}						
					}
					*/
					//usleep(0.00001*TICK_USEC);
					//while(received_reply[request_msg.sender_id][2]==false||received_reply[request_msg.sender_id][3]==false);
					no_deadlock=true;
					array_lock.lock();
					received_reply[request_msg.sender_id][2]==false;
					received_reply[request_msg.sender_id][3]==false;
					array_lock.unlock();
					count1=0;
					count2=0;
					cout<<"node_one_enter_cs"<<endl;
					node_status[1].status=HELD;
				}
			
				if (node_status[1].status==HELD)
				{
					usleep(cs_int*TICK_MSEC);
					node_status[1].status=RELEASE;
				}
				if (node_status[1].status==RELEASE)
				{
						cout<<"node_one_start_releasing"<<endl;
					RPC_send(1,request_msg,RELEASE);
					usleep(cs_int*TICK_MSEC);
					node_status[1].status=REQUEST;
				}


			}


		}
		break;
	case 2://receive thread for node 0
		{				
				/*
				chrono::monotonic_clock::time_point today2;
				today2 = system_clock::now();	//t1.push_back(request_msg);
				std::chrono::microseconds t2_diff = today2 - ini_time;
				std::cout << t2_diff.count() << "difference of time fo thread 3 in microseconds" << std::endl;
				*/
			thread_flag+=1;
			request old_top;
			//request t0_min;
			//while(thread_flag<4);
				while(1){
					//while(t0.size()<1);
					//while(notified[thread_id]==false);
					//once node 0 receive a request
					//	notified[thread_id]=false;//clear the flag to wait for the next request
							
				/*		if (notified[thread_id]==false)
									{
										cout<<"notified_canceled"<<endl;
									}
									else
										cout<<"notified_not_canceled"<<endl;
				*/

						if (node_status[0].status==HELD||node_status[1].status==HELD)//if already in cs
						{
							//enqueue and do not send reply
							//mtx.lock();
							//t0.push(current_request);
							//mtx.unlock();
						}
						else if (node_status[0].voted==true)//if voted
						{

							/*	if (node2_counter<1)
								{
									cout<<"curr_top "<<t0.top().sender_id<<endl;

									if (no_deadlock)
									{
										cout<<"no_deadlock"<<endl;
									}
									else
										cout<<"deadlock"<<endl;
									
									node2_counter++;
								}
							*/
							
									//cout<<"node_two_test_dead "<<endl;

							
									
							if (no_deadlock==false)
							{

								//if (count1<1)
								//{
								//	count1++;
									//cout<<"queue0_size_before_repush"<<t0.size()<<endl;
									if(t0.size()>1)
									{

									
									//cout<<"testing2"<<endl;
										mtx.lock();
										t0_min=t0.top();
										t0.pop();
											//mtx.unlock();

										if (!(t0_min<t0.top()))
										{
											
											
										//	cout<<"node_two_real_reply"<<t0_min.sender_id<<endl;
										//		cout<<"node_two_false_reply"<<t0.top().sender_id<<endl;
											
											//}
											
											//if (t0.size()>=1)
												
											
											old_top=t0.top();
											//mtx.lock();
											t0.push(t0_min);
											//mtx.unlock();
										//	cout<<"queue0_size_after_repush"<<t0.size()<<endl;

											array_lock.lock();
											received_reply[old_top.sender_id][request_msg.sender_id]=false;
											received_reply[t0.top().sender_id][request_msg.sender_id]=true;
											array_lock.unlock();
											mtx.unlock();
										}
										else{
												
											//mtx.lock();
											t0.push(t0_min);
											mtx.unlock();
										}
									
									//}
								}
							}	
							//if (!(current_request<t0.top())&&no_deadlock==false)
							//{
								//?send fail to original node
								//send reply to the new node
							//	cout<<"node_two_real_reply "<<current_request.sender_id<<endl;
							//	received_reply[t0.top().sender_id][request_msg.sender_id]=false;
							//	received_reply[current_request.sender_id][request_msg.sender_id]=true;
							//}
							
						//	cout<<"queue2_size"<<t0.size()<<endl;
						
						}
						else//send reply according to the smallest timestamps in the queue
						{
							//enqueue
							
							//?send reply
							if(t0.size()>=1){
								cout<<"node_two_reply"<<t0.top().sender_id<<endl;
								array_lock.lock();
								received_reply[t0.top().sender_id][request_msg.sender_id]=true;
								array_lock.unlock();

							//mark it as voted
								node_status[0].voted=true;
							}
						}
							
					//notified[thread_id]=false;//clear the flag to wait for the next request	
						//mtx.lock();
					//		flag+=1;
						//mtx.lock();
							//cout<<"flag:"<<flag<<endl;
					}
					
		}
		
		break;
	case 3://receive thread for node 1
		{	
			/*
				chrono::monotonic_clock::time_point today3;
				today3 = system_clock::now();	//t1.push_back(request_msg);
				std::chrono::microseconds t3_diff = today3 - ini_time;
				std::cout << t3_diff.count() << "difference of time fo thread 4 in microseconds" << std::endl;
				*/
			thread_flag+=1;
			request old_top2;
			//request t1_min;
			//while(thread_flag<4);
			while(1){
				//	while(notified[thread_id]==false);   process_receiving_message_lock.lock();
				//	notified[thread_id]=false;//clear the flag to wait for the next request
					//proceed once node 1 receive a request

					//	if (notified[thread_id]==false)
					//				{
					//					cout<<"notified_canceled"<<endl;
					//				}
					//				else
					//					cout<<"notified_not_canceled"<<endl;
					//while(t1.size()<1);
						if (node_status[1].status==HELD||node_status[0].status==HELD)//if already in cs
						{
							//enqueue and do not send reply
							//mtx.lock();
							//t1.push(current_request);
							//mtx.unlock();
						}
						else if (node_status[1].voted==true)//if voted
						{
							/*if (node3_counter<1)
								{
									cout<<"curr_top "<<t1.top().sender_id<<endl;

									if (no_deadlock)
									{
										cout<<"no_deadlock"<<endl;
									}
									else
										cout<<"deadlock"<<endl;
									node3_counter++;
								}
							*/
							

									//cout<<"node_three_test_dead "<<endl;
									
								//if (!(t1_min<t1.top())&&no_deadlock==false)
								if (no_deadlock==false)
								{
								//?send fail to original node
								//send reply to the new node
									//if (count2<1)
									//{

							
									//	count2++;
									//cout<<"queue1_size_before_repush"<<t1.size()<<endl;
									if(t1.size()>1)
									{
										//cout<<"testing3 "<<endl;
										mtx.lock();
										t1_min=t1.top();
										t1.pop();
										//mtx.unlock();
										if (!(t1_min<t1.top()))
										{
												
										//	cout<<"node_three_real_reply"<<t1_min.sender_id<<endl;
										//	cout<<"node_three_false_reply"<<t1.top().sender_id<<endl;
											

											old_top2=t1.top();
											//mtx.lock();
											t1.push(t1_min);
										//	mtx.unlock();
										//	cout<<"queue1_size_after_repush"<<t1.size()<<endl;
											array_lock.lock();
											received_reply[old_top2.sender_id][request_msg.sender_id]=false;
											received_reply[t1.top().sender_id][request_msg.sender_id]=true;	
											array_lock.unlock();
											mtx.unlock();
											//}
										}
										else
										{
											//mtx.lock();
											t1.push(t1_min);
											mtx.unlock();
										}
									}
									//}
								}
							//enqueue and do not send reply
							//mtx.lock();
							//t1.push(current_request);
							//mtx.unlock();
							//cout<<"queue3_size"<<t1.size()<<endl;
						}
						else//send reply according to the smallest timestamps in the queue
						{
							//enqueue and do not send reply
							//mtx.lock();
							//t1.push(current_request);
							//mtx.unlock();

							//?send reply
							//request_msg.sender_id
							if(t1.size()>=1){
							cout<<"node_three_reply"<<t1.top().sender_id<<endl;
							array_lock.lock();
							received_reply[t1.top().sender_id][request_msg.sender_id]=true;
							array_lock.unlock();
							//mark it as voted
							node_status[1].voted=true;
							}
						}
						//notified[thread_id]==false;//clear the flag to wait for the next request
						//mtx.lock();
						//notified[thread_id]=false;//clear the flag to wait for the next request
						//flag+=1;
						//mtx.lock();
						//cout<<"flag:"<<flag<<endl;
					}
					
		}
		
		

	}
	


/*	
	cout<<receive_request[1][0].timestamp<<endl;
	cout<<receive_request[1][1].timestamp<<endl;

*/
}
	
	//usleep(cs_int*TICK_USEC);
//	cout<<"threads "<<thread_id<<" ends"<<endl;
	
}


//pseudo RPC implementation
//signal a node with a request type
void RPC_send(int local_notified_target,request request_msg,state type)
{
	
	//unique_lock<mutex> locker(nodes_lock[node_id]);
	//send_request_mtx.lock();
		//cout<<current_request.sender_id<<"enter_rpc"<<endl;

	

while(thread_flag<4);
	//current_request=request_msg;
if (local_notified_target==0&&type==REQUEST)
{
	//t01_lock.lock();
	mtx.lock();
	//t0_min=request_msg;
	t0.push(request_msg);
	//counter++;
	//cout<<"push times "<<counter<<endl;
	mtx.unlock();
usleep(0.01*TICK_USEC);
	mtx.lock();
	//t1_min=request_msg;
	t1.push(request_msg);
	//counter++;
	//cout<<"push times "<<counter<<endl;
	mtx.unlock();
}
if (local_notified_target==1&&type==REQUEST)
{

		mtx.lock();
	//	t0_min=request_msg;
	t0.push(request_msg);
	//counter++;
	//cout<<"push times "<<counter<<endl;
	mtx.unlock();

	mtx.lock();
	//t1_min=request_msg;
	t1.push(request_msg);
	//counter++;
	//cout<<"push times "<<counter<<endl;
	mtx.unlock();

	usleep(0.01*TICK_USEC);
//	usleep(0.1*TICK_USEC);
}

if (local_notified_target==0&&type==RELEASE)
{
//	cout<<"node_zero_enter_rpc_releasing"<<endl;
/*	
	mtx.lock();
	t0.pop();
	cout<<"queue0_size"<<t0.size()<<endl;
	mtx.unlock();

	mtx.lock();
	t1.pop();
	mtx.unlock();
	*/
	mtx.lock();	
	//cout<<"queue0_size"<<t0.size()<<endl;
	if (t0.size()>=1)
	{
		t0.pop();
	}
	//cout<<"queue0_size"<<t0.size()<<endl;
	if (t0.size()==0)
	{
		node_status[0].voted=false;
	}
	//cout<<"queue1_size"<<t1.size()<<endl;
	if (t1.size()>=1)
	{
		t1.pop();
	}
	//cout<<"queue1_size"<<t1.size()<<endl;
	if (t1.size()==0)
	{
		node_status[1].voted=false;
	}


	mtx.unlock();

}
if (local_notified_target==1&&type==RELEASE)
{
	//cout<<"node_one_enter_rpc_releasing"<<endl;

	mtx.lock();


	//cout<<"queue0_size"<<t0.size()<<endl;
	if (t0.size()>=1)
	{
		t0.pop();
	}
	//cout<<"queue0_size"<<t0.size()<<endl;
	if (t0.size()==0)
	{
		node_status[0].voted=false;
	}
	


	//cout<<"queue1_size"<<t1.size()<<endl;
	if (t1.size()>=1)
	{
		t1.pop();
	}
	//mtx.unlock();

	//cout<<"queue1_size"<<t1.size()<<endl;
	if (t1.size()==0)
	{
		node_status[1].voted=false;
	}

	//counter++;
	//cout<<"push times "<<counter<<endl;
	mtx.unlock();


}

	/*
	while(flag<2);
	flag=0;
	*/
/*	if (local_notified_target==0)
	{
		
		cout<<"zero_finished"<<endl;
	}
	else
		cout<<"one_finished"<<endl;	
*/

/*
			//cout<<current_request.sender_id<<"leave_rpc"<<endl;
	//while(notified[local_notified_target[0]] == true ||notified[local_notified_target[1]] == true);
		for (int i = 0; i < 2; ++i)
		{			
			notified[local_notified_target[i]] = true;
		}
*/


	//while(flag1==false&&flag2==false);
		//nodes_check[node_id].notify_one();
	//send_request_mtx.lock();	
}






int main(){

		//read input and handle
		cout<<"please input message to send: "<<endl;
		string mymsg;
        getline(cin, mymsg);
        char *c_mymsg_str = new char[mymsg.length()+1];
        strcpy(c_mymsg_str, mymsg.c_str());        
        //parsing
        splitstring search_msg(c_mymsg_str);
        vector<string> search_chunk = search_msg.split(' ');
  		//convert to interger numbers
    	cs_int = stoi(search_chunk[0]);
    	next_req = stoi(search_chunk[1]);
    	tot_exec_time = stoi(search_chunk[2]);  
        delete [] c_mymsg_str;



        //initialize all node status to be init state and voted to be false
        for (int i = 0; i < 2; ++i)
        {
        	node_info node(INIT,false);
        	node_status.push_back(node);
        }
        
        //initialize target
    	notified_target[0][0]=2;
    	notified_target[0][1]=3;
    	notified_target[1][0]=2;
    	notified_target[1][1]=3;
        

		ini_time= system_clock::now();//set benchmark time

		//create 2 threads for sending requests 2 threads for receiveing requests
      	for (int thread_id = 0; thread_id <4; ++thread_id)
      	{
      		node_threads.push_back(thread(node_func, thread_id));
      	}
      	


//while(!(flag1==true&&flag2==true));//wait until 2 thread update their time


	//cout<<"central server 1 "<<t0.front().timestamp.count()<<endl;
	//cout<<"central server 2 "<<t0.back().timestamp.count()<<endl;
	/*
	now1 = system_clock::to_time_t ( today1 );
	std::cout << "today1 is: " << ctime(&now1);
	now2 = system_clock::to_time_t ( today2 );
	std::cout << "today2 is: " << ctime(&now2);
	*/


	
    usleep(tot_exec_time*TICK_USEC);
	cout<<"program ends"<<endl;
	//should never reach this point...

	return 0;
}
