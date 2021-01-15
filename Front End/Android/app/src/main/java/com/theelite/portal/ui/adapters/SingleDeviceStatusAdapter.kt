package com.theelite.portal.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.DeviceDescription
import com.theelite.portal.R

class SingleDeviceStatusAdapter(private var device: DeviceDescription) :
    RecyclerView.Adapter<SingleDeviceStatusAdapter.DeviceExtraViewHolder>() {

    private lateinit var view: View
    private lateinit var tempDevice: DeviceDescription

    class DeviceExtraViewHolder(_view: View) : RecyclerView.ViewHolder(_view) {
        val view = _view
        lateinit var textView: TextView
        lateinit var  linLayout: LinearLayout

        fun addText(text: String) {
            textView = TextView(view.context)
            textView.text = text
            linLayout = view.findViewById(R.id.statusSingleDeviceLinearLayout)
            linLayout.addView(textView,
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): DeviceExtraViewHolder {
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.system_status_single_device, parent, false)
        return DeviceExtraViewHolder(view)
    }

    override fun onBindViewHolder(holder: DeviceExtraViewHolder, position: Int) {
        if (position < device.getDevExtra().size)
            holder.addText(device.getDevExtra()[position])
    }

    override fun getItemCount(): Int {
        return device.getDevExtra().size
    }

}