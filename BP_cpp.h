#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>

using namespace std;


class ReluLayer{
public:
    ReluLayer(){
        m_isInit = false;
    }
    ReluLayer(int inputNum){
        init(inputNum);
    }
    void init(int inputNum){
        m_isInit = true;
        m_o.resize(inputNum);
        m_in.resize(inputNum);
        m_partial.resize(inputNum);
    }
    vector<double> forward(vector<double> input){
        checkInit();
        for (int i=0;i<input.size();i++){
            m_o[i]=input[i]>0?input[i]:0;
        }
        m_in = input;
        return m_o;
    }
    vector<double> backword(vector<double> partialO){
        checkInit();
        for (int i=0;i<partialO.size();i++){
            m_partial[i] = m_in[i]>0?partialO[i]:0;
        }
        return m_partial;
    }
private:
    vector<double> m_o;
    vector<double> m_in;
    vector<double> m_partial;
private:
    bool m_isInit;
    void checkInit(){
        if(!m_isInit){
            throw "not init";
        }
    }
};

class Layer{
public:
    Layer(){
        m_isInit = false;
        srand(303);

    }
    Layer(int inputNum, int outputNum){
        init(inputNum, outputNum);
        srand(303);

    }
    void init(int inputNum, int outputNum){
        m_isInit = true;
        this->m_inputNum = inputNum;
        this->m_outputNum = outputNum;

        this->m_b.resize(outputNum);
        this->m_partialB.resize(outputNum);

        this->m_w.resize(outputNum);
        this->m_partialW.resize(outputNum);

        for (int i=0;i<outputNum;i++){
            m_w[i].resize(inputNum);
            m_partialW[i].resize(inputNum);
        }
        this->m_o.resize(outputNum);
        this->m_in.resize(inputNum);
        this->randomInit("");
    }
    vector<double> forward(vector<double> inputs){
        checkInit();

        this->m_in = inputs;
        for (int i=0;i<m_outputNum;i++){
            m_o[i]=m_b[i];
            for (int j=0;j<m_inputNum;j++){
                m_o[i] = m_o[i]+m_w[i][j]*inputs[j];
            }
        }
        return m_o;
    }
    void update(double lr, string s = ""){
        checkInit();
        for (int i=0;i<m_outputNum;i++){
            m_b[i] = m_b[i] - lr* m_partialB[i];
        }
        for (int i=0;i<m_outputNum;i++){
            for (int j=0;j<m_inputNum;j++){
                m_w[i][j] = m_w[i][j] - lr * m_partialW[i][j];
            }
        }

    }
    vector<double> backward(vector<double> partialO){
        checkInit();
        vector<double> ans;
        for (int i=0;i<m_outputNum;i++){
            m_partialB[i] = partialO[i];
        }
        for (int i=0;i<m_outputNum;i++){
            for (int j=0;j<m_inputNum;j++){
                m_partialW[i][j] = partialO[i]*m_in[j];
            }
        }
        ans.resize(m_inputNum);
        for (int j=0;j<m_inputNum;j++){
            ans[j]=0;
            for (int i=0;i<m_outputNum;i++){
                ans[j] = ans[j]+m_w[i][j]*partialO[i];
            }
        }
        return ans;
    }
private:
    void randomInit(string init){
        double a = rand();
        for (int i=0;i<m_outputNum;i++){
            a=rand();
            m_b[i]=(a/RAND_MAX)*2-1;
        }
        for (int i=0;i<m_outputNum;i++){
            for (int j=0;j<m_inputNum;j++){
                a=rand();
                m_w[i][j]=(a/RAND_MAX)*2-1;
            }
        }
    }
private:
    int m_inputNum;
    int m_outputNum;
    vector<vector<double>> m_w;
    vector<double> m_in;
    vector<double> m_b;
    vector<double> m_o;
    vector<double> m_partialB;
    vector<vector<double>> m_partialW;
private:
    bool m_isInit;
    void checkInit(){
        if(!m_isInit){
            throw "not init";
        }
    }
};


class BPNet{
public:
    BPNet(vector<int> Nums = {1,6,7,5,1}){
        int layer = Nums.size();
        m_Layer.resize(layer-1);
        for (int i = 0;i<layer-1;i++){
            m_Layer[i].init(Nums[i],Nums[i+1]);
        }
        m_relu.resize(layer-2);
        for (int i=0;i<layer-2;i++){
            m_relu[i].init(Nums[i+1]);
        }
    }
    vector<double> forward(vector<double> in){
        vector<double> ans = in;
        for (int i = 0;i<m_Layer.size()-2;i++){
            ans=m_Layer[i].forward(ans);
            ans=m_relu[i].forward(ans);
        }
        ans=m_Layer.back().forward(ans);
        return ans;
    }
    void backward(vector<double> out){
        vector<double> ans=out;
        ans = m_Layer.back().backward(ans);
        for (int i=m_Layer.size()-2;i>=0;i--){
            ans=m_relu[i].backword(ans);
            ans=m_Layer[i].backward(ans);
        }
    }
    void update(){
        for (int i=m_Layer.size()-1;i>=0;i--){
            m_Layer[i].update(0.01);
        } 
    }
private:
    vector<Layer> m_Layer;
    vector<ReluLayer> m_relu;
};
