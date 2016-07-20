#ifndef SAM_MAPPING_H
#define SAM_MAPPING_H

#include <string>
#include <stdlib.h>
#include "FilesFormats.h"
#include "connectioncomponent.h";
#include "Graph.h"
#include "AssemblyStatistics.h"
#include "Path.h"
#include <boost/math/distributions/poisson.hpp>


typedef __gnu_cxx::hash_map<int, int> HashIntInt;
typedef std::map<std::string, Vertex*> HashStringVertexPointer;

const short OnlyA = 1;
const short OnlyC = 2;
const short OnlyG = 4;
const short OnlyT = 8;
const short OnlyIns = 16;


class Deletion
{
private:
		std::string _delText;
		int _startPosInQuery;
		int _startPosInDb;
		int _lengthDelTest;

public:
		static Deletion Empty()
		{
				return Deletion();
		}
		Deletion()
		{
				_delText = "";
				_startPosInQuery = -1;
				_startPosInDb = -1;
				_lengthDelTest = 0;
		}

		Deletion(std::string delText, int startPosInQuery, int startPosInDb)
		{
				_delText = delText;
				_startPosInQuery = startPosInQuery;
				_startPosInDb = startPosInDb;
				_lengthDelTest = delText.length();
		}

		std::string GetDelText()
		{
				return _delText;
		}

		int GetStartPosInQuery()
		{
				return _startPosInQuery;
		}

		int GetStartPosInDb()
		{
				return _startPosInDb;
		}

		int GetLengthDelTest()
		{
				return _lengthDelTest;
		}
};

typedef SetOf<Deletion> SetOfDeletions;

class Read_Hit
{
private:
		std::string _qName;
		int _tStart;
		int _tEnd;
		std::string _qText;
		std::string _qQual;
		std::string _qMapText;
		std::string _qMapQual;
		SetOfDeletions _deletions;

		FastqSequenceRecord* _correspondingRead;

		int lengthOfRead;
		int lengthOfMap;

public:

		static Read_Hit Empty()
		{
				return Read_Hit();
		}

		Read_Hit()
		{
				_qName = "";
				_tStart = -1;
				_tEnd = -1;
				_qText = "";
				_qQual = "";
				_qMapText = "";
				_qMapQual = "";
				lengthOfRead = 0;
				lengthOfMap = 0;
				_correspondingRead = NULL;
		}
		Read_Hit(std::string qName, int tStart, std::string qText, std::string qQual,std::string qMapText,std::string qMapQual)
		{
				_qName = qName;
				_qText = qText;
				_qQual = qQual;
				_qMapText = qMapText;
				_qMapQual = qMapQual;
				lengthOfRead = _qText.length();
				lengthOfMap = _qMapText.length();
				_tEnd = tStart + lengthOfMap;
				_tStart = tStart;
				_correspondingRead = NULL;
		}
		Read_Hit(std::string qName, int tStart, std::string qText, std::string qQual,std::string qMapText,std::string qMapQual,SetOfDeletions &dels)
		{
				_qName = qName;
				_qText = qText;
				_qQual = qQual;
				_qMapText = qMapText;
				_qMapQual = qMapQual;
				lengthOfRead = _qText.length();
				lengthOfMap = _qMapText.length();
				_tEnd = tStart + lengthOfMap;
				_tStart = tStart;
				_correspondingRead = NULL;
				for(int i = 0;i < dels.GetSizeOfSet();i++)
				{
						_deletions.AddToSet(dels.GetItemByNum(i));
				}

		}

		void Set(std::string qName, int tStart, std::string qText, std::string qQual,std::string qMapText,std::string qMapQual,SetOfDeletions &dels)
		{
				_qName = qName;
				_tStart = tStart;
				_qText = qText;
				_qQual = qQual;
				_qMapText = qMapText;
				_qMapQual = qMapQual;
				lengthOfRead = _qText.length();
				lengthOfMap = _qMapText.length();
				_tEnd = tStart + lengthOfMap;
				_deletions.Clear();
				for(int i = 0;i<dels.GetSizeOfSet();i++)
				{
						_deletions.AddToSet(dels.GetItemByNum(i));
				}

		}

		void SetCorrespondingRead(FastqSequenceRecord* read)
		{
				_correspondingRead = read;
		}

		std::string GetQName()
		{
				return _qName;
		}



		int GetTStart()
		{
				return _tStart;
		}



		int GetTEnd()
		{
				return _tEnd;
		}



		std::string GetQText()
		{
				return _qText;
		}



		std::string GetQQual()
		{
				return _qQual;
		}



		std::string GetQMapText()
		{
				return _qMapText;
		}



		std::string GetQMapQual()
		{
				return _qMapQual;
		}

		int GetNumberOfDeletions()
		{
				return _deletions.GetSizeOfSet();
		}

		Deletion GetDeletionByNum(int index)
		{
				return _deletions.GetItemByNum(index);
		}




		char GetPosByNum(unsigned int pos)
		{
				if(pos >= 0 && pos < _qText.length())
				{
						return _qText[pos];
				}
				else
				{
						return 'N';
				}
		}

		std::string GetDeletionTextAtPos(unsigned int pos)
		{
				for(int i = 0;i < _deletions.GetSizeOfSet();i++)
				{
						if(_deletions.GetItemByNum(i).GetStartPosInQuery() == pos )
						{
								return _deletions.GetItemByNum(i).GetDelText();
						}
				}

				return "";
		}

		std::string GetDeletionTextAtPosInMap(unsigned int pos)
		{
				pos++;
				for(int i = 0;i < _deletions.GetSizeOfSet();i++)
				{

						if(_deletions.GetItemByNum(i).GetStartPosInDb() == pos )
						{
								return _deletions.GetItemByNum(i).GetDelText();
						}

				}

				if((_tStart < pos) && (_tEnd > pos))
				{
						return "-";
				}


				return "";
		}

		char GetPosByNumInMap(unsigned int pos)
		{
				if(pos >= (_tStart) && pos < _tEnd)
				{
						return _qMapText[pos-_tStart];
				}
				else
				{
						return 'N';
				}
		}

		FastqSequenceRecord* GetCorrespondingRead()
		{
				return _correspondingRead;
		}

		bool operator ==(const Read_Hit& b) const
		{
				if((b._qName == _qName) && (b._tStart == _tStart) && (b._tEnd == _tEnd) && (b._qText == _qText) && (b._qQual == _qQual) && (b._qMapText == _qMapText) && (b._qMapQual == _qMapQual))
				{

						return true;
				}

				return false;
		}

		bool operator !=(const Read_Hit& b) const
		{
				if((b._qName == _qName) && (b._tStart == _tStart) && (b._tEnd == _tEnd) && (b._qText == _qText) && (b._qQual == _qQual) && (b._qMapText == _qMapText) && (b._qMapQual == _qMapQual))
				{
						return false;
				}

				return true;
		}

};

typedef SetOf<Read_Hit> SetOfRead_Hit;
typedef SetOf<SetOfRead_Hit> SetOfRead_Hit_Lists;

struct CoveredCoors
{
		long int _begin;
		long int _end;
};

class SAM_file
{
private:
    friend class SAM_mapping;
		std::string _fileName;
		bool _isFileEnd;
		std::string _qName;
		std::string _flag;
		std::string _tName;
		int _position;
		std::string _mapQual;
		std::string _CIAGR;
		std::string _MRNM;
		std::string _MPOS;
		std::string _ISIZE;
		std::string _qReadText;
		std::string _qReadMapText;
		std::string _qQualText;
		std::string _qQualMapText;
		//$self->{'_ReadHits'} = $notRead;
		bool _isReadHitsLoaded;
		bool _isReadHitsListLoaded;
		bool _isFileOpened;
		SetOfRead_Hit _readHitsArray;
		SetOfRead_Hit_Lists _readHitsListArray;
		FILE* _currFile;
		AssemblyStatistics* assembly_statistics;

		UniqueReadListRef _allReads;


		SetOfDeletions _deletionsAtPoint;




protected:
public:
		SAM_file();


		SAM_file(std::string fileName)
		{
				_fileName = fileName;
				OpenFile();
				_isReadHitsLoaded = false;
				_isReadHitsListLoaded = false;
				_isFileEnd = false;
				assembly_statistics = new AssemblyStatistics();

		}

		~SAM_file()
		{
				if(_isFileOpened)
				{
						fclose(_currFile);
				}
				SetOfFastqSeqRecordsRef& temp = this->_allReads.GetReadList();
				for(int i = 0; i < temp.GetSizeOfSet(); i++)
				{
					delete temp.GetItemByNum(i);
				}
				delete assembly_statistics;
		}


		int OpenFile(std::string fileName)
		{
			 int notError = 1;
			 _fileName = fileName;
			 notError = OpenFile();

			 return notError;

		}

		bool IsFileEnded()
		{
				return _isFileEnd;
		}

		bool IsReadHitsLoaded()
		{
				return _isReadHitsLoaded;
		}

		bool IsReadHitsListLoaded()
		{
				return _isReadHitsListLoaded;
		}

		bool IsFileOpened()
		{
				return _isFileOpened;
		}

		int OpenFile();

		void CloseFile()
		{
				if(_isFileOpened)
				{
						fclose(_currFile);
				}
				_isFileOpened = false;
		}



		void SetFileToStart()
		{
				if(_isFileOpened)
				{
						fseek(_currFile,0,SEEK_SET);
				}
		}

		SetOfRead_Hit& GetSetOfReadHits()
		{
				return _readHitsArray;
		}

		SetOfRead_Hit* GetSetOfReadHitsAtPos(int pos)
		{
				return _readHitsListArray.GetRefItemByNum(pos);
		}

		const UniqueReadListRef GetAllReadList()
		{
				return _allReads;
		}

		AssemblyStatistics* GetAssemblyStatistics()
		{
			return this->assembly_statistics;
		}

		void ReadNextLine();
		void PrintToTerminal();
		std::string GenerateMappedReadOrQual(std::string qText, std::string ciagrString);
		void LoadReadHits();
		void LoadReadHits(double percentOfReadLenLowLim);
		void LoadReadHits(int sizeOfRef);
		void GenerateDeletions(std::string qText,std::string CIAGR,SetOfDeletions &deletions, int qStart,int tStart);
};


class SAM_mapping
{
private:
		SAM_file _samFile;
		std::string _samFileName;
		FastaSequenceRecord _refSeq;
		std::string _refFileName;
		float _limCountToConsiderDiploid;
		float _fractionOfAvCov;

		HashIntInt coverage_distribution;
protected:

public:
		SAM_mapping(std::string samFileName,std::string refFileName,std::string nameRefContig,float limCount,float fraction): _samFile(samFileName)
		{

				// SAM_file _samFile(samFileName);
				// _samFile = samFile_temp;
				_samFileName = samFileName;
				_refFileName = refFileName;
				_limCountToConsiderDiploid = limCount;
				_fractionOfAvCov = fraction;

				FastaSequenceFileUnBuf fastaRefFile(refFileName.c_str());
				cout << "refFileName.c_str()-" <<refFileName.c_str() << "\n";
				_refSeq = fastaRefFile.GetSeqRecordByReadName(nameRefContig);
				cout << "nameRefContig-" <<nameRefContig << "\n";

				cout << _refSeq.GetSequenceText().length()<< "\n";
				fastaRefFile.CloseSeqFile();
				_samFile.OpenFile();
				cout << "_samFile.OpenFile()\n";
				cout << _samFile.IsFileOpened() << "\n";
				_samFile.LoadReadHits((int)(_refSeq.GetSequenceText().length()));
				cout << "_samFile.LoadReadHits()\n";
				_samFile.CloseFile();
				cout << "_samFile.CloseFile()\n";

				cout <<"_samFile.GetSetOfReadHits().GetSizeOfSet()\t"<< _samFile.GetSetOfReadHits().GetSizeOfSet()<< "\n";

		}

		int CountCoverage(std::string avFileNameOutput, std::string covTableFileNameOutput);

		SAM_mapping(std::string samFileName,std::string refFileName,std::string nameRefContig,float limCount,double readLenLowLim, float fraction):_samFile(samFileName)
		{

				_samFileName = samFileName;
				_refFileName = refFileName;
				_limCountToConsiderDiploid = limCount;
				_fractionOfAvCov = fraction;

				FastaSequenceFileUnBuf fastaRefFile(refFileName.c_str());
				_refSeq = fastaRefFile.GetSeqRecordByReadName(nameRefContig);
				fastaRefFile.CloseSeqFile();
				_samFile.OpenFile();
				cout << "_samFile.OpenFile()\n";
				_samFile.LoadReadHits(readLenLowLim);
				cout << "_samFile.LoadReadHits(readLenLowLim)\n";
				_samFile.CloseFile();
				cout << "_samFile.CloseFile()\n";
		}

		int CountNumOfLetterInReadsAtRefPos(char letter,int position);
		int CountDeletionsOfLettersInReadsAtRefPos(int position);
		short FindWhatLetterIsCorrectAtThisPos(int position);

		int PrintMappingToFile(char *fileName);

		UniqueReadListRef GetListOfReadsWithThisLetterAtThisPos(char letter, int pos);
		UniqueReadListRef GetListOfReadsWithThisTextAtThisPos(std::string text, int pos,bool isDeletion);
		UniqueReadListRef GetListOfReadsAtThisPos( int pos);


		int CountCoverageAtThisPos(int position);
		//int FindGaploidsAlgorithm(std::string allConflictPoints);

		int CountStats(std::string fileNameStats);
		int FindGaploidsAlgorithm(std::string fileGraph,std::string fileSimpleGraph, std::string resultFileName, std::string fileDebugFileName, int num_of_chr);

		//void PrintFastaWithResultContigsByPathes(std::string fasta_fn, SetOfRef<Path>* resPathes, fstream &debugFile);
		std::string GetConsensusString(int startCoor,int endCoor);
		SetOf<std::string> GetAllDelStrsAtPos(unsigned int pos);
		void ConnectNotConnectedPrevVertexies(SetOfRef<Vertex> &currVertexies,SetOfRef<Vertex> &lastVertexies, Graph *currGraph);
		//int FindGaploidsAlgorithm(std::string fileGraph,std::string fileSimpleGraph, std::string resultFileName, std::string fileDebugFileName, std::string scaffFileName,std::string scaffDebugFileName, int maxInsertSize, double fraction);
		string FindWhatLetterHasMaximumWeightAtThisPos(int position);
};

#endif // SAM_MAPPING_H
