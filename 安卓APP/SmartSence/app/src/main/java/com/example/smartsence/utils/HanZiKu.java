package com.example.smartsence.utils;

import android.content.Context;

import java.io.IOException;
import java.io.InputStream;

public  class HanZiKu {
    private final static String ENCODE = "GB2312";
    private final static String ZK16 = "HZK16";//assets下的路径
    private final static String ASC16 = "ASC16";//assets下的路径        
    private byte[][] arr;//返回的二位数组
    private int all_16_32 = 16;//16*16
    private int all_2_4 = 2;//一个汉字等于两个字节
    private int all_32_128 = 32;//汉字解析成16*16 所占字节数
    private int font_width = 8;//ascii码 8*16
    private int font_height = 16;//ascii码 8*16
    private int all_16 = 16;//ascii码解析成8*16 所占字节数
    private Context mContext;


    public HanZiKu(Context context){
        this.mContext=context;
    }
        /**
     * 解析成点阵
     * @param str
     * @return
     */
    public byte[] resolveString(String str) {
        byte[] data = null;
        int[] code = null;
        if (str.charAt(0) < 0x80) {  // 字母
            data = read_a(str.charAt(0));
        } else {
            code = getByteCode(str.substring(0, 0 + 1));
            data = read(code[0], code[1]);
        }
        return data;
    }
     /**
     * 读取字库中的ASCII 码
     */
    protected byte[] read_a(char char_num) {
        byte[] data = null;
        int ascii = (int) char_num;
        try {
            data = new byte[all_16];//定义缓存区的大小
            InputStream inputStream = mContext.getResources().getAssets()
                    .open(ASC16);//打开ascii字库的流
            int offset = ascii * 16;//ascii码在字库里的偏移量
            inputStream.skip(offset);
            inputStream.read(data, 0, all_16);//读取字库中ascii码点阵数据
            inputStream.close();
            return data;
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return data;
      }
    /**
     * 读取字库中的汉字
     * @param areaCode
     * @param posCode
     * @return
     */
    protected byte[] read(int areaCode, int posCode) {
        byte[] data = null;
        try {
            int area = areaCode - 0xa0;//区码
            int pos = posCode - 0xa0;//位码
            InputStream in = mContext.getResources().getAssets().open(ZK16);//打开中文字库的流
            long offset = all_32_128 * ((area - 1) * 94 + pos - 1);//汉字在字库里的偏移量
            in.skip(offset);//跳过偏移量
            data = new byte[all_32_128];//定义缓存区的大小
            in.read(data, 0, all_32_128);//读取该汉字的点阵数据
            in.close();
        } catch (Exception ex) {
        }
        return data;
    }
    /**
     * 获取汉字的区，位（ascii码不需要区码，位码）
     * @param str
     * @return
     */
    protected int[] getByteCode(String str) {
        int[] byteCode = new int[2];
        try {
            byte[] data = str.getBytes(ENCODE);
            byteCode[0] = data[0] < 0 ? 256 + data[0] : data[0];
            byteCode[1] = data[1] < 0 ? 256 + data[1] : data[1];
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return byteCode;
    }
}