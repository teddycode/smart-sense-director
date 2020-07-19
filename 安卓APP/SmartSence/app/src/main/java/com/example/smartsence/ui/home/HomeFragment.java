package com.example.smartsence.ui.home;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;

import com.example.smartsence.MainActivity;
import com.example.smartsence.MapActivity;
import com.example.smartsence.R;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class HomeFragment extends Fragment {

    private MainActivity  This = null;

    private HomeViewModel homeViewModel;

    private EditText edPushArea;
    private Button   btnBtConn;
    private Button   btnPushMsg;
    private  Button btnShowMap;



    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        homeViewModel = ViewModelProviders.of(getActivity()).get(HomeViewModel.class);
        View root = inflater.inflate(R.layout.fragment_home, container, false);

        edPushArea=root.findViewById(R.id.edPush);
        btnBtConn=root.findViewById(R.id.btnBtConn);
        btnPushMsg=root.findViewById(R.id.btnMsgDeliver);
        btnShowMap=root.findViewById(R.id.btnShowMap);

        //设置监听
        homeViewModel.getBtnBtconn().observe(getViewLifecycleOwner(), new Observer<String>() {
            @Override
            public void onChanged(@Nullable String s) {
                btnBtConn.setText(s);
            }
        });
        homeViewModel.getInAreaText().observe(getViewLifecycleOwner(), new Observer<String>() {
            @Override
            public void onChanged(@Nullable String string) {
                edPushArea.setText(string);
            }
        });

        return root;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        This = (MainActivity) getActivity();

        InitListeners();

    }

    private void InitListeners() {
        btnBtConn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String value = homeViewModel.getBtnBtconn().getValue();
                if (value.equals("蓝牙连接")) {
                    This.Bt_connect();

                } else {

                    This.Bt_disconn();

                }
            }
        });

        // 发送景区消息
        btnPushMsg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String text = edPushArea.getText().toString();
                byte[] bytes = new byte[4096];
                ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream(4096);

                Integer len = text.length() * 34 + 5;
                byte[] lenByte = unsignedShortToByte2(len);

                try {
                    byteArrayOutputStream.write(new byte[]{0x5a, 0x5a, 0x15, lenByte[0], lenByte[1]});
                } catch (IOException e) {
                    e.printStackTrace();
                }
                for (int i = 0; i < text.length(); i++) {
                    String str = String.valueOf(text.charAt(i));
                    try {
                        byteArrayOutputStream.write(str.getBytes("GB2312"));
                        byteArrayOutputStream.write(This.hanZiKu.resolveString(str));
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                This.Bt_send(byteArrayOutputStream.toByteArray());
                homeViewModel.setInAreaText(text);
                Toast.makeText(This, "发送成功", Toast.LENGTH_SHORT).show();
            }
        });

        btnShowMap.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(This, MapActivity.class);
                intent.putExtra("position","");
                This.startActivityForResult(intent, This.REQUEST_SHOW_LOCATION);  //设置返回宏定义
            }
        });

    }

    public static byte[] unsignedShortToByte2(Integer s) {
        byte[] targets = new byte[2];
        targets[0] = (byte) (s >> 8 & 0xFF);
        targets[1] = (byte) (s & 0xFF);
        return targets;
    }
}
