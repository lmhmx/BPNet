#include "BP_cpp.h"
#include <iostream>

using namespace std;
double myrandDouble();
int myrandInt(int m);
int main(){
    
    BPNet bp;
    vector<double> ans;
    vector<vector<double>> in;
    vector<vector<double>> label;
    int num = 100;
    in.resize(num);
    label.resize(num);

    for(int i = 0;i<num;i++){
        vector<double> x;
        vector<double> y;
        x.push_back((i-40)/20.0);
        y.push_back(x.back()  / 2);
        in[i]=x;
        label[i]=y;
    }
    for (int i=0;i<50000;i++){
        //cout << i<<" ";
        int a = myrandInt(num);
        vector<double> t = in[a];
        ans=bp.forward(t);
        vector<double> y = label[a];
        vector<double> loss;
        loss=y;
        for (int j=0;j<loss.size();j++){
            loss[j] = ans[j]-y[j];
        }
        //cout <<i <<"@";
        bp.backward(loss);
        //cout <<i <<"&";
        bp.update();
        //cout <<i<<"*";
    }
    for (int i=0;i<in.size();i++){
        ans = bp.forward(in[i]);
        cout <<"in ";
        for (int j=0;j<in[i].size();j++){
            cout<<in[i][j] <<" ";
        }
        cout <<"ans ";
        for (int j=0;j<in[i].size();j++){
            cout<<ans[j] <<" ";
        }
        cout <<"label ";
        for (int j=0;j<in[i].size();j++){
            cout<<label[i][j] <<" ";
        }
        cout <<endl;
    }
    
    return 0;
}
int myrandInt(int m){
    return rand()%m;
}
double myrandDouble(){
    //static int i = 1;
    return ((double)rand())/RAND_MAX*2-1;
}
