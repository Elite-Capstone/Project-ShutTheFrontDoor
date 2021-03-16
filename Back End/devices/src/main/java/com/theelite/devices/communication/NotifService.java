package com.theelite.devices.communication;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Path;

import java.util.List;

public interface NotifService {

    @GET("/newTopic/{doorId}")
    Call<Boolean> newDeviceAdded(@Path("doorId") String doorId);

    @GET("/deleteTopic/{doorId}")
    Call<Boolean> deviceDeleted(@Path("doorId") String doorId);

    @GET("/deleteTopics/")
    Call<Boolean> deviceDeleted(@Body List<String> doorIds);
}
