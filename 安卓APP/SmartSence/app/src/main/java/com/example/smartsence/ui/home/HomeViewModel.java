package com.example.smartsence.ui.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {

    // 输入框记录
    private MutableLiveData<String> inAreaText = new MutableLiveData<>();
    private MutableLiveData<String> btnBtconn = new MutableLiveData<>();

    //设置默认值
    public HomeViewModel(){
        btnBtconn.setValue("蓝牙连接");
    }

    public MutableLiveData<String> getInAreaText()  {
        return inAreaText;
    }
    public void setInAreaText(String text) {
        this.inAreaText.setValue(text);
    }

    public MutableLiveData<String> getBtnBtconn() {
        return btnBtconn;
    }

    public void setBtnBtconn(String string) {
        this.btnBtconn .setValue(string);
    }
}