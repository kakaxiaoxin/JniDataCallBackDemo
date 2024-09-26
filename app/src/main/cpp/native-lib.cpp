#include <jni.h>
#include <string>

static JavaVM *gJavaVM;
static jobject gObject;
static jmethodID gMethodID;
static bool g_bAttatedT;
static bool g_isStarted;

void *generateData(void *args);


extern "C"
JNIEXPORT jstring JNICALL
Java_com_xiaoxin_jnidatacallbackdemo_CallBackNative_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xiaoxin_jnidatacallbackdemo_CallBackNative_setAudioDataCallBack(JNIEnv *env, jclass clazz,
                                                                         jobject call_back) {
    if (gJavaVM == NULL) {
        env->GetJavaVM(&gJavaVM);
        if (gJavaVM == NULL) {
            return;
        }
    }
    gObject = env->NewGlobalRef(call_back);
    if (gObject == NULL) {
        return;
    }

    jclass javaClass = env->GetObjectClass(call_back);
    if (javaClass == NULL) {
        return;
    }
    gMethodID = env->GetMethodID(javaClass, "onDataCallBack", "([S)V");
    if (gMethodID == NULL) {
        return;
    }
}

static JNIEnv *Adapter_GetEnv() {
    int status;
    JNIEnv *envnow = NULL;
    if (gJavaVM == NULL) {
        return NULL;
    }
    status = gJavaVM->GetEnv((void **) &envnow, JNI_VERSION_1_6);
    if (status < 0) {
        status = gJavaVM->AttachCurrentThread(&envnow, NULL);
        if (status < 0) {
            return NULL;
        }
        g_bAttatedT = true;
    }
    return envnow;
}

static void DetachCurrent() {
    if (g_bAttatedT) {
        gJavaVM->DetachCurrentThread();
    }
}

//send data to java thread
void audioCallback(short *data, int size) {
    JNIEnv *env = Adapter_GetEnv();
    jshortArray jArray = env->NewShortArray(size);
    env->SetShortArrayRegion(jArray, 0, size, data);
    env->CallVoidMethod(gObject, gMethodID, jArray);
    env->DeleteLocalRef(jArray);
    DetachCurrent();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xiaoxin_jnidatacallbackdemo_CallBackNative_startTest(JNIEnv *env, jclass clazz) {
    g_isStarted = true;
    pthread_t gPthread;
    pthread_create(&gPthread, NULL, generateData, NULL);
}

//generate data for callback
void *generateData(void *args) {
    while (g_isStarted){
        short data[] = {1,2,3,4,5,6,7,8};
        audioCallback(data,sizeof(data)/sizeof(short));
    }
    pthread_exit(NULL);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xiaoxin_jnidatacallbackdemo_CallBackNative_stopTest(JNIEnv *env, jclass clazz) {
    g_isStarted = false;
}