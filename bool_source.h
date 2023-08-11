/*BOOL_SOURCE HEADER FILE
the bool_source class was soley created, in the context of HW2 assignment, to allow for the 
implementation of a Query function. This function determines whether a student has arrived or not by using the
probabilty of a student arriving which the bool_source object was instantiated with. Note that rand() is 
seeded in HW2.cpp*/
#ifndef BOOL_SOURCE_H
#define BOOL_SOURCE_H
#include <cassert>
#include <cstdlib>

//this class was created to implement a query function that uses the probability of a student arriving
//passed into sim function (in simulation()) to determine wehther a stduent has arrived or not. Use
//may be seen in simulation function. 
class bool_source{
public:
	bool_source(double p = 0.5) : prob_(p){
		assert(p >= 0);//assert that passed in prob is greater than 0% and less than or equal to 100%
        assert(p <= 1); 
        prob_ = p;
	}
	//member functions
    bool query(){//determine whether a student arrived or not.
		return (rand() < prob_ * RAND_MAX);//i don't even know how query() can access prob_
	}

private: 
	double prob_;//probability passed into bool_source object
};

#endif