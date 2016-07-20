//---------------------------------------------------------------------------

#pragma hdrstop

#include "BaseClasses.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

const char* StringEmpty = "";

int SequenceRecord::WriteToFile(FILE *fileToWrite)
{
	int errorHappens = 0;
	std::string seqNameToWrite = this->_sequenceName;
	std::string seqTextToWrite = this->_sequenceText;

	seqNameToWrite += "\n";
	seqTextToWrite += "\n";

	errorHappens =  fputs(seqNameToWrite.c_str(),fileToWrite);

	if(errorHappens == ErrorHappens)
	{
		return ErrorHappens;
	}

	errorHappens =  fputs(seqTextToWrite.c_str(),fileToWrite);

	if(errorHappens == ErrorHappens)
	{
		return ErrorHappens;
	}

	return errorHappens;
}

char SequenceRecord::GetCharOfSequenceTextByNum(unsigned int posToTake)
{
	char charReturn;
	if(posToTake < _sequenceText.length())
	{
		charReturn = this->_sequenceText[posToTake];
	}
	else
	{
		charReturn = 'N';
	}
	

	return charReturn;
}


