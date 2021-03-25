package com.theelite.mqttKafkaBridge;

import com.theelite.mqttKafkaBridge.model.Notification;


import org.apache.kafka.clients.producer.KafkaProducer;

import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.log4j.Logger;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import java.util.Properties;

@Component
public class Bridge implements MqttCallback {

    public Bridge(Environment environment) {
        this.brokerUri = environment.getProperty("bootstrap.servers");
        logger.info("bootstrap servers is " + environment.getProperty("bootstrap.servers"));
        logger.info("mqtt servers is " + environment.getProperty("mqtt.servers"));
        this.mqttServerUri = environment.getProperty("mqtt.servers");
        connect();
    }

    private final Logger logger = Logger.getLogger(this.getClass().getName());
    private String mqttServerUri;
    private String mqttClientId = "MqttKafkaBridge";
    private String brokerUri;

    private MqttAsyncClient mqtt;
    private Producer<String, Notification> kafkaProducer;

    public void connect() {
        try {
            mqtt = new MqttAsyncClient(mqttServerUri, mqttClientId);
            mqtt.setCallback(this);
            IMqttToken token = mqtt.connect();
            Properties props = new Properties();
            props.put("bootstrap.servers", brokerUri);
            props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
            props.put("value.serializer", "org.springframework.kafka.support.serializer.JsonSerializer");
            kafkaProducer = new KafkaProducer<>(props);
            token.waitForCompletion();

        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void reconnect() throws MqttException {
        IMqttToken token = mqtt.connect();
        token.waitForCompletion();
    }

    @Override
    public void connectionLost(Throwable cause) {
        logger.warn("Lost connection to MQTT server", cause);
        while (true) {
            try {
                logger.info("Attempting to reconnect to MQTT server");
                reconnect();
                logger.info("Reconnected to MQTT server, resuming");
                return;
            } catch (MqttException e) {
                logger.warn("Reconnect failed, retrying in 10 seconds", e);
            }
            try {
                Thread.sleep(10000);
            } catch (InterruptedException e) {
            }
        }
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        byte[] payload = message.getPayload();
        if (!topic.contains("cmd")) {
            Notification notification = new Notification(topic, new String(payload));
            kafkaProducer.send((new ProducerRecord<>(topic, notification)));
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

    }
}
