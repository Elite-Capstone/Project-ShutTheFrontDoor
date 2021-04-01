package com.theelite.devices.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.data.annotation.Id;

@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
public class Device {

    @Id
    private String deviceId;
    private String deviceName;
    private DeviceType type;
    private Status status;
}
