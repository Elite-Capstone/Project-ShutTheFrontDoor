package com.theelite.portal.ui.settings

import android.os.Bundle
import android.os.Handler
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.Objects.DeviceDescription
import com.theelite.portal.Objects.SystemDescription
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.adapters.SystemsStatusAdapter

class DeviceStatusActivity : AppCompatActivity(), ClickListener {
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var activityStatusRecyclerView: RecyclerView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_status_list)
        setUpView()
    }

    private fun setUpView() {
        setUpRefreshLayout()
        setUpRecyclerView()
    }

    //TODO: Allow user to fetch new status
    private fun setUpRefreshLayout() {
        swipeRefreshLayout = findViewById(R.id.activityStatusSwipeRefresh)
        swipeRefreshLayout.setOnRefreshListener {
            println("Status update -- Starting to Refresh!!")
            val handler = Handler()
            handler.postDelayed({
                if (swipeRefreshLayout.isRefreshing) swipeRefreshLayout.isRefreshing = false
            }, 1000)
        }
    }

    // TODO: Each system is associated to a single instance within the adapter
    private fun setUpRecyclerView() {
        activityStatusRecyclerView = findViewById(R.id.activityStatusRecyclerView)
        activityStatusRecyclerView.layoutManager =
            LinearLayoutManager(this, RecyclerView.VERTICAL, false)

        val systems = getConnectedDevices()
        val systemStatusAdapter = SystemsStatusAdapter(systems)
        activityStatusRecyclerView.adapter = systemStatusAdapter
    }

    private fun getConnectedDevices(): ArrayList<SystemDescription> {
        val systems = ArrayList<SystemDescription>()
        systems.add(getSmartDoorSystemList())
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
    private fun getSmartDoorSystemList(): SystemDescription {
        val singleSys = SystemDescription(
            "Smart Front Door",
            "Fully operational as of " + System.currentTimeMillis() + " ms since 1970", null
        )

        singleSys.addDeviceDesc(
            DeviceDescription(getString(R.string.battery_device_name), "100%")
        )
        singleSys.addDeviceDesc(
            DeviceDescription(
                getString(R.string.camera_device_name),
                "Connected and operational"
            )
        )
        singleSys.addDeviceDesc(
            DeviceDescription(
                getString(R.string.display_screen_device_name),
                "Connected and operational"
            )
        )
        singleSys.addDeviceDesc(
            DeviceDescription(
                getString(R.string.doorbell_device_name),
                "Connected and operational"
            )
        )
        singleSys.addDeviceDesc(
            DeviceDescription(
                getString(R.string.doorlock_device_name),
                "Connected and operational"
            )
        )
        singleSys.addDeviceDesc(
            DeviceDescription(
                getString(R.string.motion_sensor_device_name),
                "Connected and operational"
            )
        )

        return singleSys
    }
}