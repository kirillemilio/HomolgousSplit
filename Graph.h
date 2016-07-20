#ifndef GRAPH_H
#define GRAPH_H

#include "Vertex.h"

class Graph
{
    SetOfRef<Vertex> _vertexies;
    SetOfRef<Vertex> _firstVertexies;
    SetOfRef<Vertex> _notIncludedInPathVertexies;

public:
    Graph()
    {

    }

    void ClearGraph();


    ~Graph()
    {
        ClearGraph();
    }

    SetOfRef<Vertex>& GetAllVertexies()
    {
        return this->_vertexies;
    }

    SetOfRef<Vertex> GetFirstVertexies()
    {
        return this->_firstVertexies;
    }

    SetOfRef<Vertex> GetNotIncludedInPathVertexies()
    {
        return this->_notIncludedInPathVertexies;
    }

    void AddVertex(Vertex *ver)
    {
        _vertexies.AddToSet(ver);
    }

    void AddFirstVertex(Vertex *ver)
    {
        _firstVertexies.AddToSet(ver);
    }


    void FillFirstVertexies();
    void FillNotIncludedInPathVertexies();

    Vertex* GetVertexByIndex(int index)
    {
        return _vertexies.GetItemByNum(index);
    }

    Vertex* GetFirstVertexByIndex(int index)
    {
        return _firstVertexies.GetItemByNum(index);
    }

    int GetFirstVertexiesSize()
    {
        return _firstVertexies.GetSizeOfSet();
    }

    void PrintGraph(fstream &graphFile);

    int GetVertexSetSize()
    {
        return _vertexies.GetSizeOfSet();
    }

};

typedef __gnu_cxx::hash_map<std::string,SimplifyVertex*,string_hash_compare> HashStringSimplifyVertex;
class SimplifyGraph
{
  SetOfRef<SimplifyVertex> _simplifyVertexies;
  SetOfRef<SimplifyVertex> _simplifyFirstVertexies;
  SetOfRef<SimplifyVertex> _notIncludedInPathVertexies;
  HashStringInt _addedVertexies;
  HashStringSimplifyVertex _mapVertexToSimplifyVertex;

public:
  SimplifyGraph()
  {

  }

  SetOfRef<SimplifyVertex> GetsimplifyFirstVertexies()
  {
      return this->_simplifyFirstVertexies;
  }

  SetOfRef<SimplifyVertex> GetNotIncludedInPathSimplifyVertexies()
  {
      return this->_notIncludedInPathVertexies;
  }

  void AddVertex(SimplifyVertex *ver)
  {
	_simplifyVertexies.AddToSet(ver);
  }

  void AddFirstVertex(SimplifyVertex *ver)
  {
	  _simplifyFirstVertexies.AddToSet(ver);
  }

  void FindFirstVertex();

  void ClearGraph();

  void CreateSimplifyVertex(Vertex* verToAdd,SimplifyVertex *simplifyVertex);

  SimplifyVertex* GetSimplifyVertexByIndex(int i)
  {
	  return _simplifyVertexies.GetItemByNum(i);
  }

  void FillSimplifyVertex(SimplifyVertex *simplifyVertex, Vertex *startVertex);

  void FillNotIncludedInPathSimplifyVertexies();

  void EaseGraph(Graph *graphToSimplify);


  void PrintGraph(fstream &graphFile);

  int GetVertexSetSize()
  {
      return _simplifyVertexies.GetSizeOfSet();
  }

  ~SimplifyGraph()
  {
      ClearGraph();
  }

};


#endif // GRAPH_H
