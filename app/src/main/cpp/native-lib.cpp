#include <jni.h>
#include <string>

#include "decision_tree.cpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_peterchen_decisiontree_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

DecisionTree decisionTree;

extern "C"
JNIEXPORT void JNICALL
Java_com_peterchen_decisiontree_MainActivity_train(JNIEnv *env, jobject thiz) {
    decisionTree.train();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_peterchen_decisiontree_MainActivity_predict(JNIEnv *env, jobject thiz) {
    return decisionTree.predict({1, 0, 1, 0});
}