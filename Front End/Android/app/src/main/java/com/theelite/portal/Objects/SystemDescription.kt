package com.theelite.portal.Objects

import com.theelite.portal.R
import android.content.Context
import android.media.audiofx.AudioEffect

class SystemDescription constructor() {
    private var sysName: String = ""
    private var sysDesc: String = ""
    private var sysDevices: ArrayList<DeviceDescription> = ArrayList()

    constructor(_sysName: String, _sysDesc: String) : this() {
        sysName = _sysName
        sysDesc = _sysDesc
    }

    constructor(_sysName: String, _sysDesc: String, _sysDevices: ArrayList<DeviceDescription>) :
            this(_sysName, _sysDesc) {
        sysDevices = _sysDevices
    }

    fun getSysName() : String {
        return sysName
    }

    fun getSysDesc() : String {
        return sysDesc
    }

    fun getSysDevices() : ArrayList<DeviceDescription> {
        return sysDevices
    }

    fun setSysName(_sysName: String) {
        sysName = _sysName
    }

    fun setSysDesc(_sysDesc: String) {
        sysDesc = _sysDesc
    }

    fun setSysDevices(_sysDevices: ArrayList<DeviceDescription>) {
        sysDevices = _sysDevices
    }

    fun setSystemDesc(_sysName: String, _sysDesc: String, _sysDevices: ArrayList<DeviceDescription>) {
        sysName = _sysName
        sysDesc = _sysDesc
        sysDevices = _sysDevices
    }

    fun addDeviceDesc(dev: DeviceDescription) : ArrayList<DeviceDescription> {
        sysDevices.add(dev)
        return sysDevices
    }

    fun removeDeviceDesc(pos: Int) : ArrayList<DeviceDescription> {
        sysDevices.drop(pos)
        return sysDevices
    }
}

class DeviceDescription constructor() {
    private var devName: String = ""
    private var devDesc: String = ""
    private var devExtraInfo: ArrayList<String> = ArrayList()

    constructor(_devName: String, _devDesc: String) : this() {
        devName = _devName
        devDesc = _devDesc
    }

    constructor(_devName: String, _devDesc: String, _devExtraInfo: ArrayList<String>) :
            this(_devName, _devDesc) {
        devExtraInfo = _devExtraInfo
    }

    fun getDevName() : String {
        return devName
    }

    fun getDevDesc() : String {
        return devDesc
    }

    fun getDevExtra() : ArrayList<String> {
        return devExtraInfo
    }

    fun setDevName(_devName: String) {
        devName = _devName
    }

    fun setDevDesc(_devDesc: String) {
        devDesc = _devDesc
    }

    fun setDevExtra(_devExtraInfo: ArrayList<String>) {
        devExtraInfo = _devExtraInfo
    }

    fun setDeviceDesc(_devName: String, _devDesc: String, _devExtraInfo: ArrayList<String> = ArrayList()) {
        devName = _devName
        devDesc = _devDesc
        devExtraInfo = _devExtraInfo
    }

    fun addExtraInfo(info: String) : ArrayList<String> {
        devExtraInfo.add(info)
        return devExtraInfo
    }

    fun removeExtraInfo(pos: Int) : ArrayList<String> {
        devExtraInfo.drop(pos)
        return devExtraInfo
    }
}