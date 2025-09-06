package com.example.intelligentsmokealarmsystem.bean;

public class Receive {
    private String temp; //环境温度
    private String humi; //环境湿度
    private String somg;//烟雾浓度
    private String temp_v;//温度阈值
    private String humi_v;//湿度阈值
    private String somg_v;//烟雾浓度阈值
    private String flage;//模式
    private String fire;//火焰
    private String beep;//蜂鸣器

    @Override
    public String toString() {
        return "DataDTO{" +
                "temp='" + temp + '\'' +
                ", humi='" + humi + '\'' +
                ", somg='" + somg + '\'' +
                ", temp_v='" + temp_v + '\'' +
                ", humi_v='" + humi_v + '\'' +
                ", somg_v='" + somg_v + '\'' +
                ", flage='" + flage + '\'' +
                ", fire='" + fire + '\'' +
                ", beep='" + beep + '\'' +
                '}';
    }

    public String getTemp() {
        return temp;
    }

    public void setTemp(String temp) {
        this.temp = temp;
    }

    public String getHumi() {
        return humi;
    }

    public void setHumi(String humi) {
        this.humi = humi;
    }

    public String getSomg() {
        return somg;
    }

    public void setSomg(String somg) {
        this.somg = somg;
    }

    public String getTemp_v() {
        return temp_v;
    }

    public void setTemp_v(String temp_v) {
        this.temp_v = temp_v;
    }

    public String getHumi_v() {
        return humi_v;
    }

    public void setHumi_v(String humi_v) {
        this.humi_v = humi_v;
    }

    public String getSomg_v() {
        return somg_v;
    }

    public void setSomg_v(String somg_v) {
        this.somg_v = somg_v;
    }

    public String getFlage() {
        return flage;
    }

    public void setFlage(String flage) {
        this.flage = flage;
    }

    public String getFire() {
        return fire;
    }

    public void setFire(String fire) {
        this.fire = fire;
    }

    public String getBeep() {
        return beep;
    }

    public void setBeep(String beep) {
        this.beep = beep;
    }
}
