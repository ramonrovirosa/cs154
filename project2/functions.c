
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

// these are the structures used in this simulator

void branchControl(InstInfo*);

// global variables
// register file
int regfile[32];
// instruction memory
int instmem[100];  // only support 100 static instructions
// data memory
int datamem[1000];
// program counter
int pc;

/* load
 *
 * Given the filename, which is a text file that 
 * contains the instructions stored as integers 
 *
 * You will need to load it into your global structure that 
 * stores all of the instructions.
 *
 * The return value is the maxpc - the number of instructions
 * in the file
 */
int load(char *filename){
	FILE * in = fopen(filename, "rt");
	char mystring [100];
	int maxpc=0;
	char * nullPtr;
	while(fgets (mystring , 100 , in) != NULL){
		instmem[maxpc] = strtol(mystring,&nullPtr,10);
		//printf("see: %d", maxpc);
		maxpc=maxpc+1;
	}
	//printf("maxpc: %d",maxpc);
	if(feof(in)){
		fclose(in);
	}
	//printf("maxpc: %d \n",maxpc);
	return maxpc - 1;
}

/* fetch
 *
 * This fetches the next instruction and updates the program counter.
 * "fetching" means filling in the inst field of the instruction.
 */
void fetch(InstInfo *instruction){
  instruction->inst=instmem[pc];
  pc++;
}

/* decode
 *
 * This decodes an instruction.  It looks at the inst field of the 
 * instruction.  Then it decodes the fields into the fields data 
 * member.  The first one is given to you.
 *
 * Then it checks the op code.  Depending on what the opcode is, it
 * fills in all of the signals for that instruction.
 */
void decode(InstInfo *instruction)
{
	// fill in the signals and fields
	int val = instruction->inst;
	int op, func, MASK;
	MASK = 0b11111111111111110000000000000000;
	instruction->fields.op  = (val >> 26) & 0x003f;
	instruction->fields.rs  = (val >> 21) & 0x001f;
	instruction->fields.rt  = (val >> 16) & 0x001f;
	instruction->fields.rd  = (val >> 11) & 0x001f;
	instruction->fields.func=  val        & 0x003f;
	int bit16 = val & 0xffff;
//	instruction->fields.imm = ((bit16 >> 15) == 1)?bit16 | MASK : bit16;
	instruction->fields.imm = (bit16<<16)>>16;

	// fill in the rest of the fields here

	// now fill in the signals

	// fill in s1data and input2
	instruction->input1 = instruction->fields.rs;
	instruction->input2 = instruction->fields.rt;
	instruction->s1data = regfile[instruction->input1];
	instruction->s2data = regfile[instruction->input2];

	//printf("op code: %d" ,instruction->fields.op);

	//add operation
	if( (instruction->fields.op == 48)&& (instruction->fields.func == 10) )
	{
		instruction->signals.aluop = 1;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = 0;
		instruction->signals.asrc = 0;
		instruction->signals.btype = 0;
		instruction->signals.rdst = 1;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"add $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs, 
			instruction->fields.rt);
		instruction->destreg = instruction->fields.rd;
		// fill in s1data and input2
		instruction->input1 = instruction->fields.rs;
		instruction->input2 = instruction->fields.rt;
		instruction->s1data = regfile[instruction->input1];
		instruction->s2data = regfile[instruction->input2];
	}

	//or operation
	else if( (instruction->fields.op  == 48)&& (instruction->fields.func == 48) )
	{
		instruction->signals.aluop = 4;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = 0;
		instruction->signals.asrc = 0;
		instruction->signals.btype = 0;
		instruction->signals.rdst = 1;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"or $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs, 
			instruction->fields.rt);
		instruction->destreg = instruction->fields.rd;
		// fill in s1data and input2
		instruction->input1 = instruction->fields.rs;
		instruction->input2 = instruction->fields.rt;
		instruction->s1data = regfile[instruction->input1];
		instruction->s2data = regfile[instruction->input2];
	}

	//subi
	else if(instruction->fields.op == 28)
         {
	   instruction->signals.aluop = 5;
	   instruction->signals.mw = 0;
	   instruction->signals.mr = 0;
	   instruction->signals.mtr = 0;
	   instruction->signals.asrc = 1;
	   instruction->signals.btype = 0;
	   instruction->signals.rdst = 0;
	   instruction->signals.rw = 1;
	   sprintf(instruction->string,"subi $%d, $%d, %d",
		   instruction->fields.rt, instruction->fields.rs, instruction->fields.imm);
	   instruction->destreg = instruction->fields.rt;
	   // fill in s1data and input2
	   instruction->input1 = instruction->fields.rs;
	   instruction->input2 = instruction->fields.imm;
	   instruction->s1data = regfile[instruction->input1];
	   instruction->s2data = instruction->input2;
	 }

	//slt operation
	else if( (instruction->fields.op == 48)&& (instruction->fields.func == 15) )
	{
		instruction->signals.aluop = 6;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = 0;
		instruction->signals.asrc = 0;
		instruction->signals.btype = 0;
		instruction->signals.rdst = 1;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"slt $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs, 
			instruction->fields.rt);
		instruction->destreg = instruction->fields.rd;
		// fill in s1data and input2
		instruction->input1 = instruction->fields.rs;
		instruction->input2 = instruction->fields.rt;
		instruction->s1data = regfile[instruction->input1];
		instruction->s2data = regfile[instruction->input2];
	}
	
	//lw
	else if(instruction->fields.op == 6)
        {
	  instruction->signals.aluop = 1;
	  instruction->signals.mw = 0;
	  instruction->signals.mr = 1;
	  instruction->signals.mtr = 1;
	  instruction->signals.asrc = 1;
	  instruction->signals.btype = 0;
	  instruction->signals.rdst = 0;
	  instruction->signals.rw = 1;
	  sprintf(instruction->string,"lw $%d, %d ($%d)",
		  instruction->fields.rt, instruction->fields.imm, instruction->fields.rs);
	  instruction->destreg = instruction->fields.rt;
	  // fill in s1data and input2
	  instruction->input1 = instruction->fields.rs;
	  instruction->input2 = instruction->fields.imm;
	  instruction->s1data = regfile[instruction->input1];
	  instruction->s2data = instruction->input2;
	}

	//sw
	else if(instruction->fields.op == 2)
	  {
	    instruction->signals.aluop = 1;
	    instruction->signals.mw = 1;
	    instruction->signals.mr = 0;
	    instruction->signals.mtr = -1;
	    instruction->signals.asrc = 1;
	    instruction->signals.btype = 0;
	    instruction->signals.rdst = -1;
	    instruction->signals.rw = 0;
	    sprintf(instruction->string,"sw $%d, %d ($%d)",
		    instruction->fields.rt, instruction->fields.imm, instruction->fields.rs);
	    instruction->destreg = -1;
	    // fill in s1data and input2
	    instruction->input1 = instruction->fields.rs;
	    instruction->input2 = instruction->fields.imm;
	    instruction->s1data = regfile[instruction->input1];
	    instruction->s2data = instruction->input2;
	  }

	// if it is a bge
	else if(instruction->fields.op ==  39)
	  {
	    instruction->signals.aluop = 5;
	    instruction->signals.mw = 0;
	    instruction->signals.mr = 0;
	    instruction->signals.mtr = -1;
	    instruction->signals.asrc = 0;
	    instruction->signals.btype = 2;
	    instruction->signals.rdst = -1;
	    instruction->signals.rw = 0;
	    sprintf(instruction->string,"bge $%d, $%d, %d",
		    instruction->fields.rs, instruction->fields.rt, 
		    instruction->fields.imm);
	    instruction->destreg = -1;
	    // fill in s1data and input2
	    //instruction->input1 = instruction->fields.rs;
	    //instruction->input2 = instruction->fields.imm;
	    //instruction->s1data = regfile[instruction->input1];
	    //instruction->s2data = instruction->input2;
	    instruction->input1 = instruction->fields.rs;
	    instruction->input2 = instruction->fields.rt;
	    instruction->s1data = regfile[instruction->input1];
	    instruction->s2data = regfile[instruction->input2];
	  }
	
	// if it is a j
	else if( instruction->fields.op == 36)
	  {
	    instruction->signals.aluop = -1;
	    instruction->signals.mw = 0;
	    instruction->signals.mr = 0;
	    instruction->signals.mtr = -1;
	    instruction->signals.asrc = -1;
	    instruction->signals.btype = 1;
	    instruction->signals.rdst = -1;
	    instruction->signals.rw = 0;
	    sprintf(instruction->string,"j %d",
		    instruction->fields.imm);
	    instruction->destreg = -1;
	    // fill in s1data and input2
	    instruction->input1 = instruction->fields.imm;
	    //instruction->input2 = instruction->fields.rt;
	    instruction->s1data = instruction->input1;
	  }
	  // if it is a jal
	else if(instruction->fields.op ==  34)
	    {
	      instruction->signals.aluop = -1;
	      instruction->signals.mw = 0;
	      instruction->signals.mr = 0;
	      instruction->signals.mtr = -1;
	      instruction->signals.asrc = -1;
	      instruction->signals.btype = 1;
	      instruction->signals.rdst = -1;
	      instruction->signals.rw = 1;
	      sprintf(instruction->string,"jal %d",
		      instruction->fields.imm);
	      instruction->destreg = -1;
	      // fill in s1data and input2
	      instruction->input1 = instruction->fields.imm;
	      instruction->s1data = instruction->input1;
	    }


	//xor operation
	else if( (instruction->fields.op == 48)&& (instruction->fields.func == 20) )
	{
		instruction->signals.aluop = 3;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = 0;
		instruction->signals.asrc = 0;
		instruction->signals.btype = 0;
		instruction->signals.rdst = 1;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"xor $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs, 
			instruction->fields.rt);
		instruction->destreg = instruction->fields.rd;
	    // fill in s1data and input2
	    instruction->input1 = instruction->fields.rs;
	    instruction->input2 = instruction->fields.rt;
	}
	else{
	  //printf("Reached end, oppcode is %d", instruction->fields.op);
	}
}

/* execute
 *
 * This fills in the aluout value into the instruction and destdata
 */

void execute(InstInfo *instruction)
{

  //add, lw, sw 001
  if(instruction->signals.aluop == 1){
    instruction->aluout  = instruction->s1data + instruction->s2data;
    //instruction->destreg = instruction->aluout;
  }
  //or 100
  if(instruction->signals.aluop == 4){
    instruction->aluout = instruction->s1data | instruction->s2data;
    //instruction->destdata = instruction->aluout;
    //instruction->destreg = instruction->aluout;
  }
  //sub 101
  if(instruction->signals.aluop == 5){
    instruction->aluout = instruction->s1data - instruction->s2data;
  }
  //and 000
  if(instruction->signals.aluop == 0){
    instruction->aluout  = instruction->s1data & instruction->s2data;
  }
  //slt 110
  if(instruction->signals.aluop == 6){
    //instruction->aluout = (instruction->s1data < instruction->s2data) ? 1 : 0;
    if(instruction->s1data < instruction->s2data)
	instruction->aluout=1;
    else
 	instruction->aluout=0;
	
  }
  //not 010
  if(instruction->signals.aluop == 2){
    instruction->aluout  = ~(instruction->s1data);
  }
  //xor 011
  if(instruction->signals.aluop == 3){
    instruction->aluout = instruction->s1data ^  instruction->s2data;
  }
  
  instruction->destdata = instruction->aluout;
  //instruction->destreg = instruction->aluout;
  
  branchControl(instruction);
}

/* memory
 *
 * If this is a load or a store, perform the memory operation
 */
void memory(InstInfo *instruction)
{
  int addr = (instruction->aluout)>>2;
  //sw
  //sw $s1,100($s2) == Memory[$s2+100] = $s1
  if(instruction->fields.op==2)
    //datamem[addr]=regfile[instruction->destreg];
      datamem[addr] = regfile[instruction->fields.rt];
  //lw
  if(instruction->fields.op==6){
    //get rt mem address + imm
   //    instruction->destdata=datamem[addr];
    instruction->memout = datamem[addr];
   }
}

/* writeback
 *
 * If a register file is supposed to be written, write to it now
 */
void writeback(InstInfo *instruction)
{	//if register write
	if(instruction->signals.rw) {
		//if not jump and link
		if(instruction->fields.op != 34) {
			if(instruction->signals.mtr == 0)//choose aluoutput
				regfile[instruction->destreg] = instruction->aluout;
			else //choose memory output
				regfile[instruction->destreg] = instruction->memout;
		}
		else // for jal => $ra = PC
			regfile[instruction->destreg] = instruction->pc;
	}
}
//modifying pc & $ra for jal
void branchControl(InstInfo *instruction){
	if (instruction->signals.btype == 1){
		//JAL
		if(instruction->signals.rw){
			regfile[31] = pc;
			pc = instruction->fields.imm;
		}
		//J
		else{
			pc = instruction->fields.imm;
		}
	}
	//BGE
	if (instruction->signals.btype==2){
		//if(( (instruction->aluout | 0x10000000) == 1) || (instruction->aluout == 0 )){
	  if( instruction->aluout >= 0){
	    pc = pc + instruction->fields.imm; 
	  }
	}


}

