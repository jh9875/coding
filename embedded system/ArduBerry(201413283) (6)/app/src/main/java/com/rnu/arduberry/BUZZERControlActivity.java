package com.rnu.arduberry;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.rnu.bt.BTService;
import com.rnu.data.Constant;

public class BUZZERControlActivity extends Activity {
    //------------------------------------------------------------------------
    //- Member Variable
    //------------------------------------------------------------------------
    private static final String TAG = "BUZZERControlActivity";
    private static final boolean D = true;

    //- Request 요청 상수 -----------------------------------------------------
    private static final int 	REQUEST_CONNECT_DEVICE 		= 1;
    private static final int 	REQUEST_ENABLE_BT 			= 2;


    //- BT 관련  -------------------------------------------------------------
    private String 				mConnectedDeviceName 			= null;
    private BluetoothAdapter 		mBluetoothAdapter 			= null;
    private BTService 				mBTService 					= null;

    //- UI Widget ------------------------------------------------------------
    private Button 				mBtBTN;


    //private String 				mCmdMessage;

    private ProgressDialog  		mProgress;
    private Resources mAppRes;




    //------------------------------------------------------------------------
    //- Member Method : Activity's Override Method
    //------------------------------------------------------------------------
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.buzzer_layout);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        if(D) Log.e(TAG, "++ ON START ++");

        if (!mBluetoothAdapter.isEnabled())
            startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), REQUEST_ENABLE_BT);
        else
        if (mBTService == null) initUI();
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
        if(D) Log.e(TAG, "+ ON RESUME +");

        if (mBTService != null)
            if (mBTService.getState() == BTService.STATE_NONE) mBTService.start();
    }

    @Override
    public synchronized void onPause() {
        super.onPause();
        if(D) Log.e(TAG, "- ON PAUSE -");
    }

    @Override
    public void onStop() {
        super.onStop();
        if(D) Log.e(TAG, "-- ON STOP --");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mBTService != null) mBTService.stop();
        if(D) Log.e(TAG, "--- ON DESTROY ---");

        if(mProgress != null) mProgress.dismiss();
    }


    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);

        switch (requestCode)
        {
            case REQUEST_CONNECT_DEVICE:
                if (resultCode == Activity.RESULT_OK) connectDevice(data);
                break;

            case REQUEST_ENABLE_BT:
                if (resultCode == Activity.RESULT_OK) {
                    initUI();
                } else {
                    Log.d(TAG, "BT not enabled");
                    Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                    finish();
                }
                break;
        }
    }

    //------------------------------------------------------------------------
    //- Member Method : Custom Method
    //------------------------------------------------------------------------
    public void clickFunc(View v) {

        switch(v.getId())
        {
            case R.id.btBTN:
                if (mBTService != null) mBTService.stop();


                startActivityForResult(new Intent(this,
                                com.rnu.arduberry.DeviceListActivity.class),
                        REQUEST_CONNECT_DEVICE);
                break;



            case R.id.btn0:
              sendMessage("@PIE,0#");
                break;
            case R.id.btn1:
                sendMessage("@PIE,1#");
                break;
            case R.id.btn2:
                sendMessage("@PIE,2#");
                break;
            case R.id.btn3:
                sendMessage("@PIE,3#");
                break;
            case R.id.btn4:
                sendMessage("@PIE,4#");
                break;
            case R.id.btn5:
                sendMessage("@PIE,5#");
                break;
            case R.id.btn6:
                sendMessage("@PIE,6#");
                break;
            case R.id.btn7:
                sendMessage("@PIE,7#");
                break;
            case R.id.btn8:
                sendMessage("@PIE,8#");
                break;
            case R.id.btn9:
                sendMessage("@PIE,9#");
                break;


        }
    }


    //------------------------------------------------------------------------
    //- 초기회 함수
    //------------------------------------------------------------------------
    private void initUI(){
        mAppRes     = this.getResources();
        mBtBTN = (Button)findViewById(R.id.btBTN);
        mBtBTN.setVisibility(View.VISIBLE);
        mBtBTN.setBackgroundResource(R.drawable.selector_bt_btn);
        mBtBTN.setSelected(false);


        mBTService = new BTService(this, mHandler);
    }



    //------------------------------------------------------------------------
    //- BT 기반 제어 관련 Method
    //------------------------------------------------------------------------
    private void sendMessage(String message){
        if (mBTService.getState() != BTService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        if (message.length() > 0) {
            byte[] send = message.getBytes();
            mBTService.write(send);
        }
    }

    private void connectDevice(Intent data) {
        String address = data.getExtras().getString(com.rnu.arduberry.DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        mBTService.connect(device);
    }

    private void setResultValue(String readMessage) {
        String cmd = readMessage.substring(0, 3);
        String data = readMessage.substring(4);

        /*
        if("TMP".equals(cmd))
            mTvTemp.setText(data);
        else if("HUM".equals(cmd))
            mTvHum.setText(data);
        else if("ILL".equals(cmd))
            mTvIll.setText(data);

         */
    }


    private void showProgressDialog(String szType){

        Log.i(TAG,"showProgressDialog()");

        mProgress = new ProgressDialog(this, R.style.ProgressDialog);
        mProgress.setCancelable(false);
        mProgress.setMessage(" [ " +szType+" ] " +mAppRes.getString(R.string.title_connecting));
        mProgress.show();
    }


    //------------------------------------------------------------------------------
    //- BT 기반 송수신 데이터에 따른 UI 제어 핸들러
    //------------------------------------------------------------------------------
    private final Handler mHandler = new Handler() {

        private ProgressDialog mDialog;

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what)
            {
                case Constant.MESSAGE_STATE_CHANGE:
                    if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                    switch (msg.arg1)
                    {
                        case BTService.STATE_CONNECTED:
                            mBtBTN.setSelected(true);
                            if(mProgress != null) mProgress.dismiss();
                            break;

                        case  BTService.STATE_CONNECTING:
                            mBtBTN.setSelected(false);
                            showProgressDialog("Bluetooth");
                            break;

                        case BTService.STATE_LISTEN:
                        case BTService.STATE_NONE:
                            mBtBTN.setSelected(false);
                            break;
                    }
                    break;

                case Constant.MESSAGE_WRITE:
                    break;

                case Constant.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;

                    if(msg.arg2 == 1) {	// Image
                        int offsetHead = 4;
                        BitmapFactory.Options options = new BitmapFactory.Options();
                        options.inSampleSize = 1;
                        Bitmap bm = BitmapFactory.decodeByteArray(readBuf, offsetHead,
                                msg.arg1 - offsetHead, options);
                       // mIvPhoto.setImageBitmap(bm);
                      //  mIvPhoto.setVisibility(View.VISIBLE);
                      //  mNoTXT.setVisibility(View.GONE);
                        mDialog.dismiss();
                    } else {
                        String readMessage = new String(readBuf, 0, msg.arg1);

                        Log.d(TAG, "readMessage : "+ readMessage);
                        setResultValue(readMessage);
                     //   mNoTXT.setText(readMessage);
                    }
                    break;

                case Constant.MESSAGE_DEVICE_NAME:
                    mConnectedDeviceName = msg.getData().getString(Constant.DEVICE_NAME);
                    Toast.makeText(getApplicationContext(), "Connected to " + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                    break;

                case Constant.MESSAGE_TOAST:
                    if(msg.getData().getInt(Constant.TOAST) == R.string.unable_connected)
                        if(mProgress != null) mProgress.dismiss();

                    Toast.makeText(getApplicationContext(), mAppRes.getString(msg.getData().getInt(Constant.TOAST)), Toast.LENGTH_SHORT).show();
                    break;

                case Constant.MESSAGE_START_IMAGE:
                    mDialog = new ProgressDialog(BUZZERControlActivity.this);
                    mDialog.setMessage(getResources().getString(R.string.load_image));
                    mDialog.setCanceledOnTouchOutside(false);
                    mDialog.show();
                    break;
            }
        }
    };
}