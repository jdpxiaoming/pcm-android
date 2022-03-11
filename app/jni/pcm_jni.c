#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* ADPCM step variation table */
static int indexTable[16] =
        {
                -1, -1, -1, -1, 2, 4, 6, 8,
                -1, -1, -1, -1, 2, 4, 6, 8,
        };

static int stepsizeTable[89] =
        {
                7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
                19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
                50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
                130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
                337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
                876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
                2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
                5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
                15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
        };

static int  ADPCM_4BIT_Enconde(unsigned char* inPcm, int inLen,  unsigned char* outAdpcm);


JNIEXPORT void JNICALL
Java_com_jdpxiaoming_pcm_PCMEncode_encode2Adpcm(JNIEnv *env, jobject thiz, jbyteArray input,
                                                jint in_len, jbyteArray output, jintArray out_len) {
    unsigned char* inputData = NULL ;
    unsigned char* outputData = NULL;
    int outLen = 0;

    //java type converter to c point type .
    inputData =  (*env)->GetByteArrayElements(env, input, NULL);
    outputData = (*env)->GetByteArrayElements(env, output, NULL);
    outLen = ADPCM_4BIT_Enconde(inputData,in_len,outputData);
    //返回intArray
    (*env)->SetIntArrayRegion(env,out_len,0,1,&outLen);
    //释放内存.
    (*env)->ReleaseByteArrayElements(env,input,inputData,0);
    (*env)->ReleaseByteArrayElements(env,output,outputData,0);


}


typedef struct //2+1+1 开头使用4字节32位占位符. 第一个输入数据 所有数据的基础，当前的indexTable索引.
{
    short valprev;    /* Previous output value 2字节*/
    char index;       /* Index in stepsize table 1字节*/
    char reserved;    /* 占位符 */
}ADPCM_BLOCK_HEAD_S;

int  ADPCM_4BIT_Enconde(unsigned char* inPcm, int inLen,  unsigned char* outAdpcm)
{
    ADPCM_BLOCK_HEAD_S bHeader = {0};
    short* convPcmData = (short*)inPcm;//转码后的输入数据源.
    short prevSamp  = 0 ; // //前一帧数据.
    int index = 0;; //下一帧的indexTable索引.
    int DataIndex = 0 ; //输入数据index.
    int diff = 0 ; //下一帧数据的增量.
    unsigned char code = 0; ////编码后的输出数据.
    bool bodd = true ; //是否为消息体.
    int len = sizeof(bHeader); //输出index.跳过头信息4字节 ;


    /* PCM头信息RIFF 之后的FMT chunk*/
    bHeader.valprev = convPcmData[0];
    bHeader.index = index;
    memcpy(outAdpcm, &bHeader, sizeof(bHeader));
    //第一针作为前一个数据.
    prevSamp   = bHeader.valprev;
    //遍历输入数据.
    for ( DataIndex  = 1; DataIndex  < inLen / 2; DataIndex++)
    {
        /* step1 compute difference with previous value */
        diff  = convPcmData[DataIndex] - prevSamp  ;
        if (diff < 0)
        {
            diff = -diff;
            code = 8;
        }
        else
        {
            code = 0;
        }

        /* step 2 divide and clamp */
        if ( diff  >= stepsizeTable[index] )
        {
            code = code | 4;
            diff = diff - stepsizeTable[index];
        }
        if ( diff  >= (stepsizeTable[index] >> 1))
        {
            code = code | 2;
            diff = diff - (stepsizeTable[index] >> 1);
        }
        if ( diff  >= (stepsizeTable[index] >> 2))
        {
            code = code | 1;
        }

        /*Step 3 - Update previous value*/
        if ( bodd)
        {
            outAdpcm[len] = code;
        }
        else
        {
            outAdpcm[len++] = (code<<4) | outAdpcm[len];
        }

        bodd = !bodd;

        // predict the current sample based on the sample code:
        diff = 0;

        if (code & 4)
        {
            diff = diff + stepsizeTable[index];
        }
        if (code & 2)
        {
            diff = diff + (stepsizeTable[index] >> 1);
        }
        if (code & 1)
        {
            diff = diff + (stepsizeTable[index] >> 2);
        }
        diff = diff + (stepsizeTable[index] >> 3);
        if (code & 8)
        {
            diff = -diff;
        }

        prevSamp = prevSamp + diff;

        /* Step 4 - Clamp previous value to 16 bits */
        if (prevSamp < -32768)
        {
            prevSamp = -32768;
        }
        if (prevSamp > 32767)
        {
            prevSamp = 32767;
        }

        // adjust the step table index:
        index = index + indexTable[code];
        if (index < 0)
        {
            index = 0;
        }
        if (index > 88)
        {
            index = 88;
        }
    }

    // *outLen = len;
    return len;
}




