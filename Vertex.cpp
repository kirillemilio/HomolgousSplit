#include "Vertex.h"


void Vertex::PrintVertex(fstream &graphFile)
{
    if((_toVertexies.GetItemByNum(0)._connVertex == NULL) && (_fromVertexies.GetItemByNum(0)._connVertex == NULL))
    {
        graphFile << _name << "\n";
        return;
    }
    if((_toVertexies.GetItemByNum(0)._connVertex == NULL))
    {
        
        return;
    }

    for(int i = 0;i < _toVertexies.GetSizeOfSet();i++)
    {


        graphFile << _name << "->" << ((Vertex*)_toVertexies.GetItemByNum(i)._connVertex)->GetName();
        graphFile << " " << "[label=" << _toVertexies.GetItemByNum(i)._weight <<"]\n";

        //cout << _name << "=>" << ((Vertex*)_toVertexies.GetItemByNum(i)._connVertex)->GetName();
        //cout << " " << "[label=" << _toVertexies.GetItemByNum(i)._weight <<"]\n";
    }
}



void SimplifyVertex::PrintVertex(fstream &graphFile)
{

    if((_toVertexies.GetItemByNum(0)._connVertex == NULL) && (_fromVertexies.GetItemByNum(0)._connVertex == NULL))
    {
        graphFile << _name << "\n";
        return;
    }
    if((_toVertexies.GetItemByNum(0)._connVertex == NULL))
    {

        return;
    }
    for(int i = 0;i < _toVertexies.GetSizeOfSet();i++)
    {
        graphFile << _name << "->" << ((SimplifyVertex*)_toVertexies.GetItemByNum(i)._connVertex)->GetName();
        graphFile << " " << "[label=" << _toVertexies.GetItemByNum(i)._weight <<"]\n";
    }
    //graphFile << nameOfVertex;


}

void SimplifyVertex::FindFirstVertex()
{
    for(int i = 0; i < _vertexies.GetSizeOfSet();i++)
    {
        if(_vertexies.GetItemByNum(i)->GetConnFromVertexByIndex(0)._connVertex == NULL)
        {
            _firstVertex = _vertexies.GetItemByNum(i);
            break;
        }
    }
}

void SimplifyVertex::CountStartEndContinueLists()
{

    _startReadList.Clear();
    _endReadList.Clear();
    _continuedReadList.Clear();

   UniqueReadListRef fromList;
   UniqueReadListRef toList;

   int sizeOfFromList = _fromVertexies.GetSizeOfSet();
   int sizeOfToList = _toVertexies.GetSizeOfSet();




   if((_fromVertexies.GetRefItemByNum(0) != NULL)&&(sizeOfFromList > 1 || (_fromVertexies.GetRefItemByNum(0)->_connVertex != NULL)))
   {
       for(int i = 0; i < sizeOfFromList;i++)
       {
           if(_fromVertexies.GetRefItemByNum(i)->_weight > 0)
           {

               fromList.AddSetOfReadList(((SimplifyVertex*)_fromVertexies.GetRefItemByNum(i)->_connVertex)->GetReadList());
               //return;
           }
       }

   }
   //return;

   if((_toVertexies.GetRefItemByNum(0) != NULL) && (sizeOfToList > 1 || (_toVertexies.GetRefItemByNum(0)->_connVertex != NULL)))
   {
       for(int i = 0; i < sizeOfToList;i++)
       {
           if(_toVertexies.GetRefItemByNum(i)->_weight > 0)
           {
               toList.AddSetOfReadList(((SimplifyVertex*)_toVertexies.GetRefItemByNum(i)->_connVertex)->GetReadList());
           }
       }

   }

   _startReadList.AddSetOfReadList(_readList.DiversityFromReadListFastqSeq(fromList));
   _endReadList.AddSetOfReadList(_readList.DiversityFromReadListFastqSeq(toList));
   _continuedReadList.AddSetOfReadList(toList.GetReadList());



}
