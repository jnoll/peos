/*****************************************************************/
/* File:        testvm.c 
/* Author:      Crystal Zhu
/* Date:        11/20/1999
/* Description: This file is for test purpose
/* Inplementation: Running the command from OS and getting the result,
/*		   then comparing with the expected result. 
/*****************************************************************/     
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pml_state.h>

//simulate OS , OS output to VM(input for VM) 
char * command[34] = {
"pmlvm -p sample",
"pmlvm -p sample.001 -l 26",
"pmlvm -p sample.002 -l 63",
"pmlvm -p sample.001 -a add_ice",
"pmlvm -p sample.001 -e 3 -o add_ice",
"pmlvm -p sample.001 -a add_vermouth",
"pmlvm -p sample.001 -e 3 -o add_vermouth",
"pmlvm -p sample.001 -a add_gin",
"pmlvm -p sample.001 -e 3 -o add_gin",
"pmlvm -p sample.001 -a shake",
"pmlvm -p sample.001 -e 3 -o shake",
"pmlvm -p sample -c 0",
"pmlvm -p sample.002 -a cut_twist",
"pmlvm -p sample.002 -e 2 -o cut_twist",
"pmlvm -p sample.002 -a cut_twist",
"pmlvm -p sample.002 -e 3 -o cut_twist",
"pmlvm -p sample.002 -a twist_twis",
"pmlvm -p sample.002 -e 3 -o twist_twis",
"pmlvm -p sample.002 -e 2 -o wash_olive",
"pmlvm -p sample.002 -e 2 -o wash_olive",
"pmlvm -p sample.002 -a wash_olive",
"pmlvm -p sample.002 -e 3 -o wash_olive",
"pmlvm -p sample.002 -a skewer_olive",
"pmlvm -p sample.002 -e 3 -o skewer_olive",
"pmlvm -p sample.002 -e 2 -o pour_one",
"pmlvm -p sample -c 0",
"pmlvm -p sample -e 2 -o pour_one",
"pmlvm -p sample -a pour_one",
"pmlvm -p sample -e 3 -o pour_one",
"pmlvm -p sample -e 2 -o pour_one",
"pmlvm -p sample -a pour_one",
"pmlvm -p sample -e 3 -o pour_one",
"pmlvm -p sample -e 2 -o end",
""
};
// expected result ( VM output to OS (input for OS))
char * results[] = {
"b sample.001 26\n",
"b sample.002 63\n",
"c sample.001\n",
"c sample.002\n",
"a 3 add_ice\n",
"a 2 cut_twist wash_olive pour_one \n",
"a 3 add_vermouth\n",
"a 3 add_gin\n",
"a 3 shake\n",
"e 0\n",
"c \n",
"a 2 cut_twist wash_olive \n",
"a 3 cut_twist\n",
"a 3 twist_twis\n",
"a 2 cut_twist wash_olive pour_one \n",
"a 2 cut_twist wash_olive \n",
"a 3 wash_olive\n",
"a 3 skewer_olive\n",
"a 2 cut_twist wash_olive pour_one \n",
"e 0\n",
"a 2 pour_one end\n",
"a 3 pour_one\n",
"a 2 pour_one end\n",
"a 3 pour_one\n",
"a 2 pour_one end\n",
"e 0\n"
};


int main(int argc , char * argv[])
{
    int i = 0, j =0, z = 0;
    FILE * fp = NULL;
    char buffer[1024];

    /* clean up the repository */
    pml_open_repository("/home/qzhu/cs5728/Engine/src/", TRUNCATE);
    pml_close_repository();
    pml_open_repository("/home/qzhu/cs5728/Engine/src/", APPEND);
    for(z =0; z < 3; z++, j = 0)
    {
	printf("...... loop %d\n\n", z);
    for ( i =0 ; i < 34; i++ )
    {
	fp = popen(command[i], "r");
	assert(fp);
	while(fgets(buffer, 1024, fp)){
		printf(" %d  VM output: %s", i+1, buffer);	
	    if(strcmp(buffer, results[j++]) != 0)
            {
		fprintf(stderr, "%s not equal to %s\n", buffer, results[--j]);
		fprintf(stderr, "Failed on command: %s", command[i]);
		exit(1);
            } 
	}
	//printf("TEST passed %d %s\n\n", i+1, command[i]);
	fclose(fp);
    }
    }
    pml_close_repository();
    return 0;
}
