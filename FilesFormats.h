//---------------------------------------------------------------------------

#ifndef FilesFormatsH
#define FilesFormatsH
//---------------------------------------------------------------------------
#include "BaseClasses.h"
#include <iostream>

using namespace std;

class FastaSequenceRecord: public SequenceRecord
{


	public:

    void PrintOnTerminal()
    {
        cout << _sequenceName << " " << _sequenceText << "\n";
    }

    FastaSequenceRecord()
    {
        _sequenceName = StringEmpty;
        _sequenceText = StringEmpty;
    }
    FastaSequenceRecord(const std::string sequenceName,const std::string sequenceText)
    {
        _sequenceName = sequenceName;
        _sequenceText = sequenceText;
    }
    static FastaSequenceRecord Empty()
    {
        return FastaSequenceRecord(StringEmpty,StringEmpty);
    }
    virtual int WriteToFile(FILE *fileToWrite);
    FastaSequenceRecord(const FastaSequenceRecord &seqRec)
    {

        _sequenceName = seqRec._sequenceName;
        _sequenceText = seqRec._sequenceText;
        //cout << "constructor " << _sequenceName << " " << _sequenceText << "\n";
    }

    FastaSequenceRecord & operator=(const FastaSequenceRecord &seqRec)
    {
        _sequenceName = seqRec._sequenceName;
        _sequenceText = seqRec._sequenceText;
//        cout << "operator= " << _sequenceName << " " << _sequenceText << "\n";
    	return *this;

	}

	bool operator ==(const FastaSequenceRecord& b) const
	{
		if((b._sequenceName == _sequenceName) && (b._sequenceText == _sequenceText))
		{
			return true;
		}

		return false;
	}

	bool operator !=(const FastaSequenceRecord& b) const
	{
		if((b._sequenceName == _sequenceName) && (b._sequenceText == _sequenceText))
		{
			return false;
		}

		return true;
	}



};

class FastqSequenceRecord: public SequenceRecord
{
	protected:
	std::string _sequenceQual;

	public:

    void PrintOnTerminal()
    {
        cout << _sequenceName << " " << _sequenceText << " " << _sequenceQual << "\n";
    }

    FastqSequenceRecord()
    {
        _sequenceName = StringEmpty;
        _sequenceText = StringEmpty;
        _sequenceQual = StringEmpty;
    }
    FastqSequenceRecord(const std::string sequenceName,const std::string sequenceText, const std::string sequenceQual)
    {
        _sequenceName = sequenceName;
        _sequenceText = sequenceText;
        _sequenceQual = sequenceQual;
    }

    virtual int WriteToFile(FILE *fileToWrite);
virtual int WriteToFileSeq(FILE *fileToWrite);
	std::string GetQualityString()
	{
		return _sequenceQual;
	}

    void Set(const std::string sequenceName,const std::string sequenceText, const std::string sequenceQual)
	{
        _sequenceName = sequenceName;
		_sequenceText = sequenceText;
		_sequenceQual = sequenceQual;
	}

    static FastqSequenceRecord Empty()
    {
        return FastqSequenceRecord(StringEmpty,StringEmpty,StringEmpty);
    }

    virtual bool IsEmpty()
    {
        if((_sequenceName == StringEmpty) && (_sequenceText == StringEmpty) && (_sequenceQual == StringEmpty))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    FastqSequenceRecord(const FastqSequenceRecord &seqRec)
    {
        _sequenceName = seqRec._sequenceName;
        _sequenceText = seqRec._sequenceText;
        _sequenceQual = seqRec._sequenceQual;
    }

	FastqSequenceRecord & operator=(const FastqSequenceRecord &seqRec)
	{
		_sequenceName = seqRec._sequenceName;
		_sequenceText = seqRec._sequenceText;
		_sequenceQual = seqRec._sequenceQual;
		return *this;
	}

	bool operator ==(const FastqSequenceRecord& b) const
	{
		if((b._sequenceName == _sequenceName) && (b._sequenceText == _sequenceText) && (b._sequenceQual == _sequenceQual))
		{
			return true;
		}

		return false;
	}

	bool operator !=(const FastqSequenceRecord& b) const
	{
		if((b._sequenceName == _sequenceName) && (b._sequenceText == _sequenceText) && (b._sequenceQual == _sequenceQual))
		{
			return false;
		}

		return true;
	}
};
typedef SetOf<FastaSequenceRecord> SetOfFastaSeqRecords;
typedef SetOf<FastqSequenceRecord> SetOfFastqSeqRecords;
typedef SetOfRef<FastqSequenceRecord> SetOfFastqSeqRecordsRef;

class FastaSequenceFileBuf: public SequenceFileBuf
{
    protected:
    SetOfFastaSeqRecords _seqFastaRecords;

    public:



    FastaSequenceFileBuf()
    {

    }

    FastaSequenceFileBuf(const char* fileName)
    {
        LoadFromFile(fileName);
    }

    virtual int GetSizeOfSetOfSeq()
    {
		return _seqFastaRecords.GetSizeOfSet();
    }

    virtual FastaSequenceRecord GetSequenceRecordByIndex(int index)
    {
        if(index < _seqFastaRecords.GetSizeOfSet())
        {
            return _seqFastaRecords.GetItemByNum(index);
        }

        return FastaSequenceRecord::Empty();
    }

    virtual void AddSeqRecord(FastaSequenceRecord seqRec)
    {
        _seqFastaRecords.AddToSet(seqRec);
    }

    virtual int LoadFromFile(const char* fileName);
	virtual int SaveToFile(const char* fileName);

};



class FastqSequenceFileBuf: public SequenceFileBuf
{
    protected:
    SetOfFastqSeqRecords _seqFastqRecords;


	public:

    FastqSequenceFileBuf()
    {

    }

    FastqSequenceFileBuf(const char* fileName)
    {
        LoadFromFile(fileName);
    }

    virtual int GetSizeOfSetOfSeq()
    {
        return _seqFastqRecords.GetSizeOfSet();
    }

    virtual FastqSequenceRecord GetSequenceRecordByIndex(int index)
    {
        if(index < _seqFastqRecords.GetSizeOfSet())
        {
            return _seqFastqRecords.GetItemByNum(index);
        }

        return FastqSequenceRecord::Empty();
    }

    virtual void AddSeqRecord(FastqSequenceRecord seqRec)
    {
        _seqFastqRecords.AddToSet(seqRec);
    }


    virtual int LoadFromFile(const char* fileName);
	virtual int SaveToFile(const char* fileName);



};


class FastaSequenceFileUnBuf: public SequenceFileUnBuf
{
	private:
	char _letterFromFile;
    protected:
    FastaSequenceRecord _seqRecord;
	public:


    FastaSequenceFileUnBuf()
    {
        _isFileOpened = false;
        _seqRecord = FastaSequenceRecord::Empty();
        _letterFromFile = 'N';
    }
    FastaSequenceFileUnBuf(const char* fileName):SequenceFileUnBuf(fileName)
    {
        //OpenSeqFile(fileName);
        _seqRecord = FastaSequenceRecord::Empty();
        _letterFromFile = 'N';
    }

	virtual int GetNextSeqRead();

    virtual void OpenSeqFile(const char* fileName)
    {
        _seqRecord = FastaSequenceRecord::Empty();
        SequenceFileUnBuf::OpenSeqFile(fileName);

    }

    virtual void CloseSeqFile()
    {
        _seqRecord = FastaSequenceRecord::Empty();
        SequenceFileUnBuf::CloseSeqFile();

    }

    virtual void SetOnStart()
    {
        _seqRecord = FastaSequenceRecord::Empty();
        SequenceFileUnBuf::SetOnStart();

    }

    virtual bool IsEndOfFile()
    {
       // _seqRecord = FastaSequenceRecord::Empty();
        return SequenceFileUnBuf::IsEndOfFile();

    }

    FastaSequenceRecord GetCurrSeqRecord();


    FastaSequenceRecord GetSeqRecordByReadName(std::string readName);

};



class FastqSequenceFileUnBuf: public SequenceFileUnBuf
{
    protected:
    FastqSequenceRecord _seqRecord;




	public:
    FastqSequenceFileUnBuf()
    {
        _seqRecord = FastqSequenceRecord::Empty();
    }

    FastqSequenceFileUnBuf(const char* fileName):SequenceFileUnBuf(fileName)
    {
        _seqRecord = FastqSequenceRecord::Empty();
    }
	virtual int GetNextSeqRead();


    virtual void OpenSeqFile(const char* fileName)
    {
        _seqRecord = FastqSequenceRecord::Empty();
        SequenceFileUnBuf::OpenSeqFile(fileName);

    }

    virtual void CloseSeqFile()
    {
        _seqRecord = FastqSequenceRecord::Empty();
        SequenceFileUnBuf::CloseSeqFile();

    }

    virtual void SetOnStart()
    {
        _seqRecord = FastqSequenceRecord::Empty();
        SequenceFileUnBuf::SetOnStart();

    }

    virtual bool IsEndOfFile()
    {
        //_seqRecord = FastqSequenceRecord::Empty();
        return SequenceFileUnBuf::IsEndOfFile();

    }

    FastqSequenceRecord GetCurrSeqRecord()
    {
        return _seqRecord;
    }


    FastqSequenceRecord GetSeqRecordByReadName(std::string readName);


};






#endif
