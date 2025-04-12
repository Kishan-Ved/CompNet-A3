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
} dt3;


/* students to write the following two routines, and maybe some others */

int min_cost_3[4] = { 7, 999, 2, 0 };
int neighbors_3[2] = { 0, 2 };

int min_3(int a[4]) {
    int min = a[0];
    for (int i = 1; i < 4; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

void rtinit3() 
{
  int i, j;
  for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
          if (i == j){
              dt3.costs[i][j] = min_cost_3[i];
          }
          else{
              dt3.costs[i][j] = 999;
          }
      }
  }
  
  for (j = 0; j < 2; j++) {
      struct rtpkt pkt;
      pkt.sourceid = 3;
      pkt.destid = neighbors_3[j];
      
      for (i = 0; i < 4; i++) {
          pkt.mincost[i] = min_3(dt3.costs[i]);
      }
      
      tolayer2(pkt);
  }
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  int i;
  int source = rcvdpkt->sourceid;

  for (i = 0; i < 4; i++) {
      if (rcvdpkt->mincost[i] != 999){
          dt3.costs[i][source] = rcvdpkt->mincost[i] + dt3.costs[source][source];
      }
  }

  int updated = 0;
  for (i = 0; i < 4; i++) {
      int new_min_cost = min_3(dt3.costs[i]);
      if (new_min_cost != min_cost_3[i]) {
          min_cost_3[i] = new_min_cost;
          updated = 1;
      }
  }

  if (updated) {
      for (i = 0; i < 2; i++) {
          struct rtpkt pkt;
          pkt.sourceid = 3;
          pkt.destid = neighbors_3[i];
          
          for (int j = 0; j < 4; j++) {
              pkt.mincost[j] = min_cost_3[j];
          }
          
          tolayer2(pkt);
      }
  }
  printdt3(&dt3);
}


void printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
}






