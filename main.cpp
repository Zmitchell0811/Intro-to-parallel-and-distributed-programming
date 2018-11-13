#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <iomanip>
#include <iterator>
#include <cmath>
#include <math.h>
//#include <mpi.h>

#define INPUT_FILE  "flights.txt"
#define PI			3.14159265359
#define EARTH_RADIUS 6371
struct Airport {
	int id, altitude;
	std::string name, city, country, iata, icao, database_time, type, dst;
	long double  latitude, longtitude;
	float timezone;

};

Airport data_niceify(std::string);
Airport find_in_dataset(std::string);

std::vector<std::vector<float>> create_cost_matrix(std::vector<Airport>, int);
double convert_deg_dist(double, double, double, double);
int main() {
	std::vector<Airport> airport_parsed;
	//ENTER AIRPORT CODES
	std::string input;
	std::vector<Airport> user_airport_array;
	int node_tracker = 0; 
	bool do_exit = false;
	do {
		std::cout << "Enter IATA or ICAO code or enter q when done: ";
		std::cin >> input;
		if (input != "q") {
			//append to vector array
			auto air_end = user_airport_array.end();
			user_airport_array.insert(air_end, find_in_dataset(input));
			node_tracker++;
		}
		else {
			do_exit = true;
		}
		
	} while (do_exit == false);
	
	std::vector<std::vector<float>> cost_matrix(create_cost_matrix(user_airport_array, node_tracker));
	system("pause");
	return 0;
	//SEND CODES TO BE FOUND IN FILE
	
	//PARSE THE CORDINATES INTO FLOATS
	//STORE IN ARRAY


	
	//PASS ARRAY TO THE COST LIST FUNCTION
	//STORE COST LIST

	//SEND COST LIST TO FIND MINIMUM PATH
	//SAVE MINIMUM PATH

	//OUTPUT MINIMUM PATH
}

//void traveling_salesman(std::vector<std::vector<float>> martix) {

}
//FIND LINE IN DATA SET AND PARSE DATA
Airport find_in_dataset(std::string airport) {
	std::ifstream ifs(INPUT_FILE);
	std::string line;
	
	if (ifs.is_open()) {
		while (getline(ifs, line)) {
			Airport air;
			air = data_niceify(line);
			//COMPARE IATA OR ICAO
			if (air.iata == airport || air.icao == airport) {
				std::cout << air.name << std::endl;
				return air;
			}
			else {
				//std::cout << "No airport location found" << std::endl;
			}
		}
	}
	else {
		std::cout << "file could not be opened" << std::endl;
	}
	ifs.close();
	std::cout << 1;
}

Airport data_niceify( std::string line) {
	Airport air;
	std::string substr = line;
	std::istringstream iss(substr);
	std::string word, result;
	std::getline(iss, word, ',');
	air.id = stoi(word);

	std::getline(iss, air.name, ',');
	std::remove_copy(air.name.begin(), air.name.end(), std::back_inserter(result), '"');
	air.name = result;
	result = "";

	std::getline(iss, air.city, ',');
	std::remove_copy(air.city.begin(), air.city.end(), std::back_inserter(result), '"');
	air.city = result;
	result = "";

	std::getline(iss, air.country, ',');
	std::remove_copy(air.country.begin(), air.country.end(), std::back_inserter(result), '"');
	air.country = result;
	result = "";

	std::getline(iss, air.iata, ',');
	std::remove_copy(air.iata.begin(), air.iata.end(), std::back_inserter(result), '"');
	air.iata = result;
	result = "";

	std::getline(iss, air.icao, ',');
	std::remove_copy(air.icao.begin(), air.icao.end(), std::back_inserter(result), '"');
	air.icao = result;
	result = "";
	word = "";
	std::getline(iss, word, ',');
	air.latitude = atof(word.c_str()) ;

	word = "";
	std::getline(iss, word, ',');
	air.longtitude = atof(word.c_str());

	word = "";
	std::getline(iss, word, ',');
	air.altitude = stoi(word);

	word = "";
	std::getline(iss, word, ',');
	air.timezone = stoi(word);

	std::getline(iss, air.dst, ',');
	std::remove_copy(air.dst.begin(), air.dst.end(), std::back_inserter(result), '"');
	air.dst = result;
	result = ""; 

	std::getline(iss, air.database_time, ',');
	std::remove_copy(air.database_time.begin(), air.database_time.end(), std::back_inserter(result), '"');
	air.database_time = result;
	result = "";

	std::getline(iss, air.type, ',');
	std::remove_copy(air.type.begin(), air.type.end(), std::back_inserter(result), '"');
	air.type = result;
	result = "";

	return air;
}

//CREATE COST LIST FUNCTION
//RETURN ARRAY TAKE AIRPORT LOCATIONS AS ARUGMENT
//
//ARRAY TO STORE COST BETWEEN TWO AIRPORTS
//
//DO THIS FOR EACH NODE
//	SEARCH FILE FOR AIRPORTS IN CORDINATES
//	SEND DATA TO CONVERT DEGREES TO DISTANCE
//	STORE DATA IN ARRAY
//
//RETURN ARRAY TO FIND SHORTEST PATH
std::vector<std::vector<float>> create_cost_matrix(std::vector<Airport> v, int nodes){
	std::vector<std::vector< float>> cost_matrix;
	for (int i = 0; i < nodes; i++) {
		std::vector<float> tmp_distance;
		for (int y = 0; y < nodes; y++) {
			double distance = 0;
			int tempi = i;
			int tempy = y;
			if (tempi == tempy || tempy == tempi) {
				distance = 0;
			}
			else{
				distance = convert_deg_dist(v[i].latitude, v[i].longtitude, v[y].latitude, v[y].longtitude);
				
			}
			std::cout << distance << " " ;
			auto it = tmp_distance.insert(tmp_distance.end(), distance);
		}
		auto it = cost_matrix.insert(cost_matrix.end(), tmp_distance);
		std::cout << std::endl;
	}
	return cost_matrix;
}

//CONVERT DEGREES TO DISTANCE
//TAKE TWO ARGUMENTS ONE AIRPORT TO ANOTHER
//CORDINATE DISTANCE TO KM OR MILE DISTANCE
//RETURN VALUE
double convert_deg_dist(double lat1, double long1, double lat2, double long2) {
	double distancekm;
	double lat_1_rads, lat_2_rads, long_1_rads, long_2_rads, theta, lamda;
	
	lat_1_rads = (lat1 * PI) / 180;
	lat_2_rads = (lat2 * PI) / 180;
	long_1_rads = (long1 * PI) / 180;
	long_2_rads = (long2 * PI) / 180;
	theta = sin((lat_2_rads - lat_1_rads) / 2);
	lamda = sin((long_2_rads - long_2_rads) / 2);

	distancekm = 2.0 * EARTH_RADIUS * asin(sqrt(theta * theta + cos(lat_1_rads) * cos(lat_2_rads) * lamda * lamda));
	return distancekm;
}