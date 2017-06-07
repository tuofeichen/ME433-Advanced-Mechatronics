package com.example.chentuofei.hw13;

import android.Manifest;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.SeekBar;

import com.hoho.android.usbserial.driver.CdcAcmSerialDriver;
import com.hoho.android.usbserial.driver.ProbeTable;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static android.graphics.Color.blue;
import static android.graphics.Color.green;
import static android.graphics.Color.red;
import static android.graphics.Color.rgb;
import static java.lang.Math.abs;


public class MainActivity extends Activity implements TextureView.SurfaceTextureListener {

    int basePWM = 600;
    int thresh = 0; // comparison value (threshold for green)
    int leftPWM = 0,rightPWM = 0;
    int lastCor = 0;
    int err = 0;
    int lastcom = 0;
    boolean start = false;
    float Kp = 0.1f, Kd = 0.1f, Ki = 0.1f;

    SeekBar myControlLeft, myControlKp, myControlKd, myControlKi, myControlPWM;
    
    Button button;
    TextView mTextView; // camera status
    TextView myTextView2;
    TextView myTextView3;
    ScrollView myScrollView;

    MediaPlayer fightSong, finishSong;



    private UsbManager manager;
    private UsbSerialPort sPort;
    private final ExecutorService mExecutor = Executors.newSingleThreadExecutor();
    private SerialInputOutputManager mSerialIoManager;
    private Camera mCamera;
    private TextureView mTextureView;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;
    private Bitmap bmp = Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888);
    private Canvas canvas = new Canvas(bmp);
    private Paint paint1 = new Paint();

    static long prevtime = 0; // for FPS calculation

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // keeps the screen from turning off

        fightSong = MediaPlayer.create(getApplicationContext(), R.raw.nu);
        finishSong = MediaPlayer.create(getApplicationContext(), R.raw.finish);

        mTextView = (TextView) findViewById(R.id.cameraStatus);
        myControlLeft  = (SeekBar) findViewById(R.id.thresh);
        myControlKp  = (SeekBar) findViewById(R.id.Kp);
        myControlKd = (SeekBar) findViewById(R.id.Kd);
        myControlKi = (SeekBar) findViewById(R.id.Ki);
        myControlPWM = (SeekBar) findViewById(R.id.PWM);

        button = (Button) findViewById(R.id.button1);

        myTextView2 = (TextView) findViewById(R.id.textView02);
        myTextView3 = (TextView) findViewById(R.id.textView03);
        myScrollView = (ScrollView) findViewById(R.id.ScrollView01);



        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                start = !start;
                if (start)
                {
                    fightSong.start();
                }
                else
                {
                    fightSong.stop();
                }
            }
        });

        // see if the app has permission to use the camera
        // ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.CAMERA}, 1);
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED) {
            mSurfaceView = (SurfaceView) findViewById(R.id.surfaceview);
            mSurfaceHolder = mSurfaceView.getHolder();

            mTextureView = (TextureView) findViewById(R.id.textureview);
            mTextureView.setSurfaceTextureListener(this);

            // set the paintbrush for writing text on the image
            paint1.setColor(0xffff0000); // red
            paint1.setTextSize(24);

            mTextView.setText("started camera");
        } else {
            mTextView.setText("no camera permissions");
        }
        setMyCtrlListener(); // start listen to bar status

        manager = (UsbManager) getSystemService(Context.USB_SERVICE);
    }

    private void setMyCtrlListener() {
        myControlLeft.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                thresh = progress;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        myControlKp.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Kp = (float)progress/50.0f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        
        myControlKd.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Kd = (float)progress/50.0f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        myControlKi.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Ki = (float)progress/2000.0f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        myControlPWM.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                basePWM = progress*1200/100;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });




    }


    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        mCamera = Camera.open();
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(640, 480);
        parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_INFINITY); // no autofocusing
        parameters.setAutoExposureLock(true); // keep the white balance constant
        mCamera.setParameters(parameters);
        mCamera.setDisplayOrientation(90); // rotate to portrait mode

        try {
            mCamera.setPreviewTexture(surface);
            mCamera.startPreview();
        } catch (IOException ioe) {
            // Something bad happened
        }
    }

    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        // Ignored, Camera does all the work for us
    }

    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        mCamera.stopPreview();
        mCamera.release();
        return true;
    }


    // tuned PD loop Kp = 0.168, Kd = 0.16
    // tuned PID Kp = 0.128, Kd = 0.02 Ki = 0.001
    // the important function
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // every time there is a new Camera preview frame
        mTextureView.getBitmap(bmp);
        int center_color = bmp.getPixel(320,240);

        int com = 0, com_cnt = 0, heightInterval = 160;

        final Canvas c = mSurfaceHolder.lockCanvas();
        int[] pixels = new int[bmp.getWidth()]; // pixels[] is the RGBA data
        if (c != null) {
            for (int startY =  bmp.getHeight()/2 + heightInterval ;startY < bmp.getHeight();startY+=30) {

                bmp.getPixels(pixels, 0, bmp.getWidth(), 0, startY, bmp.getWidth(), 1);

                // in the row, see if there is more green than red
                for (int i = 0; i < bmp.getWidth(); i++) {

                    if ((red(pixels[i])-(green(pixels[i]))>thresh)&&(red(pixels[i])-(blue(pixels[i]))>thresh))
                    {
                        pixels[i] = rgb(255, 0, 0);
                        com += i;
                        com_cnt++;
                    }
                }
                // update the row
                bmp.setPixels(pixels, 0, bmp.getWidth(), 0, startY, bmp.getWidth(), 1);
            }
            if ((com_cnt != 0)&&(com!=0)) {
                lastcom = com ;
                com /= com_cnt;
            }
            else {
                com = lastcom; // remember the last com value
            }
        }

        // draw a circle at some position
        String status = "left "+leftPWM + " right "+ rightPWM + " thresh " + thresh+ " PWM " + basePWM+ '\n';
        status = status + "Kp " + Kp + " Kd " + Kd + " Ki " + Ki + " com " + com +'\n';

//        status = status + "R " + red(center_color)+ " G " +green(center_color)+ " B "+blue(center_color);
        String PWM2USB;
        if ((com != 0)&&start)
        {
            float cor = Kp*(com-bmp.getWidth()/2);
            int base = basePWM;
            // slow down at sharp turn

            if ((abs(cor) > 250 ) && (base > 10))
            {
                base -= 60;
            }
            else if((abs(cor) < 20 )&& (base < basePWM))
            {
                base = basePWM;
            }


            leftPWM  = base  + (int)cor; // proportional term
            rightPWM = base  - (int)cor;


            leftPWM  += (lastCor-(int)cor)* Kd;// derivative term (damping)
            rightPWM -= (lastCor-(int)cor)* Kd;//


            leftPWM  +=  Ki* err;       // integral term
            rightPWM -=  Ki* err;

            status = status + "cor " + cor + " dcor " + (lastCor-(int)cor)* Kd + " err " +err +'\n';


            leftPWM  = ((leftPWM > 1200) ? 1200 : leftPWM);
            rightPWM = ((rightPWM > 1200) ? 1200 : rightPWM);

            leftPWM  = ((leftPWM < 1) ? 1 : leftPWM);
            rightPWM = ((rightPWM < 1) ? 1 : rightPWM);



            err += com-bmp.getWidth()/2; // integrating error

            lastCor = (int)cor;

            PWM2USB = String.valueOf(rightPWM)+" "+String.valueOf(leftPWM)+'\n';

        }
        else
        {
            err = 0;
            leftPWM = 0;
            rightPWM = 0; //clear PWM
            PWM2USB = String.valueOf(0)+" "+String.valueOf(0)+'\n';
        }

        myTextView2.setText(status);
        try {
            sPort.write(PWM2USB.getBytes(), 10);
        }catch (IOException e){}


        canvas.drawCircle(com, bmp.getHeight()/2, 5, paint1); // x position, y position, diameter, color

        // write the pos as text
        c.drawBitmap(bmp, 0, 0, null);
        mSurfaceHolder.unlockCanvasAndPost(c);

        // calculate the FPS to see how fast the code is running
        long nowtime = System.currentTimeMillis();
        long diff = nowtime - prevtime;
        mTextView.setText("FPS " + 1000 / diff); // the FPS value
        prevtime = nowtime;
    }

    private final SerialInputOutputManager.Listener mListener =
            new SerialInputOutputManager.Listener() {
                @Override
                public void onRunError(Exception e) {

                }

                @Override
                public void onNewData(final byte[] data) {
                    MainActivity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            MainActivity.this.updateReceivedData(data);
                        }
                    });
                }
            };
//
//    @Override
    protected void onPause(){
        super.onPause();
        fightSong.stop();
        fightSong.release();
        stopIoManager();
        if(sPort != null){
            try{
                sPort.close();
            } catch (IOException e){ }
            sPort = null;
        }
//        finish();
    }

//    @Override
    protected void onResume() {
        super.onResume();

        ProbeTable customTable = new ProbeTable();
        customTable.addProduct(0x04D8,0x000A, CdcAcmSerialDriver.class);
        UsbSerialProber prober = new UsbSerialProber(customTable);

        final List<UsbSerialDriver> availableDrivers = prober.findAllDrivers(manager);

        if(availableDrivers.isEmpty()) {
            //check
            return;
        }

        UsbSerialDriver driver = availableDrivers.get(0);
        sPort = driver.getPorts().get(0);

        if (sPort == null){
            //check
        }else{
            final UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
            UsbDeviceConnection connection = usbManager.openDevice(driver.getDevice());
            if (connection == null){
                //check
                PendingIntent pi = PendingIntent.getBroadcast(this, 0, new Intent("com.android.example.USB_PERMISSION"), 0);
                usbManager.requestPermission(driver.getDevice(), pi);
                return;
            }

            try {
                sPort.open(connection);
                sPort.setParameters(9600, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);

            }catch (IOException e) {
                //check
                try{
                    sPort.close();
                } catch (IOException e1) { }
                sPort = null;
                return;
            }
        }
        onDeviceStateChange();
    }
//
    private void stopIoManager(){
        if(mSerialIoManager != null) {
            mSerialIoManager.stop();
            mSerialIoManager = null;
        }
    }

    private void startIoManager() {
        if(sPort != null){
            mSerialIoManager = new SerialInputOutputManager(sPort, mListener);
            mExecutor.submit(mSerialIoManager);
        }
    }

    private void onDeviceStateChange(){
        stopIoManager();
        startIoManager();
    }
//
    private void updateReceivedData(byte[] data) {

        //for displaying:
        String rxString = null;
////        if (start) {
//            try {
//                rxString = new String(data, "UTF-8"); // put the data you got into a string
//                myTextView3.append(rxString);
//                myTextView2.setText(rxString);
//                myScrollView.fullScroll(View.FOCUS_DOWN);
//            } catch (UnsupportedEncodingException e) {
//                e.printStackTrace();
//            }
//        }
    }
}