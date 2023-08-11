/*This program simulates an one hour long office hour session. There are some assumptions being made
such as the arrival probabilty of students and the time range a student will take up with the professor
in the office hour. We run this simulation 100 times. In each simulation, we calculate the averages of that
individual simulation, add them to the data obj, and at the end of 100 sims, extract data from data obj to
calculate averages across all sims. Note, srand() and rand() may lead to segfaults if compiler is not update
on c++11 standard. COMPILE WITH C++ 11 STANDARD
I have made 4 header files for this assignment:
data.h, HW7OH.h, students.h, and bool_source.h
They must be in the same directory as this file for successful compilation. Also need the topics text file. Map master 
file will be created upon running if it does not exist.
*/
#include <iostream>
#include <cstdlib> // Provides rand, RAND_MAX, size_t
#include <time.h> //provides time() which is used to seed rand()	

//self made header files for HW: 
#include "HW8OH.h"
#include "students.h"
#include "data.h"
#include "bool_source.h"


using namespace std;

int main(){
	//local average vars
	double avgWaitTime; //avg waitime across all sims
	double avgWorkTime; //avg time student spends with prof during oh visit across all sims
	double avgExtraTime; //avg extra time prof spent in oh across all sims.

	//create a vector of student objects
	vector<students> studentVect;
	for(size_t i = 0; i < 51; i++){//here we assume the professor has a roster of 50 students. 
		students studentObj;
		studentObj.set_name(to_string(i+1));
		studentVect.push_back(studentObj);
	}

	//instantiate data obj.
	data simData;

	//create topics array and get topics from txt file
	string topics[100];//just gave it a fixed capacity
	int topicArrSize = getTopics("topics.txt", topics);//works


	//simulation runtime - run the sim this many times
	int runtime = 100; 
	srand(time(0));//seed 

	for(int i = 0; i < runtime; i++){
		simulation(.30, simData, topics, topicArrSize, i, studentVect);
	}

	generateStudentReports("MapMasterFile");//generates individual student reports containing their name
	// and what topics they asked across all 100 sims.

	//calcuate averages from simData obj - the data obj has the sum of averages of each sim.
	//therefore we are calculating average across all sims of an individual sim's averages
	avgWaitTime = simData.get_waitTimeSum() / (runtime);
	avgWorkTime = simData.get_workingTimeSum() / runtime;
	avgExtraTime = simData.get_extraTimeSum() / runtime;

	cout << "\n"; 
	cout << "assumptions: " << endl;
	cout << "Assume a 30 percent chance of a student arriving to office hours. Also assume a student will"; 
	cout << " take between 1-6 minutes in office hours." << endl;
	cout << "\n";
	cout << "A student is also given a priority, indicated by a random integer assigned to them ranging";
	cout << " from 1-10. Highest priority students see professor first." << endl;
	cout << "\n";
	cout << "simulations completed: " << runtime;
	cout <<"\n";
	cout << "average wait time across sims: " << avgWaitTime;
	cout <<"\n";
	cout << "average time student spent with professor during office hour visit across sims: " << avgWorkTime;
	cout <<"\n";
	cout << "average time professor spent after office hour time exceeded one hour across sims: " << avgExtraTime;
	cout <<"\n";

	sortMasterMapFile("MapMasterFile", "studentName", "ascending");
	//sortByName("MapMasterFile", "questionTopic", "descending"); this line works but multiple function calls to sortByName 
	//will overwrite the sorted output file generated by the function.

	searchSortedMasterMapFile("student", "1", studentVect);

	searchSortedMasterMapFile("topic", "Stacks", studentVect);

	return 0;
}









