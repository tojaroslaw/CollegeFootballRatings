#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <algorithm>
#include <cstdint>
#include <istream>
#include <ostream>
#include <cmath>
#include <streambuf>
#include <limits>
using namespace std;

int numTeamsTotal;
int sumTeamsPlayed;
double avgTeamsPlayed;

const static double PI = 3.14159265358979323846;

void runProgram(stringstream& input, stringstream& inputcopy);
void runH2HTest(string& inputIn, double startPoint);
int getNumTeams(string& inputIn);

void getInput3(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams, stringstream& inputIn);
int getInput3H2HTest(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams, string& inputIn, double startPoint, int iterationIn);
int findTeam(vector<string>& teams, string& team);
bool teamExists(vector<string>& teams, string& team);
string toUpper(string& x);
void calcFinalRatings(vector<vector<double> >& h2hPredict, vector<double>& ratings);
void printRatings(vector<string>& teams, vector<double>& ratings);
void printRatingsInfo(vector<string>& teams, vector<double>& ratings, vector<vector<double>>& games, vector<vector<double>>& h2hPM, vector<vector<int>>& ties);
vector<vector<double> > calcNewRatingGrid5(vector<vector<double>>& games, vector<vector<double> >& h2h, vector<vector<double>>& h2hPM, vector<int>& teamsPlayed);
double predictH2Hsquare5(vector<vector<double>>& games, vector<vector<double>>& prevH2H, vector<vector<double>>& h2hPM, vector<double>& tempRatings, vector<
double>& tempAbsRatings, vector<double>& ntr, vector<int>& teamsPlayed, int a, int b);
void printGrid2(vector<vector<double>>& grid, vector<string>& teams);

double rt2(double x);
double rtX(double x, double denom);
double powX(double x, double exponent);
double pow2(double x);
double logyX(double x, double base);
double logyT(double x);
double expyX(double x, double base);
double expyT(double x);
double sinX(double x);
double asinX(double x);
double circX(double x);
double circX(double x, double power);
double acircX(double x);
double acircX(double x, double power);

double rootGamesPlus2(double x, double games);
int sum(vector<int>& v);
double avg(vector<int>& v);
vector<double> normalize(vector<double>& x);
vector<double> normalizeRT2(vector<double>& x);
vector<double> normalizePow(vector<double>& x, double exponent);

struct Team {
		string name;
		double rating;
		double rawScore = 0;
		int wins;
		int losses;
		int ties;
		int rank = 0;

		Team(string& name, double rating, int wins, int losses) {
			this->name = name;
			this->rating = rating;
			this->wins = wins;
			this->losses = losses;
			this->ties = 0;
		}

		Team(string& name, double rating, int wins, int losses, int ties) {
			this->name = name;
			this->rating = rating;
			this->wins = wins;
			this->losses = losses;
			this->ties = ties;
		}

		Team(string& name, double rating, double rawScore, int wins, int losses) {
			this->name = name;
			this->rating = rating;
			this->wins = wins;
			this->losses = losses;
			this->ties = 0;
			this->rawScore = rawScore;
		}

		Team(string& name, double rating, double rawScore, int wins, int losses, int ties) {
			this->name = name;
			this->rating = rating;
			this->wins = wins;
			this->losses = losses;
			this->ties = ties;
			this->rawScore = rawScore;
		}
};

struct comp {
		bool operator()(Team& a, Team& b) {
			return a.rating >= b.rating;
		}
} comp;

int main() {
	stringstream input;
	stringstream inputcopy;
	string inputString = "";

	string x = "$";
	do {
		getline(cin, x, '\n');
		input << x << "\n";
		inputcopy << x << "\n";
		inputString += x;
		inputString += "\n";
	} while (x.find('=') == string::npos);

	runProgram(input, inputcopy);
	//runH2HTest(inputString, 8);
	return 0;
}

void runProgram(stringstream& input, stringstream& inputcopy) {
	string temp;
	cout << setprecision(4) << fixed;

	vector<string> teams;
	vector<vector<double> > h2hPM;
	vector<vector<double> > h2h;
	vector<vector<double> > h2hPredict;
	vector<vector<double> > games;
	vector<vector<int> > ties;
	vector<double> ratings;
	vector<int> gamesPlayed;
	vector<int> teamsPlayed;

	getInput3(h2hPM, games, ties, gamesPlayed, teamsPlayed, teams, numTeamsTotal, input);

	sumTeamsPlayed = sum(teamsPlayed);
	avgTeamsPlayed = avg(teamsPlayed);

	h2h.resize(numTeamsTotal);
	h2hPredict.resize(numTeamsTotal);
	ratings.resize(numTeamsTotal);
	for (int i = 0; i < numTeamsTotal; ++i) {
		h2h[i].resize(numTeamsTotal);
		h2hPredict[i].resize(numTeamsTotal);
	}

	for (int i = 0; i < numTeamsTotal; ++i) {
		for (int j = 0; j < numTeamsTotal; ++j) {
			if (games[i][j] != 0) {
				h2hPM[i][j] = h2hPM[i][j] / (games[i][j]);
				//h2h[i][j] = rt2(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)));
				if (games[i][j] > 1.0) {
					h2h[i][j] = rtX(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)), (sqrt(games[i][j]) * abs(h2hPM[i][j]) + 1));
				}
				else {
					h2h[i][j] = rt2(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)));
				}
			}
		}
	}


	h2hPredict = calcNewRatingGrid5(games, h2h, h2hPM, teamsPlayed);
	for (int i = 0; i < (int) sqrt(numTeamsTotal) + numTeamsTotal + 100; ++i) {
		h2hPredict = calcNewRatingGrid5(games, h2hPredict, h2hPM, teamsPlayed);
	}

	calcFinalRatings(h2hPredict, ratings);
	//printGrid2(h2hPredict, teams);
	printRatingsInfo(teams, ratings, games, h2hPM, ties);
}

void runH2HTest(string& inputIn, double startPoint) {
	string temp;
	cout << setprecision(4) << fixed;

	vector<string> teams;
	vector<vector<double> > h2hPM;
	vector<vector<double> > h2h;
	vector<vector<double> > h2hPredict;
	vector<vector<double> > games;
	vector<vector<int> > ties;
	vector<double> ratings;
	vector<int> gamesPlayed;
	vector<int> teamsPlayed;

	stringstream input(inputIn);

	int iteration = 0;
	int justNumTeams = getNumTeams(inputIn);
	int gameline = 0;
	int correctPreds = 0;
	int totalPreds = 0;

	gameline = 0;
	while (gameline >= 0) {
		gameline = getInput3H2HTest(h2hPM, games, ties, gamesPlayed, teamsPlayed, teams, justNumTeams, inputIn, startPoint, iteration);
		++iteration;
		cout << iteration << ", " << gameline << endl;

		sumTeamsPlayed = sum(teamsPlayed);
		avgTeamsPlayed = avg(teamsPlayed);

		h2h.resize(numTeamsTotal);
		h2hPredict.resize(numTeamsTotal);
		ratings.resize(numTeamsTotal);
		for (int i = 0; i < numTeamsTotal; ++i) {
			h2h[i].resize(numTeamsTotal);
			h2hPredict[i].resize(numTeamsTotal);
		}

		for (int i = 0; i < numTeamsTotal; ++i) {
			for (int j = 0; j < numTeamsTotal; ++j) {
				if (games[i][j] != 0) {
					h2hPM[i][j] = h2hPM[i][j] / (games[i][j]);
					//h2h[i][j] = rt2(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)));
					if (games[i][j] > 1.0) {
						h2h[i][j] = rtX(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)), (sqrt(games[i][j]) * abs(h2hPM[i][j]) + 1));
					}
					else {
						h2h[i][j] = rt2(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)));
					}
				}
			}
		}

		h2hPredict = calcNewRatingGrid5(games, h2h, h2hPM, teamsPlayed);
		for (int i = 0; i < (int) sqrt(numTeamsTotal) + numTeamsTotal + 100; ++i) {
			h2hPredict = calcNewRatingGrid5(games, h2hPredict, h2hPM, teamsPlayed);
		}

		calcFinalRatings(h2hPredict, ratings);
		if (gameline <= -1) {
			printRatingsInfo(teams, ratings, games, h2hPM, ties);
		}
		else {
			string x;
			string match;
			string winner;
			string loser;
			int w;
			int l;
			stringstream anotherInput;
			stringstream matchstream;

			anotherInput << input.str();

			for (int i = 0; i < 1000000; ++i) {
				getline(anotherInput, x, '\n');
				if ((x.size() >= 2 && x[0] == '/' && x[1] == '/') || x.size() == 0) {
					continue;
				}
				else if (x.size() >= 1 && x[0] == '_') {
					i = 99999999;
					break;
				}
			}

			for (int i = 0; i < gameline; ++i) {
				getline(anotherInput, match, '\n');
			}
			getline(anotherInput, match, '\n');
			match = toUpper(match);
			matchstream = stringstream(match);

			getline(matchstream, winner, ',');
			getline(matchstream, loser, '\n');
			w = findTeam(teams, winner);
			l = findTeam(teams, loser);
			if (ratings[w] > ratings[l]) {
				cout << "*";
				++correctPreds;
			}
			else {
				cout << "X";
			}
			++totalPreds;
			cout << "\t" << winner << " (" << ratings[w] << ")\tdefeats\t" << loser << " (" << ratings[l] << ")" << endl;
			cout << correctPreds << " / " << totalPreds << "\t(" << (double) correctPreds / (double) totalPreds * 100.0 << "%)" << endl;

			if (iteration % (numTeamsTotal / 2) == 1) {
				printRatingsInfo(teams, ratings, games, h2hPM, ties);
			}
		}
	}
}

int getNumTeams(string& inputIn) {
	string x;
	string match;
	string winner;
	string loser;
	int numTeamsInInput = 0;
	stringstream input(inputIn);
	stringstream inputcopy(inputIn);
	stringstream matchstream;
	vector<string> teams;

	for (int i = 0; i < 1000000; ++i) {
		getline(input, x, '\n');
		if ((x.size() >= 2 && x[0] == '/' && x[1] == '/') || x.size() == 0) {
			continue;
		}
		else if (x.size() >= 1 && x[0] == '_') {
			i = 99999999;
			break;
		}
		++numTeamsInInput;
	}
	if (numTeamsInInput == 0) {
		x = "$";
		do {
			getline(inputcopy, x, '\n');
		} while (x != "_");

		bool wX;
		bool lX;
		for (int i = 0; i < 1000000; ++i) {
			getline(inputcopy, match, '\n');
			if (match.size() == 0) {
				continue;
			}
			else if (match.size() >= 1 && match[0] == '=') {
				i = 999999999;
				continue;
			}
			else if (match.size() >= 2 && match[0] == '/' && match[1] == '/') {
				continue;
			}
			match = toUpper(match);
			matchstream = stringstream(match);

			getline(matchstream, winner, ',');
			getline(matchstream, loser, '\n');
			wX = teamExists(teams, winner);
			lX = teamExists(teams, loser);
			if (wX == false) {
				teams.push_back(winner);
				++numTeamsInInput;
			}
			if (lX == false) {
				teams.push_back(loser);
				++numTeamsInInput;
			}
		}
	}

	return numTeamsInInput;
}

int findTeam(vector<string>& teams, string& team) {
	team = toUpper(team);
	for (uint64_t i = 0; i < teams.size(); ++i) {
		//cout << teams[i] << endl;
		if (team == teams[i]) {
			return i;
		}
	}
	return teams.size() - 1;
}

bool teamExists(vector<string>& teams, string& team) {
	team = toUpper(team);
	for (uint64_t i = 0; i < teams.size(); ++i) {
		//cout << teams[i] << endl;
		if (team == teams[i]) {
			return true;
		}
	}
	return false;
}

string toUpper(string& x) {
	int cutBegin = 0;
	int cutEnd = (int) x.length();
	for (uint64_t i = 0; i < x.length(); ++i) {
		if (x[i] >= 'a' && x[i] <= 'z') {
			x[i] -= 32;
		}
	}
	for (uint64_t i = 0; i < x.length(); ++i) {
		if (x[i] >= 'A' && x[i] <= 'Z') {
			cutBegin = i;
			i = x.length();
		}
	}
	for (int i = (int) x.length(); i > 0; --i) {
		if ((x[i - 1] >= 'A' && x[i - 1] <= 'Z') || x[i - 1] == ')') {
			cutEnd = i;
			i = 0;
		}
	}
	x = x.substr(cutBegin, cutEnd);

	if (x == "CALIFORNIA-LOS ANGELES" || x == "CALIFORNIA LOS ANGELES" || x == "UC-LOS ANGELES" || x == "CALIFORNIA-LA") {
		return "UCLA";
	}
	if (x == "LOUISIANA" || x == "LOUISIANA-LAFAYETTE" || x == "LOUISIANA LAFAYETTE" || x == "ULL" || x == "UL-L" || x == "UL-LAFAYETTE") {
		return "LOUISIANA-LAFAYETTE";
	}
	if (x == "LOUISIANA-MONROE" || x == "LOUISIANA MONROE" || x == "ULM" || x == "UL-M" || x == "UL-MONROE") {
		return "LOUISIANA-MONROE";
	}
	if (x == "BRIGHAM YOUNG") {
		return "BYU";
	}
	if (x == "CENTRAL FLORIDA") {
		return "UCF";
	}
	if (x == "MIAMI (FL)" || x == "MIAMI-FL" || x == "MIAMI" || x == "MIAMI FL") {
		return "MIAMI-FL";
	}
	if (x == "MIAMI (OH)" || x == "MIAMI-OH" || x == "MIAMI OH") {
		return "MIAMI-OH";
	}
	if (x == "BOWLING GREEN STATE" || x == "BGSU") {
		return "BOWLING GREEN";
	}
	if (x == "TROY STATE") {
		return "TROY";
	}
	if (x == "ALABAMA BIRMINGHAM" || x == "ALABAMA-BIRMINGHAM") {
		return "UAB";
	}
	if (x == "BYU") {
		return "BRIGHAM YOUNG";
	}
	if (x == "CAL" || x == "UC BERKELEY") {
		return "CALIFORNIA";
	}
	if (x == "LOUISIANA STATE") {
		return "LSU";
	}
	if (x == "MISSISSIPPI") {
		return "OLE MISS";
	}
	if (x == "NEVADA-LAS VEGAS" || x == "NEVADA LAS VEGAS") {
		return "UNLV";
	}
	if (x == "UNC") {
		return "NORTH CAROLINA";
	}
	if (x == "FIU") {
		return "FLORIDA INTERNATIONAL";
	}
	if (x == "FAU") {
		return "FLORIDA ATLANTIC";
	}
	if (x == "NIU") {
		return "NORTHERN ILLINOIS";
	}
	if (x == "PENNSYLVANIA STATE" || x == "PENN ST" || x == "PSU") {
		return "PENN STATE";
	}
	if (x == "PITTSBURGH") {
		return "PITT";
	}
	if (x == "SDSU" || x == "SDST") {
		return "SAN DIEGO STATE";
	}
	if (x == "SJSU" || x == "SJST") {
		return "SAN JOSE STATE";
	}
	if (x == "SOUTHERN CALIFORNIA") {
		return "USC";
	}
	if (x == "SMU") {
		return "SOUTHERN METHODIST";
	}
	if (x == "SOUTHERN MISSISSIPPI") {
		return "SOUTHERN MISS";
	}
	if (x == "TX A&M" || x == "A&M" || x == "TAMU" || x == "TEXAS AGRICULTURAL AND MECHANICAL") {
		return "TEXAS A&M";
	}
	if (x == "TEXAS CHRISTIAN") {
		return "TCU";
	}
	if (x == "TEXAS-EL PASO" || x == "TEXAS, EL PASO" || x == "TEXAS EL PASO") {
		return "UTEP";
	}
	if (x == "TEXAS-SAN ANTONIO" || x == "TEXAS, SAN ANTONIO" || x == "TEXAS SAN ANTONIO") {
		return "UTSA";
	}
	if (x == "WVU") {
		return "WEST VIRGINIA";
	}
	if (x == "ALBANY (NY)" || x == "ALBANY" || x == "ALBANY NY") {
		return "ALBANY-NY";
	}
	if (x == "LOYOLA (IL)" || x == "LOYOLA IL" || x == "LOYOLA CHICAGO" || x == "LOYOLA-CHICAGO" || x == "LOY CHICAGO" || x == "LOY-CHICAGO") {
		return "LOYOLA-IL";
	}
	if (x == "LOYOLA (MD)" || x == "LOYOLA MD") {
		return "LOYOLA-MD";
	}
	if (x == "ST. FRANCIS (NY)" || x == "ST. FRANCIS" || x == "ST. FRANCIS NY") {
		return "ST. FRANCIS-NY";
	}
	if (x == "SAINT FRANCIS (PA)" || x == "SAINT FRANCIS" || x == "ST. FRANCIS PA" || x == "SAINT FRANCIS PA") {
		return "SAINT FRANCIS-PA";
	}
	if (x == "SAINT MARY'S (CA)" || x == "SAINT MARY'S" || x == "SAINT MARY'S CA") {
		return "SAINT MARY'S-CA";
	}
	if (x == "ST. JOHN'S (NY)" || x == "ST. JOHN'S" || x == "ST. JOHN'S NY") {
		return "ST. JOHN'S-NY";
	}
	return x;
}


void getInput3(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams, stringstream& inputIn) {
	string x;
	string match;
	string winner;
	string loser;
	int w;
	int l;
	stringstream input;
	stringstream inputcopy;
	stringstream matchstream;

	x = "$";
	do {
		getline(inputIn, x, '\n');
		input << x << "\n";
		inputcopy << x << "\n";
	} while (x.find('=') == string::npos);

	numTeams = 0;
	for (int i = 0; i < 1000; ++i) {
		getline(input, x, '\n');
		if ((x.size() >= 2 && x[0] == '/' && x[1] == '/') || x.size() == 0) {
			continue;
		}
		else if (x.size() >= 1 && x[0] == '_') {
			i = 99999999;
			break;
		}
		++numTeams;
		x = toUpper(x);
		teams.push_back(x);
	}

	if (numTeams == 0) {
		x = "$";
		do {
			getline(inputcopy, x, '\n');
		} while (x != "_");

		bool wX;
		bool lX;
		for (int i = 0; i < 1000000; ++i) {
			getline(inputcopy, match, '\n');
			if (match.size() == 0) {
				continue;
			}
			else if (match.size() >= 1 && match[0] == '=') {
				i = 999999999;
				continue;
			}
			else if (match.size() >= 2 && match[0] == '/' && match[1] == '/') {
				continue;
			}
			match = toUpper(match);
			matchstream = stringstream(match);

			getline(matchstream, winner, ',');
			getline(matchstream, loser, '\n');
			wX = teamExists(teams, winner);
			lX = teamExists(teams, loser);
			if (wX == false) {
				teams.push_back(winner);
				++numTeams;
			}
			if (lX == false) {
				teams.push_back(loser);
				++numTeams;
			}
		}
	}

	h2hPM.resize(numTeams);
	games.resize(numTeams);
	gamesPlayed.resize(numTeams);
	teamsPlayed.resize(numTeams);
	ties.resize(numTeams);
	for (int i = 0; i < numTeams; ++i) {
		h2hPM[i].resize(numTeams);
		games[i].resize(numTeams);
		ties[i].resize(numTeams);
	}



	for (int i = 0; i < 1000000; ++i) {
		bool tie = false;

		getline(input, match, '\n');
		if (match.size() == 0) {
			continue;
		}
		else if (match.size() >= 1 && match[0] == '=') {
			i = 999999999;
			continue;
		}
		else if (match.size() >= 2 && match[0] == '/' && match[1] == '/') {
			continue;
		}
		else if (match[0] == '%') {
			tie = true;
		}
		match = toUpper(match);
		matchstream = stringstream(match);

		getline(matchstream, winner, ',');
		getline(matchstream, loser, '\n');
		w = findTeam(teams, winner);
		l = findTeam(teams, loser);
		if (abs(games[w][l]) <= 0.0001) {
			teamsPlayed[w] += 1;
			teamsPlayed[l] += 1;
		}
		if (tie) {
			ties[w][l] += 1;
			ties[l][w] += 1;
		}
		else {
			h2hPM[w][l] += 1;
			h2hPM[l][w] -= 1;
		}
		games[w][l] += 1;
		games[l][w] += 1;
		gamesPlayed[w] += 1;
		gamesPlayed[l] += 1;
	}
}

int getInput3H2HTest(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams, string& inputIn, double startPoint, int iteration) {
	string x;
	string match;
	string winner;
	string loser;
	int w;
	int l;
	stringstream input(inputIn);
	stringstream inputcopy(inputIn);
	stringstream matchstream;
	bool stopTime = false;

	if (numTeamsTotal <= 0) {
		numTeamsTotal = 0;
		for (int i = 0; i < 1000000; ++i) {
			getline(input, x, '\n');
			if ((x.size() >= 2 && x[0] == '/' && x[1] == '/') || x.size() == 0) {
				continue;
			}
			else if (x.size() >= 1 && x[0] == '_') {
				i = 99999999;
				break;
			}
			++numTeamsTotal;
			x = toUpper(x);
			teams.push_back(x);
		}

		if (numTeamsTotal == 0) {
			x = "$";
			do {
				getline(inputcopy, x, '\n');
			} while (x.find('_') == string::npos);

			bool wX;
			bool lX;
			for (int i = 0; i < 1000000; ++i) {
				getline(inputcopy, match, '\n');
				if (match.size() == 0) {
					continue;
				}
				else if (match.size() >= 1 && match[0] == '=') {
					i = 999999999;
					continue;
				}
				else if (match.size() >= 2 && match[0] == '/' && match[1] == '/') {
					continue;
				}
				match = toUpper(match);
				matchstream = stringstream(match);

				getline(matchstream, winner, ',');
				getline(matchstream, loser, '\n');
				wX = teamExists(teams, winner);
				lX = teamExists(teams, loser);
				if (wX == false) {
					teams.push_back(winner);
					++numTeamsTotal;
				}
				if (lX == false) {
					teams.push_back(loser);
					++numTeamsTotal;
				}
			}
		}

		h2hPM.resize(numTeamsTotal);
		games.resize(numTeamsTotal);
		gamesPlayed.resize(numTeamsTotal);
		teamsPlayed.resize(numTeamsTotal);
		ties.resize(numTeamsTotal);
		for (int i = 0; i < numTeamsTotal; ++i) {
			h2hPM[i].resize(numTeamsTotal);
			games[i].resize(numTeamsTotal);
			ties[i].resize(numTeamsTotal);
		}
	}
	else {
		x = "$";
		while (x.find('_') == string::npos) {
			getline(input, x, '\n');
		}
	}



	int gameline = (int) (startPoint * numTeams / 2.0) + iteration;

	for (int i = 0; i < 1000000; ++i) {
		int numSkips = (iteration <= 0 ? 0 : gameline - 1);

		getline(input, match, '\n');
		bool tie = false;

		if (i < numSkips) {
			continue;
		}
		if (match.size() == 0) {
			continue;
		}
		else if (match.size() >= 1 && match[0] == '=') {
			i = 999999999;
			stopTime = true;
			continue;
		}
		else if (i >= gameline) {
			i = 999999999;
			continue;
		}
		else if (match.size() >= 2 && match[0] == '/' && match[1] == '/') {
			continue;
		}
		else if (match[0] == '%') {
			tie = true;
		}
		match = toUpper(match);
		matchstream = stringstream(match);

		getline(matchstream, winner, ',');
		getline(matchstream, loser, '\n');
		w = findTeam(teams, winner);
		l = findTeam(teams, loser);
		if (abs(games[w][l]) <= 0.0001) {
			teamsPlayed[w] += 1;
			teamsPlayed[l] += 1;
		}
		if (tie) {
			ties[w][l] += 1;
			ties[l][w] += 1;
		}
		else {
			h2hPM[w][l] += 1;
			h2hPM[l][w] -= 1;
		}
		games[w][l] += 1;
		games[l][w] += 1;
		gamesPlayed[w] += 1;
		gamesPlayed[l] += 1;
	}
	if (stopTime) {
		return -1;
	}
	return gameline;
}


void calcFinalRatings(vector<vector<double> >& h2hPredict, vector<double>& ratings) {
	for (uint64_t i = 0; i < ratings.size(); ++i) {
		double sum = 0;
		for (uint64_t j = 0; j < ratings.size(); ++j) {
			sum += h2hPredict[i][j];
			//cout << h2hPredict[i][j] << endl;
		}
		ratings[i] = sum;
	}
}


vector<vector<double> > calcNewRatingGrid5(vector<vector<double>>& games, vector<vector<double> >& h2h, vector<vector<double>>& h2hPM, vector<int>& teamsPlayed) {
	vector<vector<double> > newGrid;
	vector<double> tempRatings;
	vector<double> tempAbsRatings;
	double prediction = 0;
	newGrid.resize(h2h.size());
	tempRatings.resize(h2h.size());
	tempAbsRatings.resize(h2h.size());
	for (uint64_t i = 0; i < newGrid.size(); ++i) {
		newGrid[i].resize(h2h[i].size());
		for (uint64_t j = 0; j < newGrid[i].size(); ++j) {
			tempRatings[i] += h2h[i][j];
			tempAbsRatings[i] += abs(h2h[i][j]);
		}
	}
	vector<double> ntr = normalize(tempRatings);
	for (uint64_t i = 0; i < newGrid.size(); ++i) {
		for (uint64_t j = 0; j < newGrid.size(); ++j) {
			prediction = predictH2Hsquare5(games, h2h, h2hPM, tempRatings, tempAbsRatings, ntr, teamsPlayed, i, j);
			newGrid[i][j] = prediction;
		}
	}
	return newGrid;
}

void printRatings(vector<string>& teams, vector<double>& ratings) {
	for (uint64_t i = 0; i < teams.size(); ++i) {
		cout << teams[i] << "," << ratings[i] << endl;
	}
}

void printRatingsInfo(vector<string>& teams, vector<double>& ratings, vector<vector<double>>& games, vector<vector<double>>& h2hPM, vector<vector<int>>& ties) {
	vector<Team> teamRatings;
	vector<double> normRatings;
	bool hasTies = false;

	teamRatings.reserve(ratings.size());
	normRatings.reserve(ratings.size());
	normRatings = normalize(ratings);

	for (int i = 0; i < (int) ties.size(); ++i) {
		for (int j = 0; j < (int) ties[i].size(); ++j) {
			if (ties[i][j] >= 1) {
				hasTies = true;
				break;
			}
		}
		if (hasTies) {
			break;
		}
	}

	if (!hasTies) {
		cout << "RANK,TEAM,RATING,RAW_SCORE,RAW_SCORE_PER_TEAM,WINS,LOSSES" << endl;
		int wins = 0;
		int losses = 0;
		for (uint64_t i = 0; i < teams.size(); ++i) {
			wins = 0;
			losses = 0;
			for (uint64_t j = 0; j < games[i].size(); ++j) {
				if (games[i][j] != 0) {
					wins += (int) (((h2hPM[i][j] + 1.0) / 2.0) * games[i][j] + 0.0000001);
					losses += (int) (games[i][j] - ((h2hPM[i][j] + 1.0) / 2.0) * games[i][j] + 0.0000001);
				}
			}
			teamRatings.push_back(Team(teams[i], normRatings[i], ratings[i], wins, losses));
		}
		sort(teamRatings.begin(), teamRatings.end(), comp);
		for (uint64_t i = 0; i < teamRatings.size(); ++i) {
			teamRatings[i].rank = i + 1;
			cout << teamRatings[i].rank << "," << teamRatings[i].name << "," << teamRatings[i].rating << "," << teamRatings[i].rawScore << "," << teamRatings[i].rawScore / ((double) (numTeamsTotal - 1)) << "," << teamRatings[i].wins << "," << teamRatings[i].losses << endl;
		}
	}
	else {
		cout << "RANK,TEAM,RATING,RAW_SCORE,RAW_SCORE_PER_TEAM,WINS,LOSSES,TIES" << endl;
		int wins = 0;
		int losses = 0;
		int tieGames = 0;
		for (int i = 0; i < (int) teams.size(); ++i) {
			wins = 0;
			losses = 0;
			tieGames = 0;
			for (int j = 0; j < (int) games[i].size(); ++j) {
				if (games[i][j] != 0) {
					wins += (int) (((h2hPM[i][j] + 1.0) / 2.0) * (games[i][j]) + 0.0000001);
					losses += (int) (games[i][j] - ((h2hPM[i][j] + 1.0) / 2.0) * (games[i][j]) + 0.0000001);
					tieGames += ties[i][j];
				}
			}
			teamRatings.push_back(Team(teams[i], normRatings[i], ratings[i], wins, losses, tieGames));
		}
		sort(teamRatings.begin(), teamRatings.end(), comp);
		for (uint64_t i = 0; i < teamRatings.size(); ++i) {
			teamRatings[i].rank = i + 1;
			cout << teamRatings[i].rank << "," << teamRatings[i].name << "," << teamRatings[i].rating << "," << teamRatings[i].rawScore << "," << teamRatings[i].rawScore / ((double) (numTeamsTotal - 1)) << "," << teamRatings[i].wins << "," << teamRatings[i].losses << "," << teamRatings[i].ties << endl;
		}
	}
}

void printGrid2(vector<vector<double>>& grid, vector<string>& teams) {
	for (uint64_t i = 0; i < grid.size(); ++i) {
		cout << "," << teams[i];
	}
	cout << endl;
	for (uint64_t i = 0; i < grid.size(); ++i) {
		cout << teams[i];
		for (uint64_t j = 0; j < grid[i].size(); ++j) {
			cout << "," << grid[i][j];
		}
		cout << endl;
	}
}



double predictH2Hsquare5(vector<vector<double>>& games, vector<vector<double>>& prevH2H, vector<vector<double>>& h2hPM, vector<double>& tempRatings, vector<
double>& tempAbsRatings, vector<double>& ntr, vector<int>& teamsPlayed, int a, int b) {
	if (a == b) {
		return 0;
	}
	double ntrAAdj = rt2(ntr[a] * 2.0 - 1.0) / 4.0 + 0.75;
	//double revNtrAAdj = 1.5 - ntrAAdj;

	double ntrBAdj = rt2(ntr[b] * 2.0 - 1.0) / 4.0 + 0.75;
	double revNtrBAdj = 1.5 - ntrBAdj;

	double proximityImp = 1.0 - (abs((ntr[a]) - (ntr[b]))) / 2.0;
	//double proximityImpAdj = 1.0 - (abs(((ntrAAdj) - (ntrBAdj)))) / 2.0;
	double rev2multImp = 1.0 - (1.0 - ntr[a]) * (1.0 - ntr[b]);
	//double rev2multImpAdj = 1.0 - (1.0 - ntrAAdj) * (1.0 - ntrBAdj);
	//double multImp = ntr[a] * ntr[b];
	double multImpAdj = ntrAAdj * ntrBAdj;
	double importance = sqrt(pow2(proximityImp) * (rev2multImp) * (multImpAdj) * ntrBAdj + 1.0 / double(numTeamsTotal));

	double oppQAdj = ((h2hPM[a][b] >= 0.0 ? 2.0 : 0.0) * importance * (ntrBAdj)) / (sqrt(numTeamsTotal) * teamsPlayed[a] + 1.0);
	double negOppQAdj = -(2.0 * importance * (ntrBAdj)) / (sqrt(numTeamsTotal) * (numTeamsTotal - teamsPlayed[a]) + 1.0);

	double nonPlayAdj = 1.0;
	double playAdj = 1.0;

	double nonPlayPenalty = (ntr[a] >= ntr[b] ? revNtrBAdj - 0.5 : ntrBAdj - 0.5);
	double competitionBonus = (h2hPM[a][b] >= 0 ? ntrBAdj + 0.0 : revNtrBAdj + 0.0);

	if (games[a][b] != 0) {
		double gameScore = playAdj * (competitionBonus * (importance) * (h2hPM[a][b] * games[a][b]) / (games[a][b] + 1)) + oppQAdj;
		if (games[a][b] > 1.0) {
			return rtX(gameScore, (sqrt(games[a][b]) * abs(h2hPM[a][b]) + 1));
		}
		else {
			return rt2(gameScore);
		}
	}
	double sqPrediction = nonPlayAdj * nonPlayPenalty * (importance) * ((((tempRatings[a]) - (tempRatings[b])) / ((tempAbsRatings[a]) + (tempAbsRatings[b]))) / 2.0) + negOppQAdj;
	double prediction = rt2(sqPrediction);
	return prediction;
}



double rt2(double x) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), 1.0 / 2.0);
}

double rtX(double x, double denom) {
	return powX(x, 1.0 / denom);
}

double powX(double x, double exponent) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), exponent);
}

double pow2(double x) {
	return powX(x, 2);
}

double logyX(double x, double base) {
	if (x > 0) {
		return (log((base - 1) * x + 1) / log(base));
	}
	else if (x < 0) {
		return (-log((base - 1) * abs(x) + 1) / log(base));
	}
	else {
		return 0;
	}
}

double logyT(double x) {
	return logyX(x, sqrt(numTeamsTotal));
}

double expyX(double x, double base) {
	if (x > 0) {
		return (pow(base, x) - 1) / (base - 1);
	}
	else if (x < 0) {
		return -(pow(base, abs(x)) - 1) / (base - 1);
	}
	else {
		return 0;
	}
}

double expyT(double x) {
	return expyX(x, numTeamsTotal);
}

double sinX(double x) {
	return sin(PI * x / 2);
}

double asinX(double x) {
	return 2 * asin(x) / PI;
}

double circX(double x) {
	return rtX(sinX(x), 2.0);
}

double circX(double x, double power) {
	return rtX(sinX(x), power);
}

double acircX(double x) {
	return powX(asinX(x), 2.0);
}

double acircX(double x, double power) {
	return powX(asinX(x), power);
}



double rootGamesPlus2(double x, double games) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), 1.0 / (2.0 + games));
}

int sum(vector<int>& v) {
	int total = 0;
	for (int x : v) {
		total += x;
	}
	return total;
}

double avg(vector<int>& v) {
	return (double) sum(v) / (double) v.size();
}

vector<double> normalize(vector<double>& x) {
	vector<double> normed;
	double min = numeric_limits<double>::max();
	double max = numeric_limits<double>::lowest();

	normed.resize(x.size());

	for (double d : x) {
		if (d > max) {
			max = d;
		}
		if (d < min) {
			min = d;
		}
	}

	max = max - min;

	for (uint64_t i = 0; i < x.size(); ++i) {
		normed[i] = (x[i] - min) / max;
	}

	return normed;
}

vector<double> normalizeRT2(vector<double>& x) {
	vector<double> normed;
	double min = numeric_limits<double>::max();
	double max = numeric_limits<double>::lowest();

	normed.resize(x.size());

	for (double d : x) {
		if (d > max) {
			max = d;
		}
		if (d < min) {
			min = d;
		}
	}

	max = max - min;

	for (uint64_t i = 0; i < x.size(); ++i) {
		normed[i] = rt2((x[i] - min) / max);
	}

	return normed;
}

vector<double> normalizePow(vector<double>& x, double exponent) {
	vector<double> normed;
	double min = numeric_limits<double>::max();
	double max = numeric_limits<double>::lowest();

	normed.resize(x.size());

	for (double d : x) {
		if (d > max) {
			max = d;
		}
		if (d < min) {
			min = d;
		}
	}

	max = max - min;

	for (uint64_t i = 0; i < x.size(); ++i) {
		normed[i] = pow(((x[i] - min) / max), exponent);
	}

	return normed;
}
