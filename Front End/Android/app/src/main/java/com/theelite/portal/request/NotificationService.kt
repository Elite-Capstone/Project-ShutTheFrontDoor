package com.theelite.portal.request

import com.theelite.portal.Objects.Notification
import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Path

interface NotificationService {

    @GET("notif/getRecent/{email}/{token}")
    fun getRecentNotifications(@Path("email") email: String, @Path("token") token: String) : Call<List<Notification>>
}