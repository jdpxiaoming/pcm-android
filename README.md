# pcm-android
ADPCM Encoder and Decoder for Android. 

Android is not capable of playing WAV files in ADPCM format and since most of the Voicemails are encoded in ADPCM, apps will not be able to play them using MediaPlayer.

PCM is uncompressed format whereas Adaptive differential pulse-code modulation (ADPCM) is a compressed using a lossy algorithm. 

This code uses NDK for faster encoding (PCM to ADPCM) and decoding (ADPCM to PCM).


# 本项目主要是把java的 byte[]数组通过底层的jni转化为adpcm 并输出长度。

```java

	    byte[] audioData= new byte[514];

	    int[] outLen = new int[1];

	    byte[] outPut = new byte[256];

        try {
            new PCMEncode().encode(audioData , audioData.length , outPut , outLen);
        } catch (Exception e) {
            e.printStackTrace();
        }
```