//---------------------------------------------------------------------------

#ifndef BaseClassesH
#define BaseClassesH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <hash_map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "string_hash_compare.h";

const int ErrorHappens = -1;
const int AllOk = 1;
const int minWeightToScaffoldByPE = 5;
extern const char* StringEmpty;

typedef __gnu_cxx::hash_map<std::string,int,string_hash_compare> HashStringInt;
typedef __gnu_cxx::hash_map<unsigned,int> HashUnsignedInt;


using namespace std;
template < typename _TypeToStore > class SetOf
{
    protected:
	std::vector < _TypeToStore > storedValues;
	int _sizeOfSet;
    public:

	SetOf()
	{
		_sizeOfSet = 0;
	}

	int GetSizeOfSet() const
	{
		return _sizeOfSet;
	}

	_TypeToStore& operator[](int itemIndex)
	{
		if(itemIndex < _sizeOfSet)
		{
			return storedValues[itemIndex];
		}
		return _TypeToStore::Empty();
	}

	void AddToSet(_TypeToStore valToStore)
	{
		storedValues.push_back(valToStore);
		_sizeOfSet++;
	}

    _TypeToStore GetItemByNum(int itemIndex) const
	{

        if(itemIndex < _sizeOfSet)
		{
            return storedValues[itemIndex];
		}

        return _TypeToStore::Empty();
	}

    _TypeToStore* GetRefItemByNum(int itemIndex)
    {

        if(itemIndex < _sizeOfSet)
        {
            return &(storedValues[itemIndex]);
        }

        return NULL;
    }

	void Clear()
	{
		storedValues.clear();
        _sizeOfSet = 0;
	}

	~SetOf()
	{

	}

    bool IsValueExistInSet(const _TypeToStore& strToCompare) const
    {
        for(int i = 0; i < _sizeOfSet;i++)
        {
            if(strToCompare == storedValues[i])
            {
                return true;
            }
        }

        return false;
    }

    void Remove(int index)
    {

        _TypeToStore *tempArr = new _TypeToStore[_sizeOfSet];

        int tempSize = _sizeOfSet;

        for(int i = 0; i < tempSize;i++)
        {
            tempArr[i] = storedValues[i];
        }


        storedValues.clear();


        for(int i = 0; i < tempSize;i++)
        {
            if(i!= index)
            {
                storedValues.push_back(tempArr[i]);
                _sizeOfSet++;
            }
        }

        delete []tempArr;
    }

    void RemoveFromIndex(int index)
    {
        for(int i = _sizeOfSet - 1; i >= index; i--)
        {
            storedValues.pop_back();
        }
        _sizeOfSet = index;
    }




};

template < typename _TypeToStore > class SetOfRef
{
    protected:
    std::vector < _TypeToStore* > storedValues;
    int _sizeOfSet;
    public:

    SetOfRef()
    {
        _sizeOfSet = 0;
    }

    // SetOfRef(const SetOfRef<_TypeToStore>& item)
    // {
    //   this->_sizeOfSet = 0;
    //   for(int i = 0; i < item.GetSizeOfSet(); i++)
    //   {
    //       this->AddToSet(item.GetItemByNum(i));
    //   }
    // }

    std::vector<_TypeToStore*> GetInnerVector()
    {
        return this->storedValues;
    }

    int GetSizeOfSet() const
    {
        return _sizeOfSet;
    }


    void AddToSet(_TypeToStore* valToStore)
    {
        //cout << "valAdd-" << valToStore << "\n";
        storedValues.push_back(valToStore);
        //cout << "valAdded-"<< (storedValues.size()-1)<<"-" << storedValues[storedValues.size()-1] << "\n";
        //cout << "_sizeOfSet-"<< _sizeOfSet<< "\n";
        _sizeOfSet++;
    }

    _TypeToStore* GetItemByNum(int itemIndex) const
    {

        if(itemIndex < _sizeOfSet)
        {
           // cout << "val-" << itemIndex<<"-"<< storedValues[itemIndex] << "\n";


            return storedValues[itemIndex];
        }

        return NULL;
    }

    _TypeToStore*& operator[](int itemIndex)
    {
    	if(itemIndex < _sizeOfSet)
    	{
    		return storedValues[itemIndex];
    	}
    	return NULL;
    }

    void Clear()
    {
        storedValues.clear();
        _sizeOfSet = 0;
    }

    ~SetOfRef()
    {

    }

    void RemoveFromIndex(int index)
    {
        for(int i = _sizeOfSet - 1; i >= index; i--)
        {
            storedValues.pop_back();
        }
        _sizeOfSet = index;
    }

    void RemoveAllRefsFromIndex(int index)
    {
        for(int i = _sizeOfSet - 1; i >= index; i--)
        {
            delete this->GetItemByNum(i);
            storedValues.pop_back();
        }
        _sizeOfSet = index;
    }

    template<class Compare>
    void SortInnerVector(Compare comparator)
    {
        std::sort(this->storedValues.begin(), this->storedValues.end(), comparator);
    }

};


class SequenceRecord
{
	protected:
	std::string _sequenceName;
	std::string _sequenceText;

	public:

	static SequenceRecord Empty()
	{
		return SequenceRecord(StringEmpty,StringEmpty);
	}

	SequenceRecord(const std::string sequenceName,const std::string sequenceText)
	{
		_sequenceName = sequenceName;
		_sequenceText = sequenceText;
	}

	SequenceRecord(const char* sequenceName,const char* sequenceText)
	{
		_sequenceName = sequenceName;
		_sequenceText = sequenceText;
	}

	SequenceRecord()
	{
		_sequenceName = StringEmpty;
		_sequenceText = StringEmpty;
	}

    SequenceRecord(const SequenceRecord &seqRec)
    {
        _sequenceName = seqRec._sequenceName;
        _sequenceText = seqRec._sequenceText;
    }

    SequenceRecord& operator=(const SequenceRecord &seqRec)
    {
        _sequenceName = seqRec._sequenceName;
		_sequenceText = seqRec._sequenceText;
		return *this;
    }

    int GetSeqTextLength()
    {
    	return this->_sequenceText.length();
    }
	virtual std::string GetSequenceName()
	{
		return _sequenceName;
	}

	virtual std::string GetSequenceText()
	{
		return _sequenceText;
	}

	virtual void Clear()
	{
		_sequenceName = StringEmpty;
		_sequenceText = StringEmpty;
	}

	void Set(const std::string sequenceName,const std::string sequenceText)
	{
		_sequenceName = sequenceName;
		_sequenceText = sequenceText;
	}

	virtual bool IsEmpty()
	{
		if((_sequenceName == StringEmpty) && (_sequenceText == StringEmpty))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	virtual char GetCharOfSequenceTextByNum(unsigned int posToTake);

	virtual int WriteToFile(FILE *fileToWrite);



	~SequenceRecord()
	{

	}
};

//typedef SetOf<SequenceRecord> SetOfSeqRecords;



class SequenceFileBuf
{
    private:
    //SetOfSeqRecords _seqRecords;
	public:
	SequenceFileBuf()
	{

	}


    virtual int GetSizeOfSetOfSeq()= 0;
    virtual int LoadFromFile(const char* fileName) = 0;
    virtual int SaveToFile(const char* fileName) = 0;


	virtual ~SequenceFileBuf()
	{

    }
};


class SequenceFileUnBuf
{
    protected:
	FILE* _seqFile;
    bool _isFileOpened;
    std::string _fileName;
    public:
	SequenceFileUnBuf()
	{
	    _isFileOpened = false;
	}

	SequenceFileUnBuf(const char* fileName)
	{
        _isFileOpened = false;
        OpenSeqFile(fileName);
        _fileName = fileName;
	}

	virtual void OpenSeqFile(const char* fileName)
	{
        //cout  << "2_1" << "\n";
        CloseSeqFile();
	    _seqFile = fopen(fileName,"r");
        //cout << "2_2" << "\n";
	    if(_seqFile == NULL)
	    {
          //  cout << "_isFileOpened " << " false" << "\n";
            _isFileOpened = false;
	    }
	    else
	    {

            _isFileOpened = true;
            //cout << "_isFileOpened " << " true " << _isFileOpened<< "\n";
	    }

        _fileName = fileName;
	}

	virtual void CloseSeqFile()
	{
        //cout << "2_1_1" << "\n";
        if(_isFileOpened)
	    {
        //cout << "2_1_2" << "\n";
        fclose(_seqFile);
		//cout << "2_1_3" << "\n";
		_isFileOpened = false;
	    }
	}

	virtual void SetOnStart()
	{
        CloseSeqFile();
        OpenSeqFile(_fileName.c_str());
        // fseek(_seqFile,0,SEEK_SET);
	}

    virtual bool IsEndOfFile()
	{
	    return feof(_seqFile);
	}

    virtual int GetNextSeqRead() = 0;



	virtual ~SequenceFileUnBuf()
	{

	}
};


#endif
