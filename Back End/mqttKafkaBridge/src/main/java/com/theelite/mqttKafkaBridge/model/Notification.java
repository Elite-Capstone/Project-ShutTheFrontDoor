package com.theelite.mqttKafkaBridge.model;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.Date;
import java.util.UUID;

@Getter
@Setter
@NoArgsConstructor
public class Notification {
    private String notification;
    private Date date;
    private UUID doorId;
    private UUID accountId;

    public Notification(String notification, String doorId) {
        this.date = new Date();
        this.notification = notification;
        this.doorId = UUID.fromString(doorId);
    }
}
