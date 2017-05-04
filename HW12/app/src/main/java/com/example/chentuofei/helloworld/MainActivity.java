package com.example.chentuofei.helloworld;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    SeekBar myCtrl_1,myCtrl_2,myCtrl_3;
    TextView myTextView;
    int progress_1, progress_2, progress_3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // initialize instances
        myCtrl_1  = (SeekBar) findViewById(R.id.thresh1);
        myCtrl_2  = (SeekBar) findViewById(R.id.thresh2);
        myCtrl_3  = (SeekBar) findViewById(R.id.thresh3);
        progress_1 = myCtrl_1.getProgress();
        progress_2 = myCtrl_2.getProgress();
        progress_3 = myCtrl_3.getProgress();

        myTextView = (TextView) findViewById(R.id.textView01);
        myTextView.setText("Enter whatever you Like!");
        setMyCtrlListener();
    }

    private void setMyCtrlListener() {
        myCtrl_1.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
        {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progress_1 = progress;
                myTextView.setText("Bar Status " + progress_1 + " " + progress_2+" "+progress_3);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        myCtrl_2.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
        {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progress_2 = progress;
                myTextView.setText("Bar Status " + progress_1 + " " + progress_2+" "+progress_3);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        myCtrl_3.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
        {

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progress_3 = progress;
                myTextView.setText("Bar Status " + progress_1 + " " + progress_2+" "+progress_3);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });



    }

}
