package com.theelite.portal.ui.settings

import android.os.Bundle
import android.os.Handler
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.OptionsAdapter
import com.theelite.portal.ui.OptionsList
import com.theelite.portal.ui.adapters.SystemStatusAdapter
import com.theelite.portal.ui.notifications.RecentNotificationsAdapter
import kotlinx.android.synthetic.main.activity_main.*

class DeviceStatusActivity : AppCompatActivity(), ClickListener {
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var systemStatusRecyclerView: RecyclerView
//    private lateinit var root: View

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_status_list)
        setUpView()
    }

    private fun setUpView() {
//        root = this.layoutInflater
//            .inflate(R.layout.activity_device_status_list, parent, false)
        setUpRecyclerView()
        setUpRefreshLayout()
    }

    //TODO: Allow user to fetch new status
    private fun setUpRefreshLayout() {
        swipeRefreshLayout = findViewById(R.id.deviceStatusSwipeRefresh)
        swipeRefreshLayout.setOnRefreshListener {
            println("Status update -- Starting to Refresh!!")
            val handler = Handler()
            handler.postDelayed({
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }, 1000)
        }
    }

    private fun setUpRecyclerView() {
        systemStatusRecyclerView = findViewById(R.id.systemStatusRecyclerView)
        systemStatusRecyclerView.layoutManager =
            LinearLayoutManager(this, RecyclerView.VERTICAL, false)

        val systems = getSmartDoorSystemList()
        val systemStatusAdapter = SystemStatusAdapter(systems)
        systemStatusRecyclerView.adapter = systemStatusAdapter
//        }
    }

    private fun getConnectedDevices(): ArrayList<ArrayList<String>> {
        val systems = ArrayList<ArrayList<String>>()
//        systems.add(getSmartDoorSystemList())
        return systems
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        println("Pressed on back arrow from Device Status Settings")
        super.onBackPressed()
        return true
    }

    override fun onItemClicked(name: String) {
        TODO("Not yet implemented")
    }

    // Temporary elements
    private fun getSmartDoorSystemList(): ArrayList<ArrayList<String>> {
        val singleSys = ArrayList<ArrayList<String>>()
        var device = ArrayList<String>()

        device.add("Smart Front Door")
        device.add("Fully operational as of ${System.currentTimeMillis()} ms since 1970")
        singleSys.add(device)
        device = ArrayList()
        device.add(getString(R.string.battery_device_name))
        device.add("100%")
        singleSys.add(device)
        device = ArrayList()
        device.add(getString(R.string.camera_device_name))
        device.add("Connected and operational")
        singleSys.add(device)
        device = ArrayList()
        device.add(getString(R.string.display_screen_device_name))
        device.add("Connected and operational")
        singleSys.add(device)
        device = ArrayList()
        device.add(getString(R.string.doorbell_device_name))
        device.add("Connected and operational")
        singleSys.add(device)
        device = ArrayList()
        device.add(getString(R.string.doorlock_device_name))
        device.add("Connected and operational")
        singleSys.add(device)
        device = ArrayList()
        device.add(getString(R.string.motion_sensor_device_name))
        device.add("Connected and operational")
        singleSys.add(device)
//        device = ArrayList()

        return singleSys
    }
}