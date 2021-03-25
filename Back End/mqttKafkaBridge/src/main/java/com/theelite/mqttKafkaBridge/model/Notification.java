package com.theelite.mqttKafkaBridge.model;

import java.util.Date;
import java.util.UUID;

public class Notification {
    private String notification;
    private Date date;
    private UUID doorId;

    public Notification(String notification, String doorId) {
        this.date = new Date();
        this.notification = notification;
        this.doorId = UUID.fromString(doorId);
    }
}
