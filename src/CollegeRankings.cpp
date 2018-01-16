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

int numTeams;

void getInput(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<string>& teams, int numTeams);
void getInput2(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<string>& teams, int numTeams);
int findTeam(vector<string>& teams, string& team);
string toUpper(string& x);
void calcFinalRatings(vector<vector<double> >& h2hPredict, vector<double>& ratings);
void printRatings(vector<string>& teams, vector<double>& ratings);
void printRatingsInfo(vector<string>& teams, vector<double>& ratings, vector<vector<double>>& games, vector<vector<double>>& h2hPM);
vector<vector<double> > calcNewRatingGrid(vector<vector<double> >& games, vector<vector<double>>& h2h);
vector<vector<double> > calcNewRatingGrid2(vector<vector<double> >& games, vector<vector<double>>& h2h);
vector<vector<double> > calcNewRatingGrid3(vector<vector<double>>& games, vector<vector<double>>& origH2HPM, vector<vector<double> >& h2h, vector<int>& gamesPlayed);
vector<vector<double> > calcNewRatingGrid4(vector<vector<double>>& games, vector<vector<double> >& h2h, vector<vector<double>>& h2hPM, vector<int>& teamsPlayed);
double predictH2Hsquare(vector<vector<double>>& originalH2HGames, vector<vector<double>>& prevH2H, int a, int b);
double predictH2Hsquare2(vector<vector<double>>& originalH2HGames, vector<vector<double>>& prevH2H, vector<double>& tempRatings, vector<double>& tempAbsRatings, int a, int b);
double predictH2Hsquare3(vector<vector<double>>& games, vector<vector<double>>& rawH2HPM, vector<vector<double>>& prevH2H, vector<double>& tempRatings, vector<
double>& tempAbsRatings, vector<double>& ntr, vector<int>& gamesPlayed, int a, int b);
double predictH2Hsquare4(vector<vector<double>>& games, vector<vector<double>>& prevH2H, vector<vector<double>>& h2hPM, vector<double>& tempRatings, vector<
double>& tempAbsRatings, vector<
double>& ntr, vector<
int>& teamsPlayed, int a, int b);
void printGrid(vector<vector<double>>& grid);
void printGrid2(vector<vector<double>>& grid, vector<string>& teams);
double rt2(double x);
double rt3(double x);
double rt4(double x);
double rtX(double x, double denom);
double powX(double x, double exponent);
double pow2(double x);
double rootGamesPlus2(double x, double games);
vector<double> normalize(vector<double>& x);
vector<double> normalizeRT2(vector<double>& x);
vector<double> normalizePow(vector<double>& x, double exponent);

struct Team {
		string name;
		double rating;
		int wins;
		int losses;
		int rank = 0;

		Team(string& name, double rating, int wins, int losses) {
			this->name = name;
			this->rating = rating;
			this->wins = wins;
			this->losses = losses;
		}
};

struct comp {
		bool operator()(Team& a, Team& b) {
			return a.rating >= b.rating;
		}
} comp;

int main() {
	string temp;
	cin >> numTeams;
	cout << setprecision(4) << fixed;
	getline(cin, temp);

	vector<string> teams;
	vector<vector<double> > h2hPM;
	vector<vector<double> > h2h;
	vector<vector<double> > h2hPredict;
	vector<vector<double> > games;
	vector<double> ratings;
	vector<int> gamesPlayed;
	vector<int> teamsPlayed;

	h2h.resize(numTeams);
	h2hPM.resize(numTeams);
	h2hPredict.resize(numTeams);
	games.resize(numTeams);
	gamesPlayed.resize(numTeams);
	teamsPlayed.resize(numTeams);
	for (int i = 0; i < numTeams; ++i) {
		h2h[i].resize(numTeams);
		h2hPM[i].resize(numTeams);
		h2hPredict[i].resize(numTeams);
		games[i].resize(numTeams);
	}

	ratings.resize(numTeams);

	getInput2(h2hPM, games, gamesPlayed, teamsPlayed, teams, numTeams);

	for (int i = 0; i < numTeams; ++i) {
		for (int j = 0; j < numTeams; ++j) {
			if (games[i][j] != 0) {
				h2hPM[i][j] = h2hPM[i][j] / (games[i][j]);
				//h2h[i][j] = rt2(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)));
				if (games[i][j] > 1.5) {
					h2h[i][j] = rtX(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)), (sqrt(games[i][j]) * abs(h2hPM[i][j]) + 1));
				}
				else {
					h2h[i][j] = rt2(((h2hPM[i][j] * games[i][j]) / (games[i][j] + 1)));
				}
			}
		}
	}


	h2hPredict = calcNewRatingGrid4(games, h2h, h2hPM, teamsPlayed);
	//h2hPredict = calcNewRatingGrid3(games, h2hPM, h2h, gamesPlayed);
	for (int i = 0; i < (int) sqrt(numTeams) + 1000; ++i) {
		h2hPredict = calcNewRatingGrid4(games, h2hPredict, h2hPM, teamsPlayed);
		//h2hPredict = calcNewRatingGrid3(games, h2hPM, h2hPredict, gamesPlayed);
	}

	calcFinalRatings(h2hPredict, ratings);
	//printGrid(h2hPredict);
	//printGrid2(h2hPredict, teams);
	//cout << endl;
	printRatingsInfo(teams, ratings, games, h2hPM);
	return 0;
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

	if (x == "CENTRAL FLORIDA") {
		return "UCF";
	}
	if (x == "MIAMI (FL)" || x == "MIAMI-FL" || x == "MIAMI") {
		return "MIAMI FL";
	}
	if (x == "MIAMI (OH)" || x == "MIAMI-OH") {
		return "MIAMI OH";
	}
	if (x == "BOWLING GREEN STATE") {
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
	if (x == "CAL") {
		return "CALIFORNIA";
	}
	if (x == "LOUISIANA STATE") {
		return "LSU";
	}
	if (x == "MISSISSIPPI") {
		return "OLE MISS";
	}
	if (x == "NEVADA-LAS VEGAS") {
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
	if (x == "PENNSYLVANIA STATE") {
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
		return "PENN STATE";
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
	if (x == "ALBANY (NY)" || x == "ALBANY") {
		return "ALBANY NY";
	}
	if (x == "LOYOLA (IL)" || x == "LOYOLA IL") {
		return "PENN";
	}
	if (x == "LOYOLA (MD)" || x == "LOYOLA MD") {
		return "PENN";
	}
	if (x == "ST. FRANCIS (NY)" || x == "ST. FRANCIS") {
		return "ST. FRANCIS NY";
	}
	if (x == "SAINT FRANCIS (PA)" || x == "SAINT FRANCIS") {
		return "SAINT FRANCIS PA";
	}
	if (x == "SAINT MARY'S (CA)" || x == "SAINT MARY'S") {
		return "SAINT MARY'S CA";
	}
	if (x == "ST. JOHN'S (NY)" || x == "ST. JOHN'S") {
		return "ST. JOHN'S NY";
	}
	return x;
}

void getInput(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<string>& teams, int numTeams) {
	string x;
	string wins;
	stringstream winstream;

	for (int i = 0; i < numTeams; ++i) {
		getline(cin, x, '\n');
		x = toUpper(x);
		teams.push_back(x);
	}
	getline(cin, x, '\n');
	for (int i = 0; i < numTeams; ++i) {
		getline(cin, wins, '\n');
		wins = toUpper(wins);
		winstream = stringstream(wins);
		//cout << "-" << i << "-" << endl;
		//cout << " # " << winstream.str() << " # " << endl;

		while (getline(winstream, x, ',')) {
			int j = findTeam(teams, x);
			h2hPM[i][j] += 1;
			h2hPM[j][i] -= 1;
			games[i][j] += 1;
			games[j][i] += 1;
		}
	}
}

void getInput2(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<string>& teams, int numTeams) {
	string x;
	string match;
	string winner;
	string loser;
	int w;
	int l;
	int numGames;
	stringstream matchstream;

	for (int i = 0; i < numTeams; ++i) {
		getline(cin, x, '\n');
		x = toUpper(x);
		teams.push_back(x);
	}
	cin >> numGames;
	getline(cin, x, '\n');
	for (int i = 0; i < numGames; ++i) {
		getline(cin, match, '\n');
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
		h2hPM[w][l] += 1;
		h2hPM[l][w] -= 1;
		games[w][l] += 1;
		games[l][w] += 1;
		gamesPlayed[w] += 1;
		gamesPlayed[l] += 1;
	}
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

vector<vector<double> > calcNewRatingGrid(vector<vector<double>>& games, vector<vector<double> >& h2h) {
	vector<vector<double> > newGrid;
	double prediction = 0;
	newGrid.resize(h2h.size());
	for (uint64_t i = 0; i < newGrid.size(); ++i) {
		newGrid[i].resize(h2h[i].size());
	}
	for (uint64_t i = 0; i < newGrid.size() - 1; ++i) {
		for (uint64_t j = i + 1; j < newGrid.size(); ++j) {
			prediction = predictH2Hsquare(games, h2h, i, j);
			newGrid[i][j] = prediction;
			newGrid[j][i] = -prediction;
		}
	}
	return newGrid;
}

vector<vector<double> > calcNewRatingGrid2(vector<vector<double>>& games, vector<vector<double> >& h2h) {
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
	for (uint64_t i = 0; i < newGrid.size() - 1; ++i) {
		for (uint64_t j = i + 1; j < newGrid.size(); ++j) {
			prediction = predictH2Hsquare2(games, h2h, tempRatings, tempAbsRatings, i, j);
			newGrid[i][j] = prediction;
			newGrid[j][i] = -prediction;
		}
	}
	return newGrid;
}

vector<vector<double> > calcNewRatingGrid3(vector<vector<double>>& games, vector<vector<double>>& origH2HPM, vector<vector<double> >& h2h, vector<int>& gamesPlayed) {
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
			prediction = predictH2Hsquare3(games, origH2HPM, h2h, tempRatings, tempAbsRatings, ntr, gamesPlayed, i, j);
			newGrid[i][j] = prediction;
		}
	}
	return newGrid;
}

vector<vector<double> > calcNewRatingGrid4(vector<vector<double>>& games, vector<vector<double> >& h2h, vector<vector<double>>& h2hPM, vector<int>& teamsPlayed) {
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
			prediction = predictH2Hsquare4(games, h2h, h2hPM, tempRatings, tempAbsRatings, ntr, teamsPlayed, i, j);
			newGrid[i][j] = prediction;
			//newGrid[j][i] = -prediction;
		}
	}
	return newGrid;
}

void printRatings(vector<string>& teams, vector<double>& ratings) {
	for (uint64_t i = 0; i < teams.size(); ++i) {
		cout << teams[i] << "," << ratings[i] << endl;
	}
}

void printRatingsInfo(vector<string>& teams, vector<double>& ratings, vector<vector<double>>& games, vector<vector<double>>& h2hPM) {
	vector<Team> teamRatings;
	teamRatings.reserve(ratings.size());
	ratings = normalize(ratings);

	cout << "RANK,TEAM,RATING,WINS,LOSSES" << endl;
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
		teamRatings.push_back(Team(teams[i], ratings[i], wins, losses));
	}
	sort(teamRatings.begin(), teamRatings.end(), comp);
	for (uint64_t i = 0; i < teamRatings.size(); ++i) {
		teamRatings[i].rank = i + 1;
		cout << teamRatings[i].rank << "," << teamRatings[i].name << "," << teamRatings[i].rating << "," << teamRatings[i].wins << "," << teamRatings[i].losses << endl;
	}
}

void printGrid(vector<vector<double>>& grid) {
	for (uint64_t i = 0; i < grid.size(); ++i) {
		for (uint64_t j = 0; j < grid[i].size(); ++j) {
			cout << grid[i][j] << "\t\t";
		}
		cout << endl;
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

double predictH2Hsquare(vector<vector<double>>& originalH2HGames, vector<vector<double>>& prevH2H, int a, int b) {
	if (a == b) {
		return 0;
	}
	if (originalH2HGames[a][b] != 0) {
		return prevH2H[a][b];
	}
	double prediction = 0.0;
	double numnum = 0.0;
	double numdenom = 0.0;
	double denom = 0.0;
	for (uint64_t i = 0; i < prevH2H[a].size(); ++i) {
		numnum += ((prevH2H[a][i] - prevH2H[b][i]) / 2.0);
		numdenom += sqrt(abs(prevH2H[a][i] * prevH2H[b][i]));
		//numdenom += abs(prevH2H[a][i] * prevH2H[b][i]);
		denom += (abs(prevH2H[a][i]) + abs(prevH2H[b][i])) / 2.0;
	}
	if (denom == 0 || numdenom == 0) {
		return 0;
	}
	prediction = (numnum / numdenom) / denom;
	prediction = -1 * (2 * signbit(prediction) - 1) * sqrt(abs(prediction) / 2.0);
	return prediction;
}

double predictH2Hsquare2(vector<vector<double>>& originalH2HGames, vector<vector<double>>& prevH2H, vector<double>& tempRatings, vector<double>& tempAbsRatings, int a, int b) {
	if (a == b) {
		return 0;
	}
	if (originalH2HGames[a][b] != 0) {
		return prevH2H[a][b];
	}
	double prediction = rt2(((tempRatings[a]) - (tempRatings[b])) / (2 * ((tempAbsRatings[a]) + (tempAbsRatings[b]))));
	return prediction;
}

double predictH2Hsquare3(vector<vector<double>>& games, vector<vector<double>>& rawH2HPM, vector<vector<double>>& prevH2H, vector<double>& tempRatings, vector<
double>& tempAbsRatings, vector<double>& ntr, vector<int>& gamesPlayed, int a, int b) {
	if (a == b) {
		return 0;
	}
	if (games[a][b] != 0) {
		double mab = rawH2HPM[a][b] / 2.0 - 0.5;
		double rawab = rawH2HPM[a][b] * games[a][b];
		double sqPower = (mab + ntr[b] + rawab) / (abs(mab + ntr[b]) + abs(rawab) + 1);
		/*if (a == 112 && b == 57) {
		 cout << rawH2HPM[a][b] << "\t" << games[a][b] << "\t" << mab << "\t" << rawab << "\t" << ntr[b] << "\t" << sqPredict << "\t" << rt2(sqPredict) << endl;
		 }*/
		//return rtX(sqPower, games[a][b] + 2 * pow(ntr[b], 2) + 2 * (abs(sqPower)) - 1);
		//return rtX(sqPower, games[a][b] + 2 * pow(ntr[b], 2) + 2 * pow((sqPower), 2) - 0.5);
		return rtX(sqPower, games[a][b] + 2 * pow(ntr[b], 2) + 2 * sqrt(abs(sqPower)) - 2 * sqrt(0.5));
	}
	//double sqPrediction = (ntr[a] - ntr[b]) / 2;
	double sqPrediction = ((tempRatings[a]) - (tempRatings[b])) / (2 * ((tempAbsRatings[a]) + (tempAbsRatings[b])));
	//double prediction = rtX((sqPrediction), (2 + 2 * pow(ntr[b], 2) + 2 * sqrt(abs(sqPrediction)) - 2 * sqrt(0.5)));
	return rt2(sqPrediction);
	//return prediction;
}

double predictH2Hsquare4(vector<vector<double>>& games, vector<vector<double>>& prevH2H, vector<vector<double>>& h2hPM, vector<double>& tempRatings, vector<
double>& tempAbsRatings, vector<double>& ntr, vector<int>& teamsPlayed, int a, int b) {
	if (a == b) {
		return 0;
	}
	double gameFactor = sqrt((((double) numTeams - (double) teamsPlayed[a] + 1) / ((double) numTeams + 1)));
	double importance = ((ntr[a] * ntr[b] + 1.0) / 2.0);
	if (games[a][b] != 0) {
		if (games[a][b] > 1.5) {
			return rtX((importance * (h2hPM[a][b] * games[a][b]) / (games[a][b] + 1)), (sqrt(games[a][b]) * abs(h2hPM[a][b]) + 1));
		}
		else {
			return rt2((importance * (h2hPM[a][b] * games[a][b]) / (games[a][b] + 1)));
		}
		return prevH2H[a][b];
	}
	double sqPrediction = gameFactor * importance * (((tempRatings[a]) - (tempRatings[b])) / (2 * ((tempAbsRatings[a]) + (tempAbsRatings[b]))));
	double prediction = rtX(sqPrediction, 2);
	return prediction;
}

double rt2(double x) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), 1.0 / 2.0);
}

double rt3(double x) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), 1.0 / 3.0);
}

double rt4(double x) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), 1.0 / 4.0);
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

double rootGamesPlus2(double x, double games) {
	return -1 * (2 * signbit(x) - 1) * pow(abs(x), 1.0 / (2.0 + games));
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
