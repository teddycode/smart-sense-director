package com.example.smartsence;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.baidu.mapapi.SDKInitializer;
import com.example.smartsence.R;
import com.example.smartsence.ui.charts.ChartsViewModel;
import com.example.smartsence.ui.home.HomeViewModel;
import com.example.smartsence.utils.HanZiKu;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.gson.Gson;
import com.kongzue.dialog.v2.DialogSettings;
import com.kongzue.dialog.v2.SelectDialog;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProviders;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.NavigationUI;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final String TAG="MainActivity";
    public final static int REQUEST_CONNECT_DEVICE = 2;    //宏定义查询设备句柄
    public final static int REQUEST_SHOW_LOCATION = 3;    //宏定义查询设备句柄

    public static boolean mIsBtConnected=false;

    public static String mBtAddress= "";
    public static BluetoothSocket mBtSocket;
    public static BluetoothDevice mBtDevice;

    public HanZiKu hanZiKu;

    private boolean _isBtOpened;;

    private final static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";   //S串口传输服务UUID号

    private InputStream is;         //输入流，用来接收蓝牙数据
    private String smsg = "";        //显示用数据缓存
    private String fmsg = "";        //保存用数据缓存
    private int msgSize = 0;        // 数据量

    BluetoothDevice _device = null;      //蓝牙设备
    BluetoothSocket _socket = null;      //蓝牙通信socket
    boolean _discoveryFinished = false;
    boolean bRun = true;
    boolean bThread = false;

    private BluetoothAdapter _bluetooth = BluetoothAdapter.getDefaultAdapter();    //获取本地蓝牙适配器，即蓝牙设备

    private HomeViewModel homeViewModel;
    private ChartsViewModel chartsViewModel;

    @Nullable
    @Override
    public View onCreateView(@NonNull String name, @NonNull Context context, @NonNull AttributeSet attrs) {
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT > 8)
        {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder()
                    .permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }
        return super.onCreateView(name, context, attrs);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        homeViewModel = ViewModelProviders.of(this).get(HomeViewModel.class);
        chartsViewModel = ViewModelProviders.of(this).get(ChartsViewModel.class);

        onRecoverData();

        // 权限检测
        final int MY_PERMISSION_ACCESS_COARSE_LOCATION = 11;
        final int MY_PERMISSION_ACCESS_FINE_LOCATION = 12;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if(this.checkSelfPermission(android.Manifest.permission.ACCESS_COARSE_LOCATION)!= PackageManager.PERMISSION_GRANTED){
                requestPermissions(new String[]{android.Manifest.permission.ACCESS_COARSE_LOCATION},MY_PERMISSION_ACCESS_COARSE_LOCATION);
                Log.e("11111","ACCESS_COARSE_LOCATION");
            }
            if(this.checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION)!= PackageManager.PERMISSION_GRANTED){
                requestPermissions(new String[]{android.Manifest.permission.ACCESS_FINE_LOCATION},MY_PERMISSION_ACCESS_FINE_LOCATION);
                Log.e("11111","ACCESS_FINE_LOCATION");
            }
        }

        // 配置导航栏
        BottomNavigationView navView = findViewById(R.id.nav_view);
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupWithNavController(navView, navController);

        //蓝牙打开失败，提示信息
        if (_bluetooth == null) {
            _isBtOpened = false;
            Toast.makeText(this, "无法打开手机蓝牙，请确认手机是否有蓝牙功能！", Toast.LENGTH_LONG).show();
        } else {
            _isBtOpened = true;
        }

        // 设置设备可以被搜索
        new Thread() {
            public void run() {
                if (_bluetooth.isEnabled() == false) {
                    _bluetooth.enable();
                }
            }
        }.start();

        // 汉字库
        hanZiKu = new HanZiKu(getApplicationContext());

        SDKInitializer.initialize(getApplicationContext());
    }

    public void Bt_send(byte[] bytes){
        int i = 0;
        int n = 0;
        if (_socket == null) {
            Toast.makeText(this, "请先连接蓝牙模块", Toast.LENGTH_SHORT).show();
            return;
        }
        if (bytes.length == 0) {
            Toast.makeText(this, "请先输入数据", Toast.LENGTH_SHORT).show();
            return;
        }
        try {
            OutputStream os = _socket.getOutputStream();   //蓝牙连接输出流
//            for (i = 0; i < bytes.length; i++) {
//                if (bytes[i] == 0x0a) n++;
//            }
//            byte[] bytes_new = new byte[bytes.length + n];
//            n = 0;
//            for (i = 0; i < bytes.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
//                if (bytes[i] == 0x0a) {
//                    bytes_new[n] = 0x0d;
//                    n++;
//                    bytes_new[n] = 0x0a;
//                } else {
//                    bytes_new[n] = bytes[i];
//                }
//                n++;
//            }
            os.write(bytes);
        } catch (IOException e) {
        }
    }

    public void Bt_connect(){
        if(_bluetooth.isEnabled()==false){  //如果蓝牙服务不可用则提示
            Toast.makeText(this, " 打开蓝牙中...", Toast.LENGTH_LONG).show();
            return;
        }
        //如未连接设备则打开DeviceListActivity进行设备搜索
        if(_socket==null){
            Log.d("CALLBACK",this.toString());
            Intent serverIntent = new Intent(this, BtListActivity.class); //跳转程序设置
            this.startActivityForResult(serverIntent, this.REQUEST_CONNECT_DEVICE);  //设置返回宏定义
        }
    }

    public void Bt_disconn(){
        //关闭连接socket
        try{
            bRun = false;
            Thread.sleep(1000);
            is.close();
            if(_socket!=null) _socket.close();
            _socket = null;
            homeViewModel.setBtnBtconn("蓝牙连接");
        }catch(IOException e){}
        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    //activity回调消息处理
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Log.d("CALLBACK", "MainActivity has been called! request code:" + requestCode + "| result code: " + resultCode + " | data:" + data);
//        Toast.makeText(this,"Get result form intent, code is: "+resultCode,Toast.LENGTH_LONG);
        switch (requestCode) {
            case REQUEST_CONNECT_DEVICE:     //蓝牙连接地址，由btListActivity设置返回
                if (resultCode == RESULT_OK) {
                    mBtAddress = data.getStringExtra("msg_BtAddress");
                    mIsBtConnected = true;
                    BtStartConn();
                } else {
                    mBtAddress = "";
                }
                break;
        }
    }

    private void BtStartConn(){
        String address = this.mBtAddress;
        if (!address.isEmpty()){
            // 得到蓝牙设备句柄
            _device = _bluetooth.getRemoteDevice(address);
            // 用服务号得到socket
            try{
                _socket = _device.createRfcommSocketToServiceRecord(UUID.fromString(MY_UUID));
            }catch(IOException e){
                Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
            }
            //连接socket
            try{
                _socket.connect();
                Toast.makeText(this, "连接"+_device.getName()+"成功！", Toast.LENGTH_SHORT).show();
                homeViewModel.setBtnBtconn("蓝牙断开");
                this.mBtSocket = _socket;
                this.mBtDevice = _device;
            }catch(IOException e){
                try{
                    Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                    _socket.close();
                    _socket = null;
                }catch(IOException ee){
                    Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                }
                return;
            }

            //打开接收线程
            try{
                is = _socket.getInputStream();   //得到蓝牙数据输入流
            }catch(IOException e){
                Toast.makeText(this, "接收数据失败！", Toast.LENGTH_SHORT).show();
                return;
            }
            if(bThread==false){
                readThread.start();
                bThread=true;
            }else{
                bRun = true;
            }
        }
    }
    //接收数据线程
    Thread readThread=new Thread(){
        public void run(){
            int num = 0;
            byte[] buffer = new byte[1024];
            byte[] buffer_new = new byte[1024];
            int i = 0;
            int n = 0;
            bRun = true;
            //接收线程
            while(true){
                try{
                    while(is.available()==0){
                        while(bRun == false){}
                    }
                    while(true){
                        if(!bThread)//跳出循环
                            return;

                        num = is.read(buffer);         //读入数据
                        n=0;

                        String s0 = new String(buffer,0,num);
                        fmsg+=s0;    //保存收到数据
                        for(i=0;i<num;i++){
                            if((buffer[i] == 0x0d)&&(buffer[i+1]==0x0a)){
                                buffer_new[n] = 0x0a;
                                i++;
                            }else{
                                buffer_new[n] = buffer[i];
                            }
                            n++;
                        }
                        String s = new String(buffer_new,0,n);
                        smsg+=s;   //写入接收缓存
                        if(is.available()==0)break;  //短时间没有数据才跳出进行显示
                    }
                    //发送显示消息，进行显示刷新
                    handler.sendMessage(handler.obtainMessage());
                }catch(IOException e){
                }
            }
        }
    };

    //消息处理队列
    Handler handler= new Handler(){
        public void handleMessage(Message msg){
            super.handleMessage(msg);
            msgSize++;
            Log.d("REV", fmsg);
            if(fmsg.contains("*")&&fmsg.contains("#"))  // 处理一帧数据
            {
                processMessages(fmsg.substring(0,fmsg.indexOf("#")+1));
                fmsg = fmsg.substring(fmsg.indexOf("#")+1);
            }
            if(msgSize > 200) {
                msgSize=0;
                smsg="";
                fmsg="";
            }
        }
    };

    //解析
    private void processMessages(String msg){
       if(msg.length()<4) return;
       if(msg.charAt(0)=='*' && msg.charAt(msg.length()-1)=='#')
       {
           switch (msg.charAt(1))
           {
               case '1' :
               final String str = msg.substring(2,msg.length()-2);
               final MainActivity mainActivity = this;
               DialogSettings.style = DialogSettings.STYLE_IOS;
               SelectDialog.show(this, "报警信息", String.format("用户已报警，所在位置为：%s,是否要在地图中显该位置？",str), "确定", new DialogInterface.OnClickListener() {
                   @Override
                   public void onClick(DialogInterface dialog, int which) {
                       Intent intent = new Intent(mainActivity, MapActivity.class);
                       intent.putExtra("position",str);
                       startActivityForResult(intent, REQUEST_SHOW_LOCATION);  //设置返回宏定义
                       dialog.dismiss();
                   }
               }, "取消", new DialogInterface.OnClickListener() {
                   @Override
                   public void onClick(DialogInterface dialog, int which) {
                       dialog.dismiss();
                   }
               });

               break; // 报警消息

               case '2' :
               if(msg.charAt(2)>='0' && msg.charAt(2)<'5' ){
                   chartsViewModel.updateDianzan(msg.charAt(2)-'0');
               }
               break; // 点赞消息
           }
       }
    }

    @Override
    //关闭程序掉用处理部分
    public void onDestroy(){
        super.onDestroy();
        if(_socket!=null)  //关闭连接socket
            try{
                _socket.close();
            }catch(IOException e){}
        //	_bluetooth.disable();  //关闭蓝牙服务
        onSaveData();
    }

    //数据恢复
    private  void onRecoverData()
    {
        SharedPreferences pref = getSharedPreferences("data",MODE_PRIVATE);
        for (int i=0;i<5;++i)
        {
            chartsViewModel.setDianzan(i,pref.getInt(String.valueOf(i),0));
        }

    }
    // 数据持久化
    private void  onSaveData()
    {
        SharedPreferences.Editor editor = getSharedPreferences("data",MODE_PRIVATE).edit();
        Integer[] integer = chartsViewModel.getDianzan().getValue();
        for(int i=0;i<5;++i)
        {
            editor.putInt(String.valueOf(i),integer[i]);
        }
        editor.apply();
    }

}
