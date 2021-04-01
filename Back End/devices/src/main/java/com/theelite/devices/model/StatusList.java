package com.theelite.devices.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class StatusList {
    private boolean gotWifiIP;
    private boolean camInit;
    private boolean sdcardInit;
    private boolean camStreamInit;
    private boolean doorLocked;
    private boolean doorClosed;
    private int batteryLevel;
}
