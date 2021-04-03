package com.theelite.devices.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.beans.factory.annotation.Value;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@AllArgsConstructor
@NoArgsConstructor
@Setter
@Getter
@Entity
@Table(name = "device_ip")
public class DeviceIp {
    @Id
    @Value("deviceId")
    private String deviceId;
    @Value("ipAddress")
    private String ipAddress;
}
