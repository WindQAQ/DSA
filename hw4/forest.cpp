#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

#define MAX_FEATURE 10000
#define Error 0.0000001

class Data{
public:
    int label;
    double *feature;
};

double confusion(int a, int b)
{
    return (a+b == 0)? 0: 1 - pow((double)a/(double)(a+b), 2) - pow((double)b/(double)(a+b), 2);
}

void print_tab(int tab)
{
    for(int i = 0; i < tab; i++) printf("\t");
}

void predict_fun(vector <Data>& data, vector <int> line, int max_feature, int tab)
{
    double min_conf = 2.0, temp_conf, correct_thres = 0;
    int correct_feature = -1, yes = 0, size = line.size();
    for(int i = 0; i < size; i++)
        if(data[line[i]].label == 1)
            yes++; 
    for(int index = 1; index <= max_feature; index++){
        int temp_yes = 0;
        sort(line.begin(), line.end(), 
        	[index, &data](int a, int b)->bool{return data[a].feature[index] < data[b].feature[index];});
        for(int i = 0; i < size - 1; i++){
            if(data[line[i]].label == 1)
                temp_yes++;
            if(fabs(data[line[i]].feature[index] - data[line[i+1]].feature[index]) > Error){//round-off-error
                temp_conf = (confusion(temp_yes, i+1-temp_yes)*(i+1) + 
                             confusion(yes-temp_yes, size-i-1-yes+temp_yes)*(size-i-1))/(double)size;
                if(min_conf > temp_conf){
                    min_conf = temp_conf;
                    correct_thres = (data[line[i]].feature[index]+data[line[i+1]].feature[index])/2;
                    correct_feature = index;
                }
            }
        }
    }
    if(confusion(yes, size-yes) <= Error || correct_feature == -1){//terminal
        print_tab(tab);
        if(yes > size-yes)
        	printf("Yes++;\n");
        else printf("No++;\n");
        return;
    } 
    else{
        vector <int> L, R;
        for(int i = 0; i < size; i++){
            if(data[line[i]].feature[correct_feature] < correct_thres)
                L.push_back(line[i]);
            else R.push_back(line[i]);
        }
        print_tab(tab);
        printf("if(attr[%d] < %f){\n", correct_feature, correct_thres);
        predict_fun(data, L, max_feature, tab+1);
        print_tab(tab);
        printf("}\n");
        print_tab(tab);
        printf("else{\n");
        predict_fun(data, R, max_feature, tab+1);
        print_tab(tab);
        printf("}\n");
    }
}

int main(int argc, char** argv)
{
    ifstream fin;
    string istring;
    vector <Data> data;
    vector <int> line;
    int max_feature = 0, n = 0;
    fin.open(argv[1]);
    assert(fin.is_open());

    while (getline(fin, istring)) {
        Data data_tmp;
        data_tmp.feature = new double [MAX_FEATURE];
        char *cstring, *tmp;
        memset(data_tmp.feature, 0, sizeof(double) * MAX_FEATURE);
        cstring = new char[istring.size() + 1];
        strncpy(cstring, istring.c_str(), istring.size()+1);

        tmp =  strtok(cstring, ": ");
        data_tmp.label = atof(tmp);
        tmp = strtok(NULL, ": ");
       
        while(tmp != NULL) {
            int id = atoi(tmp);
            if(id > max_feature)
                max_feature = id;
            tmp = strtok(NULL, ": ");
            data_tmp.feature[id] = atof(tmp);
            tmp = strtok(NULL, ": ");
        }

        line.push_back(n);
        data.push_back(data_tmp);
        n++;
        delete[] cstring;

    }

    fin.close();
    srand((unsigned)time(NULL));
    printf("int forest_predict(double *attr){\n");
    printf("\tint Yes = 0, No = 0;\n");
    for(int i = 1; i <= atoi(argv[2]); i++){
    	printf("tree%d_predict:\n", i);
    	vector <int> line_temp;
    	vector <int> shuffle;
    	shuffle.resize(n);
    	for(int j = 0; j < n; j++)
    		shuffle[j] = j;
    	for(int j = 0; j < n / 3; j++){
    		int k = j + rand() % (n - j);
    		swap(shuffle[j], shuffle[k]);
    		line_temp.push_back(shuffle[j]);
    	}
    	predict_fun(data, line_temp, max_feature, 1);
    }
    printf("voting:\n\tif(Yes > No)\n\t\treturn 1;\n\telse\n\t\treturn -1;\n}\n");
    return 0;
}
