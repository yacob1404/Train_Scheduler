/*
Author Jacob Hanafin
*/
#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>

//Global Variables
using namespace std;
const int maxtrain = 100;
const int maxstation = 100;
int station_count = 0;
const int infin = 500;
int midPointArr[maxstation];
int midPoint = 0;

//This struct defines the trains the go from town to town
struct TRAINS {
	int Dstation;
	int Astation;
	int Dtime;
	int Atime;
	int travel;
};

//Any train struct/struct array
TRAINS train[maxtrain][maxtrain];
string stations[maxstation];
int P[maxstation][maxstation];
int A[maxstation][maxstation];

//Protos
void LoadTrains(void);
void LoadStations(void);
void spacer(void);
void shortest(void);
void printStations(void);
void path (int, int);
void printRoute(int, int);
//Fix dis shit


//================================================================
//================================================================
int main(void) {
	int userChoice, currentStation, destStation;
	int end = 0;
	LoadStations();
	LoadTrains();
	shortest();
	spacer();

	while (end != 1) {
		//No point in making this into a menu function
		cout << "---------------------------------------------" << endl;
		cout << "--          Author: Jacob Hanafin          --" << endl;
		cout << "---------------------------------------------" << endl;
		cout << "-- 1. List Stations and #'s                --" << endl;
		cout << "-- 2. Find Shortest Travel Time on Trains  --" << endl;
		cout << "-- 3. EXIT                                 --" << endl;
		cout << "---------------------------------------------" << endl;
		cout << "User Choice: ";
		cin >> userChoice;
		if (userChoice == 1) {
			spacer();
			printStations();
		} else if (userChoice == 2) {
			cout << "What station are we at?" << endl;
			cout << "Current Station #: ";
			cin >> currentStation;
			cout << endl;
			cout << "What station do we need to go to?" << endl;
			cout << "Destination #: ";
			cin >> destStation;
			cout << endl;
			midPoint = 0;
			for (int i = 0; i < maxtrain; i++) {
				midPointArr[i] = -1;
			}
			if(currentStation == destStation){
				cout << "YOU ARE ALREADY HERE !!!!" << endl;
			} else {
				path(currentStation,destStation);
				if (A[currentStation][destStation] == infin){
					cout << "There is no path between " << stations[currentStation] << " and " << stations[destStation] << endl;
				} else if (midPointArr[0] == -1) {
					cout << "This is a direct route" << endl;
					cout << "To go from " << stations[currentStation] << " to " << stations[destStation]
					<< " you will need to ride on train for " << A[currentStation][destStation]/60 << " hours " << A[currentStation][destStation]%60<< " minutes." << endl;
					printRoute(midPointArr[midPoint-1], destStation);
				} else {
					midPoint = 1;
					cout << "To go from " << stations[currentStation] << " to " << stations[destStation]
					<< " you will need to ride on train for " << A[currentStation][destStation]/60 << " hours " << A[currentStation][destStation]%60<< " minutes." << endl;
					cout << endl;
					cout << "You will take the following trains: " << endl;
					printRoute(currentStation,midPointArr[0]);
					while (midPointArr[midPoint] != -1) {
						printRoute(midPointArr[midPoint-1], midPointArr[midPoint]);
						midPoint++;
					}
					printRoute(midPointArr[midPoint-1], destStation);
				}
			}
		}  else if (userChoice == 3) {
			// Will close the program
			end = 1;
		} else {
			cout << "Your input was not understood" << endl;
			cout << "Plese remember to use integers for you input" << endl;
		}
		cout << "" << endl;
	}
	return 0;
}
//================================================================
//================================================================
void LoadTrains(void){
	int x,y;
	ifstream file;

	for (int i = 0; i < maxtrain; i++)
		for (int j=0;j<maxtrain;j++)
			train[i][j].travel = infin;

	file.open("trains.dat");
	if(!file)
	{
		cout<<"trains.dat does not exist"<<endl;
		return;
	}
	while(!file.eof())
	{
		file>>x;
		file>>y;

		file>>train[x][y].Dtime;
		file>>train[x][y].Atime;
		train[x][y].Dstation = x;
		train[x][y].Astation = y;
		train[x][y].travel = train[x][y].Atime - train[x][y].Dtime;
	}
}
//================================================================
//================================================================
void LoadStations(void) {
	//string stations[maxstation];
	int count = 0;
	ifstream infile;
	infile.open("stations.dat");
	if(!infile) {
		cout << "stations.dat does not exist" << endl;
		return;
	}
	count=0;
	int num;
	string name;
	while(!infile.eof()) {
		infile>>num;
		infile>>name;
		//cout<<"num: "<<num<<" name: "<<name<<endl;
		stations[count] = name;
		count++;
	}
	infile.close();
	station_count = count;
	cout << "" << endl;
}
//================================================================
//================================================================
void spacer(void) {
	int i = 0;
	while (i < 100) {
		cout << endl;
		i++;
	}
}

//================================================================
//================================================================
void shortest() {
    int i,j,k;

		for (i = 0; i < station_count; i++) {
			for (j = 0; j < station_count; j++) {
				A[i][j] = train[i][j].travel;
				P[i][j] = -1;
			}
		}

		for (i = 0; i < station_count; i++) {
			A[i][i] = 0;
		}

		for( int k = 0; k < station_count; k++ )
		{
			for( int i = 0; i < station_count; i++ )
			{
				for( int j = 0; j <station_count; j++ )
				{
					if( A[i][k] + A[k][j] < A[i][j] )
					{
						A[i][j] = A[i][k] + A[k][j];
						P[i][j] = k;
					}
				}
			}
		}
}
//================================================================
//================================================================
void printStations(void) {
	for (int i = 0; i < station_count; i++)
		cout << i << ": " << stations[i] << endl;
}
//================================================================
//================================================================
void path (int i, int j)  // find midpoints
{
	int k;
	k = P[i][j];

	if( k == -1 ) {
		//cout << "This is a direct path!*" << endl;
		return;
	}
	path(i, k);
	midPointArr[midPoint++] = k;
	path(k, j);
}
//================================================================
//================================================================
void printRoute(int curr, int dest){
	cout << "Leave " << stations[curr] << " at ";
	if (train[curr][dest].Dtime >= 720) {
		if (train[curr][dest].Dtime >= 780) {
			train[curr][dest].Dtime = train[curr][dest].Dtime - 720;
			cout << setw(2) << setfill('0') << train[curr][dest].Dtime / 60 << ":" << setw(2) << setfill('0') << train[curr][dest].Dtime % 60;
		} else {
			cout << setw(2) << setfill('0') << train[curr][dest].Dtime / 60 << ":" << setw(2) << setfill('0') << train[curr][dest].Dtime % 60;
		} cout << " PM";
	} else {
		cout << setw(2) << setfill('0') << train[curr][dest].Dtime / 60 << ":" << setw(2) << setfill('0') << train[curr][dest].Dtime % 60 << " AM";
	}
	cout << " and arrive at " << stations[dest] << " at ";
	if (train[curr][dest].Atime >= 720) {
		if (train[curr][dest].Atime >= 780) {
			train[curr][dest].Atime = train[curr][dest].Atime - 720;
			cout << setw(2) << setfill('0') << train[curr][dest].Atime / 60 << ":" << setw(2) << setfill('0') << train[curr][dest].Atime % 60;
		} else {
			cout << setw(2) << setfill('0') << train[curr][dest].Atime / 60 << ":" << setw(2) << setfill('0') << train[curr][dest].Atime % 60;
		} cout << " PM";
	} else {
		cout << setw(2) << setfill('0') << train[curr][dest].Atime / 60 << ":" << setw(2) << setfill('0') << train[curr][dest].Atime % 60 << " AM";
	} cout << endl;
}
