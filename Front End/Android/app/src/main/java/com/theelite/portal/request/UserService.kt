package com.theelite.portal.request

import com.theelite.portal.Objects.User
import retrofit2.Call
import retrofit2.http.Body
import retrofit2.http.POST
import retrofit2.http.PUT

interface UserService {

    @POST("user/auth")
    fun authenthicateUser(@Body user: User): Call<String>

    @PUT("user/new")
    fun newUser(@Body user: User): Call<String>

    @POST("user/get")
    fun getUserInfo(@Body user: User): Call<User>


}