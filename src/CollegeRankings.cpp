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
int sumTeamsPlayed;
double avgTeamsPlayed;

void getInput3(vector<vector<double>>& h2hPM, vector<vector<double>>& games, vector<vector<int>>& ties, vector<int>& gamesPlayed, vector<int>& teamsPlayed, vector<
string>& teams, int& numTeams);
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
	ios_base::sync_with_stdio(false);
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

	getInput3(h2hPM, games, ties, gamesPlayed, teamsPlayed, teams, numTeams);

	sumTeamsPlayed = sum(teamsPlayed);
	avgTeamsPlayed = avg(teamsPlayed);

	h2h.resize(numTeams);
	h2hPredict.resize(numTeams);
	ratings.resize(numTeams);
	for (int i = 0; i < numTeams; ++i) {
		h2h[i].resize(numTeams);
		h2hPredict[i].resize(numTeams);
	}

	for (int i = 0; i < numTeams; ++i) {
		for (int j = 0; j < numTeams; ++j) {
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
	for (int i = 0; i < (int) sqrt(numTeams) + numTeams + 100; ++i) {
		h2hPredict = calcNewRatingGrid5(games, h2hPredict, h2hPM, teamsPlayed);
	}

	calcFinalRatings(h2hPredict, ratings);
	//printGrid2(h2hPredict, teams);
	printRatingsInfo(teams, ratings, games, h2hPM, ties);
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

	if (x == "LOUISIANA" || x == "LOUISIANA-LAFAYETTE" || x == "LOUISIANA LAFAYETTE" || x == "ULL") {
		return "LOUISIANA-LAFAYETTE";
	}
	if (x == "LOUISIANA-MONROE" || x == "LOUISIANA MONROE" || x == "ULM") {
		return "LOUISIANA-MONROE";
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
	if (x == "PENNSYLVANIA STATE" || x == "PENN ST") {
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
string>& teams, int& numTeams) {
	string x;
	string match;
	string winner;
	string loser;
	int w;
	int l;
	stringstream matchstream;
	stringstream input;
	stringstream inputcopy;

	x = "$";
	do {
		getline(cin, x, '\n');
		input << x << "\n";
		inputcopy << x << "\n";
	} while (x != "=");


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
			cout << teamRatings[i].rank << "," << teamRatings[i].name << "," << teamRatings[i].rating << "," << teamRatings[i].rawScore << "," << teamRatings[i].rawScore / ((double) (numTeams - 1)) << "," << teamRatings[i].wins << "," << teamRatings[i].losses << endl;
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
			cout << teamRatings[i].rank << "," << teamRatings[i].name << "," << teamRatings[i].rating << "," << teamRatings[i].rawScore << "," << teamRatings[i].rawScore / ((double) (numTeams - 1)) << "," << teamRatings[i].wins << "," << teamRatings[i].losses << "," << teamRatings[i].ties << endl;
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
	double importance = sqrt((ntr[a] * ntr[b] + 1.0) / 2.0); //sqrt(sqrt((1 - (abs(ntr[a] - ntr[b]))) + ntr[b] * ntr[a]) / (1 + ntr[b] * ntr[a]));

	double ntrAAdj = rt2(ntr[a] * 2.0 - 1.0) / 4.0 + 0.75;
	double revNtrAAdj = 1.5 - ntrAAdj;

	double ntrBAdj = rt2(ntr[b] * 2.0 - 1.0) / 4.0 + 0.75;
	double revNtrBAdj = 1.5 - ntrBAdj;

	double oppQAdj = ntrBAdj / (sqrt(numTeams) * teamsPlayed[a]); //(rt2((ntr[b] - 0.5) / 2.0) + 0.5) / (sqrt(numTeams) * teamsPlayed[a]);
	double negOppQAdj = -1 * ntrBAdj * teamsPlayed[a] / (sqrt(numTeams) * pow2(avgTeamsPlayed)); //-1 * sqrt(ntr[b] / 2.0) * teamsPlayed[a] / (sqrt(numTeams) * avgTeamsPlayed);

	double nonPlayAdj = sqrt(sqrt(0.5)); //.7071*1 vs .7071*.7071 || 1*1*.7071 vs. 1*.7071
	double playAdj = 1; //sqrt(1.0 + sqrt(teamsPlayed[a] / numTeams));

	double nonPlayPenalty = (ntr[a] >= ntr[b] ? sqrt((ntr[a] - ntr[b]) / 2.0) : ntrBAdj) * revNtrAAdj;
	double competitionBonus = (h2hPM[a][b] >= 0 ? ntrBAdj : revNtrBAdj) * ntrAAdj;

	if (games[a][b] != 0) {
		double gameScore = playAdj * (competitionBonus * importance * (h2hPM[a][b] * games[a][b]) / (games[a][b] + 1)) + oppQAdj;
		if (games[a][b] > 1.0) {
			return rtX(gameScore, (sqrt(games[a][b]) * abs(h2hPM[a][b]) + 1));
		}
		else {
			return rt2(gameScore);
		}
	}
	double sqPrediction = nonPlayAdj * nonPlayPenalty * importance * ((((tempRatings[a]) - (tempRatings[b])) / ((tempAbsRatings[a]) + (tempAbsRatings[b]))) / 2.0) + negOppQAdj;
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
