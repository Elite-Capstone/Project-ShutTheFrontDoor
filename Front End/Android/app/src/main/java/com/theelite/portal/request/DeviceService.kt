package com.theelite.portal.request

import com.theelite.portal.Objects.Command
import com.theelite.portal.Objects.Device
import retrofit2.Call
import retrofit2.http.Body
import retrofit2.http.GET
import retrofit2.http.POST
import retrofit2.http.Path

interface DeviceService {

    @GET("device/{email}/{token}")
    fun getDevices(@Path("email") email: String, @Path("token") token : String) : Call<List<Device>>

    @POST("device/command/{email}/{token}")
    fun sendCommand(@Body command: Command, @Path("email") email: String, @Path("token") token : String) : Call<String>
}