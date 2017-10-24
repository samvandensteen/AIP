/*
*  This file is part of the program: Architecture Independent Profiler (AIP).
*
*  AIP is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  AIP is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with AIP.  If not, see <http://www.gnu.org/licenses/>.
*
*  Authors: Sam Van den Steen, Ghent University
*						Sander De Pestel, Ghent University
*  Copyright: 2016, Ghent University
*/

#ifndef ENTROPY_H_
#define ENTROPY_H_

#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

using std::string;
using std::map;
using std::unordered_map;
using std::vector;

#define ENT false
#define LIN true
#define WARMUP_BRANCH true

class BranchHistory{
	private:
		uint8_t bhl;
		uint64_t bhr;
		uint64_t bhrMask;
	public:
		BranchHistory(int n);
		uint64_t branchInput(int taken);
};

class EntroNumber {
	private:
		double entropy;
		uint64_t number;
	public:
		EntroNumber();
		EntroNumber(double entropy_a, uint64_t number_a);

		double getEntropy();
		uint64_t getNumber();
		void setEntropy(double entropy_a);
		void setNumber(uint64_t number_a);
};

class EntropyFast /*: public Entropy*/ {
	private:
		uint64_t number;
		int n;
		vector<uint64_t> patternList;
		int max;

	public:
		EntropyFast(int n);
		EntropyFast(int n, EntropyFast* entropy_i);
		~EntropyFast();

		uint64_t getNumber();
		void reset(void);

		uint64_t addPattern(uint64_t index);
		uint64_t addPattern(uint64_t index, uint64_t number);
		void addPattern(EntropyFast* entropyFast_i);

		map<uint64_t, EntroNumber* > analyse();

	private:

		vector<uint64_t> getPatternList();
		map<uint64_t, EntroNumber* > getEntropy(int maxN, vector<uint64_t> patternListGet, uint64_t number);
};


class Trace{
	private:
		map<uint64_t, EntroNumber* > entropyTrace;	//Entropy per BHL
		map<uint64_t, map<uint64_t, EntroNumber* > > entropyPerBranch; //Entropy per BHL per branch
	public:
		map<uint64_t, EntroNumber* > getEntropy();
		map<uint64_t, map<uint64_t, EntroNumber* > > getEntropyPerBranch();
		void addEntropyPerBranch (uint64_t index, map<uint64_t, EntroNumber* > list_v);
		void calculateTraceEntropy();
};

class EntropySimulator{
	private:
		unordered_map<uint64_t, EntropyFast*> uniqueListMap;		//List of all branch counts per branch
		unordered_map<uint64_t, BranchHistory*> uniqueListBhr;	//Branch History per branch
		map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > entropyPerBranchPerPHB;

		int PH_BITS;
		int BH_BITS;
		uint64_t PH_BITS_MASK;
		uint64_t BH_BITS_MASK;
		int MAX_BHR;
		int MAX_IP;
		string folderName;

		uint64_t branchCount;
		uint64_t uniqueBranchCount;

	public:

		EntropySimulator(int ph_bits, int bh_bits, string folder, int max_bhr, int max_ip);
		~EntropySimulator();
		void reset();

		uint32_t addBranch(uint64_t pc, int taken, bool acceptNew);

		void getOutput(map<int, map<uint64_t, EntroNumber* > > &entropyPerPHB, map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > &entropyPerBranchPerPHB);
		void collectTraceInformation(map<int, Trace*>&);
		void analyseEntropy(Trace* trace, unordered_map<uint64_t, EntropyFast*> uniqueListMap);
};

class TourEntropySimulator{
	private:
		uint64_t branchCount;
	public:
		TourEntropySimulator();
		~TourEntropySimulator();
		void reset();
		void addBranch();
		void getOutput(map<int, map<uint64_t, EntroNumber* > > &entropyPerPHB, map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > &globalInput, map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > &localInput);

};

#endif /* ENTROPY_H_ */
