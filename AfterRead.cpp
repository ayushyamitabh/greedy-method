#include <iostream>
#include <fstream> // For ifstream
#include <cstdlib> // For exit(1)
#include <vector>
#include <cstdbool>

using namespace std;

static vector<int> _starts; // <<-----------------------------------------|| Contains starting times
static vector<int> _ends; // <<-------------------------------------------|| Contains ending times
static vector<int> _values; // <<-----------------------------------------|| Contains raw profit values
static vector<float> _profitValues; // <<---------------------------------|| Used to sort channel listings by profit value, [profit value / (end time - start time)]
static vector<vector<bool>> _channels; // <<------------------------------|| Contains what channel each ad. can air on
static vector<int> _Ch1, _Ch2, _Ch3, _Ch4; // <<=============================||================>> Positions for each channel's advertisements
static vector<int> _times1, _times2, _times3, _times4;  //<<~~~~~~~~~~~~~~||~~~~~~~~~~~~~~~~>> Contains final listings for each channel
																	   // ||
void read_file(ifstream& inFile);                                      // ||---->> Creates vectors using data from input.txt
void sortByStart(int left = 0, int right = _starts.size() - 1); // <<~~~~~~~||~~~~~~~~~~~~~~~~>> Sorts data based on starting times -- USING QUICK SORT
void swapContent(int _currentPos, int _newPos); // <<~~~~~~~~~~~~~~~~~~~~~||~~~~~~~~~~~~~~~~>> Used by sortByStart(); Swaps elements of each vector, helps maintain order
void createChannels(); // <<==============================================||================>> Indexing : Creates _Ch1, _Ch2, _Ch3, _Ch4;
void createListings(vector<int> _chNum, vector<int> &_list); // <<~~~~~~~~||~~~~~~~~~~~~~~~~>> Creates final listing for each channel [Contatins indexes]
void printChannelAvailibity(); // <<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||~~~~~~~~~~~~~~~~>> Prints values using _Ch1,_Ch2,_Ch3,_Ch4
void printFinalListings(); // <<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||~~~~~~~~~~~~~~~~>> Prints values using _times1, _times2, _times3, _times4
void createProfitRatings(); // <<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||~~~~~~~~~~~~~~~~>> Creates values to be stored in _profitValues
void printProfitRatings(); // <<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||~~~~~~~~~~~~~~~~>> Prints all data including the values from _profitValues
void createProfitListings(vector<int> _chNum, vector<int> &_list); // <<~~||~~~~~~~~~~~~~~~~>> Creates final listing for each channel based on profit values [Contatins indexes]

ofstream output;

int main() {

	output.open("allread-output.txt");

	//"""""""""""""""GETTING DATA""""""""""""""""//
	ifstream inFile;                             //----------------------->> Input file variable, inFile
	inFile.open("input.txt");                    //----------------------->> Open file with name, 'input.txt'
	if (inFile.fail()) {                         //----------------------->> Checking to make sure file was opened
		//cout << "Fail to open the file" << endl; //----------------------->> If failed to open file
		exit(1);                                 //----------------------->> Prevents program from crashing...regular clean up
	}                                            //                       ||
	read_file(inFile);                           //----------------------->> Getting data from the file and creating vectors
	inFile.close();                              //----------------------->> Close the file
	//"""""""""""""DONE GETTING DATA"""""""""""""//

	//"""""""""""""""SORT AND SHOW DATA""""""""""""""""//
	sortByStart();                                     //----------------->> Sorting all data based on the starting times
	//cout << "==========All Data=============" << endl; //                 ||
	for (int i = 0; i < _starts.size(); i++) {         //-----------------||-->> Print data to console
													   //                 ||
		//cout << _starts.at(i) << "\t"                  //                 ||> Printing starting time...
			 //<< _ends.at(i) << "\t"                    //                 ||> Printing ending time...
			 //<< _values.at(i) << "\t";                 //                 ||> Printing profit value...
		for (int j = 0; j < 4; j++) {}                    //                 ||
			//cout << _channels[i][j] << ' ';            //                 ||> Printing available channels...
		//cout << endl;                                  //                 ||
	}                                                  //                 ||
	//"""""""""""""""""""""DONE""""""""""""""""""""""""//

	//""""""CREATE AND SHOW CHANNELS""""""//
	createChannels();                     //
	printChannelAvailibity();             //
	//""""""""""""""""DONE""""""""""""""""//

	createProfitRatings();
	printProfitRatings();

	//""""""CREATE AND SHOW LISTINGS""""""//
	createListings(_Ch1, _times1);        //
	createListings(_Ch2, _times2);        //
	createListings(_Ch3, _times3);        //
	createListings(_Ch4, _times4);        //
	cout << "Channel 1:" << _times1.size() << '\t' << "Channel 2:" << _times2.size() << '\t' << "Channel 3:" << _times3.size() << '\t' << "Channel 4:" << _times4.size() << endl;
	printFinalListings();                 //
	//""""""""""""""""DONE""""""""""""""""//

	//""""""CREATE AND SHOW LISTINGS""""""//
	createProfitListings(_Ch1, _times1);  //
	createProfitListings(_Ch2, _times2);  //
	createProfitListings(_Ch3, _times3);  //
	createProfitListings(_Ch4, _times4);  //
	cout << "Channel 1:" << _times1.size() << '\t' << "Channel 2:" << _times2.size() << '\t' << "Channel 3:" << _times3.size() << '\t' << "Channel 4:" << _times4.size() << endl;
	printFinalListings();                 //
	//""""""""""""""""DONE""""""""""""""""//
	return 0;
}

//""""""""READ FILE FUNCTION""""""""//
// INPUT : ifstream & (reference)   //
void read_file(ifstream& inFile) {  //
	int start,                      //------------------------------------------>> Will store starting time
		end,                        //------------------------------------------>> Will store ending time
		value;                      //------------------------------------------>> Will store profit value
	bool _channel[4];               //------------------------------------------>> Will store True/False for each channel
	vector<bool> _channelVector(4); //------------------------------------------>> Vector containing values of _channel[4] to be passed to **STATIC VECTOR<VECTOR<BOOL>> _CHANNELS**
	while (!inFile.eof()) {         //------------------------------------------||--->> While the end of file is not reached
		inFile >> start                                                      // ||> Get start time
			>> end                                                        // ||> Get end time
			>> value                                                      // ||> Get profit value
			>> _channel[0] >> _channel[1] >> _channel[2] >> _channel[3];  // ||> Get channel availibility
		_starts.push_back(start);   //------------------------------------------>> Starting time pushed to **STATIC VECTOR<INT> _STARTS**
		_ends.push_back(end);       //------------------------------------------>> Ending time pushed to **STATIC VECTOR<INT> _ENDS**
		_values.push_back(value);	//------------------------------------------>> Profit values pushed to **STATIC VECTOR<INT> _VALUES**
		for (int i = 0; i < 4; i++) _channelVector[i] = _channel[i]; //--------->> Creating _channelVector for (see next line)...
		_channels.push_back(_channelVector); //--------------------------------->> Channel availibilty pushed to **STATIC VECTOR<VECTOR<BOOL>> _CHANNELS**
	}																		 // ||
}																			 //	||
//""""""""""""""""""""""""""""""""""//

//""""""""""""SORT DATA FUNCTION""""""""""""""//
//'''''''''Uses Quick Sort algorithm''''''''''//
// INPUT : int left  &  int right             //
void sortByStart(int left, int right) {       //
	int i = left, j = right;                  // Left and Right ends of the array to be sorted
	int pivot = _starts[(left + right) / 2];  // Pivot used by Quick Sort, here it is the middle element of the array
	while (i <= j) {                          // As long as the left and right cursors don't cross
		while (_starts[i] < pivot) {          // Increment left cursor while value at left cursor is less than the pivot
			i++;
		}                                     
		while (_starts[j] > pivot) {          // Increment right cursor while the value at right cursor is more than the pivot
			j--;                              
		}                                     
		if (i <= j) {                         // If left cursor is less than right cursor
			swapContent(i, j);                // Swap values at these cursors...
			i++;                              // Increase 'i' by 1
			j--;                              // Decrease 'j' by 1
		}                                     
	}                                          
	if (left < j) sortByStart(left, j);       // Recursive call to partition array [left half] 
	if (i < right) sortByStart(i, right);     // Recursive call to partition array [right half]
}											
//""""""""""""""""""""""""""""""""""""""""""""//

//""""""""""""""""SWAP CONTENT FUNCTION""""""""""""""""""//
//'''''''''''''''Collectively swaps content''''''''''''''//
// INPUT : int _currentPos  &  int _newPos               //
void swapContent(int _currentPos, int _newPos) {         // Swaps content of _starts, _ends, and _values
	int _tmpStart, _tmpEnd, _tmpValue;					 // Stores temporary value of the element in the new position
	bool _tmpChannels[4];								 // Temporarily stores the array of channel listings
	_tmpStart = _starts[_newPos];							
	_tmpEnd = _ends[_newPos];									
	_tmpValue = _values[_newPos];								
	for (int i = 0; i < 4; i++) {									
		_tmpChannels[i] = _channels[_newPos][i];				
	}											
	_starts[_newPos] = _starts[_currentPos];			 // Swaps starting time value
	_ends[_newPos] = _ends[_currentPos];				 // Swaps ending time value
	_values[_newPos] = _values[_currentPos];			 // Swaps profit value
	for (int i = 0; i < 4; i++) {						 // Swaps channel listings
		_channels[_newPos][i] = _channels[_currentPos][i];		
	}													
	_starts[_currentPos] = _tmpStart;					
	_ends[_currentPos] = _tmpEnd;						
	_values[_currentPos] = _tmpValue;					
	for (int i = 0; i < 4; i++) {						
		_channels[_currentPos][i] = _tmpChannels[i];	
	}													
}														
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""//

//""""""""""""CREATE CHANNEL FUNCTION"""""""""""""""//
//'''''''''Creates _ch1, _ch2, _ch3, _ch4'''''''''''//
// INPUT : [none]                                   //
void createChannels() {								//
	for (int j = 0; j < 4; j++) {					//
		for (int i = 0; i < _channels.size(); i++) {//
			if (_channels[i][j] == 1) {				//
				if (j == 0) {						//
					_Ch1.push_back(i);				//
				}									//
				else if (j == 1) {					//
					_Ch2.push_back(i);				//
				}									//
				else if (j == 2) {					//
					_Ch3.push_back(i);				//
				}									//
				else if (j == 3) {					//
					_Ch4.push_back(i);				//
				}									//
			}										//
		}											//
	}												//
}													//
//""""""""""""""""""""""""""""""""""""""""""""""""""//

//""""""""""""""""""CREATE LISTINGS FUNCTION""""""""""""""""""""//
//''''''''''' Creates final listings based on timings'''''''''''//
// INPUT : vector<int> _chNum  & vector<int> &_list             //
void createListings(vector<int> _chNum, vector<int> &_list) {	//
	int _lastEnd = 0;											// Stores the value of the last ending time
	for (int i = 0; i < _chNum.size(); i++) {					// Will iterate through each listing for the channel
		if (_starts[_chNum[i]] >= _lastEnd) {					// If starting time is after for equal to the last ending time
			_list.push_back(_chNum[i]);							// Will insert index of the listing into the final list
			_lastEnd = _ends[_chNum[i]];						// Updates the value of the last ending time for next comparison
		}
		else {
			//cout << '\n';										// Fixes console output display
		}
	}
}
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//

//""""""""""""""""""""""""""""""""""""""""" PRINTING CHANNEL DATA FUNCTIONS """"""""""""""""""""""""""""""""""""//
void printChannelAvailibity() {
	/*//
	//cout << "Ch1: " << _Ch1.size() << '\n'																		//
		<< "Ch2: " << _Ch2.size() << '\n'																		//
		<< "Ch3: " << _Ch3.size() << '\n'																		//
		<< "Ch4: " << _Ch4.size() << '\n';																		//
																												//
	//cout << "==========CHANNEL 1============" << endl;															//
	for (int i = 0; i < _Ch1.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch1[i]) << "\t" << _ends.at(_Ch1[i]) << "\t" << _values.at(_Ch1[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch1[i]][j] << ' ';																//
		//cout << endl;																							//
	}																											//
																												//
	//cout << "==========CHANNEL 2============" << endl;															//
	for (int i = 0; i < _Ch2.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch2[i]) << "\t" << _ends.at(_Ch2[i]) << "\t" << _values.at(_Ch2[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch2[i]][j] << ' ';																//
		//cout << endl;																							//
	}																											//
																												//
	//cout << "==========CHANNEL 3============" << endl;															//
	for (int i = 0; i < _Ch3.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch3[i]) << "\t" << _ends.at(_Ch3[i]) << "\t" << _values.at(_Ch3[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch3[i]][j] << ' ';																//
		//cout << endl;																							//
	}																											//
																												//
	//cout << "==========CHANNEL 4============" << endl;															//
	for (int i = 0; i < _Ch4.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch4[i]) << "\t" << _ends.at(_Ch4[i]) << "\t" << _values.at(_Ch4[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch4[i]][j] << ' ';																//
		//cout << endl;																							//
	}				*/																							//
}																												//
																												//
void printProfitRatings() {
	/*//
	//cout << "==========CHANNEL 1============" << endl;															//
	for (int i = 0; i < _Ch1.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch1[i]) << "\t" << _ends.at(_Ch1[i]) << "\t" << _values.at(_Ch1[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch1[i]][j] << ' ';																//
		//cout << '\t' << _profitValues[_Ch1[i]] << endl;															//
	}																											//
																												//
	//cout << "==========CHANNEL 2============" << endl;															//
	for (int i = 0; i < _Ch2.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch2[i]) << "\t" << _ends.at(_Ch2[i]) << "\t" << _values.at(_Ch2[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch2[i]][j] << ' ';																//
		//cout << '\t' << _profitValues[_Ch2[i]] << endl;															//
	}																											//
																												//
	//cout << "==========CHANNEL 3============" << endl;															//
	for (int i = 0; i < _Ch3.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch3[i]) << "\t" << _ends.at(_Ch3[i]) << "\t" << _values.at(_Ch3[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch3[i]][j] << ' ';																//
		//cout << '\t' << _profitValues[_Ch3[i]] << endl;															//
	}																											//
																												//
	//cout << "==========CHANNEL 4============" << endl;															//
	for (int i = 0; i < _Ch4.size() - 1; i++) {																	//
		//cout << _starts.at(_Ch4[i]) << "\t" << _ends.at(_Ch4[i]) << "\t" << _values.at(_Ch4[i]) << "\t";		//
		for (int j = 0; j < 4; j++)																				//
			//cout << _channels[_Ch4[i]][j] << ' ';																//
		//cout << '\t' << _profitValues[_Ch4[i]] << endl;															//
	}																											//
	*/
}																												//
void printFinalListings() {																						//
	output << "==========CHANNEL 1============" << endl;														//
	for (int i = 0; i < _times1.size(); i++) {																	//
		output << _starts[_times1[i]] << '\t' << _ends[_times1[i]] << '\t' << _values[_times1[i]] << endl;		//
	}																											//
																												//
	output << "==========CHANNEL 2============" << endl;														//
	for (int i = 0; i < _times2.size(); i++) {																	//
		output << _starts[_times2[i]] << '\t' << _ends[_times2[i]] << '\t' << _values[_times2[i]] << endl;		//
	}																											//
																												//
	output << "==========CHANNEL 3============" << endl;														//
	for (int i = 0; i < _times3.size(); i++) {																	//
		output << _starts[_times3[i]] << '\t' << _ends[_times3[i]] << '\t' << _values[_times3[i]] << endl;		//
	}																											//
																												//
	output << "==========CHANNEL 4============" << endl;														//
	for (int i = 0; i < _times4.size(); i++) {																	//
		output << _starts[_times4[i]] << '\t' << _ends[_times4[i]] << '\t' << _values[_times4[i]] << endl;		//
	}																											//
}																												//
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//


//""""""""""""""""""""""""""""""PROFIT RATINGS FUNCTION""""""""""""""""""""""""""""""""//
//''''''''''''''''''''''Creates profit values (profit / run time)''''''''''''''''''''''//
// INPUT : [none]                                                                      //
void createProfitRatings() {														   //
	for (int i = 0; i < _starts.size() - 1; i++) {									   //
		if (_ends[i] - _starts[i] == 0) _profitValues.push_back(_values[i]);		   //-------------->> If end time - start time = 0, set it to the given values, avoids denominator from being 0
		else _profitValues.push_back((_values[i] * 1.0) / (_ends[i] - _starts[i]));	   //-------------->> To find the profit value, divide given profit value by the run time
	}																				   //
}																					   //
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//

//"""""""""""""""""""""""""""""CREATE LISTINGS FUNCTION"""""""""""""""""""""""""""""""""//
//'''''''''''''''''''''''Creates listings based on profit values''''''''''''''''''''''''//
// INPUT : vector<int> _chNum & vector<int> &_list										//
void createProfitListings(vector<int> _chNum, vector<int> &_list) {						//
    _list.clear();					
    int _lastEnd = 0;																	
    for (int i = 0; i < _chNum.size() - 1; i++) {
        if (_starts[_chNum[i]] < _lastEnd) {
            int checkIndex = 0;
            if ((i - 1) < 0) { checkIndex = 0; }
            else { checkIndex = i - 1; }
            if (_profitValues[_chNum[i]] > _profitValues[_chNum[checkIndex]]) {	
                _list.back() = _chNum[i];
                _lastEnd = _ends[_chNum[i]];				
            }																				 
        }																		
        else if (_starts[_chNum[i]] >= _lastEnd) {
            cout << "start= " << _starts[_chNum[i]] << " | end= " << _ends[_chNum[i]] << " | last end= " << _lastEnd << endl;
            if (_list.size() == 0) {
                cout << "Size is 0, inserting..." << endl;				
                _list.push_back(_chNum[i]);											
                _lastEnd = _ends[_chNum[i]];												
            }
            else {
                bool none = true;
                if (_ends[_chNum[i]] == _lastEnd) {
                    for (int k = _list.size() - 1; k > 0; k--) {
                        if (_starts[_list[k]] == _lastEnd) {
                            if (_profitValues[_chNum[i]] > _profitValues[_list[k]]) {
                                none = false;
                                cout << "Replacing " << _starts[_list[k]] << "with higher profit value" << _starts[_chNum[i]] << endl;
                                _lastEnd = _ends[_chNum[i]];
                                _list[k] = _chNum[i];
                                break;
                            }
                            else {
                                cout << "Found start = " << _starts[_list[k]] << " = last end = " << _lastEnd;
                                none = false;
                                cout << "Found end = " << _ends[_list[k]] << " = last end = " << _lastEnd;
                                cout << "Breaking..." << endl;
                                break;
                            }
                        }
                    }
                }
                if (none) {
                    cout << "Didn't find end..." << endl;
                    cout << "Inserting...& breaking..." << endl;
                    _list.push_back(_chNum[i]);													
                    _lastEnd = _ends[_chNum[i]];												
                }
            }
        }
        else {																			
            //do nothing														
        }																				
    }																					
}																						
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//