package com.example.intelligentsmokealarmsystem.dao;

public class SysData {
    private int s_id;


    private int temp;
    private int humi;
    private int somg;

    private String createDateTime;


    @Override
    public String toString() {
        return "SysData{" +
                "s_id=" + s_id +
                ", temp=" + temp +
                ", humi=" + humi +
                ", somg=" + somg +
                ", createDateTime='" + createDateTime + '\'' +
                '}';
    }

    public String getCreateDateTime() {
        return createDateTime;
    }

    public void setCreateDateTime(String createDateTime) {
        this.createDateTime = createDateTime;
    }

    public int getS_id() {
        return s_id;
    }

    public void setS_id(int s_id) {
        this.s_id = s_id;
    }

    public int getTemp() {
        return temp;
    }

    public void setTemp(int temp) {
        this.temp = temp;
    }

    public int getHumi() {
        return humi;
    }

    public void setHumi(int humi) {
        this.humi = humi;
    }

    public int getSomg() {
        return somg;
    }

    public void setSomg(int somg) {
        this.somg = somg;
    }
}
