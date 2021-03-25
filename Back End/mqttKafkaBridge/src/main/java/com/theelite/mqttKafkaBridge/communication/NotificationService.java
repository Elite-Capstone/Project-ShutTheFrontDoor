package com.theelite.mqttKafkaBridge.communication;

import retrofit2.Call;
import retrofit2.http.GET;

import java.util.List;

public interface NotificationService {
    @GET("getTopics")
    Call<List<String>> getTopics();
}
