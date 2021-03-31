package com.theelite.devices.mqtt;

import com.theelite.devices.service.DeviceServiceImpl;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

@Component
public class MqttConnect implements MqttCallback {
    private String mqttServerUrl;
    public final int fiveMinutes = 300000;
    public static MqttAsyncClient mqttClient;
    private DeviceServiceImpl deviceService;

    public MqttConnect(Environment environment, DeviceServiceImpl deviceService) {
        mqttServerUrl = environment.getProperty("mqtt.server");
        this.deviceService = deviceService;
        connectToMqttBroker();
        periodicallySubscribeToTopics.run();
    }


    private void connectToMqttBroker() {
        try {
            String clientId = MqttAsyncClient.generateClientId();
            mqttClient = new MqttAsyncClient(mqttServerUrl, clientId);
            mqttClient.setCallback(this);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            IMqttToken token = mqttClient.connect(connOpts);
            token.waitForCompletion();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void reconnectToMqttBroker() throws MqttException {
        IMqttToken token = mqttClient.connect();
        token.waitForCompletion();
    }

    @Override
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


    Runnable periodicallySubscribeToTopics = new Runnable() {
        @Override
        public void run() {
//            logger.info("Started Thread to periodically check and subscribe to kafka topics");
            while (true) {
                try {
                    var deviceIds = deviceService.getDeviceIds();
//                    logger.log(Level.INFO, "Got topics");
                    if (deviceIds != null && deviceIds.size() > 0) {
//                        logger.log(Level.INFO, "Topics not null and more than one");
                        subscribeToTopics(getMqttTopicsFromDeviceIds(deviceIds).toArray(String[]::new));
                    }
//                    logger.trace("Thread to check new Topics going to sleep");
                    Thread.sleep(fiveMinutes);
                } catch (Exception e) {
//                    logger.error(e.getMessage());
                }
            }
        }
    };

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
}
