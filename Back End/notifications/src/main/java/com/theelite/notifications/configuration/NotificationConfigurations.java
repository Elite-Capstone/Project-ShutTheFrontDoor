package com.theelite.notifications.configuration;

import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.KafkaAdminClient;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.kafka.KafkaProperties;
import org.springframework.context.EnvironmentAware;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;
import org.springframework.kafka.core.ConsumerFactory;
import org.springframework.kafka.core.DefaultKafkaConsumerFactory;
import org.springframework.kafka.core.KafkaAdmin;
import org.springframework.kafka.support.serializer.JsonDeserializer;

import java.util.Properties;
import java.util.UUID;

@Configuration
public class NotificationConfigurations {
    @Value("${spring.kafka.bootstrap-servers}")
    public String bootstrapServer;

    @Bean
    public ConsumerFactory<UUID, Notification> consumerFactory(KafkaProperties kafkaProperties) {
        return new DefaultKafkaConsumerFactory<>(kafkaProperties.buildConsumerProperties());
    }

    @Bean
    public KafkaAdmin kafkaAdmin(KafkaProperties kafkaProperties) {
        return new KafkaAdmin(kafkaProperties.buildAdminProperties());
    }

    @Bean
    public AdminClient kafkaAdminClient(KafkaProperties kafkaProperties) {
        return KafkaAdminClient.create(kafkaProperties.buildAdminProperties());
    }


    public static Properties getConsumerProps(String clientId, String consumerGroup, String bootstrapServer) {
        Properties props = new Properties();
        props.setProperty("bootstrap.servers", bootstrapServer);
        props.setProperty("group.id", consumerGroup);
        props.setProperty("client.id", clientId);
        props.setProperty("enable.auto.commit", "true");
        props.setProperty("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
        props.setProperty("value.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
//        props.setProperty(JsonDeserializer.TRUSTED_PACKAGES,"*");

        return props;
    }

    public static Properties getProducerProps(String bootstrapServer){
        // serializer for java object
        Properties props = new Properties();
        props.setProperty("bootstrap.servers", bootstrapServer);
        props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");

        return props;
    }
}
