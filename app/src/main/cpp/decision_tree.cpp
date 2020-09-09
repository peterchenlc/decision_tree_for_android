#ifndef DECISIONTREE
#define DECISIONTREE
#include <vector>
#include <math.h>
#include <iostream>
#include <android/log.h>

#define APPNAME "MyApp"

using namespace std;

class TrainData {
public:
    vector<vector<int> > input;
    vector<int> output;
    void insertSample(vector<int> data, int out) {
        input.push_back(data);
        output.push_back(out);
    }
};

class TreeNode {
public:
    int attribute;
    bool isLeaf;
    vector<TreeNode*> children;
    TreeNode(int _attribute, bool _isLeaf) : attribute(_attribute), isLeaf(_isLeaf) {}
};

class DecisionTree {
private:
    TreeNode *root = nullptr;
    vector<vector<int>> AttrData;
    TrainData trainData;

    TreeNode* trainID3(TrainData trainData, vector<int> usedAttr) {
        TreeNode *root = new TreeNode(0, false);

        bool sameOutput = true;
        for (int i = 1; i < trainData.output.size(); i++) {
            if (trainData.output[i] != trainData.output[i - 1]) {
                sameOutput = false;
                break;
            }
        }
        if (sameOutput) {
            return new TreeNode(trainData.output[0], true);
        }

        if (usedAttr.size() == AttrData.size()) {
            return new TreeNode(getLargestClass(trainData), true);
        }

        int largestInfoGainAttribute = getLargestInfoGainAttribute(trainData, usedAttr);
        usedAttr.push_back(largestInfoGainAttribute);
        root->attribute = largestInfoGainAttribute;

        for (int i = 0; i < AttrData[largestInfoGainAttribute].size(); i++) {
            TrainData temp;
            for (int j = 0; j < trainData.output.size(); j++) {
                if (i == trainData.input[j][largestInfoGainAttribute]) {
                    temp.insertSample(trainData.input[j], trainData.output[j]);
                }
            }
            if (temp.input.empty()) {
                root->children.push_back(new TreeNode(getLargestClass(trainData), true));
            } else {
                root->children.push_back(trainID3(temp, usedAttr));
            }
        }

        return root;
    }

    int getLargestClass(TrainData trainData) {
        vector<int> output;
        vector<int> count;
        for (int i = 0; i < trainData.output.size(); i++) {
            bool exist = false;
            int index = 0;
            for (int j = 0; j < output.size(); j++) {
                if (output[j] == trainData.output[i]) {
                    exist = true;
                    index = j;
                    break;
                }
            }
            if (exist) {
                count[index]++;
            } else {
                output.push_back(trainData.output[i]);
                count.push_back(1);
            }
        }

        int maxi = 0;
        int max = 0;
        for (int i = 0 ; i < count.size(); i++) {
            if (count[i]>max) {
                maxi = i;
                max = count[i];
            }
        }
        return output[maxi];
    }

    int getLargestInfoGainAttribute(TrainData trainData, vector<int> usedAttr) {
        vector<double> informationGains;

        bool used;
        for (int i = 0; i < AttrData.size(); i++) {
            used = false;
            for (int j = 0; j < usedAttr.size(); j++) {
                if (i == usedAttr[j]) {
                    used = true;
                    break;
                }
            }

            if (used) {
                informationGains.push_back(0.0);
            } else {
                double infoEntropy = calculateInformationEntropy(trainData);
                for (int j = 0; j < AttrData[i].size(); j++) {
                    TrainData temp;
                    for (int k = 0; k < trainData.input.size(); k++) {
                        if (j == trainData.input[k][i]) {
                            temp.insertSample(trainData.input[k], trainData.output[k]);
                        }
                    }
                    if (!temp.input.empty()) {
                        infoEntropy -= (double(temp.input.size()) / double(trainData.input.size())) *
                                       calculateInformationEntropy(temp);
                    }
                }
                informationGains.push_back(infoEntropy);
            }
        }

        /*计算信息增益最高的属性*/
        int maxi=0;
        double max=0;
        for (int i = 0; i < informationGains.size(); i++) {
            if (informationGains[i] > max) {
                maxi=i;
                max=informationGains[i];
            }
        }
        return maxi;
    }

    double calculateInformationEntropy(TrainData data) {
        vector<double> output;
        vector<double> count;
        for (int i = 0; i < data.output.size(); i++) {
            bool exist = false;
            int index = 0;
            for (int j = 0; j < output.size(); j++) {
                if (output[j] == data.output[i]) {
                    exist = true;
                    index = j;
                    break;
                }
            }
            if (exist) {
                count[index]++;
            } else {
                output.push_back(data.output[i]);
                count.push_back(1);
            }
        }

        double total = 0;
        for (int i = 0; i < count.size(); i++) {
            total += count[i];
        }
        double entropy = 0;
        for (int i = 0; i < count.size(); i++) {
            double ratio = count[i] / total;
            entropy -= ratio * log2(ratio);
        }
        return entropy;
    }

    void setTrainData(TrainData _trainData) {
        trainData = _trainData;
    }

    TrainData getTrainData() {
        TrainData trainData;
        trainData.insertSample({0, 0, 0, 0}, 0);
        trainData.insertSample({0, 0, 0, 1}, 0);
        trainData.insertSample({1, 0, 0, 0}, 1);
        trainData.insertSample({2, 1, 0, 0}, 1);
        trainData.insertSample({2, 2, 1, 0}, 1);
        trainData.insertSample({2, 2, 1, 1}, 0);
        trainData.insertSample({1, 2, 1, 1}, 1);
        trainData.insertSample({0, 1, 0, 0}, 0);
        trainData.insertSample({0, 2, 1, 0}, 1);
        trainData.insertSample({2, 1, 1, 0}, 1);
        trainData.insertSample({0, 1, 1, 1}, 1);
        trainData.insertSample({1, 1, 0, 1}, 1);
        trainData.insertSample({1, 0, 1, 0}, 1);
        trainData.insertSample({2, 1, 0, 1}, 0);
        return trainData;
    }

public:
    DecisionTree() {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Constructor...");

        AttrData.push_back({0, 1, 2});
        AttrData.push_back({0, 1, 2});
        AttrData.push_back({0, 1});
        AttrData.push_back({0, 1});
    }

    void train() {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "train begin...");
        setTrainData(getTrainData());

        vector<int> usedAttr;
        root = trainID3(trainData, usedAttr);
    }

    int predict(vector<int> data) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "predict begin...");
        if (root == nullptr) {
            return -1;
        }
        if (root->isLeaf) {
            return root->attribute;
        }
        TreeNode *p = root->children[data[root->attribute]];
        while (!p->isLeaf) {
            p = p -> children[data[p->attribute]];
        }
        string logStr = "predict result = " + std::to_string(p->attribute);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s", logStr.c_str());
        return p->attribute;
    }
};

#endif