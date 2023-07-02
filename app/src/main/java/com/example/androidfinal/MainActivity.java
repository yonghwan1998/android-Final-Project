package com.example.androidfinal;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.example.androidfinal.databinding.ActivityMainBinding;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'androidfinal' library on application startup.
    static {
        System.loadLibrary("androidfinal");
    }

    private ActivityMainBinding binding;
    public TextView Message;
    Button startBtn;
    Button resetBtn;
    String ret;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        Message = (TextView)findViewById(R.id.resultMessage);
        startBtn = (Button) findViewById(R.id.startBtn);
        startBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                StartDot();
                Message.setText("FIND MINE");
                Message.setTextColor(Color.BLACK);
            }
        });

        resetBtn = (Button) findViewById(R.id.resetBtn);
        resetBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                StartDot();
                Message.setText("MineSweeper");
                Message.setTextColor(Color.RED);
            }
        });

        TimerTask task = new TimerTask() {
            Handler mHandler = new Handler();

            @Override
            public void run() {
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        int value;
                        value = DeviceOpen();

                        if (value != -1)
                            value = RecriveRpushSwitchValue();
                        if (value != -1)
                            DeviceClose();
                        String str = Integer.toString(value, 10);

                        ret = Integer.toString(ReceiveDotValue(Integer.parseInt(str)));

                        // 실패 무반응 성공
                        switch (ret){
                            case "1":
                                break;
                            case "0":
                                Message.setText("!!!!!!BOOOOMB!!!!!!");
                                Message.setTextColor(Color.RED);
                                break;
                            default:
                        }
                    }
                }, 200);

            }
        };
        Timer t = new Timer();
        t.schedule(task, 200, 200);
    }
    public native int DeviceOpen();
    public native int DeviceClose();
    public native int RecriveRpushSwitchValue();
    public native int ReceiveDotValue(int dir);
    public native void StartDot();
}