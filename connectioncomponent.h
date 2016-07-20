#ifndef CONNECTIONCOMPONENT_H
#define CONNECTIONCOMPONENT_H

#include "FilesFormats.h"
#include "ConflictPoint.h"

class UniqueReadListRef
{
private:
    SetOfFastqSeqRecordsRef _setOfReads;
    HashStringInt _existedReadsList;

public:
    UniqueReadListRef():_setOfReads(),_existedReadsList()
    {

    }

    void Add(FastqSequenceRecord* addingValue)
    {

        if(_existedReadsList.find(addingValue->GetSequenceName()) == _existedReadsList.end())
        {
           _setOfReads.AddToSet(addingValue);
           _existedReadsList[addingValue->GetSequenceName()] = 1;
        }
    }

    void Clear()
    {
        _setOfReads.Clear();
        _existedReadsList.clear();
    }

    SetOfFastqSeqRecordsRef& GetReadList()
    {
        return _setOfReads;
    }

    SetOfFastqSeqRecordsRef GetReadListCopy() const
    {
        return _setOfReads;
    }

    bool ExistThisRead(std::string readName) const;

    int FindIntersectionOfLists(const SetOfFastqSeqRecordsRef& readListB) const;

    void PrintToStdIn();
    int PrintToFastqFile(std::string fileName);
    int PrintToFastqFile(FILE *fileToWrite);

    void AddSetOfReadList(const SetOfFastqSeqRecordsRef& readListToAdd);

    int FindIntersectionOfListsByPE(const SetOfFastqSeqRecordsRef& readListB) const;
    UniqueReadListRef DiversityFromReadList(const UniqueReadListRef& readListB) const;
    UniqueReadListRef DiversityFromReadListIn(const UniqueReadListRef& readListB);
    SetOfFastqSeqRecordsRef DiversityFromReadListFastqSeq(const UniqueReadListRef& readListB) const;
    UniqueReadListRef UnionReadLists(const UniqueReadListRef& readListB) const;
    SetOfFastqSeqRecordsRef UnionReadListsFastqSeq(const UniqueReadListRef& readListB) const;

};


class Diploid
{
private:
    UniqueReadListRef _firstGaploidReadList;
    UniqueReadListRef _secondGaploidReadList;
    SetOfConflictPoints _conflPointFirstGaploid;
    SetOfConflictPoints _conflPointSecondGaploid;

public:
    Diploid():_firstGaploidReadList(),_secondGaploidReadList()
    {

    }

    Diploid(UniqueReadListRef firstGaploidReadList, UniqueReadListRef secondGaploidReadList):_firstGaploidReadList(firstGaploidReadList),_secondGaploidReadList(secondGaploidReadList)
    {

    }

    UniqueReadListRef* GetFirstGaploid()
    {
        return &_firstGaploidReadList;
    }

    UniqueReadListRef* GetSecondGaploid()
    {
        return &_secondGaploidReadList;
    }

    void Swap()
    {
        UniqueReadListRef temp = _firstGaploidReadList;
        _firstGaploidReadList = _secondGaploidReadList;
        _secondGaploidReadList = temp;
    }

    void AddConflictPointToFirstGaploid(ConflictPoint conflPoint)
    {
       _conflPointFirstGaploid.AddToSet(conflPoint);
    }

    void AddConflictPointToSecondGaploid(ConflictPoint conflPoint)
    {
       _conflPointSecondGaploid.AddToSet(conflPoint);
    }

    SetOfConflictPoints& GetSetOfConflictPointsFirstGaploid()
    {
       return _conflPointFirstGaploid;
    }

    SetOfConflictPoints& GetSetOfConflictPointsSecondGaploid()
    {
       return _conflPointSecondGaploid;
    }

    void AddSetOfConflictPointToFirstGaploid(SetOfConflictPoints& setConflPoint)
    {
        for(int i = 0; i < setConflPoint.GetSizeOfSet();i++)
        {
            _conflPointFirstGaploid.AddToSet(setConflPoint.GetItemByNum(i));
        }
    }

    void AddSetOfConflictPointToSecondGaploid(SetOfConflictPoints& setConflPoint)
    {
        for(int i = 0; i < setConflPoint.GetSizeOfSet();i++)
        {
            _conflPointSecondGaploid.AddToSet(setConflPoint.GetItemByNum(i));
        }
    }

    int FindIntersecAndSetCorrectOrder(Diploid* diploidA, fstream &fstrToStore);
    int FindIntersecAndSetCorrectOrder(Diploid* diploidA);
};



class ConnectionComponent
{
private:
    UniqueReadListRef _firstGaploidReadList;
    UniqueReadListRef _secondGaploidReadList;
    Diploid _lastDiploid;
    SetOfConflictPoints _conflPointFirstGaploid;
    SetOfConflictPoints _conflPointSecondGaploid;



public:
    static ConnectionComponent Empty()
    {
        ConnectionComponent emptyVal;
        return emptyVal;
    }

    ConnectionComponent():_firstGaploidReadList(),_secondGaploidReadList(),_lastDiploid(),_conflPointFirstGaploid(),_conflPointSecondGaploid()
    {

    }

    ConnectionComponent(Diploid diploidToAdd):_firstGaploidReadList(),_secondGaploidReadList(),_conflPointFirstGaploid(),_conflPointSecondGaploid()
    {

        _firstGaploidReadList.AddSetOfReadList(diploidToAdd.GetFirstGaploid()->GetReadList());
        _secondGaploidReadList.AddSetOfReadList(diploidToAdd.GetSecondGaploid()->GetReadList());
        _conflPointFirstGaploid = diploidToAdd.GetSetOfConflictPointsFirstGaploid();
        _conflPointSecondGaploid = diploidToAdd.GetSetOfConflictPointsSecondGaploid();
        _lastDiploid = diploidToAdd;

    }


    UniqueReadListRef* GetFirstGaploid()
    {
        return &_firstGaploidReadList;
    }

    UniqueReadListRef* GetSecondGaploid()
    {
        return &_secondGaploidReadList;
    }

   void AddConflictPointToFirstGaploid(ConflictPoint conflPoint)
   {
      _conflPointFirstGaploid.AddToSet(conflPoint);
   }

   void AddConflictPointToSecondGaploid(ConflictPoint conflPoint)
   {
      _conflPointSecondGaploid.AddToSet(conflPoint);
   }

   SetOfConflictPoints GetSetOfConflictPointsFirstGaploid()
   {
      return _conflPointFirstGaploid;
   }

   SetOfConflictPoints GetSetOfConflictPointsSecondGaploid()
   {
      return _conflPointSecondGaploid;
   }

   void AddSetOfConflictPointToFirstGaploid(SetOfConflictPoints setConflPoint)
   {
       for(int i = 0; i < setConflPoint.GetSizeOfSet();i++)
       {
           _conflPointFirstGaploid.AddToSet(setConflPoint.GetItemByNum(i));
       }
   }

   void AddSetOfConflictPointToSecondGaploid(SetOfConflictPoints setConflPoint)
   {
       for(int i = 0; i < setConflPoint.GetSizeOfSet();i++)
       {
           _conflPointSecondGaploid.AddToSet(setConflPoint.GetItemByNum(i));
       }
   }

    int AddDiploid(Diploid diploidA,fstream &fstrToStore);
    int AddDiploid(Diploid diploidA);
    void PrintToStdIn();
    int PrintToFastqFiles(std::string baseFileName);

    void Swap();
    int FindIntersectionByPEAndSetCorrectOrder(ConnectionComponent *conCompB);


};

typedef SetOf<ConnectionComponent> SetOfConComp;



#endif // CONNECTIONCOMPONENT_H
