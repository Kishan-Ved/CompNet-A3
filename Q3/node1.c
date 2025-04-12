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
} dt1;


/* students to write the following two routines, and maybe some others */

int min_cost_1[4] = { 1, 0, 1, 999 };
int neighbors_1[2] = { 0, 2 };

int min_1(int a[4]) {
  int min = a[0];
  for (int i = 1; i < 4; i++) {
      if (a[i] < min) {
          min = a[i];
      }
  }
  return min;
}

void rtinit1()
{
  int i, j;
  for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
          if (i == j){
              dt1.costs[i][j] = min_cost_1[i];
          }
          else{
              dt1.costs[i][j] = 999;
          }
      }
  }
    
  for (j = 0; j < 2; j++) {
    struct rtpkt pkt;
    pkt.sourceid = 1;
    pkt.destid = neighbors_1[j];
    
    for (i = 0; i < 4; i++) {
        pkt.mincost[i] = min_1(dt1.costs[i]);
    }
    
    tolayer2(pkt);
  }
}


void rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;  
{
  int i;
  int source = rcvdpkt->sourceid;
  
  for (i = 0; i < 4; i++) {
      if (rcvdpkt->mincost[i] != 999){
          dt1.costs[i][source] = rcvdpkt->mincost[i] + dt1.costs[source][source];
      }
  }

  int updated = 0;
  for (i = 0; i < 4; i++) {
      int new_min_cost_1 = min_1(dt1.costs[i]);
      if (new_min_cost_1 != min_cost_1[i]) {
          min_cost_1[i] = new_min_cost_1;
          updated = 1;
      }
  }

  if (updated) {
      for (i = 0; i < 2; i++) {
          struct rtpkt pkt;
          pkt.sourceid = 1;
          pkt.destid = neighbors_1[i];
          for (int j = 0; j < 4; j++) {
              pkt.mincost[j] = min_cost_1[j];
          }
          
          tolayer2(pkt);
      }
  }
  
  printdt1(&dt1);
}


void printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

void linkhandler1(linkid, newcost)   
  int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
  int i, j;
  int old_cost = dt1.costs[linkid][linkid];

  for (i = 0; i < 4; i++) {
    dt1.costs[i][linkid] = dt1.costs[i][linkid] - old_cost + newcost;
    min_cost_1[i] = min_1(dt1.costs[i]);
  }

  for (j = 0; j < 2; j++) {
      struct rtpkt pkt;
      pkt.sourceid = 1;
      pkt.destid = neighbors_1[j];
      
      for (i = 0; i < 4; i++) {
          pkt.mincost[i] = min_cost_1[i];
      }
      
      tolayer2(pkt);
  }
}

