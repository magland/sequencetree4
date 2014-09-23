
#include "createsequence.h"
#include "stsimscanner.h"
#include <string.h>
#include <stdlib.h>

void display_usage() {
	printf("Usage: simulator [check|stat|run|raw_template] [param_input_fname] [param_output_fname] [sim_output_fname/raw_output_fname] [results_output_fname] [min_block] [max_block]");
}

void write_to_results_file(char *results_fname,const char *str) {
	FILE *outf=fopen(results_fname,"a");
	if (!outf) return;
	fprintf(outf,"%s\n",str);
	fclose(outf);
}

///////////////////////////////// ** ////////////////////////////////////
int main(int argc, char *argv[])
{
	if (argc-1!=7) {
		display_usage();
		return -1;
	}
	char command[500];
	char input_param_fname[500];
	char output_param_fname[500];
	char output_sim_fname[500];
	char output_results_fname[500];
	int min_block;
	int max_block;
	strcpy(command,argv[1]);
	strcpy(input_param_fname,argv[2]);
	strcpy(output_param_fname,argv[3]);
	strcpy(output_sim_fname,argv[4]);
	strcpy(output_results_fname,argv[5]);
	min_block=atoi(argv[6]);
	max_block=atoi(argv[7]);
	STSimScanner scanner;
	if ((min_block>=0)&&(max_block>=0)) {
		scanner.setBlockRange(min_block,max_block);
	}
	STSequence *Seq=createSequence(input_param_fname);
	//STSequence *Seq=createSequence(0);
	Seq->setScanner(&scanner);
	Seq->setup();
	Seq->setTreeIndices(); //important to do this before preparing
	Seq->initialize();
	Seq->prepare();
	bool ret=Seq->check();
	if (strcmp(command,"check")==0) {
	}
	else if (strcmp(command,"stat")==0) {
		if (!Seq->computeStatistics())
			ret=false;
		char outstr[500];
		sprintf(outstr,"duration=%.0f",Seq->statDuration());
		write_to_results_file(output_results_fname,outstr);
		sprintf(outstr,"num_blocks=%d",(int)Seq->statBlockCount());
		write_to_results_file(output_results_fname,outstr);
		sprintf(outstr,"SAR=%g",Seq->statSAR());
		write_to_results_file(output_results_fname,outstr);
	}
	else if (strcmp(command,"run")==0) {
		if (!Seq->run()) ret=false;
		char outstr[500];
		sprintf(outstr,"duration=%.0f",scanner.totalDuration());
		write_to_results_file(output_results_fname,outstr);
		sprintf(outstr,"num_blocks=%d",(int)scanner.blockCount());
		write_to_results_file(output_results_fname,outstr);
	}
	else if (strcmp(command,"raw_template")==0) {
		FILE *rawf=fopen(output_sim_fname,"wb");
		if (rawf) {
			scanner.setRawTemplateFile(rawf);
			if (!Seq->run()) ret=false;
			fclose(rawf);
		}
		else ret=false;
	}
	if (strcmp(command,"raw_template")!=0) 
		scanner.writeBlocks(output_sim_fname);
	Seq->writeParametersToFile(output_param_fname);
	if (ret) write_to_results_file(output_results_fname,"ret=true");
	else write_to_results_file(output_results_fname,"ret=false");
    	return 0;
}

