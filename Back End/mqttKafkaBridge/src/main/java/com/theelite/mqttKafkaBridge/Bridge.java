package com.theelite.mqttKafkaBridge;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.theelite.mqttKafkaBridge.communication.NotificationService;
import com.theelite.mqttKafkaBridge.model.Notification;


import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.log4j.Logger;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import java.util.Arrays;
import java.util.List;
import java.util.Properties;

@Component
public class Bridge implements MqttCallback {
    public NotificationService notificationService;

    public Bridge(Environment environment) {
        this.brokerUri = environment.getProperty("spring.kafka.producer.bootstrap-servers");
        this.mqttServerUri = environment.getProperty("mqtt.servers");
        setUpRetroObject(environment.getProperty("notif.url"));
        connect();
        Thread periodicThread = new Thread(periodicallyGetKafkaTopics);
        periodicThread.start();
    }

    public void setUpRetroObject(String url) {
        Gson gson = new GsonBuilder()
                .setLenient()
                .create();
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(url)
                .addConverterFactory(GsonConverterFactory.create(gson))
                .build();
        notificationService = retrofit.create(NotificationService.class);
    }

    Runnable periodicallyGetKafkaTopics = new Runnable() {
        @Override
        public void run() {
            while (true) {
                try {
                    List<String> topics = notificationService.getTopics().execute().body();
                    if (topics != null && topics.size() > 0) {
                        subcribeToTopics(topics.toArray(new String[topics.size()]));
                    }
                    Thread.sleep(30000);
                    logger.trace("Thread going to sleep");
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
        }
    };

    private final Logger logger = Logger.getLogger(this.getClass().getName());
    private String mqttServerUri;
    private String mqttClientId = "MqttKafkaBridge";
    private String brokerUri;

    private MqttAsyncClient mqtt;
    public Producer<String, Notification> kafkaProducer;

    public void connect() {
        try {
            mqtt = new MqttAsyncClient(mqttServerUri, MqttAsyncClient.generateClientId());
            mqtt.setCallback(this);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            IMqttToken token = mqtt.connect(connOpts);
            Properties props = new Properties();
            props.put("bootstrap.servers", brokerUri);
            props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
            props.put("value.serializer", "org.springframework.kafka.support.serializer.JsonSerializer");
            kafkaProducer = new KafkaProducer<>(props);
            token.waitForCompletion();
            mqtt.subscribe("test", 0);
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
        System.out.println("NEW MQTT MESSAGE - " + new String(payload));
        if (!topic.contains("cmd")) {
            Notification notification = new Notification(topic, new String(payload));
            kafkaProducer.send((new ProducerRecord<>(topic, notification)));
        }
    }

    private void subcribeToTopics(String[] topics) {
        int[] qos = new int[topics.length];
        try {
            IMqttToken token = mqtt.subscribe(topics, qos);
            token.waitForCompletion();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        try {
            logger.info("DELIVERY COMPLETE " + iMqttDeliveryToken.getMessage().toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
