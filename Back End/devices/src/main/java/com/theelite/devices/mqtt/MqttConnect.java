package com.theelite.devices.mqtt;

import com.theelite.devices.service.DeviceServiceImpl;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.core.env.Environment;
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

import java.io.*;
import java.util.*;

@Service
@EnableAsync
public class MqttConnect implements MqttCallback {
    private final String mqttServerUrl;
    public final int fiveMinutes = 300000;
    public static MqttAsyncClient mqttClient;
    private final DeviceServiceImpl deviceService;

    public MqttConnect(Environment environment, DeviceServiceImpl deviceService) {
        mqttServerUrl = environment.getProperty("mqtt.server");
        this.deviceService = deviceService;
        mqttThread.run();
        Timer timer = new Timer("Timer");
        timer.scheduleAtFixedRate(periodicallySubscribeToTopics, 0, fiveMinutes);
    }

    @Async
    void connectToMqttBroker() {
        try {
            String clientId = MqttAsyncClient.generateClientId();
            mqttClient = new MqttAsyncClient(mqttServerUrl, clientId);
            mqttClient.setCallback(this);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            IMqttToken token = mqttClient.connect(connOpts);
            token.waitForCompletion();
            findTopicsToSubsribe();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void reconnectToMqttBroker() throws MqttException {
        IMqttToken token = mqttClient.connect();
        token.waitForCompletion();
    }

    @Override
    @Async
    public void connectionLost(Throwable throwable) {
        while (true) {
            try {
                System.out.println("Attempting to reconnect");
                reconnectToMqttBroker();
                System.out.println("Resuming connection");
                return;
            } catch (MqttException e) {
                System.out.println(e.getMessage());
            }
            try {
                Thread.sleep(10000);
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }
    }

    @Override
    public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
        System.out.println("Received Messaged from " + s);
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        try {
            System.out.println("Delivery complete: " + iMqttDeliveryToken.getMessage());
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    Runnable mqttThread = this::connectToMqttBroker;


    TimerTask periodicallySubscribeToTopics = new TimerTask() {
        public void run() {
            System.out.println("Started Thread to periodically check and subscribe to kafka topics");
            findTopicsToSubsribe();
        }
    };

    @Async
    void findTopicsToSubsribe() {
        try {
            var deviceIds = deviceService.getDeviceIds();
            System.out.println("Got topics");
            if (deviceIds != null && deviceIds.size() > 0) {
                System.out.println("Topics not null and more than one");
                subscribeToTopics(getMqttTopicsFromDeviceIds(deviceIds).toArray(String[]::new));
            }
            System.out.println("Thread to check new Topics going to sleep");
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    private void subscribeToTopics(String[] topics) throws MqttException {
        int[] qos = new int[topics.length];
        Arrays.stream(qos).forEach(q -> q = 0);
        mqttClient.subscribe(topics, qos);
    }

    private List<String> getMqttTopicsFromDeviceIds(List<String> deviceIds) {
        var topics = new ArrayList<String>();
        deviceIds.forEach(id -> {
            topics.add("command/" + id);
            topics.add("status/" + id);
        });
        return topics;
    }

    public static <T> byte[] serializeObject(T object) throws IOException {
        try (ByteArrayOutputStream bos = new ByteArrayOutputStream();
             ObjectOutputStream out = new ObjectOutputStream(bos)) {
            out.writeObject(object);
            return bos.toByteArray();
        }
    }

    private Object convertFromBytes(byte[] bytes) throws IOException, ClassNotFoundException {
        try (ByteArrayInputStream bis = new ByteArrayInputStream(bytes);
             ObjectInputStream in = new ObjectInputStream(bis)) {
            return in.readObject();
        }
    }
}
