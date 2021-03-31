package com.theelite.devices.mqtt;

import com.theelite.devices.service.DeviceServiceImpl;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.context.annotation.Bean;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

@Component
public class MqttConnect implements MqttCallback {
    private String mqttServerUrl;
    public static MqttAsyncClient mqttClient;
    private DeviceServiceImpl deviceService;

    public MqttConnect(Environment environment, DeviceServiceImpl deviceService) {
        this.mqttServerUrl = environment.getProperty("mqtt.server");
        this.deviceService = deviceService;
    }


    private void connectToMqttBroker() {
        try {
            mqttClient = new MqttAsyncClient(this.mqttServerUrl, MqttAsyncClient.generateClientId());
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
}
