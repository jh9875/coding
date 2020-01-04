package com.rnu.arduberry;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.os.Bundle;
import android.app.Activity;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

public class IntroActivity extends Activity {
    //------------------------------------------------------------------------
    //- Member Variable
    //------------------------------------------------------------------------
    private static final String TAG = "ArduBerry_APP";
    private static final boolean D = true;

    public static final int 	REQUEST_PEERMISSION_FINE 	= 1001;

    //- Permission 체크 ------------------------------------------------------
    private int     			mPermissionCheck = -1;


    //- UI Widget ------------------------------------------------------------
    private Resources           mAppRes;


    //------------------------------------------------------------------------
    //- Member Method : Activity's Override Method
    //------------------------------------------------------------------------
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.intro);

        mAppRes     = this.getResources();
        requestPermission(Manifest.permission.ACCESS_FINE_LOCATION, REQUEST_PEERMISSION_FINE);
    }

    //------------------------------------------------------------------------
    //- Member Method : Custom Method
    //------------------------------------------------------------------------
    public void moveFunc(View v) {

        switch(v.getId())
        {
            case R.id.goBtBTN:
                 startActivity(new Intent(this,
                               BTControlActivity.class));
                break;

            case R.id.goWiFiBTN:
                 startActivity(new Intent(this,
                               WIFIControlActivity.class));
                break;

                case R.id.goSwitch:
           startActivity(new Intent(this,
                   SWITCHControlActivity.class));
            break;

            case R.id.goPotentionmeter:
                startActivity(new Intent(this,
                        POTControlActivity.class));
                break;
            case R.id.goPir:
                startActivity(new Intent(this,
                        PIRControlActivity.class));
                break;

            case R.id.goFnd:
                startActivity(new Intent(this,
                        FNDControlActivity.class));
                break;
            case R.id.goMoter:
                startActivity(new Intent(this,
                        MOTERControlActivity.class));
                break;

            case R.id.goLCD:
                startActivity(new Intent(this,
                        LCDControlActivity.class));
                break;

            case R.id.goBuzzer:
                startActivity(new Intent(this,
                        BUZZERControlActivity.class));
                break;
            case R.id.goQ1:
                startActivity(new Intent(this,
                        Q1ControlActivity.class));
                break;
            case R.id.goQ2:
                startActivity(new Intent(this,
                        Q2ControlActivity.class));
                break;
            case R.id.goQ3:
                startActivity(new Intent(this,
                        Q3ControlActivity.class));
                break;


        }
    }


    //- 권한 허용 여부 사용자 설정에 대한 처리 -----------------------------------------------------------------------------------
    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        switch(requestCode)
        {
            case REQUEST_PEERMISSION_FINE:
                if(grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    //-권한 허용
                    Log.i(TAG, mAppRes.getString(R.string.permission_granted));
                }else{
                    //-허용 불가
                    Toast.makeText(this, R.string.permission_denied, Toast.LENGTH_LONG).show();
                    finish();
                }
                break;
        }
    }

    //- Bluetooth 기기 검색이 가능하도록 권한 허용 요청 ---------------------------------------------------------------------
    private void requestPermission(String szPermission, int reqCode){

        //- 위치 검색 권한 허용 여부 검사
        mPermissionCheck = ContextCompat.checkSelfPermission(this, szPermission);

        if(mPermissionCheck != PackageManager.PERMISSION_GRANTED)
        {
            //- 허용 불가 된 경우 사용자가 재 확인 요청
            if(ActivityCompat.shouldShowRequestPermissionRationale(this, szPermission))
                ActivityCompat.requestPermissions(this,  new String[]{Manifest.permission.ACCESS_FINE_LOCATION,
                        Manifest.permission.ACCESS_COARSE_LOCATION}, reqCode);
            else
                //- 최초 요청
                ActivityCompat.requestPermissions(this,  new String[]{Manifest.permission.ACCESS_FINE_LOCATION,
                        Manifest.permission.ACCESS_COARSE_LOCATION}, reqCode);
        }
    }
}
