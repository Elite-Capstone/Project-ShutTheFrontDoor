package com.theelite.notifications.communication;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Path;

import java.util.List;

public interface DeviceService {

    @GET("getDeviceIdsForUser/{email}/{token}")
    Call<List<String>> getDeviceIdsForUser(@Path("email") String email, @Path("token") String token);

    @GET("getDeviceIdsForAccount/{acc}")
    Call<List<String>> getDeviceIdsForAccount(@Path("acc") String acc);

}
