#ifndef ASSTAT
#define ASSTAT

#include "AdditionClasses.h"

class AssemblyStatistics
{
//расстояние между парными ридами полагается равным количеству оснований с первого нуклеотида первогорид до первого нуклеотида второго рида ВКЛЮЧИТЕЛЬНО, поэтому равно i_2 - i_1 + 1
private:
		bool probabilities_computed;
		int reads_counter;
		int pe_reads_counter;

		int shortest_read_length;
		int longest_read_length;
		int average_read_length;

		int shortest_pe_distance;
		int longest_pe_distance;
		int average_pe_distance;

		HashUnsignedInt* read_length_hist;
		HashUnsignedInt* PE_reads_distance_hist;

		double* probabilities;

		double* PEprobabilities;

		HashToMapPE* PE_table;

		HashStringFastqSeqRecordPointer* read_existence;

		HashStringInt* read_name_position_hash;

		void CheckAndFillSETable(FastqSequenceRecord* read_to_add, int position_in_genome);
		void CheckAndFillPETable(FastqSequenceRecord* read_to_ad, int position);
		void ComputeReadsProbabilityDistribution();
		void ComputePEReadsDistanceDistribution();


public:
		double GetMarkovProbability(int source_coor, int middle_coor, int last_coor);
		AssemblyStatistics()
		{
				read_length_hist = new HashUnsignedInt();
				PE_reads_distance_hist = new HashUnsignedInt();
				PE_table = new HashToMapPE();
				read_existence = new HashStringFastqSeqRecordPointer();
				read_name_position_hash = new HashStringInt();

				PEprobabilities = NULL;
				probabilities = NULL;

				reads_counter = 0;
				shortest_read_length = -1;
				longest_read_length = 0;
				average_read_length = 0;

				pe_reads_counter = 0;
				shortest_pe_distance = -1; // тут должен быть порог
				longest_pe_distance = 0;
				average_pe_distance = 0;
				probabilities_computed = false;
		}

		~AssemblyStatistics()
		{
				delete this->read_length_hist;
				delete this->PE_reads_distance_hist;
				delete this->PE_table;
				delete this->read_existence;
				delete this->read_name_position_hash;
				delete[] this->probabilities;
				delete[] this->PEprobabilities;
				cout << "Assembly statistics was deleted\n===================================================\n";
		}

		bool ProbabilitiesComputed()
		{
				return this->probabilities_computed;
		}

		bool ExistReadName_1(std::string name)
		{
        return this->read_existence->find(name + "::1") != this->read_existence->end();
		}

	    bool ExistReadName_2(std::string name)
	    {
	        return this->read_existence->find(name + "::2") != this->read_existence->end();
	    }

		FastqSequenceRecord* GetFastqSeqRefByName(std::string name)
		{
				if(this->read_existence->find(name) == this->read_existence->end())
				{
						return NULL;
				}
				return (*this->read_existence)[name];
		}

		FastqSequenceRecord* GetPairedRead(FastqSequenceRecord* read)
		{
				if(this->PE_table->find(read) == this->PE_table->end())
				{
						return NULL;
				}
				return (*this->PE_table)[read];
		}

		int GetShortestReadLength() const
		{
				return this->shortest_read_length;
		}

		int GetShortestDistance() const
		{
				return this->shortest_pe_distance;
		}

		int GetLongestReadLength() const
		{
				return this->longest_read_length;
		}

		int GetLongestDistance() const
		{
				return this->longest_pe_distance;
		}

		double GetAverageReadLength() const
		{
				return double(this->average_read_length) / this->reads_counter;
		}

		double GetAveragePEDistance() const
		{
				return double(this->average_pe_distance) / this->pe_reads_counter;
		}

		int GetNumberOfPairEnded() const
		{
				return this->pe_reads_counter;
		}

		int GetNumberOfAllReads() const
		{
				return this->reads_counter;
		}

		int GetNumberOfSingleEndedReads() const
		{
				return this->reads_counter - 2 * this->pe_reads_counter;
		}

		void Add(FastqSequenceRecord* read, int position)
		{
				this->CheckAndFillSETable(read, position);
				this->probabilities_computed = false;
		}

		UniqueReadListRef GetPairsForReadsThatHavePair(Vertex* check_vertex)
		{
				UniqueReadListRef out_list;
				SetOfFastqSeqRecordsRef& temp_reads = check_vertex->GetReadsList().GetReadList();
				for(int i = 0; i < temp_reads.GetSizeOfSet(); i++)
				{
						if(this->PE_table->find(temp_reads.GetItemByNum(i)) != this->PE_table->end())
						{
								out_list.Add((*PE_table)[temp_reads.GetItemByNum(i)]);
						}
				}
				return out_list;
		}

		double GetProbabilityForReadToCoverBySE(int i, int j)
		{
				// cout << "temp vertex start position:\t" << i << "\n";
				// cout << "current vertex position:\t" << j << "\n";

				if(this->ProbabilitiesComputed() == false)
				{
						this->ComputeProbabilities();
				}
				if(this->longest_read_length >= (i > j ? i - j + 1 : j - i + 1))
				{
						// cout << "Distance is:\t" << (i > j) ? i - j : j - i;
						// cout << "\n";
						// cout << "Probability to cover is:\t" << (i > j) ? this->probabilities[i - j] : this->probabilities[j - i];
						// cout << "\n";
						return i > j ? this->probabilities[i - j] : this->probabilities[j - i];
				}
				return 0;
		}

		double GetProbabilityForReadToCoverByPE(int i, int j)
		{
				if(!this->probabilities_computed)
				{
						this->ComputeProbabilities();
				}
				if(this->longest_pe_distance + (this->longest_read_length - 1) >= (i > j ? i - j + 1 : j - i + 1) && this->shortest_pe_distance - (this->longest_read_length - 1) <= (i > j ? i - j + 1 : j - i + 1))
				{
						return i > j ? this->PEprobabilities[i - j - this->shortest_pe_distance + (this->longest_read_length - 1)] : this->PEprobabilities[j - i - this->shortest_pe_distance + (this->longest_read_length - 1)];
				}
				return 0;
		}

		void ComputeProbabilities()
		{
				this->ComputeReadsProbabilityDistribution();
				this->ComputePEReadsDistanceDistribution();
				this->probabilities_computed = true;
		}
		// void PrintPositionsDistanceDistribution(std::string out_file);
		// void PrintPairDistanceStatistics(std::string out_file);
		// void PrintReadLenghtStatistics(std::string out_file);
		void PrintReadLenghtStatistics(std::string out_file)
		{
				std::fstream fileToWrite(out_file.c_str(),std::fstream::out);
				for(HashUnsignedInt::iterator iter = this->read_length_hist->begin(); iter != this->read_length_hist->end(); iter++)
				{
						fileToWrite << iter->first <<","<< iter->second <<"\n";
				}
				fileToWrite.close();
		}

		void PrintPairDistanceStatistics(std::string out_file)
		{
				std::fstream fileToWrite(out_file.c_str(),std::fstream::out);
				for(HashUnsignedInt::iterator iter = this->PE_reads_distance_hist->begin(); iter != this->PE_reads_distance_hist->end(); iter++)
				{
						fileToWrite << iter->first << ","<< iter->second / float(this->pe_reads_counter) <<"\n";
				}
				fileToWrite.close();
		}

		void PrintPositionsDistanceDistribution(std::string out_file)
		{
				std::fstream fileToWrite(out_file.c_str(),std::fstream::out);
				for(int i = 0; i < this->longest_read_length; i++)
				{
						fileToWrite << i + 1 << "," << this->probabilities[i]<<"\n";
				}
				fileToWrite.close();
		}
		void PrintPositionsDistanceDistributionPE(std::string out_file)
		{
				std::fstream fileToWrite(out_file.c_str(),std::fstream::out);
				for(int i = this->shortest_pe_distance - (this->longest_read_length - 1); i <= this->longest_pe_distance + (this->longest_read_length - 1); i++)
				{
						fileToWrite << i << "," << this->PEprobabilities[i]<<"\n";
				}
				fileToWrite.close();
		}
};
#endif
