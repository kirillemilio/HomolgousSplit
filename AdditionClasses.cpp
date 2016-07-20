#include "AdditionClasses.h"
// 
// double ComputeBinomialDistributionPDF(double probability, int k, int N)
// {
//     boost::math::binomial binom(N, probability);
//     return pdf(binom, k);
// }
//
// double ComputeBinomialDistributionCDF(double probability, int k, int N)
// {
//     boost::math::binomial binom(N, probability);
//     return cdf(binom, k);
// }
//
// double ComputePoissonDistributionPDF(double mean, int point)
// {
//     boost::math::poisson_distribution poisson(double mean);
//     return pdf(poisson, point);
// }
//

// std::vector<double> ComputePosteriorProbabilityForChromosomes(int coor_i, int coor_j, int num_intersected, int max_num_reads, int num_of_chromosomes, double coverage_one_chromosome, AssemblyStatistics* as std::vector<double> apriori)
// {
//     std::vector<double> out_probs;
//
//     std::vector<double> conditioned_probs;
//     double probs_sum = 0.0;
//
//     double temp_probability_1 = as->GetProbabilityForReadToCoverBySE(coor_i, coor_j);
//
//     for(int i = 0; i < apriori.size(); i++)
//     {
//         double temp_prob = 0.0;
//         for(int j = num_intersected; j <= max_num_reads; j++)
//         {
//             double poisson_prob = ComputePoissonDistributionPDF(coverage_one_chromosome * (i + 1), j);
//             double temp_probability_2 = ComputeBinomialDistributionPDF(temp_probability_1, num_intersected, j);
//             temp_prob += temp_probability_2 * poisson_prob;
//         }
//         probs_sum += temp_prob * apriori.at(i);
//         conditioned_probs.push_back(temp_prob * apriori.at(i));
//     }
//
//     for(int i = 0; i < apriori.size(); i++)
//     {
//         conditioned_probs[i] /= probs_sum;
//     }
//     return out_probs;
// }

int abs_modul(int a)
{
    return a > 0 ? a : -a;
}

double accumulate(vector<double>* v)
{
    double sum_of_elems = 0;
    for(std::vector<double>::iterator it = v->begin(); it != v->end(); ++it)
        sum_of_elems += *it;
    return sum_of_elems;
}

// double ComputeL0NormNormalised(HashSimplifyVertexInt& ideal_table, HashSimplifyVertexInt& table_to_compare)
// {
//     double out_value = 0;
//     int counter = 0;
//     for(HashSimplifyVertexInt::iterator iter = ideal_table.begin(); iter != ideal_table.end(); iter++)
//     {
//         counter++;
//         int temp_value = 1;
//         if(table_to_compare.find(iter->first) != table_to_compare.end())
//         {
//             temp_value -= 1;
//         }
//         out_value += temp_value;
//     }
//     return out_value / counter;
// }

bool VertexPointer_Compare(Vertex*& a, Vertex*& b)
{
    return a->GetCoor() > b->GetCoor();
}

bool SimplifyVertexPointer_Compare(SimplifyVertex*& a, SimplifyVertex*& b)
{
    return a->GetVertexByIndex(0)->GetCoor() > b->GetVertexByIndex(0)->GetCoor();
}

//
// double ComputeL0Norm(HashSimplifyVertexInt& ideal_table, HashSimplifyVertexInt& table_to_compare)
// {
//     // cout << "Starting to compute L0 norm\n";
//     int out_value = 0;
//     for(HashSimplifyVertexInt::iterator iter = ideal_table.begin(); iter != ideal_table.end(); iter++)
//     {
//         // int temp_value = (iter->second != 0) ?  1 : 0;
//         int temp_value = 1;
//         if(table_to_compare.find(iter->first) != table_to_compare.end())
//         {
//             temp_value -= 1;
//         }
//         out_value += temp_value;
//     }
//     // cout << "End computing L0 norm\n";
//     return out_value;
// }
//
//
// double CountTriangleDistAverageScore(std::vector<double>* scores)
// {
//     if(scores->size() == 1)
//     {
//         return scores->at(0);
//     }
//     else if(scores->size() == 2)
//     {
//         return 0.5 * scores->at(0) + 0.5 * scores->at(1);
//     }
//
//     double a = 0.0;
//     double weight_average = 0.0;
//     if(scores->size() % 2 == 0)
//     {
//         a = 1.0 / double(scores->size() * (scores->size() / 2 - 1));
//         for(int i = 0; i < scores->size() / 2; i++)
//         {
//             weight_average += (1.0 / (scores->size() * 2) + a * i) * (scores->at(i) + scores->at(scores->size() - i - 1));
//         }
//     }
//     else
//     {
//         a = 0.5 / ((scores->size() / 2) * (scores->size() / 2));
//         for(int i = 0; i < scores->size() / 2; i++)
//         {
//             weight_average += (1.0 / (scores->size() * 2) + a * i) * (scores->at(i) + scores->at(scores->size() - i - 1));
//         }
//         weight_average += (1.0 / (scores->size() * 2) + a * (scores->size() / 2)) * scores->at(scores->size() / 2);
//     }
//     return weight_average;
// }

// void RHPosteriorDistribution::ComputePosteriorProbability(double coverage_one_chromosome, AssemblyStatistics* as, std::vector<double> apriori)
// {
//     double temp_probability_1 = as->GetProbabilityForReadToCoverBySE(this->last_vertex_coor, this->first_vertex_coor);
//     double denominator = 0.0;
//
//     for(int i = 0; i < this->chr_num; i++)
//     {
//         for(int j = this->num_reads_min; j <= this->num_reads_max; j++)
//         {
//             double poisson_prob = ComputePoissonDistributionPDF(coverage_one_chromosome * (i + 1), j);
//             double temp_probability_2 = ComputeBinomialDistributionPDF(temp_probability_1, this->num_reads_min, j);
//             this->SetValue(i, j, poisson_prob * temp_probability_2 * apriori.at(i));
//             denominator += poisson_prob * temp_probability_2 * apriori.at(i);
//         }
//     }
//
//     for(int i = 0; i < this->chr_num; i++)
//     {
//         for(int j = this->num_reads_min; j <= this->num_reads_max; j++)
//         {
//             double unnormed_prob = this->GetValue(i, j);
//             this->SetValue(i, j, unnormed_prob / denominator );
//         }
//     }
//     return;
// }
//
// double RHPosteriorDistribution::GetConditionedProbabilityToHaveIntersectedReads(int new_num_intersected, int vertex_further_first_coor)
// {
//     double temp_probability_1 = as->GetProbabilityForReadToCoverBySE(this->last_vertex_coor, vertex_further_first_coor);
//     double result = 0.0;
//     for(int i = 0; i < this->chr_num; i++)
//     {
//         for(int j = this->num_reads_min; j <= this->num_reads_max; j++)
//         {
//             double temp_probability_2 = ComputeBinomialDistributionPDF(temp_probability_1, new_num_intersected, j);
//             result += temp_probability_2 * this->GetValue(i, j);
//         }
//     }
//     return result;
// }
//
// double RHPosteriorDistribution::KullbackLeiblerDivergence(const RHPosteriorDistribution& p1)
// {
//     int min_R = 0;
//     if(this->num_reads_max != p1.num_reads_max || this->last_vertex_coor != p1.last_vertex_coor)
//     {
//         return -1.0;
//     }
//     if(this->num_reads_min >= p1.num_reads_min)
//     {
//         min_R = this->num_reads_min;
//     }
//     else
//     {
//         min_R = p1.num_reads_min;
//     }
//     double divergence = 0.0;
//     for(int i = 1; i <= this->chr_num; i++)
//     {
//         for(int j = min_R; j <= this->num_reads_max; j++)
//         {
//             if(this->GetValue(i, j) != 0 && p1.GetValue(i, j) != 0)
//             {
//                 divergence += this->GetValue(i, j) * log(this->GetValue(i, j) / p1.GetValue(i, j));
//             }
//         }
//     }
//     if(divergence < 0)
//     {
//         cout << "Divergence is lower than 0\n";
//     }
//     return divergence;
// }
