package com.theelite.portal.ui.notifications

import android.view.LayoutInflater
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
            if (view.getTag(R.layout.recent_notifications_w_actions) == "recent") {
                titleTextView = view.findViewById(R.id.recentNotificationsWActionsTitleLabel)
                descriptionTextView =
                    view.findViewById(R.id.recentNotificationsWActionsDescriptionLabel)
                dateTextView = view.findViewById(R.id.recentNotificationsWActionsDateLabel)
            } else {
                titleTextView = view.findViewById(R.id.recentNotificationsNoActionsTitleLabel)
                descriptionTextView =
                    view.findViewById(R.id.recentNotificationsNoActionsDescriptionLabel)
                dateTextView = view.findViewById(R.id.recentNotificationsNoActionsDateLabel)
            }
        }

    }

    override fun getItemViewType(position: Int): Int {
        return position
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        var view: View

        if (viewType % 2 == 0) {
            println("View type $viewType is even")
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_no_actions, parent, false)
            view.setTag(R.layout.recent_notifications_no_actions, "old")

        } else {
            println("View type $viewType is odd")
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_w_actions, parent, false)
            view.setTag(R.layout.recent_notifications_w_actions, "recent")
        }
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