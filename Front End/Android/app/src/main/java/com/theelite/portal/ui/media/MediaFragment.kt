package com.theelite.portal.ui.media

import android.os.Bundle
import android.os.Handler
import android.os.StrictMode
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.theelite.portal.R
import com.theelite.portal.ui.adapters.RecentNotificationsAdapter
import kotlinx.android.synthetic.main.fragment_media.*
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.URL
import kotlinx.android.synthetic.main.activity_media.*


class MediaFragment : Fragment() {

    private lateinit var root: View

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        root = inflater.inflate(R.layout.fragment_media, container, false)
        return root

    }



}