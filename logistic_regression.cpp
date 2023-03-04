#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char*argv[]) {
    ifstream f1;
    string line;
    string id1,pclass1,survival1,sex1,age1;
    const int MAXLEN = 100000;
    vector<string> id(MAXLEN);
    vector<int> pclass(MAXLEN);
    vector<int> survival(MAXLEN);
    vector<int> sex(MAXLEN);
    vector<double> age(MAXLEN);

    //Attempting to open file
    f1.open("titanic_project.csv");
    if (!f1.is_open()) {
        cout << "Open failed" << endl;
        return 1;
    }

    //Read in the header
    getline(f1,line);
    cout << "HEADING: " << line << endl << endl;

    //Read in the rest of the file and close it
    int numObservations = 0;
    while (f1.good()) {
        getline(f1,id1,',');
        getline(f1,pclass1,',');
        getline(f1,survival1,',');
        getline(f1,sex1,',');
        getline(f1,age1,'\n');
        id.at(numObservations) = id1;
        pclass.at(numObservations) = stoi(pclass1);
        survival.at(numObservations) = stoi(survival1);
        sex.at(numObservations) = stoi(sex1);
        age.at(numObservations) = stof(age1);
        numObservations++;
    }
    id.resize(numObservations);
    pclass.resize(numObservations);
    survival.resize(numObservations);
    sex.resize(numObservations);
    age.resize(numObservations);
    f1.close();

    //Initialize values
    double errorValue1;
    double b0 = 1.0;
    double b1 = 1.0;
    double learnRate = 0.001;
    double e = 2.718281828459045;
    int epochNumber = 10000;
    int v;
    double mp1;
    double sp1;
    int trimLength = 800;
    double srv;

    //Perform gradient descent.
    for (int i=0; i<epochNumber*trimLength; i++) {
        v = i%trimLength;
        mp1 = -(b0+b1*sex.at(v));
        sp1 = 1.0/(1+pow(e,mp1));
        srv = survival.at(v);
        errorValue1 = (srv-sp1);
        b0 = b0+learnRate*errorValue1;
        b1 = b1+learnRate*errorValue1*sex.at(v);
    }
    //Output final weights.
    cout << "FINAL WEIGHTS: " << b0 << " " << b1 <<endl<<endl;
    
    int real;
    int rp1;
    double confusion1[2][2];
    confusion1[0][0]=0.0;
    confusion1[0][1]=0.0;
    confusion1[1][0]=0.0;
    confusion1[1][1]=0.0;
    
    for(int i=trimLength; i<numObservations; i++) {
        mp1 = -(b0+b1*sex.at(i));
        sp1 = 1.0/(1+pow(e,mp1));
        if (sp1 <= 0.5) {
            rp1 = 0;
        } else {
            rp1 = 1;
        }
        real = survival.at(i);
        confusion1[rp1][real]+=1.0;
    }
    cout << "CONFUSION MATRIX" << endl;
    cout << confusion1[0][0] << " " << confusion1[0][1] << endl;
    cout << confusion1[1][0] << " " << confusion1[1][1] << endl << endl;
    double accuracy1 = (confusion1[0][0]+confusion1[1][1])/(numObservations-trimLength);
    double sensitivity1 = (confusion1[1][1])/(confusion1[1][1]+confusion1[1][0]);
    double specificity1 = (confusion1[0][0])/(confusion1[0][0]+confusion1[0][1]);
    cout << "ACCURACY: " << accuracy1 << endl;
    cout << "SENSITIVITY: " << sensitivity1 << endl;
    cout << "SPECIFICITY: " << specificity1 << endl;


}