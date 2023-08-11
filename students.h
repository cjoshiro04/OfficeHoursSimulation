/*STUDENTS HEADER FILE
Student Class definition. Each student object, upon intialization, is assigned a random number betwen 1-6
as the time they will take up in office hours. This is an assumption and may be adjusted accordingly. Each student
also has an arrival time that may be set or retrieved by sim function in HW2.cpp. Note that rand() is 
seeded in HW2.cpp*/
#ifndef STUDENTS_H
#define STUDENTS_H

#include<cstdlib> //provides srand() and rand()
#include<cassert> //provides assert()
#include<string>
#include<vector>
using namespace std;

class students{

public: 
	//constructor
	students(){
		timeNeeded = rand() % 6 + 1; //rand generates 1 - 6, meaning student can take between 1 - 6 min with prof in oh.
		priority_ = rand() % 10 + 1; // generates 1- 10
		//name_ = to_string(rand() % 50 + 1); got rid for hw8 due to different implementation
		//rand returns int
	}

	//accessors
	int get_timeNeeded() const {return timeNeeded;}
	int get_arrivalTime() const {return arrivalTime;}
	int get_priority() const {return priority_;}
	string get_topic() const {return topic_;}
	string get_name() const {return name_;}
	vector<pair<string, string>> get_attendedOH() const {return attendedOH;}

	void return_attendedOHVector() const {
		for(size_t i = 0; i < attendedOH.size(); i++){
			cout << "OH Attended: " << attendedOH[i].first << endl;
			cout << "Topic Asked in this OH: " << attendedOH[i].second << endl;
			cout << endl;
			cout << endl;
		}
		cout << endl;
	}
	
	//mutators
	void set_name(string name) {name_=name;}
	void set_timeNeeded(int random){timeNeeded=random;}//will have rng outside of class and just pass in val into this.
	void set_arrivalTime(int time){arrivalTime=time;}
	void set_topic(string text){topic_=text;}
	void set_attendedOH(string ohAttended, string topic){attendedOH.push_back(make_pair(ohAttended, topic));}

private:
	int timeNeeded;//amount of time student takes up in office hours
	int priority_;
	int arrivalTime; //time student is pushed into working queue;
	string topic_;
	string name_;
	vector<pair<string, string>> attendedOH;
};

#endif