#include "FilesFormats.h"
#include <string>
#include "sam_mapping.h"


using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 11)
	{
			cout << "error in command line " << argc <<"\n";
			return 0;
	}

	std::string samFileName = argv[1];
	std::string refFileName = argv[2];
	std::string refContigName = argv[3];
	double limVal = atof(argv[4]);
	float fraction = atof(argv[5]);

	std::string fileGraph = argv[6];
	std::string fileSimpleGraph = argv[7];
	std::string resultFileName = argv[8];
	std::string fileDebugFileName = argv[9];
	int num_chr = atoi(argv[10]);
	// int reduce_counter_threshold = atoi(argv[10]);
	// double alpha = atof(argv[11]);
	// double entropy_factor = atof(argv[12]);
	// int norm_changes_threshold = atoi(argv[13]);

  SAM_mapping samMap(samFileName,refFileName,refContigName,limVal,fraction);
  samMap.FindGaploidsAlgorithm(fileGraph, fileSimpleGraph,resultFileName, fileDebugFileName, num_chr);
	return 0;
}
