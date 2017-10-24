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

#include "entropy.h"

using namespace std;

//BranchHistoryMap
BranchHistory::BranchHistory(int n){
	this->bhl = n;
	this->bhrMask = ((uint64_t)1 << n) - 1;
	this->bhr = 0;
}

uint64_t BranchHistory::branchInput(int taken){
	this->bhr <<= (uint64_t)1;
	this->bhr += (uint64_t)taken;
	this->bhr &= this->bhrMask;
	return this->bhr;
}

//EntroNumber
EntroNumber::EntroNumber(){
	this->entropy = 0;
	this->number = 0;
}

EntroNumber::EntroNumber(double entropy_a, uint64_t number_a){
	this->entropy = entropy_a;
	this->number = number_a;
}

double EntroNumber::getEntropy(){
	return entropy;
}

uint64_t EntroNumber::getNumber(){
	return number;
}

void EntroNumber::setEntropy(double entropy_a){
	this->entropy = entropy_a;
}

void EntroNumber::setNumber(uint64_t number_a){
	this->number = number_a;
}

//EntropyFast
EntropyFast::EntropyFast(int n) /*: Entropy(n)*/ {
	this->n = n;
	max = 1 << n;
	patternList = std::vector<uint64_t>(max, 0);
	reset();
}

EntropyFast::EntropyFast(int n, EntropyFast* entropy_i) /*: Entropy(n)*/ {
	this->n = n;
	max = 1 << n;
	patternList = std::vector<uint64_t>(max, 0);
	reset();
	addPattern(entropy_i);
}

EntropyFast::~EntropyFast(){
	patternList.clear();
}

uint64_t EntropyFast::getNumber(){
	return number;
}

void EntropyFast::reset() {
	//cout << "RESET" << endl;
	number = 0;
	for(int i=0; i<max; i++)
		patternList[i] = 0;
}

std::vector<uint64_t> EntropyFast::getPatternList(){
	return patternList;
}

uint64_t EntropyFast::addPattern(uint64_t index){
	number++;
	patternList[index] ++;
	return patternList[index];
}

uint64_t EntropyFast::addPattern(uint64_t index, uint64_t number_a){
	number+=number_a;
	patternList[index] += number_a;
	return patternList[index];
}

void EntropyFast::addPattern(EntropyFast* entropyFast_i){
	std::vector<uint64_t> patternList_i = ((EntropyFast*) entropyFast_i)->getPatternList();
	for(int i=0; i<max; i++){
		number += patternList_i[i];
		patternList[i] += patternList_i[i];
	}
}

map<uint64_t, EntroNumber* > EntropyFast::analyse(){
	if(number == 0){
		map<uint64_t, EntroNumber* > entropyNumbers;
		for(int i=(n-1); i >= 0; i--){
			entropyNumbers[i-1] = new EntroNumber(0, 0);
		}
		return entropyNumbers;
	} else {
		return getEntropy(n-1, patternList, number);
	}
}

map<uint64_t, EntroNumber* > EntropyFast::getEntropy(int maxN, std::vector<uint64_t> patternListGet, uint64_t number){
	//getEntropy
	map<uint64_t, EntroNumber* > entropyNumbers;
	for(int i=maxN; i >= 0; i--){
		double entropy = 0;
		uint64_t newNumber = 0;
		//calculate Entropy
		for(int j = 0; j < (1 << (i+1)); j+=2){
			uint64_t n1 = patternListGet[j];
			uint64_t n2 = patternListGet[j + 1];
			double entropyTemp = 0;

			if(ENT){
				// - p*log(p) - (1-p)*log(1-p)
				entropyTemp -= ((double)n1/(n1+n2)) * log((double)n1/(n1+n2))/log(2) * (n1+n2);
				entropyTemp -= ((double)n2/(n1+n2)) * log((double)n2/(n1+n2))/log(2) * (n1+n2);
			}
			if(LIN){
				// lineair: (n1/n2 OF n2/n1)
				entropyTemp += min(n1, n2) * 2; // / (n1+n2) * (n1+n2)
			}

			if(WARMUP_BRANCH){
				if(n1 != n2){
					entropyTemp += 1;
				}
			}
			entropy += entropyTemp;
			newNumber += (n1 + n2);

		}
		//if(newNumber != number)
		//	cout << newNumber << " != " << number << endl;
		if(newNumber > 0){
			entropyNumbers[i] = new EntroNumber(entropy/newNumber, newNumber);
		} else {
			entropyNumbers[i] = new EntroNumber(0, 0);
		}

		//Make the table halve the size.
		uint32_t newSize = (uint32_t)1<<i;
		for(uint32_t j = 0; j < newSize; j++){
			patternListGet[j] += patternListGet[newSize + j];
		}
	}

	//Entropy of highest Bhr is first in this list.
	return entropyNumbers;
}

map<uint64_t, EntroNumber* > Trace::getEntropy(){
	return entropyTrace;
}

map<uint64_t, map<uint64_t, EntroNumber* > > Trace::getEntropyPerBranch(){
	return entropyPerBranch;
}

void Trace::addEntropyPerBranch (uint64_t branch, map<uint64_t, EntroNumber* > list_v){
	entropyPerBranch[branch] = list_v;
}

void Trace::calculateTraceEntropy(){
	//loop over all unique branches
	for(map<uint64_t, map<uint64_t, EntroNumber* > >::iterator It_1 = entropyPerBranch.begin(); It_1 != entropyPerBranch.end(); ++It_1){
		//loop over all possible BHL
		for(map<uint64_t, EntroNumber* >::iterator It_2 = It_1->second.begin(); It_2 != It_1->second.end(); ++It_2){
			//Init entropyTrace map
			if(entropyTrace.count(It_2->first) == 0){
				entropyTrace[It_2->first] = new EntroNumber();
			}
			entropyTrace[It_2->first]->setEntropy( entropyTrace[It_2->first]->getEntropy() + It_2->second->getEntropy() * It_2->second->getNumber());
			entropyTrace[It_2->first]->setNumber ( entropyTrace[It_2->first]->getNumber()  + It_2->second->getNumber() );
		}
	}

	for(map<uint64_t, EntroNumber* >::iterator It = entropyTrace.begin(); It != entropyTrace.end(); ++It){
		if(It->second->getNumber() > 0){
        	It->second->setEntropy(It->second->getEntropy() / It->second->getNumber());
        } else {
        	if(It->second->getEntropy() != 0){
        		cerr << "No branches but still entropyValue" << endl;
        	}
        }
    }
}

//EntropySimulator
EntropySimulator::EntropySimulator(int ph_bits, int bh_bits, string folder, int max_bhr, int max_ip){
	PH_BITS = ph_bits;
	PH_BITS_MASK = (((uint64_t)1 << PH_BITS) - 1);
	BH_BITS = bh_bits;
	BH_BITS_MASK = (((uint64_t)1 << BH_BITS) - 1);
	MAX_BHR = max_bhr;
	MAX_IP = max_ip;

	uniqueListMap.clear();
	uniqueListBhr.clear();

	branchCount = 0;
	uniqueBranchCount = 0;

	folderName = folder;
}

EntropySimulator::~EntropySimulator(){
	reset();
}

uint32_t EntropySimulator::addBranch(uint64_t pc, int taken, bool acceptNew){
	uint64_t ip_bh = pc & BH_BITS_MASK;
    uint64_t ip_ph = pc & PH_BITS_MASK;

    branchCount++;

    if(acceptNew && uniqueListMap.count(ip_ph) == 0){
    	uniqueBranchCount ++;
        uniqueListMap[ip_ph] = new EntropyFast(MAX_BHR + 1);
    }
    if(acceptNew && uniqueListBhr.count(ip_bh) == 0){
        uniqueListBhr[ip_bh] = new BranchHistory(MAX_BHR + 1);
    }

	if(acceptNew || uniqueListBhr.count(ip_bh) != 0){
	    uint64_t index = uniqueListBhr.at(ip_bh)->branchInput(taken);
	    if(acceptNew || uniqueListMap.count(ip_ph) != 0){
	    	uniqueListMap.at(ip_ph)->addPattern(index, acceptNew);
	    }
	}

	return uniqueListMap.size();
}

void EntropySimulator::analyseEntropy(Trace* trace, unordered_map<uint64_t, EntropyFast*> uniqueListMap){
	//Loop over all branch tables
    unordered_map<uint64_t, EntropyFast*>::iterator listIt;
    for (listIt = uniqueListMap.begin(); listIt != uniqueListMap.end(); ++listIt){
    	EntropyFast* ent = new EntropyFast(MAX_BHR + 1, listIt->second);
        trace->addEntropyPerBranch(listIt->first, ent->analyse());
        delete ent;
    }

    trace->calculateTraceEntropy();
}

void EntropySimulator::collectTraceInformation(map<int, Trace*> &traceInfo){

	//First perfectly local
	traceInfo[PH_BITS] = new Trace();
	analyseEntropy(traceInfo[PH_BITS], (uniqueListMap));

	//Then the rest.
	for(int i = MAX_IP; i >= 0 ; i--){
		unordered_map<uint64_t, EntropyFast*>::iterator listIt;
		vector<uint64_t> mapIndex;
		//It seems that you are not allowed to grow a list when iterating over it.
		//So first create a list of all key elements
		for(listIt = uniqueListMap.begin(); listIt != uniqueListMap.end(); ++listIt) {
			mapIndex.push_back(listIt->first);
		}

		for(uint64_t ip_i = 0; ip_i < mapIndex.size(); ip_i++){
			listIt = uniqueListMap.find(mapIndex.at(ip_i));
			uint64_t ip_v = listIt->first;
			uint64_t newIndex = ip_v & (((uint64_t)1 << i) - 1);
			if(newIndex != ip_v){
				if(uniqueListMap.count(newIndex) == 0)
					uniqueListMap[newIndex] = new EntropyFast(MAX_BHR + 1);
				uniqueListMap.at(newIndex)->addPattern(uniqueListMap.at(ip_v));

				delete listIt->second;
				uniqueListMap.erase(ip_v);
			}
		}

		traceInfo[i] = new Trace();
		analyseEntropy(traceInfo[i], (uniqueListMap));
	}
}

void EntropySimulator::reset(){
	//Reset the branch history and branch count for the next window
	for(unordered_map<uint64_t, BranchHistory*>::iterator ii = uniqueListBhr.begin(); ii != uniqueListBhr.end(); ++ii)
		delete ii->second;
	uniqueListBhr.clear();

	for(unordered_map<uint64_t, EntropyFast*>::iterator ii = uniqueListMap.begin(); ii != uniqueListMap.end(); ++ii)
		delete ii->second;
	uniqueListMap.clear();

	for(map<int, map<uint64_t, map<uint64_t, EntroNumber* > > >::iterator It_1 = entropyPerBranchPerPHB.begin(); It_1 != entropyPerBranchPerPHB.end(); ++It_1){
		for(map<uint64_t, map<uint64_t, EntroNumber* > >::iterator It_2 = It_1->second.begin(); It_2 != It_1->second.end(); ++It_2){
			for(map<uint64_t, EntroNumber* >::iterator It_3 = It_2->second.begin(); It_3 != It_2->second.end(); ++It_3){
				delete It_3->second;
			}
			It_2->second.clear();
		}
		It_1->second.clear();
	}
	entropyPerBranchPerPHB.clear();

	branchCount = 0;
	uniqueBranchCount = 0;
}

void EntropySimulator::getOutput(map<int, map<uint64_t, EntroNumber* > > &entropyPerPHB, map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > &entropyPerBranchPerPHB){

	if(uniqueListMap.size() > 0 || uniqueListBhr.size() > 0){
		map<int, Trace*> traceInfo;
		collectTraceInformation(traceInfo);

		for(map<int, Trace*>::iterator ii = traceInfo.begin(); ii != traceInfo.end(); ++ii){
			entropyPerPHB[ii->first] = ii->second->getEntropy();
			entropyPerBranchPerPHB[ii->first] = ii->second->getEntropyPerBranch();
			delete ii->second;
		}
	}
}

//TourEntropySimulator
TourEntropySimulator::TourEntropySimulator(){
	branchCount = 0;
}

TourEntropySimulator::~TourEntropySimulator(){
	//cout << "DELETE" << endl;
}

void TourEntropySimulator::reset(){
	branchCount = 0;
}

void TourEntropySimulator::addBranch(){
	branchCount++;
}

void TourEntropySimulator::getOutput(map<int, map<uint64_t, EntroNumber* > > &entropyPerPHB, map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > &globalInput, map<int, map<uint64_t, map<uint64_t, EntroNumber* > > > &localInput){
	// IP BITS
	for(map<int, map<uint64_t, map<uint64_t, EntroNumber* > > >::iterator It_1 = globalInput.begin(); It_1 != globalInput.end(); ++It_1){
		// BRANCH PC
		for(map<uint64_t, map<uint64_t, EntroNumber* > >::iterator It_2 = It_1->second.begin(); It_2 != It_1->second.end(); ++It_2){
			// BRANCH History
			for(map<uint64_t, EntroNumber* >::iterator It_3 = It_2->second.begin(); It_3 != It_2->second.end(); ++It_3){
				if(entropyPerPHB[It_1->first].count(It_3->first) == 0)
					entropyPerPHB[It_1->first][It_3->first] = new EntroNumber();
				double minEnt = min(It_3->second->getEntropy(), localInput[It_1->first][It_2->first][It_3->first]->getEntropy());
				entropyPerPHB[It_1->first][It_3->first]->setEntropy(entropyPerPHB[It_1->first][It_3->first]->getEntropy() + minEnt * It_3->second->getNumber());
				entropyPerPHB[It_1->first][It_3->first]->setNumber(entropyPerPHB[It_1->first][It_3->first]->getNumber() + It_3->second->getNumber());

				delete localInput[It_1->first][It_2->first][It_3->first];
				delete globalInput[It_1->first][It_2->first][It_3->first];
			}
			localInput[It_1->first][It_2->first].clear();
			globalInput[It_1->first][It_2->first].clear();
		}
		localInput[It_1->first].clear();
		globalInput[It_1->first].clear();
	}
	localInput.clear();
	globalInput.clear();

	// IP BITS
	for(map<int, map<uint64_t, EntroNumber* > >::iterator It_1 = entropyPerPHB.begin(); It_1 != entropyPerPHB.end(); ++It_1){
		// BRANCH History
		for(map<uint64_t, EntroNumber* >::iterator It_3 = It_1->second.begin(); It_3 != It_1->second.end(); ++It_3){
			It_3->second->setEntropy ( It_3->second->getEntropy() / It_3->second->getNumber() );
			It_3->second->setNumber  ( branchCount );
		}
	}
}
