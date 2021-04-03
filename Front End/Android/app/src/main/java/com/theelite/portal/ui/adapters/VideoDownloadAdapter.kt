package com.theelite.portal.ui.adapters


import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R
import com.theelite.portal.ui.media.MediaFragment


class VideoDownloadAdapter(val videoList: List<String>, var context: MediaFragment) : RecyclerView.Adapter<VideoDownloadAdapter.ViewHolder>() {

    //this method is returning the view for each item in the list
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val v = LayoutInflater.from(parent.context).inflate(R.layout.videos_list, parent, false)
        return ViewHolder(v)
    }

    //this method is binding the data on the list
    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.bindItems(videoList[position])
        holder.downloadButton.setOnClickListener {
            context.onItemClicked("http://34.117.160.50/file/download/${videoList[position]}")
        }
    }

    //this method is giving the size of the list
    override fun getItemCount(): Int {
        return videoList.size
    }

    //the class is hodling the list view
    class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        lateinit var textViewName: TextView
        lateinit var downloadButton: Button

        fun bindItems(videoName: String) {
            textViewName = itemView.findViewById(R.id.videoName) as TextView
            downloadButton  = itemView.findViewById(R.id.download) as Button
            textViewName.text = videoName
        }
    }
}