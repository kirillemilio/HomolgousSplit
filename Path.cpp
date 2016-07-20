#include "Path.h"

bool PathStackPointersCompare(PathStack*& a, PathStack*& b)
{
    return a->GetScore() > b->GetScore();
}

void Path::ComputeScore()
{
    if(this->length <= 1)
    {
        this->score = 0;
        return;
    }

    int vertexies_counter = 0;
    UniqueReadListRef last_vertex_reads = this->GetLastVertex()->GetReadsList();
    for(int i = this->vertexies.GetSizeOfSet() - 2; i >= 0; i--)
    {
        int intersection = last_vertex_reads.FindIntersectionOfLists(this->GetVertexByIndex(i)->GetReadsList().GetReadList());
        if(intersection != 0)
        {
            vertexies_counter += 1;
        }
    }
    this->score = double(vertexies_counter); // раньше здесь стоял плюс и все делилось на длину пути; В какой-то момент я решил это убрать
}

std::vector<Vertex*> Path::GetListOfVertexiesConnectedWithLast()
{
    std::vector<Vertex*> out_vector;
    if(this->vertexies.GetSizeOfSet() <= 1)
    {
        return out_vector;
    }

    UniqueReadListRef last_vertex_reads = this->GetLastVertex()->GetReadsList();
    for(int i = this->vertexies.GetSizeOfSet() - 2; i >= 0; i--)
    {
        int intersection = last_vertex_reads.FindIntersectionOfLists(this->GetVertexByIndex(i)->GetReadsList().GetReadList());
        if(intersection != 0)
        {
            out_vector.push_back(this->GetVertexByIndex(i));
        }
    }
    return out_vector;
}

// std::vector<Vertex*> Path::GetListOfVertexiesConnectedWithLast()
// {
//     std::vector<Vertex*> out_vector;
//     if(this->vertexies.GetSizeOfSet() <= 1)
//     {
//         return out_vector;
//     }
//
//     UniqueReadListRef last_vertex_reads = this->GetLastVertex()->GetReadsList();
//     IntersectedReadsObject inter_obj(last_vertex_reads.GetReadList());
//     for(int i = this->vertexies.GetSizeOfSet() - 2; i >= 0; i--)
//     {
//         inter_obj.AddSetOfReadsPointers(this->GetVertexByIndex(i)->GetReadsList().GetReadList());
//         int intersection = inter_obj.GetNumberOfIntersectedReads();
//         if(intersection != 0)
//         {
//             out_vector.push_back(this->GetVertexByIndex(i));
//         }
//         else
//         {
//             break;
//         }
//     }
//     return out_vector;
// }

std::vector<Vertex*> Path::GetPossibleVertexiesForExtend()
{
    std::vector<Vertex*> out_vector;
    if(this->GetLastVertex()->GetConnectToSize() == 1 && this->GetLastVertex()->GetConnToVertexByIndex(0)._connVertex == NULL)
    {
        return out_vector;
    }
    for(int i = 0; i < this->GetLastVertex()->GetConnectToSize(); i++)
    {
        out_vector.push_back((Vertex*)this->GetLastVertex()->GetConnToVertexByIndex(i)._connVertex);
    }
    return out_vector;
}

bool Path::CheckConnectedToVertex(Vertex* v)
{
    std::set<Vertex*> out_set;
    if(this->GetLastVertex()->GetConnectToSize() == 1 && this->GetLastVertex()->GetConnToVertexByIndex(0)._connVertex == NULL)
    {
        return false;
    }
    for(int i = 0; i < this->GetLastVertex()->GetConnectToSize(); i++)
    {
        out_set.insert((Vertex*)this->GetLastVertex()->GetConnToVertexByIndex(i)._connVertex);
    }
    return out_set.find(v) != out_set.end();
}

void PrintPathStack(std::vector<PathStack*> input_vector)
{
    for(int i = 0; i < input_vector.size(); i++)
    {
        cout <<"=================\n";
        for(int j = 0; j < input_vector.at(i)->paths.size(); j++)
        {
            cout <<"++++++++++++++\n";
            for(int k = 0; k < input_vector.at(i)->paths.at(j)->GetLength(); k++)
            {
                cout << ">>" << input_vector.at(i)->paths.at(j)->GetVertexByIndex(k)->GetName();
            }
            cout <<"\n++++++++++++++\n";
        }
        cout <<"Score is:\t"<< input_vector.at(i)->GetScore() << "\n";
        cout <<"=================\n";
    }
}

void PrintPathsInVector(std::vector<Path*> input_vector)
{
    for(int i = 0; i < input_vector.size(); i++)
    {
        cout <<"++++++++++++++\n";
        for(int j = 0; j < input_vector.at(i)->GetLength(); j++)
        {
            cout << ">>" << input_vector.at(i)->GetVertexByIndex(j)->GetName();
        }
        cout << "\t" << input_vector.at(i)->GetScore() << "\n";
        cout <<"\n++++++++++++++\n";
    }
}

bool CheckVectorValuesAccending(std::vector<int> input_vector)
{
    if(input_vector.size() <= 1)
    {
        return true;
    }
    for(int i = 1; i < input_vector.size(); i++)
    {
        if(input_vector.at(i - 1) >= input_vector.at(i))
        {
            return false;
        }
    }
    return true;
}

bool PathStack::CheckNextExtentContainAllPossibleVertexiesAndAllTailsConnected()
{
    std::set<Vertex*> represented_vertexies;
    for(int i = 0; i < this->paths.size(); i++)
    {
        std::vector<Vertex*> temp_vertexies = this->paths.at(i)->GetPossibleVertexiesForExtend();
        if(temp_vertexies.size() == 0)
        {
            return false;
        }
        represented_vertexies.insert(temp_vertexies.begin(), temp_vertexies.end());
    }


    int temp_coor = (*represented_vertexies.begin())->GetCoor();

    std::vector<std::string> array;
    char *tempBuf = new char[100];
    itoa(temp_coor, tempBuf, 10);
    array.push_back(std::string("A_") + tempBuf);
    array.push_back(std::string("G_") + tempBuf);
    array.push_back(std::string("T_") + tempBuf);
    array.push_back(std::string("C_") + tempBuf);
    array.push_back(std::string("Ins_") + tempBuf);
    delete[] tempBuf;

    int counter = 0;
    for(int i = 0; i < array.size(); i++)
    {
        if(this->vertexies_table->find(array.at(i)) != this->vertexies_table->end())
        {
            counter++;
        }
    }

    return counter == represented_vertexies.size();
}

void PathStack::ComputeScoreByLastVertexies()
{
    typedef std::map<Vertex*, std::set<Vertex*> > VertexMapSet;
    VertexMapSet temp_table;

    for(int i = 0; i < this->paths.size(); i++)
    {
        Vertex* current_last_vertex = this->paths.at(i)->GetLastVertex();
        std::vector<Vertex*> current_connected_with_last_vertexies = this->paths.at(i)->GetListOfVertexiesConnectedWithLast();
        temp_table[current_last_vertex].insert(current_connected_with_last_vertexies.begin(), current_connected_with_last_vertexies.end());
    }

    int temp_score = 0;
    for(VertexMapSet::iterator it = temp_table.begin(); it != temp_table.end(); it++)
    {
        temp_score += it->second.size();
    }
    this->score = temp_score;
}

PathStack* PathStack::GetValidExtentionsOfPathStack(int num_of_result_paths, int score_difference_threshold)
{
    cout << "Entered main function\n";
    if(!this->CheckNextExtentContainAllPossibleVertexiesAndAllTailsConnected())
    {
        return this;
    }
    std::vector<Path*> possible_extended_paths;
    std::set<Vertex*> represented_vertexies;
    for(int i = 0; i < this->paths.size(); i++)
    {
        std::vector<Vertex*> temp_vertexies = this->paths.at(i)->GetPossibleVertexiesForExtend();
        represented_vertexies.insert(temp_vertexies.begin(), temp_vertexies.end());
    }

    for(int i = 0; i < this->paths.size(); i++)
    {
        for(std::set<Vertex*>::iterator it = represented_vertexies.begin(); it != represented_vertexies.end(); it++)
        {
            if(this->paths.at(i)->CheckConnectedToVertex(*it))
            {
                Path* new_path = new Path(*(this->paths.at(i)));
                new_path->SetParent(this->paths.at(i));
                new_path->AddVertex(*it);
                new_path->ComputeScore();
                possible_extended_paths.push_back(new_path);
            }
        }
    }
    cout << "Number of paths\t" << num_of_result_paths << "\n";
    // PrintPathsInVector(possible_extended_paths);
    // cout << "Possible extended paths are printed\n";
    if(possible_extended_paths.size() == num_of_result_paths)
    {
        std::set<Vertex*> temp_vertexies;
        std::set<Path*> parent_paths;
        for(int i = 0; i < possible_extended_paths.size(); i++)
        {
            temp_vertexies.insert(possible_extended_paths.at(i)->GetLastVertex());
            parent_paths.insert(possible_extended_paths.at(i)->GetParent());
        }
        if(parent_paths.size() == this->paths.size() && temp_vertexies.size() == represented_vertexies.size())
        {
            PathStack* out_path_stack = new PathStack(possible_extended_paths, this->vertexies_table, this->max_size);
            out_path_stack->ComputeScoreByLastVertexies();
            // this->DeleteInnerPaths()
            return out_path_stack;
        }
        else
        {
            cout << "Not all vertexies were used in path extentions\n" << "Number of result paths which is " << num_of_result_paths << "\n";
            // char c;
            // cin >> c;
            return this;
        }
    }

    else if(possible_extended_paths.size() < num_of_result_paths)
    {
        cout << "Number of extended paths is " << possible_extended_paths.size() << "\n";
        cout << "number of possible extended paths is lower than number of result paths which is " << num_of_result_paths << "\n";
        // char c;
        // cin >> c;
        return this;
    }

    std::vector<PathStack*> path_stacks;
    std::vector<int> counter_vector(num_of_result_paths, 0);
    cout << "Counter vector size is " << counter_vector.size() << "\n";
    while(true)
    {
        std::set<int> temp_set(counter_vector.begin(), counter_vector.end());
        if(CheckVectorValuesAccending(counter_vector))
        {
            std::vector<Path*> temp_paths_vector;
            std::set<Vertex*> temp_vertexies;
            std::set<Path*> parent_paths;
            for(int i = 0; i < counter_vector.size(); i++)
            {
                temp_paths_vector.push_back(possible_extended_paths.at(counter_vector.at(i)));
                temp_vertexies.insert(possible_extended_paths.at(counter_vector.at(i))->GetLastVertex());
                parent_paths.insert(possible_extended_paths.at(counter_vector.at(i))->GetParent());
            }
            if(parent_paths.size() == this->paths.size() && temp_vertexies.size() == represented_vertexies.size())
            {
                PathStack* temp_path_stack = new PathStack(temp_paths_vector, this->vertexies_table, this->max_size);
                temp_path_stack->ComputeScoreByLastVertexies();
                path_stacks.push_back(temp_path_stack);
            }
        }
        else if(temp_set.size() == 1 && temp_set.find(possible_extended_paths.size() - 1) != temp_set.end())
        {
            break;
        }

        for(int i = counter_vector.size() - 1; i >= 0; i--)
        {
            if(counter_vector.at(i) == possible_extended_paths.size() - 1)
            {
                counter_vector.at(i) = 0;
                continue;
            }
            else
            {
                counter_vector.at(i)++;
                break;
            }
        }
    }
    cout << "Is going to print possible path_stacks\n";
    PrintPathStack(path_stacks);
    std::sort(path_stacks.begin(), path_stacks.end(), PathStackPointersCompare);
    // char c;
    // cin >> c;
    PathStack* out_path_stack;
    if(path_stacks.size() == 0)
    {
        out_path_stack = this;
    }
    else if(path_stacks.size() == 1)
    {
        out_path_stack = path_stacks.at(0);
    }
    else
    {
        if(path_stacks.at(path_stacks.size() - 1)->GetScore() - path_stacks.at(path_stacks.size() - 2)->GetScore() > score_difference_threshold)
        {
            out_path_stack = path_stacks.at(path_stacks.size() - 1);
        }
        else
        {
            cout << "Two paths with close score => Extention was interupted\n";
            PrintPathStack(path_stacks);
            // char c;
            // cin >> c;
            out_path_stack = this;
        }
    }
    // if(out_path_stack != this)
    // {
    //     cout << "Deleting current paths\n";
    //     this->DeleteInnerPaths();
    // }

    std::set<Path*> paths_not_to_delete(out_path_stack->paths.begin(), out_path_stack->paths.end());
    cout << "Deleting possible extention paths\n";
    for(int i = 0; i < possible_extended_paths.size(); i++)
    {
        if(paths_not_to_delete.find(possible_extended_paths.at(i)) == paths_not_to_delete.end())
        {
            delete possible_extended_paths.at(i);
        }
    }
    cout << "Deleting unsued path stacks\n";
    for(int i = 0; i < path_stacks.size(); i++)
    {
        if(path_stacks.at(i) != out_path_stack)
        {
            delete path_stacks.at(i);
        }
    }
    cout << "Exiting main function\n";
    return out_path_stack;
}

// PathStack* PathStack::ContinuePathStack(PathStack* current_path_stack, int num_of_result_paths, int score_difference_threshold)
// {
//     PathStack* curr = current_path_stack;
//     bool continue_cycle_flag = true;
//     while(continue_cycle_flag)
//     {
//         continue_cycle_flag = false;
//         for(int i = num_of_result_paths; i >= curr->actual_size; i--)
//         {
//             cout << "Before entering get valid extentions of path stack\n";
//             PathStack* next = curr->GetValidExtentionsOfPathStack(i, score_difference_threshold);
//             cout << "After entering get valid extentions of path stack\n";
//             if(next != curr)
//             {
//                 cout << "Before deleting previous paths\n";
//                 // curr->DeleteInnerPaths();
//                 cout << "Before deleting previous current path stack\n";
//                 delete curr;
//                 cout << "After that all\n";
//                 curr = next;
//                 continue_cycle_flag = true;
//                 break;
//             }
//         }
//     }
//     return curr;
// }

// double ComputeTwoPathsConnectScore(Path* previous, Path* next)
// {
//     double out_score = 0;
//     for(int i = previous->GetLength() - 1; i >= 0; i--)
//     {
//         UniqueReadListRef prev_reads = previous->GetVertexByIndex(i)->GetReadsList();
//         for(int j = 0; i < next->GetLength(); j++)
//         {
//             SetOfFastqSeqRecordsRef next_reads = next->GetVertexByIndex(j)->GetReadsList().GetReadList();
//             int intersection = prev_reads.FindIntersectionOfLists(next_reads);
//             if(intersection > 0)
//             {
//                 out_score += 1;
//             }
//         }
//     }
//     return out_score;
// }

typedef std::map<Vertex*, bool> VertexBoolMap;

void ComputeTwoPathsConnectionPairs(Path* previous, Path* next, std::map<Vertex*, VertexBoolMap>& table_to_extend)
{
    for(int i = previous->GetLength() - 1; i >= 0; i--)
    {
        UniqueReadListRef prev_reads = previous->GetVertexByIndex(i)->GetReadsList();
        for(int j = 0; j < next->GetLength(); j++)
        {
            SetOfFastqSeqRecordsRef next_reads = next->GetVertexByIndex(j)->GetReadsList().GetReadList();
            int intersection = prev_reads.FindIntersectionOfLists(next_reads);
            if(intersection > 0)
            {
                if(table_to_extend.find(previous->GetVertexByIndex(i)) == table_to_extend.end())
                {
                    table_to_extend[previous->GetVertexByIndex(i)] = VertexBoolMap();
                }
                table_to_extend[previous->GetVertexByIndex(i)][next->GetVertexByIndex(j)] = true;
            }
        }
    }
    return;
}

double ComputeTableToExtendSumScore(std::map<Vertex* , VertexBoolMap>& table_to_extend)
{
    double out_score = 0.0;
    int counter = 0;
    for(std::map<Vertex* , VertexBoolMap>::iterator it1 = table_to_extend.begin(); it1 != table_to_extend.end(); it1++)
    {
        for(VertexBoolMap::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        {
            if(it2->second)
            {
                out_score += 1;
            }
            counter++;
        }
    }
    return out_score;
}

Path* UniteTwoPaths(Path* previous, Path* next)
{
    Path* out_path = new Path(*previous);
    for(int i = 0; i < next->GetLength(); i++)
    {
        out_path->AddVertex(next->GetVertexByIndex(i));
    }
    return out_path;
}

PathStack* UniteTwoPathStacks(PathStack* path_stack1, PathStack* path_stack2)
{
    if(path_stack1 == NULL || path_stack2 == NULL)
    {
        return NULL;
    }
    int path_stack_1_next_vertex_coor = path_stack1->GetNextVertexiesCoor();
    int path_stack_2_first_vertex_coor = path_stack2->GetStartVertexCoor();
    if(path_stack_1_next_vertex_coor == -1 || path_stack_2_first_vertex_coor == -1 || path_stack_1_next_vertex_coor != path_stack_2_first_vertex_coor)
    {
        cout << "Previous paths\n";
        std::vector<PathStack*> govno;
        govno.push_back(path_stack1);
        PrintPathStack(govno);
        cout << "Previous path first vertex coordinate is \t" << path_stack1->GetStartVertexCoor() << "\n";
        cout << "Previous path last vertex coordinate is \t" << path_stack1->GetLastVertexCoor() << "\n";
        cout << "Previous path next vertex coordinate is \t" << path_stack_1_next_vertex_coor << "\n";
        cout << "Next paths\n";
        govno.clear();
        govno.push_back(path_stack2);
        PrintPathStack(govno);
        cout << "Next path first vertex coordinate is \t" << path_stack_2_first_vertex_coor << "\n";
        // char c;
        // cin >> c;
        return NULL;
    }

    bool direct_orientation_flag = true;
    if(path_stack1->GetActualSize() < path_stack2->GetActualSize())
    {
        direct_orientation_flag = false;
        PathStack* temp = path_stack1;
        path_stack1 = path_stack2;
        path_stack2 = temp;
    }

    std::vector<PathStack*> path_stacks;
    std::vector<double> path_stacks_scores;
    std::vector<int> counter_vector(path_stack1->GetActualSize(), 0);
    while(true)
    {
        std::set<int> temp_set(counter_vector.begin(), counter_vector.end());

        std::set<Path*> minor_paths_included_set;
        for(int i = 0; i < counter_vector.size(); i++)
        {
            minor_paths_included_set.insert(path_stack2->GetPathByIndex(counter_vector.at(i)));
        }
        if(minor_paths_included_set.size() == path_stack2->GetActualSize())
        {
            std::map<Vertex*, VertexBoolMap> table_to_extend;
            std::map<Path*, Path*> path_connection_table;
            for(int i = 0; i < counter_vector.size(); i++)
            {
                // cout << "Before compute two paths connection pairs\n";
                if(direct_orientation_flag)
                {
                    ComputeTwoPathsConnectionPairs(path_stack1->GetPathByIndex(i), path_stack2->GetPathByIndex(counter_vector.at(i)), table_to_extend);
                }
                else
                {
                    ComputeTwoPathsConnectionPairs(path_stack2->GetPathByIndex(counter_vector.at(i)), path_stack1->GetPathByIndex(i), table_to_extend);
                }
                // cout << "After compute two paths connection pairs\n";
                path_connection_table[path_stack1->GetPathByIndex(i)] = path_stack2->GetPathByIndex(counter_vector.at(i));
                // cout << "Path connection table was refreshed\n";

            }

            double total_score = ComputeTableToExtendSumScore(table_to_extend);
            if(total_score != 0)
            {
                std::vector<Path*> temp_paths;
                for(std::map<Path*, Path*>::iterator it = path_connection_table.begin(); it != path_connection_table.end(); it++)
                {
                    if(direct_orientation_flag)
                    {
                        temp_paths.push_back(UniteTwoPaths(it->first, it->second));
                    }
                    else
                    {
                        temp_paths.push_back(UniteTwoPaths(it->second, it->first));
                    }
                }
                PathStack* path_stack_to_add = new PathStack(temp_paths, path_stack1->GetVertexiesTable(), path_stack1->GetMaxSize());
                path_stack_to_add->SetScore(total_score);
                path_stacks.push_back(path_stack_to_add);
                path_stacks_scores.push_back(total_score);
            }
        }

        else if(temp_set.size() == 1 && temp_set.find(path_stack2->GetActualSize() - 1) != temp_set.end())
        {
            break;
        }

        for(int i = counter_vector.size() - 1; i >= 0; i--)
        {
            if(counter_vector.at(i) == path_stack2->GetActualSize() - 1)
            {
                counter_vector.at(i) = 0;
                continue;
            }
            else
            {
                counter_vector.at(i)++;
                break;
            }
        }
    }
    // cout << "Is going to print path stacks before their reduction\n";
    // PrintPathStack(path_stacks);
    int max_1_index = -1;
    int max_2_index = -1;
    double max_1_score = -1;
    double max_2_score = -1;
    for(int i = 0; i < path_stacks_scores.size(); i++)
    {
        if(path_stacks_scores.at(i) > max_1_score)
        {
            max_1_score = path_stacks_scores.at(i);
            max_1_index = i;
            continue;
        }
        if(path_stacks_scores.at(i) > max_2_score)
        {
            max_2_score = path_stacks_scores.at(i);
            max_2_index = i;
            continue;
        }
    }

    // cout << "Max 1 element\t" << max_1_score << "\n";
    // cout << "Max 2 element\t" << max_2_score << "\n";
    if(abs(max_1_score - max_2_score) >= 2 && max_2_index != -1 && max_2_score != -1 && max_1_score != 0)
    {
        for(int i = 0; i < path_stacks_scores.size(); i++)
        {
            if(i != max_1_index)
            {
                path_stacks.at(i)->DeleteInnerPaths();
                delete path_stacks.at(i);
            }
        }
        return path_stacks.at(max_1_index);
    }
    else
    {
        if(direct_orientation_flag)
        {
            return path_stack1;
        }
        else
        {
            return path_stack2;
        }
    }
}

std::vector<PathStack*> UniteNeighbourPathStacks(std::vector<PathStack*> input_path_stacks)
{
    std::vector<PathStack*> final_path_stacks;
    if(input_path_stacks.size() == 0)
    {
        return final_path_stacks;
    }

    std::vector<PathStack*> temp_vector;
    PathStack* current_path_stack_to_extend = input_path_stacks.at(0);

    for(int i = 1; i < input_path_stacks.size(); i++)
    {
        cout << "In unite neighbour path stacks:\t" << "iteration " << i << "\n";
        PathStack* next_path_stack = UniteTwoPathStacks(current_path_stack_to_extend, input_path_stacks.at(i));
        // cout << "United\t";
        if(next_path_stack == current_path_stack_to_extend || next_path_stack == NULL)
        {
            // cout << "Impossible to unite\t";
            final_path_stacks.push_back(current_path_stack_to_extend);
            current_path_stack_to_extend = input_path_stacks.at(i);
        }
        else
        {
            // cout << "United successfully\t";
            current_path_stack_to_extend->DeleteInnerPaths();
            delete current_path_stack_to_extend;
            input_path_stacks.at(i)->DeleteInnerPaths();
            delete input_path_stacks.at(i);
            current_path_stack_to_extend = next_path_stack;
            // std::vector<PathStack*> tmp;
            // tmp.push_back(current_path_stack_to_extend);
            // PrintPathStack(tmp);
        }
    }
    return final_path_stacks;
}

PathStack* PathStack::ContinuePathStack(PathStack* current_path_stack, int num_of_result_paths, int score_difference_threshold)
{
    PathStack* curr = current_path_stack;
    while(true)
    {
        std::vector<int> scores;
        std::vector<PathStack*> possible_path_stacks;
        for(int i = curr->actual_size; i <= num_of_result_paths;i++)
        {
            PathStack* next = curr->GetValidExtentionsOfPathStack(i, score_difference_threshold);
            if(next != curr)
            {
                scores.push_back(next->GetScore());
                possible_path_stacks.push_back(next);
            }
        }
        if(scores.size() == 0)
        {
            cout << "Is going to exit from continue Path Stack\n";
            return curr;
        }
        else if(scores.size() == 1)
        {
            cout << "Before deleting previous paths\n";
            cout << "Before deleting previous current path stack\n";
            delete curr;
            cout << "After that all\n";
            curr = possible_path_stacks.at(0);
        }
        else
        {
            int best_index = -1;
            for(int i = scores.size() - 1; i >= 0; i--)
            {
                bool temp_flag = true;
                for(int j = i - 1; j >= 0; j--)
                {
                    if(scores.at(i) <= scores.at(j))
                    {
                        temp_flag = false;
                        break;
                    }
                }
                if(temp_flag)
                {
                    best_index = i;
                    break;
                }
            }
            for(int i = 0; i < scores.size(); i++)
            {
                if(i != best_index)
                {
                    possible_path_stacks.at(i)->DeleteInnerPaths();
                    delete possible_path_stacks.at(i);
                }
            }
            curr->DeleteInnerPaths();
            delete curr;
            curr = possible_path_stacks.at(best_index);
        }
    }
}

std::vector<Vertex*> GetVertexiesWithCoor(int coor, HashStringVertexPointer* vertexies_name_table)
{
    std::vector<Vertex*> out_vertexies;
    std::vector<std::string> array;
    char *tempBuf = new char[100];
    itoa(coor, tempBuf, 10);
    array.push_back(std::string("A_") + tempBuf);
    array.push_back(std::string("G_") + tempBuf);
    array.push_back(std::string("T_") + tempBuf);
    array.push_back(std::string("C_") + tempBuf);
    array.push_back(std::string("Ins_") + tempBuf);
    delete[] tempBuf;
    for(int i = 0; i < array.size(); i++)
    {
        if(vertexies_name_table->find(array.at(i)) != vertexies_name_table->end())
        {
            out_vertexies.push_back((*vertexies_name_table)[array.at(i)]);
        }
    }
    return out_vertexies;
}

bool ComparePathStacks(PathStack* a, PathStack* b)
{
    return a->GetStartVertexCoor() < b->GetStartVertexCoor();
}

void PathStack::GenerateInitialPathStacks(Graph* graph, int num_of_chr, HashStringVertexPointer* vertexies_name_table, std::string out_print_path)
{
    std::vector<Vertex*> vertexies_to_start = graph->GetAllVertexies().GetInnerVector();
    std::sort(vertexies_to_start.begin(), vertexies_to_start.end(), VertexPointer_Compare);

    std::vector<Vertex*> initial_vertexies = GetVertexiesWithCoor(vertexies_to_start.at(vertexies_to_start.size() - 1)->GetCoor(), vertexies_name_table);
    std::vector<PathStack*> setOfPathStack;
    int index_in_vertexies_to_start = vertexies_to_start.size() - 1;
    bool flag = true;
    while(flag)
    {
        if(initial_vertexies.size() != 0)
        {
            cout << "Current initial vertexies coordinate is\t" << initial_vertexies.at(0)->GetCoor() << "\n";
        }
        std::vector<Path*> temp_paths_vector;
        for(int i = 0; i < initial_vertexies.size(); i++)
        {
            Path* temp_path = new Path();
            temp_path->AddVertex(initial_vertexies.at(i));
            temp_paths_vector.push_back(temp_path);
        }
        PathStack* new_path_stack = new PathStack(temp_paths_vector, vertexies_name_table, num_of_chr);
        new_path_stack = PathStack::ContinuePathStack(new_path_stack, num_of_chr, 1);
        setOfPathStack.push_back(new_path_stack);

        int next_coor = new_path_stack->GetNextVertexiesCoor();
        if(next_coor != -1)
        {
            initial_vertexies.clear();
            initial_vertexies = GetVertexiesWithCoor(next_coor, vertexies_name_table);
        }
        else
        {
            while(new_path_stack->GetLastVertexCoor() >= vertexies_to_start.at(index_in_vertexies_to_start)->GetCoor())
            {
                index_in_vertexies_to_start--;
                if(index_in_vertexies_to_start < 0)
                {
                    flag = false;
                    break;
                }
            }
            if(flag)
            {
                initial_vertexies.clear();
                initial_vertexies = GetVertexiesWithCoor(vertexies_to_start.at(index_in_vertexies_to_start)->GetCoor(), vertexies_name_table);
            }
        }
    }

    cout << "Size of path stack set is\t" << setOfPathStack.size() << "\n";

    cout << "Now is going to print result paths\n";

    ofstream my_file_debug(out_print_path.c_str(),std::ios::out);
    my_file_debug.close();

    // char c;
    cout << "Is going to unite neighbours\n";
    // cin >> c;

    std::sort(setOfPathStack.begin(), setOfPathStack.end(), ComparePathStacks);
    setOfPathStack = UniteNeighbourPathStacks(setOfPathStack);
    for(int i = 0; i < setOfPathStack.size(); i++)
    {
        setOfPathStack.at(i)->PrintAllPaths(out_print_path);
    }
    cout << "Paths are printed\n";
    for(int i = 0; i < setOfPathStack.size(); i++)
    {
        delete setOfPathStack.at(i);
    }
    return;
}

// void PathStack::GenerateInitialPathStacks(Graph* graph, int num_of_chr, HashStringVertexPointer* vertexies_name_table, std::string out_print_path)
// {
//     SetOfRef<Vertex> first_vertexies = graph->GetFirstVertexies();
//     first_vertexies.SortInnerVector(VertexPointer_Compare);
//     std::vector<PathStack*> setOfPathStack;
//
//     do
//     {
//
//         cout << "Size of first vertexies set:\t" << first_vertexies.GetSizeOfSet() << "\n";
//
//         int vertexies_start = 0;
//         int vertexies_end = 0;
//
//         int start_vertex_coor = first_vertexies.GetItemByNum(0)->GetCoor();
//         int end_vertex_coor = 0;
//
//         SetOfRef<Vertex> initial_vertexies;
//
//         while(vertexies_end < first_vertexies.GetSizeOfSet())
//         {
//             // SetOfRef<Path> final_paths;
//
//             end_vertex_coor = first_vertexies.GetItemByNum(vertexies_end)->GetCoor();
//             if(end_vertex_coor == start_vertex_coor)
//             {
//                 initial_vertexies.AddToSet(first_vertexies.GetItemByNum(vertexies_end));
//                 first_vertexies.GetItemByNum(vertexies_end)->IncrimentInclusionInPath();
//                 vertexies_end++;
//             }
//             else
//             {
//                 int temp_coor = initial_vertexies.GetItemByNum(0)->GetCoor();
//                 SetOfRef<Vertex> new_initial_vertexies;
//                 std::vector<std::string> array;
//                 char *tempBuf = new char[100];
//                 itoa(temp_coor, tempBuf, 10);
//                 array.push_back(std::string("A_") + tempBuf);
//                 array.push_back(std::string("G_") + tempBuf);
//                 array.push_back(std::string("T_") + tempBuf);
//                 array.push_back(std::string("C_") + tempBuf);
//                 array.push_back(std::string("Ins_") + tempBuf);
//                 delete[] tempBuf;
//                 for(int i = 0; i < array.size(); i++)
//                 {
//                     if(vertexies_name_table->find(array.at(i)) != vertexies_name_table->end())
//                     {
//                         new_initial_vertexies.AddToSet((*vertexies_name_table)[array.at(i)]);
//                     }
//                 }
//
//                 cout << "Initial paths set size is:\t" << initial_vertexies.GetSizeOfSet() << "\n";
//                 vertexies_start = vertexies_end;
//                 start_vertex_coor = first_vertexies.GetItemByNum(vertexies_start)->GetCoor();
//
//                 std::vector<Path*> temp_paths_vector;
//                 for(int i = 0; i < new_initial_vertexies.GetSizeOfSet(); i++)
//                 {
//                     Path* temp_path = new Path();
//                     temp_path->AddVertex(new_initial_vertexies.GetItemByNum(i));
//                     temp_paths_vector.push_back(temp_path);
//                 }
//                 PathStack* new_path_stack = new PathStack(temp_paths_vector, vertexies_name_table, num_of_chr);
//                 new_path_stack = PathStack::ContinuePathStack(new_path_stack, num_of_chr, 2);
//                 setOfPathStack.push_back(new_path_stack);
//
//                 initial_vertexies.Clear();
//             }
//         }
//         if(initial_vertexies.GetSizeOfSet() != 0)
//         {
//             int temp_coor = initial_vertexies.GetItemByNum(0)->GetCoor();
//             SetOfRef<Vertex> new_initial_vertexies;
//             std::vector<std::string> array;
//             char *tempBuf = new char[100];
//             itoa(temp_coor, tempBuf, 10);
//             array.push_back(std::string("A_") + tempBuf);
//             array.push_back(std::string("G_") + tempBuf);
//             array.push_back(std::string("T_") + tempBuf);
//             array.push_back(std::string("C_") + tempBuf);
//             array.push_back(std::string("Ins_") + tempBuf);
//             delete[] tempBuf;
//
//             for(int i = 0; i < array.size(); i++)
//             {
//                 if(vertexies_name_table->find(array.at(i)) != vertexies_name_table->end())
//                 {
//                     new_initial_vertexies.AddToSet((*vertexies_name_table)[array.at(i)]);
//                 }
//             }
//
//             cout << "Initial paths set size is:\t" << initial_vertexies.GetSizeOfSet() << "\n";
//             vertexies_start = vertexies_end;
//             start_vertex_coor = first_vertexies.GetItemByNum(vertexies_start)->GetCoor();
//
//             std::vector<Path*> temp_paths_vector;
//             for(int i = 0; i < new_initial_vertexies.GetSizeOfSet(); i++)
//             {
//                 Path* temp_path = new Path();
//                 temp_path->AddVertex(new_initial_vertexies.GetItemByNum(i));
//                 temp_paths_vector.push_back(temp_path);
//             }
//             PathStack* new_path_stack = new PathStack(temp_paths_vector, vertexies_name_table, num_of_chr);
//             new_path_stack = PathStack::ContinuePathStack(new_path_stack, num_of_chr, 2);
//             setOfPathStack.push_back(new_path_stack);
//
//             initial_vertexies.Clear();
//         }
//         // cout << "Before filling not included simplify vertexies\n";
//         graph->FillNotIncludedInPathVertexies();
//         first_vertexies.Clear();
//         first_vertexies = graph->GetNotIncludedInPathVertexies();
//         // cout << "After filling not included simplify vertexies\n";
//
//         break;// необходимо удалить эту строку, потому что это фигня какая-то
//     }while(first_vertexies.GetSizeOfSet() > 0);
//
//     cout << "Size of path stack set is\t" << setOfPathStack.size() << "\n";
//
//     cout << "Now is going to print result paths\n";
//
//     ofstream my_file_debug(out_print_path.c_str(),std::ios::out);
//     my_file_debug.close();
//
//     for(int i = 0; i < setOfPathStack.size(); i++)
//     {
//         setOfPathStack.at(i)->PrintAllPaths(out_print_path);
//     }
//     cout << "Paths are printed\n";
//     for(int i = 0; i < setOfPathStack.size(); i++)
//     {
//         delete setOfPathStack.at(i);
//     }
//     return;
// }
