#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//Calculate the sum of a vector
double mysum(vector<double> inv) {
    double summa = 0.0;
    for(int i=0; i<inv.size(); i++) {
        summa+=inv.at(i);
    }
    return summa;
}

//Calculate the mean of a vector
double mymean(vector<double> inv) {
    double summa = mysum(inv);
    double leng = (double) inv.size();
    return summa/leng;
}

//Calculate the median of a vector
double mymedian(vector<double> inv) {
    sort(inv.begin(),inv.end());
    int s = inv.size();
    int hs = s/2;
    if (s%2 == 0) {
        return ((double) (inv.at(hs-1)+inv.at(hs))) / 2.0;
    } else {
        return inv.at(hs);
    }
}

//Calculate the range of a vector
double myrange(vector<double> inv) {
    sort(inv.begin(),inv.end());
    int s = inv.size()-1;
    return inv.at(s) - inv.at(0);
}

//Calculate the standard deviation of a vector
double mysd(vector<double> inv) {
    double m = mymean(inv);
    double s1 = 0.0;
    double ss1;
    for (int i=0; i<inv.size(); i++) {
        ss1=(inv.at(i) - m);
        s1+=(ss1*ss1);
    }
    s1 = s1/( (double) (inv.size()-1) );
    s1 = sqrt(s1);
    return s1;
}

//Calculate the covariance of two vectors
double mycovariance(vector<double> inv1,vector<double> inv2) {
    double m1 = mymean(inv1);
    double m2 = mymean(inv2);
    double s1 = 0.0;
    double ss1;
    double ss2;
    int sz = inv1.size();
    for (int i=0; i<inv1.size(); i++) {
        ss1=(inv1.at(i) - m1);
        ss2=(inv2.at(i) - m2);
        s1+=(ss1*ss2);
    }
    return s1/((double) (inv1.size()-1));
}

//Calculate the correlation of two vectors (using functions from earlier)
double mycorrelation(vector<double> inv1,vector<double> inv2) {
    return mycovariance(inv1,inv2)/(mysd(inv1)*mysd(inv2));
}

int main(int argc, char *argv[]) {

    //Initializing reading variables
    ifstream f1;
    string line;
    string rm1, medv1;
    const int MAXLEN = 1000;
    vector<double> rm(MAXLEN);
    vector<double> medv(MAXLEN);

    //Attempting to open file
    f1.open("Boston.csv");
    if (!f1.is_open()) {
        cout << "Open failed" << endl;
        return 1;
    }

    //Read in the header
    getline(f1,line);
    cout << "HEADING: " << line << endl;

    //Read in the rest of the file and close it
    int numObservations = 0;
    while (f1.good()) {
        getline(f1,rm1,',');
        getline(f1,medv1,'\n');

        rm.at(numObservations) = stof(rm1);
        medv.at(numObservations) = stof(medv1);

        numObservations++;
    }
    rm.resize(numObservations);
    medv.resize(numObservations);
    f1.close();
    
    //Print the statistics using the above functions
    cout << "SUMS: " << mysum(rm) << " " << mysum(medv) << endl;
    cout << "MEANS: " << mymean(rm) << " " << mymean(medv) << endl;
    cout << "MEDIANS: " << mymedian(rm) << " " << mymedian(medv) << endl;
    cout << "RANGES: " << myrange(rm) << " " << myrange(medv) << endl;
    cout << "COVARIANCE: " << mycovariance(rm,medv) << endl;
    cout << "CORRELATION: " << mycorrelation(rm,medv) << endl;

}