# ch32v305_dap
Alternate firmware for the WCH-LinkE - enable ARM DAP debugging in USB-2.0 High Speed mode.

Supports SWD clock speed up to 10MHz - flash speed of >100kB/s. 

See https://oshwhub.com/fanhuacloud/wch-linke/ for details and instructions (machine-translated: https://oshwhub-com.translate.goog/fanhuacloud/wch-linke?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en&_x_tr_pto=wapp)

Binaries included. Flash them to a WCH-LinkE probe: push the 'IAP' button and then plug in the probe. Then flash it using the [wchisp](https://github.com/ch32-rs/wchisp) tool, or [wlink](https://github.com/ch32-rs/wlink).

Forked from https://gitee.com/fhcloud/ch32v305_dap/

# Build

1. Install [MRS Toolchain](https://github.com/ch32-riscv-ug/MounRiver_Studio_Community_miror) from mirror.
```bash
curl -OL "https://github.com/ch32-riscv-ug/MounRiver_Studio_Community_miror/releases/download/1.92-toolchain/MRS_Toolchain_Linux_x64_V1.92.tar.xz"
tar -xf MRS_Toolchain_Linux_x64_V1.92.tar.xz
sudo cp -r "MRS_Toolchain_Linux_x64_V1.92/RISC-V_Embedded_GCC12" "/opt/riscv/"
rm MRS_Toolchain_Linux_x64_V1.92.tar.xz
rm -r MRS_Toolchain_Linux_x64_V1.92
export PATH=/opt/riscv/bin:$PATH
```

2. Run cmake as following commands
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

3. Binaries are created in build directory.
