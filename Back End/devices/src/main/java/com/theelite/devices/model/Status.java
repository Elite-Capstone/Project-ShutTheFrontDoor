package com.theelite.devices.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@AllArgsConstructor
@NoArgsConstructor
@Setter
@Getter
public class Status {
    private TimeOfPublish timeOfPublish;
    private StatusList statusList;
}
