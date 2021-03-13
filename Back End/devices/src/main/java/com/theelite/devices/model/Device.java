package com.theelite.devices.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.data.annotation.Id;

import java.util.UUID;

@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
public class Device {

    @Id
    private String deviceId;
    private String deviceName;
    private DeviceType type;

    //@JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    //private UUID accountId;

}
