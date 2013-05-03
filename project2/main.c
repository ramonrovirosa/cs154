
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

InstInfo * pipelineInsts[5];

int main(int argc, char *argv[])
{
	InstInfo curInst;
	InstInfo *instPtr = &curInst;
	int instnum = 0;
	int maxpc;
	FILE *program;
	if (argc != 2)
	{
		printf("Usage: sim filename\n");
		exit(0);
	}
	int i;
	for (i=0;i<5;i++){
		pipelineInsts[i]=malloc(sizeof(InstInfo));
		pipelineInsts[i]->inst=0;
	}
	maxpc = load(argv[1]);
	//printLoad(maxpc);
	//printf("finished load");
	int cycles = 0, executions=0;
	while (pc <= maxpc + 4)
	{
	  //if(pipelineInsts[0]->inst != 0)
	  fetch(pipelineInsts[0]);
	  if(pipelineInsts[1]->inst != 0)
		decode(pipelineInsts[1]);

	  if(pipelineInsts[2]->inst != 0){
		execute(pipelineInsts[2]);
		
		//add1 $s2,$s1,$0
		//add2 $s1,$0,$0 //$s1 dependency
		//if the dest of the second one is used in the first one, than
		//you must data forward the aluout of add2 to the $s1 register
		
		
		if( pipelineInsts[2]->destreg == pipelineInsts[1]->input1)
		  pipelineInsts[1]->s1data = pipelineInsts[2]->aluout;
        
		if( pipelineInsts[2]->destreg == pipelineInsts[1]->input2)
		  pipelineInsts[1]->s2data = pipelineInsts[2]->aluout;
	  }

	  if(pipelineInsts[3]->inst != 0){
	    memory(pipelineInsts[3]);	
		
		if( pipelineInsts[3]->destreg == pipelineInsts[1]->input1){
		  // if the instruction that just went through the memory function
		  // is a lw then you pull from memout otherwise you pull from aluout
		  if(pipelineInsts[3]->fields.op == 6){
		    pipelineInsts[1]->s1data = pipelineInsts[3]->memout;
		  }else{
		    pipelineInsts[1]->s1data = pipelineInsts[3]->aluout;
		  }
		}
	        if(pipelineInsts[3]->destreg  == pipelineInsts[1]->input2){
		  if(pipelineInsts[3]->fields.op == 6){
		    pipelineInsts[1]->s2data = pipelineInsts[3]->memout;
		  }else{
		    pipelineInsts[1]->s2data = pipelineInsts[3]->aluout;
		  }
		}
	  }

	  if(pipelineInsts[4]->inst != 0){
	    writeback(pipelineInsts[4]);
		
	    //if( pipelineInsts[4]->input1 == pipelineInsts[1]->destreg ){
	    if(pipelineInsts[4]->destreg == pipelineInsts[1]->input1){
	      if(pipelineInsts[4]->fields.op == 6){
		    pipelineInsts[1]->s1data = pipelineInsts[4]->memout;
		  }else{
		    pipelineInsts[1]->s1data = pipelineInsts[4]->aluout;
		  }
		}
		
	    //if(  pipelineInsts[4]->input2  == pipelineInsts[1]->destreg){
	    if(pipelineInsts[4]->destreg == pipelineInsts[1]->input2){
	      if(pipelineInsts[4]->fields.op == 6){
		    pipelineInsts[1]->s2data = pipelineInsts[4]->memout;
		  }else{
		    pipelineInsts[1]->s2data = pipelineInsts[4]->aluout;
		  }
		}
	  } 
		//print(instPtr,instnum++);
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],
			pipelineInsts[3],pipelineInsts[4],cycles);


		//check # of executions
		if(pipelineInsts[2]->inst !=0)
		  executions++;



		InstInfo * tmp = pipelineInsts[4];
		pipelineInsts[4]=pipelineInsts[3];
		pipelineInsts[3]=pipelineInsts[2];
		pipelineInsts[2]=pipelineInsts[1];
		pipelineInsts[1]=pipelineInsts[0];
		pipelineInsts[0]=tmp;
		cycles++;

	}

	
	
	// put in your own variables
	printf("Cycles: %d\n",cycles );
	printf("Instructions Executed: %d\n", executions );
	exit(0);
}



/*
 * print out the loaded instructions.  This is to verify your loader
 * works.
 */
void printLoad(int max)
{
	int i;
	for(i=0;i<max;i++)
		printf("%d\n",instmem[i]);
}

/* print
 *
 * prints out the state of the simulator after each instruction
 */
void print(InstInfo *inst, int count)
{
	int i, j;
	printf("Instruction %d: %d\n",count,inst->inst);
	printf("%s\n\n",inst->string);
	printf("Fields:\n rd: %d\nrs: %d\nrt: %d\nimm: %d\n\n",
		inst->fields.rd, inst->fields.rs, inst->fields.rt, inst->fields.imm);
	printf("Control Bits:\nalu: %d\nmw: %d\nmr: %d\nmtr: %d\nasrc: %d\nbt: %d\nrdst: %d\nrw: %d\n\n",
		inst->signals.aluop, inst->signals.mw, inst->signals.mr, inst->signals.mtr, inst->signals.asrc,
		inst->signals.btype, inst->signals.rdst, inst->signals.rw);
	printf("ALU Result: %d\n\n",inst->aluout);
	if (inst->signals.mr == 1)
		printf("Mem Result: %d\n\n",inst->memout);
	else
		printf("Mem Result: X\n\n");
	for(i=0;i<8;i++)
	{
		for(j=0;j<32;j+=8)
			printf("$%d: %4d ",i+j,regfile[i+j]);
		printf("\n");
	}
	printf("\n");
}


void printP2(InstInfo *inst0, InstInfo *inst1, InstInfo *inst2, InstInfo *inst3, InstInfo *inst4,  int count)
{
	int i, j;
	printf("Cycle %d:\n",count);
	if(inst0->inst != 0)
		printf("Fetch instruction: %d\n", inst0->inst);
	else
		printf("Fetch instruction: \n");
	if(inst1->inst != 0)
		printf("Decode instruction: %s\n", inst1->string);
	else
		printf("Decode instruction: \n");
	if(inst2->inst !=0)
		printf("Execute instruction: %s\n", inst2->string);
	else
		printf("Execute instruction: \n");

	if(inst3->inst !=0)
		printf("Memory instruction: %s\n", inst3->string);
	else
		printf("Memory instruction: \n");

	if(inst4->inst !=0)
		printf("Writeback instruction: %s\n", inst4->string);
	else
		printf("Writeback instruction: \n");



	for(i=0;i<8;i++)
	{
		for(j=0;j<32;j+=8)
			printf("$%d: %4d ",i+j,regfile[i+j]);
		printf("\n");
	}
	printf("\n");
}

