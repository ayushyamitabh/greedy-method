#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;

// VOID CREATE CHANNEL --------------------------------------------------------------------------------------------------------------------//
// INPUT: vector<vector<int>> &channel, int start, int end, int value, bool byProfit
//		  ^ channel to create		 || ^ start  || ^ end || ^ profit || ^ sort by profit value? 
void createChannel(vector<vector<int>> &channel, int start, int end, int value, bool byProfit = false) {
	//cout << "Read:\t" << start << "\t" << end << "\t" << value << endl;
	//cout << "Channel has " << channel.size() << " elements" << endl;
	if (channel.size() == 0) {																												// If channel has no previous elements
		//cout << "Size is 0, will insert new ad..." << endl;
		vector<int> _temp;  																												// Vector<int> to be inserted
		_temp.push_back(start);																												// starting time
		_temp.push_back(end);																												// ending time
		_temp.push_back(value);																												// value of ad
		channel.push_back(_temp);																											// Insert this vector<int>
		//cout << "Pushed new ad...\t" << "start: " << start << "\t end: " << end << "\t value: " << value << endl;
	}
	else {																																	// Else : Size of channel IS NOT 0
		//cout << "Size is not 0..." << endl;																									// AND END TIME START TIME IS AFTER EXISTING ENDING TIME
		for (int i = 0; i < channel.size(); i++) {																							// Will compare to every element in channel
			//cout << "Comparing to: \t start: " << channel[i][0] << "\t end: " << channel[i][1] << endl;										// to find best position to insert ad

			if (start >= channel[i][1]) {																									// CONDITION 1: IF END TIME IS BETWEEN RUNTIME OF ANOTHER AD
				//cout << start << " was greater than " << channel[i][1] << endl;
				bool none = true;																											// Keeps track of whether element is b/w or around existing ad
				for (int j = 0; j < channel.size(); j++) {																					// For every element already in the channel, checks the
																																			// following conditions

					//cout << "\t Is " << end << " between " << channel[j][0] << "\t & \t" << channel[j][1] << "? || Answer=";				// Is the ending time between the runtime of another ad
					if ((end > channel[j][0]) && (end < channel[j][1])) {  
						if (byProfit) {																										// If should sort by profit - does current ad have better value
							float currProfit = 0, compProfit = 0;																			// float currProfit and compProfit

							if ((end - start) == 0) currProfit = value;
							else currProfit = value / (end - start);																		// currProfit is profit value of current read ad

							if ((channel[j][1] - channel[j][0]) == 0) compProfit = channel[j][2];
							else compProfit = channel[j][2] / (channel[j][1] - channel[j][0]);												// compProfit is profit value of comparing ad

							if (currProfit > compProfit) {																					// If currProfit is greater than compProfit,
								vector<int> _temp;																							// swap elements
								_temp.push_back(start);  
								_temp.push_back(end);  
								_temp.push_back(value);  
								channel[j].swap(_temp);
								//cout << "Yes... REPLACED WITH GREATER PROFIT" << endl;

								none = false;																								// none = false, prevents re-inserting same ad again
								break;
							}
							else {																											// If profit value is not greater, still break & none = false
								//cout << "Yes... BREAKING" << endl;																			// because current is in between runtime of another ad and 
								none = false;																								// does not provide greater value
								break;
							}
						}
						else {																												// If not comparing by profit, just break & none = false
							//cout << "Yes... BREAKING" << endl;																				// prevents inserting this ad to the channel
							none = false;  
							break;
						}
					}
					else
					{
						//cout << "No..." << endl;
					}

					//cout << "\t Is " << start << " between " << channel[j][0] << "\t & \t" << channel[j][1] << "? || Answer=";
					if ((channel[j][0] < start) && (channel[j][1] > start)) {																// CONDITION 2: IF STARTING TIME IS BETWEEN RUNTIME OF AD
						if (byProfit) {																										// Following operations are the same as above,
							float currProfit = 0, compProfit = 0;																			// Checks for greater profit opportunity if {bool byProfit = true}
																																			// {bool byProfit = true}. Inserts this ad if greater profit value
							if ((end - start) == 0) currProfit = value;																		// or if it doesn't not conflict with any other ad already in 
							else currProfit = value / (end - start);																		// channel queue

							if ((channel[j][1] - channel[j][0]) == 0) compProfit = channel[j][2];
							else compProfit = channel[j][2] / (channel[j][1] - channel[j][0]);

							if (currProfit > compProfit) {
								vector<int> _temp;  
								_temp.push_back(start);  
								_temp.push_back(end);  
								_temp.push_back(value);
								channel[j].swap(_temp);
								//cout << "Yes... REPLACED WITH GREATER PROFIT" << endl;

								none = false;  
								break;
							}
						}
						else {
							//cout << "Yes... BREAKING" << endl;
							none = false;  
							break;
						}
					}
					else
					{
						//cout << "No..." << endl;
					}

					//cout << "\t Is " << start << " less than " << channel[j][0] << "\t & \t"												// CONDITION 3 : IF CURRENT AD SURROUNDS EXISTING AD
						//<< end << "greater than " << channel[j][1] << "? || Answer=";														// i.e.: starting time is before already existing start
					if (start <= channel[j][0] && end >= channel[j][0]) {																		// and ending time is after already existing end time
						if (byProfit) {																										// Follow same procedure for as the last two conditions
							float currProfit = 0, compProfit = 0;																			// compare profit values if needed and insert ad accordingly

							if ((end - start) == 0) currProfit = value;
							else currProfit = value / (end - start);

							if ((channel[j][1] - channel[j][0]) == 0) compProfit = channel[j][2];
							else compProfit = channel[j][2] / (channel[j][1] - channel[j][0]);

							if (currProfit > compProfit) {
								vector<int> _temp;  
								_temp.push_back(start);  
								_temp.push_back(end);  
								_temp.push_back(value);  
								channel[j].swap(_temp);
								//cout << "Yes... REPLACED WITH GREATER PROFIT" << endl;

								none = false;  
								break;
							}
						}
						else {
							//cout << "Yes... BREAKING" << endl;
							none = false;  
							break;
						}
					}
					else
					{
						//cout << "No..." << endl;
					}

				}
				if (none) {  
					//cout << "Will insert new ad..." << endl;
					vector<int> _temp;  
					_temp.push_back(start);  
					_temp.push_back(end);  
					_temp.push_back(value);  
					channel.push_back(_temp);  
					//cout << "Pushed new ad...\t" << "start: " << start << "\t end: " << end << "\t value: " << value << endl;
				}
				break;
			}


			else if (end <= channel[i][0]) {																								// ELSE IF ENDING TIME IS BEFORE EXISTING START TIME
				//cout << end << " was less than " << channel[i][1] << endl;																	// FOLLOWS SAME STEPS AS ABOVED
				bool none = true;																											// will check for same three condition as above
				for (int j = 0; j < channel.size(); j++) {  

					//cout << "\t Is " << end << " between " << channel[j][0] << "\t & \t" << channel[j][1] << "? || Answer=";
					if ((end > channel[j][0]) && (end < channel[j][1])) {  
						if (byProfit) {
							float currProfit = 0, compProfit = 0;

							if ((end - start) == 0) currProfit = value;
							else currProfit = value / (end - start);

							if ((channel[j][1] - channel[j][0]) == 0) compProfit = channel[j][2];
							else compProfit = channel[j][2] / (channel[j][1] - channel[j][0]);

							if (currProfit > compProfit) {
								vector<int> _temp;  
								_temp.push_back(start);  
								_temp.push_back(end);  
								_temp.push_back(value);  
								channel[j].swap(_temp);
								//cout << "Yes... REPLACED WITH GREATER PROFIT" << endl;

								none = false;  
								break;
							}
							else {
								//cout << "Yes... BREAKING" << endl;
								none = false;  
								break;
							}
						}
						else {
							//cout << "Yes... BREAKING" << endl;
							none = false;  
							break;
						}
					}
					else
					{
						//cout << "No..." << endl;
					}

					//cout << "\t Is " << start << " between " << channel[j][0] << "\t & \t" << channel[j][1] << "? || Answer=";
					if ((channel[j][0] < start) && (channel[j][1] > start)) {  
						if (byProfit) {
							float currProfit = 0, compProfit = 0;

							if ((end - start) == 0) currProfit = value;
							else currProfit = value / (end - start);

							if ((channel[j][1] - channel[j][0]) == 0) compProfit = channel[j][2];
							else compProfit = channel[j][2] / (channel[j][1] - channel[j][0]);

							if (currProfit > compProfit) {
								vector<int> _temp;  
								_temp.push_back(start);  
								_temp.push_back(end);  
								_temp.push_back(value);
								channel[j].swap(_temp);
								//cout << "Yes... REPLACED WITH GREATER PROFIT" << endl;

								none = false;  
								break;
							}
						}
						else {
							//cout << "Yes... BREAKING" << endl;
							none = false;  
							break;
						}
					}
					else
					{
						//cout << "No..." << endl;
					}

					//cout << "\t Is " << start << " less than " << channel[j][0] << "\t & \t" << end << "greater than " << channel[j][1] << "? || Answer=";
					if (start <= channel[j][0] && end >= channel[j][0]) {
						if (byProfit) {
							float currProfit = 0, compProfit = 0;

							if ((end - start) == 0) currProfit = value;
							else currProfit = value / (end - start);

							if ((channel[j][1] - channel[j][0]) == 0) compProfit = channel[j][2];
							else compProfit = channel[j][2] / (channel[j][1] - channel[j][0]);

							if (currProfit > compProfit) {
								vector<int> _temp;  
								_temp.push_back(start);  
								_temp.push_back(end);  
								_temp.push_back(value);  
								channel[j].swap(_temp);
								//cout << "Yes... REPLACED WITH GREATER PROFIT" << endl;

								none = false;  
								break;
							}
						}
						else {
							//cout << "Yes... BREAKING" << endl;
							none = false;  
							break;
						}
					}
					else
					{
						//cout << "No..." << endl;
					}
				}
				if (none) {  
					//cout << "Will insert new ad..." << endl;
					vector<int> _temp;  
					_temp.push_back(start);  
					_temp.push_back(end);  
					_temp.push_back(value);  
					channel.push_back(_temp);  
					//cout << "Pushed new ad...\t" << "start: " << start << "\t end: " << end << "\t value: " << value << endl;
				}
				break;
			}
		}
	}
}

int main() {

	vector<vector<int>> _ch1, _ch2, _ch3, _ch4;
	ofstream output;
	output.open("whileread-wo-profit-output.txt");
	ifstream input;
	input.open("input.txt");
	if (input.fail()) {
		output << "Fail to open the file" << endl;
		exit(1);
	}

	while (!input.eof()) {
		int start, end, value,
			_end1 = 0, _end2 = 0,
			_end3 = 0, _end4 = 0;
		bool _channel[4];
		input >> start >> end >> value
			>> _channel[0] >> _channel[1]
			>> _channel[2] >> _channel[3];
		if (_channel[0]) {  
			createChannel(_ch1, start, end, value);
		}
		if (_channel[1]) {
			createChannel(_ch2, start, end, value);
		}
		if (_channel[2]) {
			createChannel(_ch3, start, end, value);
		}
		if (_channel[3]) {
			createChannel(_ch4, start, end, value);
		}
	}
	output << "Channel 1:" << _ch1.size() << '\t'
		<< "Channel 2:" << _ch2.size() << '\t'
		<< "Channel 3:" << _ch3.size() << '\t'
		<< "Channel 4:" << _ch4.size() << '\n';
	output << "============CHANNEL 1===============" << endl;
	for (int i = 0; i < _ch1.size(); i++) {
		for (int j = 0; j < _ch1[i].size(); j++) {
			output << _ch1[i][j] << '\t';
		}
		output << '\n';
	}
	output << "============CHANNEL 2===============" << endl;
	for (int i = 0; i < _ch2.size(); i++) {
		for (int j = 0; j < _ch2[i].size(); j++) {
			output << _ch2[i][j] << '\t';
		}
		output << '\n';
	}
	output << "============CHANNEL 3===============" << endl;
	for (int i = 0; i < _ch3.size(); i++) {
		for (int j = 0; j < _ch3[i].size(); j++) {
			output << _ch3[i][j] << '\t';
		}
		output << '\n';
	}
	output << "============CHANNEL 4===============" << endl;
	for (int i = 0; i < _ch4.size(); i++) {
		for (int j = 0; j < _ch4[i].size(); j++) {
			output << _ch4[i][j] << '\t';
		}
		output << '\n';
	}
	return 0;
}