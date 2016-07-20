//---------------------------------------------------------------------------

//#pragma hdrstop

#include "FilesFormats.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)


int FastaSequenceFileBuf::LoadFromFile(const char* fileName)
{
    _seqFastaRecords.Clear();
    FILE *fileWithSeq = fopen(fileName,"r");
	if(fileWithSeq == NULL)
	{
		return ErrorHappens;
	}
	char *bufToRead = new char[255];

	std::string readedSeqNam;
	std::string readedSeqText;
	FastaSequenceRecord seqRecord;


	readedSeqNam = "";
	readedSeqText = "";
	while (!feof(fileWithSeq))
	{
		char letterFromFile;
		letterFromFile=fgetc(fileWithSeq);
        if((letterFromFile == '\n') || (letterFromFile == '\r'))
        {
            continue;
        }
		if(letterFromFile == '>')
		{
			if((readedSeqNam != "") && readedSeqText != "")
			{
				//cout << "readedSeqNam="<<readedSeqNam<<"\n";
                //cout << "readedSeqText="<<readedSeqText<<"\n";
                readedSeqNam = DeleteEndOfLineChars(readedSeqNam);
                readedSeqText = DeleteEndOfLineChars(readedSeqText);

                seqRecord.Set(readedSeqNam,readedSeqText);
                _seqFastaRecords.AddToSet(seqRecord);
				seqRecord.Clear();
				readedSeqNam = "";
				readedSeqText = "";
			}

			if(!feof(fileWithSeq) && (fgets(bufToRead,255,fileWithSeq) == NULL))
			{
				while(!feof(fileWithSeq) && fgets(bufToRead,255,fileWithSeq) == NULL)
				{
					readedSeqNam += bufToRead;
				}
			}
			else
			{
				readedSeqNam += bufToRead;
			}
        }
		else
		{
            if(!feof(fileWithSeq))
            {
                readedSeqText += letterFromFile;
            }
		}

	}
    if((readedSeqNam != "") && readedSeqText != "")
    {
		//cout << "readedSeqNam="<<readedSeqNam<<"\n";
        //cout << "readedSeqText="<<readedSeqText<<"\n";


        readedSeqNam = DeleteEndOfLineChars(readedSeqNam);
        readedSeqText = DeleteEndOfLineChars(readedSeqText);


        seqRecord.Set(readedSeqNam,readedSeqText);
        _seqFastaRecords.AddToSet(seqRecord);
        seqRecord.Clear();
        readedSeqNam = "";
        readedSeqText = "";
    }
	fclose(fileWithSeq);
	return 1;
}

int FastaSequenceFileBuf::SaveToFile(const char* fileName)
{
	FILE *fileToWrite = fopen(fileName,"w");
	if(fileToWrite == NULL)
	{
		return ErrorHappens;
	}

    int sizeOfSet = _seqFastaRecords.GetSizeOfSet();

	for(int i = 0; i < sizeOfSet;i++)
	{
      FastaSequenceRecord seqRecord_i = _seqFastaRecords.GetItemByNum(i);

       seqRecord_i.WriteToFile(fileToWrite);
    }

	fclose(fileToWrite);
	return 1;
}

int FastaSequenceRecord::WriteToFile(FILE *fileToWrite)
{
	int errorHappens = 0;
	std::string seqNameToWrite = ">";
	seqNameToWrite += this->_sequenceName;
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












int FastqSequenceFileBuf::LoadFromFile(const char* fileName)
{
    _seqFastqRecords.Clear();
    FILE *fileWithSeq = fopen(fileName,"r");
	if(fileWithSeq == NULL)
	{
		return ErrorHappens;
	}
	char *bufToRead = new char[255];

	std::string readedSeqNam;
	std::string readedSeqText;
	std::string readedSeqQual;
	FastqSequenceRecord seqRecord;


	readedSeqNam = "";
	readedSeqText = "";
	readedSeqQual = "";
    char tempLetter;
	while (!feof(fileWithSeq))
	{
		readedSeqNam = "";
		readedSeqText = "";
		readedSeqQual = "";
        tempLetter = fgetc(fileWithSeq);
        if((tempLetter == '\n') || (tempLetter == '\r') || feof(fileWithSeq))
        {
            continue;
        }
		if(!feof(fileWithSeq) && (fgets(bufToRead,255,fileWithSeq) == NULL))
		{
			while(!feof(fileWithSeq) && fgets(bufToRead,255,fileWithSeq) == NULL)
			{
				readedSeqNam += bufToRead;
			}
		}
		else
		{
			if(bufToRead[0] == '\n')
			{
				break;
			}
			readedSeqNam += bufToRead;
		}


		if(!feof(fileWithSeq) && (fgets(bufToRead,255,fileWithSeq) == NULL))
		{
			while(!feof(fileWithSeq) && fgets(bufToRead,255,fileWithSeq) == NULL)
			{
				readedSeqText += bufToRead;
			}
		}
		else
		{
			if(bufToRead[0] == '\n')
			{
				break;
			}
			readedSeqText += bufToRead;
		}

		fgets(bufToRead,255,fileWithSeq);

		if(!feof(fileWithSeq) && (fgets(bufToRead,255,fileWithSeq) == NULL))
		{
			while(!feof(fileWithSeq) && fgets(bufToRead,255,fileWithSeq) == NULL)
			{
				readedSeqQual += bufToRead;
			}
		}
		else
		{
			if(bufToRead[0] == '\n')
			{
				break;
			}
			readedSeqQual += bufToRead;
		}

		//cout << "readedSeqNam="<<readedSeqNam<<"\n";
		//cout << "readedSeqText="<<readedSeqText<<"\n";
        //cout << "readedSeqQual="<<readedSeqQual<<"\n";
        readedSeqNam = DeleteEndOfLineChars(readedSeqNam);
        readedSeqText = DeleteEndOfLineChars(readedSeqText);
        readedSeqQual = DeleteEndOfLineChars(readedSeqQual);
        seqRecord.Set(readedSeqNam,readedSeqText,readedSeqQual);
        _seqFastqRecords.AddToSet(seqRecord);
		seqRecord.Clear();

	}
	fclose(fileWithSeq);
	return 1;
}

int FastqSequenceFileBuf::SaveToFile(const char* fileName)
{
	FILE *fileToWrite = fopen(fileName,"w");
	if(fileToWrite == NULL)
	{
		return ErrorHappens;
	}

    int sizeOfSet = _seqFastqRecords.GetSizeOfSet();

	for(int i = 0; i < sizeOfSet;i++)
	{
       FastqSequenceRecord seqRecord_i =	_seqFastqRecords.GetItemByNum(i);

       seqRecord_i.WriteToFile(fileToWrite);
    }

	fclose(fileToWrite);
	return 1;
}

int FastqSequenceRecord::WriteToFile(FILE *fileToWrite)
{
	int errorHappens = 0;
	std::string seqNameToWrite = "@";
	seqNameToWrite += this->_sequenceName;
	std::string seqTextToWrite = this->_sequenceText;
	std::string seqQualToWrite = "+\n";
	seqQualToWrite += this->_sequenceQual;

	seqNameToWrite += "\n";
	seqTextToWrite += "\n";
    seqQualToWrite += "\n";

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

	errorHappens =  fputs(seqQualToWrite.c_str(),fileToWrite);

	if(errorHappens == ErrorHappens)
	{
		return ErrorHappens;
	}

	return errorHappens;
}


int FastqSequenceRecord::WriteToFileSeq(FILE *fileToWrite)
{
	int errorHappens = 0;
	std::string seqNameToWrite = "";
	seqNameToWrite += this->_sequenceName;


	seqNameToWrite += "\n";


	errorHappens =  fputs(seqNameToWrite.c_str(),fileToWrite);

	if(errorHappens == ErrorHappens)
	{
		return ErrorHappens;
	}



	return errorHappens;
}
int FastaSequenceFileUnBuf::GetNextSeqRead()
{
	if(!_isFileOpened)
	{
		return ErrorHappens;
	}
	char *bufToRead = new char[255];

	std::string readedSeqNam;
	std::string readedSeqText;
	FastaSequenceRecord seqRecord;


	readedSeqNam = "";
	readedSeqText = "";
	//cout << "before <" << "\n";
	while((_letterFromFile != '>') && (!feof(_seqFile)))
	{
		_letterFromFile=fgetc(_seqFile);
	}
	//cout << "after <" << _letterFromFile << "\n";


	bool isFound = false;
	while (!feof(_seqFile))
	{

        if((_letterFromFile == '>') && (isFound))
		{
			//cout << "iterationLoad readedSeqNam " << readedSeqNam << " readedSeqText " << readedSeqText << "\n";
            readedSeqNam = DeleteEndOfLineChars(readedSeqNam);
            readedSeqText = DeleteEndOfLineChars(readedSeqText);
            seqRecord.Set(readedSeqNam,readedSeqText);
			_seqRecord = seqRecord;
            //_seqRecord.PrintOnTerminal();
			seqRecord.Clear();
            //_seqRecord.PrintOnTerminal();
			break;
		}

		if((_letterFromFile == '>') && (!isFound))
		{
			if(!feof(_seqFile) && (fgets(bufToRead,255,_seqFile) == NULL))
			{
				while(!feof(_seqFile) && fgets(bufToRead,255,_seqFile) == NULL)
				{

					readedSeqNam += bufToRead;

				}
				_letterFromFile=fgetc(_seqFile);
				readedSeqText += _letterFromFile;

			}
			else
			{
				readedSeqNam += bufToRead;
				_letterFromFile=fgetc(_seqFile);
				readedSeqText += _letterFromFile;
			}
			isFound = true;
		}
		else
		{

			_letterFromFile=fgetc(_seqFile);
            if((_letterFromFile!= '>') && (!feof(_seqFile)) && (_letterFromFile != '\r') && (_letterFromFile != '\n'))
			{
				readedSeqText += _letterFromFile;
			}
			isFound = true;
		}

	}

	if(feof(_seqFile))
	{
        readedSeqNam = DeleteEndOfLineChars(readedSeqNam);
        readedSeqText = DeleteEndOfLineChars(readedSeqText);
        seqRecord.Set(readedSeqNam,readedSeqText);
		_seqRecord = seqRecord;
        seqRecord.Clear();
    }
	return 1;
}

FastaSequenceRecord FastaSequenceFileUnBuf::GetCurrSeqRecord()
{
    //cout << "GetCurrSeqRecord\n";
    //_seqRecord.PrintOnTerminal();
    return _seqRecord;
}


int FastqSequenceFileUnBuf::GetNextSeqRead()
{
    //cout << "_isFileOpened " << this->_isFileOpened+1 << "\n";
    if(!_isFileOpened)
	{
        //cout << "Error Happens \n";
        return ErrorHappens;
	}
	char *bufToRead = new char[255];

	std::string readedSeqNam;
	std::string readedSeqText;
	std::string readedSeqQual;
	FastqSequenceRecord seqRecord;

    //cout << "here \n";
	readedSeqNam = "";
	readedSeqText = "";
	readedSeqQual = "";
    //cout << "GetNextSeqRead 1\n";
    char tempLetter = fgetc(_seqFile);

    if((tempLetter == '\r') && (tempLetter == '\n'))
    {
        return 1;
        //tempLetter = fgetc(_seqFile);
    }

    //cout << "GetNextSeqRead 2\n";
    if(!feof(_seqFile) && (fgets(bufToRead,255,_seqFile) == NULL))
	{
        while(!feof(_seqFile) && fgets(bufToRead,255,_seqFile) == NULL)
		{
            //cout << "GetNextSeqRead 3\n";
            readedSeqNam += bufToRead;
            //cout << "GetNextSeqRead 4\n";
		}
	}
	else
	{
		//cout << "GetNextSeqRead 5\n";
		readedSeqNam += bufToRead;
		//cout << "GetNextSeqRead 6\n";
	}

	//cout << "here \n";
	if(!feof(_seqFile) && (fgets(bufToRead,255,_seqFile) == NULL))
	{
        while(!feof(_seqFile) && fgets(bufToRead,255,_seqFile) == NULL)
		{
			readedSeqText += bufToRead;
		}
	}
	else
	{
		readedSeqText += bufToRead;
	}

    fgets(bufToRead,255,_seqFile);
    //cout << "here \n";
    if(!feof(_seqFile) && (fgets(bufToRead,255,_seqFile) == NULL))
	{
        while(!feof(_seqFile) && fgets(bufToRead,255,_seqFile) == NULL)
		{
			readedSeqQual += bufToRead;
		}
	}
	else
	{
		readedSeqQual += bufToRead;
	}
	//cout << "here \n";
	//cout << "readedSeqNam = " << readedSeqNam << "\n";
	//cout << "readedSeqText = " << readedSeqText << "\n";
    //cout << "readedSeqQual = " << readedSeqQual << "\n";
    readedSeqNam = DeleteEndOfLineChars(readedSeqNam);
    readedSeqText = DeleteEndOfLineChars(readedSeqText);
    readedSeqQual = DeleteEndOfLineChars(readedSeqQual);
    seqRecord.Set(readedSeqNam,readedSeqText,readedSeqQual);
    _seqRecord = seqRecord;
    seqRecord.Clear();
	return 1;
}


FastaSequenceRecord FastaSequenceFileUnBuf::GetSeqRecordByReadName(std::string readName)
{
    if(!_isFileOpened)
    {
        return FastaSequenceRecord::Empty();
    }
    this->SetOnStart();
	FastaSequenceRecord seqRecToSearch;
	bool isFound = false;
	while(!this->IsEndOfFile())
	{
		this->GetNextSeqRead();
		seqRecToSearch = this->GetCurrSeqRecord();
		//cout << seqRecToSearch.GetSequenceName() << " " << readName << "\n";
		if(seqRecToSearch.GetSequenceName() == readName)
		{
			isFound = true;
			break;
		}
	}
	if(isFound)
	{
		return seqRecToSearch;
	}

	return FastaSequenceRecord::Empty();

}


FastqSequenceRecord FastqSequenceFileUnBuf::GetSeqRecordByReadName(std::string readName)
{
    if(!_isFileOpened)
    {
        return FastqSequenceRecord::Empty();
    }
	//cout << "GetSeqRecordByReadName 1\n";
	this->SetOnStart();
    //cout << "GetSeqRecordByReadName 2\n";
	FastqSequenceRecord seqRecToSearch;
	bool isFound = false;
	while(!this->IsEndOfFile())
	{
		this->GetNextSeqRead();
		//cout << "GetSeqRecordByReadName 3\n";
		seqRecToSearch = this->GetCurrSeqRecord();
		//cout << "GetSeqRecordByReadName 4\n";

		if(seqRecToSearch.GetSequenceName() == readName)
		{
			isFound = true;
			break;
        }
		//cout << "GetSeqRecordByReadName 5\n";
    }
	if(isFound)
	{
		return seqRecToSearch;
	}

	return FastqSequenceRecord::Empty();
}



