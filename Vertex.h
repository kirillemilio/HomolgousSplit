//---------------------------------------------------------------------------

#ifndef VertexH
#define VertexH
//---------------------------------------------------------------------------

#include <string>
#include "BaseClasses.h"
#include "connectioncomponent.h"


using namespace std;



class BaseVertex
{
    virtual void PrintVertex(fstream &graphText) = 0;
};

struct ConnVertex{
    BaseVertex *_connVertex;
    int _weight;

    ConnVertex()
    {
        _connVertex = NULL;
        _weight = 0;
    }

    ConnVertex(BaseVertex *connVertex, int weight)
    {
        _connVertex = connVertex;
        _weight = weight;
    }

    static ConnVertex Empty()
    {
        return ConnVertex();
    }


};


class Vertex: public BaseVertex
{
    private:
        UniqueReadListRef      _readList;
        std::string         _name;
        std::string         _symbol;
        int                 _coor;
        SetOf<ConnVertex>   _toVertexies;
        SetOf<ConnVertex>   _fromVertexies;
        int inclusion_in_path;

    public:
        Vertex():_readList(),_name(""),_symbol(),_coor(-1),_toVertexies(),_fromVertexies()
        {
            this->inclusion_in_path = 0;
        }

        Vertex(UniqueReadListRef& readList, std::string name, std::string symbol, int coor,SetOf<ConnVertex> toVertexies,SetOf<ConnVertex> fromVertexies )
        {
            _readList.AddSetOfReadList(readList.GetReadList());
            _name = name;
            _symbol = symbol;
            _coor = coor;
            _toVertexies = toVertexies;
            _fromVertexies = fromVertexies;
            inclusion_in_path = 0;

        }

        static Vertex Empty()
        {
            return Vertex();
        }


        void SetReadsList(UniqueReadListRef& readList)
        {
            _readList.GetReadList().Clear();
            _readList.AddSetOfReadList(readList.GetReadList());
        }

        void SetName(std::string name)
        {
            _name = name;
        }

        void SetSymbol(std::string symbol)
        {
            _symbol = symbol;
        }

        void SetCoor(int coor)
        {
            _coor = coor;
        }



        UniqueReadListRef& GetReadsList()
        {
            return _readList;
        }

        std::string GetName()
        {
           return _name;
        }

        std::string GetSymbol()
        {
            return _symbol;
        }

        int GetCoor()
        {
            return _coor;
        }


        ConnVertex GetConnToVertexByIndex(int index)
        {
            return _toVertexies.GetItemByNum(index);
        }

        ConnVertex GetConnFromVertexByIndex(int index)
        {
            return _fromVertexies.GetItemByNum(index);
        }

        void ConnectToVertex(Vertex *vertexTo, int weight)
        {
            if((_toVertexies.GetSizeOfSet() == 1) && (_toVertexies.GetItemByNum(0)._connVertex == NULL))
            {
                _toVertexies.GetRefItemByNum(0)->_connVertex = (BaseVertex*)vertexTo;
                _toVertexies.GetRefItemByNum(0)->_weight = weight;

                //cout << "vertexTo-" << (BaseVertex*)vertexTo << "\n";
                //cout << "weight-" << _toVertexies.GetItemByNum(0)._weight << "\n";
                //cout << "_connVertex-" << _toVertexies.GetItemByNum(0)._connVertex  << "\n";
                return;
            }

            ConnVertex tempConnVertex((BaseVertex*)vertexTo, weight);

            _toVertexies.AddToSet(tempConnVertex);

        }

        void ConnectFromVertex(Vertex *vertexFrom, int weight)
        {
            if((_fromVertexies.GetSizeOfSet() == 1) && (_fromVertexies.GetItemByNum(0)._connVertex == NULL))
            {
                _fromVertexies.GetRefItemByNum(0)->_connVertex = (BaseVertex*)vertexFrom;
                _fromVertexies.GetRefItemByNum(0)->_weight = weight;
                return;
            }


            ConnVertex tempConnVertex((BaseVertex*)vertexFrom, weight);

            _fromVertexies.AddToSet(tempConnVertex);

        }

        int GetConnectFromSize()
        {
            return _fromVertexies.GetSizeOfSet();

        }

        int GetConnectToSize()
        {
            return _toVertexies.GetSizeOfSet();

        }

        bool CheckInclusionInPath() const
        {
            return this->inclusion_in_path > 0;
        }

        int GetInclusionInPathCounter() const
        {
            return this->inclusion_in_path;
        }

        void IncrimentInclusionInPath()
        {
            this->inclusion_in_path++;
        }

        void DecrimentInclusionInPath()
        {
            this->inclusion_in_path--;
        }

        virtual void PrintVertex(fstream &graphFile);


};




class SimplifyVertex: public BaseVertex
{
    SetOfRef<Vertex> _vertexies;
    Vertex *_firstVertex;
    SetOf<ConnVertex> _toVertexies;
    SetOf<ConnVertex> _fromVertexies;
    std::string _name;
    UniqueReadListRef _readList;

    UniqueReadListRef _startReadList;
    UniqueReadListRef _endReadList;
    UniqueReadListRef _continuedReadList;

    int inclusion_in_path;


public:

    SimplifyVertex()
    {
        _firstVertex = NULL;
        inclusion_in_path = 0;
    }



    void AddVertex(Vertex *ver)
    {

        _vertexies.AddToSet(ver);

        _readList.AddSetOfReadList(ver->GetReadsList().GetReadList());
    }

    void FindFirstVertex();

    Vertex* GetVertexByIndex(int index)
    {
        return _vertexies.GetItemByNum(index);
    }

    Vertex* GetFirstVertex()
    {
        return _firstVertex;
    }

    unsigned GetStartPos()
    {
        return _vertexies.GetItemByNum(0)->GetCoor();
    }

    unsigned GetEndPos()
    {
        return _vertexies.GetItemByNum(_vertexies.GetSizeOfSet()-1)->GetCoor();
    }

    ConnVertex GetConnToVertexByIndex(int index)
    {
        return _toVertexies.GetItemByNum(index);
    }

    ConnVertex GetConnFromVertexByIndex(int index)
    {
        return _fromVertexies.GetItemByNum(index);
    }

    int GetConnectFromSize()
    {
        return _fromVertexies.GetSizeOfSet();

    }

    int GetConnectToSize()
    {
        return _toVertexies.GetSizeOfSet();

    }

    void ConnectToVertex(SimplifyVertex *vertexTo, int weight)
    {
        if((_toVertexies.GetSizeOfSet() == 1) && (_toVertexies.GetRefItemByNum(0)->_connVertex == NULL))
        {
            _toVertexies.GetRefItemByNum(0)->_connVertex = (BaseVertex*)vertexTo;
            _toVertexies.GetRefItemByNum(0)->_weight = weight;
            return;
        }


        ConnVertex tempConnVertex((BaseVertex*)vertexTo, weight);

        _toVertexies.AddToSet(tempConnVertex);

    }

    void ConnectFromVertex(SimplifyVertex *vertexFrom, int weight)
    {

        if((_fromVertexies.GetSizeOfSet() == 1) && (_fromVertexies.GetRefItemByNum(0)->_connVertex == NULL))
        {
            _fromVertexies.GetRefItemByNum(0)->_connVertex = (BaseVertex*)vertexFrom;
            _fromVertexies.GetRefItemByNum(0)->_weight = weight;
            return;
        }


        ConnVertex tempConnVertex((BaseVertex*)vertexFrom, weight);

        _fromVertexies.AddToSet(tempConnVertex);

    }

    virtual void PrintVertex(fstream &graphFile);

    std::string GetName()
    {
        return _name;
    }

    void SetName(std::string name)
    {
        _name = name;
    }

    int GetVertexSetSize()
    {
        return _vertexies.GetSizeOfSet();
    }

    std::string GetReadNameByNum(int index)
    {
        return _readList.GetReadList().GetItemByNum(index)->GetSequenceName();
    }

    int GetSizeOfReadList()
    {
        return _readList.GetReadList().GetSizeOfSet();
    }

    SetOfFastqSeqRecordsRef& GetReadList()
    {
        return _readList.GetReadList();
    }

    UniqueReadListRef& GetUniqueReadList()
    {
        return _readList;
    }

    UniqueReadListRef& GetStartList()
    {
        return _startReadList;
    }

    UniqueReadListRef& GetEndList()
    {
        return _endReadList;
    }

    UniqueReadListRef& GetContinuedReadList()
    {
        return _continuedReadList;
    }

    bool CheckInclusionInPath() const
    {
        return this->inclusion_in_path > 0;
    }

    int GetInclusionInPathCounter() const
    {
        return this->inclusion_in_path;
    }

    void IncrimentInclusionInPath()
    {
        this->inclusion_in_path++;
    }

    void DecrimentInclusionInPath()
    {
        this->inclusion_in_path--;
    }

    void CountStartEndContinueLists();





};






#endif
