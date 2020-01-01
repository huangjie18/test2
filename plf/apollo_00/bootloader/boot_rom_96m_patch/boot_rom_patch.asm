
========================================================================

** ELF Header Information

    File Name: .\Objects\boot_rom_patch.axf

    Machine class: ELFCLASS32 (32-bit)
    Data encoding: ELFDATA2LSB (Little endian)
    Header version: EV_CURRENT (Current version)
    Operating System ABI: none
    ABI Version: 0
    File Type: ET_EXEC (Executable) (2)
    Machine: EM_ARM (ARM)

    Image Entry point: 0x001280e5
    Flags: EF_ARM_HASENTRY + EF_ARM_ABI_FLOAT_SOFT (0x05000202)

    ARM ELF revision: 5 (ABI version 2)

    Conforms to Soft float procedure-call standard

    Built with
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]

    Header size: 52 bytes (0x34)
    Program header entry size: 32 bytes (0x20)
    Section header entry size: 40 bytes (0x28)

    Program header entries: 1
    Section header entries: 18

    Program header offset: 17864 (0x000045c8)
    Section header offset: 17896 (0x000045e8)

    Section header string table index: 17

========================================================================

** Program header #0

    Type          : PT_LOAD (1)
    File Offset   : 52 (0x34)
    Virtual Addr  : 0x00127ff0
    Physical Addr : 0x00127ff0
    Size in file  : 484 bytes (0x1e4)
    Size in memory: 3560 bytes (0xde8)
    Flags         : PF_X + PF_W + PF_R + PF_ARM_ENTRY (0x80000007)
    Alignment     : 8


========================================================================

** Section #1

    Name        : BOOT_HEADER_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC (0x00000002)
    Addr        : 0x00127ff0
    File Offset : 52 (0x34)
    Size        : 16 bytes (0x10)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    0x127ff0:   42 58 32 34 00 80 12 00 ff ff ff ff f9 80 12 00    BX24............


====================================

** Section #2

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_EXECINSTR (0x00000006)
    Addr        : 0x00128000
    File Offset : 68 (0x44)
    Size        : 228 bytes (0xe4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    $d.realdata
    RESET
    __Vectors
        0x00128000:    00128dd8    ....    DCD    1215960
        0x00128004:    001280f9    ....    DCD    1212665
        0x00128008:    00128101    ....    DCD    1212673
        0x0012800c:    00128101    ....    DCD    1212673
        0x00128010:    00128101    ....    DCD    1212673
        0x00128014:    00128101    ....    DCD    1212673
        0x00128018:    00128101    ....    DCD    1212673
        0x0012801c:    00000000    ....    DCD    0
        0x00128020:    00000000    ....    DCD    0
        0x00128024:    00000000    ....    DCD    0
        0x00128028:    00000000    ....    DCD    0
        0x0012802c:    00128101    ....    DCD    1212673
        0x00128030:    00128101    ....    DCD    1212673
        0x00128034:    00000000    ....    DCD    0
        0x00128038:    00128101    ....    DCD    1212673
        0x0012803c:    00128101    ....    DCD    1212673
        0x00128040:    00128101    ....    DCD    1212673
        0x00128044:    00128101    ....    DCD    1212673
        0x00128048:    00128101    ....    DCD    1212673
        0x0012804c:    00128101    ....    DCD    1212673
        0x00128050:    00128101    ....    DCD    1212673
        0x00128054:    00128101    ....    DCD    1212673
        0x00128058:    00128101    ....    DCD    1212673
        0x0012805c:    00128101    ....    DCD    1212673
        0x00128060:    00128101    ....    DCD    1212673
        0x00128064:    00128101    ....    DCD    1212673
        0x00128068:    00128101    ....    DCD    1212673
        0x0012806c:    00128101    ....    DCD    1212673
        0x00128070:    00128101    ....    DCD    1212673
        0x00128074:    00128101    ....    DCD    1212673
        0x00128078:    00128101    ....    DCD    1212673
        0x0012807c:    00128101    ....    DCD    1212673
        0x00128080:    00128101    ....    DCD    1212673
        0x00128084:    00128101    ....    DCD    1212673
    $t
    uart_6m_section
    __Vectors_End
    uart_6m_patch_c
    __tagsym$$used
        0x00128088:    b510        ..      PUSH     {r4,lr}
        0x0012808a:    4913        .I      LDR      r1,[pc,#76] ; [0x1280d8] = 0x20201040
        0x0012808c:    4811        .H      LDR      r0,[pc,#68] ; [0x1280d4] = 0x10cf5cb3
        0x0012808e:    6308        .c      STR      r0,[r1,#0x30]
        0x00128090:    4811        .H      LDR      r0,[pc,#68] ; [0x1280d8] = 0x20201040
        0x00128092:    3040        @0      ADDS     r0,r0,#0x40
        0x00128094:    6b01        .k      LDR      r1,[r0,#0x30]
        0x00128096:    0689        ..      LSLS     r1,r1,#26
        0x00128098:    d5fc        ..      BPL      0x128094 ; uart_6m_patch_c + 12
        0x0012809a:    207d        }       MOVS     r0,#0x7d
        0x0012809c:    01c0        ..      LSLS     r0,r0,#7
        0x0012809e:    f000f831    ..1.    BL       bx_delay_asm ; 0x128104
        0x001280a2:    480d        .H      LDR      r0,[pc,#52] ; [0x1280d8] = 0x20201040
        0x001280a4:    490d        .I      LDR      r1,[pc,#52] ; [0x1280dc] = 0xe00091
        0x001280a6:    3840        @8      SUBS     r0,r0,#0x40
        0x001280a8:    6001        .`      STR      r1,[r0,#0]
        0x001280aa:    bf00        ..      NOP      
        0x001280ac:    bf00        ..      NOP      
        0x001280ae:    490b        .I      LDR      r1,[pc,#44] ; [0x1280dc] = 0xe00091
        0x001280b0:    1c49        I.      ADDS     r1,r1,#1
        0x001280b2:    6001        .`      STR      r1,[r0,#0]
        0x001280b4:    2105        .!      MOVS     r1,#5
        0x001280b6:    480a        .H      LDR      r0,[pc,#40] ; [0x1280e0] = 0x20149000
        0x001280b8:    0249        I.      LSLS     r1,r1,#9
        0x001280ba:    6101        .a      STR      r1,[r0,#0x10]
        0x001280bc:    bf00        ..      NOP      
        0x001280be:    bf00        ..      NOP      
        0x001280c0:    2101        .!      MOVS     r1,#1
        0x001280c2:    0249        I.      LSLS     r1,r1,#9
        0x001280c4:    6001        .`      STR      r1,[r0,#0]
        0x001280c6:    bf00        ..      NOP      
        0x001280c8:    bf00        ..      NOP      
        0x001280ca:    2105        .!      MOVS     r1,#5
        0x001280cc:    0209        ..      LSLS     r1,r1,#8
        0x001280ce:    6101        .a      STR      r1,[r0,#0x10]
        0x001280d0:    bd10        ..      POP      {r4,pc}
    $d
        0x001280d2:    0000        ..      DCW    0
        0x001280d4:    10cf5cb3    .\..    DCD    282025139
        0x001280d8:    20201040    @.      DCD    538972224
        0x001280dc:    00e00091    ....    DCD    14680209
        0x001280e0:    20149000    ...     DCD    538218496

====================================

** Section #3

    Name        : EXEC2_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_EXECINSTR (0x00000006)
    Addr        : 0x001280e4
    File Offset : 296 (0x128)
    Size        : 236 bytes (0xec)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    $t
    .ARM.Collect$$$$00000000
    .ARM.Collect$$$$00000001
    __main
    _main_stk
        0x001280e4:    4803        .H      LDR      r0,__lit__00000000 ; [0x1280f4] = 0x128dd8
        0x001280e6:    4685        .F      MOV      sp,r0
    .ARM.Collect$$$$00000004
    _main_scatterload
        0x001280e8:    f000f814    ....    BL       __scatterload ; 0x128114
    .ARM.Collect$$$$00000008
    .ARM.Collect$$$$0000000A
    .ARM.Collect$$$$0000000B
    __main_after_scatterload
    _main_clock
    _main_cpp_init
    _main_init
        0x001280ec:    4800        .H      LDR      r0,[pc,#0] ; [0x1280f0] = 0x128159
        0x001280ee:    4700        .G      BX       r0
    $d
        0x001280f0:    00128159    Y...    DCD    1212761
    .ARM.Collect$$$$00002712
    __lit__00000000
    .ARM.Collect$$$$0000000D
    .ARM.Collect$$$$0000000F
    __rt_final_cpp
    __rt_final_exit
        0x001280f4:    00128dd8    ....    DCD    1215960
    $t
    .text
    Reset_Handler
        0x001280f8:    4804        .H      LDR      r0,[pc,#16] ; [0x12810c] = 0x128139
        0x001280fa:    4780        .G      BLX      r0
        0x001280fc:    4804        .H      LDR      r0,[pc,#16] ; [0x128110] = 0x1280e5
        0x001280fe:    4700        .G      BX       r0
    BLE_LP_IRQHandler
    BLE_MAC_IRQHandler
    BusFault_Handler
    DMAC_IRQHandler
    DebugMon_Handler
    ECC_IRQHandler
    EXT_INTR_IRQHandler
    GPIO_IRQHandler
    HardFault_Handler
    IIC0_IRQHandler
    IIC1_IRQHandler
    MemManage_Handler
    NMI_Handler
    PendSV_Handler
    QSPI_IRQHandler
    RTC_IRQHandler
    SFT_IRQHandler
    SPIM0_IRQHandler
    SPIM1_IRQHandler
    SPIS_IRQHandler
    SVC_Handler
    SysTick_Handler
    TIMER_IRQHandler
    UART0_IRQHandler
    UART1_IRQHandler
    UsageFault_Handler
    WDT_IRQHandler
        0x00128100:    e7fe        ..      B        BLE_LP_IRQHandler ; 0x128100
    $d
        0x00128102:    0000        ..      DCW    0
    $t
    bx_delay_asm
        0x00128104:    1e40        @.      SUBS     r0,r0,#1
        0x00128106:    2800        .(      CMP      r0,#0
        0x00128108:    d1fc        ..      BNE      bx_delay_asm ; 0x128104
        0x0012810a:    4770        pG      BX       lr
    $d
        0x0012810c:    00128139    9...    DCD    1212729
        0x00128110:    001280e5    ....    DCD    1212645
    $t
    .text
    __scatterload
    __scatterload_rt2
        0x00128114:    4c06        .L      LDR      r4,[pc,#24] ; [0x128130] = 0x1281c0
        0x00128116:    2501        .%      MOVS     r5,#1
        0x00128118:    4e06        .N      LDR      r6,[pc,#24] ; [0x128134] = 0x1281d0
        0x0012811a:    e005        ..      B        0x128128 ; __scatterload + 20
        0x0012811c:    68e3        .h      LDR      r3,[r4,#0xc]
        0x0012811e:    cc07        ..      LDM      r4!,{r0-r2}
        0x00128120:    432b        +C      ORRS     r3,r3,r5
        0x00128122:    3c0c        .<      SUBS     r4,r4,#0xc
        0x00128124:    4798        .G      BLX      r3
        0x00128126:    3410        .4      ADDS     r4,r4,#0x10
        0x00128128:    42b4        .B      CMP      r4,r6
        0x0012812a:    d3f7        ..      BCC      0x12811c ; __scatterload + 8
        0x0012812c:    f7ffffde    ....    BL       __main_after_scatterload ; 0x1280ec
    $d
        0x00128130:    001281c0    ....    DCD    1212864
        0x00128134:    001281d0    ....    DCD    1212880
    $t
    i.SystemInit
    SystemInit
        0x00128138:    4770        pG      BX       lr
    i.__scatterload_copy
    __scatterload_copy
        0x0012813a:    e002        ..      B        0x128142 ; __scatterload_copy + 8
        0x0012813c:    c808        ..      LDM      r0!,{r3}
        0x0012813e:    1f12        ..      SUBS     r2,r2,#4
        0x00128140:    c108        ..      STM      r1!,{r3}
        0x00128142:    2a00        .*      CMP      r2,#0
        0x00128144:    d1fa        ..      BNE      0x12813c ; __scatterload_copy + 2
        0x00128146:    4770        pG      BX       lr
    i.__scatterload_null
    __scatterload_null
        0x00128148:    4770        pG      BX       lr
    i.__scatterload_zeroinit
    __scatterload_zeroinit
        0x0012814a:    2000        .       MOVS     r0,#0
        0x0012814c:    e001        ..      B        0x128152 ; __scatterload_zeroinit + 8
        0x0012814e:    c101        ..      STM      r1!,{r0}
        0x00128150:    1f12        ..      SUBS     r2,r2,#4
        0x00128152:    2a00        .*      CMP      r2,#0
        0x00128154:    d1fb        ..      BNE      0x12814e ; __scatterload_zeroinit + 4
        0x00128156:    4770        pG      BX       lr
    i.main
    main
        0x00128158:    b510        ..      PUSH     {r4,lr}
        0x0012815a:    490b        .I      LDR      r1,[pc,#44] ; [0x128188] = 0x130100
        0x0012815c:    2022        "       MOVS     r0,#0x22
        0x0012815e:    6008        .`      STR      r0,[r1,#0]
        0x00128160:    21c7        .!      MOVS     r1,#0xc7
        0x00128162:    480a        .H      LDR      r0,[pc,#40] ; [0x12818c] = 0x20133000
        0x00128164:    0089        ..      LSLS     r1,r1,#2
        0x00128166:    6001        .`      STR      r1,[r0,#0]
        0x00128168:    1d09        ..      ADDS     r1,r1,#4
        0x0012816a:    6041        A`      STR      r1,[r0,#4]
        0x0012816c:    4807        .H      LDR      r0,[pc,#28] ; [0x12818c] = 0x20133000
        0x0012816e:    4908        .I      LDR      r1,[pc,#32] ; [0x128190] = 0xf1276030
        0x00128170:    3040        @0      ADDS     r0,r0,#0x40
        0x00128172:    6001        .`      STR      r1,[r0,#0]
        0x00128174:    4907        .I      LDR      r1,[pc,#28] ; [0x128194] = 0xf000feb3
        0x00128176:    6041        A`      STR      r1,[r0,#4]
        0x00128178:    4904        .I      LDR      r1,[pc,#16] ; [0x12818c] = 0x20133000
        0x0012817a:    2003        .       MOVS     r0,#3
        0x0012817c:    3180        .1      ADDS     r1,r1,#0x80
        0x0012817e:    6008        .`      STR      r0,[r1,#0]
        0x00128180:    f000f80a    ....    BL       start_rom_run_function ; 0x128198
        0x00128184:    2000        .       MOVS     r0,#0
        0x00128186:    bd10        ..      POP      {r4,pc}
    $d
        0x00128188:    00130100    ....    DCD    1245440
        0x0012818c:    20133000    .0.     DCD    538128384
        0x00128190:    f1276030    0`'.    DCD    4045889584
        0x00128194:    f000feb3    ....    DCD    4026597043
    $t
    i.start_rom_run_function
    start_rom_run_function
        0x00128198:    b430        0.      PUSH     {r4,r5}
        0x0012819a:    4807        .H      LDR      r0,[pc,#28] ; [0x1281b8] = 0x1281d0
        0x0012819c:    2100        .!      MOVS     r1,#0
        0x0012819e:    6001        .`      STR      r1,[r0,#0]
        0x001281a0:    6808        .h      LDR      r0,[r1,#0]
        0x001281a2:    f3808808    ....    MSR      MSP,r0
        0x001281a6:    2004        .       MOVS     r0,#4
        0x001281a8:    6804        .h      LDR      r4,[r0,#0]
        0x001281aa:    4b04        .K      LDR      r3,[pc,#16] ; [0x1281bc] = 0x3399
        0x001281ac:    46a4        .F      MOV      r12,r4
        0x001281ae:    bc30        0.      POP      {r4,r5}
        0x001281b0:    460a        .F      MOV      r2,r1
        0x001281b2:    4608        .F      MOV      r0,r1
        0x001281b4:    4760        `G      BX       r12
    $d
        0x001281b6:    0000        ..      DCW    0
        0x001281b8:    001281d0    ....    DCD    1212880
        0x001281bc:    00003399    .3..    DCD    13209
    $d.realdata
    Region$$Table$$Base
        0x001281c0:    001281d4    ....    DCD    1212884
        0x001281c4:    001281d4    ....    DCD    1212884
        0x001281c8:    00000c04    ....    DCD    3076
        0x001281cc:    0012814a    J...    DCD    1212746
    Region$$Table$$Limit

====================================

** Section #4

    Name        : EXEC2_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x001281d0
    File Offset : 532 (0x214)
    Size        : 4 bytes (0x4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    0x1281d0:   00 00 00 00                                        ....


====================================

** Section #5

    Name        : EXEC2_SECTION
    Type        : SHT_NOBITS (0x00000008)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x001281d4
    File Offset : 536 (0x218)
    Size        : 3076 bytes (0xc04)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 8
    Entry Size  : 0


====================================

** Section #6

    Name        : .debug_abbrev
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 536 (0x218)
    Size        : 1476 bytes (0x5c4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #7

    Name        : .debug_frame
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 2012 (0x7dc)
    Size        : 500 bytes (0x1f4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #8

    Name        : .debug_info
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 2512 (0x9d0)
    Size        : 4068 bytes (0xfe4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #9

    Name        : .debug_line
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 6580 (0x19b4)
    Size        : 976 bytes (0x3d0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #10

    Name        : .debug_loc
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 7556 (0x1d84)
    Size        : 204 bytes (0xcc)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #11

    Name        : .debug_macinfo
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 7760 (0x1e50)
    Size        : 3000 bytes (0xbb8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #12

    Name        : .debug_pubnames
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 10760 (0x2a08)
    Size        : 177 bytes (0xb1)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #13

    Name        : .symtab
    Type        : SHT_SYMTAB (0x00000002)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 10940 (0x2abc)
    Size        : 1920 bytes (0x780)
    Link        : Section 14 (.strtab)
    Info        : Last local symbol no = 63
    Alignment   : 4
    Entry Size  : 16


====================================

** Section #14

    Name        : .strtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 12860 (0x323c)
    Size        : 1556 bytes (0x614)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #15

    Name        : .note
    Type        : SHT_NOTE (0x00000007)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 14416 (0x3850)
    Size        : 36 bytes (0x24)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

        Section     Segment
    ====================================

              1           0
              2           0
              3           0
              4           0
              5           0

====================================

** Section #16

    Name        : .comment
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 14452 (0x3874)
    Size        : 3228 bytes (0xc9c)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0

    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    ArmLink --strict --library_type=microlib --callgraph --load_addr_map_info --map --symbols --xref --diag_suppress=9931 --cpu=Cortex-M0+ --list=.\Listings\boot_rom_patch.map --output=.\Objects\boot_rom_patch.axf --scatter=.\boot_ram_scatter.txt --info=summarysizes,sizes,totals,unused,veneers

    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\h_p.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\m_ps.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\mc_p.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\mf_p.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\vfpsupport.l
    Input Comments:
    
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\boot_ram.o --vfemode=force

    Input Comments:
    
    p3c5c-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\boot_ram.o --depend=.\objects\boot_ram.d --cpu=Cortex-M0+ --apcs=interwork --diag_suppress=9931 -I.\uart -I.\qspi -I.\flash -I.\dmac -I.\uart_awo -I.\clkg -I.\CMSIS -I..\..\src\arch -I..\..\bootloader -I.\RTE\_boot_rom_96m_patch -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\boot_ram.crf boot_ram.c
    
    
    
    
    startup_uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    ArmAsm --debug --xref --diag_suppress=9931 --cpu=Cortex-M0+ --apcs=interwork --depend=.\objects\startup_uart_download.d  -I.\RTE\_boot_rom_96m_patch -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\In
    
    
    uart_6m_patch.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\uart_6m_patch.o --vfemode=force

    Input Comments:
    
    p5474-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    uart_6m_patch.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\uart_6m_patch.o --depend=.\objects\uart_6m_patch.d --cpu=Cortex-M0+ --apcs=interwork --diag_suppress=9931 -I.\CMSIS -I..\..\src\arch -I..\..\bootloader -I.\RTE\_boot_rom_96m_patch -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\uart_6m_patch.crf uart_6m_patch.c
    
    
    
    
    
    

====================================

** Section #17

    Name        : .shstrtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 17680 (0x4510)
    Size        : 184 bytes (0xb8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

address     size       variable name                            type
0x00127ff0  0x10       boot_header                              const boot_header_t
0x00127ff0  0x4        boot_header.bx_flag                      array[4] of uint8_t
0x00127ff4  0x4        boot_header.base_addr                    pointer to uint8_t
0x00127ff8  0x4        boot_header.length                       uint32_t
0x00127ffc  0x4        boot_header.entry_point                  entry_point_t

address     size       variable name                            type
0x001281d0  0x4        vec_int_base                             pointer to uint32_t

