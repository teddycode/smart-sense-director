package com.example.smartsence.ui.charts;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;


public class ChartsViewModel extends ViewModel {

    Integer[] integer = new Integer[]{0,0,0,0,0};
    private MutableLiveData<Integer[] > dianzan = new MutableLiveData<>();
    private MutableLiveData<Integer> selected = new MutableLiveData<>();

    public ChartsViewModel(){
        this.dianzan.setValue(integer);
        this.selected.setValue(0);
    }

    public MutableLiveData<Integer> getSelected() {
        return selected;
    }
    public void setSelected(Integer integer) {
        this.selected.setValue(integer);
    }

    public MutableLiveData<Integer[]> getDianzan() {
        return dianzan;
    }

    public void updateDianzan(Integer pos) {
        Integer[] integers = this.dianzan.getValue();
        if(pos>=0 && pos <5) integers[pos]++;
        this.dianzan.setValue(integers);
    }

    public void setDianzan(Integer pos,Integer value)
    {
        Integer[] integers = this.dianzan.getValue();
        if(pos>=0 && pos <5) integers[pos]=value;
        this.dianzan.setValue(integers);
    }
}
