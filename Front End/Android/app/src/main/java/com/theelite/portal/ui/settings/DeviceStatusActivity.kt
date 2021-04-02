package com.theelite.portal.ui.settings

import android.content.SharedPreferences
import android.os.Bundle
import android.os.Handler
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.Objects.*
import com.theelite.portal.R
import com.theelite.portal.request.DeviceService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.adapters.SystemsStatusAdapter
import com.theelite.portal.ui.login.LoginActivity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import okhttp3.Dispatcher
import okio.IOException

class DeviceStatusActivity : AppCompatActivity() {
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var activityStatusRecyclerView: RecyclerView
    private lateinit var email: String
    private lateinit var token: String
    private var devices: MutableList<Device> = mutableListOf()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_status_list)
        setUpView()
    }

    private fun setUpView() {
        loadState()
        setUpRefreshLayout()
        retrieveDevices()
        setUpRecyclerView()
    }

    private fun setUpRefreshLayout() {
        swipeRefreshLayout = findViewById(R.id.activityStatusSwipeRefresh)
        swipeRefreshLayout.setOnRefreshListener {
            println("Status update -- Starting to Refresh!!")
            retrieveDevices()
        }
    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = this.getSharedPreferences(
            LoginActivity.SHARED_PREFS, MODE_PRIVATE
        )
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)!!
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)!!
    }


    private fun setUpRecyclerView() {
        activityStatusRecyclerView = findViewById(R.id.activityStatusRecyclerView)
        activityStatusRecyclerView.layoutManager =
            LinearLayoutManager(this, RecyclerView.VERTICAL, false)
        val systemStatusAdapter = SystemsStatusAdapter(devices)
        activityStatusRecyclerView.adapter = systemStatusAdapter
    }


    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        println("Pressed on back arrow from Device Status Settings")
        super.onBackPressed()
        return true
    }

    private fun retrieveDevices() {
        println("Email $email")
        println("Token $token")
        GlobalScope.launch(Dispatchers.Main) {
            devices = get().toMutableList()
            activityStatusRecyclerView.adapter = SystemsStatusAdapter(devices)
            if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
        }
    }


    private suspend fun get(): List<Device> = withContext(Dispatchers.IO) {
        var devicesList: List<Device> = ArrayList()
        val retrofit = RetroFit.get(getString(R.string.url))
        val deviceService = retrofit.create(DeviceService::class.java)
        try {
            println("Starting to retrieve device info")
            val result = deviceService.getDevices(email, token).execute()
            if (result.isSuccessful && result.body() != null) {
                println("Result is successful!")
                println("Devices has ${devices.size}")
                devicesList = result.body()!!
            }
        } catch (e: IOException) {
            println(e.message)
        }
        return@withContext devicesList
    }


}