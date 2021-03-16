package com.theelite.s3.communication;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Path;

public interface DeviceService {

    @GET("getFamilyAccountForDevice/{id}")
    Call<String> getFamilyAccountForDeviceWithId(@Path("id") String id);
}
