package com.theelite.portal.ui.adapters

import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.DeviceDescription

class SingleDeviceStatusAdapter(private var device: DeviceDescription) :
    RecyclerView.Adapter<SingleSysStatusAdapter.ViewHolder>() {

    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ): SingleSysStatusAdapter.ViewHolder {
        TODO("Not yet implemented")
    }

    override fun onBindViewHolder(holder: SingleSysStatusAdapter.ViewHolder, position: Int) {
        TODO("Not yet implemented")
    }

    override fun getItemCount(): Int {
        return 2 + device.getDevExtra().size
    }

}