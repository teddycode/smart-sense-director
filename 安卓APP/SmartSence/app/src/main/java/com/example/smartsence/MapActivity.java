package com.example.smartsence;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;

import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.MapStatus;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationConfiguration;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.model.LatLng;

import java.util.List;

public class MapActivity extends Activity {

    private LocationManager locationManager;
    private String provider;
    MapView mapView;
    BaiduMap baiduMap;
    boolean ifFrist = true;

    @Nullable
    @Override
    public View onCreateView(@NonNull String name, @NonNull Context context, @NonNull AttributeSet attrs) {
        return super.onCreateView(name, context, attrs);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_map);

        Initialzie();

        UpdateUI();

    }

    private void UpdateUI()
    {

        Intent intent = getIntent();
        String position = intent.getStringExtra("position");  //lat:%f,lng:%f
        if (position.isEmpty()) {
            Toast.makeText(this,"位置为空！",Toast.LENGTH_SHORT).show();
            return;
        } else {
            int s,m,e;
            s=position.indexOf(":");
            m=position.indexOf(",");
            e=position.lastIndexOf(":");
            String str_lat=position.substring(s+1,m);
            String str_lng=position.substring(e+1,position.length());
            if(checkItude(str_lng,str_lat)==false) {
                Toast.makeText(this,"输入值不合法！",Toast.LENGTH_LONG).show();
                return;
            }
            LatLng latLng = new LatLng(Double.valueOf(str_lat),Double.valueOf(str_lng));
            MapStatus.Builder builder = new MapStatus.Builder();
            builder.target(latLng).zoom(18.0f);
            baiduMap.animateMapStatus(MapStatusUpdateFactory.newMapStatus(builder.build()));

            BitmapDescriptor searchBdA = BitmapDescriptorFactory.fromResource(R.drawable.icon_gcoding);
            MarkerOptions option = new MarkerOptions().icon(searchBdA).position(latLng);
            baiduMap.addOverlay(option);
        }
    }

    private void Initialzie() {
        mapView = (MapView) findViewById(R.id.bmapView);
        // 获取baiduMap对象
        baiduMap = mapView.getMap();
        // 设置可改变地图位置
        baiduMap.setMyLocationEnabled(true);
        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        List<String> list = locationManager.getProviders(true);
        if (list.contains(LocationManager.GPS_PROVIDER)) {
            provider = LocationManager.GPS_PROVIDER;
        } else if (list.contains(LocationManager.NETWORK_PROVIDER)) {
            provider = LocationManager.NETWORK_PROVIDER;

        } else {
            Toast.makeText(this, "当前不能提供位置信息", Toast.LENGTH_LONG).show();
            return;
        }
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        Location location = locationManager.getLastKnownLocation(provider);
        if (location != null) {
            navigateTo(location);
        }
        locationManager.requestLocationUpdates(provider, 5000, 1,
                locationListener);

    }

    private void navigateTo(Location location) {
        // 按照经纬度确定地图位置
        if (ifFrist) {
            LatLng ll = new LatLng(location.getLatitude(),
                    location.getLongitude());
            MapStatusUpdate update = MapStatusUpdateFactory.newLatLng(ll);
            // 移动到某经纬度
            baiduMap.animateMapStatus(update);
            update = MapStatusUpdateFactory.zoomBy(5f);
            // 放大
            baiduMap.animateMapStatus(update);

            ifFrist = false;
        }
        // 显示个人位置图标
        MyLocationData.Builder builder = new MyLocationData.Builder();
        builder.latitude(location.getLatitude());
        builder.longitude(location.getLongitude());
        MyLocationData data = builder.build();
        baiduMap.setMyLocationData(data);
    }

    LocationListener locationListener = new LocationListener() {

        @Override
        public void onStatusChanged(String arg0, int arg1, Bundle arg2) {
            // TODO Auto-generated method stub

        }

        @Override
        public void onProviderEnabled(String arg0) {
            // TODO Auto-generated method stub

        }

        @Override
        public void onProviderDisabled(String arg0) {
            // TODO Auto-generated method stub

        }

        @Override
        public void onLocationChanged(Location arg0) {
            // TODO Auto-generated method stub
            // 位置改变则重新定位并显示地图
            navigateTo(arg0);
        }
    };

    public static boolean checkItude(String longitude,String latitude){
        String reglo = "((?:[0-9]|[1-9][0-9]|1[0-7][0-9])\\.([0-9]{0,6}))|((?:180)\\.([0]{0,6}))";
        String regla = "((?:[0-9]|[1-8][0-9])\\.([0-9]{0,6}))|((?:90)\\.([0]{0,6}))";
        longitude = longitude.trim();
        latitude = latitude.trim();
        return longitude.matches(reglo)==true?latitude.matches(regla):false;
    }

    @Override
    protected void onResume() {
        super.onResume();
        mapView.onResume();
    }
    @Override
    protected void onPause() {
        super.onPause();
        mapView.onPause();
    }
    @Override
    protected void onDestroy() {
        mapView.onDestroy();
        mapView = null;
        super.onDestroy();
    }

}


