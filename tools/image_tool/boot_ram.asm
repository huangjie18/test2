
========================================================================

** ELF Header Information

    File Name: boot_ram.elf

    Machine class: ELFCLASS32 (32-bit)
    Data encoding: ELFDATA2LSB (Little endian)
    Header version: EV_CURRENT (Current version)
    Operating System ABI: none
    ABI Version: 0
    File Type: ET_EXEC (Executable) (2)
    Machine: EM_ARM (ARM)

    Image Entry point: 0x0012e00d
    Flags: EF_ARM_HASENTRY + EF_ARM_ABI_FLOAT_SOFT (0x05000202)

    ARM ELF revision: 5 (ABI version 2)

    Conforms to Soft float procedure-call standard

    Built with
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]

    Header size: 52 bytes (0x34)
    Program header entry size: 32 bytes (0x20)
    Section header entry size: 40 bytes (0x28)

    Program header entries: 1
    Section header entries: 15

    Program header offset: 106216 (0x00019ee8)
    Section header offset: 106248 (0x00019f08)

    Section header string table index: 14

========================================================================

** Program header #0

    Type          : PT_LOAD (1)
    File Offset   : 52 (0x34)
    Virtual Addr  : 0x0012dff0
    Physical Addr : 0x0012dff0
    Size in file  : 1704 bytes (0x6a8)
    Size in memory: 1896 bytes (0x768)
    Flags         : PF_X + PF_W + PF_R + PF_ARM_ENTRY (0x80000007)
    Alignment     : 4


========================================================================

** Section #1

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_EXECINSTR (0x00000006)
    Addr        : 0x0012dff0
    File Offset : 52 (0x34)
    Size        : 1704 bytes (0x6a8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    $d.realdata
    boot_header_area
    boot_ram_header
        0x0012dff0:    34325842    BX24    DCD    875714626
        0x0012dff4:    0012e000    ....    DCD    1236992
        0x0012dff8:    ffffffff    ....    DCD    4294967295
        0x0012dffc:    0012e00d    ....    DCD    1237005
        0x0012e000:    ffffffff    ....    DCD    4294967295
        0x0012e004:    ffffffff    ....    DCD    4294967295
        0x0012e008:    ffffffff    ....    DCD    4294967295
    $t
    i.boot_ram_entry
    boot_ram_entry
        0x0012e00c:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012e00e:    b672        r.      CPSID    i
        0x0012e010:    2020                MOVS     r0,#0x20
        0x0012e012:    4940        @I      LDR      r1,[pc,#256] ; [0x12e114] = 0x20149000
        0x0012e014:    6188        .a      STR      r0,[r1,#0x18]
        0x0012e016:    f000f8a3    ....    BL       dmac_init ; 0x12e160
        0x0012e01a:    2101        .!      MOVS     r1,#1
        0x0012e01c:    2002        .       MOVS     r0,#2
        0x0012e01e:    f000f9e7    ....    BL       qspi_init ; 0x12e3f0
        0x0012e022:    4d3d        =M      LDR      r5,[pc,#244] ; [0x12e118] = 0x12e698
        0x0012e024:    462f        /F      MOV      r7,r5
        0x0012e026:    3780        .7      ADDS     r7,r7,#0x80
        0x0012e028:    462e        .F      MOV      r6,r5
        0x0012e02a:    3660        `6      ADDS     r6,r6,#0x60
        0x0012e02c:    2401        .$      MOVS     r4,#1
        0x0012e02e:    0324        $.      LSLS     r4,r4,#12
        0x0012e030:    4a39        9J      LDR      r2,[pc,#228] ; [0x12e118] = 0x12e698
        0x0012e032:    2160        `!      MOVS     r1,#0x60
        0x0012e034:    4620         F      MOV      r0,r4
        0x0012e036:    f000f96d    ..m.    BL       flash_read ; 0x12e314
        0x0012e03a:    4838        8H      LDR      r0,[pc,#224] ; [0x12e11c] = 0x12dff0
        0x0012e03c:    6980        .i      LDR      r0,[r0,#0x18]
        0x0012e03e:    4632        2F      MOV      r2,r6
        0x0012e040:    1900        ..      ADDS     r0,r0,r4
        0x0012e042:    2160        `!      MOVS     r1,#0x60
        0x0012e044:    f000f966    ..f.    BL       flash_read ; 0x12e314
        0x0012e048:    6d28        (m      LDR      r0,[r5,#0x50]
        0x0012e04a:    07c1        ..      LSLS     r1,r0,#31
        0x0012e04c:    d103        ..      BNE      0x12e056 ; boot_ram_entry + 74
        0x0012e04e:    0780        ..      LSLS     r0,r0,#30
        0x0012e050:    d501        ..      BPL      0x12e056 ; boot_ram_entry + 74
        0x0012e052:    4c31        1L      LDR      r4,[pc,#196] ; [0x12e118] = 0x12e698
        0x0012e054:    e007        ..      B        0x12e066 ; boot_ram_entry + 90
        0x0012e056:    6b38        8k      LDR      r0,[r7,#0x30]
        0x0012e058:    07c1        ..      LSLS     r1,r0,#31
        0x0012e05a:    d103        ..      BNE      0x12e064 ; boot_ram_entry + 88
        0x0012e05c:    0780        ..      LSLS     r0,r0,#30
        0x0012e05e:    d501        ..      BPL      0x12e064 ; boot_ram_entry + 88
        0x0012e060:    4634        4F      MOV      r4,r6
        0x0012e062:    e000        ..      B        0x12e066 ; boot_ram_entry + 90
        0x0012e064:    2400        .$      MOVS     r4,#0
        0x0012e066:    2c00        .,      CMP      r4,#0
        0x0012e068:    d0e0        ..      BEQ      0x12e02c ; boot_ram_entry + 32
        0x0012e06a:    42ac        .B      CMP      r4,r5
        0x0012e06c:    d101        ..      BNE      0x12e072 ; boot_ram_entry + 102
        0x0012e06e:    2500        .%      MOVS     r5,#0
        0x0012e070:    e001        ..      B        0x12e076 ; boot_ram_entry + 106
        0x0012e072:    482a        *H      LDR      r0,[pc,#168] ; [0x12e11c] = 0x12dff0
        0x0012e074:    6985        .i      LDR      r5,[r0,#0x18]
        0x0012e076:    6da1        .m      LDR      r1,[r4,#0x58]
        0x0012e078:    6de0        .m      LDR      r0,[r4,#0x5c]
        0x0012e07a:    1a40        @.      SUBS     r0,r0,r1
        0x0012e07c:    2121        !!      MOVS     r1,#0x21
        0x0012e07e:    1828        (.      ADDS     r0,r5,r0
        0x0012e080:    01c9        ..      LSLS     r1,r1,#7
        0x0012e082:    1847        G.      ADDS     r7,r0,r1
        0x0012e084:    6d20         m      LDR      r0,[r4,#0x50]
        0x0012e086:    06c0        ..      LSLS     r0,r0,#27
        0x0012e088:    0f40        @.      LSRS     r0,r0,#29
        0x0012e08a:    2802        .(      CMP      r0,#2
        0x0012e08c:    d018        ..      BEQ      0x12e0c0 ; boot_ram_entry + 180
        0x0012e08e:    2804        .(      CMP      r0,#4
        0x0012e090:    d016        ..      BEQ      0x12e0c0 ; boot_ram_entry + 180
        0x0012e092:    2003        .       MOVS     r0,#3
        0x0012e094:    f000fad4    ....    BL       sysc_cpu_qspi_en_setf ; 0x12e640
        0x0012e098:    6d20         m      LDR      r0,[r4,#0x50]
        0x0012e09a:    4e21        !N      LDR      r6,[pc,#132] ; [0x12e120] = 0x100000
        0x0012e09c:    06c0        ..      LSLS     r0,r0,#27
        0x0012e09e:    0f41        A.      LSRS     r1,r0,#29
        0x0012e0a0:    d018        ..      BEQ      0x12e0d4 ; boot_ram_entry + 200
        0x0012e0a2:    0f43        C.      LSRS     r3,r0,#29
        0x0012e0a4:    4632        2F      MOV      r2,r6
        0x0012e0a6:    4638        8F      MOV      r0,r7
        0x0012e0a8:    6da1        .m      LDR      r1,[r4,#0x58]
        0x0012e0aa:    f000f8d7    ....    BL       flash_multi_read ; 0x12e25c
        0x0012e0ae:    481d        .H      LDR      r0,[pc,#116] ; [0x12e124] = 0x20201080
        0x0012e0b0:    6b00        .k      LDR      r0,[r0,#0x30]
        0x0012e0b2:    06c0        ..      LSLS     r0,r0,#27
        0x0012e0b4:    0ec0        ..      LSRS     r0,r0,#27
        0x0012e0b6:    2814        .(      CMP      r0,#0x14
        0x0012e0b8:    d312        ..      BCC      0x12e0e0 ; boot_ram_entry + 212
        0x0012e0ba:    21ff        .!      MOVS     r1,#0xff
        0x0012e0bc:    318e        .1      ADDS     r1,r1,#0x8e
        0x0012e0be:    e011        ..      B        0x12e0e4 ; boot_ram_entry + 216
        0x0012e0c0:    200f        .       MOVS     r0,#0xf
        0x0012e0c2:    f000fabd    ....    BL       sysc_cpu_qspi_en_setf ; 0x12e640
        0x0012e0c6:    f000f91b    ....    BL       flash_quad_enable_check ; 0x12e300
        0x0012e0ca:    2800        .(      CMP      r0,#0
        0x0012e0cc:    d1e4        ..      BNE      0x12e098 ; boot_ram_entry + 140
        0x0012e0ce:    f000f911    ....    BL       flash_quad_enable_01 ; 0x12e2f4
        0x0012e0d2:    e7e1        ..      B        0x12e098 ; boot_ram_entry + 140
        0x0012e0d4:    4632        2F      MOV      r2,r6
        0x0012e0d6:    4638        8F      MOV      r0,r7
        0x0012e0d8:    6da1        .m      LDR      r1,[r4,#0x58]
        0x0012e0da:    f000f91b    ....    BL       flash_read ; 0x12e314
        0x0012e0de:    e7e6        ..      B        0x12e0ae ; boot_ram_entry + 162
        0x0012e0e0:    21ff        .!      MOVS     r1,#0xff
        0x0012e0e2:    31b2        .1      ADDS     r1,r1,#0xb2
        0x0012e0e4:    4620         F      MOV      r0,r4
        0x0012e0e6:    f000fa7f    ....    BL       ram_code_crc_check ; 0x12e5e8
        0x0012e0ea:    2800        .(      CMP      r0,#0
        0x0012e0ec:    d103        ..      BNE      0x12e0f6 ; boot_ram_entry + 234
        0x0012e0ee:    490d        .I      LDR      r1,[pc,#52] ; [0x12e124] = 0x20201080
        0x0012e0f0:    2002        .       MOVS     r0,#2
        0x0012e0f2:    3940        @9      SUBS     r1,r1,#0x40
        0x0012e0f4:    6008        .`      STR      r0,[r1,#0]
        0x0012e0f6:    6830        0h      LDR      r0,[r6,#0]
        0x0012e0f8:    f3808808    ....    MSR      MSP,r0
        0x0012e0fc:    4808        .H      LDR      r0,[pc,#32] ; [0x12e120] = 0x100000
        0x0012e0fe:    6d21        !m      LDR      r1,[r4,#0x50]
        0x0012e100:    1d00        ..      ADDS     r0,r0,#4
        0x0012e102:    06c9        ..      LSLS     r1,r1,#27
        0x0012e104:    0f4a        J.      LSRS     r2,r1,#29
        0x0012e106:    4905        .I      LDR      r1,[pc,#20] ; [0x12e11c] = 0x12dff0
        0x0012e108:    6949        Ii      LDR      r1,[r1,#0x14]
        0x0012e10a:    6804        .h      LDR      r4,[r0,#0]
        0x0012e10c:    4b06        .K      LDR      r3,[pc,#24] ; [0x12e128] = 0x3399
        0x0012e10e:    4628        (F      MOV      r0,r5
        0x0012e110:    47a0        .G      BLX      r4
        0x0012e112:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012e114:    20149000    ...     DCD    538218496
        0x0012e118:    0012e698    ....    DCD    1238680
        0x0012e11c:    0012dff0    ....    DCD    1236976
        0x0012e120:    00100000    ....    DCD    1048576
        0x0012e124:    20201080    ..      DCD    538972288
        0x0012e128:    00003399    .3..    DCD    13209
    $t
    i.dmac_block_ts_config
    dmac_block_ts_config
        0x0012e12c:    b510        ..      PUSH     {r4,lr}
        0x0012e12e:    4a0b        .J      LDR      r2,[pc,#44] ; [0x12e15c] = 0x20120000
        0x0012e130:    2458        X$      MOVS     r4,#0x58
        0x0012e132:    4361        aC      MULS     r1,r4,r1
        0x0012e134:    4b08        .K      LDR      r3,[pc,#32] ; [0x12e158] = 0xfff
        0x0012e136:    1889        ..      ADDS     r1,r1,r2
        0x0012e138:    69ca        .i      LDR      r2,[r1,#0x1c]
        0x0012e13a:    4298        .B      CMP      r0,r3
        0x0012e13c:    d904        ..      BLS      0x12e148 ; dmac_block_ts_config + 28
        0x0012e13e:    431a        .C      ORRS     r2,r2,r3
        0x0012e140:    61ca        .a      STR      r2,[r1,#0x1c]
        0x0012e142:    4259        YB      RSBS     r1,r3,#0
        0x0012e144:    1840        @.      ADDS     r0,r0,r1
        0x0012e146:    bd10        ..      POP      {r4,pc}
        0x0012e148:    0500        ..      LSLS     r0,r0,#20
        0x0012e14a:    0d00        ..      LSRS     r0,r0,#20
        0x0012e14c:    0b12        ..      LSRS     r2,r2,#12
        0x0012e14e:    0312        ..      LSLS     r2,r2,#12
        0x0012e150:    4310        .C      ORRS     r0,r0,r2
        0x0012e152:    61c8        .a      STR      r0,[r1,#0x1c]
        0x0012e154:    2000        .       MOVS     r0,#0
        0x0012e156:    bd10        ..      POP      {r4,pc}
    $d
        0x0012e158:    00000fff    ....    DCD    4095
        0x0012e15c:    20120000    ...     DCD    538050560
    $t
    i.dmac_init
    dmac_init
        0x0012e160:    4903        .I      LDR      r1,[pc,#12] ; [0x12e170] = 0x20120380
        0x0012e162:    2001        .       MOVS     r0,#1
        0x0012e164:    6188        .a      STR      r0,[r1,#0x18]
        0x0012e166:    4902        .I      LDR      r1,[pc,#8] ; [0x12e170] = 0x20120380
        0x0012e168:    20ff        .       MOVS     r0,#0xff
        0x0012e16a:    3980        .9      SUBS     r1,r1,#0x80
        0x0012e16c:    6388        .c      STR      r0,[r1,#0x38]
        0x0012e16e:    4770        pG      BX       lr
    $d
        0x0012e170:    20120380    ...     DCD    538051456
    $t
    i.dmac_start
    dmac_start
        0x0012e174:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012e176:    4604        .F      MOV      r4,r0
        0x0012e178:    460f        .F      MOV      r7,r1
        0x0012e17a:    6880        .h      LDR      r0,[r0,#8]
        0x0012e17c:    f7ffffd6    ....    BL       dmac_block_ts_config ; 0x12e12c
        0x0012e180:    4602        .F      MOV      r2,r0
        0x0012e182:    68e0        .h      LDR      r0,[r4,#0xc]
        0x0012e184:    0441        A.      LSLS     r1,r0,#17
        0x0012e186:    0f49        I.      LSRS     r1,r1,#29
        0x0012e188:    d008        ..      BEQ      0x12e19c ; dmac_start + 40
        0x0012e18a:    2901        .)      CMP      r1,#1
        0x0012e18c:    d003        ..      BEQ      0x12e196 ; dmac_start + 34
        0x0012e18e:    2902        .)      CMP      r1,#2
        0x0012e190:    d106        ..      BNE      0x12e1a0 ; dmac_start + 44
        0x0012e192:    2602        .&      MOVS     r6,#2
        0x0012e194:    e003        ..      B        0x12e19e ; dmac_start + 42
        0x0012e196:    2600        .&      MOVS     r6,#0
        0x0012e198:    2502        .%      MOVS     r5,#2
        0x0012e19a:    e001        ..      B        0x12e1a0 ; dmac_start + 44
        0x0012e19c:    2600        .&      MOVS     r6,#0
        0x0012e19e:    2500        .%      MOVS     r5,#0
        0x0012e1a0:    0481        ..      LSLS     r1,r0,#18
        0x0012e1a2:    05c3        ..      LSLS     r3,r0,#23
        0x0012e1a4:    0f89        ..      LSRS     r1,r1,#30
        0x0012e1a6:    0f5b        [.      LSRS     r3,r3,#29
        0x0012e1a8:    0509        ..      LSLS     r1,r1,#20
        0x0012e1aa:    039b        ..      LSLS     r3,r3,#14
        0x0012e1ac:    4319        .C      ORRS     r1,r1,r3
        0x0012e1ae:    0503        ..      LSLS     r3,r0,#20
        0x0012e1b0:    0f5b        [.      LSRS     r3,r3,#29
        0x0012e1b2:    02db        ..      LSLS     r3,r3,#11
        0x0012e1b4:    4319        .C      ORRS     r1,r1,r3
        0x0012e1b6:    07b3        ..      LSLS     r3,r6,#30
        0x0012e1b8:    0d5b        [.      LSRS     r3,r3,#21
        0x0012e1ba:    4319        .C      ORRS     r1,r1,r3
        0x0012e1bc:    07ab        ..      LSLS     r3,r5,#30
        0x0012e1be:    0ddb        ..      LSRS     r3,r3,#23
        0x0012e1c0:    4319        .C      ORRS     r1,r1,r3
        0x0012e1c2:    0743        C.      LSLS     r3,r0,#29
        0x0012e1c4:    0e5b        [.      LSRS     r3,r3,#25
        0x0012e1c6:    0680        ..      LSLS     r0,r0,#26
        0x0012e1c8:    4319        .C      ORRS     r1,r1,r3
        0x0012e1ca:    0f40        @.      LSRS     r0,r0,#29
        0x0012e1cc:    0040        @.      LSLS     r0,r0,#1
        0x0012e1ce:    4301        .C      ORRS     r1,r1,r0
        0x0012e1d0:    2058        X       MOVS     r0,#0x58
        0x0012e1d2:    4b0f        .K      LDR      r3,[pc,#60] ; [0x12e210] = 0x20120000
        0x0012e1d4:    4378        xC      MULS     r0,r7,r0
        0x0012e1d6:    18c0        ..      ADDS     r0,r0,r3
        0x0012e1d8:    6181        .a      STR      r1,[r0,#0x18]
        0x0012e1da:    2100        .!      MOVS     r1,#0
        0x0012e1dc:    6401        .d      STR      r1,[r0,#0x40]
        0x0012e1de:    68e1        .h      LDR      r1,[r4,#0xc]
        0x0012e1e0:    024b        K.      LSLS     r3,r1,#9
        0x0012e1e2:    0349        I.      LSLS     r1,r1,#13
        0x0012e1e4:    0f1b        ..      LSRS     r3,r3,#28
        0x0012e1e6:    0f09        ..      LSRS     r1,r1,#28
        0x0012e1e8:    02db        ..      LSLS     r3,r3,#11
        0x0012e1ea:    01c9        ..      LSLS     r1,r1,#7
        0x0012e1ec:    430b        .C      ORRS     r3,r3,r1
        0x0012e1ee:    1c9b        ..      ADDS     r3,r3,#2
        0x0012e1f0:    6443        Cd      STR      r3,[r0,#0x44]
        0x0012e1f2:    6821        !h      LDR      r1,[r4,#0]
        0x0012e1f4:    6001        .`      STR      r1,[r0,#0]
        0x0012e1f6:    6861        ah      LDR      r1,[r4,#4]
        0x0012e1f8:    6081        .`      STR      r1,[r0,#8]
        0x0012e1fa:    2001        .       MOVS     r0,#1
        0x0012e1fc:    40b8        .@      LSLS     r0,r0,r7
        0x0012e1fe:    0681        ..      LSLS     r1,r0,#26
        0x0012e200:    0e89        ..      LSRS     r1,r1,#26
        0x0012e202:    0680        ..      LSLS     r0,r0,#26
        0x0012e204:    0c80        ..      LSRS     r0,r0,#18
        0x0012e206:    4301        .C      ORRS     r1,r1,r0
        0x0012e208:    4802        .H      LDR      r0,[pc,#8] ; [0x12e214] = 0x20120380
        0x0012e20a:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e20c:    4610        .F      MOV      r0,r2
        0x0012e20e:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012e210:    20120000    ...     DCD    538050560
        0x0012e214:    20120380    ...     DCD    538051456
    $t
    i.dmac_transfer_wait
    dmac_transfer_wait
        0x0012e218:    b5f3        ..      PUSH     {r0,r1,r4-r7,lr}
        0x0012e21a:    b081        ..      SUB      sp,sp,#4
        0x0012e21c:    9902        ..      LDR      r1,[sp,#8]
        0x0012e21e:    4e0e        .N      LDR      r6,[pc,#56] ; [0x12e258] = 0x201202c0
        0x0012e220:    2401        .$      MOVS     r4,#1
        0x0012e222:    408c        .@      LSLS     r4,r4,r1
        0x0012e224:    6831        1h      LDR      r1,[r6,#0]
        0x0012e226:    4221        !B      TST      r1,r4
        0x0012e228:    d0fc        ..      BEQ      0x12e224 ; dmac_transfer_wait + 12
        0x0012e22a:    4f0b        .O      LDR      r7,[pc,#44] ; [0x12e258] = 0x201202c0
        0x0012e22c:    3740        @7      ADDS     r7,r7,#0x40
        0x0012e22e:    63bc        .c      STR      r4,[r7,#0x38]
        0x0012e230:    06a5        ..      LSLS     r5,r4,#26
        0x0012e232:    0ead        ..      LSRS     r5,r5,#26
        0x0012e234:    06a1        ..      LSLS     r1,r4,#26
        0x0012e236:    0c89        ..      LSRS     r1,r1,#18
        0x0012e238:    430d        .C      ORRS     r5,r5,r1
        0x0012e23a:    e009        ..      B        0x12e250 ; dmac_transfer_wait + 56
        0x0012e23c:    9902        ..      LDR      r1,[sp,#8]
        0x0012e23e:    f7ffff75    ..u.    BL       dmac_block_ts_config ; 0x12e12c
        0x0012e242:    4905        .I      LDR      r1,[pc,#20] ; [0x12e258] = 0x201202c0
        0x0012e244:    31c0        .1      ADDS     r1,r1,#0xc0
        0x0012e246:    620d        .b      STR      r5,[r1,#0x20]
        0x0012e248:    6831        1h      LDR      r1,[r6,#0]
        0x0012e24a:    4221        !B      TST      r1,r4
        0x0012e24c:    d0fc        ..      BEQ      0x12e248 ; dmac_transfer_wait + 48
        0x0012e24e:    63bc        .c      STR      r4,[r7,#0x38]
        0x0012e250:    2800        .(      CMP      r0,#0
        0x0012e252:    d1f3        ..      BNE      0x12e23c ; dmac_transfer_wait + 36
        0x0012e254:    bdfe        ..      POP      {r1-r7,pc}
    $d
        0x0012e256:    0000        ..      DCW    0
        0x0012e258:    201202c0    ...     DCD    538051264
    $t
    i.flash_multi_read
    flash_multi_read
        0x0012e25c:    b57f        ..      PUSH     {r0-r6,lr}
        0x0012e25e:    4c21        !L      LDR      r4,[pc,#132] ; [0x12e2e4] = 0x12e658
        0x0012e260:    0200        ..      LSLS     r0,r0,#8
        0x0012e262:    68a5        .h      LDR      r5,[r4,#8]
        0x0012e264:    68e4        .h      LDR      r4,[r4,#0xc]
        0x0012e266:    9101        ..      STR      r1,[sp,#4]
        0x0012e268:    b2e9        ..      UXTB     r1,r5
        0x0012e26a:    4301        .C      ORRS     r1,r1,r0
        0x0012e26c:    9200        ..      STR      r2,[sp,#0]
        0x0012e26e:    220c        ."      MOVS     r2,#0xc
        0x0012e270:    9102        ..      STR      r1,[sp,#8]
        0x0012e272:    20f0        .       MOVS     r0,#0xf0
        0x0012e274:    0191        ..      LSLS     r1,r2,#6
        0x0012e276:    9403        ..      STR      r4,[sp,#0xc]
        0x0012e278:    2b01        .+      CMP      r3,#1
        0x0012e27a:    d006        ..      BEQ      0x12e28a ; flash_multi_read + 46
        0x0012e27c:    2b02        .+      CMP      r3,#2
        0x0012e27e:    d00a        ..      BEQ      0x12e296 ; flash_multi_read + 58
        0x0012e280:    2b03        .+      CMP      r3,#3
        0x0012e282:    d016        ..      BEQ      0x12e2b2 ; flash_multi_read + 86
        0x0012e284:    2b04        .+      CMP      r3,#4
        0x0012e286:    d10e        ..      BNE      0x12e2a6 ; flash_multi_read + 74
        0x0012e288:    e01e        ..      B        0x12e2c8 ; flash_multi_read + 108
        0x0012e28a:    233b        ;#      MOVS     r3,#0x3b
        0x0012e28c:    466c        lF      MOV      r4,sp
        0x0012e28e:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e290:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e292:    4393        .C      BICS     r3,r3,r2
        0x0012e294:    e013        ..      B        0x12e2be ; flash_multi_read + 98
        0x0012e296:    236b        k#      MOVS     r3,#0x6b
        0x0012e298:    466c        lF      MOV      r4,sp
        0x0012e29a:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e29c:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e29e:    4393        .C      BICS     r3,r3,r2
        0x0012e2a0:    4383        .C      BICS     r3,r3,r0
        0x0012e2a2:    e019        ..      B        0x12e2d8 ; flash_multi_read + 124
        0x0012e2a4:    81e3        ..      STRH     r3,[r4,#0xe]
        0x0012e2a6:    4668        hF      MOV      r0,sp
        0x0012e2a8:    f000f8ae    ....    BL       qspi_multi_read ; 0x12e408
        0x0012e2ac:    2000        .       MOVS     r0,#0
        0x0012e2ae:    b004        ..      ADD      sp,sp,#0x10
        0x0012e2b0:    bd70        p.      POP      {r4-r6,pc}
        0x0012e2b2:    23bb        .#      MOVS     r3,#0xbb
        0x0012e2b4:    466c        lF      MOV      r4,sp
        0x0012e2b6:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e2b8:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e2ba:    4393        .C      BICS     r3,r3,r2
        0x0012e2bc:    1d1b        ..      ADDS     r3,r3,#4
        0x0012e2be:    4383        .C      BICS     r3,r3,r0
        0x0012e2c0:    438b        .C      BICS     r3,r3,r1
        0x0012e2c2:    33ff        .3      ADDS     r3,r3,#0xff
        0x0012e2c4:    3301        .3      ADDS     r3,#1
        0x0012e2c6:    e7ed        ..      B        0x12e2a4 ; flash_multi_read + 72
        0x0012e2c8:    23eb        .#      MOVS     r3,#0xeb
        0x0012e2ca:    466c        lF      MOV      r4,sp
        0x0012e2cc:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e2ce:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e2d0:    4393        .C      BICS     r3,r3,r2
        0x0012e2d2:    1d1b        ..      ADDS     r3,r3,#4
        0x0012e2d4:    4383        .C      BICS     r3,r3,r0
        0x0012e2d6:    3340        @3      ADDS     r3,r3,#0x40
        0x0012e2d8:    438b        .C      BICS     r3,r3,r1
        0x0012e2da:    33ff        .3      ADDS     r3,r3,#0xff
        0x0012e2dc:    33ff        .3      ADDS     r3,r3,#0xff
        0x0012e2de:    3302        .3      ADDS     r3,#2
        0x0012e2e0:    e7e0        ..      B        0x12e2a4 ; flash_multi_read + 72
    $d
        0x0012e2e2:    0000        ..      DCW    0
        0x0012e2e4:    0012e658    X...    DCD    1238616
    $t
    i.flash_operation_wait
    flash_operation_wait
        0x0012e2e8:    b510        ..      PUSH     {r4,lr}
        0x0012e2ea:    f000f84b    ..K.    BL       flash_read_status_reg_1 ; 0x12e384
        0x0012e2ee:    07c1        ..      LSLS     r1,r0,#31
        0x0012e2f0:    d1fb        ..      BNE      0x12e2ea ; flash_operation_wait + 2
        0x0012e2f2:    bd10        ..      POP      {r4,pc}
    i.flash_quad_enable_01
    flash_quad_enable_01
        0x0012e2f4:    b510        ..      PUSH     {r4,lr}
        0x0012e2f6:    2102        .!      MOVS     r1,#2
        0x0012e2f8:    2000        .       MOVS     r0,#0
        0x0012e2fa:    f000f867    ..g.    BL       flash_write_status_reg_2bytes ; 0x12e3cc
        0x0012e2fe:    bd10        ..      POP      {r4,pc}
    i.flash_quad_enable_check
    flash_quad_enable_check
        0x0012e300:    b510        ..      PUSH     {r4,lr}
        0x0012e302:    f000f84c    ..L.    BL       flash_read_status_reg_2 ; 0x12e39e
        0x0012e306:    0780        ..      LSLS     r0,r0,#30
        0x0012e308:    d501        ..      BPL      0x12e30e ; flash_quad_enable_check + 14
        0x0012e30a:    2001        .       MOVS     r0,#1
        0x0012e30c:    bd10        ..      POP      {r4,pc}
        0x0012e30e:    2000        .       MOVS     r0,#0
        0x0012e310:    bd10        ..      POP      {r4,pc}
        0x0012e312:    0000        ..      MOVS     r0,r0
    i.flash_read
    flash_read
        0x0012e314:    b5fe        ..      PUSH     {r1-r7,lr}
        0x0012e316:    4604        .F      MOV      r4,r0
        0x0012e318:    0c08        ..      LSRS     r0,r1,#16
        0x0012e31a:    9002        ..      STR      r0,[sp,#8]
        0x0012e31c:    b288        ..      UXTH     r0,r1
        0x0012e31e:    2500        .%      MOVS     r5,#0
        0x0012e320:    2701        .'      MOVS     r7,#1
        0x0012e322:    4616        .F      MOV      r6,r2
        0x0012e324:    043f        ?.      LSLS     r7,r7,#16
        0x0012e326:    9001        ..      STR      r0,[sp,#4]
        0x0012e328:    e012        ..      B        0x12e350 ; flash_read + 60
        0x0012e32a:    a015        ..      ADR      r0,{pc}+0x56 ; 0x12e380
        0x0012e32c:    6800        .h      LDR      r0,[r0,#0]
        0x0012e32e:    9000        ..      STR      r0,[sp,#0]
        0x0012e330:    0c20         .      LSRS     r0,r4,#16
        0x0012e332:    4669        iF      MOV      r1,sp
        0x0012e334:    7048        Hp      STRB     r0,[r1,#1]
        0x0012e336:    0a20         .      LSRS     r0,r4,#8
        0x0012e338:    7088        .p      STRB     r0,[r1,#2]
        0x0012e33a:    70cc        .p      STRB     r4,[r1,#3]
        0x0012e33c:    463b        ;F      MOV      r3,r7
        0x0012e33e:    4632        2F      MOV      r2,r6
        0x0012e340:    2104        .!      MOVS     r1,#4
        0x0012e342:    4668        hF      MOV      r0,sp
        0x0012e344:    f000f90a    ....    BL       qspi_std_read ; 0x12e55c
        0x0012e348:    19e4        ..      ADDS     r4,r4,r7
        0x0012e34a:    19f6        ..      ADDS     r6,r6,r7
        0x0012e34c:    1c6d        m.      ADDS     r5,r5,#1
        0x0012e34e:    b2ad        ..      UXTH     r5,r5
        0x0012e350:    9802        ..      LDR      r0,[sp,#8]
        0x0012e352:    4285        .B      CMP      r5,r0
        0x0012e354:    d3e9        ..      BCC      0x12e32a ; flash_read + 22
        0x0012e356:    9801        ..      LDR      r0,[sp,#4]
        0x0012e358:    2800        .(      CMP      r0,#0
        0x0012e35a:    d00e        ..      BEQ      0x12e37a ; flash_read + 102
        0x0012e35c:    a008        ..      ADR      r0,{pc}+0x24 ; 0x12e380
        0x0012e35e:    6800        .h      LDR      r0,[r0,#0]
        0x0012e360:    9000        ..      STR      r0,[sp,#0]
        0x0012e362:    0c20         .      LSRS     r0,r4,#16
        0x0012e364:    4669        iF      MOV      r1,sp
        0x0012e366:    7048        Hp      STRB     r0,[r1,#1]
        0x0012e368:    0a20         .      LSRS     r0,r4,#8
        0x0012e36a:    7088        .p      STRB     r0,[r1,#2]
        0x0012e36c:    70cc        .p      STRB     r4,[r1,#3]
        0x0012e36e:    4632        2F      MOV      r2,r6
        0x0012e370:    2104        .!      MOVS     r1,#4
        0x0012e372:    4668        hF      MOV      r0,sp
        0x0012e374:    9b01        ..      LDR      r3,[sp,#4]
        0x0012e376:    f000f8f1    ....    BL       qspi_std_read ; 0x12e55c
        0x0012e37a:    2000        .       MOVS     r0,#0
        0x0012e37c:    bdfe        ..      POP      {r1-r7,pc}
    $d
        0x0012e37e:    0000        ..      DCW    0
        0x0012e380:    00000003    ....    DCD    3
    $t
    i.flash_read_status_reg_1
    flash_read_status_reg_1
        0x0012e384:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012e386:    2005        .       MOVS     r0,#5
        0x0012e388:    4669        iF      MOV      r1,sp
        0x0012e38a:    7108        .q      STRB     r0,[r1,#4]
        0x0012e38c:    2301        .#      MOVS     r3,#1
        0x0012e38e:    466a        jF      MOV      r2,sp
        0x0012e390:    4619        .F      MOV      r1,r3
        0x0012e392:    a801        ..      ADD      r0,sp,#4
        0x0012e394:    f000f8e2    ....    BL       qspi_std_read ; 0x12e55c
        0x0012e398:    4669        iF      MOV      r1,sp
        0x0012e39a:    7808        .x      LDRB     r0,[r1,#0]
        0x0012e39c:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_read_status_reg_2
    flash_read_status_reg_2
        0x0012e39e:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012e3a0:    2035        5       MOVS     r0,#0x35
        0x0012e3a2:    4669        iF      MOV      r1,sp
        0x0012e3a4:    7108        .q      STRB     r0,[r1,#4]
        0x0012e3a6:    2301        .#      MOVS     r3,#1
        0x0012e3a8:    466a        jF      MOV      r2,sp
        0x0012e3aa:    4619        .F      MOV      r1,r3
        0x0012e3ac:    a801        ..      ADD      r0,sp,#4
        0x0012e3ae:    f000f8d5    ....    BL       qspi_std_read ; 0x12e55c
        0x0012e3b2:    4669        iF      MOV      r1,sp
        0x0012e3b4:    7808        .x      LDRB     r0,[r1,#0]
        0x0012e3b6:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_write_enable
    flash_write_enable
        0x0012e3b8:    b508        ..      PUSH     {r3,lr}
        0x0012e3ba:    2006        .       MOVS     r0,#6
        0x0012e3bc:    4669        iF      MOV      r1,sp
        0x0012e3be:    7008        .p      STRB     r0,[r1,#0]
        0x0012e3c0:    2101        .!      MOVS     r1,#1
        0x0012e3c2:    4668        hF      MOV      r0,sp
        0x0012e3c4:    f000f8fe    ....    BL       qspi_std_write ; 0x12e5c4
        0x0012e3c8:    bd08        ..      POP      {r3,pc}
        0x0012e3ca:    0000        ..      MOVS     r0,r0
    i.flash_write_status_reg_2bytes
    flash_write_status_reg_2bytes
        0x0012e3cc:    b508        ..      PUSH     {r3,lr}
        0x0012e3ce:    a207        ..      ADR      r2,{pc}+0x1e ; 0x12e3ec
        0x0012e3d0:    6812        .h      LDR      r2,[r2,#0]
        0x0012e3d2:    9200        ..      STR      r2,[sp,#0]
        0x0012e3d4:    466a        jF      MOV      r2,sp
        0x0012e3d6:    7050        Pp      STRB     r0,[r2,#1]
        0x0012e3d8:    7091        .p      STRB     r1,[r2,#2]
        0x0012e3da:    f7ffffed    ....    BL       flash_write_enable ; 0x12e3b8
        0x0012e3de:    2103        .!      MOVS     r1,#3
        0x0012e3e0:    4668        hF      MOV      r0,sp
        0x0012e3e2:    f000f8ef    ....    BL       qspi_std_write ; 0x12e5c4
        0x0012e3e6:    f7ffff7f    ....    BL       flash_operation_wait ; 0x12e2e8
        0x0012e3ea:    bd08        ..      POP      {r3,pc}
    $d
        0x0012e3ec:    00000001    ....    DCD    1
    $t
    i.qspi_init
    qspi_init
        0x0012e3f0:    4a04        .J      LDR      r2,[pc,#16] ; [0x12e404] = 0x20300000
        0x0012e3f2:    2300        .#      MOVS     r3,#0
        0x0012e3f4:    6093        .`      STR      r3,[r2,#8]
        0x0012e3f6:    6150        Pa      STR      r0,[r2,#0x14]
        0x0012e3f8:    4802        .H      LDR      r0,[pc,#8] ; [0x12e404] = 0x20300000
        0x0012e3fa:    30c0        .0      ADDS     r0,r0,#0xc0
        0x0012e3fc:    6301        .c      STR      r1,[r0,#0x30]
        0x0012e3fe:    6113        .a      STR      r3,[r2,#0x10]
        0x0012e400:    4770        pG      BX       lr
    $d
        0x0012e402:    0000        ..      DCW    0
        0x0012e404:    20300000    ..0     DCD    540016640
    $t
    i.qspi_multi_read
    qspi_multi_read
        0x0012e408:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012e40a:    4604        .F      MOV      r4,r0
        0x0012e40c:    6840        @h      LDR      r0,[r0,#4]
        0x0012e40e:    b085        ..      SUB      sp,sp,#0x14
        0x0012e410:    0885        ..      LSRS     r5,r0,#2
        0x0012e412:    0786        ..      LSLS     r6,r0,#30
        0x0012e414:    7ba0        .{      LDRB     r0,[r4,#0xe]
        0x0012e416:    0fb6        ..      LSRS     r6,r6,#30
        0x0012e418:    0601        ..      LSLS     r1,r0,#24
        0x0012e41a:    0782        ..      LSLS     r2,r0,#30
        0x0012e41c:    0f09        ..      LSRS     r1,r1,#28
        0x0012e41e:    02c9        ..      LSLS     r1,r1,#11
        0x0012e420:    0d92        ..      LSRS     r2,r2,#22
        0x0012e422:    0700        ..      LSLS     r0,r0,#28
        0x0012e424:    4311        .C      ORRS     r1,r1,r2
        0x0012e426:    0f80        ..      LSRS     r0,r0,#30
        0x0012e428:    4301        .C      ORRS     r1,r1,r0
        0x0012e42a:    2020                MOVS     r0,#0x20
        0x0012e42c:    4301        .C      ORRS     r1,r1,r0
        0x0012e42e:    4831        1H      LDR      r0,[pc,#196] ; [0x12e4f4] = 0x203000c0
        0x0012e430:    6341        Ac      STR      r1,[r0,#0x34]
        0x0012e432:    4f30        0O      LDR      r7,[pc,#192] ; [0x12e4f4] = 0x203000c0
        0x0012e434:    3fc0        .?      SUBS     r7,r7,#0xc0
        0x0012e436:    2d00        .-      CMP      r5,#0
        0x0012e438:    d029        ).      BEQ      0x12e48e ; qspi_multi_read + 134
        0x0012e43a:    89e0        ..      LDRH     r0,[r4,#0xe]
        0x0012e43c:    492e        .I      LDR      r1,[pc,#184] ; [0x12e4f8] = 0x1f0200
        0x0012e43e:    0580        ..      LSLS     r0,r0,#22
        0x0012e440:    0f80        ..      LSRS     r0,r0,#30
        0x0012e442:    0540        @.      LSLS     r0,r0,#21
        0x0012e444:    1840        @.      ADDS     r0,r0,r1
        0x0012e446:    6038        8`      STR      r0,[r7,#0]
        0x0012e448:    1e68        h.      SUBS     r0,r5,#1
        0x0012e44a:    6078        x`      STR      r0,[r7,#4]
        0x0012e44c:    4829        )H      LDR      r0,[pc,#164] ; [0x12e4f4] = 0x203000c0
        0x0012e44e:    2107        .!      MOVS     r1,#7
        0x0012e450:    3880        .8      SUBS     r0,r0,#0x80
        0x0012e452:    6141        Aa      STR      r1,[r0,#0x14]
        0x0012e454:    2100        .!      MOVS     r1,#0
        0x0012e456:    60c1        .`      STR      r1,[r0,#0xc]
        0x0012e458:    2101        .!      MOVS     r1,#1
        0x0012e45a:    60c1        .`      STR      r1,[r0,#0xc]
        0x0012e45c:    60b9        .`      STR      r1,[r7,#8]
        0x0012e45e:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x0012e460:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e462:    68a1        .h      LDR      r1,[r4,#8]
        0x0012e464:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e466:    4825        %H      LDR      r0,[pc,#148] ; [0x12e4fc] = 0x12e688
        0x0012e468:    c80f        ..      LDM      r0,{r0-r3}
        0x0012e46a:    9000        ..      STR      r0,[sp,#0]
        0x0012e46c:    a801        ..      ADD      r0,sp,#4
        0x0012e46e:    c00e        ..      STM      r0!,{r1-r3}
        0x0012e470:    6820         h      LDR      r0,[r4,#0]
        0x0012e472:    9001        ..      STR      r0,[sp,#4]
        0x0012e474:    2100        .!      MOVS     r1,#0
        0x0012e476:    9502        ..      STR      r5,[sp,#8]
        0x0012e478:    4668        hF      MOV      r0,sp
        0x0012e47a:    f7fffe7b    ..{.    BL       dmac_start ; 0x12e174
        0x0012e47e:    2101        .!      MOVS     r1,#1
        0x0012e480:    6139        9a      STR      r1,[r7,#0x10]
        0x0012e482:    2100        .!      MOVS     r1,#0
        0x0012e484:    f7fffec8    ....    BL       dmac_transfer_wait ; 0x12e218
        0x0012e488:    2000        .       MOVS     r0,#0
        0x0012e48a:    60b8        .`      STR      r0,[r7,#8]
        0x0012e48c:    6138        8a      STR      r0,[r7,#0x10]
        0x0012e48e:    2e00        ..      CMP      r6,#0
        0x0012e490:    d02d        -.      BEQ      0x12e4ee ; qspi_multi_read + 230
        0x0012e492:    89e0        ..      LDRH     r0,[r4,#0xe]
        0x0012e494:    491a        .I      LDR      r1,[pc,#104] ; [0x12e500] = 0x70200
        0x0012e496:    0580        ..      LSLS     r0,r0,#22
        0x0012e498:    0f80        ..      LSRS     r0,r0,#30
        0x0012e49a:    0540        @.      LSLS     r0,r0,#21
        0x0012e49c:    1840        @.      ADDS     r0,r0,r1
        0x0012e49e:    6038        8`      STR      r0,[r7,#0]
        0x0012e4a0:    1e70        p.      SUBS     r0,r6,#1
        0x0012e4a2:    6078        x`      STR      r0,[r7,#4]
        0x0012e4a4:    4813        .H      LDR      r0,[pc,#76] ; [0x12e4f4] = 0x203000c0
        0x0012e4a6:    2100        .!      MOVS     r1,#0
        0x0012e4a8:    3880        .8      SUBS     r0,r0,#0x80
        0x0012e4aa:    60c1        .`      STR      r1,[r0,#0xc]
        0x0012e4ac:    2101        .!      MOVS     r1,#1
        0x0012e4ae:    60b9        .`      STR      r1,[r7,#8]
        0x0012e4b0:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x0012e4b2:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e4b4:    68a1        .h      LDR      r1,[r4,#8]
        0x0012e4b6:    00ab        ..      LSLS     r3,r5,#2
        0x0012e4b8:    0a0a        ..      LSRS     r2,r1,#8
        0x0012e4ba:    18d2        ..      ADDS     r2,r2,r3
        0x0012e4bc:    0212        ..      LSLS     r2,r2,#8
        0x0012e4be:    b2c9        ..      UXTB     r1,r1
        0x0012e4c0:    430a        .C      ORRS     r2,r2,r1
        0x0012e4c2:    6202        .b      STR      r2,[r0,#0x20]
        0x0012e4c4:    2101        .!      MOVS     r1,#1
        0x0012e4c6:    6139        9a      STR      r1,[r7,#0x10]
        0x0012e4c8:    6ab9        .j      LDR      r1,[r7,#0x28]
        0x0012e4ca:    0749        I.      LSLS     r1,r1,#29
        0x0012e4cc:    d5fc        ..      BPL      0x12e4c8 ; qspi_multi_read + 192
        0x0012e4ce:    6ab9        .j      LDR      r1,[r7,#0x28]
        0x0012e4d0:    07c9        ..      LSLS     r1,r1,#31
        0x0012e4d2:    d1fc        ..      BNE      0x12e4ce ; qspi_multi_read + 198
        0x0012e4d4:    6821        !h      LDR      r1,[r4,#0]
        0x0012e4d6:    18cb        ..      ADDS     r3,r1,r3
        0x0012e4d8:    2100        .!      MOVS     r1,#0
        0x0012e4da:    e003        ..      B        0x12e4e4 ; qspi_multi_read + 220
        0x0012e4dc:    6a02        .j      LDR      r2,[r0,#0x20]
        0x0012e4de:    545a        ZT      STRB     r2,[r3,r1]
        0x0012e4e0:    1c49        I.      ADDS     r1,r1,#1
        0x0012e4e2:    b2c9        ..      UXTB     r1,r1
        0x0012e4e4:    42b1        .B      CMP      r1,r6
        0x0012e4e6:    d3f9        ..      BCC      0x12e4dc ; qspi_multi_read + 212
        0x0012e4e8:    2000        .       MOVS     r0,#0
        0x0012e4ea:    60b8        .`      STR      r0,[r7,#8]
        0x0012e4ec:    6138        8a      STR      r0,[r7,#0x10]
        0x0012e4ee:    b005        ..      ADD      sp,sp,#0x14
        0x0012e4f0:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012e4f2:    0000        ..      DCW    0
        0x0012e4f4:    203000c0    ..0     DCD    540016832
        0x0012e4f8:    001f0200    ....    DCD    2032128
        0x0012e4fc:    0012e688    ....    DCD    1238664
        0x0012e500:    00070200    ....    DCD    459264
    $t
    i.qspi_std_byte_write_dma_config
    qspi_std_byte_write_dma_config
        0x0012e504:    4a05        .J      LDR      r2,[pc,#20] ; [0x12e51c] = 0x70100
        0x0012e506:    6002        .`      STR      r2,[r0,#0]
        0x0012e508:    1f09        ..      SUBS     r1,r1,#4
        0x0012e50a:    6501        .e      STR      r1,[r0,#0x50]
        0x0012e50c:    2100        .!      MOVS     r1,#0
        0x0012e50e:    64c1        .d      STR      r1,[r0,#0x4c]
        0x0012e510:    2102        .!      MOVS     r1,#2
        0x0012e512:    64c1        .d      STR      r1,[r0,#0x4c]
        0x0012e514:    2101        .!      MOVS     r1,#1
        0x0012e516:    6081        .`      STR      r1,[r0,#8]
        0x0012e518:    4770        pG      BX       lr
    $d
        0x0012e51a:    0000        ..      DCW    0
        0x0012e51c:    00070100    ....    DCD    459008
    $t
    i.qspi_std_byte_write_dma_start
    qspi_std_byte_write_dma_start
        0x0012e520:    b57f        ..      PUSH     {r0-r6,lr}
        0x0012e522:    4604        .F      MOV      r4,r0
        0x0012e524:    480c        .H      LDR      r0,[pc,#48] ; [0x12e558] = 0x12e668
        0x0012e526:    460d        .F      MOV      r5,r1
        0x0012e528:    68c0        .h      LDR      r0,[r0,#0xc]
        0x0012e52a:    9003        ..      STR      r0,[sp,#0xc]
        0x0012e52c:    4620         F      MOV      r0,r4
        0x0012e52e:    3060        `0      ADDS     r0,r0,#0x60
        0x0012e530:    9001        ..      STR      r0,[sp,#4]
        0x0012e532:    9200        ..      STR      r2,[sp,#0]
        0x0012e534:    2100        .!      MOVS     r1,#0
        0x0012e536:    9302        ..      STR      r3,[sp,#8]
        0x0012e538:    4668        hF      MOV      r0,sp
        0x0012e53a:    f7fffe1b    ....    BL       dmac_start ; 0x12e174
        0x0012e53e:    6125        %a      STR      r5,[r4,#0x10]
        0x0012e540:    2100        .!      MOVS     r1,#0
        0x0012e542:    f7fffe69    ..i.    BL       dmac_transfer_wait ; 0x12e218
        0x0012e546:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x0012e548:    0740        @.      LSLS     r0,r0,#29
        0x0012e54a:    d5fc        ..      BPL      0x12e546 ; qspi_std_byte_write_dma_start + 38
        0x0012e54c:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x0012e54e:    07c0        ..      LSLS     r0,r0,#31
        0x0012e550:    d1fc        ..      BNE      0x12e54c ; qspi_std_byte_write_dma_start + 44
        0x0012e552:    60a0        .`      STR      r0,[r4,#8]
        0x0012e554:    6120         a      STR      r0,[r4,#0x10]
        0x0012e556:    bd7f        ..      POP      {r0-r6,pc}
    $d
        0x0012e558:    0012e668    h...    DCD    1238632
    $t
    i.qspi_std_read
    qspi_std_read
        0x0012e55c:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012e55e:    4694        .F      MOV      r12,r2
        0x0012e560:    4c16        .L      LDR      r4,[pc,#88] ; [0x12e5bc] = 0x20300000
        0x0012e562:    b085        ..      SUB      sp,sp,#0x14
        0x0012e564:    4a14        .J      LDR      r2,[pc,#80] ; [0x12e5b8] = 0x70300
        0x0012e566:    6022        "`      STR      r2,[r4,#0]
        0x0012e568:    1e5a        Z.      SUBS     r2,r3,#1
        0x0012e56a:    6062        b`      STR      r2,[r4,#4]
        0x0012e56c:    4d13        .M      LDR      r5,[pc,#76] ; [0x12e5bc] = 0x20300000
        0x0012e56e:    2607        .&      MOVS     r6,#7
        0x0012e570:    3540        @5      ADDS     r5,r5,#0x40
        0x0012e572:    616e        na      STR      r6,[r5,#0x14]
        0x0012e574:    2600        .&      MOVS     r6,#0
        0x0012e576:    60ee        .`      STR      r6,[r5,#0xc]
        0x0012e578:    2601        .&      MOVS     r6,#1
        0x0012e57a:    60ee        .`      STR      r6,[r5,#0xc]
        0x0012e57c:    60a6        .`      STR      r6,[r4,#8]
        0x0012e57e:    2700        .'      MOVS     r7,#0
        0x0012e580:    e003        ..      B        0x12e58a ; qspi_std_read + 46
        0x0012e582:    5dc2        .]      LDRB     r2,[r0,r7]
        0x0012e584:    622a        *b      STR      r2,[r5,#0x20]
        0x0012e586:    1c7f        ..      ADDS     r7,r7,#1
        0x0012e588:    b2ff        ..      UXTB     r7,r7
        0x0012e58a:    428f        .B      CMP      r7,r1
        0x0012e58c:    d3f9        ..      BCC      0x12e582 ; qspi_std_read + 38
        0x0012e58e:    480c        .H      LDR      r0,[pc,#48] ; [0x12e5c0] = 0x12e668
        0x0012e590:    6901        .i      LDR      r1,[r0,#0x10]
        0x0012e592:    69c0        .i      LDR      r0,[r0,#0x1c]
        0x0012e594:    9003        ..      STR      r0,[sp,#0xc]
        0x0012e596:    4660        `F      MOV      r0,r12
        0x0012e598:    9100        ..      STR      r1,[sp,#0]
        0x0012e59a:    9001        ..      STR      r0,[sp,#4]
        0x0012e59c:    2100        .!      MOVS     r1,#0
        0x0012e59e:    9302        ..      STR      r3,[sp,#8]
        0x0012e5a0:    4668        hF      MOV      r0,sp
        0x0012e5a2:    f7fffde7    ....    BL       dmac_start ; 0x12e174
        0x0012e5a6:    6126        &a      STR      r6,[r4,#0x10]
        0x0012e5a8:    2100        .!      MOVS     r1,#0
        0x0012e5aa:    f7fffe35    ..5.    BL       dmac_transfer_wait ; 0x12e218
        0x0012e5ae:    2000        .       MOVS     r0,#0
        0x0012e5b0:    60a0        .`      STR      r0,[r4,#8]
        0x0012e5b2:    6120         a      STR      r0,[r4,#0x10]
        0x0012e5b4:    b005        ..      ADD      sp,sp,#0x14
        0x0012e5b6:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012e5b8:    00070300    ....    DCD    459520
        0x0012e5bc:    20300000    ..0     DCD    540016640
        0x0012e5c0:    0012e668    h...    DCD    1238632
    $t
    i.qspi_std_write
    qspi_std_write
        0x0012e5c4:    b570        p.      PUSH     {r4-r6,lr}
        0x0012e5c6:    4c07        .L      LDR      r4,[pc,#28] ; [0x12e5e4] = 0x20300000
        0x0012e5c8:    460d        .F      MOV      r5,r1
        0x0012e5ca:    4606        .F      MOV      r6,r0
        0x0012e5cc:    2140        @!      MOVS     r1,#0x40
        0x0012e5ce:    4620         F      MOV      r0,r4
        0x0012e5d0:    f7ffff98    ....    BL       qspi_std_byte_write_dma_config ; 0x12e504
        0x0012e5d4:    462b        +F      MOV      r3,r5
        0x0012e5d6:    4632        2F      MOV      r2,r6
        0x0012e5d8:    2101        .!      MOVS     r1,#1
        0x0012e5da:    4620         F      MOV      r0,r4
        0x0012e5dc:    f7ffffa0    ....    BL       qspi_std_byte_write_dma_start ; 0x12e520
        0x0012e5e0:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012e5e2:    0000        ..      DCW    0
        0x0012e5e4:    20300000    ..0     DCD    540016640
    $t
    i.ram_code_crc_check
    ram_code_crc_check
        0x0012e5e8:    b5fe        ..      PUSH     {r1-r7,lr}
        0x0012e5ea:    4606        .F      MOV      r6,r0
        0x0012e5ec:    6d80        .m      LDR      r0,[r0,#0x58]
        0x0012e5ee:    460f        .F      MOV      r7,r1
        0x0012e5f0:    0101        ..      LSLS     r1,r0,#4
        0x0012e5f2:    0500        ..      LSLS     r0,r0,#20
        0x0012e5f4:    0c09        ..      LSRS     r1,r1,#16
        0x0012e5f6:    0d00        ..      LSRS     r0,r0,#20
        0x0012e5f8:    2501        .%      MOVS     r5,#1
        0x0012e5fa:    052d        -.      LSLS     r5,r5,#20
        0x0012e5fc:    2400        .$      MOVS     r4,#0
        0x0012e5fe:    9101        ..      STR      r1,[sp,#4]
        0x0012e600:    9000        ..      STR      r0,[sp,#0]
        0x0012e602:    e00d        ..      B        0x12e620 ; ram_code_crc_check + 56
        0x0012e604:    2201        ."      MOVS     r2,#1
        0x0012e606:    0312        ..      LSLS     r2,r2,#12
        0x0012e608:    4629        )F      MOV      r1,r5
        0x0012e60a:    2000        .       MOVS     r0,#0
        0x0012e60c:    47b8        .G      BLX      r7
        0x0012e60e:    0061        a.      LSLS     r1,r4,#1
        0x0012e610:    5a71        qZ      LDRH     r1,[r6,r1]
        0x0012e612:    4281        .B      CMP      r1,r0
        0x0012e614:    d10f        ..      BNE      0x12e636 ; ram_code_crc_check + 78
        0x0012e616:    2001        .       MOVS     r0,#1
        0x0012e618:    0300        ..      LSLS     r0,r0,#12
        0x0012e61a:    182d        -.      ADDS     r5,r5,r0
        0x0012e61c:    1c64        d.      ADDS     r4,r4,#1
        0x0012e61e:    b2a4        ..      UXTH     r4,r4
        0x0012e620:    9801        ..      LDR      r0,[sp,#4]
        0x0012e622:    4284        .B      CMP      r4,r0
        0x0012e624:    d3ee        ..      BCC      0x12e604 ; ram_code_crc_check + 28
        0x0012e626:    4629        )F      MOV      r1,r5
        0x0012e628:    2000        .       MOVS     r0,#0
        0x0012e62a:    9a00        ..      LDR      r2,[sp,#0]
        0x0012e62c:    47b8        .G      BLX      r7
        0x0012e62e:    0061        a.      LSLS     r1,r4,#1
        0x0012e630:    5a71        qZ      LDRH     r1,[r6,r1]
        0x0012e632:    4281        .B      CMP      r1,r0
        0x0012e634:    d001        ..      BEQ      0x12e63a ; ram_code_crc_check + 82
        0x0012e636:    2000        .       MOVS     r0,#0
        0x0012e638:    bdfe        ..      POP      {r1-r7,pc}
        0x0012e63a:    2001        .       MOVS     r0,#1
        0x0012e63c:    bdfe        ..      POP      {r1-r7,pc}
        0x0012e63e:    0000        ..      MOVS     r0,r0
    i.sysc_cpu_qspi_en_setf
    sysc_cpu_qspi_en_setf
        0x0012e640:    4904        .I      LDR      r1,[pc,#16] ; [0x12e654] = 0x20132000
        0x0012e642:    6a0a        .j      LDR      r2,[r1,#0x20]
        0x0012e644:    0700        ..      LSLS     r0,r0,#28
        0x0012e646:    0912        ..      LSRS     r2,r2,#4
        0x0012e648:    0112        ..      LSLS     r2,r2,#4
        0x0012e64a:    0f00        ..      LSRS     r0,r0,#28
        0x0012e64c:    4302        .C      ORRS     r2,r2,r0
        0x0012e64e:    620a        .b      STR      r2,[r1,#0x20]
        0x0012e650:    4770        pG      BX       lr
    $d
        0x0012e652:    0000        ..      DCW    0
        0x0012e654:    20132000    . .     DCD    538124288
    $d.realdata
    .constdata
        0x0012e658:    00000000    ....    DCD    0
        0x0012e65c:    00000000    ....    DCD    0
        0x0012e660:    00000000    ....    DCD    0
        0x0012e664:    00020000    ....    DCD    131072
    .constdata
        0x0012e668:    00000000    ....    DCD    0
        0x0012e66c:    00000000    ....    DCD    0
        0x0012e670:    00000000    ....    DCD    0
        0x0012e674:    00009240    @...    DCD    37440
        0x0012e678:    20300060    `.0     DCD    540016736
        0x0012e67c:    00000000    ....    DCD    0
        0x0012e680:    00000000    ....    DCD    0
        0x0012e684:    0000a480    ....    DCD    42112
        0x0012e688:    203000ec    ..0     DCD    540016876
        0x0012e68c:    00000000    ....    DCD    0
        0x0012e690:    00000000    ....    DCD    0
        0x0012e694:    0000a482    ....    DCD    42114

====================================

** Section #2

    Name        : EXEC_SECTION
    Type        : SHT_NOBITS (0x00000008)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x0012e698
    File Offset : 1756 (0x6dc)
    Size        : 192 bytes (0xc0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0


====================================

** Section #3

    Name        : .debug_abbrev
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 1756 (0x6dc)
    Size        : 1444 bytes (0x5a4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #4

    Name        : .debug_frame
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 3200 (0xc80)
    Size        : 712 bytes (0x2c8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #5

    Name        : .debug_info
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 3912 (0xf48)
    Size        : 80032 bytes (0x138a0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #6

    Name        : .debug_line
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 83944 (0x147e8)
    Size        : 7404 bytes (0x1cec)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #7

    Name        : .debug_loc
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 91348 (0x164d4)
    Size        : 2016 bytes (0x7e0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #8

    Name        : .debug_macinfo
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 93364 (0x16cb4)
    Size        : 4120 bytes (0x1018)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #9

    Name        : .debug_pubnames
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 97484 (0x17ccc)
    Size        : 676 bytes (0x2a4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #10

    Name        : .symtab
    Type        : SHT_SYMTAB (0x00000002)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 98160 (0x17f70)
    Size        : 1456 bytes (0x5b0)
    Link        : Section 11 (.strtab)
    Info        : Last local symbol no = 71
    Alignment   : 4
    Entry Size  : 16


====================================

** Section #11

    Name        : .strtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 99616 (0x18520)
    Size        : 1552 bytes (0x610)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #12

    Name        : .note
    Type        : SHT_NOTE (0x00000007)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 101168 (0x18b30)
    Size        : 24 bytes (0x18)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

        Section     Segment
    ====================================

              1           0
              2           0

====================================

** Section #13

    Name        : .comment
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 101192 (0x18b48)
    Size        : 4876 bytes (0x130c)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0

    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --verbose --library_type=microlib --callgraph --map --symbols --xref --cpu=Cortex-M0+ --datacompressor=off --list=../../../../output/boot_ram/boot_ram.map --output=../../../../output/boot_ram/boot_ram.elf --scatter=D:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/ARMCC/boot_ram_link.txt --entry=boot_ram_entry --info=any,architecture,exceptions,inline,inputs,libraries,merge,pltgot,sizes,stack,summarysizes,summarystack,tailreorder,totals,unused,unusedsymbols,veneers,veneercallers,veneerpools,visibility,weakrefs

    Input Comments:
    
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=CMakeFiles/boot_ram.dir/boot_ram.o --vfemode=force

    Input Comments:
    
    p56c-3
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    armasm --debug --diag_suppress=1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork --no_divide 
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    
    flash.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=CMakeFiles/boot_ram.dir/dma_for_qspi.o --vfemode=force

    Input Comments:
    
    p3c24-3
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    armasm --debug --diag_suppress=1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork --no_divide 
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    
    
    
    
    qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=CMakeFiles/boot_ram.dir/qspi.o --vfemode=force

    Input Comments:
    
    p3710-3
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    armasm --debug --diag_suppress=1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork --no_divide 
    qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    
    
    
    
    
    
    

====================================

** Section #14

    Name        : .shstrtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 106068 (0x19e54)
    Size        : 148 bytes (0x94)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

address     size       variable name                            type
0x0012dff0  0x1c       boot_ram_header                          const boot_ram_header_t
0x0012dff0  0x10       boot_ram_header.header                   boot_header_t
0x0012dff0  0x4        boot_ram_header.header.bx_flag           array[4] of uint8_t
0x0012dff4  0x4        boot_ram_header.header.base_addr         pointer to uint8_t
0x0012dff8  0x4        boot_ram_header.header.length            uint32_t
0x0012dffc  0x4        boot_ram_header.header.entry_point       entry_point_t
0x0012e000  0x4        boot_ram_header.flash_total_size         volatile uint32_t
0x0012e004  0x4        boot_ram_header.nvds_base                volatile uint32_t
0x0012e008  0x4        boot_ram_header.image_limit              volatile uint32_t

address     size       variable name                            type
0x0012e698  0xc0       image_header                             array[2] of image_header_t

