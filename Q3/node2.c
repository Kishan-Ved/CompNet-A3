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
} dt2;


/* students to write the following two routines, and maybe some others */

int min_cost_2[4] = { 3, 1, 0, 2 };
int neighbors_2[3] = { 0, 1, 3 };

int min_2(int a[4]) {
    int min = a[0];
    for (int i = 1; i < 4; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

void rtinit2() 
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j){
                dt2.costs[i][j] = min_cost_2[i];
            }
            else{
                dt2.costs[i][j] = 999;
            }
        }
    }

    for (j = 0; j < 3; j++) {
        struct rtpkt pkt;
        pkt.sourceid = 2;
        pkt.destid = neighbors_2[j];
        
        for (i = 0; i < 4; i++) {
            pkt.mincost[i] = min_2(dt2.costs[i]);
        }
        
        tolayer2(pkt);
    }
}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  int i;
  int source = rcvdpkt->sourceid;

  for (i = 0; i < 4; i++) {
      if (rcvdpkt->mincost[i] != 999){
          dt2.costs[i][source] = rcvdpkt->mincost[i] + dt2.costs[source][source];
      }
  }

  int updated = 0;
  for (i = 0; i < 4; i++) {
      int new_min_cost_2 = min_2(dt2.costs[i]);
      if (new_min_cost_2 != min_cost_2[i]) {
          min_cost_2[i] = new_min_cost_2;
          updated = 1;
      }
  }

  if (updated) {
      for (i = 0; i < 3; i++) {
          struct rtpkt pkt;
          pkt.sourceid = 2;
          pkt.destid = neighbors_2[i];
          for (int j = 0; j < 4; j++) {
              pkt.mincost[j] = min_cost_2[j];
          }
          
          tolayer2(pkt);
      }
  }
  printdt2(&dt2);
}


void printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}






