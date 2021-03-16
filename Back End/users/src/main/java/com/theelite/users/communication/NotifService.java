package com.theelite.users.communication;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Path;

public interface NotifService {

    @GET("newConsumerGroup/{acc}")
    Call<Boolean> createNewConsumerGroup(@Path("acc") String acc);

    @GET("deleteConsumerGroup/{acc}")
    Call<Boolean> deleteConsumerGroup(@Path("acc") String acc);

}
