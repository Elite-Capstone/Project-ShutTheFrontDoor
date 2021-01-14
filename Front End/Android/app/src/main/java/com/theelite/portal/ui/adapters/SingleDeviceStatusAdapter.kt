package com.theelite.portal.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.DeviceDescription
import com.theelite.portal.R

class SingleDeviceStatusAdapter(private var device: DeviceDescription) :
    RecyclerView.Adapter<SingleDeviceStatusAdapter.DeviceExtraViewHolder>() {

    private lateinit var view: View

    class DeviceExtraViewHolder(_view: View) : RecyclerView.ViewHolder(_view) {
        val view = _view
        val deviceTitleTextView: TextView
        val deviceDescriptionTextView: TextView
        private lateinit var deviceRecyclerView: RecyclerView

        init {
            deviceTitleTextView = view.findViewById(R.id.statusSingleDeviceNameTextView)
            deviceDescriptionTextView = view.findViewById(R.id.statusSingleDeviceDescTextView)
        }

        // Used to attach the adapter to the recycler found within each instance
        fun setUpRecyclerView(device : DeviceDescription) {
            deviceRecyclerView = view.findViewById(R.id.statusSingleDeviceExtraInfoRecyclerView)
            deviceRecyclerView.layoutManager =
                LinearLayoutManager(view.context, RecyclerView.VERTICAL, false)

            deviceRecyclerView.adapter = SingleDeviceStatusAdapter(device)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DeviceExtraViewHolder {
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.system_status_single_device, parent, false)
        return DeviceExtraViewHolder(view)
    }

    override fun onBindViewHolder(holder: DeviceExtraViewHolder, position: Int) {
        holder.deviceTitleTextView.text = device.getDevName()
        holder.deviceDescriptionTextView.text = device.getDevDesc()

        // Pos 0 and 1 are name and description
        //if (position >= 2)
        //holder.setUpRecyclerView(deviceSet[position])
    }

    override fun getItemCount(): Int {
        return device.getItemCount()
    }

}