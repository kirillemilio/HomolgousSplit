#ifndef ADDCLASSES
#define ADDCLASSES

#include <string>
#include <vector>
#include <hash_map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "string_hash_compare.h";
#include <math.h>
#include <iostream>
#include <map>
#include <functional>
#include <limits>
#include <set>
#include "Vertex.h"
#include "Graph.h"
// #include "AssemblyStatistics.h"
#include <boost/math/distributions/binomial.hpp>


typedef __gnu_cxx::hash_map<unsigned,int> HashUnsignedInt;
typedef __gnu_cxx::hash_map<int,int> HashIntInt;
typedef	 std::map<FastqSequenceRecord*, FastqSequenceRecord*> HashToMapPE;
typedef __gnu_cxx::hash_map<std::string, int, string_hash_compare> HashStringInt;
typedef __gnu_cxx::hash_map<std::string, FastqSequenceRecord*, string_hash_compare> HashStringFastqSeqRecordPointer;
typedef std::map<SimplifyVertex*, int> HashSimplifyVertexInt;


int abs_modul(int a);

double accumulate(vector<double>* v);


bool VertexPointer_Compare(Vertex*& a, Vertex*& b);

bool SimplifyVertexPointer_Compare(SimplifyVertex*& a, SimplifyVertex*& b);

//
// double ComputeL0Norm(HashSimplifyVertexInt& ideal_table, HashSimplifyVertexInt& table_to_compare);
//
//
// double CountTriangleDistAverageScore(std::vector<double>* scores);
//
// double ComputeBinomialDistributionPDF(double probability, int N, int k);
// double ComputeBinomialDistributionCDF(double probability, int N, int k);
// double ComputePoissonDistributionPDF(double mean, int point);

class RHPosteriorDistribution
{

private:

    double** base_array;
    int chr_num;
    int num_reads_min;
    int num_reads_max;
    int first_vertex_coor;
    int last_vertex_coor;

public:

    RHPosteriorDistribution(int chr_num, int num_reads_min, int num_reads_max, int first_vertex_coor, int last_vertex_coor) // first_vertex_coor < last_vertex_coor
    {
        this->chr_num = chr_num;
        this->num_reads_min = num_reads_min;
        this->num_reads_max = num_reads_max;
        this->first_vertex_coor = first_vertex_coor;
        this->last_vertex_coor = last_vertex_coor;
        this->base_array = new double*[chr_num];
        for(int i = 0; i < chr_num; i++)
        {
            base_array[i] = new double[num_reads_max - num_reads_min + 1];
        }
    }

    ~RHPosteriorDistribution()
    {
        for(int i = 0; i < chr_num; i++)
        {
            delete[] base_array[i];
        }
        delete[] base_array;
    }

    void SetValue(int chr_index, int num_of_reads, double probability)
    {
        if((num_of_reads >= this->num_reads_min) && (num_of_reads <= this->num_reads_max) && (chr_index >= 0) && (chr_index < chr_num))
        {
            this->base_array[chr_index][num_of_reads - this->num_reads_min] = probability;
        }
    }

    double GetValue(int chr_index, int num_of_reads)
    {
        if((num_of_reads >= this->num_reads_min) && (num_of_reads <= this->num_reads_max) && (chr_index >= 0) && (chr_index < chr_num))
        {
            return this->base_array[chr_index][num_of_reads - this->num_reads_min];
        }
        else
        {
            return -1.0;
        }
    }

    // void ComputePosteriorProbability(double coverage_one_chromosome, AssemblyStatistics* as, std::vector<double> apriori);
    // double GetConditionedProbabilityToHaveIntersectedReads(int new_num_intersected, int vertex_further_first_coor);
    // double KullbackLeiblerDivergence(const RHPosteriorDistribution& p1);
};

#endif
