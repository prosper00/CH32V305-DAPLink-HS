# ch32v305_dap
Alternate firmware for the WCH-LinkE - enable ARM DAP debugging in USB-2.0 High Speed mode.

Supports SWD clock speed up to 10MHz - flash speed of >100kB/s. 

See https://oshwhub.com/fanhuacloud/wch-linke/ for details and instructions (machine-translated: https://oshwhub-com.translate.goog/fanhuacloud/wch-linke?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en&_x_tr_pto=wapp )

Binaries included. Flash them to a WCH-LinkE probe: push the 'IAP' button and then plug in the probe. Then flash it using the wchisp tool, or a WCH flasher like https://github.com/ch32-rs/wlink

If you want to build this for yourself, you will need to download and install the WCH toolchain, then edit CMakeLists.txt to point to the location of their GCC version.

Fork from https://gitee.com/fhcloud/ch32v305_dap/

