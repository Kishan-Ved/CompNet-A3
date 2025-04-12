#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt0;


/* students to write the following two routines, and maybe some others */

int min_cost_0[4] = { 0, 1, 3, 7 };
int neighbors_0[3] = { 1, 2, 3 };

int min_0(int a[4]) {
    int min = a[0];
    for (int i = 1; i < 4; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

void rtinit0() 
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j){
                dt0.costs[i][j] = min_cost_0[i];
            }
            else{
                dt0.costs[i][j] = 999;
            }
        }
    }

    for (j = 0; j < 3; j++) {
        struct rtpkt pkt;
        pkt.sourceid = 0;
        pkt.destid = neighbors_0[j];
        
        for (i = 0; i < 4; i++) {
            pkt.mincost[i] = min_0(dt0.costs[i]);
        }
        
        tolayer2(pkt);
    }
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
    int i;
    int source = rcvdpkt->sourceid;
    
    for (i = 0; i < 4; i++) {
        if (rcvdpkt->mincost[i] != 999){
            dt0.costs[i][source] = rcvdpkt->mincost[i] + dt0.costs[source][source];
        }
    }

    int updated = 0;
    for (i = 0; i < 4; i++) {
        int new_min_cost_0 = min_0(dt0.costs[i]);
        if (new_min_cost_0 != min_cost_0[i]) {
            min_cost_0[i] = new_min_cost_0;
            updated = 1;
        }
    }

    if (updated) {
        for (i = 0; i < 3; i++) {
            struct rtpkt pkt;
            pkt.sourceid = 0;
            pkt.destid = neighbors_0[i];
            for (int j = 0; j < 4; j++) {
                pkt.mincost[j] = min_cost_0[j];
            }
            
            tolayer2(pkt);
        }
    }
    
    printdt0(&dt0);
}


void printdt0(dtptr)
  struct distance_table *dtptr;
  
{
    printf("                via     \n");
    printf("   D0 |    1     2    3 \n");
    printf("  ----|-----------------\n");
    printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
       dtptr->costs[1][2],dtptr->costs[1][3]);
    printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
       dtptr->costs[2][2],dtptr->costs[2][3]);
    printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
       dtptr->costs[3][2],dtptr->costs[3][3]);
}

void linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
    int i, j;
    int oldcost = dt0.costs[linkid][linkid];
    for (i = 0; i < 4; i++) {
        dt0.costs[i][linkid] = dt0.costs[i][linkid] - oldcost + newcost;
        min_cost_0[i] = min_0(dt0.costs[i]);
    }
        
    for (j = 0; j < 3; j++) {
        struct rtpkt pkt;
        pkt.sourceid = 0;
        pkt.destid = neighbors_0[j];
        
        for (i = 0; i < 4; i++) {
            pkt.mincost[i] = min_cost_0[i];
        }
        
        tolayer2(pkt);
    }
}
