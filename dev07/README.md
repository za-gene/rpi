# dev07 - WROOM clock with NTP client and DS1307

**Schematics** at DB5.148

## BLINKT STATUS LIGHTS

Read left-to-right, starting at index 0:
```
0 blinkt active
1 DS1307 (external clock) inserted
2 Wifi available
3 NTP working
```

## BUILDING

Ignore WARNING about library Timezone class

## IMPLEMENTATION NOTES

**Wifi** libraries are stored in 
```
~/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries/WiFi/src
```

`WiFiType.h` defines `wl_status_t`, which is returned by `WiFi.status()`. It takes one of the values   `WL_NO_SHIELD, WL_IDLE_STATUS, WL_NO_SSID_AVAIL, WL_SCAN_COMPLETED, WL_CONNECTED,`
`WL_CONNECT_FAILED, WL_CONNECTION_LOST, WL_DISCONNECTED`

