package com.theelite.devices.model;

import lombok.*;

import java.io.Serializable;

@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
@Data
public class TimeOfPublish implements Serializable {
    private long year;
    private long month;
    private long day;
    private long hour;
    private long minute;
    private long second;
}
