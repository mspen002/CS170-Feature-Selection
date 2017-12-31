#include <ctime>
#include <bits/stdc++.h>
#include <functional>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

//Arithmetic Eq's
double sum(vector <float> v)
{
	double sum = 0;
	for(int i = 0; i < v.size(); i++)
		sum+=v.at(i);
	return sum;
}

double sumSquared(vector <float> v)
{
	double sum = 0;
	for(int i = 0; i < v.size(); i++)
		sum+=(v.at(i) * v.at(i));
	return sum;
}

double mean(vector <float> v)
{
	return (sum(v) / v.size());
}

double standardDev(vector <float> v)
{
	return (sumSquared(v) - ((sum(v)*sum(v))) / v.size()) / (v.size() - 1);
}

//Normalization
void zNorm(vector< vector<float> > &d) 
{
	vector <float> temp;
	for(int i = 1; i < d.at(0).size(); i++)
	{
		temp.clear();
		for(int j = 0; j < d.size(); j++)
		{
			temp.push_back(d.at(j).at(i));
		}
		double m = mean(temp);
		double s = standardDev(temp);
		if(s != 0)
		{
			for(int j = 0; j < d.size(); j++)
				d.at(j).at(i) = (d.at(j).at(i) - m) / s;
		}
	}
}

//Not my function
float euclidean_distance(vector <float> a, vector <float> b, vector <int> f, int j, bool forwards) 
{
	double x = 0;
	for (int i = 0; i < f.size(); i++) 
		x += (pow(a.at(f.at(i)) - b.at(f.at(i)), 2));
	if (forwards)
		x += (pow(a.at(j) - b.at(j), 2));
	return sqrt(x);
}

//Used online resources
float kCross(vector < vector <float> > data, vector <int> f, int j, bool forwards) 
{
	float c = 0;
		
	for (int i = 0; i < data.size(); i++) {	
		vector <float> test_data = data.at(i); 
		float min_dist = 9999;
		vector <float> min_neighbor;

		for (int k = 0; k < data.size(); k++) { 
			if(k != i) { 
				float distance = euclidean_distance(test_data, data.at(k), f, j, forwards);
				if (distance < min_dist) { 
					min_dist = distance;
					min_neighbor = data.at(k);
				}
			}	
		}	
		if (min_neighbor.at(0) == 1 && test_data.at(0) == 1) 
			c++;
		else if (min_neighbor.at(0) == 2 && test_data.at(0) == 2)
			c++;
	}
	return c / (float)data.size();
}

bool check(vector <int> cur, int check)
{
	for(int i = 0; i < cur.size(); i++)
		if(cur.at(i) == check)
			return true;
	return false;
}


float forwardSearch(vector< vector<float> > d)
{
	vector <int> feats;
	float acc = 0;
	vector <int> bestfeats;
	for (int i = 1; i < d.at(0).size(); i++) 
	{
		int curfeat;
		float max = 0;
		for (int j = 1; j < d.at(0).size(); j++) 
		{ 
			if (!check(feats, j)) 
			{
				cout << "Feats: {";
				for (int p = 0; p < feats.size(); p++) 
					cout << feats.at(p) << ",";
				cout << j << "} acc: ";

				float accuracy = kCross(d, feats, j, true);
				cout << accuracy << endl;
				if (accuracy > max) 
				{
					max = accuracy;
					curfeat = j;
				}
			}
		}
		feats.push_back(curfeat);
		if (max > acc) {
			acc = max;
			bestfeats = feats;
		}
		else
			cout << "Accuracy decreased, reverting to old max." << endl;

		cout << "Feats {";
		for (int p = 0; p < feats.size() - 1; p++)
			cout << feats.at(p) << ",";
		cout << feats.at(feats.size() - 1);
		cout <<"} best set, acc: ";
		cout << max * 100 << "%" << endl;
	}
	cout << endl << "Best feat set: {";
	for (int p = 0; p < bestfeats.size() - 1; p++)
		cout << bestfeats.at(p) << ",";
	cout << bestfeats.at(bestfeats.size() - 1);
	cout << "}, acc: " << acc * 100 << "%" << endl;
	return acc*100;
}

vector <int> removefeat(vector <int> feat, int index)
{
	for(int i = 0; i < feat.size(); i++)
	{
		if(feat.at(i) == index) 
		{
			feat.erase(feat.begin() + index);
			return feat;
		}
	}
	return feat;
}

float backwardSearch(vector< vector<float> > d)
{
	vector <int> feats;
	float acc = 0;
	vector <int> bestfeats;
	for(int i = 1; i < d.at(0).size(); i++)
		feats.push_back(i);
	for (int i = 1; i < d.at(0).size() - 1; i++) 
	{
		int curfeat;
		float max = 0;
		for (int j = 1; j < feats.size(); j++) 
		{ 
			if (check(feats, j)) 
			{
				vector <int> temp = removefeat(feats,j);
				cout << "Feats: {";
				for (int p = 0; p < temp.size() - 1; p++) 
					cout << temp.at(p) << ",";
				cout << temp.at(temp.size() - 1) << "} acc: ";

				float accuracy = kCross(d, temp, j, false);
				cout << accuracy << endl;
				if (accuracy > max) 
				{
					max = accuracy;
					curfeat = j;
				}
			}
		}
		feats = removefeat(feats, curfeat);
		if (max > acc) {
			acc = max;
			bestfeats = feats;
		}
		else
			cout << "Accuracy decreased, reverting to old max." << endl;

		cout << "Feats {";
		for (int p = 0; p < feats.size() - 1; p++)
			cout << feats.at(p) << ",";
		cout << feats.at(feats.size() - 1);
		cout <<"} best set, acc: ";
		cout << max * 100 << "%" << endl;
	}
	cout << endl << "Best feat set: {";
	for (int p = 0; p < bestfeats.size() - 1; p++)
		cout << bestfeats.at(p) << ",";
	cout << bestfeats.at(bestfeats.size() - 1);
	cout << "}, acc: " << acc * 100 << "%" << endl;
	return acc*100;
}

float markComperator(vector< vector<float> > d)
{
	cout << "Giving you our best estimate for features for a limited depth!" << endl;
	
	
	vector <int> feats;
	float acc = 0;
	int lowstreak = 0;
	vector <int> bestfeats;
	int numfeats = 3;
	if(d.at(0).size() < 3)
		numfeats = d.at(0).size();
	else
		numfeats = 3;
	for (int i = 1; i < numfeats; i++) 
	{
		int curfeat;
		float max = 0;
		for (int j = 1; j < d.at(0).size(); j++) 
		{ 
			if (!check(feats, j)) 
			{
				cout << "Feats: {";
				for (int p = 0; p < feats.size(); p++) 
					cout << feats.at(p) << ",";
				cout << j << "} acc: ";

				float accuracy = kCross(d, feats, j, true);
				cout << accuracy << endl;
				if (accuracy > max) 
				{
					max = accuracy;
					curfeat = j;
				}
			}
		}

		feats.push_back(curfeat);
		if (max > acc) {
			acc = max;
			bestfeats = feats;
			lowstreak = 0;
		}
		else
		{
			cout << "Accuracy decreased, reverting to old max." << endl;
			lowstreak++;
		}
		
		if(lowstreak == 0 && i > 1)
		{
			if(numfeats < d.at(0).size())
				numfeats++;
		}
		

		cout << "Feats {";
		for (int p = 0; p < feats.size() - 1; p++)
			cout << feats.at(p) << ",";
		cout << feats.at(feats.size() - 1);
		cout <<"} best set, acc: ";
		cout << max * 100 << "%" << endl;
	}
	cout << endl << "Best feat set: {";
	for (int p = 0; p < bestfeats.size() - 1; p++)
		cout << bestfeats.at(p) << ",";
	cout << bestfeats.at(bestfeats.size() - 1);
	cout << "}, acc: " << acc * 100 << "%" << endl;
	return acc*100;
}

int main()
{
	cout << "Welcome to Mark Spencer's Feature Selector! Please input the filename to be tested: " << endl;
	
	string f;
	cin >> f;
	
	vector< vector<float> > d;
	string read;
	ifstream file(f.c_str());
	
	if(file.is_open())
	{
		while(getline(file, read))
		{
			stringstream line(read);
			float v;
			vector<float> curline;
			while(line >> v)
				curline.push_back(v);
			d.push_back(curline);
			//cout << "reading... ";
		}
		file.close();
	}
	else
	{
		cout << "Could not open file, please double check the name of the input!" << endl;
		return 0;
	}
	
	cout << "Please wait while I normalize your data!" << endl;
	zNorm(d);
	cout << "Normalized!" << endl << endl;
	int a;
	cout << endl << "Please now select an algorithm to run: \n1) Forward Search\n2) Backward Search\n3) Mark's Patent Pending Custom Algorithm of Justice" << endl;
	cin >> a;
	cout << endl;

	if(a == 1)
	{
		forwardSearch(d);
	}
	else if(a == 2)
	{
		backwardSearch(d);
	}
	else
		markComperator(d);
	return 0;
	
}
