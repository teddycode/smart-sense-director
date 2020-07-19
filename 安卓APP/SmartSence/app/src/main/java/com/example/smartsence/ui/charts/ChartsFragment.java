package com.example.smartsence.ui.charts;

import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;


import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModel;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProviders;

import com.example.smartsence.MainActivity;
import com.example.smartsence.R;

public class ChartsFragment extends Fragment implements View.OnClickListener {

    public static final String[] SPOTS={"伴月亭","世纪宝鼎","桂林动物园","三将军及八百将士墓","七星欢乐谷","敬请期待"};
    private ChartsViewModel chartsViewModel;

    private MainActivity This;

    private ImageView imageView1;
    private ImageView imageView2;
    private ImageView imageView3;
    private ImageView imageView4;
    private ImageView imageView5;
    private ImageView imageView6;

    private TextView[] tvDianzans = new TextView[5];

    private EditText edSelected;
    private Button btnSend;

    public View onCreateView(@NonNull final LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_chart, container, false);

        edSelected =root.findViewById(R.id.edSelect);
        btnSend=root.findViewById(R.id.btnSendSpot);
        imageView1 = root.findViewById(R.id.imageView1);
        imageView2 = root.findViewById(R.id.imageView2);
        imageView3 = root.findViewById(R.id.imageView3);
        imageView4 = root.findViewById(R.id.imageView4);
        imageView5 = root.findViewById(R.id.imageView5);
        imageView6 = root.findViewById(R.id.imageView6);

        tvDianzans[0] = root.findViewById(R.id.tvDianzan1);
        tvDianzans[1] = root.findViewById(R.id.tvDianzan2);
        tvDianzans[2] = root.findViewById(R.id.tvDianzan3);
        tvDianzans[3] = root.findViewById(R.id.tvDianzan4);
        tvDianzans[4] = root.findViewById(R.id.tvDianzan5);

        chartsViewModel = ViewModelProviders.of(getActivity()).get(ChartsViewModel.class);

        chartsViewModel.getSelected().observe(getViewLifecycleOwner(), new Observer<Integer>() {
            @Override
            public void onChanged(Integer integer) {
                if(integer>=0 && integer<6)
                edSelected.setText(SPOTS[integer]);
            }
        });

        chartsViewModel.getDianzan().observe(getViewLifecycleOwner(), new Observer<Integer[]>() {
            @Override
            public void onChanged(Integer[] integers) {
                for(int i=0;i<5;++i)
                {
                    tvDianzans[i].setText("点赞数："+integers[i]);
                }
            }
        });
        return root;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        This = (MainActivity) getActivity();
        InitUI();

    }

    private  void InitUI(){

        imageView1.setOnClickListener(this);
        imageView2.setOnClickListener(this);
        imageView3.setOnClickListener(this);
        imageView4.setOnClickListener(this);
        imageView5.setOnClickListener(this);
        imageView6.setOnClickListener(this);

        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Integer integer=chartsViewModel.getSelected().getValue();
                byte[] bytes = new byte[]{0x5a,0x5a,0x45,0x00,0x06,integer.byteValue()};
                This.Bt_send(bytes);
                Toast.makeText(This,"发送成功！",Toast.LENGTH_SHORT);
            }
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.imageView1:
                chartsViewModel.setSelected(0);
                Toast.makeText(This,"images1 clicked!",Toast.LENGTH_SHORT).show();
                break;
            case R.id.imageView2:
                chartsViewModel.setSelected(1);
                Toast.makeText(This,"images2 clicked!",Toast.LENGTH_SHORT).show();
                break;
            case R.id.imageView3:
                chartsViewModel.setSelected(2);
                Toast.makeText(This,"images3 clicked!",Toast.LENGTH_SHORT).show();
                break;
            case R.id.imageView4:
                chartsViewModel.setSelected(3);
                Toast.makeText(This,"images4 clicked!",Toast.LENGTH_SHORT).show();
                break;
            case R.id.imageView5:
                chartsViewModel.setSelected(4);
                Toast.makeText(This,"images5 clicked!",Toast.LENGTH_SHORT).show();
                break;
            case R.id.imageView6:
                chartsViewModel.setSelected(5);
                Toast.makeText(This,"images6 clicked!",Toast.LENGTH_SHORT).show();
                break;
        }
    }
}

