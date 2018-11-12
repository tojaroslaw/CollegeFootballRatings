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
void runH2HTest(string& inputIn, double startPoint, int batchSize);
int getNumTeams(string& inputIn);

void getInput3(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams, stringstream& inputIn);
int getInput3H2HTest(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams, string& inputIn, double startPoint, int iterationIn, int batchSize);
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
	//runH2HTest(inputString, 7.0, 30);
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

void runH2HTest(string& inputIn, double startPoint, int batchSize) {
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
	string lastMatch = "";

	int iteration = 0;
	int justNumTeams = getNumTeams(inputIn);
	int gameline = 0;
	int correctPreds = 0;
	int totalPreds = 0;

	gameline = 0;
	while (gameline >= 0) {
		gameline = getInput3H2HTest(h2hPM, games, ties, gamesPlayed, teamsPlayed, teams, justNumTeams, inputIn, startPoint, iteration, batchSize);
		cout << iteration * batchSize << ", " << gameline << endl;

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
			for (int i = 0; i < batchSize; ++i) {
				getline(anotherInput, match, '\n');
				if (lastMatch == match || match == "=" || match == "") {
					gameline = -1;
					i = 99999999;
					cout << "BREAK\t" << match << endl;
					//printRatingsInfo(teams, ratings, games, h2hPM, ties);
					stringstream progInput;
					stringstream progInputCopy;
					progInput << input.str();
					progInputCopy << input.str();
					runProgram(progInput, progInputCopy);
					cout << correctPreds << " / " << totalPreds << "\t(" << (double) correctPreds / (double) totalPreds * 100.0 << "%)" << endl;
					cout << "END" << endl;
					return;
				}
				matchstream = stringstream(match);

				if (/*(iteration - batchSize + i) % (numTeamsTotal / 2) >= 0*/i == 0) {
					printRatingsInfo(teams, ratings, games, h2hPM, ties);
				}

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
			}
		}
		++iteration;
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
	string newX = "";
	for (uint64_t i = 0; i < x.length(); ++i) {
		if (x[i] == '.' || x[i] == ',') {
			continue;
		}
		newX += x[i];
	}
	x = newX;

	if (x == "ABILENE CHR") {
		return "ABILENE CHRISIAN";
	}
	if (x == "BAMA" || x == "'BAMA") {
		return "ALABAMA";
	}
	if (x == "ALABAMA STATE") {
		return "ALABAMA ST";
	}
	if (x == "ALBANY (NY)" || x == "ALBANY" || x == "ALBANY NY" || x == "SUNY ALBANY" || x == "SUNY-ALBANY") {
		return "ALBANY-NY";
	}
	if (x == "ALCORN STATE") {
		return "ALCORN ST";
	}
	if (x == "AMERICAN UNIV" || x == "AMERICAN UNIVERSITY" || x == "AMERICAN U") {
		return "AMERICAN";
	}
	if (x == "APPALACHIAN STATE") {
		return "APPALACHIAN ST";
	}
	if (x == "AZ" || x == "ARIZ") {
		return "ARIZONA";
	}
	if (x == "ARIZONA STATE" || x == "AZ ST" || x == "ARIZ ST") {
		return "ARIZONA ST";
	}
	if (x == "ARK LITTLE ROCK" || x == "AR LITTLE ROCK" || x == "ARK-LITTLE ROCK" || x == "AR-LITTLE ROCK" || x == "ARKANSAS LITTLE ROCK") {
		return "ARKANSAS-LITTLE ROCK";
	}
	if (x == "ARKANSAS STATE") {
		return "ARKANSAS ST";
	}
	if (x == "ARKINE BLUFF" || x == "AR PINE BLUFF" || x == "ARK PINE BLUFF" || x == "ARKANSAS PINE BLUFF" || x == "AR-PINE BLUFF" || x == "ARK-PINE BLUFF") {
		return "ARKANSAS-PINE BLUFF";
	}
	if (x == "AUSTINEAY" || x == "AUSTIN-PEAY") {
		return "AUSTIN PEAY";
	}
	if (x == "BALL STATE") {
		return "BALL ST";
	}
	if (x == "BETHUNE COOKMAN") {
		return "BETHUNE-COOKMAN";
	}
	if (x == "SUNY BINGHAMTON" || x == "SUNY-BINGHAMTOM") {
		return "BINGHAMTON";
	}
	if (x == "BOISE STATE") {
		return "BOISE ST";
	}
	if (x == "BC" || x == "BOSTON C" || x == "BOSTON COL") {
		return "BOSTON COLLEGE";
	}
	if (x == "BU" || x == "BOSTON UNIVERSITY" || x == "BOSTON U") {
		return "BOSTON UNIV";
	}
	if (x == "BOWLING GREEN STATE" || x == "BGSU" || x == "BG" || x == "BG ST" || x == "BGST") {
		return "BOWLING GREEN";
	}
	if (x == "BRIGHAM YOUNG") {
		return "BYU";
	}
	if (x == "SUNY BUFFALO") {
		return "BUFFALO";
	}
	if (x == "C MICHIGAN" || x == "CMU") {
		return "CENTRAL MICHIGAN";
	}
	if (x == "CAL" || x == "UC BERKELEY" || x == "UC-BERKELEY") {
		return "CALIFORNIA";
	}
	if (x == "CALOLY" || x == "CAL-POLY" || x == "CALIFORNIA POLY" || x == "CALIFORNIA-POLY" || x == "CALIFORNIA POLYTECHNIC" || x == "CALIFORNIA POLYTECHNIC INSTITUTE") {
		return "CAL POLY";
	}
	if (x == "CENT ARKANSAS" || x == "CENTRAL ARK" || x == "CENT ARK" || x == "C ARK" || x == "C ARKANSAS") {
		return "CENTRAL ARKANSAS";
	}
	if (x == "CENTRAL CONN" || x == "C CONN" || x == "C CONNECTICUT" || x == "CENT CONN" || x == "CENT CONNECTICUT") {
		return "CENTRAL CONNECTICUT";
	}
	if (x == "CHARLESTON SO") {
		return "CHARLESTON SOUTHERN";
	}
	if (x == "UT CHATTANOOGA" || x == "UT-CHATTANOOGA" || x == "TENN CHATTANOOGA" || x == "TENN-CHATTANOOGA" || x == "TENNESSEE CHATTANOOGA" || x == "TENNESSEE-CHATTANOOGA") {
		return "CHATTANOOGA";
	}
	if (x == "CHICAGO STATE") {
		return "CHICAGO ST";
	}
	if (x == "CIN" || x == "CINCY" || x == "CINCI") {
		return "CINCINNATTI";
	}
	if (x == "THE CITADEL") {
		return "CITADEL";
	}
	if (x == "CLEVELAND STATE") {
		return "CLEVELAND ST";
	}
	if (x == "C CAROLINA" || x == "C CAR" || x == "COASTAL CAR") {
		return "COASTAL CAROLINA";
	}
	if (x == "COL CHARLESTON" || x == "C OF C" || x == "C CHARLESTON" || x == "COLLEGE CHARLESTON" || x == "COL OF CHARLESTON" || x == "C OF CHARLESTON") {
		return "COLLEGE OF CHARLESTON";
	}
	if (x == "COLORADO STATE") {
		return "COLORADO ST";
	}
	if (x == "COPPIN STATE") {
		return "COPPIN ST";
	}
	if (x == "CS BAKERSFIELD" || x == "CS-BAKERSFIELD" || x == "CAL ST-BAKERSFIELD" || x == "CAL STATE-BAKERSFIELD" || x == "CAL STATE-BAKERSFIELD" || x == "CALIFORNIA ST BAKERSFIELD" || x == "CALIFORNIA ST-BAKERSFIELD" || x == "CALIFORNIA STATE BAKERSFIELD" || x == "CALIFORNIA STATE-BAKERSFIELD") {
		return "CAL ST BAKERSFIELD";
	}
	if (x == "CS FULLERTON" || x == "CS-FULLERTON" || x == "CAL ST-FULLERTON" || x == "CAL STATE-FULLERTON" || x == "CAL STATE-FULLERTON" || x == "CALIFORNIA ST FULLERTON" || x == "CALIFORNIA ST-FULLERTON" || x == "CALIFORNIA STATE FULLERTON" || x == "CALIFORNIA STATE-FULLERTON") {
		return "CAL ST FULLERTON";
	}
	if (x == "CS NORTHRIDGE" || x == "CS-NORTHRIDGE" || x == "CAL ST-NORTHRIDGE" || x == "CAL STATE-NORTHRIDGE" || x == "CAL STATE-NORTHRIDGE" || x == "CALIFORNIA ST NORTHRIDGE" || x == "CALIFORNIA ST-NORTHRIDGE" || x == "CALIFORNIA STATE NORTHRIDGE" || x == "CALIFORNIA STATE-NORTHRIDGE") {
		return "CAL ST NORTHRIDGE";
	}
	if (x == "CS SACRAMENTO" || x == "CS-SACRAMENTO" || x == "CAL ST-SACRAMENTO" || x == "CAL STATE-SACRAMENTO" || x == "CAL STATE-SACRAMENTO" || x == "CALIFORNIA ST SACRAMENTO" || x == "CALIFORNIA ST-SACRAMENTO" || x == "CALIFORNIA STATE SACRAMENTO" || x == "CALIFORNIA STATE-SACRAMENTO") {
		return "CAL ST SACRAMENTO";
	}
	if (x == "DELAWARE STATE") {
		return "DELAWARE ST";
	}
	if (x == "E CAROLINA") {
		return "EAST CAROLINA";
	}
	if (x == "ETSU" || x == "E TENNESSEE" || x == "E TENN" || x == "EAST TENNESSEE" || x == "EAST TENN" || x == "EAST TENNESSEE STATE" || x == "E TENNESSEE ST" || x == "E TENN ST") {
		return "EAST TENNESSEE ST";
	}
	if (x == "E ILLINOIS") {
		return "EASTERN ILLINOIS";
	}
	if (x == "E KENTUCKY") {
		return "EASTERN KENTUCKY";
	}
	if (x == "E MICHIGAN" || x == "EMU") {
		return "EASTERN MICHIGAN";
	}
	if (x == "E WASHINGTON") {
		return "EASTERN WASHINGTON";
	}
	if (x == "F DICKINSON" || x == "F-DICKINSON" || x == "FARLEIGH DICKINSON") {
		return "FARLEIGH DICKINSON";
	}
	if (x == "FLORIDA ATLANTIC" || x == "FL ATLANTIC" || x == "FL ATL" || x == "FLORIDA ATL") {
		return "FAU";
	}
	if (x == "FLORIDA INTERNATIONAL" || x == "FL INTL" || x == "FLORIDA INTL" || x == "FL INTERNATIONAL") {
		return "FIU";
	}
	if (x == "FAM" || x == "FA&M" || x == "FL AM" || x == "FL A&M" || x == "FLORIDA AM" || x == "FAMU") {
		return "FLORIDA A&M";
	}
	if (x == "FL GULF COAST" || x == "FGC") {
		return "FLORIDA GULF COAST";
	}
	if (x == "FSU" || x == "FL ST" || x == "FL STATE" || x == "FLORIDA STATE") {
		return "FLORIDA ST";
	}
	if (x == "FRESNO STATE") {
		return "FRESNO ST";
	}
	if (x == "G WASHINGTON" || x == "GW" || x == "GWU") {
		return "GEORGE WASHINGTON";
	}
	if (x == "GA SOUTHERN" || x == "GA SO" || x == "GA S" || x == "GEORGIA SO" || x == "GEORGIA SOUTHERN") {
		return "GEORGIA SOUTHERN";
	}
	if (x == "GARDNER-WEBB") {
		return "GARDNER-WEBB";
	}
	if (x == "GTOWN" || x == "G'TOWN") {
		return "GEORGETOWN";
	}
	if (x == "UGA") {
		return "GEORGIA";
	}
	if (x == "GEORGIA STATE" || x == "GA STATE" || x == "GA ST") {
		return "GEORGIA ST";
	}
	if (x == "GA TECH" || x == "GEORGIA INSTITUTE OF TECHNOLOGY") {
		return "GEORGIA TECH";
	}
	if (x == "GRAMBLING" || x == "GRAMBLING STATE") {
		return "GRAMBLING ST";
	}
	if (x == "HAWAI'I") {
		return "HAWAII";
	}
	if (x == "HIGHOINT" || x == "HI PT" || x == "HIGH PT" || x == "HI POINT") {
		return "HIGH POINT";
	}
	if (x == "HOUSTON BAP") {
		return "HOUSTON BAPTIST";
	}
	if (x == "IDAHO STATE") {
		return "IDAHO ST";
	}
	if (x == "IL CHICAGO" || x == "IL-CHICAGO" || x == "ILLINOIS CHICAGO") {
		return "ILLINOIS-CHICAGO";
	}
	if (x == "ILLINOIS STATE") {
		return "ILLINOIS ST";
	}
	if (x == "INDIANA STATE") {
		return "INDIANA ST";
	}
	if (x == "IOWA STATE") {
		return "IOWA ST";
	}
	if (x == "IPFW" || x == "INDIANA-PURDUE FW" || x == "INDIANA-PURDUE-FW" || x == "INDIANA-PURDUE FORT WAYNE" || x == "IUPUFW") {
		return "INDIANA-PURDUE-FORT WAYNE";
	}
	if (x == "IUPU-INDIANAPOLIS" || x == "INDIANA-PURDUE-I" || x == "INDIANA-PURDUE-INDIANAPOLIS") {
		return "IUPUI";
	}
	if (x == "JACKSON STATE") {
		return "JACKSON ST";
	}
	if (x == "JACKSONVILLE STATE" || x == "JVILLE ST" || x == "J'VILLE ST" || x == "JVILLE STATE" || x == "J'VILLE STATE") {
		return "JACKSONVILLE ST";
	}
	if (x == "JMU") {
		return "JAMES MADISON";
	}
	if (x == "KANSAS STATE" || x == "K ST" || x == "K-ST" || x == "K STATE" || x == "K-STATE") {
		return "KANSAS ST";
	}
	if (x == "KENNESAW" || x == "KENNESAW STATE") {
		return "KENNESAW ST";
	}
	if (x == "KENT" || x == "KENT STATE") {
		return "KENT ST";
	}
	if (x == "LONG BEACH STATE") {
		return "LONG BEACH ST";
	}
	if (x == "LOUISIANA" || x == "LOUISIANA-LAFAYETTE" || x == "LOUISIANA LAFAYETTE" || x == "ULL" || x == "UL-L" || x == "UL-LAFAYETTE") {
		return "LOUISIANA-LAFAYETTE";
	}
	if (x == "LOUISIANA-MONROE" || x == "LOUISIANA MONROE" || x == "ULM" || x == "UL-M" || x == "UL-MONROE") {
		return "LOUISIANA-MONROE";
	}
	if (x == "LA TECH") {
		return "LOUISIANA TECH";
	}
	if (x == "LOYOLA (IL)" || x == "LOYOLA IL" || x == "LOYOLA CHICAGO" || x == "LOYOLA-CHICAGO" || x == "LOY CHICAGO" || x == "LOY-CHICAGO") {
		return "LOYOLA-IL";
	}
	if (x == "LOY MARYMOUNT" || x == "LOYOLA-MARYMOUNT" || x == "LOY-MARYMOUNT" || x == "LOYOLA (MARYMOUNT)") {
		return "LOYOLA MARYMOUNT";
	}
	if (x == "LOYOLA (MD)" || x == "LOYOLA MD") {
		return "LOYOLA-MD";
	}
	if (x == "LOUISIANA STATE" || x == "LA STATE" || x == "LA ST" || x == "LOUISIANA ST") {
		return "LSU";
	}
	if (x == "UMD") {
		return "MARYLAND";
	}
	if (x == "MCNEESE STATE") {
		return "MCNEESE STATE";
	}
	if (x == "MD E SHORE" || x == "MARYLAND E SHORE" || x == "MD-E SHORE" || x == "MARYLAND-E SHORE" || x == "MD EASTERN SHORE" || x == "MARYLAND EASTERN SHORE" || x == "MD-EASTERN SHORE") {
		return "MARYLAND-EASTERN SHORE";
	}
	if (x == "MIAMI (FL)" || x == "MIAMI" || x == "MIAMI FL") {
		return "MIAMI-FL";
	}
	if (x == "MIAMI (OH)" || x == "MIAMI OH") {
		return "MIAMI-OH";
	}
	if (x == "UMICH" || x == "MICH") {
		return "MICHIGAN";
	}
	if (x == "MICHIGAN STATE" || x == "MSU") {
		return "MICHIGAN ST";
	}
	if (x == "MTSU" || x == "MIDDLE TENNESSEE STATE" || x == "MIDDLE TENNESSEE ST" || x == "MID TENNESSEE STATE" || x == "MID TENNESSEE ST" || x == "MID TENNESSEE" || x == "MIDDLE TENN STATE" || x == "MIDDLE TENN ST" || x == "MIDDLE TENN" || x == "MID TENN STATE" || x == "MID TENN" || x == "MID TENN") {
		return "MIDDLE TENNESSEE";
	}
	if (x == "MISSISSIPPI STATE" || x == "MISS STATE" || x == "MISS ST") {
		return "MISSISSIPPI ST";
	}
	if (x == "MS VALLEY ST" || x == "MISSISSIPPI VALLEY STATE" || x == "MS VALLEY STATE") {
		return "MISSISSIPPI VALLEY ST";
	}
	if (x == "MIZZOU") {
		return "MISSOURI";
	}
	if (x == "MISSOURI KC" || x == "MO KC" || x == "MISSOURI KANSAS CITY" || x == "MISSOURI-KC" || x == "MO-KC") {
		return "MISSOURI-KANSAS CITY";
	}
	if (x == "MISSOURI STATE" || x == "MO STATE" || x == "MO ST") {
		return "MISSOURI ST";
	}
	if (x == "MONMOUTH" || x == "MONMOUTH NJ") {
		return "MONMOUTH-NJ";
	}
	if (x == "MONTANA STATE") {
		return "MONTANA ST";
	}
	if (x == "MOREHEAD STATE") {
		return "MOREHEAD ST";
	}
	if (x == "MORGAN STATE") {
		return "MORGAN ST";
	}
	if (x == "MT ST MARY'S" || x == "MT SAINT MARY'S" || x == "MT-ST MARY'S" || x == "MT-SAINT MARY'S" || x == "MONTANA ST MARY'S" || x == "MONTANA SAINT MARY'S" || x == "MONTANA-SAINT MARY'S") {
		return "MONTANA-ST MARY'S";
	}
	if (x == "MURRAY STATE") {
		return "MURRAY ST";
	}
	if (x == "NC CENT" || x == "NORTH CAROLINA CENTRAL" || x == "N CAROLINA CENT" || x == "N CAROLINA CENTRAL") {
		return "NC CENTRAL";
	}
	if (x == "NC STATE" || x == "N CAROLINA ST" || x == "N CAROLINA STATE" || x == "NORTH CAROLINA ST" || x == "NORTH CAROLINA STATE") {
		return "NC ST";
	}
	if (x == "NE OMAHA" || x == "NE-OMAHA" || x == "NEBRASKA OMAHA") {
		return "NEBRASKA-OMAHA";
	}
	if (x == "UNH") {
		return "NEW HAMPSHIRE";
	}
	if (x == "UNM" || x == "NM") {
		return "NEW MEXICO";
	}
	if (x == "NM ST" || x == "NM STATE" || x == "NEW MEXICO STATE") {
		return "NEW MEXICO ST";
	}
	if (x == "NOLA") {
		return "NEW ORLEANS";
	}
	if (x == "NICHOLLS STATE") {
		return "NICHOLLS ST";
	}
	if (x == "NEW JERSEY INSTITUTE OF TECHNOLOGY" || x == "NEW JERSEY INSTITUTE OF TECH" || x == "NJ INSTITUTE OF TECHNOLOGY" || x == "NJ INSTITUTE OF TECH" || x == "NEW JERSEY IT") {
		return "NJIT";
	}
	if (x == "NORFOLK STATE") {
		return "NORFOLK ST";
	}
	if (x == "UNC" || x == "N CAROLINA") {
		return "NORTH CAROLINA";
	}
	if (x == "NC A&T" || x == "NC AT" || x == "N CAROLINA A&T" || x == "N CAROLINA AT" || x == "NORTH CAROLINA AT") {
		return "NORTH CAROLINA A&T";
	}
	if (x == "UND" || x == "NODAK" || x == "N DAKOTA" || x == "NO DAKOTA") {
		return "NORTH DAKOTA";
	}
	if (x == "NORTH DAKOTA STATE" || x == "N DAKOTA ST" || x == "NDSU" || x == "NDST" || x == "ND ST") {
		return "NORTH DAKOTA ST";
	}
	if (x == "UNF" || x == "N FLORIDA" || x == "NORTH FL") {
		return "NORTH FLORIDA";
	}
	if (x == "N TEXAS" || x == "N TX" || x == "NORTH TX") {
		return "NORTH TEXAS";
	}
	if (x == "N ARIZONA") {
		return "NORTHERN ARIZONA";
	}
	if (x == "N COLORADO") {
		return "NORTHERN COLORADO";
	}
	if (x == "NIU" || x == "N ILLINOIS") {
		return "NORTHERN ILLINOIS";
	}
	if (x == "N IOWA") {
		return "NORTHERN IOWA";
	}
	if (x == "N KENTUCKY" || x == "NKU" || x == "NO KENTUCKY") {
		return "NORTHERN KENTUCKY";
	}
	if (x == "NW" || x == "NWU" || x == "N'WESTERN") {
		return "NORTHWESTERN";
	}
	if (x == "NORTHWESTERN LA" || x == "NW LA" || x == "NW LOUISIANA") {
		return "NORTHWESTERN LOUISIANA";
	}
	if (x == "ND") {
		return "NOTRE DAME";
	}
	if (x == "OHIO STATE") {
		return "OHIO ST";
	}
	if (x == "OKLAHOMA STATE" || x == "OK ST" || x == "OK-ST") {
		return "OKLAHOMA ST";
	}
	if (x == "MISSISSIPPI" || x == "MISS") {
		return "OLE MISS";
	}
	if (x == "OREGON STATE") {
		return "OREGON ST";
	}
	if (x == "PENNSYLVANIA" || x == "UPENN") {
		return "PENN";
	}
	if (x == "PENNSYLVANIA STATE" || x == "PENNSYLVANIA ST" || x == "PENN STATE" || x == "PSU") {
		return "PENN ST";
	}
	if (x == "PITTSBURGH") {
		return "PITT";
	}
	if (x == "PORTLAND STATE") {
		return "PORTLAND ST";
	}
	if (x == "PRAIRIE VIEW" || x == "PRAIRIE VIEW AM") {
		return "PRAIRIE VIEW A&M";
	}
	if (x == "SAM HOUSTON STATE") {
		return "SAM HOUSTON ST";
	}
	if (x == "SDSU" || x == "SDST") {
		return "SAN DIEGO STATE";
	}
	if (x == "SJSU" || x == "SJST") {
		return "SAN JOSE STATE";
	}
	if (x == "SAVANNAH STATE") {
		return "SAVANNAH ST";
	}
	if (x == "S METHODIST" || x == "SOUTHERN METHODIST") {
		return "SMU";
	}
	if (x == "S ALABAMA") {
		return "SOUTH ALABAMA";
	}
	if (x == "S CAROLINA" || x == "SC") {
		return "SOUTH CAROLINA";
	}
	if (x == "S DAKOTA") {
		return "SOUTH DAKOTA";
	}
	if (x == "USF" || x == "S FLORIDA") {
		return "SOUTH CAROLINA";
	}
	if (x == "S CAROLINA ST" || x == "S CAROLINA STATE" || x == "SC ST" || x == "SC STATE" || x == "SOUTH CAROLINA STATE") {
		return "SOUTH CAROLINA ST";
	}
	if (x == "S CAROLINA UPSTATE" || x == "SC UPSTATE") {
		return "SOUTH CAROLINA UPSTATE";
	}
	if (x == "SE LOUISIANA" || x == "SE LA" || x == "SOUTHEAST LA") {
		return "SOUTHEAST LOUISIANA";
	}
	if (x == "SOUTHEAST MISSOURI STATE" || x == "SE MISSOURI STATE" || x == "SE MO STATE" || x == "SOUTHEAST MO STATE" || x == "SE MISSOURI ST" || x == "SE MO ST" || x == "SOUTHEAST MO ST") {
		return "SOUTHEAST MISSOURI ST";
	}
	if (x == "S ILLINOIS") {
		return "SOUTHERN ILLINOIS";
	}
	if (x == "S ILLINOIS EDWARDSVILLE" || x == "S ILLINOIS-EDWARDSVILLE" || x == "SIU EDWARDSVILLE" || x == "SIU-EDWARDSVILLE" || x == "SOUTHERN ILLINOIS EDWARDSVILLE" || x == "SIUE") {
		return "SOUTHERN ILLINOIS-EDWARDSVILLE";
	}
	if (x == "SOUTHERN MISSISSIPPI") {
		return "SOUTHERN MISS";
	}
	if (x == "SOUTHERN" || x == "SOUTHERN U" || x == "SOUTHERN UNIV") {
		return "SOUTHERN UNIVERSITY";
	}
	if (x == "S UTAH") {
		return "SOUTHERN UTAH";
	}
	if (x == "SAINT BONAVENTURE") {
		return "ST BONAVENTURE";
	}
	if (x == "ST FRANCIS (NY)" || x == "ST FRANCIS" || x == "ST FRANCIS NY") {
		return "ST FRANCIS-NY";
	}
	if (x == "ST FRANCISA" || x == "SAINT FRANCIS (PA)" || x == "SAINT FRANCIS" || x == "ST FRANCIS PA" || x == "SAINT FRANCIS PA" || x == "SAINT FRANCIS-PA" || x == "SAINT FRANCIS-PA") {
		return "ST FRANCIS-PA";
	}
	if (x == "ST JOHN'S (NY)" || x == "ST JOHN'S" || x == "ST JOHN'S NY" || x == "SAINT JOHN'S (NY)" || x == "SAINT JOHN'S" || x == "SAINT JOHN'S NY" || x == "SAINT JOHN'S-NY") {
		return "ST JOHN'S-NY";
	}
	if (x == "ST JOSEPH'SA" || x == "ST JOSEPH'S (PA)" || x == "ST JOSEPH'S" || x == "ST JOSEPH'S PA" || x == "SAINT JOSEPH'S (PA)" || x == "SAINT JOSEPH'S" || x == "SAINT JOSEPH'S PA" || x == "SAINT JOSEPH'S-PA") {
		return "ST JOSEPH'S-PA";
	}
	if (x == "SAINT LOUIS") {
		return "ST LOUIS";
	}
	if (x == "SAINT MARY'S (CA)" || x == "SAINT MARY'S" || x == "SAINT MARY'S CA" || x == "SAINT MARY'S-CA" || x == "ST MARY'S (CA)" || x == "ST MARY'S" || x == "ST MARY'S CA") {
		return "ST MARY'S-CA";
	}
	if (x == "STETER'S" || x == "SAINT PETER'S") {
		return "ST PETERS";
	}
	if (x == "SF AUSTIN") {
		return "STEPHEN F AUSTIN";
	}
	if (x == "SUNY STONY BROOK" || x == "SUNY-STONY BROOK") {
		return "STONY BROOK";
	}
	if (x == "TENN") {
		return "TENNESSEE";
	}
	if (x == "TN MARTIN" || x == "TENNESSEE MARTIN" || x == "UT MARTIN" || x == "TN-MARTIN" || x == "UT-MARTIN") {
		return "TENNESSEE-MARTIN";
	}
	if (x == "TENN ST" || x == "TENN STATE" || x == "TENNESSEE STATE") {
		return "TENNESSEE ST";
	}
	if (x == "TENN TECH") {
		return "TENNESSEE TECH";
	}
	if (x == "TX" || x == "UT AUSTIN" || x == "UT-AUSTIN") {
		return "TEXAS";
	}
	if (x == "UT ARLINGTON" || x == "UT-ARLINGTON" || x == "TX ARLINGTON" || x == "TX-ARLINGTON" || x == "TEXAS ARLINGTON" || x == "UTA") {
		return "TEXAS-ARLINGTON";
	}
	if (x == "UT RIO GRANDE VALLEY" || x == "UT-RIO GRANDE VALLEY" || x == "TX RIO GRANDE VALLEY" || x == "TX-RIO GRANDE VALLEY" || x == "TEXAS RIO GRANDE VALLEY" || x == "UTRGV" || x == "TEXAS RGV" || x == "TEXAS-RGV") {
		return "TEXAS-RIO GRANDE VALLEY";
	}
	if (x == "TX A&M" || x == "A&M" || x == "TAMU" || x == "TEXAS AGRICULTURAL AND MECHANICAL") {
		return "TEXAS A&M";
	}
	if (x == "TAM C CHRISTI" || x == "TX A&M-CORPUS CHRISTI" || x == "TX A&M CORPUS CHRISTI" || x == "TX A&M-C CHRISTI" || x == "TX A&M C CHRISTI" || x == "TX A&M-CC" || x == "TX A&M CC" || x == "TX AM-CORPUS CHRISTI" || x == "TX AM CORPUS CHRISTI" || x == "TX AM-C CHRISTI" || x == "TX AM C CHRISTI" || x == "TX AM-CC" || x == "TX AM CC" || x == "TAM-CORPUS CHRISTI" || x == "TAM CORPUS CHRISTI" || x == "TAM-C CHRISTI" || x == "TAM C CHRISTI" || x == "TAM-CC" || x == "TAM CC" || x == "TEXAS AM-CORPUS CHRISTI" || x == "TEXAS AM CORPUS CHRISTI" || x == "TEXAS AM-C CHRISTI" || x == "TEXAS AM C CHRISTI" || x == "TEXAS AM-CC" || x == "TEXAS AM CC" || x == "TEXAS A&M CORPUS CHRISTI" || x == "TEXAS A&M-C CHRISTI" || x == "TEXAS A&M C CHRISTI" || x == "TEXAS A&M-CC" || x == "TEXAS A&M CC") {
		return "TEXAS A&M-CORPUS CHRISTI";
	}
	if (x == "TX SOUTHERN" || x == "TX SO" || x == "TEXAS SO") {
		return "TEXAS SOUTHERN";
	}
	if (x == "TEXAS CHRISTIAN" || x == "TX CHRISTIAN") {
		return "TCU";
	}
	if (x == "TROY ST" || x == "TROY STATE") {
		return "TROY";
	}
	if (x == "MA LOWELL" || x == "MA-LOWELL" || x == "MASSACHUSETTS LOWELL" || x == "MASSACHUSETTS-LOWELL" || x == "UMASS LOWELL") {
		return "UMASS-LOWELL";
	}
	if (x == "UTAH STATE") {
		return "UTAH ST";
	}
	if (x == "TEXAS-EL PASO" || x == "TEXAS EP" || x == "TEXAS EL PASO" || x == "TEXAS-EP" || x == "UT EL PASO" || x == "UT-EL PASO" || x == "TX EL PASO" || x == "TX-EL PASO" || x == "TX EP" || x == "TX-EP") {
		return "UTEP";
	}
	if (x == "TEXAS-SAN ANTONIO" || x == "TEXAS SA" || x == "TEXAS SAN ANTONIO" || x == "TEXAS-SA" || x == "UT SAN ANTONIO" || x == "UT-SAN ANTONIO" || x == "TX SAN ANTONIO" || x == "TX-SAN ANTONIO" || x == "TX SA" || x == "TX-SA") {
		return "UTSA";
	}
	if (x == "ALABAMA BIRMINGHAM" || x == "ALABAMA-BIRMINGHAM") {
		return "UAB";
	}
	if (x == "UC DAVIS" || x == "CALIFORNIA DAVIS" || x == "CALIFORNIA-DAVIS") {
		return "UC-DAVIS";
	}
	if (x == "UC IRVINE" || x == "CALIFORNIA IRVINE" || x == "CALIFORNIA-IRVINE") {
		return "UC-IRVINE";
	}
	if (x == "UC RIVERSIDE" || x == "CALIFORNIA RIVERSIDE" || x == "CALIFORNIA-RIVERSIDE") {
		return "UC-RIVERSIDE";
	}
	if (x == "UC SANTA BARBARA" || x == "CALIFORNIA SANTA BARBARA" || x == "CALIFORNIA-SANTA BARBARA") {
		return "UC-SANTA BARBARA";
	}
	if (x == "CENTRAL FLORIDA") {
		return "UCF";
	}
	if (x == "CALIFORNIA-LOS ANGELES" || x == "CALIFORNIA LOS ANGELES" || x == "UC-LOS ANGELES" || x == "CALIFORNIA-LA") {
		return "UCLA";
	}
	if (x == "CONNECTICUT" || x == "UCONNECTICUT" || x == "U CONNECTICUT" || x == "U CONN") {
		return "UCONN";
	}
	if (x == "MASSACHUSETTS" || x == "UMASSACHUSETTS" || x == "U MASSACHUSETTS") {
		return "UMASS";
	}
	if (x == "UM BALTIMORE COUNTY" || x == "UM-BALTIMORE COUNTY" || x == "UMD BALTIMORE COUNTY" || x == "UMD-BALTIMORE COUNTY" || x == "MD BALTIMORE COUNTY" || x == "MD-BALTIMORE COUNTY" || x == "MARYLAND-BC" || x == "MARYLAND BC" || x == "MARYLAND BALTIMORE COUNTY" || x == "MARYLAND-BALTIMORE COUNTY") {
		return "UMBC";
	}
	if (x == "UNC ASHEVILLE" || x == "N CAROLINA ASHEVILLE" || x == "N CAROLINA-ASHEVILLE" || x == "NORTH CAROLINA ASHEVILLE" || x == "NORTH CAROLINA-ASHEVILLE") {
		return "UNC-ASHEVILLE";
	}
	if (x == "UNC GREENSBORO" || x == "N CAROLINA GREENSBORO" || x == "N CAROLINA-GREENSBORO" || x == "NORTH CAROLINA GREENSBORO" || x == "NORTH CAROLINA-GREENSBORO") {
		return "UNC-GREENSBORO";
	}
	if (x == "UNC WILMINGTON" || x == "N CAROLINA WILMINGTON" || x == "N CAROLINA-WILMINGTON" || x == "NORTH CAROLINA WILMINGTON" || x == "NORTH CAROLINA-WILMINGTON") {
		return "UNC-WILMINGTON";
	}
	if (x == "NEVADA-LAS VEGAS" || x == "NEVADA LAS VEGAS") {
		return "UNLV";
	}
	if (x == "SOUTHERN CALIFORNIA") {
		return "USC";
	}
	if (x == "VANDY") {
		return "VANDERBILT";
	}
	if (x == "UVM") {
		return "VERMONT";
	}
	if (x == "NOVA" || x == "'NOVA") {
		return "VILLANOVA";
	}
	if (x == "UVA") {
		return "VIRGINIA";
	}
	if (x == "VA COMMONWEALTH" || x == "VA-COMMONWEALTH" || x == "VIRGINIA-COMMONWEALTH") {
		return "VIRGINIA COMMONWEALTH";
	}
	if (x == "VMI") {
		return "VIRGINIA MILLITARY INSTITUTE";
	}
	if (x == "VA TECH") {
		return "VIRGINIA TECH";
	}
	if (x == "WAKE" || x == "WF") {
		return "WAKE FOREST";
	}
	if (x == "WASH" || x == "U WASHINGTON") {
		return "WASHINGTON";
	}
	if (x == "WAZZU" || x == "WASHINGTON STATE" || x == "WASH STATE" || x == "WASH ST") {
		return "WASHINGTON ST";
	}
	if (x == "WEBER STATE") {
		return "WEBER ST";
	}
	if (x == "W CAROLINA") {
		return "WEST CAROLINA";
	}
	if (x == "WVU" || x == "WV" || x == "W VIRGINIA") {
		return "WEST VIRGINIA";
	}
	if (x == "W ILLINOIS" || x == "WIU") {
		return "WESTERN ILLINOIS";
	}
	if (x == "WKU" || x == "W KENTUCKY" || x == "W KY" || x == "WESTERN KY") {
		return "WESTERN KENTUCKY";
	}
	if (x == "W MICHIGAN" || x == "WMU") {
		return "WESTERN MICHIGAN";
	}
	if (x == "WICHITA STATE") {
		return "WICHITA ST";
	}
	if (x == "WILLIAM AND MARY") {
		return "WILLIAM & MARY";
	}
	if (x == "UW" || x == "WISCO" || x == "WISCY") {
		return "WISCONSIN";
	}
	if (x == "UW GREEN BAY" || x == "UW-GREEN BAY" || x == "UW-GB" || x == "UW GB" || x == "UWGB" || x == "WISCONSIN GREEN BAY") {
		return "WISCONSIN-GREEN BAY";
	}
	if (x == "UW MILWAUKEE" || x == "UW-MILWAUKEE" || x == "UW-M" || x == "UW M" || x == "UWM" || x == "WISCONSIN MILWAUKEE") {
		return "WISCONSIN-MILWAUKEE";
	}
	if (x == "WRIGHT STATE") {
		return "WRIGHT ST";
	}
	if (x == "YOUNGSTOWN STATE") {
		return "YOUNGSTOWN ST";
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
string>& teams, int& numTeams, string& inputIn, double startPoint, int iteration, int batchSize) {
	string x;
	string match;
	string winner;
	string loser;
	int w;
	int l;
	stringstream input(inputIn);
	stringstream inputcopy(inputIn);
	stringstream matchstream;

	h2hPM = vector<vector<double>>();
	games = vector<vector<double>>();
	ties = vector<vector<int>>();
	gamesPlayed = vector<int>();
	teamsPlayed = vector<int>();
	h2hPM.resize(numTeamsTotal);
	games.resize(numTeamsTotal);
	ties.resize(numTeamsTotal);
	gamesPlayed.resize(numTeamsTotal);
	teamsPlayed.resize(numTeamsTotal);
	for (int i = 0; i < numTeamsTotal; ++i) {
		h2hPM[i].resize(numTeamsTotal);
		games[i].resize(numTeamsTotal);
		ties[i].resize(numTeamsTotal);
	}

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
		ties.resize(numTeamsTotal);
		gamesPlayed.resize(numTeamsTotal);
		teamsPlayed.resize(numTeamsTotal);
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



	int gameline = (int) (startPoint * numTeams / 2.0) + iteration * batchSize;

	for (int i = 0; i < gameline; ++i) {

		getline(input, match, '\n');
		bool tie = false;

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
	double ntrAAdj = (rt2(ntr[a] * 2.0 - 1.0) / 4.0 + 0.75);
	//double revNtrAAdj = 1.5 - ntrAAdj;

	double ntrBAdj = (rt2(ntr[b] * 2.0 - 1.0) / 4.0 + 0.75);
	double revNtrBAdj = 1.5 - ntrBAdj;

	double proximityImp = 1.0 - (abs((ntr[a]) - (ntr[b]))) / 2.0;
	//double proximityImpAdj = 1.0 - (abs(((ntrAAdj) - (ntrBAdj)))) / 2.0;
	double rev2multImp = 1.0 - (1.0 - ntr[a]) * (1.0 - ntr[b]);
	//double rev2multImpAdj = 1.0 - (1.0 - ntrAAdj) * (1.0 - ntrBAdj);
	//double multImp = ntr[a] * ntr[b];
	double multImpAdj = ntrAAdj * ntrBAdj;
	double importance = sqrt(pow2(proximityImp) * (rev2multImp) * (multImpAdj) * ntrBAdj * 2.0 + (1.0 + 1.0 * ntrAAdj + 1.0 * ntrBAdj) / (1.0 * (double) (numTeamsTotal)));

	double oppQAdj = ((h2hPM[a][b] >= 0.0 ? 2.0 : 0.0) * importance * (ntrBAdj)) / (sqrt(numTeamsTotal) * teamsPlayed[a] + 1.0);
	double negOppQAdj = -(2.0 * importance * (ntrBAdj)) / (sqrt(numTeamsTotal) * (numTeamsTotal - teamsPlayed[a]) + 1.0);

	double nonPlayAdj = 1.0;
	double playAdj = 1.0;

	//constants -0.49,+0.02,+0.25,-0.02 work, are there better constants?
	double nonPlayPenalty = (ntr[a] >= ntr[b] ? revNtrBAdj - 0.49 : ntrBAdj + 0.02);
	double competitionBonus = (h2hPM[a][b] >= 0 ? ntrBAdj + 0.25 : revNtrBAdj - 0.02);

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
