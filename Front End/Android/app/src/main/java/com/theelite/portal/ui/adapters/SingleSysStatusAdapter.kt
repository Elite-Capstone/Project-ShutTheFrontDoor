package com.theelite.portal.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.DeviceDescription
import com.theelite.portal.Objects.SystemDescription
import com.theelite.portal.R

class SingleSysStatusAdapter(private val deviceSet : ArrayList<DeviceDescription>?) :
    RecyclerView.Adapter<SingleSysStatusAdapter.SingleSysViewHolder>() {

    private lateinit var view: View

    class SingleSysViewHolder(_view: View) : RecyclerView.ViewHolder(_view) {
        val view = _view
        val deviceTitleTextView: TextView
        val deviceDescriptionTextView: TextView
        private lateinit var systemRecyclerView: RecyclerView

        init {
            deviceTitleTextView = view.findViewById(R.id.statusSingleDeviceNameTextView)
            deviceDescriptionTextView = view.findViewById(R.id.statusSingleDeviceDescTextView)
        }

        // Used to attach the adapter to the recycler found within each instance
        fun setUpRecyclerView(device : DeviceDescription) {
            systemRecyclerView = view.findViewById(R.id.statusSingleDeviceExtraInfoRecyclerView)
            systemRecyclerView.layoutManager =
                LinearLayoutManager(view.context, RecyclerView.VERTICAL, false)

            systemRecyclerView.adapter = SingleDeviceStatusAdapter(device)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): SingleSysViewHolder {
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.system_status_single_device, parent, false)
        return SingleSysViewHolder(view)
    }

    // The device position depends on the amount of information stored per device
    override fun onBindViewHolder(holder: SingleSysViewHolder, position: Int) {
        holder.deviceTitleTextView.text = deviceSet!![position].getDevName()
        holder.deviceDescriptionTextView.text = deviceSet[position].getDevDesc()
        holder.setUpRecyclerView(deviceSet[position])
    }

    override fun getItemCount(): Int {
        return deviceSet?.size ?: 0
    }
}