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

class SingleSysStatusAdapter(private val deviceSet : ArrayList<DeviceDescription>) :
    RecyclerView.Adapter<SingleSysStatusAdapter.ViewHolder>() {

    private lateinit var view: View

    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val titleTextView: TextView
        val systemDescriptionTextView: TextView
//        val params: ViewGroup.LayoutParams
//        var deviceNameTextView: TextView
//        var deviceDescTextView: TextView
//        var it: Int

        init {
            titleTextView = view.findViewById(R.id.systemStatusTitleLabel)
            systemDescriptionTextView =
                view.findViewById(R.id.systemStatusDescriptionLabel)
//            params = ViewGroup.LayoutParams(
//                ViewGroup.LayoutParams.MATCH_PARENT,
//                ViewGroup.LayoutParams.WRAP_CONTENT
//            )

            // Holds device name
//            deviceNameTextView = TextView(view.context)
//            deviceNameTextView.layoutParams = params
//            deviceNameTextView.textSize = (R.dimen.card_subtitle_size_float).toFloat()
//            deviceNameTextView.setPadding(0, 0, 0, 2)

            // Holds device status
//            deviceDescTextView = TextView(view.context)
//            deviceDescTextView.layoutParams = params
//            deviceDescTextView.textSize = (R.dimen.card_subtitle_size_float).toFloat()
//            deviceDescTextView.setPadding(0, 0, 0, 5)
//            var id = MAX_VALUE
//            it = 0
//            while (++it <= nDev) {
//                deviceNameTextView.id = id--
//                deviceDescTextView.id = id--
//                viewGroup.addView(deviceNameTextView)
//                viewGroup.addView(deviceDescTextView)
//            }
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_system_status, parent, false)
        return ViewHolder(view)
    }

    // The device position depends on the amount of information stored per device
    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        var parentArrayPos = 0
        var childArrayPos  = position

//        when (position) {
//            0 -> holder.titleTextView.text = deviceSet[0][0]
//            1 -> holder.systemDescriptionTextView.text = deviceSet[0][1]
//            else -> {
//                while (childArrayPos >= deviceSet[parentArrayPos].size)
//                    childArrayPos -= deviceSet[parentArrayPos++].size
//                (holder.itemView.findViewById<TextView>(this.getItemId(position).toInt()))
//                    .text = deviceSet[parentArrayPos][childArrayPos]
//            }
//        }
    }

    override fun getItemCount(): Int {
        return deviceSet.size
    }

    private fun setUpRecyclerView(index: Int) {
        val deviceRecyclerView = view.findViewById<RecyclerView>(R.id.statusSingleDeviceRecyclerView)
        deviceRecyclerView.layoutManager =
            LinearLayoutManager(view.context, RecyclerView.VERTICAL, false)

        deviceRecyclerView.adapter = SingleDeviceStatusAdapter(deviceSet[index])
    }
}