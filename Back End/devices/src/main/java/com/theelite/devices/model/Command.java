package com.theelite.devices.model;

import lombok.*;

import java.io.Serializable;

@AllArgsConstructor
@Getter
@Setter
@NoArgsConstructor
@Data
public class Command implements Serializable {
    private String targetDevice;
    private String commandRequest;
    private long requestFlag;
    private long commandDelayMS;
    private TimeOfPublish timeOfPublish;
}
