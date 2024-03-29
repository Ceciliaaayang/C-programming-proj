/*student ID:qy5*/
//collaborating student:
//other collaborators:
//other resources:
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int num_jobs;
int num_machines;
int** job;

// J1 and J2
int** jJob1;
int** jJob2;

void checkMin(int** job, int start, int end ,int chosen[end-start],int result[2] ){
  int min;
  int minInd;
  int minMachine;
// For funciton johnson()
// determine the soonest finishing one from current working machines and store result in result[2]
// result = [job index, machine index (0 for B, 1 for C)]
  for (int i = start; i< end; i++){
    if (chosen[i-start] == 0){
      min = job[i][1];
      minInd = i;
      minMachine = 0;
      break;
    }

  }
  for (int i = start; i< end; i++){
    for (int j = 0; j< 2; j++ ){
      if (chosen[i-start]== 0 && job[i][j+1]<= min){
        min = job[i][j+1];
        minInd = i;
        minMachine = j;

      }
    }
  }

  result[0] = minInd;
  result[1] = minMachine;
}

int** johnsonSort(int section){
// sort a scetion by the johnson rule,return the sorted array of jobs. section = 1 or 2
  int start;
  int end;
  int** jJob;
  if (section == 1){
    //section 1
    jJob = (int **) malloc(sizeof(int*)* num_machines);
    for (int i = 0; i< num_machines; i ++){
      jJob[i] = (int*) malloc(4* sizeof(int));

    }
  start = 0;
  end = num_machines;
  if (num_jobs < num_machines)
    end = num_jobs;
  }
  else{
    // section 2
    jJob = (int **) malloc( (num_jobs-num_machines)* sizeof(int*));
    for (int i = 0; i< num_jobs-num_machines; i ++){
      jJob[i] = (int*) malloc(4 * sizeof(int));
    }
  start = num_machines;
  end   = num_jobs;
  }

  int startI = start;
  int endI = end -1;
  int chosen[end - start];
  for (int i = 0; i< end-start; i++){
    chosen[i] = 0;
  }

  int result[2];

  int insertInd;
  while (startI <= endI){
    // keep track of the positions of two cursors starting from two ends
    checkMin(job, start, end,chosen, result);
    chosen[result[0]-start] = 1;
    if (result[1] == 0){
      insertInd = startI;
      startI ++;
    }
    else{
      insertInd = endI;
      endI --;
    }
    for (int i = 0; i< 4; i++){

      jJob[insertInd- start][i] = job[result[0]][i];
    }

  }
  return jJob;
}


int mAvailable(int states[num_machines]){
// determine available machines for the function lpt()
  for (int i = 0; i<num_machines; i++){
    if (states[i] == -1) return i;
  }
  return -1;
}
int* mAvailableJ(int states[2]){
// determine available machines for the function johnson()
  int* result;
  result = (int*) malloc(2* sizeof(int));

  for (int i = 0; i<2; i++){
    if (states[i] == -1) result[i] = -1;
    else{
    result[i] = states[i];
    }
  }
  return result;
}

int comp(const void* a, const void* b){
// the comparison function for the lpt order, which happens be realized by built-in qsort
  int A = **((int**) a);
  int B = **((int**) b);
  return B - A;

}

int getMin(int et[num_machines]){
// get the soonest finishing machine for lpt()
  int min;
  int minInd;
  for (int i = 0; i<num_machines; i++){
    if (et[i] != -1) {
      min = et[i];
      minInd = i;
    }
  }
  for (int i = 0; i< num_machines; i++){
    if (et[i] != -1 && et[i] <= min) {
      min = et[i];
      minInd = i;
    }
  }

  return minInd;
}

int getMax(int et[num_machines]){
// get the last finishing machine for lpt();
  int max;
  int maxInd;
  for (int i = 0; i<num_machines; i++){
    if (et[i] != -1) {
      max = et[i];
      maxInd = i;
    }
  }

  for (int i = 0; i< num_machines; i++){
    if (et[i] >= max) {
      max = et[i];
      maxInd = i;
    }
  }

  return maxInd;
}



int lpt(int j, int t){
// j is 1 or 2 indication J1 or J2
// returns updated time
  int start;
  int end;
  if ( j == 1){
    start = 0;
    end = num_machines;
    if (num_jobs<= num_machines) end = num_jobs;
  }
  else{
    start = num_machines;
    end = num_jobs;
  }
  int order[end-start];
  int jobInfo[end-start][2];
  int* mInfo[num_machines];
  int processedTotal[num_machines];
  int cT = t;
  int cJ = start;
  int machineStates[num_machines];
  int et[num_machines];
  int minMachine;
  for (int i = 0; i< num_machines; i++){
    et[i] = -1;
  }

  for (int i = 0; i< num_machines; i++){
    machineStates[i] = -1;
  }

  for (int i = 0; i< num_machines; i++){
    processedTotal[i] = 0;
    mInfo[i] = (int *) malloc(2*sizeof(int));
  }



  int available;
  while (cJ< end){
    //cJ keeps track of job progress
    available = mAvailable(machineStates);

    if (available != -1){
      // the case where there is available machine
      machineStates[available] = cJ;
      order[cJ-start] = job[cJ][3];
      jobInfo[cJ-start][0] = available;
      jobInfo[cJ-start][1] = cT;
      processedTotal[available] ++;
      mInfo[available] = (int *) realloc(mInfo[available], 2* processedTotal[available]* sizeof(int));

      mInfo[available][2*(processedTotal[available]-1)] = job[cJ][3];
      mInfo[available][2*(processedTotal[available]-1)+1] = cT;

      et[available] = cT + job[cJ][0];
    cJ ++;
    }
    else{
        // when there's no available machine, advancing time to the soonest finishing one
        minMachine = getMin(et);
        machineStates[minMachine] = -1;
        machineStates[minMachine] = -1;
        cT = et[minMachine];
    }
  }
  // finishing the rests by getting the latest finishing one
  cT = et[getMax(et)];



  printf("LPT order: ");
  for (int i = start; i< end; i++){
    printf("%d, ", order[i-start]+1);
  }
  printf("\n");
  printf("\n");

  printf("Job information:\n");
  for (int i= start; i < end; i++){
    printf("Job %d is processed on A_%d starting %d;\n", job[i][3]+1, jobInfo[i-start][0]+1, jobInfo[i-start][1]);
  }
  printf("\n");

  printf("Machine information:\n");
  for (int i = 0; i< num_machines; i ++){
    printf("A_%d processes ", i+1);

    for (int j = 0; j< processedTotal[i]; j++){
      printf("job %d at %d;", mInfo[i][2*j]+1, mInfo[i][2*j+1]);
    }
    if (processedTotal[i] == 0) printf(".");
  printf("\n");
  }
  printf("\n");
  printf("The job processing time interval is [%d, %d], and the makespan is %d\n", t, cT, cT - t);
  printf("\n");
  return cT;
}





int allPass(int size , int processed[size]){
// a helper function to determine where the array processed is all 1
  int result = 1;
  for (int i = 0; i< size; i++){
    result = processed[i]* result;
  }

  return result;
}


int johnson(int j, int t){
// j is either 1 or 2

  int start;
  int end;
  int** jJ;
  if ( j == 1){
    start = 0;
    end = num_machines;
    if (num_jobs< num_machines) end = num_jobs;
    jJ = jJob1;
  }
  else{
    start = 0;
    end = num_jobs- num_machines;
    jJ = jJob2;
  }
  int order[end-start];
  int jobInfo[end-start][2];
  int* mInfo[2];
  int cT = t;
  int cJ = start;
  int machineStates[2];
  int et[2];
  int minMachine;
  int mCounter[2] = {0,0};

  for (int i = 0; i< 2; i++){
    machineStates[i] = -1;
  }

  for (int i = 0; i< 2; i++){
    mInfo[i] = (int *) malloc((end)*2*sizeof(int));
  }



  int* available;
  available = (int*)malloc(2*sizeof(int));
  int bProcessed[end - start];
  int cProcessed[end - start];
  for (int i = 0; i< end- start; i++){
    bProcessed[i] = 0;
    cProcessed[i] = 0;
  }





  int newTaken;
  int minEt;

  while ((allPass(end-start ,bProcessed)==0 || allPass(end-start,cProcessed)==0)){
  // feed jobs util the jobs are all fed to machines
    newTaken = 0;

    available = mAvailableJ(machineStates);

    if (available[0] == -1 || available[1] == -1){
      if (available[0] == -1){
        // if machine B is available
        cJ = -1;
        for (int i = 0; i< end; i++){
          if (bProcessed[i] == 0) {
            cJ = i;
            break;
          }
        }
        if (cJ != -1){
          machineStates[0] = cJ;
          order[cJ] = jJ[cJ][3];
          jobInfo[cJ][0] = cT;
          mInfo[0][mCounter[0]* 2] = cJ;
          mInfo[0][mCounter[0]*2 +1] = cT;
          mCounter[0] ++;
          et[0] = jJ[cJ][1]+ cT;
          newTaken = 1;
        }


    }
    if (available[1] == -1){
        // if machine C is available
        cJ = -1;
        for (int i = 0; i< end; i++){
          if (cProcessed[i] == 0 && bProcessed[i] == 1) {
            cJ = i;
            break;
          }
        }
        if (cJ != -1){
          machineStates[1] = cJ;
          jobInfo[cJ][1] = cT;
          mInfo[1][mCounter[1]* 2] = cJ;
          mInfo[1][mCounter[1]*2 +1] = cT;
          mCounter[1] ++;
          et[1] = jJ[cJ][2]+ cT;
          newTaken = 1;
        }
    }
  }
    if (newTaken != 1){
      // if no new job is fed, determine which one of B and C finishes sooner
      // then advances time
      if (machineStates[0] == -1){
        if (machineStates[1] == -1)printf("not possible\n");
        else {
          minEt = et[1];
          minMachine = 1;
        }
      }
      else{
        if (machineStates[1] == -1){
          minEt = et[0];
          minMachine = 0;
        }
        else{
          minEt = et[0];
          minMachine = 0;
          if (et[1]<= minEt){
            minEt = et[1];
            minMachine = 1;
          }
        }
      }
      if (minMachine == 0) bProcessed[machineStates[0]] = 1;
      else cProcessed[machineStates[1]] = 1;
      machineStates[minMachine] = -1;
      cT = et[minMachine];
    }




  }
  int maxInd = 0;
  if (et[1]>= et[maxInd]){
    maxInd = 1;
  }
  cT = et[maxInd];

  printf("johnson order: ");
  for (int i = start; i< end; i++){
    printf("%d, ", order[i-start]+1);
  }
  printf("\n");
  printf("\n");
  printf("Job information:\n");
  for (int i= start; i < end; i++){
    printf("job %d is processed on B starting %d, on C starting %d\n", jJ[i-start][3]+1, jobInfo[i-start][0], jobInfo[i-start][1]);
  }
  printf("\n");
  printf("Machine information:\n");
  for (int i = 0; i< 2; i ++){
    if (i == 0)
      printf("B processes ");
    else
      printf("C processes ");
    for (int j = 0; j< end; j++){

      printf("job %d at %d; ", jJ[mInfo[i][2*j]][3]+1, mInfo[i][2*j+1]);
    }
  printf("\n");
  printf("\n");
  }
  printf("The job processing time interval is [%d, %d], and the makespan is %d\n", t, cT, cT - t);
  printf("\n");
  return cT;
}







int main(int argc,char* argv[])
{
        int inst_num,row,col,interval1,interval2,i,r;
        if (argc != 2){
                printf("%s -i| -r\n",argv[0]);
                return -10;
        }
        if(strcmp(argv[1],"-i")==0){
                printf("Enter the number of machines in stage 1:");
                if (scanf("%d",&num_machines)!=1){
/*if input value is not integer*/
                        printf("Error in reading the instance\n");
                        return -1;
/*quit*/
                }
                printf("Enter the number of jobs:");
                /*scanf("%d",&num_jobs);*/
                if(scanf("%d",&num_jobs)!=1){
                        printf("Error in reading the instance\n");
                        return -2;}
                printf("Enter in each line the processing time for a job:");
/*build an array which the row is number of jobs, col is 3*/
                job = (int** ) malloc(num_jobs * sizeof(int*));
                for (int i = 0; i< num_jobs; i++){
                  job[i] = (int *) malloc(4 * sizeof(int));
                }

                for (row=0;row<num_jobs;row++){
                                /*scanf("%d",&job[row][col]);*/
                                if (scanf("%d",&job[row][0])!=1){
                                        printf("Error in reading the instance\n");
                                        return -3;}
                                if (scanf("%d",&job[row][1])!=1){
                                        printf("Error in reading the instance\n");
                                        return -3;}
                                if (scanf("%d",&job[row][2])!=1){
                                        printf("Error in reading the instance\n");
                                        return -3;}
                                job[row][3] = row;
                }

                qsort(job, num_jobs, sizeof(int*), comp);
                int t1;
                int t2 = -1;
                int t3 = -1;
                int t4;
                int final;
                if (num_machines< num_jobs)
                        jJob2 = johnsonSort(2);
                jJob1 = johnsonSort(1);
                // step a
                t1 = lpt(1, 0);
                if (num_machines < num_jobs)
                        // step b
                        t2 = johnson(2,0);
                // step c
                int t = t1;
                if (t1 != -1 && t2>= t1) t = t2;
                if (num_machines < num_jobs)
                        // step d
                        t3 = lpt(2, t);
                // step e
                t4 = johnson(1,t);
                final = t4;
                // step f
                if (t3 != -1 && t3>= t4) final = t3;
                printf("The LPT-Johnson schedule has an overall makespan %d\n",final);


}
        if(strcmp(argv[1],"-r")==0){
                printf("Generating random instance...\n");
                printf("Enter the number of machines in stage 1:");
                if (scanf("%d",&num_machines)!=1){
                        printf("Error in reading the instance!\n");
                        return -1;
                }
                printf("Enter the number of jobs:");
                if (scanf("%d",&num_jobs)!=1){
                        printf("Error in reading the instance!\n");
                        return -2;
                }
                printf("Enter the processing time interval[t_1,t_2]:");
                if (scanf("%d",&interval1)!=1){
                        printf("Error in reading the instance!\n");
                        return -3;}
                if (scanf("%d",&interval2)!=1){
                        printf("Error in reading the instance!\n");
                        return -4;}
                printf("Enter the number of instance to be generated:");
                if (scanf("%d",&inst_num)!=1){
                        printf("Error in reading the instance!\n");
                        return -5;}
                for (i=0;i<inst_num;i++){
                        printf("#number of machines in stage 1\n%d\n",num_machines);
                        printf("#number of jobs\n%d\n",num_jobs);
                        printf("#job processing times\n");
                        for (row=0;row<num_jobs;row++){
                                for(col=0;col<3;col++){
                                        r=(rand() % interval2) + interval1;
/*get the random number between the interval1 and interval2*/
                                        printf("%d ",r);}
                                printf("\n");
                        }
                }
                printf("#end of instance\n");


}


        return 0;
}