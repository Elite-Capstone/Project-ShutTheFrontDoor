package com.theelite.notifications.communication;

import com.theelite.notifications.model.User;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;

public interface UserService {

    @GET("/")
    Call<User> getUserWithId(@Body User user);
}
