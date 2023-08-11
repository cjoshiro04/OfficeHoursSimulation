#ifndef HW8OH_H
#define HW8OH_H

/*
This file provides inline implementation for functions called in HW5OH.cpp.
Here contains file reading and outputting functions and the main driving simulation function.
*/

#include <iostream>
#include <cstdlib> // Provides rand, RAND_MAX, size_t
#include <queue> 	
#include "students.h"
#include "data.h"
#include "bool_source.h"
#include <time.h> //provides time() which is used to seed rand()
#include <set>
#include <fstream>//allows for reading in txt files
#include <string>
#include <map>
#include <sstream>
#include <vector>
using namespace std;


//this overload allows priority attribute to be compared between student objects by priority_queue front() and pop()
bool operator <(const students& lhs, const students& rhs){
	return lhs.get_priority() < rhs.get_priority();
}

//returns numbver of elements in topicsArray
int getTopics(string fileName, string* topicsArray){//fileName needs to be in "" and should end with .txt
	int i = 0;
	ifstream myFile;
	myFile.open(fileName);
	string line;
	if(myFile.is_open()){
		while(myFile.good()){//
			getline(myFile, line);
			topicsArray[i] = line;
			i++;
		}
	}
	return i;
}

template <typename T>
void writeOutFile(set<T> setName, int iteration){
	ofstream outfile;
	string fileName = to_string(iteration);//from string header file
	outfile.open(fileName);

	//set<T>::iterator it;
	for(auto it = setName.begin(); it!=setName.end(); it++){
		outfile << *it << endl;
	} 
}

bool isEmpty(string fileName){
	fstream file(fileName);
    return file.peek() == ifstream::traits_type::eof();
}

multimap<string, string> openMapMasterFile(string fileName){
	string line;
	int i = 1;//starting from line 1
	int k = 0;
	string key;
	string value;
	multimap<string, string> m; 
	fstream mapFile(fileName, ios::in | ios::out | ios::app);
	if(mapFile.is_open()){
		if(!isEmpty(fileName)){//if empty, nothing to read
			while(mapFile.good()){
				if(i%2!=0){//every odd number line is key
					getline(mapFile, key);
					i++;
					k++;
				}
				else{
					getline(mapFile, value);//every even number line is value
					i++;
					k++;
				}
				if(k==2){
					m.insert(pair<string, string>(key, value));
					k = 0;
 				}
			}
		}
	}
	return m;
}

template<typename T, typename U>
void writeToMasterMapFile(multimap<T,U> map, string fileName){
	fstream mapFile(fileName, ios::in | ios::out | ios::app);
	if(mapFile.is_open()){
		int i = 1;
		for(auto it = map.begin(); it!=map.end(); it++){
			mapFile << it->first << endl;
			mapFile << it->second << endl;
		}
	}
}

void clearMapMasterFile(string fileName){
	ofstream file(fileName);
}

void generateStudentReports(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Failed to open the file." << endl;
        return;
    }

    map<int, vector<string>> studentTopics;
    map<int, int> studentTopicCount;  // New map to store topic count per student

    int studentId;
    string topic;
    while (inputFile >> studentId >> ws && getline(inputFile, topic)) {
        studentTopics[studentId].push_back(topic);
        studentTopicCount[studentId]++;  // Increment the topic count for the student
    }

    inputFile.close();

    for (const auto& entry : studentTopics) {
        int id = entry.first;
        const vector<string> topics = entry.second;
        int topicCount = studentTopicCount[id];  // Retrieve the topic count for the student

        string reportFilename = "student_report_" + to_string(id) + ".txt";
        ofstream reportFile(reportFilename);
        if (reportFile.is_open()) {
            reportFile << "Student ID: " << id << endl;
            reportFile << "Total # of OH visits: " << topicCount << endl;  // Use topicCount instead of timesAttended
            reportFile << "Topics: ";
            for (const string& topic : topics) {
                reportFile << topic << ", ";
            }
            reportFile << endl;
            reportFile.close();
        } else {
            cout << "Failed to create report file for student ID " << id << endl;
        }
    }
}

void simulation(double arrivalProb, data& obj, string* topics, int numTopics, int iteration, 
	vector<students>& studentVect){
	//check if we need to clear map master file
	if(iteration==0){
		clearMapMasterFile("MapMasterFile");
	}
	//queues
	queue<students*> working; //student currently meeting with prof in this queue
	priority_queue<students*> waiting; //line of students waiting to meet with prof, 
	//students with highest priority leave waiting queue first. 

	set<string> askedTopics;//instantiate topic set
	multimap<string, string> map;//instantiate map

	map = openMapMasterFile("MapMasterFile");

	//objects
	bool_source arriveCheck(arrivalProb);//object of bool_source class

	//local variables - the initialization to zero is important for the local vars.
	int studentCount = 0; //how many students went to oh.
	int waitTime = 0; //sum of wait time
	int timeNeeded = 0; //sum of the amount of time each student takes up in oh.
	int done;//when to push current student out of working queue.
	int extraTime = 0;//time prosfessors spends outside 1 hr
	int serveTime = 0;//same as timeNeeded but used for a different functionality

	//averages
	double avgWait = 0;
	double avgTimeNeeded = 0;

	//one hour for loop
	for(int i = 1; i <= 60; i++){//this might need to be 61
		//check to see if student arrives
		
		if(arriveCheck.query()){// i is being used to seed srand() in query()
			//students student;//create student object :: for old hws
			//in HW8, trying to implement with student vect

			int studentFromVector = rand() % 50 + 1;
			students* student = &studentVect[studentFromVector-1]; //-1 because index 0 is student 1.

			//randomly get topic to assign to student
			int index = rand() % numTopics;
			student->set_topic(topics[index]);

			student->set_arrivalTime(i);//set arrival time of student.
			waiting.push(student);
		}
		if(working.empty() && !waiting.empty()){//if no one in oh, push from waiting queue. also, prevent underflow
			//grab front student from waiting queue
			students* current = waiting.top();
			waiting.pop();
			//get needed data from student obj.
			serveTime = current->get_timeNeeded();
			timeNeeded += current->get_timeNeeded();//time needed is the sum, serve time is used for operation purposes
			waitTime += (i - current->get_arrivalTime());

			//get topic and insert into set
			string topic = current->get_topic();
			askedTopics.insert(topic);

			//update attendedOH vector with current OH iteration
			current->set_attendedOH(to_string(iteration), topic);

			//add name and topic to map
			map.insert({current->get_name(), current->get_topic()}); //note the {}

			//send student to working queue.
			working.push(current); 
			done = i + serveTime;//calculate when student will finish based off their serveTime
			studentCount++;
		}
		if(i == done){//when student done with prof
			working.pop();
		}
	}
	int k = 61;//after oh done but students remain in line. No students will be added to waiting queue.
	while(!waiting.empty()){//after one hr is done.
		extraTime++;//keep track of how long prof remains in oh after one hour
		if(working.empty()){//if no one in oh, push from waiting queue
			students* current = waiting.top();
			waiting.pop();

			serveTime = current->get_timeNeeded();
			timeNeeded += current->get_timeNeeded();
			waitTime += (k - current->get_arrivalTime());

			//get topic and insert into set
			string topic = current->get_topic();
			askedTopics.insert(topic);

			//update attendedOH vector with current OH iteration
			current->set_attendedOH(to_string(iteration), topic);

			//add name and topic to map
			map.insert({current->get_name(), current->get_topic()});

			working.push(current);
			done = k + serveTime;//is this math right?
			studentCount++;
		}
		if(k == done){
			working.pop();
		}

		k++;
	}

	//finally, update data obj sums with averages from sim
	//first calc sim averages
	avgWait = waitTime / studentCount;
	avgTimeNeeded += timeNeeded / studentCount;

	//update data obj
	obj.set_waitTimeSum(obj.get_waitTimeSum()+avgWait);
	obj.set_workingTimeSum(obj.get_workingTimeSum()+avgTimeNeeded);
	obj.set_extraTimeSum(obj.get_extraTimeSum()+extraTime);

	//write out topics asked to a file
	writeOutFile(askedTopics, iteration);

	//write map data to map master file
	writeToMasterMapFile(map, "MapMasterFile");

}

//needed for quick sort algorithm. Like quick.xx but just make it able to handle vector of pairs.
int partition(vector<pair<int, string>>& arr, int low, int high, string column){
	if(column == "studentName"){
		int pivot = arr[high].first;
	    int i = low - 1;

	    for (int j = low; j < high; j++) {
	        if (arr[j].first <= pivot) {
	            i++;
	            swap(arr[i], arr[j]);
	        }
	    }

	    swap(arr[i + 1], arr[high]);
	    return i + 1;
	}
	else if(column == "questionTopic"){
		string pivot = arr[high].second;
	    int i = low - 1;

	    for(int j = low; j < high; j++){
	        if(arr[j].second <= pivot){
	            i++;
	            swap(arr[i], arr[j]);
	        }
	    }
	    swap(arr[i + 1], arr[high]);
	    return i + 1;
	}
	else{
		return -1;
	}
    
}

// QuickSort function for a vector of pairs based on the column element of each pair
void quickSort(vector<pair<int, string>>& arr, int low, int high, string column){
    if(low < high){
        int pivot = partition(arr, low, high, column);
        quickSort(arr, low, pivot - 1, column);
        quickSort(arr, pivot + 1, high, column);
    }
}

//NOTE: the first parameter must be a MapMasterFile generated by this code. 
//the second paramter must be either "studentName" or "questionTopic" exactly.
//the third parameter must be sorting direction which either must be "ascending" or "descending" exactly.
void sortMasterMapFile(string filename, string column, string sortDirection){
    ifstream inputFile(filename);
    if(!inputFile.is_open()){
        cout << "Failed to open the file." << endl;
        return;
    }

    vector<pair<int, string>> namesAndTopics;

    int studentId;
    string topic;
    while(inputFile >> studentId >> ws && getline(inputFile, topic)){
        namesAndTopics.push_back(make_pair(studentId, topic));
    }

    inputFile.close();

    if(sortDirection == "ascending"){
        quickSort(namesAndTopics, 0, namesAndTopics.size() - 1, column);
    } 
    else if(sortDirection == "descending"){
        // Sort in descending order by reversing the vector after sorting in ascending order
        quickSort(namesAndTopics, 0, namesAndTopics.size() - 1, column);
        reverse(namesAndTopics.begin(), namesAndTopics.end());
    }

    ofstream outFile("SortedMasterMap.txt");
    if(!outFile){
        cerr << "Failed to open output file." << endl;
        return;
    }

    //vector<pair<int, string>>::iterator p = namesAndTopics.begin();
    for(vector<pair<int, string>>::iterator it = namesAndTopics.begin(); it != namesAndTopics.end(); it++){
		outFile << "Student: " << it->first << endl;
		outFile << "Topic: " << it->second << endl;
		outFile << "\n";
    }

    outFile.close();

    cout << "Sorted file written to SortedMasterMap.txt file" << endl;
}

void searchSortedMasterMapFile(string studentOrTopic, string target, 
vector<students>& studentVect){//am I just supposed to output to
	//console or to a file?
	if(studentOrTopic == "student"){//search for student
	    if(stoi(target)>50){
	    	cout << "student doesn't exist!" << endl;
	    	return;
	    }
	    //extract student object from vector
	    students student = studentVect[stoi(target)-1];
	    cout << endl;
	   	cout << "searching for student " << target << endl;
	    cout << "Student Name: " << student.get_name() << endl;
	    cout << "Number of Office Hours Attended: " << (student.get_attendedOH()).size() << endl;
	    cout << endl;
	    cout << endl;
	    student.return_attendedOHVector();
	}
	else{//search for topic
	    cout << endl;
	    cout << "Searching for students who asked about the topic: " << target << endl;
	    cout << endl;

	    bool topicFound = false;

	    for(const auto& student : studentVect){
	        const vector<pair<string, string>>& attendedOH = student.get_attendedOH();

	        for(const auto& oh : attendedOH){
	            if(oh.second == target){
	                cout << "Student Name: " << student.get_name() << endl;
	                cout << "Number of Office Hours Attended: " << attendedOH.size() << endl;
	                cout << "Office Hour Attended: " << oh.first << endl;
	                cout << endl;
	                topicFound = true;
	            }
	        }
	    }
	    if(!topicFound){
	        cout << "No students asked about the topic: " << target << endl;
	    }
	}
}

#endif