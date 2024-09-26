package com.xiaoxin.jnidatacallbackdemo;

public class CallBackNative {
    // Used to load the 'jnidatacallbackdemo' library on application startup.
    static {
        System.loadLibrary("jnidatacallbackdemo");
    }

    public static native void setAudioDataCallBack(DataCallBack callBack);

    public static native void startTest();
    public static native void stopTest();

    /**
     * A native method that is implemented by the 'jnidatacallbackdemo' native library,
     * which is packaged with this application.
     */
    public static native String stringFromJNI();

    public interface DataCallBack{
        void onDataCallBack(short[] data);
    }



}