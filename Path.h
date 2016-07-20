#ifndef PATH
#define PATH

#include "AdditionClasses.h"
#include "AssemblyStatistics.h"
#include "sam_mapping.h"

typedef std::map<std::string, Vertex*> HashStringVertexPointer;

typedef std::map<FastqSequenceRecord*, int> HashReadPointerInt;

class IntersectedReadsObject
{

private:

    HashReadPointerInt base_table;

public:

    IntersectedReadsObject(SetOfFastqSeqRecordsRef& reads_set)
    {
        for(int i = 0; i < reads_set.GetSizeOfSet(); i++)
        {
            this->base_table[reads_set.GetItemByNum(i)] = 1;
        }
    }

    void AddSetOfReadsPointers(SetOfFastqSeqRecordsRef& reads_set)
    {
        HashReadPointerInt new_table;
        for(int i = 0; i < reads_set.GetSizeOfSet(); i++)
        {
            if(this->base_table.find(reads_set.GetItemByNum(i)) != this->base_table.end())
            {
                new_table[reads_set.GetItemByNum(i)] = 1;
            }
        }
        this->base_table = new_table;
    }

    int GetNumberOfIntersectedReads()
    {
        return this->base_table.size();
    }

    SetOfFastqSeqRecordsRef GetIntersectedReadsSet()
    {
        SetOfFastqSeqRecordsRef out_set;
        for(HashReadPointerInt::iterator it = this->base_table.begin(); it != this->base_table.end(); it++)
        {
            out_set.AddToSet(it->first);
        }
    }
};

class Path
{

private:
    Path* parent;
    SetOfRef<Vertex> vertexies;
    int length;
    double score;

public:

    Path()
    {
        this->length = 0;
        this->score = 0;
        this->parent = NULL;
    }

    Path(const Path& p)
    {
        this->length = p.length;
        this->score = p.score;
        this->vertexies = p.vertexies;
        this->parent = p.parent;
    }

    void PrintPath(std::string out_path)
    {
        ofstream my_file_debug(out_path.c_str(),std::ios::out | std::ios::app);
        for(int j = 0; j < this->vertexies.GetSizeOfSet(); j++)
        {
            Vertex* current_vertex = this->vertexies.GetItemByNum(j);
            my_file_debug << current_vertex->GetCoor() << "_" << current_vertex->GetSymbol() << ">>";

        }
        my_file_debug << "[score= " << this->score << "]" << " END\n\n";
        my_file_debug.close();
    }

    void AddVertex(Vertex* v)
    {
        this->vertexies.AddToSet(v);
        this->length++;
    }

    double GetScore()
    {
        return this->score;
    }

    int GetLength()
    {
        return this->length;
    }

    Path* GetParent()
    {
        return this->parent;
    }

    void SetParent(Path* parent)
    {
        this->parent = parent;
    }

    Vertex* GetVertexByIndex(int index)
    {
        if(index >= 0 && index < this->vertexies.GetSizeOfSet())
        {
            return this->vertexies.GetItemByNum(index);
        }
        else
        {
            return NULL;
        }
    }

    Vertex* GetLastVertex()
    {
        return this->vertexies.GetItemByNum(this->vertexies.GetSizeOfSet() - 1);
    }

    bool CheckConnectedToVertex(Vertex* v);
    std::vector<Vertex*> GetPossibleVertexiesForExtend();
    std::vector<Vertex*> GetListOfVertexiesConnectedWithLast();
    void ComputeScore();
};

class PathStack
{

private:
    friend void PrintPathStack(std::vector<PathStack*> input_vector);
    std::vector<Path*> paths;
    int actual_size;
    int max_size;
    double score;
    HashStringVertexPointer* vertexies_table;

public:

    PathStack(std::vector<Path*> paths, HashStringVertexPointer* vertexies_table, int max_size):paths(paths.begin(), paths.end())
    {
        this->vertexies_table = vertexies_table;
        this->actual_size = paths.size();
        this->max_size = max_size;
        this->score = 0;
        // for(int i = 0; i < paths.size(); i++)
        // {
        //     score += paths.at(i)->GetScore();
        // }
    }

    Path* GetPathByIndex(int index)
    {
        if(index >= 0 && index < actual_size)
        {
            return this->paths.at(index);
        }
        else
        {
            return NULL;
        }
    }

    HashStringVertexPointer* GetVertexiesTable()
    {
        return this->vertexies_table;
    }

    int GetActualSize()
    {
        return this->actual_size;
    }

    int GetMaxSize()
    {
        return this->max_size;
    }

    double GetScore()
    {
        return this->score;
    }

    void SetScore(double score)
    {
        this->score = score;
    }

    void DeleteInnerPaths()
    {
        for(std::vector<Path*>::iterator it = this->paths.begin(); it != this->paths.end(); it++)
        {
            delete *it;
        }
    }

    void PrintAllPaths(std::string out_path)
    {
        // cout << "Entered path stack print function\n";
        ofstream my_file_debug(out_path.c_str(),std::ios::out | std::ios::app);
        my_file_debug << "================================\n";
        my_file_debug.close();
        for(int i = 0; i < this->paths.size(); i++)
        {
            this->paths.at(i)->PrintPath(out_path);
        }
        my_file_debug.open(out_path.c_str(),std::ios::out | std::ios::app);
        my_file_debug << "================================\n";
        my_file_debug.close();
        // cout << "Leaving path stack printing function\n";
    }

    int GetNextVertexiesCoor()
    {
        if(this->paths.size() == 0)
        {
            return -1;
        }
        std::set<Vertex*> represented_vertexies;
        for(int i = 0; i < this->paths.size(); i++)
        {
            std::vector<Vertex*> temp_vertexies = this->paths.at(i)->GetPossibleVertexiesForExtend();
            represented_vertexies.insert(temp_vertexies.begin(), temp_vertexies.end());
        }
        if(represented_vertexies.size() == 0)
        {
            return -1;
        }
        else
        {
            return (*represented_vertexies.begin())->GetCoor();
        }
    }

    int GetStartVertexCoor()
    {
        if(this->paths.size() != 0)
        {
            return this->paths.at(0)->GetVertexByIndex(0)->GetCoor();
        }
        return -1;
    }

    int GetLastVertexCoor()
    {
        if(this->paths.size() == -1)
        {
            return -1;
        }
        else
        {
            return this->paths.at(0)->GetVertexByIndex(this->paths.at(0)->GetLength() - 1)->GetCoor();
        }
    }

    void ComputeScoreByLastVertexies();
    bool CheckNextExtentContainAllPossibleVertexiesAndAllTailsConnected();
    PathStack* GetValidExtentionsOfPathStack(int num_of_result_paths, int score_difference_threshold);
    static PathStack* ContinuePathStack(PathStack* current_path_stack, int num_of_result_paths, int score_difference_threshold);
    static void GenerateInitialPathStacks(Graph* graph, int num_of_chr, HashStringVertexPointer* vertexies_name_table, std::string file_to_print_path);

};
#endif
