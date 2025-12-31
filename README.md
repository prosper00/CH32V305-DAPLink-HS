# ch32v305_dap
Alternate firmware for the WCH-LinkE - enable ARM DAP debugging in USB-2.0 High Speed mode.

Supports SWD clock speed up to 10MHz - flash speed of >100kB/s. 

See https://oshwhub.com/fanhuacloud/wch-linke/ for details and instructions (machine-translated: https://oshwhub-com.translate.goog/fanhuacloud/wch-linke?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en&_x_tr_pto=wapp )

Binaries included. Flash them to a WCH-LinkE probe: push the 'IAP' button and then plug in the probe. Then flash it using the [wchisp](https://github.com/ch32-rs/wchisp) tool, or [wlink](https://github.com/ch32-rs/wlink)

Fork from https://gitee.com/fhcloud/ch32v305_dap/

# Build

1. Install [RISC-V GNU Compiler Toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain) for Newlib with `--disable-linux -march=rv32imac_zicsr -mabi=ilp32`.
```bash
sudo apt-get update
sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev libncurses-dev

git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain

./configure --prefix=/opt/riscv --disable-linux --with-abi=ilp32 --with-arch=rv32imac_zicsr
sudo make -j $(nproc)

export PATH=/opt/riscv/bin:$PATH
```

2. Run cmake as following commands
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

3. Binaries are created in build directory.
