package com.jdpxiaoming.pcm;

/**
 */

public class PCMEncode {
    private static final String TAG = PCMEncode.class.getName();

    static {
        System.loadLibrary("adpcm");
    }


    public void encode(byte[] input , int inLen , byte[] outPut , int[] outLen) throws Exception {
        encode2Adpcm(input , inLen , outPut , outLen);
    }
    private native void encode2Adpcm(byte[] input, int inLen, byte[] output , int[] outLen) throws Exception;
}
