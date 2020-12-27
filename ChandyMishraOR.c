
#include <stdio.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>

#define MAX_NODES 10
int wait_for_graph[MAX_NODES][MAX_NODES];
int n_nodes;
int waitnum[MAX_NODES];

typedef struct MsgReply {
        int rsender;
        int rrecv;
} Reply;

enum DeadlockState {
    DefaultState = -1,
    CycleNotFound ,
    CycleFound
};

void InputToCreateGraph();
void DisplayGraph();
void DetectKnot(int probe);
int Query(int probe, int sender, Reply *reply);

int GlobalState = DefaultState;

int main() {
    int i_Initiator =-1;
    
    printf("**************Chandy–Misra–Haas Algorithm **************\n");
    
    InputToCreateGraph();
    DisplayGraph();
    
    printf("\nEnter the Initiator node : \t ");
    scanf(" %d", &i_Initiator);
    DetectKnot(i_Initiator);
    
    return 0;
   
}

void InputToCreateGraph() {
    char s_response;
    int row, col;
    
    printf("Enter the number of nodes(Max nodes 10) : ");
    scanf("%d",&n_nodes);
    

    printf("Enter Y if connection exists else N\n");
    for(row=0; row < n_nodes; row++){
        for(col=0; col < n_nodes; col++){
            printf(" Directed link from %d ",row);
            printf("-> %d", col);
            printf("\n");
            scanf(" %c",&s_response);
            if(s_response == 'Y') {
                wait_for_graph[row][col] =1;
            } else {
                wait_for_graph[row][col] = 0;
            }
        }
    }
}

void DisplayGraph(){
    int row, col;
    
    printf("\n \t Matrix represtation of graph \n");
    printf("\t");
    for (row =0; row < n_nodes; row++ ) {
        printf("N%d\t", row);
    }
    
    printf("\n");
    for(row=0; row < n_nodes; row++){
        printf("\nN%d\t", row);
        for(col=0; col < n_nodes; col++){
            printf("%d \t", wait_for_graph[row][col]);
        }
    }   
}


void DetectKnot(int probe){
    int col;
    Reply replyprobe;
    int i_KnotState;
    bool wait = false;
    
    for(col = 0 ; col < n_nodes; col++){
        if(wait_for_graph[probe][col] == 1){
            waitnum[probe]++;
	    wait = true;
            replyprobe.rsender =col;
            replyprobe.rrecv = probe;
            
            Query(probe, col, &replyprobe);
            waitnum[probe]--;
            
        }
    }
    if(GlobalState == CycleFound){
        printf("\n\t KNOT PRESENT IN GRAPH  \n ");
        printf("************** DEADLOCK FOUND ************** \n");
    }else{
	    printf("\n\t KNOT NOT PRESENT IN GRAPH  \n");
            printf("************** DEADLOCK NOT FOUND ************** \n");
    }
}

int Query(int probe, int sender, Reply *reply){
    int col;
    int continueCounter = waitnum[sender];
    Reply localReply;
    int i_state = DefaultState;
    bool wait = false;

    localReply.rsender = -1;
    localReply.rrecv = -1;


    for(col=0; col< n_nodes; col++){
        if(wait_for_graph[sender][col]==1){
            if(continueCounter >0) {
                continueCounter --;
                continue;
            }
	    wait = true;
            waitnum[sender]++;
           
            localReply.rsender = col;
            localReply.rrecv = sender ;
            
            if(Query(probe, col, &localReply) == DefaultState  && waitnum[col] == 0 ){
	        	
                GlobalState = CycleNotFound;
                return CycleNotFound;

            } else{
                
                if(GlobalState != CycleNotFound){
                    GlobalState = CycleFound;
		            i_state = CycleFound;
		            waitnum[sender]--;
                }
            }
       }
    }


    return i_state;
}
