# SG2042 EDK2 compilation and construction process

1. Install the package
Ubuntu ：
$ sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build uuide-dev

2.	Git clone
edk2: https://github.com/AII-SDU/edk2.git
edk2-non-osi: https://github.com/AII-SDU/edk2-non-osi.git
edk2-platforms: https://github.com/AII-SDU/edk2-platforms.git 
3.	Make (根据目录进行指定)
○1 export WORKSPACE=/home/Libing/SD_EDK2_SG2042
○2 export PACKAGES_PATH=$WORKSPACE/edk2:$WORKSPACE/edk2-platforms:$WORKSPACE/edk2-non-osi
○3 export GCC5_RISCV64_PREFIX=/mnt/share/riscv_gnu/bin/riscv64-unknown-elf-
○4 export PYTHON_COMMAND=/usr/bin/python3
○5 . edk2/edksetup.sh
○6 make -C edk2/BaseTools
○7 build -a RISCV64 -t GCC5 -p 	Platform/Sophgo/SG2042Pkg/SG2042_EVB_v1_0_Board/SG2042.dsc
○8 cd $WORKSPACE/Build/SG2042_EVB_v1_0/DEBUG_GCC5/FV
○9 cp SG2042.fd fw_jump.bin
○10 cd $WORKSPACE
4.	replace file
替换原SD卡中的fw_jump.bin文件（OpenSBI）为EDK2重命名的SG2042.fd
5.	start
>>fs0:
>>grubriscv64.efi
6.	OS runtime

