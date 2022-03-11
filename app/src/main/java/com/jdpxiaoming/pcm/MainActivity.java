package com.jdpxiaoming.pcm;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

public class MainActivity extends Activity {

	private static final String TAG = MainActivity.class.getName();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		findViewById(R.id.btEncode).setOnClickListener(
				new View.OnClickListener() {

					@Override
					public void onClick(View arg0) {
						doEncode();
					}
				});

	}

	//开始编码
    private void doEncode() {

	    Log.i(TAG,"doEncode start~");

	    byte[] audioData= new byte[514];

	    int[] outLen = new int[1];

	    byte[] outPut = new byte[256];

        try {
            new PCMEncode().encode(audioData , audioData.length , outPut , outLen);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
