package com.theelite.devices.model;

import lombok.*;

@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
@Data
public class TimeOfRequest {
    private long year;
    private long month;
    private long day;
    private long hour;
    private long minute;
    private long second;
}
