#include "AssemblyStatistics.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void AssemblyStatistics::CheckAndFillPETable(FastqSequenceRecord* read_to_add, int position)
{
    std::string name_to_add = read_to_add->GetSequenceName();
    int sizeOfRead = name_to_add.length();
    int posDishB = sizeOfRead - 1;
    std::string baseName = name_to_add.substr(0,posDishB);
    std::string numB = name_to_add.substr(posDishB,1);
    std::string nameToSearch = baseName;

    if(numB == "1")
    {
        nameToSearch = nameToSearch + "2";
    }

    else
    {
        nameToSearch = nameToSearch + "1";
    }

    if(this->read_existence->find(nameToSearch) != this->read_existence->end())
    {
        FastqSequenceRecord* pair_for_adding_read = (*read_existence)[nameToSearch];
        if(position != -1 && (*this->read_name_position_hash)[nameToSearch] != -1)
        {

            int paired_read_position = (*this->read_name_position_hash)[nameToSearch];
            int distance = position >= paired_read_position ? position - paired_read_position + 1 : paired_read_position - position + 1;

            if(distance <= 3000 && distance >= 500) // тут тоже должен быть порог
            {
              this->pe_reads_counter += 1; //это количество пар
              (*this->PE_table)[read_to_add] = pair_for_adding_read;
              (*this->PE_table)[pair_for_adding_read] = read_to_add;
              this->average_pe_distance += distance;

              if(distance > this->longest_pe_distance)
              {
                  this->longest_pe_distance = distance;
              }
              if(distance < this->shortest_pe_distance || shortest_pe_distance == -1)
              {
                  this->shortest_pe_distance = distance;
              }

              if(this->PE_reads_distance_hist->find(distance) == this->PE_reads_distance_hist->end())
              {
                  (*this->PE_reads_distance_hist)[distance] = 1;
              }
              else
              {
                  (*this->PE_reads_distance_hist)[distance] = (*this->PE_reads_distance_hist)[distance] + 1;
              }
            }
        }
    }

    return;
}

void AssemblyStatistics::CheckAndFillSETable(FastqSequenceRecord* read_to_add, int position_in_genome)
{
    // cout<<"AddingRead"<<"\n";
    if(this->read_existence->find(read_to_add->GetSequenceName()) != this->read_existence->end())
    {
        char c;
        cout<<"Oooops,read exists"<<"\n";
        cin>>c;
        return;
    }
    else
    {
        CheckAndFillPETable(read_to_add, position_in_genome);
    }

    this->reads_counter++;
    (*this->read_existence)[read_to_add->GetSequenceName()] = read_to_add;
    (*this->read_name_position_hash)[read_to_add->GetSequenceName()] = position_in_genome;

    int temp_length = read_to_add->GetSeqTextLength();
    this->average_read_length += temp_length;

    if(temp_length < this->shortest_read_length || this->shortest_read_length == -1)
    {
        this->shortest_read_length = temp_length;
    }

    if(temp_length > this->longest_read_length)
    {
        this->longest_read_length = temp_length;
    }
    if(read_length_hist->find(temp_length) == read_length_hist->end())
    {
        (*read_length_hist)[temp_length] = 1;
        return;
    }
    else
    {
        (*read_length_hist)[temp_length]++;
        return;
    }
}

void AssemblyStatistics::ComputeReadsProbabilityDistribution()
{
    this->probabilities = new double[this->longest_read_length];
    for(int l = 1; l <= this->longest_read_length; l++)
    {
        double result = 0;
        int counter = 0;
        if(l < this->shortest_read_length)
        {
            counter = shortest_read_length - l;
        }
        while(l + counter <= this->longest_read_length)
        {
            if(this->read_length_hist->find(l + counter) != this->read_length_hist->end())
            {
                result += (counter + 1.0) / (double(l) + counter) * double((*this->read_length_hist)[l + counter]) / this->reads_counter;
            }
            counter++;
        }
        this->probabilities[l - 1] = result; //каждый элемент j-1 этого массива  содержит вероятность двум мозициям в геноме, расстояние между которыми j, быть покрытыми ридом. Расстояние j = j2 - j1 + 1.
    }
}

void AssemblyStatistics::ComputePEReadsDistanceDistribution()
{
    int distance_upper_limit = this->longest_pe_distance + (this->longest_read_length - 1);//я тут что-то поменял, хз, был нетрезв
    int distance_lower_limit = this->shortest_pe_distance - this->longest_read_length + 1;//и тут тоже
    this->PEprobabilities = new double[distance_upper_limit];
    for(int l = distance_lower_limit; l <= distance_upper_limit; l++)
    {
        if(l < distance_lower_limit)
        {
            this->PEprobabilities[l - 1] = 0;
            continue;
        }
        double sum = 0;
        for(int read_length_1 = this->shortest_read_length; read_length_1 <= this->longest_read_length; read_length_1++)
        {
            for(int read_length_2 = this->shortest_read_length; read_length_2 <= this->longest_read_length; read_length_2++)
            {
                for(int distance_between_reads = this->shortest_pe_distance; distance_between_reads <= this->longest_pe_distance; distance_between_reads++)
                {
                    double temp = 0;
                    if(distance_between_reads > l && (read_length_1 - distance_between_reads + l >= 0))
                    {
                        temp = min(read_length_1 - distance_between_reads + l, read_length_2);
                    }
                    else if(distance_between_reads <= l && (read_length_2 - l + distance_between_reads >= 0))
                    {
                        temp = min(read_length_2 - l + distance_between_reads, read_length_1);
                    }
                    else
                    {
                        temp = 0;
                    }
                    if(this->PE_reads_distance_hist->find(distance_between_reads) != this->PE_reads_distance_hist->end() &&
                       this->read_length_hist->find(read_length_1) != this->read_length_hist->end() && this->read_length_hist->find(read_length_2) != this->read_length_hist->end())
                    {

                        double current_reads_probability = double((*this->read_length_hist)[read_length_1]) * double((*this->read_length_hist)[read_length_2]) / double(this->reads_counter) / double(this->reads_counter);
                        double current_distance_reads_probability = double((*this->PE_reads_distance_hist)[distance_between_reads]) / double(this->pe_reads_counter);

                        sum += (temp / double(read_length_1)) * current_reads_probability * current_distance_reads_probability;
                    }
                }
            }
        }
        this->PEprobabilities[l - distance_lower_limit] = sum;
    }
}

double AssemblyStatistics::GetMarkovProbability(int source_coor, int middle_coor, int last_coor)
{
    double nominator =  this->GetProbabilityForReadToCoverBySE(source_coor, last_coor);
    double denominator = this->GetProbabilityForReadToCoverBySE(source_coor, middle_coor);
    if(denominator == 0)
    {
        return 0;
    }
    else
    {
        return 1.0 - nominator / denominator;
    }
}
