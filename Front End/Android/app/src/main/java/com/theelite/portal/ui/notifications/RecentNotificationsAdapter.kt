package com.theelite.portal.ui.notifications

import android.view.LayoutInflater
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R
import kotlinx.android.synthetic.main.recent_notifications_no_actions.*

class RecentNotificationsAdapter(private val dataSet: ArrayList<String>) :
    RecyclerView.Adapter<RecentNotificationsAdapter.ViewHolder>() {


    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val titleTextView: TextView
        val descriptionTextView: TextView
        val dateTextView: TextView

        init {
            titleTextView = view.findViewById(R.id.recentNotificationsNoActionsTitleLabel)
            descriptionTextView =
                view.findViewById(R.id.recentNotificationsNoActionsDescriptionLabel)
            dateTextView = view.findViewById(R.id.recentNotificationsNoActionsDateLabel)
        }

    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.recent_notifications_no_actions, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.titleTextView.text = dataSet[position]
        holder.descriptionTextView.text = dataSet[position]
        holder.dateTextView.text = dataSet[position]
    }

    override fun getItemCount(): Int {
        return dataSet.size
    }
}