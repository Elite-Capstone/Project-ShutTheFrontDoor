package com.theelite.notifications.communication;

import com.theelite.notifications.model.User;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Path;

public interface UserService {

    @GET("/")
    Call<User> getUserWithId(@Body User user);

    @GET("/validateUser/{email}/{token}")
    Call<Boolean> validateUser(@Path("email") String email, @Path("token") String token);

    @GET("/familyAccount/{email}")
    Call<String> getFamilyAccount(@Path("email") String email);
}
