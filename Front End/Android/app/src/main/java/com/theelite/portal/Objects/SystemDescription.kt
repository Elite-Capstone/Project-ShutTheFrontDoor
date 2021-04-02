package com.theelite.portal.Objects

class SystemDescription(
    private var sysName: String = "",
    private var sysDesc: String = "",
    private var sysDevices: ArrayList<DeviceDescription> = ArrayList()
) {

    fun getSysName(): String {
        return sysName
    }

    fun getSysDesc(): String {
        return sysDesc
    }

    fun getSysDevices(): ArrayList<DeviceDescription> {
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

    fun setSystemDesc(
        _sysName: String,
        _sysDesc: String,
        _sysDevices: ArrayList<DeviceDescription>
    ) {
        sysName = _sysName
        sysDesc = _sysDesc
        sysDevices = _sysDevices
    }

    fun addDeviceDesc(dev: DeviceDescription): ArrayList<DeviceDescription> {
        sysDevices.add(dev)
        return sysDevices
    }

    fun popDeviceDesc(): DeviceDescription {
        val device: DeviceDescription
        if (sysDevices.isNotEmpty()) {
            device = sysDevices[0]
            sysDevices.drop(0)
        } else {
            device = DeviceDescription()
            println("Empty Device Array")
        }
        return device
    }

    fun getItemCount(): Int {
        return 2 + sysDevices.size
    }
}

class DeviceDescription(
    private var devName: String = "",
    private var devDesc: String = "",
    private var devExtraInfo: ArrayList<String> = ArrayList()
) {

    fun getDevName(): String {
        return devName
    }

    fun getDevDesc(): String {
        return devDesc
    }

    fun getDevExtra(): ArrayList<String> {
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

    fun setDeviceDesc(
        _devName: String,
        _devDesc: String,
        _devExtraInfo: ArrayList<String> = ArrayList()
    ) {
        devName = _devName
        devDesc = _devDesc
        devExtraInfo = _devExtraInfo
    }

    fun addExtraInfo(info: String): ArrayList<String> {
        devExtraInfo.add(info)
        return devExtraInfo
    }

    fun popExtraInfo(): String {
        val extra: String
        if (devExtraInfo.isNotEmpty()) {
            extra = devExtraInfo[0]
            devExtraInfo.drop(0)
        } else {
            extra = ""
            println("Empty Extra Info Array")
        }
        return extra
    }

    fun getItemCount(): Int {
        return 2 + devExtraInfo.size
    }
}