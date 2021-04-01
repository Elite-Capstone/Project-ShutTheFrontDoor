package com.theelite.devices.model;

import lombok.*;

@AllArgsConstructor
@Getter
@Setter
@NoArgsConstructor
@Data
public class Command {
    private String targetDevice;
    private String commandRequest;
    private long requestFlag;
    private long commandDelayMS;
    private TimeOfRequest timeOfRequest;
}
