package com.theelite.portal.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.Device
import com.theelite.portal.Objects.SystemDescription
import com.theelite.portal.R

class SystemsStatusAdapter(private var devices: MutableList<Device>) :
    RecyclerView.Adapter<SystemsStatusAdapter.DeviceViewHolder>() {

    private lateinit var view: View

    class DeviceViewHolder(_view: View) : RecyclerView.ViewHolder(_view) {
        val view = _view
        val deviceNameTextView: TextView = view.findViewById(R.id.deviceNameLabel)
        val doorLockedTextView: TextView = view.findViewById(R.id.doorLockedValue)
        val doorClosedTextView: TextView = view.findViewById(R.id.doorClosedValue)
        val batteryLevelTextView: TextView = view.findViewById(R.id.batteryLevelValue)
        val wifiConnectionTextView: TextView = view.findViewById(R.id.wifiValue)
        val sdCardInitTextView: TextView = view.findViewById(R.id.sdCardValue)
        val camStreamInit: TextView = view.findViewById(R.id.camStreamInitValue)
        val camInitTextView: TextView = view.findViewById(R.id.camInitValue)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DeviceViewHolder {
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_system_status, parent, false)
        return DeviceViewHolder(view)
    }

    override fun onBindViewHolder(holder: DeviceViewHolder, position: Int) {
        holder.deviceNameTextView.text = devices[position].deviceName
        holder.doorClosedTextView.text = devices[position].status.statusList.doorClosed.toString()
        holder.doorLockedTextView.text = devices[position].status.statusList.doorLocked.toString()
        holder.wifiConnectionTextView.text =
            devices[position].status.statusList.gotWifiIP.toString()
        holder.batteryLevelTextView.text =
            devices[position].status.statusList.batteryLevel.toString()
        holder.sdCardInitTextView.text = devices[position].status.statusList.sdcardInit.toString()
        holder.camStreamInit.text = devices[position].status.statusList.camStreamInit.toString()
        holder.camInitTextView.text = devices[position].status.statusList.camInit.toString()
    }

    override fun getItemCount(): Int {
        return devices.size
    }
}
