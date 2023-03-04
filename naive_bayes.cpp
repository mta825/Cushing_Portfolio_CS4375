#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

double pi = 3.141592653589793;
double e =  2.718281828459045;

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

//Calculate the variance of a vector
double myvariance(vector<double> inv) {
    double m = mymean(inv);
    double s1 = 0.0;
    double ss1;
    for (int i=0; i<inv.size(); i++) {
        ss1=(inv.at(i) - m);
        s1+=(ss1*ss1);
    }
    s1 = s1/( (double) (inv.size()-1) );
    //s1 = sqrt(s1);
    return s1;
}

//Calculate the likelihood for a continuous value
double likelihood(double value, double mean, double variance) {
    return pow(e,-1*((value-mean)*(value-mean))/(2*variance))/sqrt(2*pi*variance);
}

int main(int argc, char*argv[]) {
    //Initialize variables for the file read
    ifstream f1;
    string line;
    string id1,pclass1,survival1,sex1,age1;
    const int MAXLEN = 100000;
    vector<string> id0(MAXLEN);
    vector<double> pclass0(MAXLEN);
    vector<double> survival0(MAXLEN);
    vector<double> sex0(MAXLEN);
    vector<double> age0(MAXLEN);

    vector<string> id2(MAXLEN);
    vector<double> pclass2(MAXLEN);
    vector<double> survival2(MAXLEN);
    vector<double> sex2(MAXLEN);
    vector<double> age2(MAXLEN);

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
        if (numObservations < 800) {
            id0.at(numObservations) = id1;
            pclass0.at(numObservations) = stoi(pclass1);
            survival0.at(numObservations) = stoi(survival1);
            sex0.at(numObservations) = stoi(sex1);
            age0.at(numObservations) = stof(age1);
        } else {
            id2.at(numObservations-800) = id1;
            pclass2.at(numObservations-800) = stoi(pclass1);
            survival2.at(numObservations-800) = stoi(survival1);
            sex2.at(numObservations-800) = stoi(sex1);
            age2.at(numObservations-800) = stof(age1);
        }
        numObservations++;
    }
    id0.resize(800);
    pclass0.resize(800);
    survival0.resize(800);
    sex0.resize(800);
    age0.resize(800);
    id2.resize(numObservations-800);
    pclass2.resize(numObservations-800);
    survival2.resize(numObservations-800);
    sex2.resize(numObservations-800);
    age2.resize(numObservations-800);
    f1.close();

    //Initialize variables for the data passthrough
    double percentLived =  mymean(survival0);
    double percentDied = 1.0-percentLived;

    double pclass1Lived = 0.0;
    double pclass2Lived = 0.0;
    double pclass3Lived = 0.0;
    double pclass1Total = 0.0;
    double pclass2Total = 0.0;
    double pclass3Total = 0.0;
        
    double sex0Lived = 0.0;
    double sex1Lived = 0.0;
    double sex0Total = 0.0;
    double sex1Total = 0.0;

    double totalLived = 0.0;

    vector<double> ageLived;
    vector<double> ageDied;
    vector<double> ages;

    //Train the model.
    for (int i=0; i<800; i++) {
        if (sex0.at(i) == 0) {
            sex0Lived+=survival0.at(i);
            sex0Total+=1;
        } else if (sex0.at(i) == 1){
            sex1Lived+=survival0.at(i);
            sex1Total+=1;
        }

        if (pclass0.at(i) == 1) {
            pclass1Lived+=survival0.at(i);
            pclass1Total+=1;
        } else if (pclass0.at(i) == 2) {
            pclass2Lived+=survival0.at(i);
            pclass2Total+=1;
        } else if (pclass0.at(i) == 3) {
            pclass3Lived+=survival0.at(i);
            pclass3Total+=1;
        }

        if (survival0.at(i) == 1) totalLived++;

        if (survival0.at(i) == 0) ageDied.push_back(age0.at(i));
        else ageLived.push_back(age0.at(i));
        ages.push_back(age0.at(i));
           
    }

    //Output model values 
    cout << "INITIAL LIKELIHOODS FOR PREDICTORS: " << endl; 
    cout << "SEX VERSUS SURVIVAL" << endl;  
    cout << (sex0Total-sex0Lived)/(800.0-totalLived) << " " << (sex1Total-sex1Lived)/(800.0-totalLived) << endl;
    cout << sex0Lived/totalLived << " " << sex1Lived/totalLived << endl << endl;
    cout << "PASSENGER CLASS VERSUS SURVIVAL" << endl;
    cout << (pclass1Total-pclass1Lived)/(800.0-totalLived) << " "
         << (pclass2Total-pclass2Lived)/(800.0-totalLived) << " "
         << (pclass3Total-pclass3Lived)/(800.0-totalLived) << endl;
    cout << pclass1Lived/totalLived << " " << pclass2Lived/totalLived << " " << pclass3Lived/totalLived << endl << endl;
    
    //Calculate values for age
    double meanAgeDied = mymean(ageDied);
    double meanAgeLived = mymean(ageLived);
    double meanAges = mymean(ages);
    double meanVariances = myvariance(ages);
    vector<double> meanAge;
    meanAge.push_back(meanAgeDied);
    meanAge.push_back(meanAgeLived);
    double varianceAgeDied = myvariance(ageDied);
    double varianceAgeLived = myvariance(ageLived);
    vector<double> varianceAge;
    varianceAge.push_back(varianceAgeDied);
    varianceAge.push_back(varianceAgeLived);

    //Output age values
    cout << "AGE MEAN AND STANDARD DEVIATION: " << endl;
    cout << meanAgeDied << " " << meanAgeLived << endl;
    cout << sqrt(varianceAgeDied) << " " << sqrt(varianceAgeLived) << endl << endl;
    
    //Calculate and output the overall survival likelihoods
    double probLived = totalLived/800.0;
    double probDied = 1.0-probLived;
    
    cout << "OVERALL SURVIVAL PRIORS:" << endl;
    cout << probDied << " " << probLived << endl << endl;

    //Calculate the likelihoods relative to sex
    double probSex0Lived = sex0Lived/totalLived;
    double probSex1Lived = sex1Lived/totalLived;
    double probSex0Died = (sex0Total-sex0Lived)/(800.0-totalLived);
    double probSex1Died = (sex1Total-sex1Lived)/(800.0-totalLived);
    
    vector<double> probSexLived;
    probSexLived.push_back(probSex0Lived);
    probSexLived.push_back(probSex1Lived);

    vector<double> probSexDied;
    probSexDied.push_back(probSex0Died);
    probSexDied.push_back(probSex1Died);
        
    //Calculate passenger class llikelihoods
    double probPclass1Lived = pclass1Lived/totalLived;
    double probPclass2Lived = pclass2Lived/totalLived;
    double probPclass3Lived = pclass3Lived/totalLived;
    double probPclass1Died = (pclass1Total-pclass1Lived)/(800.0-pclass1Total);
    double probPclass2Died = (pclass2Total-pclass2Lived)/(800.0-pclass2Total);
    double probPclass3Died = (pclass3Total-pclass3Lived)/(800.0-pclass3Total);
    vector<double> probPclassLived;
    probPclassLived.push_back(probPclass1Lived);
    probPclassLived.push_back(probPclass2Lived);
    probPclassLived.push_back(probPclass3Lived);
    vector<double> probPclassDied;
    probPclassDied.push_back(probPclass1Died);
    probPclassDied.push_back(probPclass2Died);
    probPclassDied.push_back(probPclass3Died);
    
    //Initialize values
    double iDied;
    double iDidnt;
    double s0;
    double s1;
    double p0;
    double p1;
    double a0;
    double a1;
    int srv;
    double denom;
    double s0p=sex0Total/800.0;
    double p1p=pclass1Total/800.0;
    double s1p=sex1Total/800.0;
    double p2p=pclass2Total/800.0;
    double p3p=pclass3Total/800.0;
    vector<double> scs;
    scs.push_back(s0p);
    scs.push_back(s1p);
    vector<double> pcp;
    pcp.push_back(p1p);
    pcp.push_back(p2p);
    pcp.push_back(p3p);
    vector< vector<double> > predicts;
    double aggs;
    int mytack = numObservations-800;
    vector<double> predictions;

    //Output priors to be used later
    cout << "PRIORS FOR THE PREDICTORS IN THE TRAINING DATA:" << endl;
    cout << s0p << " " << s1p << endl;
    cout << p1p << " " << p2p << " " << p3p << endl << endl;

    //Make predictions
    for (int g=0; g<mytack; g++) {
        srv = survival2.at(g);
        s0 = probSexDied.at(sex2.at(g));
        s1 = probSexLived.at(sex2.at(g));
        p0 = probPclassDied.at(pclass2.at(g)-1);
        p1 = probPclassLived.at(pclass2.at(g)-1);
        a0 = likelihood(age2.at(g),meanAge.at(0),varianceAge.at(0));
        a1 = likelihood(age2.at(g),meanAge.at(1),varianceAge.at(1));
        aggs = likelihood(age2.at(g),meanAges,meanVariances);
        iDied = probDied*s0*p0*a0;
        iDidnt = probLived*s1*p1*a1;
        denom = iDied+iDidnt;
        vector<double> predict;
        predict.push_back(iDied/denom);
        predict.push_back(iDidnt/denom);
        predicts.push_back(predict);
        predictions.push_back(iDidnt/denom);
    }

    //Evaluate the model's predictions
    int real;
    int rp1;
    double confusion1[2][2];
    confusion1[0][0]=0.0;
    confusion1[0][1]=0.0;
    confusion1[1][0]=0.0;
    confusion1[1][1]=0.0;
    
    for (int j=0; j<survival2.size(); j++) {
        real = survival2.at(j);
        if (predictions.at(j) <= 0.5) {
            rp1=0;
        } else {
            rp1=1;
        }
        confusion1[rp1][real]+=1;
    }
    
    //Output the final evaluation
    cout << "CONFUSION MATRIX" << endl;
    cout << confusion1[0][0] << " " << confusion1[0][1] << endl;
    cout << confusion1[1][0] << " " << confusion1[1][1] << endl << endl;
    double accuracy1 = (confusion1[0][0]+confusion1[1][1])/(numObservations-800);
    double sensitivity1 = (confusion1[1][1])/(confusion1[1][1]+confusion1[1][0]);
    double specificity1 = (confusion1[0][0])/(confusion1[0][0]+confusion1[0][1]);
    cout << "ACCURACY: " << accuracy1 << endl;
    cout << "SENSITIVITY: " << sensitivity1 << endl;
    cout << "SPECIFICITY: " << specificity1 << endl;


}