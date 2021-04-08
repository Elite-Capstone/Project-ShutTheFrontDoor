package com.theelite.portal.ui

interface ClickListener {
    fun onItemClicked(name: String)
}

interface MediaClickListener {
    fun toDownload(name: String)
    fun toDelete(name: String, position: Int)
}