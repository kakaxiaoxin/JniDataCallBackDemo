package com.xiaoxin.jnidatacallbackdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.xiaoxin.jnidatacallbackdemo.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {


    private ActivityMainBinding binding;
    int count, num = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Example of a call to a native method
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        TextView tv = binding.sampleText;

        binding.btnStart.setOnClickListener(v -> {
            CallBackNative.startTest();
        });

        binding.btnStop.setOnClickListener(v -> {
            CallBackNative.stopTest();
        });

        //receive callback data and show it
        CallBackNative.setAudioDataCallBack(data -> {
            if (++count == 10000) {
                runOnUiThread(() -> {
                    tv.setText("data is " + data[num]);
                });
                if (++num == data.length) {
                    num = 0;
                }
                count = 0;
            }
        });

    }

}