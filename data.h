/*DATA HEADER FILE
header file for data class. Created to organize sum of individual simulations. Simulation function
in main extracts these sums and divides them by total number of sims ran to calculate averages across
all simulations ran. 
*/
#ifndef DATA_H
#define DATA_H

class data{
public:
	//constructor
	data() : waitTimeSum_(0), workingTimeSum_(0), extraTimeSum_(0) {}

	//accessors
	double get_waitTimeSum() const {return waitTimeSum_;}
	double get_workingTimeSum() const {return workingTimeSum_;}
	double get_extraTimeSum() const {return extraTimeSum_;}

	//mutators
	void set_waitTimeSum(double wait){waitTimeSum_=wait;}
	void set_workingTimeSum(double time){workingTimeSum_=time;}
	void set_extraTimeSum(double extra){extraTimeSum_=extra;}

private:
	//these averages are across ALL SIMULATIONS
	double waitTimeSum_;//avg time student spends waiting.
	double workingTimeSum_;//avg time students spends with prof during oh visit.
	double extraTimeSum_;//avg time prof spends in oh beyond oh time limit

};

#endif