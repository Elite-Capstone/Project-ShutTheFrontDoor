package com.theelite.portal.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R

class SystemStatusAdapter(private var systemSet: ArrayList<ArrayList<String>>) :
    RecyclerView.Adapter<SystemStatusAdapter.ViewHolder>() {

    class ViewHolder(view: View, viewGroup: ViewGroup, nDev: Int) : RecyclerView.ViewHolder(view) {
        val titleTextView: TextView
        val systemDescriptionTextView: TextView
        val params: ViewGroup.LayoutParams
        var deviceNameTextView: TextView
        var deviceDescTextView: TextView
        var it: Int

        init {
            titleTextView = view.findViewById(R.id.deviceStatusTitleLabel)
            systemDescriptionTextView =
                view.findViewById(R.id.deviceStatusDescriptionLabel)
            params = ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
            )

            // Holds device name
            deviceNameTextView = TextView(view.context)
            deviceNameTextView.layoutParams = params
            deviceNameTextView.textSize = (R.dimen.card_subtitle_size_float).toFloat()
            deviceNameTextView.setPadding(0, 0, 0, 2)

            // Holds device status
            deviceDescTextView = TextView(view.context)
            deviceDescTextView.layoutParams = params
            deviceDescTextView.textSize = (R.dimen.card_subtitle_size_float).toFloat()
            deviceNameTextView.setPadding(0, 0, 0, 5)

            it = 0
            while (++it <= nDev) {
                deviceNameTextView.id =
                    (systemDescriptionTextView.id.toString() + (2*it).toString()).toInt()
                deviceDescTextView.id =
                    (systemDescriptionTextView.id.toString() + (2*it + 1).toString()).toInt()
                viewGroup.addView(deviceNameTextView)
                viewGroup.addView(deviceDescTextView)
            }
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_system_status, parent, false)
        return ViewHolder(view, parent, itemCount)
    }

    // The device position depends on the amount of information stored per device
    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        var parentArrayPos = 0
        var childArrayPos  = position
        while (childArrayPos >= systemSet[parentArrayPos].size)
            childArrayPos -= systemSet[parentArrayPos++].size

        when (position) {
            0 -> holder.titleTextView.text = systemSet[0][0]
            1 -> holder.systemDescriptionTextView.text = systemSet[0][1]
            else -> {
                (holder.itemView.findViewById<TextView>(this.getItemId(position).toInt()))
                    .text = systemSet[parentArrayPos][childArrayPos]
            }
        }
    }

    override fun getItemCount(): Int {
        return systemSet.size
    }
}