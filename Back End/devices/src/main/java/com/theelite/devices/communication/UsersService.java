package com.theelite.devices.communication;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Path;

public interface UsersService {

    @GET("validateUser/{email}/{token}")
    Call<Boolean> validateUser(@Path("email") String email, @Path("token") String token);

    @GET("familyAccount/{email}")
    Call<String> getFamilyAccountFor(@Path("email") String email);
}
