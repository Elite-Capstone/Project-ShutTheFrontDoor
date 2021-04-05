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
        holder.deviceNameTextView.text = "ESP32 Cam Unit"
        holder.doorClosedTextView.text = "Yes"
        holder.doorLockedTextView.text = "Yes"
        holder.wifiConnectionTextView.text = "Connected"
        holder.batteryLevelTextView.text = "100%"
        holder.sdCardInitTextView.text = "True"
        holder.camStreamInit.text = "Running"
        holder.camInitTextView.text = "True"
    }

    override fun getItemCount(): Int {
        return devices.size
    }
}
