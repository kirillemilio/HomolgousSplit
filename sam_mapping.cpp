#include "sam_mapping.h"
#include <math.h>
#include "AssemblyStatistics.h"
#include "Path.h"

int factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

bool CountPoisson(double lambda, int upper_limit, int number_of_chromosomes)
{
    double result = 0;
    for(int i = 0; i <= upper_limit; i++)
    {
        result += 1 / factorial(i) * pow(lambda / double(number_of_chromosomes), i);
    }
    result *= exp(-lambda / number_of_chromosomes);
    if(result >= 0.05)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PrintCoverageDistributionToFile(HashIntInt coverage_distribution, std::string fileName)
{
    std::fstream fileToWrite(fileName.c_str(),std::fstream::out);
    for(HashIntInt::iterator iter = coverage_distribution.begin(); iter != coverage_distribution.end(); iter++)
    {
            fileToWrite << iter->first << "," << iter->second <<"\n";
    }
    fileToWrite.close();
}

SAM_file::SAM_file()
{
    _isFileEnd = false;
    _isReadHitsLoaded = false;
    _isFileOpened = false;
    _fileName = "";
    _qName = "";
    _flag = "";
    _tName = "";
    _position = -1;
    _mapQual = "";
    _CIAGR = "";
    _MRNM = "";
    _MPOS = "";
    _ISIZE = "";
	_qReadText = "";
	_qReadMapText = "";
	_qQualText = "";
	_qQualMapText = "";
	_readHitsArray = SetOfRead_Hit();
	_currFile = NULL;
}

int SAM_file::OpenFile()
{
    int notError = 1;

    _currFile = fopen(_fileName.c_str(),"r");
    if(_currFile == NULL)
    {
        _isFileOpened = false;
        notError = ErrorHappens;
    }
    else
	{
        _isFileOpened = true;
    }

    return notError;
}

void SAM_file::ReadNextLine()
{
    if((!_isFileOpened) || (_isFileEnd))
	{
        return;
    }

    char* qName = new char[200];
    char* flag = new char[200];
    char* tName = new char[200];
    int position = -1;
    char* mapQual = new char[200];
    char* CIAGR = new char[200];
    char* MRNM = new char[200];
    char* MPOS = new char[200];
    char* ISIZE = new char[200];
    char* qReadText = new char[2000];
    char* qQualText = new char[2000];


    fscanf(_currFile,"%s %s %s %u %s %s %s %s %s %s %s",qName,flag,tName,&position,mapQual,CIAGR,MRNM,MPOS,ISIZE,qReadText,qQualText);

    char tempChar = fgetc(_currFile);

	while(!feof(_currFile))
    {
        if(tempChar == '\n')
        {
            break;
        }

        tempChar = fgetc(_currFile);
    }

    if(feof(_currFile))
    {
        _isFileEnd = true;
    }

    _qName = qName;
    _flag = flag;
    _tName = tName;
    _position = position;
    _mapQual = mapQual;
	_CIAGR = CIAGR;
	_MRNM = MRNM;
    _MPOS = MPOS;
    _ISIZE = ISIZE;
	_qReadText = qReadText;
	_qQualText = qQualText;

    //int qTextLen = _qReadText.length();
    _qReadMapText =GenerateMappedReadOrQual(qReadText,_CIAGR);
    //int qMapTextLen = _qReadMapText.length();
    _qQualMapText =GenerateMappedReadOrQual(qQualText,_CIAGR);
   // qTextLen = _qReadText.length();
    _isFileOpened = true;

    delete[] qName;
    delete[] flag;
    delete[] tName;
    delete[] mapQual;
    delete[] CIAGR;
    delete[] MRNM;
	delete[] MPOS;
	delete[] ISIZE;
	delete[] qReadText;
	delete[] qQualText;

}

void SAM_file::PrintToTerminal()
{
	cout << _qName << " " << _flag << " " << _tName << " " << _position << " " << _mapQual << " " << _CIAGR;
	cout << _MRNM << " " <<_MPOS <<" " << _ISIZE << " " << _qReadText << " " << _qQualText << "\n";
}

void SAM_file::LoadReadHits()
{


  if(_isReadHitsLoaded )
  {
	return;
  }
  if(!_isFileOpened)
  {
	return;
  }
  _readHitsArray.Clear();
  int numberOfHit = 0;
  this->SetFileToStart();
  ReadNextLine();
  while(!_isFileEnd)
  {


	Read_Hit ReadHitToWrite(_qName,_position, _qReadText, _qQualText,_qReadMapText,_qQualMapText);

    FastqSequenceRecord* readToAdd = new FastqSequenceRecord(_qName,_qReadText,_qQualText);

    ReadHitToWrite.SetCorrespondingRead(readToAdd);

    _readHitsArray.AddToSet(ReadHitToWrite);

    _allReads.Add(readToAdd);

    ReadNextLine();
	numberOfHit++;


  }

  //cout << "numberOfHit=" << numberOfHit << "\n";
  //cout << "_readHitsArray_size" << _readHitsArray.GetSizeOfSet() << "\n";


  _isReadHitsLoaded= 1;
}

std::string SAM_mapping::GetConsensusString(int startCoor,int endCoor)
{
    std::string resultStr = "";

    if(startCoor > endCoor)
    {
        return resultStr;
    }

    for(int i = startCoor; i <= endCoor;i++)
    {
        resultStr = resultStr + this->_refSeq.GetCharOfSequenceTextByNum(i);
    }

    return resultStr;
}

void SAM_file::LoadReadHits(int sizeOfRef)
{


  if(_isReadHitsLoaded )
  {
    return;
  }
  if(_isReadHitsListLoaded )
  {
    return;
  }
  if(!_isFileOpened)
  {
    return;
  }
  _readHitsArray.Clear();
  _readHitsListArray.Clear();
  int numberOfHit = 0;
  this->SetFileToStart();
  ReadNextLine();
	ReadNextLine();
	ReadNextLine();
  cout << "startLoadingHits" << "\n";
  int countSets = sizeOfRef/1000 + 1;
  cout << countSets << "\n";

  for(int i = 0;i<countSets;i++)
  {
      SetOfRead_Hit currSetOfReadHits;
      _readHitsListArray.AddToSet(currSetOfReadHits);
  }


  int first_sector_counter = 0; // потом удалить
  double average_position = 0.0; // потом удалить

  int num_reads_aligned = 0;

  while(!_isFileEnd)
  {

    int currReadHitSet = _position/1000;

		if(numberOfHit % 10000 == 0)
		{
				cout<<"Reads loaded:\t"<<numberOfHit<<"\n";
		}


    if(currReadHitSet == 0) // потом удалить
    {
        if(_position == 0)
        {
          ReadNextLine();
          numberOfHit++;
          continue;
        }
        first_sector_counter++;
        average_position += _position;
    }

    if(this->assembly_statistics->ExistReadName_1(_qName))
    {
        _qName = _qName + std::string("::2");
    }

    else
    {
        _qName = _qName + std::string("::1");
    }
    Read_Hit ReadHitToWrite(_qName,_position, _qReadText, _qQualText,_qReadMapText,_qQualMapText);

    FastqSequenceRecord* readToAdd = new FastqSequenceRecord(_qName,_qReadText,_qQualText);

    ReadHitToWrite.SetCorrespondingRead(readToAdd);

    _readHitsArray.AddToSet(ReadHitToWrite);

    SetOfRead_Hit *rHitsList = _readHitsListArray.GetRefItemByNum(currReadHitSet);

    rHitsList->AddToSet(ReadHitToWrite);

    _allReads.Add(readToAdd);

		this->assembly_statistics->Add(readToAdd, _position);

    ReadNextLine();

    num_reads_aligned++;
    numberOfHit++;


  }

  //cout << "numberOfHit=" << numberOfHit << "\n";
  //cout << "_readHitsArray_size" << _readHitsArray.GetSizeOfSet() << "\n";

  cout << "Number of reads aligned:\t" << num_reads_aligned << " out of " << numberOfHit << "\n";
  //
  // his->assembly_statistics->ComputeProbabilities();
  // cout<<"Statistics computed"<<"\n";
  // his->assembly_statistics->PrintReadLenghtStatistics("read_length_statistics.txt");
  // cout<<"Read_length statistics computed"<<"\n";
  // his->assembly_statistics->PrintPairDistanceStatistics("pair_distances_statistics.txt");
  // cout<<"Pair distance statistics computed"<<"\n";
  // his->assembly_statistics->PrintPositionsDistanceDistribution("position_distance_distribution.txt");
  // cout<<"position_distance_distribution computed"<<"\n";
  // his->assembly_statistics->PrintPositionsDistanceDistributionPE("position_distance_distributionPE.txt");
  // / cout<<this->assembly_statistics->GetNumberOfAllReads()<<"\n"<<this->assembly_statistics->GetNumberOfPairEnded()<<"\n";
  // if(this->assembly_statistics->ProbabilitiesComputed() == true)
  // {
  //   cout << "Probabilities computed\n";
  // }
  //
  // cout << "First sector counter:\t" << first_sector_counter << "\n"; // потом удалить
  // cout << "First sector average_position:\t" << average_position / first_sector_counter << "\n"; // потом удалить

  _isReadHitsLoaded= 1;
  _isReadHitsListLoaded= 1;
}

void SAM_file::LoadReadHits(double percentOfReadLenLowLim)
{


  if(_isReadHitsLoaded )
  {
    return;
  }
  if(!_isFileOpened)
  {
    return;
  }
  _readHitsArray.Clear();
  int numberOfHit = 0;
  this->SetFileToStart();
  ReadNextLine();

  while(!_isFileEnd)
  {

    double percentOfReadLen = (_qReadMapText.length())/(_qReadText.length());
    if(percentOfReadLen >= percentOfReadLenLowLim)
    {
        Read_Hit ReadHitToWrite(_qName,_position, _qReadText, _qQualText,_qReadMapText,_qQualMapText);

        FastqSequenceRecord* readToAdd = new FastqSequenceRecord(_qName,_qReadText,_qQualText);

        ReadHitToWrite.SetCorrespondingRead(readToAdd);

        _readHitsArray.AddToSet(ReadHitToWrite);

        _allReads.Add(readToAdd);
    }

    ReadNextLine();
    numberOfHit++;

  }





  _isReadHitsLoaded= 1;
}


std::string SAM_file::GenerateMappedReadOrQual(std::string qText, std::string ciagrString)
{

	std::string qTextOut = "";
	std::string numberOfLetters = "";
	int positionWrited = 0;
    for(int i = 0;i < ciagrString.length();i++)
    {

        char letter;
        letter = ciagrString[i];

        int numOfLetters = atoi(numberOfLetters.c_str());

        if(letter == '=')
        {
            qTextOut = qText;
            numberOfLetters = "";

            break;
        }
        else
        {
            if(letter == 'X')
            {
                numberOfLetters = "";
                break;
            }
            else
            {
                if(letter == 'I')
                {

                    positionWrited = positionWrited + numOfLetters;
                    numberOfLetters = "";
                    continue;
                }
                else
                {
                    if(letter == 'D')
                    {
                        for(int j = 0;j < numOfLetters;j++)
                        {
                            qTextOut = qTextOut + '-';
                        }
                        numberOfLetters = "";
                        continue;
                    }
                    else
                    {
                        if(letter == 'S')
                        {
                            positionWrited = positionWrited + numOfLetters;
                            numberOfLetters = "";
                            continue;
                        }
                        else
                        {
                            if(letter == 'N')
                            {
                                positionWrited = positionWrited + numOfLetters;
                                numberOfLetters = "";
                                continue;
                            }
                            else
                            {
                                if(letter == 'M')
                                {
                                    qTextOut = qTextOut + qText.substr(positionWrited,numOfLetters);
                                    positionWrited = positionWrited + numOfLetters;
                                    numberOfLetters = "";
                                    continue;
                                }
                                else
                                {
                                    numberOfLetters = numberOfLetters + letter;
                                    continue;
                                }
                            }
                        }
                    }
                }
            }

        }

    }

    return qTextOut;
}

int SAM_mapping::CountNumOfLetterInReadsAtRefPos(char letter,int position)
{


		SetOfRead_Hit* readHits;

		if(_samFile.IsReadHitsListLoaded())
		{
				readHits = (_samFile.GetSetOfReadHitsAtPos((int)(position/(1000))));
		}
		else
		{
				readHits = &_samFile.GetSetOfReadHits();
		}
		int numberOfHits = readHits->GetSizeOfSet();

		int countOfLetter = 0;

		for(int i = 0; i < numberOfHits;i++)
		{
				if(readHits->GetItemByNum(i).GetPosByNumInMap(position) == letter)
				{
						countOfLetter++;
				}
		}

		return countOfLetter;
}

int SAM_mapping::CountDeletionsOfLettersInReadsAtRefPos(int position)
{
int countOfLetter = 0;

		/*

		SetOfRead_Hit readHits = _samFile.GetSetOfReadHits();

		int numberOfHits = readHits.GetSizeOfSet();

		int countOfLetter = 0;
		char currLetterMap = readHits.GetItemByNum(i).GetPosByNumInMap(position);
		char currLetterInRead =

		for(int i = 0; i < numberOfHits;i++)
		{

				for(j = position+1;j<readHits.GetItemByNum(i).GetQText().length();j++)
				{
						if(readHits.GetItemByNum(i).GetPosByNumInMap(j) == letter)
						{
								countOfLetter++;
						}
				}
		}

		return countOfLetter;*/
return countOfLetter;
}

int SAM_mapping::CountCoverageAtThisPos(int position)
{
		SetOfRead_Hit* readHits;

		int coorTest = (int)(position/1000);
		//cout <<"position-" << position << " coorTest-" << coorTest << "\n";
		if(_samFile.IsReadHitsListLoaded())
		{
				readHits = (_samFile.GetSetOfReadHitsAtPos((int)(position/(1000))));
		}
		else
		{
				readHits = &_samFile.GetSetOfReadHits();
		}

		//cout << "readHits position -" << position << " length-" <<readHits.GetSizeOfSet() << "\n";
		//SetOfRead_Hit readHits = _samFile.GetSetOfReadHits();

		int numberOfHits = readHits->GetSizeOfSet();

		int countOfLetter = 0;
    // cout<<"numberOfHits"<<numberOfHits<<"\n";

		for(int i = 0; i < numberOfHits;i++)
		{
				if(readHits->GetItemByNum(i).GetPosByNumInMap(position) != 'N')
				{
						countOfLetter++;
				}
		}

		return countOfLetter;
}

int FindMax(double *arrVal, int size)
{
		double maxVal = -1;
		int maxI = -1;

		for(int i = 0;i<size;i++)
		{
				if(*(arrVal+i)> maxVal)
				{
						maxI = i;
						maxVal = *(arrVal+i);
				}
		}

		return maxI;
}

void ProcessingCountMaxOutput3(double &count1, double &count2, double &count3)
{
		double *arrForCount3 = new double[3];

		arrForCount3[0] = count1;
		arrForCount3[1] = count2;
		arrForCount3[2] = count3;


		int indexOfMax = FindMax(arrForCount3,3);



		if(indexOfMax == 0)
		{
				count2 = 0.0;
				count3 = 0.0;
		}
		else if(indexOfMax == 1)
		{
				count1 = 0.0;
				count3 = 0.0;
		}
		else if(indexOfMax == 2)
		{
				count1 = 0.0;
				count2 = 0.0;
		}

		delete[] arrForCount3;

}


void ProcessingCountMaxOutput4(double &count1, double &count2, double &count3, double &count4)
{
		double *arrForCount4 = new double[4];
		arrForCount4[0] = count1;
		arrForCount4[1] = count2;
		arrForCount4[2] = count3;
		arrForCount4[3] = count4;

		int indexOfMax = FindMax(arrForCount4,4);

		if(indexOfMax == 0)
		{
				ProcessingCountMaxOutput3(count2,count3,count4);
		}
		else if(indexOfMax == 1)
		{
				ProcessingCountMaxOutput3(count1,count3,count4);
		}
		else if(indexOfMax == 2)
		{
				ProcessingCountMaxOutput3(count1,count2,count4);
		}
		else if(indexOfMax == 3)
		{
				ProcessingCountMaxOutput3(count1,count2,count3);
		}




		delete[] arrForCount4;




}

void ProcessingCountMaxOutput5(double &count1, double &count2, double &count3, double &count4, double &count5)
{
		double *arrForCount5 = new double[5];
		arrForCount5[0] = count1;
		arrForCount5[1] = count2;
		arrForCount5[2] = count3;
		arrForCount5[3] = count4;
		arrForCount5[4] = count5;

		int indexOfMax = FindMax(arrForCount5,5);

		if(indexOfMax == 0)
		{
				ProcessingCountMaxOutput4(count2,count3,count4,count5);
		}
		else if(indexOfMax == 1)
		{
				ProcessingCountMaxOutput4(count1,count3,count4,count5);
		}
		else if(indexOfMax == 2)
		{
				ProcessingCountMaxOutput4(count1,count2,count4,count5);
		}
		else if(indexOfMax == 3)
		{
				ProcessingCountMaxOutput4(count1,count2,count3,count5);
		}
		else if(indexOfMax == 4)
		{
				ProcessingCountMaxOutput4(count1,count2,count3,count4);
		}



		delete[] arrForCount5;




}



short SAM_mapping::FindWhatLetterIsCorrectAtThisPos(int position)
{
		short correctLetter = 0;

		int Acount = 0;
		int Tcount = 0;
		int Gcount = 0;
		int Ccount = 0;
		int Inscount = 0;




		Acount = CountNumOfLetterInReadsAtRefPos('A',position);
		Tcount = CountNumOfLetterInReadsAtRefPos('T',position);
		Gcount = CountNumOfLetterInReadsAtRefPos('G',position);
		Ccount = CountNumOfLetterInReadsAtRefPos('C',position);
		Inscount = CountNumOfLetterInReadsAtRefPos('-',position);


		double percentAcount = 0;
		double percentTcount = 0;
		double percentGcount = 0;
		double percentCcount = 0;
		double percentInscount = 0;


		int sumOfMappedReads = Acount + Tcount + Gcount + Ccount + Inscount;
		if(sumOfMappedReads > 0)
		{
				percentAcount = (double)Acount/(double)sumOfMappedReads;
				percentTcount = (double)Tcount/(double)sumOfMappedReads;
				percentGcount = (double)Gcount/(double)sumOfMappedReads;
				percentCcount = (double)Ccount/(double)sumOfMappedReads;
				percentInscount = (double)Inscount/(double)sumOfMappedReads;

		}

	 // ProcessingCountMaxOutput5(percentAcount,percentTcount,percentGcount,percentCcount,percentInscount);


 //cout << "percentAcount=" << percentAcount << "\n";
 //cout << "percentTcount=" << percentTcount << "\n";
 //cout << "percentGcount=" << percentGcount << "\n";
 //cout << "percentCcount=" << percentCcount << "\n";

	 // cout << "_limCountToConsiderDiploid " << _limCountToConsiderDiploid << "\n";
		if(percentAcount > _limCountToConsiderDiploid)
		{
			 // cout << "percentAcount " << percentAcount << "\n";
				correctLetter = correctLetter | OnlyA;

		}

		if(percentTcount > _limCountToConsiderDiploid)
		{
				//cout << "percentTcount " << percentTcount << "\n";
				correctLetter = correctLetter | OnlyT;
		}

		if(percentGcount > _limCountToConsiderDiploid)
		{
				//cout << "percentGcount " << percentGcount << "\n";
				correctLetter = correctLetter | OnlyG;
		}

		if(percentCcount > _limCountToConsiderDiploid)
		{
				//cout << "percentCcount " << percentCcount << "\n";
				correctLetter = correctLetter | OnlyC;

		}

		if(percentInscount > _limCountToConsiderDiploid)
		{
				//cout << "percentInscount " << percentInscount << "\n";
				correctLetter = correctLetter | OnlyIns;

		}





		return correctLetter;

}


UniqueReadListRef SAM_mapping::GetListOfReadsWithThisLetterAtThisPos(char letter, int pos)
{


		//int numberOfReadsFound = 0;
		UniqueReadListRef readsList;

		SetOfRead_Hit* readHitsList;

		if(_samFile.IsReadHitsListLoaded())
		{
				readHitsList = (_samFile.GetSetOfReadHitsAtPos((int)(pos/(1000))));
		}
		else
		{
				readHitsList = &_samFile.GetSetOfReadHits();
		}

		//SetOfRead_Hit readHitsList = _samFile.GetSetOfReadHits();

		int readHitsListSize = readHitsList->GetSizeOfSet();


		for(int i = 0; i < readHitsListSize;i++)
		{

				if(readHitsList->GetItemByNum(i).GetPosByNumInMap(pos) == letter)
				{
						// FastqSequenceRecord* seqRecord(readHitsList.GetItemByNum(i).GetQName(),readHitsList.GetItemByNum(i).GetQText(),readHitsList.GetItemByNum(i).GetQQual());
						FastqSequenceRecord* seqRecord = readHitsList->GetItemByNum(i).GetCorrespondingRead();

						readsList.Add(seqRecord);
				}
		}
		return readsList;
}




int SAM_mapping::PrintMappingToFile(char *fileName)
{

		std::fstream fileToWrite(fileName,std::fstream::out);

		SetOfRead_Hit setReadHits = _samFile.GetSetOfReadHits();

		int sizeOfNumberReadHits = setReadHits.GetSizeOfSet();
		int lenOfRef = _refSeq.GetSequenceText().length();



		int maxName = -1;
		std::string seqNam_1 = _refSeq.GetSequenceName();
		int lenSeqNam = seqNam_1.length();
		if(lenSeqNam > maxName)
		{
				maxName = _refSeq.GetSequenceName().length();
		}

		std::string debugQName_max = "";
		std::string debugQText_max = "";
		std::string debugQMapText_max = "";
		int i_max = 0;
		for(int i = 0;i<sizeOfNumberReadHits;i++)
		{
				std::string debugQName = setReadHits.GetItemByNum(i).GetQName();
				std::string debugQText = setReadHits.GetItemByNum(i).GetQText();
				std::string debugQMapText = setReadHits.GetItemByNum(i).GetQMapText();
				if(i==6932)
				{
						int a = 0;
						a++;
				}
				if(setReadHits.GetItemByNum(i).GetQName().length() > maxName)
				{
						i_max = i;

						debugQName_max = setReadHits.GetItemByNum(i).GetQName();
						debugQText_max = setReadHits.GetItemByNum(i).GetQText();
						debugQMapText_max = setReadHits.GetItemByNum(i).GetQMapText();

						maxName = setReadHits.GetItemByNum(i).GetQName().length();
				}
		}

		fileToWrite << _refSeq.GetSequenceName();

		int diff = maxName - _refSeq.GetSequenceName().length() + 1;

		for(int k = 0; k < diff;k++)
		{
				fileToWrite << " ";
		}


		for(int j = 0;j<lenOfRef;j++)
		{
				fileToWrite << _refSeq.GetCharOfSequenceTextByNum(j);
		}

		fileToWrite <<"\n";



		for(int i = 0;i<sizeOfNumberReadHits;i++)
		{
				fileToWrite << setReadHits.GetItemByNum(i).GetQName();

				diff = maxName - setReadHits.GetItemByNum(i).GetQName().length() + 1;

				for(int k = 0; k < diff;k++)
				{
						fileToWrite << " ";
				}

				for(unsigned int j=1;j<=lenOfRef;j++)
				{
						fileToWrite << setReadHits.GetItemByNum(i).GetPosByNumInMap(j);
				}
				fileToWrite << "\n";
		}

		fileToWrite.close();

		return AllOk;



}

int SAM_mapping::CountCoverage(std::string avFileNameOutput, std::string covTableFileNameOutput)
{

		int sizeOfRef = this->_refSeq.GetSequenceText().length();
		double avCovLim = 0.0;

		fstream fileToWrite(covTableFileNameOutput.c_str(),fstream::out);








		for(int i =1;i <= sizeOfRef;i++)
		{
				int currCov = CountCoverageAtThisPos(i);

				fileToWrite << i << " "<< currCov << "\n";
				avCovLim += currCov;
		}

		avCovLim = avCovLim/sizeOfRef;

		 fileToWrite.close();





		fstream fileToWrite1(avFileNameOutput.c_str(),fstream::out);

		fileToWrite1 << avCovLim << "\n";

		fileToWrite1.close();



}

void WriteMovingAverageToFile(std::vector<double> moving_average, std::vector<int> positions, std::string out_file)
{
    if(moving_average.size() != positions.size())
    {
        return;
    }
    ofstream my_file_debug;
    my_file_debug.open(out_file.c_str(),std::ios::app);
    for(int i = 0; i < moving_average.size(); i++)
    {
        my_file_debug << positions.at(i) << "," << moving_average.at(i) << "\n";
    }
    my_file_debug.close();
}


int SAM_mapping::FindGaploidsAlgorithm(std::string fileGraph,std::string fileSimpleGraph, std::string resultFileName, std::string fileDebugFileName, int num_of_chr)
{


		cout << "We are here 1\n";
    if(this->_samFile.assembly_statistics->ProbabilitiesComputed() == true)
    {
      cout << "Probabilities computed\n";
    }
		int sizeOfRef = this->_refSeq.GetSequenceText().length();
		cout << sizeOfRef <<"\n";
		//sizeOfRef = 10000;

		short lettersInThisPos = 0;

		//fstream allConflictPointsFile(allConflictPoints.c_str(),fstream::out);
        ofstream my_file_debug(std::string("watching_graph.txt").c_str(),std::ios::out);
		fstream fileStreamGraph(fileGraph.c_str(),fstream::out);
		fstream fileStreamSimpleGraph(fileSimpleGraph.c_str(),fstream::out);
		fstream fileStreamDebug(fileDebugFileName.c_str(),fstream::out);

		Graph *currGraph = new Graph();
        HashStringVertexPointer* vertexies_name_table = new HashStringVertexPointer();


		int i_last = -1;
        double avCovLim = 70;

        double average_coverage = 0;
        double coverage_variance = 0;
        int counter = 0;
        // double avCovSqrLim = 0;
        // for(int i = sizeOfRef / 100; i < 2 * sizeOfRef / 100; i++)
        // {
        //     int currentCoverage = CountCoverageAtThisPos(i);
        //     coverage_distribution[currentCoverage] += 1;
        //     avCovLim += currentCoverage;
        //     avCovSqrLim += currentCoverage * currentCoverage;
        // }
        // avCovLim /= sizeOfRef / 100;
        // double variance = avCovSqrLim / (sizeOfRef / 100 - 1) - (sizeOfRef / 100) / ((sizeOfRef / 100) - 1) * avCovLim * avCovLim;
        // PrintCoverageDistributionToFile(coverage_distribution, "/home/kemelyanov/Diploma/Generate_Polyploids/coverage_distribution.csv");
        // double avCovLim = 40.0 * sizeOfRef; // нужно будет раскоментить нижние строки, это сделано для ускорения

		cout << "We are here 2\n";
		// avCovLim = avCovLim/sizeOfRef;
		// avCovLim = avCovLim*_fractionOfAvCov;
		cout << avCovLim <<"\n";

		cout << "sizeOfRef="<< sizeOfRef<<"\n";

		SetOfRef<Vertex> lastVertexies;

        double moving_average = 1.0;
        double alpha_param = 0.4;
        std::vector<double> moving_averages_in_pos;
        std::vector<int> positions_vector;

        std::vector<double> coverage_local;
        std::vector<double> probabilities_of_coverage_changes;
        remove("watching_graph");
        remove("moving_average.csv");
        remove("local_coverage.csv");
        remove("probabilities_for_local_coverage_to_change.csv");
        double my_threshold = 0.01;
		// for(int i =sizeOfRef / 30; i <= sizeOfRef / 30 + sizeOfRef / 50; i++)
        for(int i = 1000; i <= 200000; i++)
		{

                if(i % 1000 == 0)
                {
                  cout << "Position in genome:\t" << i << "\n";
                  cout << "Moving average is:\t" << moving_average << "\n";
                  my_file_debug.close();
                  my_file_debug.open(std::string("watching_graph.txt").c_str(),std::ios::app);
                  WriteMovingAverageToFile(moving_averages_in_pos, positions_vector, "moving_average.csv");
                  WriteMovingAverageToFile(coverage_local, positions_vector, "local_coverage.csv");
                  WriteMovingAverageToFile(probabilities_of_coverage_changes, positions_vector, "probabilities_for_local_coverage_to_change.csv");
                  coverage_local.clear();
                  moving_averages_in_pos.clear();
                  probabilities_of_coverage_changes.clear();
                  positions_vector.clear();
                }

				int currCov = CountCoverageAtThisPos(i);
                moving_average = moving_average * (1.0 - alpha_param) + alpha_param * currCov;
                positions_vector.push_back(i);
                moving_averages_in_pos.push_back(moving_average);
                coverage_local.push_back(currCov);
                boost::math::poisson poisson_var(moving_average);
                if(moving_average >= currCov)
                {
                    probabilities_of_coverage_changes.push_back(boost::math::cdf(poisson_var, currCov));
                }
                else
                {
                    probabilities_of_coverage_changes.push_back(1.0 - boost::math::cdf(poisson_var, currCov));
                }
                if(probabilities_of_coverage_changes.at(probabilities_of_coverage_changes.size() - 1) <= my_threshold && moving_average < currCov)
                {
                    lastVertexies.Clear();
                    average_coverage += currCov;
                    coverage_variance += currCov * currCov;
                    counter++;
                    continue;
                }
        // cout << "Coverage at this pos is computed\n";

				lettersInThisPos = 0;
				lettersInThisPos = FindWhatLetterIsCorrectAtThisPos(i);

        // cout << "correct letter is computed\n";
				if((lettersInThisPos == OnlyA) || (lettersInThisPos == OnlyC) || (lettersInThisPos == OnlyG)
								|| (lettersInThisPos == OnlyT) || (lettersInThisPos == OnlyIns) || (lettersInThisPos == 0))
				{
						continue;
				}
				SetOfRef<Vertex> currVertexies;

				if(lettersInThisPos & OnlyA)
				{
						UniqueReadListRef tempReadList = GetListOfReadsWithThisLetterAtThisPos('A',i);
    						std::string tempLetter = "";
    						char *tempBuf = new char[100];
    						itoa(i,tempBuf,10);
    						tempLetter =tempLetter + "A_" + tempBuf;
    						SetOf<ConnVertex> conVerFrom;
    						SetOf<ConnVertex> conVerTo;
    						ConnVertex connVerVal_From;
    						connVerVal_From._connVertex = NULL;
    						ConnVertex connVerVal_To;
    						connVerVal_To._connVertex = NULL;
    						conVerFrom.AddToSet(connVerVal_From);
    						conVerTo.AddToSet(connVerVal_To);

    						Vertex *currVertex = new Vertex(tempReadList,tempLetter,"A",i,conVerFrom,conVerTo);
                            (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            currVertexies.AddToSet(currVertex);
                            // boost::math::poisson temp_poisson(moving_average / 4);
                            // int num_reads = tempReadList.GetReadList().GetSizeOfSet();
                            // double probability;
                            // if(num_reads >= moving_average / 4)
                            // {
                            //     // probability = 1 - boost::math::cdf(temp_poisson, num_reads);
                            //     probability = 1;
                            // }
                            // else
                            // {
                            //     probability = boost::math::cdf(temp_poisson, num_reads);
                            // }
                            //
                            // if(probability < my_threshold && num_reads <= 2)
                            // {
                            //     delete currVertex;
                            // }
                            // else
                            // {
                            //     (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            //     currVertexies.AddToSet(currVertex);
                            // }
                            delete[] tempBuf;
				}

				if(lettersInThisPos & OnlyT)
				{
						UniqueReadListRef tempReadList = GetListOfReadsWithThisLetterAtThisPos('T',i);
    						std::string tempLetter = "";
    						char *tempBuf = new char[100];
    						itoa(i,tempBuf,10);
    						tempLetter =tempLetter + "T_" + tempBuf;
    						SetOf<ConnVertex> conVerFrom;
    						SetOf<ConnVertex> conVerTo;
    						ConnVertex connVerVal_From;
    						connVerVal_From._connVertex = NULL;
    						ConnVertex connVerVal_To;
    						connVerVal_To._connVertex = NULL;
    						conVerFrom.AddToSet(connVerVal_From);
    						conVerTo.AddToSet(connVerVal_To);

    						Vertex *currVertex = new Vertex(tempReadList,tempLetter,"T",i,conVerFrom,conVerTo);
                            (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            currVertexies.AddToSet(currVertex);
                            // boost::math::poisson temp_poisson(moving_average / 4);
                            // int num_reads = tempReadList.GetReadList().GetSizeOfSet();
                            // double probability;
                            // if(num_reads >= moving_average / 4)
                            // {
                            //     // probability = 1 - boost::math::cdf(temp_poisson, num_reads);
                            //     probability = 1;
                            // }
                            // else
                            // {
                            //     probability = boost::math::cdf(temp_poisson, num_reads);
                            // }
                            //
                            // if(probability < my_threshold && num_reads <= 2)
                            // {
                            //     delete currVertex;
                            // }
                            // else
                            // {
                            //     (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            //     currVertexies.AddToSet(currVertex);
                            // }
    						delete[] tempBuf;
				}

				if(lettersInThisPos & OnlyC)
				{
                        UniqueReadListRef tempReadList = GetListOfReadsWithThisLetterAtThisPos('C',i);
    						std::string tempLetter = "";
    						char *tempBuf = new char[100];
    						itoa(i,tempBuf,10);
    						tempLetter =tempLetter + "C_" + tempBuf;
    						SetOf<ConnVertex> conVerFrom;
    						SetOf<ConnVertex> conVerTo;
    						ConnVertex connVerVal_From;
    						connVerVal_From._connVertex = NULL;
    						ConnVertex connVerVal_To;
    						connVerVal_To._connVertex = NULL;
    						conVerFrom.AddToSet(connVerVal_From);
    						conVerTo.AddToSet(connVerVal_To);

    						Vertex *currVertex = new Vertex(tempReadList,tempLetter,"C",i,conVerFrom,conVerTo);
                            (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            currVertexies.AddToSet(currVertex);

                            // boost::math::poisson temp_poisson(moving_average / 4);
                            // int num_reads = tempReadList.GetReadList().GetSizeOfSet();
                            // double probability;
                            // if(num_reads >= moving_average / 4)
                            // {
                            //     // probability = 1 - boost::math::cdf(temp_poisson, num_reads);
                            //     probability = 1;
                            // }
                            // else
                            // {
                            //     probability = boost::math::cdf(temp_poisson, num_reads);
                            // }
                            //
                            // if(probability < my_threshold && num_reads <= 2)
                            // {
                            //     delete currVertex;
                            // }
                            // else
                            // {
                            //     (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            //     currVertexies.AddToSet(currVertex);
                            // }
                            delete[] tempBuf;
				}

				if(lettersInThisPos & OnlyG)
				{
                        UniqueReadListRef tempReadList = GetListOfReadsWithThisLetterAtThisPos('G',i);
    						std::string tempLetter = "";
    						char *tempBuf = new char[100];
    						itoa(i,tempBuf,10);
    						tempLetter =tempLetter + "G_" + tempBuf;
    						SetOf<ConnVertex> conVerFrom;
    						SetOf<ConnVertex> conVerTo;
    						ConnVertex connVerVal_From;
    						connVerVal_From._connVertex = NULL;
    						ConnVertex connVerVal_To;
    						connVerVal_To._connVertex = NULL;
    						conVerFrom.AddToSet(connVerVal_From);
    						conVerTo.AddToSet(connVerVal_To);

    						Vertex *currVertex = new Vertex(tempReadList,tempLetter,"G",i,conVerFrom,conVerTo);
                            (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            currVertexies.AddToSet(currVertex);

                            // boost::math::poisson temp_poisson(moving_average / 4);
                            // int num_reads = tempReadList.GetReadList().GetSizeOfSet();
                            // double probability;
                            // if(num_reads >= moving_average / 4)
                            // {
                            //     // probability = 1 - boost::math::cdf(temp_poisson, num_reads);
                            //     probability = 1;
                            // }
                            // else
                            // {
                            //     probability = boost::math::cdf(temp_poisson, num_reads);
                            // }
                            //
                            // if(probability < my_threshold && num_reads <= 2)
                            // {
                            //     delete currVertex;
                            // }
                            // else
                            // {
                            //     (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            //     currVertexies.AddToSet(currVertex);
                            // }
                            delete[] tempBuf;
				}

				if(lettersInThisPos & OnlyIns)
				{
                    UniqueReadListRef tempReadList = GetListOfReadsWithThisLetterAtThisPos('-',i);
    						char *tempBuf = new char[100];
                            std::string tempLetter = "";
    						itoa(i,tempBuf,10);
    						tempLetter =tempLetter + "Ins_" + tempBuf;
    						SetOf<ConnVertex> conVerFrom;
    						SetOf<ConnVertex> conVerTo;
    						ConnVertex connVerVal_From;
    						connVerVal_From._connVertex = NULL;
    						ConnVertex connVerVal_To;
    						connVerVal_To._connVertex = NULL;
    						conVerFrom.AddToSet(connVerVal_From);
    						conVerTo.AddToSet(connVerVal_To);

    						Vertex *currVertex = new Vertex(tempReadList,tempLetter,"-",i,conVerFrom,conVerTo);
                            (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            currVertexies.AddToSet(currVertex);
                            // boost::math::poisson temp_poisson(moving_average / 4);
                            // int num_reads = tempReadList.GetReadList().GetSizeOfSet();
                            // double probability;
                            // if(num_reads >= moving_average / 4)
                            // {
                            //     // probability = 1 - boost::math::cdf(temp_poisson, num_reads);
                            //     probability = 1;
                            // }
                            // else
                            // {
                            //     probability = boost::math::cdf(temp_poisson, num_reads);
                            // }
                            //
                            // if(probability < my_threshold && num_reads <= 2)
                            // {
                            //     delete currVertex;
                            // }
                            // else
                            // {
                            //     (*vertexies_name_table)[currVertex->GetName()] = currVertex;
                            //     currVertexies.AddToSet(currVertex);
                            // }
                            delete[] tempBuf;
				}
			 // cout << "\n";
			 // allConflictPointsFile << "\n";
                // if(currVertexies.GetSizeOfSet() == 1)
                // {
                //     delete currVertexies.GetItemByNum(0);
                //     currVertexies.Clear();
                // }
				if(lastVertexies.GetSizeOfSet()>0)
				{
            // cout<<"\n"<<"weights:"<<"\n";
						for(int j = 0;j<lastVertexies.GetSizeOfSet();j++)
						{
								UniqueReadListRef currLastVertex = GetListOfReadsWithThisLetterAtThisPos(lastVertexies.GetItemByNum(j)->GetSymbol().c_str()[0],lastVertexies.GetItemByNum(j)->GetCoor());
								my_file_debug << "[ " << currLastVertex.GetReadList().GetSizeOfSet() << " ]" << lastVertexies.GetItemByNum(j)->GetCoor() << "_" << lastVertexies.GetItemByNum(j)->GetSymbol();
								for(int m = 0;m<currVertexies.GetSizeOfSet();m++)
								{
										UniqueReadListRef currCurrVertex = GetListOfReadsWithThisLetterAtThisPos(currVertexies.GetItemByNum(m)->GetSymbol().c_str()[0],currVertexies.GetItemByNum(m)->GetCoor());

										int weight = currCurrVertex.FindIntersectionOfLists(currLastVertex.GetReadList());
                    // cout<<weight<<"\n";
										if(weight > 0)
										{
												lastVertexies.GetItemByNum(j)->ConnectToVertex(currVertexies.GetItemByNum(m),weight);
												currVertexies.GetItemByNum(m)->ConnectFromVertex(lastVertexies.GetItemByNum(j),weight);
                        my_file_debug << " =" << "(" << weight << ")" << "=> " << currVertexies.GetItemByNum(m)->GetCoor()  << "_" << currVertexies.GetItemByNum(m)->GetSymbol() << ";";
										}

								}
								my_file_debug <<"\n";
						}

            my_file_debug <<"================================================================================================\n";

				}

				for(int j=0;j<currVertexies.GetSizeOfSet();j++)
				{
						//cout << "Add Vertex Start\n";
						currGraph->AddVertex(currVertexies.GetItemByNum(j));
						//cout << "Add Vertex End\n";
				}
				lastVertexies = currVertexies;
                average_coverage += currCov;
                coverage_variance += currCov * currCov;
                counter++;
		}

        average_coverage /= counter;
        coverage_variance = (coverage_variance - average_coverage * average_coverage) / (counter - 1);
        cout << "average_coverage is:\t" << average_coverage << "\n";
        cout << "coverage standard deviation is:\t" << sqrt(coverage_variance) << "\n";
		cout << "before graph construction\n";
		// SimplifyGraph *simplGraph = new SimplifyGraph();

		currGraph->FillFirstVertexies();
		// currGraph->PrintGraph(fileStreamGraph);
		cout << "after printing source graph\n";
		cout << "count first vertexies - " << currGraph->GetFirstVertexiesSize()<< "\n";

        PathStack::GenerateInitialPathStacks(currGraph, num_of_chr, vertexies_name_table, "all_paths_with_score.txt");

        cout << "Going to delete vertexies name table\n";
        delete vertexies_name_table;

        system( "python generate_output_for_artem.py" );
        system("python make_csv_results.py");
        cout << "That is all!\n";
		return AllOk;
}


int SAM_mapping::CountStats(std::string fileNameStats)
{
		int sizeOfRef = this->_refSeq.GetSequenceText().length();
		short lettersInThisPos = 0;

		int countAbove100 = 0;
		int countBelow100 = 0;

		int i_last = -1;

		for(int i =0;i < sizeOfRef;i++)
		{
				lettersInThisPos = 0;

				lettersInThisPos = FindWhatLetterIsCorrectAtThisPos(i);

				if((lettersInThisPos == OnlyA) || (lettersInThisPos == OnlyC) || (lettersInThisPos == OnlyG)
								|| (lettersInThisPos == OnlyT) || (lettersInThisPos == 0))
				{
						continue;
				}

				if(i_last > 0)
				{
						int diff = i - i_last;

						if(diff > 100)
						{
								countAbove100++;
						}
						else
						{
								countBelow100++;
						}

						//i_last = i;
				}

				i_last = i;

				cout << "above100 - " << countAbove100 << "\n";
				cout << "below100 - " << countBelow100 << "\n";


		}

		std::fstream fileToWrite(fileNameStats.c_str(),std::fstream::out);

		fileToWrite << "above100 - " << countAbove100 << "\n";
		fileToWrite << "below100 - " << countBelow100 << "\n";

		fileToWrite.close();


		return AllOk;
}
