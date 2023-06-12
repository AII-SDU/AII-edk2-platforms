SG2042 EDK2编译构建流程

1.  Install package

    Ubuntu ：

    \$ sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build uuide-dev

2.  Git clone

    edk2: https://github.com/AII-SDU/edk2.git

    edk2-non-osi: https://github.com/AII-SDU/edk2-non-osi.git

    edk2-platforms: <https://github.com/AII-SDU/edk2-platforms.git>

3.  Make (根据目录进行指定)

	export WORKSPACE=/home/Libing/SD_EDK2_SG2042

	export PACKAGES_PATH=\$WORKSPACE/edk2:\$WORKSPACE/edk2-platforms:\$WORKSPACE/edk2-non-osi

	export GCC5_RISCV64_PREFIX=/mnt/share/riscv_gnu/bin/riscv64-unknown-elf-

	export PYTHON_COMMAND=/usr/bin/python3

	. edk2/edksetup.sh

	make -C edk2/BaseTools

	build -a RISCV64 -t GCC5 -p Platform/Sophgo/SG2042Pkg/SG2042_EVB_v1_0_Board/SG2042.dsc

	cd \$WORKSPACE/Build/SG2042_EVB_v1_0/DEBUG_GCC5/FV

	cp SG2042.fd fw_jump.bin

	cd \$WORKSPACE

4. replace file

    替换原SD卡中的fw_jump.bin文件（OpenSBI）为EDK2重命名的SG2042.fd
5. start

    \>\>fs0:

    \>\>grubriscv64.efi

![](media/2791276595faac8e051051613c020942.png)

6. Select OS

![](media/6102f830419b9cc72b7552b64961a4e1.png)

7. OS runtime
