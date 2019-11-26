package com.rnu.arduberry;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;


import com.rnu.data.Constant;
import com.rnu.wifi.WIFIService;

import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class WIFIControlActivity extends Activity{
	//-----------------------------------------------------------------------------------------
	//- Member Variable
	//-----------------------------------------------------------------------------------------
	private static final String TAG = "WIFIControlActivity";

	private static final Pattern PARTIAl_IP_ADDRESS =
	          Pattern.compile("^((25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[1-9][0-9]|[0-9])\\.){0,3}"+
	                           "((25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[1-9][0-9]|[0-9])){0,1}$"); 

	private Context 		mContext;
	private WIFIService 	mWIFIService = null;

	private AlertDialog 	mDlgWifi;
    private EditText 		mEtHost;
    private Button 			mWiFiBTN;
	private TextView 		mTvTemp, mTvHum, mTvIll, mNoTXT;
	private ImageView 		mIvPhoto;

	private boolean 		bRedOn = false, bGreenOn=false, bYellowOn=false, bWhiteOn=false;
	private ProgressDialog  mProgress;
	private Resources       mAppRes;

	//-----------------------------------------------------------------------------------------
	//- Member Method : Activity's Override Method
	//-----------------------------------------------------------------------------------------
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_control);

		StrictMode.enableDefaults();
		mContext = this;
		initUI();
		mDlgWifi = dialogIpPort();
		mDlgWifi.show();
	}


    @Override
    protected void onStop() {
        super.onStop();

        sendMessage("@DIS#");
        Log.i(TAG, "onStop()");
        try {
                Thread.sleep(1500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

	@Override
	protected void onDestroy() {
		super.onDestroy();

		if(mWIFIService != null) mWIFIService.stop();
		if(mDlgWifi != null && mDlgWifi.isShowing()) mDlgWifi.dismiss();
		if(mProgress != null) mProgress.dismiss();

	}


	//-----------------------------------------------------------------------------------------
	//- Member Method : Custome Method
	//-----------------------------------------------------------------------------------------
	private void initUI() {
		mAppRes     = this.getResources();
		mWiFiBTN = (Button)findViewById(R.id.wifiBTN);
		mWiFiBTN.setVisibility(View.VISIBLE);
		mWiFiBTN.setBackgroundResource(R.drawable.selector_wifi_btn);

		mTvTemp = (TextView)findViewById(R.id.tv_temp);
		mTvHum = (TextView)findViewById(R.id.tv_hum);
		mTvIll = (TextView)findViewById(R.id.tv_ill);
		mIvPhoto = (ImageView)findViewById(R.id.imageView);
        mNoTXT = (TextView) findViewById(R.id.noImgTXT);
	}

    //- 위젯 클릭 시 처리 ------------------------------------------------------------------------
	public void clickFunc(View v) {
		switch(v.getId()) 
		{
			case R.id.wifiBTN:
				 if(mWIFIService != null) mWIFIService.stop();
                 mNoTXT.setVisibility(View.VISIBLE);
                 mIvPhoto.setVisibility(View.GONE);

				 if(mDlgWifi != null) {
						mDlgWifi.show();
			  	 } else {
						mDlgWifi = dialogIpPort();
						mDlgWifi.show();
				 }
				 break;

			case R.id.onButton:
				 ((Button)v).setTextColor((bRedOn == false) ? Color.RED : Color.BLACK);
			     sendMessage((bRedOn == false) ? "@RED,1#" : "@RED,0#");
				 bRedOn = !bRedOn;
				 break;

			case R.id.offButton:
				 ((Button)v).setTextColor((bGreenOn == false) ? Color.GREEN : Color.BLACK);
			     sendMessage((bGreenOn == false) ? "@GED,1#" : "@GED,0#");
				 bGreenOn = !bGreenOn;
				 break;

			case R.id.clearButton:
				 ((Button)v).setTextColor((bYellowOn == false) ? Color.YELLOW : Color.BLACK);
			     sendMessage((bYellowOn == false) ? "@YED,1#" : "@YED,0#");
				 bYellowOn = !bYellowOn;
				 break;

			case R.id.white_btn:
				 ((Button)v).setTextColor((bWhiteOn == false) ? Color.WHITE : Color.BLACK);
			     sendMessage((bWhiteOn == false) ? "@LED,1#" : "@LED,0#");
				 bWhiteOn = !bWhiteOn;
				 break;

			case R.id.tg_btn_buzzer:
				 if(((ToggleButton)v).isChecked())
					sendMessage("@BUZ,1#");
				 else
					sendMessage("@BUZ,0#");
				 break;

			case R.id.btn_capture:
				 sendMessage("@SHT#");
				 break;
		}
	}


	//-----------------------------------------------------------------------------------------
	//- Member Method : WiFi 통신 제어 관련 메소드
	//-----------------------------------------------------------------------------------------
    private void sendMessage(String message) {
    	if(null == mWIFIService)
            return;
    	else if (mWIFIService.getState() != mWIFIService.STATE_CONNECTED)
            return;

        if (message.length() > 0) {
            byte[] send = message.getBytes();
            mTXHandler.obtainMessage(Constant.MESSAGE_WRITE, send.length, -1, send).sendToTarget();
        }
    }

    private void setResultValue(String readMessage) {
    	String cmd = readMessage.substring(0, 3);
    	String data = readMessage.substring(4);

    	if("TMP".equals(cmd))
    		mTvTemp.setText(data+"℃");
		else if("HUM".equals(cmd))
			mTvHum.setText(data+"%");
		else if("ILL".equals(cmd))
			mTvIll.setText(data+"Lux");
    }


	//- Member Method : WiFi 통신 접속 제어 메소드 -------------------------------------------------
	private AlertDialog dialogIpPort() {
		final View dialogView = getLayoutInflater().inflate(R.layout.dialog_wifi, null);
		mEtHost = (EditText)dialogView.findViewById(R.id.et_host_ip);
		//mEtPort = (EditText)dialogView.findViewById(R.id.et_host_port);

		AlertDialog.Builder ab = new AlertDialog.Builder(this);
		ab.setTitle(R.string.wifi_dialog_title);
		ab.setView(dialogView);
		ab.setNeutralButton(R.string.wifi_dialog_connect, new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				String host = mEtHost.getText().toString();

				Matcher matcher = PARTIAl_IP_ADDRESS.matcher(host);
				if(!("".equals(host)) && matcher.matches()) {
					Log.i(TAG, "matches()~! : " + host);
					mWIFIService = new WIFIService(mContext, mHandler, host, Constant.SERVER_PORT);
					mWIFIService.start();
				} else {
					Toast.makeText(mContext, "Invalid host address.", Toast.LENGTH_SHORT).show();
				}
				mDlgWifi.dismiss();
			}
		});
		
		return ab.create();
	}



	private void showProgressDialog(String szType){

		Log.i(TAG,"showProgressDialog()");

		mProgress = new ProgressDialog(this, R.style.ProgressDialog);
		mProgress.setCancelable(false);
		mProgress.setMessage(" [ " +szType+" ] " +mAppRes.getString(R.string.title_connecting));
		mProgress.show();
	}


	//-----------------------------------------------------------------------------------------
		//- Member Method : WiFi통신 Thread & UI 연동 제어 핸들러
		//-----------------------------------------------------------------------------------------
		private final Handler mTXHandler = new Handler() {

			private ProgressDialog mDialog;

			@Override
			public void handleMessage(Message msg) {
				switch(msg.what)
				{

					case Constant.MESSAGE_WRITE:
						byte[] readBuf = (byte[]) msg.obj;
						//String readMessage = new String(readBuf, 0, msg.arg1);
						//Toast.makeText(WIFIControlActivity.this, "[TXHandler] readMessage :"+readMessage, Toast.LENGTH_SHORT).show();
						mWIFIService.write(readBuf);
						break;
				}
			};
    };

	private final Handler mHandler = new Handler() {

		   private ProgressDialog mDialog;

	        @Override
	        public void handleMessage(Message msg) {
	            switch (msg.what)
				{
		            case Constant.MESSAGE_STATE_CHANGE:
		                 if(mDialog != null && mDialog.isShowing()) mDialog.dismiss();
		                 switch (msg.arg1)
						 {
							 case WIFIService.STATE_CONNECTED:
								  mWiFiBTN.setSelected(true);
								  if(mProgress != null) mProgress.dismiss();
								  break;

							 case WIFIService.STATE_CONNECTING:
								  mWiFiBTN.setSelected(false);
								  showProgressDialog("WiFi");
								  break;

							 case WIFIService.STATE_NONE:
								  mWiFiBTN.setSelected(false);
								  break;
		                 }
		                 break;


		            case Constant.MESSAGE_READ:
		                 byte[] readBuf = (byte[]) msg.obj;

		                 Log.d(TAG,"image length="+msg.arg1);
		                 if(msg.arg2 == 1)
		                 {	// Image
		                	int offsetHead = 4;
		        			BitmapFactory.Options options = new BitmapFactory.Options();
		        			options.inSampleSize = 1;
		        			Bitmap bm = BitmapFactory.decodeByteArray(readBuf, offsetHead, msg.arg1 - offsetHead, options);
		        			mNoTXT.setVisibility(View.GONE);
		        			mIvPhoto.setVisibility(View.VISIBLE);
		        			mIvPhoto.setImageBitmap(bm);
		        			mDialog.dismiss();
		        			
		                 }else{	// Other
			                String readMessage = new String(readBuf, 0, msg.arg1);
			                Log.i(TAG,"readMessage :"+readMessage);
			                setResultValue(readMessage);
		                 }
		                 break;

		            case Constant.MESSAGE_TOAST:
		                 Toast.makeText(getApplicationContext(), msg.getData().getString(Constant.TOAST),  Toast.LENGTH_SHORT).show();
		                 break;

		            case Constant.MESSAGE_START_IMAGE:
						 mDialog = new ProgressDialog(WIFIControlActivity.this);
						 mDialog.setMessage("Please wait while image loading...");
						 mDialog.setCanceledOnTouchOutside(false);
						 mDialog.show();
		            	 break;
	            }
	        }
	    };
}
