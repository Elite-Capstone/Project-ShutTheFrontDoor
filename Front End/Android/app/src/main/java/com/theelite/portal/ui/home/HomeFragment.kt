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
import com.theelite.portal.R
import com.theelite.portal.request.DeviceService
import com.theelite.portal.request.RetroFit
import com.theelite.portal.ui.login.LoginActivity
import com.theelite.portal.ui.stream.StreamActivity
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

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
        }
        return root

    }

    private fun createRetrofitObject() {
        val retrofit = RetroFit.get(getString(R.string.url))
        deviceService = retrofit.create(DeviceService::class.java);
    }

    fun onItemClicked(click: String) {
        val intent = Intent(this.context, StreamActivity::class.java)
        this.startActivity(intent)
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
            Toast.makeText(requireContext(), "Sending Command", Toast.LENGTH_LONG).show()
            GlobalScope.launch {
                sendCommand(command)
                setLockButton()
            }
        }
    }

    private fun sendCommand(command: Command) {
        deviceService.sendCommand(command, email!!, token!!).execute()
    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = this.requireActivity()
            .getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
        lockState = sharedPreferences.getBoolean(LoginActivity.DOORSTATE, false)
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)
    }

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

