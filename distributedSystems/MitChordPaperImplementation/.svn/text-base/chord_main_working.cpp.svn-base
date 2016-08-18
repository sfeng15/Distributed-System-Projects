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

using namespace std;

//finger structure
typedef struct fingers{
	int start;
	int node;
}finger_t;


//chordNode class
class chordNode{
	//public variable
public:
	chordNode(int id){
		myID = id;
		for(int i=0;i<8;i++){ 
			myfinger[i].node=id;
			myfinger[i].start = ( id+(int)pow(2,i) ) % 256;
		}

		successor = id;
		predecessor = id;
	}
	
	finger_t myfinger[8];	//finger array for 8-bit chord ring
	int myID;
	int successor;
	int predecessor;
	vector<int> mykeys;
};
//msg count
int numMsg;

//msg passing system
int request_type;
/*
type 1: find_predecccessor
type 2: return its predecessor
type 3: modify its predecessor
type 4: ask a node to modify its finger table
type 5: return its closest preceding finger
type 6: return its successor
type 7: show local keys
*/

mutex nodes_lock[256];		//mutex for "sending" a request
mutex nodes_lock_R[256];		//mutex for request is done

condition_variable nodes_check[256]; //cond var for "sending" a request
condition_variable inbox;			//cond var for request is done
bool notified[256];
bool request_done;
int updating;
bool done;							//a series of request is done
volatile bool request_flag[256];				

int MSG;	//
int PARA;	//passed in parameter for RPC
int INDEX;

vector<int> exist_node;		//record all existing node, only viewable by the coordinator
vector<thread> node_threads;

void cord_func();			//coordinator thread working func
void node_func(int new_id);			//every node thread working func, except node 0?

void init_glob();
bool exist(int id);

bool exist(int id){
	for (int i: exist_node)
		if(i == id) return true;
	return false;
}

//for node structure
//public method that can be called from each node
//int find_successor(int id);		//id can also be key to be looked up
int find_predecessor(int id, chordNode &thisNode);
int closest_preceding_finger(chordNode &thisNode, int id);
void init_finger_table(chordNode &thisNode);
void update_others(chordNode &thisNode);
void update_finger_table(chordNode &thisNode, int s, int i);
int withinrange(int id, int A, int B, int compare_case);
//utility
void showfingers(chordNode &thisNode);
void update_keys(chordNode &thisNode);

void showfingers(chordNode &thisNode){
	cout<<"showing my fingers: ----------- @ "<<thisNode.myID<<endl;
	for(int i=0;i<8;i++){
		cout<<i<<": "<<thisNode.myfinger[i].node<<endl;
	}
	cout<<"--------------------"<<endl;
}

void update_keys(chordNode &thisNode){
	int temp_predecessor = thisNode.predecessor;
	int limit;
	thisNode.mykeys.clear();
	if (thisNode.myID <= temp_predecessor) limit = 256;
	else limit = thisNode.myID+1;
	for(int i = temp_predecessor+1; i<limit; i++){
		thisNode.mykeys.push_back(i);
	}
}


//RPC system
void RPC_send(int node_id, int Q_type, int temp_para, int temp_index);
void RPC_recv(int node_id);

int main(){
	init_glob();
	//hard-coded node 0, who never leaves this chord implementation
	thread Node_zero_thread(node_func,0);
	

	thread cord_thread(cord_func);	//start listening in coordinator
	while(1);
	//should never reach this point...
	return 0;
}

//put all entry in mailbox to 0
void init_glob(){
	for(int i=0; i< 256; i++){
		notified[i] = false;
		request_flag[i] = false;
	}	
	numMsg =0;
	request_type= 0;	// no request yet
	MSG = -1;			// no received yet 
	PARA = -1;			// no parameter yet
	INDEX = -1;
	request_done = false;
	done = false;
	updating = -1;
}

//coordinator that parse commands
//and pass command to corresponding thread
void cord_func(){
	//this threads is always listening
	string mymsg;

	while(1){
		cout<<"please input message to send: "<<endl;

        getline(cin, mymsg);
        cout<<"mymsg is "<<mymsg<<endl;

        char *c_mymsg_str = new char[mymsg.length()+1];
        strcpy(c_mymsg_str, mymsg.c_str());
        
        //parsing
        splitstring search_msg(c_mymsg_str);
        vector<string> search_chunk = search_msg.split(' ');

        char *control = new char[search_chunk[0].length()+1];
        strcpy(control, search_chunk[0].c_str());

        if(strcmp(control, "join") != 0 && strcmp(control, "find") != 0 && strcmp(control, "leave") != 0
          && strcmp(control, "show") != 0 )
        {
          cout<<"unrecognized control word! input again!"<<endl;
          delete [] c_mymsg_str;
          delete [] control;
          continue;
        }
        //join p, always from node 0
        if(strcmp(control, "join") == 0){
        	done = false;
        	int new_id = stoi(search_chunk[1]);
        	//cout <<"joining id: "<<new_id<<endl;
        	node_threads.push_back(thread(node_func, new_id));
        	

        	//clean up
        	delete [] c_mymsg_str;
            delete [] control;
            continue;
        }
        //find p k, node key
        if(strcmp(control, "find") == 0)
        {
        	int temp_key_2, temp_id_2;
            temp_key_2 = stoi(search_chunk[2]);
            temp_id_2 = stoi(search_chunk[1]);
            cout << temp_id_2<<", "<<temp_key_2<<endl;
            if( exist(temp_id_2) ){
            	RPC_send(temp_id_2,1,temp_key_2,-1);		//ask node p find temp_key predecessor, type 1 request
				RPC_recv(temp_id_2);
				int temp_predecessor_2 = MSG;
				cout << temp_id_2<<", "<<temp_key_2<<endl;
				cout<<"found pred: "<<MSG<<endl;
				RPC_send(temp_predecessor_2,6,-1,-1);		//ask returned node return its successor, type 6 request
				RPC_recv(temp_predecessor_2);
				cout<<"node join used "<<numMsg<< " message passing! "<<endl;
				int found_2 = MSG;
				cout <<"key: "<<temp_key_2<<" found at @"<<found_2<<", cheers!!!"<<endl;
			}            
            else
			cout <<"this Node: "<<temp_id_2<<" has not joined yet! QAQ..." <<endl;
            //cout<< "using "<<numMsg<<" messages to retrieve key "<<temp_key<<" from "<<temp_id<<" by "<<numMsg<<" messages"<<endl;
            
            delete [] c_mymsg_str;
            delete [] control;
            continue;
        }
        //show p
        //show all
        if(strcmp(control, "show") == 0)
        {
            char *arg = new char[search_chunk[1].length()+1];
        	strcpy(arg, search_chunk[1].c_str());
            if(strcmp(arg, "all") == 0){
            	sort(exist_node.begin(), exist_node.end());
            	for (int i: exist_node){
					RPC_send(i,7,-1,-1);		//ask a node show its keys, request 7
					RPC_recv(i);
				}	
            }			//show all
            else{
				int temp_id = stoi(search_chunk[1]);
				if( exist(temp_id) ){
				RPC_send(temp_id,7,-1,-1);		//ask a node show its keys, request 7
				RPC_recv(temp_id);
				}
				else
				cout <<"this Node: "<<temp_id<<" has not joined yet! QAQ..." <<endl;
            }			//show p

            delete [] arg;
            delete [] c_mymsg_str;
            delete [] control;
            continue;
        }

        if(strcmp(control, "leave") == 0)
        {

            
            delete [] c_mymsg_str;
            delete [] control;
            continue;
        }

	}//while loop ends
}

void node_func(int new_id){
	cout<<"creating node thread: "<<new_id<<endl;
	chordNode thisNode(new_id);
	exist_node.push_back(new_id);
	////cout<<"aaa"<<endl;
	//node 0 never leaves
	if(new_id == 0){
		bool show_flag = true;
		for (int i = 0; i < 256; ++i)
			thisNode.mykeys.push_back(i);	//keys 0 to 255 initially at node 0
		while(1){
			//condition variable, wait for signal
			unique_lock<mutex> locker(nodes_lock[0]);
			//spurious notifier protection
			//cout<< "node 0 ready"<<endl;

			while(!notified[0]){
				nodes_check[0].wait(locker);
			}
			//cout<< "request to node 0"<<endl;
			//cout<<request_type<<endl;
			switch (request_type){
				case 1: //global type 1: find_predecessor
					{
					//cout <<"PARA: "<<PARA<<endl;
					//cout<<thisNode.myID<<endl;
					int temp_predecessor = find_predecessor(PARA, thisNode);					
					MSG = temp_predecessor;
					cout << "MSG: " <<MSG<<endl; 
					break;
					}
				case 2:		//return its predecessor
					{
					MSG = thisNode.predecessor;
					//cout << "MSG: " <<MSG<<endl;
					break;
					}
				case 3: 	//modify its predecessor
					{
					thisNode.predecessor = PARA;
					//cout << "PARA: "<<PARA<<endl;						
					break;
					}
				case 4:		//modify its finger table
					{	
					update_finger_table(thisNode,PARA,INDEX);
					//cout << "PARA: "<<PARA<<endl;
					//cout << "INDEX: "<<INDEX<<endl;		
					break;
					}
				case 5:		//return its closest preceding finger
					{
					MSG = closest_preceding_finger(thisNode,PARA);
					//cout << "MSG: " <<MSG<<endl;						
					break;
					}	
				case 6:	//return its successor, global request type 6
					{
					MSG = thisNode.successor;
					cout << "MSG: " <<MSG<<endl;
					break;
					}
				case 7:	//show keys, request type 7
					{
						cout<<"This is node: @@"<<thisNode.myID<<" QAQ..."<<endl;
						for (int i: thisNode.mykeys)
							cout<<i<<", ";
						cout<<endl;
						break;
					}
				case 8:	//update its keys, global request type 8
					{
					update_keys(thisNode);
					//cout << "MSG: " <<MSG<<endl;
					break;
					}
				default:
					cout<<"service worng!"<<endl;
			}
			//do sth at node 0
			//numMsg ++;
			while(!request_flag[new_id]);
			notified[0]=false;		//clear notified
			//signal that request is done
			unique_lock<mutex> locker_R(nodes_lock_R[new_id]);	
			request_done = true;
			request_flag[new_id] = false;
			inbox.notify_one();
		}
	}
	//node 1-255 enters here
	else{
	//cout<<"node: "<<new_id<<" starts to join"<<endl;
	updating = new_id;	
	init_finger_table(thisNode);
	update_others(thisNode);
	cout<<"node join used "<<numMsg<< " message passing! "<<endl;
	for (int i : exist_node){
		//update local
		if(i == thisNode.myID){
			update_keys(thisNode);
		}
		//update others
		else{
			RPC_send(i,8,-1,-1);		//ask a node update its keys, request 8
			RPC_recv(i);
		}
	}	
	//showfingers(thisNode);
	numMsg =0;
	updating = -1;
		//wait for doing service
		while(1){
			//condition variable, wait for signal
			unique_lock<mutex> locker(nodes_lock[new_id]);
			while(!notified[new_id]){
				nodes_check[new_id].wait(locker);
			}
			//cout<< "request to node: "<<new_id<<endl;
			//do sth at node[id]
			switch (request_type){
				case 1: //global type 1: find_predecessor
					{
					int temp_predecessor = find_predecessor(PARA, thisNode);					
					MSG = temp_predecessor; 
					cout <<"here:::" <<"MSG: " <<MSG<<endl;
					break;
					}
				case 2:		//return its predecessor
					{
					MSG = thisNode.predecessor;
					//cout << "MSG: " <<MSG<<endl;
					break;
					}
				case 3: 	//modify its predecessor
					{
					thisNode.predecessor = PARA;
					//cout << "PARA: "<<PARA<<endl;					
					break;
					}
				case 4:		//modify its finger table
					{
					update_finger_table(thisNode,PARA,INDEX);
					//cout << "PARA: "<<PARA<<endl;	
					//cout << "INDEX: "<<INDEX<<endl;	
					break;
					}
				case 5:		//return its closest preceding finger
					{
					MSG = closest_preceding_finger(thisNode,PARA);
					//cout << "MSG: " <<MSG<<endl;					
					break;
					}	
				case 6:	//return its successor, global request type 6
					{
					MSG = thisNode.successor;
					cout <<thisNode.myID <<"MSG: " <<MSG<<endl;
					break;
					}
				case 7:	//show keys, request type 7
					{
						cout<<"This is node: @@"<<thisNode.myID<<" QAQ..."<<endl;
						for (int i: thisNode.mykeys)
							cout<<i<<", ";
						cout<<endl;
						break;
					}
				case 8:	//update its keys, global request type 8
					{
					update_keys(thisNode);
					//cout << "MSG: " <<MSG<<endl;
					break;
					}
				default:
					cout<<"service worng!"<<endl;
			}
			//do sth at node 0
			//numMsg ++;
			while(!request_flag[new_id]);
			notified[new_id]=false;		//clear notified
			//signal that request is done
			unique_lock<mutex> locker_R(nodes_lock_R[new_id]);	
			request_done = true;
			request_flag[new_id] = false;
			inbox.notify_one();
		}
	}
}


//pseudo RPC implementation
//signal a node with a request type
void RPC_send(int node_id, int Q_type, int temp_para, int temp_index)
	{
		cout <<"aaa: "<<node_id<<endl;
		cout <<"bbb: "<<Q_type<<endl;
		INDEX = temp_index;
		PARA = temp_para;
		request_type = Q_type;		//global type 1: find_succcessor
		unique_lock<mutex> locker(nodes_lock[node_id]);	
		notified[node_id] = true;
		nodes_check[node_id].notify_one();
	}

void RPC_recv(int node_id)
	{
			unique_lock<mutex> locker(nodes_lock_R[node_id]);
			request_flag[node_id] = true;
			//spurious notifier protection
			while(!request_done){
				inbox.wait(locker);
			}
			numMsg ++;
			request_done = false;
	}

//class method implementation
int find_predecessor(int id, chordNode &thisNode){
	//thisNode is always node_zero when called by join
	int temp = thisNode.myID;

	//RPC_send(temp,6,-1,-1);		//ask a node return its successor, global request type 6
	//RPC_recv();
	int temp_succesor = thisNode.successor;
	cout <<thisNode.myID<<": "<<temp<<", " <<temp_succesor<<endl;
	while( !withinrange(id, temp, temp_succesor,1) ){
		temp = closest_preceding_finger(thisNode,id);	//should be RPC called from node temp
		if(temp == thisNode.myID){
			cout<<"here!"<<endl;
			temp = closest_preceding_finger(thisNode, id);
			if(temp == thisNode.myID){
				temp_succesor = thisNode.successor;
			}
			else{
				RPC_send(temp,6,-1,-1);		//ask a node return its successor, global request type 6
				RPC_recv(temp);
				temp_succesor = MSG;
				
			}
			continue;
		}	//first round, asking for node 0 closest preceding finger
		RPC_send(temp,5,id,-1);		//ask a node return its closest preceding finger, global request type 5
		RPC_recv(temp);
		temp = MSG;
		cout << "temp: "<<MSG<<endl;
		

		RPC_send(temp,6,-1,-1);		//ask a node return its successor, global request type 6
		RPC_recv(temp);
		temp_succesor = MSG;
		cout << "temp successor: "<<MSG<<endl;
	}
	////cout<<"found 3: "<<temp<<endl;
	return temp;
}

int closest_preceding_finger(chordNode &thisNode, int id){
	for (int i = 7; i >= 0; i--){
		////cout<<"check 1: "<<thisNode.myfinger[i].node<<endl;
		////cout<<"check 2: "<<id<<endl;
		//myfinger[i].node initialized at node joining
		if( withinrange(thisNode.myfinger[i].node, thisNode.myID, id, 2) ){
			cout<<"found 1: "<<thisNode.myfinger[i].node<<endl;
			return thisNode.myfinger[i].node;
		}
	}
	cout<<"found 2: "<<thisNode.myID<<endl;
	return thisNode.myID;
}


void init_finger_table(chordNode &thisNode){
	//notify node 0, who is the introducer
	//request type 1, find_predecessor
	//myfinger[0].node = introducer.find_successor(myfinger[0].start);
	//predecessor = succesor.predecessor;
	////cout << "finger[0] start: " <<thisNode.myfinger[0].start<<endl;
	RPC_send(0,1,thisNode.myfinger[0].start,-1);
	RPC_recv(0);	
	//collect inbox msg and clean it up
	int temp_predecessor = MSG;
	RPC_send(temp_predecessor,6,-1,-1);		//ask a node return its successor, global request type 6
	RPC_recv(temp_predecessor);
	thisNode.myfinger[0].node = MSG;
	

	thisNode.successor = MSG;
	RPC_send(thisNode.successor,2,-1,-1);		//ask successor to return its predecessor, global request type 2
	RPC_recv(thisNode.successor);
	//thisNode.predecessor = succesor.predecessor;		//RPC
	thisNode.predecessor = MSG;
	

	////cout<<"pred and succ updated!"<<thisNode.successor<<", "<< thisNode.predecessor<<endl;
	//succesor.predecessor = n; 	//RPC
	RPC_send(thisNode.successor,3,thisNode.myID,-1);		//ask a node to modify its predecessor, global request type 3
	RPC_recv(thisNode.successor);

	for (int i=0; i < 7; i++){
		if( withinrange(thisNode.myfinger[i+1].start, thisNode.myID, thisNode.myfinger[i].node, 3) ){
			thisNode.myfinger[i+1].node = thisNode.myfinger[i].node;
			//cout<<"updated finger: "<<thisNode.myfinger[i+1].node<<endl;
		}
		else {
		  	//myfinger[i+1].node = introducer.find_successor(myfinger[i+1].start);
		  	//type 1: find id's predecessor
		  	RPC_send(0,1,thisNode.myfinger[i+1].start,-1);
			RPC_recv(0);
			
			temp_predecessor = MSG;
			RPC_send(temp_predecessor,6,-1,-1);		//ask a node return its successor, global request type 6
			RPC_recv(temp_predecessor);
			thisNode.myfinger[i+1].node = MSG;				
		}
	}

}

void update_others(chordNode &thisNode){
	for(int i=0; i<8; i++){
		int tempid = thisNode.myID-pow(2,i);
		if (tempid < 0) tempid = tempid+256;	//loop back for 8 bit key
		//cout<<"tempid: "<<tempid<<endl;
		int p = find_predecessor(tempid, thisNode);
		//p.update_finger_table(n,i);	//RPC
		if(p == updating) return;
		//cout<<"to update entry i: "<<i<<" with: "<<thisNode.myID<<" @"<<p<<endl;
		RPC_send(p,4,thisNode.myID,i);		//ask a node to modify its finger table, global request type 4
		RPC_recv(p);
	}
	
}

void update_finger_table(chordNode &thisNode, int s, int i){
	if ( withinrange(s, thisNode.myID, thisNode.myfinger[i].node, 2) ){
		thisNode.myfinger[i].node = s;
		if(i == 0) thisNode.successor = s;
		//cout<<"@ "<<thisNode.myID<<": updated entry i: "<<i<<" with: "<< s <<endl;
		int p = thisNode.predecessor;
		//cout <<"p is: "<<p<<endl;
		//p.update_finger_table(s,i);	//RPC
		if( p!= thisNode.myID && p!=updating){
		RPC_send(p,4,s,i);		//ask a node to modify its finger table, global request type 4
		RPC_recv(p);
		}
	}

}

//above is all pseudo code

//check whether id is within range of A and B
//0 for false, 1 for true
//case 1: (A, B]
//case 2: (A, B)
//case 3: [A, B)
int withinrange(int id, int A, int B, int compare_case){
	if (B <= A) 
		{
			if (id <= B) id=id+256;
			B = B+256;			
		}
	if(compare_case == 1){
		if(A<id && id<=B) return 1;
		else return 0;
	}
	if(compare_case == 2){
		if(A<id && id<B) return 1;
		else return 0;
	}
	if(compare_case == 3){
		if(A<=id && id<B) return 1;
		else return 0;
	}
}