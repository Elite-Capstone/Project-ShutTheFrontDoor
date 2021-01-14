package com.theelite.portal.ui.adapters

import android.bluetooth.BluetoothClass
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.DeviceDescription
import com.theelite.portal.Objects.SystemDescription
import com.theelite.portal.R

class SingleSysStatusAdapter(private val system : SystemDescription) :
    RecyclerView.Adapter<SingleSysStatusAdapter.SingleSysViewHolder>() {

    private lateinit var view: View

    class SingleSysViewHolder(_view: View) : RecyclerView.ViewHolder(_view) {
        val view = _view
        val statusTitleTextView: TextView
        val statusDescritpionTextView: TextView
        private lateinit var systemRecyclerView: RecyclerView

        init {
            statusTitleTextView = view.findViewById(R.id.systemStatusTitleLabel)
            statusDescritpionTextView = view.findViewById(R.id.systemStatusDescriptionLabel)
        }

        // Used to attach the adapter to the recycler found within each instance
        fun setUpRecyclerView(device : DeviceDescription) {
            systemRecyclerView = view.findViewById(R.id.statusSingleDeviceRecyclerView)
            systemRecyclerView.layoutManager =
                LinearLayoutManager(view.context, RecyclerView.VERTICAL, false)

            systemRecyclerView.adapter = SingleDeviceStatusAdapter(device)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): SingleSysViewHolder {
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_system_status, parent, false)
        return SingleSysViewHolder(view)
    }

    // The device position depends on the amount of information stored per device
    override fun onBindViewHolder(holder: SingleSysViewHolder, position: Int) {
        holder.statusTitleTextView.text = system.getSysName()
        holder.statusDescritpionTextView.text = system.getSysDesc()

        // Pos 0 and 1 are title and description
        if (position >= 2)
            holder.setUpRecyclerView(system.getSysDevices()[position])
    }

    override fun getItemCount(): Int {
        return system.getItemCount()
    }
}