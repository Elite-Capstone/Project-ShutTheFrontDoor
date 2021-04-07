package com.theelite.portal.ui.home

import android.content.Intent
import android.content.SharedPreferences
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.Observer
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.Objects.Command
import com.theelite.portal.Objects.Device
import com.theelite.portal.Objects.TimeOfPublish
import com.theelite.portal.R
import com.theelite.portal.request.DeviceService
import com.theelite.portal.request.LockService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.login.LoginActivity
import com.theelite.portal.ui.stream.StreamActivity
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response
import java.util.*

class HomeFragment : Fragment() {

    private val homeViewModel by activityViewModels<HomeViewModel>()
    private lateinit var textViewHome: TextView
    private lateinit var streamButton: Button
    private lateinit var lockButton: Button
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var deviceService: DeviceService
    private var devices: MutableList<Device> = mutableListOf()

    private var lockState: Boolean = false
    private var email: String? = null
    private var token: String? = null


    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_home, container, false)
        textViewHome = root.findViewById(R.id.text_home)
        createRetrofitObject()
        streamButton = root.findViewById(R.id.streamVideo)
        lockButton = root.findViewById(R.id.doorButton)
        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshHome)
        swipeRefreshLayout.setOnRefreshListener {
            setLockButton()
        }
        loadState()

        setLockButton()

        homeViewModel.text.observe(viewLifecycleOwner, Observer {
            textViewHome.text = it
        })

        streamButton.setOnClickListener() {
            onItemClicked("stream")
        }
        lockButton.setOnClickListener() {
            sendCommandToFirstDevice()
//            if (lockState) {
//                lockButton.text = "Lock"
//                onItemClicked("unlock")
//
//            } else {
//                lockButton.text = "Unlock"
//                onItemClicked("lock")
//            }
        }
        return root

    }

    private fun createRetrofitObject() {
        val retrofit = RetroFit.get(getString(R.string.url))
        deviceService = retrofit.create(DeviceService::class.java);
    }

    fun onItemClicked(click: String) {
//        when (click) {
//            "stream" -> {
        val intent = Intent(this.context, StreamActivity::class.java)
        this.startActivity(intent)
//            }
//        }
    }

    private fun sendCommandToFirstDevice() {
        if (devices != null && devices.size > 0) {
            var command = Command(
                null,
                devices[0].deviceId,
                if (devices[0].status?.statusList?.doorLocked!!) "Unlock door" else "Lock door",
                0,
                0
            )

            GlobalScope.launch {
                sendCommand(command)
                setLockButton()
            }
        }
    }

    private fun sendCommand(command: Command) {
        deviceService.sendCommand(command, email!!, token!!).execute()
    }

//    private fun changeState(state: String) {
//        val retrofit = RetroFit.get(getString(R.string.url))
//        val lockService: LockService = retrofit.create(LockService::class.java)
//
//        var commandRequest: String = ""
//
//        val c = Calendar.getInstance()
//
//        val year = c.get(Calendar.YEAR)
//        val month = c.get(Calendar.MONTH)
//        val day = c.get(Calendar.DAY_OF_MONTH)
//
//        val hour = c.get(Calendar.HOUR_OF_DAY)
//        val minute = c.get(Calendar.MINUTE)
//        val second = c.get(Calendar.SECOND)
//
//        when (state) {
//            "lock" -> {
//                commandRequest = "Lock door"
//            }
//            "unlock" -> {
//                commandRequest = "Unlock door"
//            }
//        }
//
//        var timeOfPublish = TimeOfPublish(year, month, day, hour, minute, second)
//        var command: Command =
//            Command(timeOfPublish, devices[0].deviceId, commandRequest, 0, 0)
//
//        val call = lockService.sendCommand(
//            command,
//            "$email",
//            "$token"
//        )
//
//        call.enqueue(object : Callback<String> {
//            override fun onResponse(call: Call<String>, response: Response<String>) {
//                if (response.isSuccessful) {
//                    saveState()
//                }
//            }
//
//            override fun onFailure(call: Call<String>, t: Throwable) {
//                Toast.makeText(activity, "${t.message}", Toast.LENGTH_SHORT).show()
//            }
//        })
//    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = this.requireActivity()
            .getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
        lockState = sharedPreferences.getBoolean(LoginActivity.DOORSTATE, false)
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)
    }

//    private fun saveState() {
//        val sharedPreferences: SharedPreferences = this.requireActivity()
//            .getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
//        val editor: SharedPreferences.Editor = sharedPreferences.edit()
//        editor.putBoolean(LoginActivity.DOORSTATE, lockState)
//        editor.apply()
//    }

    private fun setLockButton() {
        GlobalScope.launch {
            lockButton.text = getDeviceStatus()
            if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
        }
    }

    private fun getDeviceStatus(): String {
        val resultOfRequest = deviceService.getDevices(email!!, token!!).execute()
        if (resultOfRequest != null && resultOfRequest.isSuccessful && resultOfRequest.body()!! != null) {
            devices = (resultOfRequest.body() as MutableList<Device>?)!!
        }
        return if (devices[0].status?.statusList?.doorLocked == true) "Unlock" else "Lock"
    }
}

