#include "Graph.h"
#include <string>
#include <stdlib.h>


void Graph::ClearGraph()
{
    for(int i = 0;i < _vertexies.GetSizeOfSet();i++)
    {
        delete _vertexies.GetItemByNum(i);
    }

    _vertexies.Clear();
    _firstVertexies.Clear();
}


void Graph::FillFirstVertexies()
{
    for(int i = 0;i< _vertexies.GetSizeOfSet();i++)
    {
        if(_vertexies.GetItemByNum(i)->GetConnFromVertexByIndex(0)._connVertex == NULL)
        {
            _firstVertexies.AddToSet(_vertexies.GetItemByNum(i));
        }
    }
}

void Graph::FillNotIncludedInPathVertexies()
{
    this->_notIncludedInPathVertexies.Clear();
    for(int i = 0; i < _vertexies.GetSizeOfSet(); i++)
    {
        if(!_vertexies.GetItemByNum(i)->CheckInclusionInPath())
        {
            _notIncludedInPathVertexies.AddToSet(_vertexies.GetItemByNum(i));
        }
    }
}

void SimplifyGraph::FillNotIncludedInPathSimplifyVertexies()
{
    this->_notIncludedInPathVertexies.Clear();
    for(int i = 0; i < _simplifyVertexies.GetSizeOfSet(); i++)
    {
        if(!_simplifyVertexies.GetItemByNum(i)->CheckInclusionInPath())
        {
            _notIncludedInPathVertexies.AddToSet(_simplifyVertexies.GetItemByNum(i));
        }
    }
}

void SimplifyGraph::FindFirstVertex()
{
    for(int i = 0;i< _simplifyVertexies.GetSizeOfSet();i++)
    {
        if(_simplifyVertexies.GetItemByNum(i)->GetConnFromVertexByIndex(0)._connVertex == NULL)
        {
            _simplifyFirstVertexies.AddToSet(_simplifyVertexies.GetItemByNum(i));
        }
    }
}

void SimplifyGraph::ClearGraph()
{
    for(int i = 0;i < _simplifyVertexies.GetSizeOfSet();i++)
    {
        delete _simplifyVertexies.GetItemByNum(i);
    }

    _simplifyVertexies.Clear();
    _simplifyFirstVertexies.Clear();
}

void SimplifyGraph::CreateSimplifyVertex(Vertex* verToAdd,SimplifyVertex *simplifyVertex)
{
	Vertex *currVertex = verToAdd;

	while((currVertex->GetConnFromVertexByIndex(0)._connVertex != NULL) &&
			(currVertex->GetConnectFromSize() == 1) &&
		  (((Vertex*)currVertex->GetConnFromVertexByIndex(0)._connVertex)->GetConnectToSize() == 1) &&
		  _addedVertexies.find(currVertex->GetName()) == _addedVertexies.end())
	{
		currVertex = (Vertex*)currVertex->GetConnFromVertexByIndex(0)._connVertex;
	}




	while((currVertex->GetConnToVertexByIndex(0)._connVertex != NULL) && (currVertex->GetConnectToSize() == 1) &&
	(((Vertex*)currVertex->GetConnToVertexByIndex(0)._connVertex)->GetConnectFromSize() == 1))
	{
		simplifyVertex->AddVertex(currVertex);
		_addedVertexies[currVertex->GetName()] = 1;
		currVertex = (Vertex*)currVertex->GetConnToVertexByIndex(0)._connVertex;
	}

	simplifyVertex->AddVertex(currVertex);
	_addedVertexies[currVertex->GetName()] = 1;
	simplifyVertex->SetName(simplifyVertex->GetVertexByIndex(0)->GetName() + "_" + simplifyVertex->GetVertexByIndex(simplifyVertex->GetVertexSetSize()-1)->GetName());


}

void SimplifyGraph::EaseGraph(Graph *graphToSimplify)
{


    for(int i = 0;i<graphToSimplify->GetVertexSetSize();i++)
    {
        Vertex *currVertex = graphToSimplify->GetVertexByIndex(i);
        if(_addedVertexies.find(currVertex->GetName()) == _addedVertexies.end())
        {

            SimplifyVertex *tempPtrSimplifyVertex = new SimplifyVertex();
            this->AddVertex(tempPtrSimplifyVertex);
            CreateSimplifyVertex(currVertex,tempPtrSimplifyVertex);

        }
    }


    for(int i = 0;i < this->GetVertexSetSize();i++)
    {
        SimplifyVertex *currSimplifyVertex;
        currSimplifyVertex = this->GetSimplifyVertexByIndex(i);


        for(int j = 0;j < currSimplifyVertex->GetVertexSetSize();j++)
        {
            Vertex* currVertex = currSimplifyVertex->GetVertexByIndex(j);

            this->_mapVertexToSimplifyVertex[currVertex->GetName()] = currSimplifyVertex;
        }



    }


    for(int i = 0;i < this->GetVertexSetSize();i++)
    {
        SimplifyVertex *currSimplifyVertex = this->GetSimplifyVertexByIndex(i);


        Vertex *firstVertex = currSimplifyVertex->GetVertexByIndex(0);
        Vertex *lastVertex = currSimplifyVertex->GetVertexByIndex(currSimplifyVertex->GetVertexSetSize()-1);

        if(firstVertex->GetConnFromVertexByIndex(0)._connVertex != NULL)
        {
            for(int j = 0;j < firstVertex->GetConnectFromSize();j++)
            {


                Vertex *currConnFromVertex = (Vertex*)firstVertex->GetConnFromVertexByIndex(j)._connVertex;

                SimplifyVertex *currConnFromSimplifyVertex = this->_mapVertexToSimplifyVertex[currConnFromVertex->GetName()];

                currSimplifyVertex->ConnectFromVertex(currConnFromSimplifyVertex,firstVertex->GetConnFromVertexByIndex(j)._weight);

            }
        }


        if(lastVertex->GetConnToVertexByIndex(0)._connVertex != NULL)
        {
            for(int j = 0;j < lastVertex->GetConnectToSize();j++)
            {

                Vertex *currConnToVertex = (Vertex*)lastVertex->GetConnToVertexByIndex(j)._connVertex;

                SimplifyVertex *currConnToSimplifyVertex = this->_mapVertexToSimplifyVertex[currConnToVertex->GetName()];

                currSimplifyVertex->ConnectToVertex(currConnToSimplifyVertex,lastVertex->GetConnToVertexByIndex(j)._weight);
            }
		}

	}



}

void Graph::PrintGraph(fstream &graphFile)
{
	graphFile << "digraph startGraph{\n";


    for(int i = 0; i < _vertexies.GetSizeOfSet();i++)
    {

        _vertexies.GetItemByNum(i)->PrintVertex((graphFile));

    }

    graphFile << "}\n";
}

void SimplifyGraph::PrintGraph(fstream &graphFile)
{
    graphFile << "digraph simplifyGraph{\n";
    for(int i = 0; i < _simplifyVertexies.GetSizeOfSet();i++)
    {
        _simplifyVertexies.GetItemByNum(i)->PrintVertex((graphFile));
    }
    graphFile << "}\n";
}
