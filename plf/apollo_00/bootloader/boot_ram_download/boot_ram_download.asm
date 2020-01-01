
========================================================================

** ELF Header Information

    File Name: .\Objects\boot_ram_download.axf

    Machine class: ELFCLASS32 (32-bit)
    Data encoding: ELFDATA2LSB (Little endian)
    Header version: EV_CURRENT (Current version)
    Operating System ABI: none
    ABI Version: 0
    File Type: ET_EXEC (Executable) (2)
    Machine: EM_ARM (ARM)

    Image Entry point: 0x00128089
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
    Section header entries: 17

    Program header offset: 168436 (0x000291f4)
    Section header offset: 168468 (0x00029214)

    Section header string table index: 16

========================================================================

** Program header #0

    Type          : PT_LOAD (1)
    File Offset   : 52 (0x34)
    Virtual Addr  : 0x00127ff0
    Physical Addr : 0x00127ff0
    Size in file  : 5336 bytes (0x14d8)
    Size in memory: 25864 bytes (0x6508)
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

    0x127ff0:   42 58 32 34 00 80 12 00 ff ff ff ff 9d 80 12 00    BX24............


====================================

** Section #2

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_EXECINSTR (0x00000006)
    Addr        : 0x00128000
    File Offset : 68 (0x44)
    Size        : 5264 bytes (0x1490)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    $d.realdata
    RESET
    __Vectors
        0x00128000:    0012e4f8    ....    DCD    1238264
        0x00128004:    0012809d    ....    DCD    1212573
        0x00128008:    001280b1    ....    DCD    1212593
        0x0012800c:    001280b3    ....    DCD    1212595
        0x00128010:    001280b5    ....    DCD    1212597
        0x00128014:    001280b7    ....    DCD    1212599
        0x00128018:    001280b9    ....    DCD    1212601
        0x0012801c:    00000000    ....    DCD    0
        0x00128020:    00000000    ....    DCD    0
        0x00128024:    00000000    ....    DCD    0
        0x00128028:    00000000    ....    DCD    0
        0x0012802c:    001280bb    ....    DCD    1212603
        0x00128030:    001280bd    ....    DCD    1212605
        0x00128034:    00000000    ....    DCD    0
        0x00128038:    001280bf    ....    DCD    1212607
        0x0012803c:    001280c1    ....    DCD    1212609
        0x00128040:    001280c3    ....    DCD    1212611
        0x00128044:    001280c3    ....    DCD    1212611
        0x00128048:    001280c3    ....    DCD    1212611
        0x0012804c:    001280c3    ....    DCD    1212611
        0x00128050:    001280c3    ....    DCD    1212611
        0x00128054:    001280c3    ....    DCD    1212611
        0x00128058:    001280c3    ....    DCD    1212611
        0x0012805c:    001280c3    ....    DCD    1212611
        0x00128060:    001280c3    ....    DCD    1212611
        0x00128064:    001280c3    ....    DCD    1212611
        0x00128068:    001280c3    ....    DCD    1212611
        0x0012806c:    00128159    Y...    DCD    1212761
        0x00128070:    001280c3    ....    DCD    1212611
        0x00128074:    001280c3    ....    DCD    1212611
        0x00128078:    001280c3    ....    DCD    1212611
        0x0012807c:    001280c3    ....    DCD    1212611
        0x00128080:    001280c3    ....    DCD    1212611
        0x00128084:    001280c3    ....    DCD    1212611
    $t
    .ARM.Collect$$$$00000000
    .ARM.Collect$$$$00000001
    __Vectors_End
    __main
    _main_stk
        0x00128088:    4803        .H      LDR      r0,__lit__00000000 ; [0x128098] = 0x12e4f8
        0x0012808a:    4685        .F      MOV      sp,r0
    .ARM.Collect$$$$00000004
    _main_scatterload
        0x0012808c:    f000f83a    ..:.    BL       __scatterload ; 0x128104
    .ARM.Collect$$$$00000008
    .ARM.Collect$$$$0000000A
    .ARM.Collect$$$$0000000B
    __main_after_scatterload
    _main_clock
    _main_cpp_init
    _main_init
        0x00128090:    4800        .H      LDR      r0,[pc,#0] ; [0x128094] = 0x1287e1
        0x00128092:    4700        .G      BX       r0
    $d
        0x00128094:    001287e1    ....    DCD    1214433
    .ARM.Collect$$$$00002712
    __lit__00000000
    .ARM.Collect$$$$0000000D
    .ARM.Collect$$$$0000000F
    __rt_final_cpp
    __rt_final_exit
        0x00128098:    0012e4f8    ....    DCD    1238264
    $t
    .text
    Reset_Handler
        0x0012809c:    4809        .H      LDR      r0,[pc,#36] ; [0x1280c4] = 0x128129
        0x0012809e:    4780        .G      BLX      r0
        0x001280a0:    4809        .H      LDR      r0,[pc,#36] ; [0x1280c8] = 0x128089
        0x001280a2:    4700        .G      BX       r0
    exception_exit
        0x001280a4:    4809        .H      LDR      r0,[pc,#36] ; [0x1280cc] = 0x12809d
        0x001280a6:    9006        ..      STR      r0,[sp,#0x18]
        0x001280a8:    4809        .H      LDR      r0,[pc,#36] ; [0x1280d0] = 0xf1000000
        0x001280aa:    9007        ..      STR      r0,[sp,#0x1c]
        0x001280ac:    4809        .H      LDR      r0,[pc,#36] ; [0x1280d4] = 0xfffffff9
        0x001280ae:    4700        .G      BX       r0
    NMI_Handler
        0x001280b0:    e7fe        ..      B        NMI_Handler ; 0x1280b0
    HardFault_Handler
        0x001280b2:    e7fe        ..      B        HardFault_Handler ; 0x1280b2
    MemManage_Handler
        0x001280b4:    e7fe        ..      B        MemManage_Handler ; 0x1280b4
    BusFault_Handler
        0x001280b6:    e7fe        ..      B        BusFault_Handler ; 0x1280b6
    UsageFault_Handler
        0x001280b8:    e7fe        ..      B        UsageFault_Handler ; 0x1280b8
    SVC_Handler
        0x001280ba:    e7fe        ..      B        SVC_Handler ; 0x1280ba
    DebugMon_Handler
        0x001280bc:    e7fe        ..      B        DebugMon_Handler ; 0x1280bc
    PendSV_Handler
        0x001280be:    e7fe        ..      B        PendSV_Handler ; 0x1280be
    SysTick_Handler
        0x001280c0:    e7fe        ..      B        SysTick_Handler ; 0x1280c0
    BLE_LP_IRQHandler
    BLE_MAC_IRQHandler
    DMAC_IRQHandler
    ECC_IRQHandler
    EXT_INTR_IRQHandler
    GPIO_IRQHandler
    IIC0_IRQHandler
    IIC1_IRQHandler
    QSPI_IRQHandler
    RTC_IRQHandler
    SFT_IRQHandler
    SPIM0_IRQHandler
    SPIM1_IRQHandler
    SPIS_IRQHandler
    TIMER_IRQHandler
    UART1_IRQHandler
    WDT_IRQHandler
        0x001280c2:    e7fe        ..      B        BLE_LP_IRQHandler ; 0x1280c2
    $d
        0x001280c4:    00128129    )...    DCD    1212713
        0x001280c8:    00128089    ....    DCD    1212553
        0x001280cc:    0012809d    ....    DCD    1212573
        0x001280d0:    f1000000    ....    DCD    4043309056
        0x001280d4:    fffffff9    ....    DCD    4294967289
    $t
    bx_delay_asm
        0x001280d8:    1e40        @.      SUBS     r0,r0,#1
        0x001280da:    2800        .(      CMP      r0,#0
        0x001280dc:    d1fc        ..      BNE      bx_delay_asm ; 0x1280d8
        0x001280de:    4770        pG      BX       lr
    .text
    __aeabi_memcpy
    __aeabi_memcpy4
    __aeabi_memcpy8
        0x001280e0:    4603        .F      MOV      r3,r0
        0x001280e2:    430b        .C      ORRS     r3,r3,r1
        0x001280e4:    079b        ..      LSLS     r3,r3,#30
        0x001280e6:    d003        ..      BEQ      0x1280f0 ; __aeabi_memcpy + 16
        0x001280e8:    e009        ..      B        0x1280fe ; __aeabi_memcpy + 30
        0x001280ea:    c908        ..      LDM      r1!,{r3}
        0x001280ec:    1f12        ..      SUBS     r2,r2,#4
        0x001280ee:    c008        ..      STM      r0!,{r3}
        0x001280f0:    2a04        .*      CMP      r2,#4
        0x001280f2:    d2fa        ..      BCS      0x1280ea ; __aeabi_memcpy + 10
        0x001280f4:    e003        ..      B        0x1280fe ; __aeabi_memcpy + 30
        0x001280f6:    780b        .x      LDRB     r3,[r1,#0]
        0x001280f8:    7003        .p      STRB     r3,[r0,#0]
        0x001280fa:    1c40        @.      ADDS     r0,r0,#1
        0x001280fc:    1c49        I.      ADDS     r1,r1,#1
        0x001280fe:    1e52        R.      SUBS     r2,r2,#1
        0x00128100:    d2f9        ..      BCS      0x1280f6 ; __aeabi_memcpy + 22
        0x00128102:    4770        pG      BX       lr
    .text
    __scatterload
    __scatterload_rt2
        0x00128104:    4c06        .L      LDR      r4,[pc,#24] ; [0x128120] = 0x129480
        0x00128106:    2501        .%      MOVS     r5,#1
        0x00128108:    4e06        .N      LDR      r6,[pc,#24] ; [0x128124] = 0x129490
        0x0012810a:    e005        ..      B        0x128118 ; __scatterload + 20
        0x0012810c:    68e3        .h      LDR      r3,[r4,#0xc]
        0x0012810e:    cc07        ..      LDM      r4!,{r0-r2}
        0x00128110:    432b        +C      ORRS     r3,r3,r5
        0x00128112:    3c0c        .<      SUBS     r4,r4,#0xc
        0x00128114:    4798        .G      BLX      r3
        0x00128116:    3410        .4      ADDS     r4,r4,#0x10
        0x00128118:    42b4        .B      CMP      r4,r6
        0x0012811a:    d3f7        ..      BCC      0x12810c ; __scatterload + 8
        0x0012811c:    f7ffffb8    ....    BL       __main_after_scatterload ; 0x128090
    $d
        0x00128120:    00129480    ....    DCD    1217664
        0x00128124:    00129490    ....    DCD    1217680
    $t
    i.SystemInit
    SystemInit
        0x00128128:    b530        0.      PUSH     {r4,r5,lr}
        0x0012812a:    bf00        ..      NOP      
        0x0012812c:    bf00        ..      NOP      
        0x0012812e:    bf00        ..      NOP      
        0x00128130:    f3bf8f5f    .._.    DMB      
        0x00128134:    bf00        ..      NOP      
        0x00128136:    bf00        ..      NOP      
        0x00128138:    bf00        ..      NOP      
        0x0012813a:    2425        %$      MOVS     r4,#0x25
        0x0012813c:    03e4        ..      LSLS     r4,r4,#15
        0x0012813e:    4d05        .M      LDR      r5,[pc,#20] ; [0x128154] = 0xe000ed00
        0x00128140:    60ac        .`      STR      r4,[r5,#8]
        0x00128142:    bf00        ..      NOP      
        0x00128144:    bf00        ..      NOP      
        0x00128146:    bf00        ..      NOP      
        0x00128148:    f3bf8f4f    ..O.    DSB      
        0x0012814c:    bf00        ..      NOP      
        0x0012814e:    bf00        ..      NOP      
        0x00128150:    bf00        ..      NOP      
        0x00128152:    bd30        0.      POP      {r4,r5,pc}
    $d
        0x00128154:    e000ed00    ....    DCD    3758157056
    $t
    i.UART0_IRQHandler
    UART0_IRQHandler
        0x00128158:    b510        ..      PUSH     {r4,lr}
        0x0012815a:    f000fdd5    ....    BL       uart_isr ; 0x128d08
        0x0012815e:    bd10        ..      POP      {r4,pc}
    i.__ARM_common_switch8
    __ARM_common_switch8
        0x00128160:    b430        0.      PUSH     {r4,r5}
        0x00128162:    4674        tF      MOV      r4,lr
        0x00128164:    1e64        d.      SUBS     r4,r4,#1
        0x00128166:    7825        %x      LDRB     r5,[r4,#0]
        0x00128168:    1c64        d.      ADDS     r4,r4,#1
        0x0012816a:    42ab        .B      CMP      r3,r5
        0x0012816c:    d304        ..      BCC      0x128178 ; __ARM_common_switch8 + 24
        0x0012816e:    5d63        c]      LDRB     r3,[r4,r5]
        0x00128170:    005b        [.      LSLS     r3,r3,#1
        0x00128172:    18e3        ..      ADDS     r3,r4,r3
        0x00128174:    bc30        0.      POP      {r4,r5}
        0x00128176:    4718        .G      BX       r3
        0x00128178:    461d        .F      MOV      r5,r3
        0x0012817a:    e7f8        ..      B        0x12816e ; __ARM_common_switch8 + 14
    i.__scatterload_copy
    __scatterload_copy
        0x0012817c:    e002        ..      B        0x128184 ; __scatterload_copy + 8
        0x0012817e:    c808        ..      LDM      r0!,{r3}
        0x00128180:    1f12        ..      SUBS     r2,r2,#4
        0x00128182:    c108        ..      STM      r1!,{r3}
        0x00128184:    2a00        .*      CMP      r2,#0
        0x00128186:    d1fa        ..      BNE      0x12817e ; __scatterload_copy + 2
        0x00128188:    4770        pG      BX       lr
    i.__scatterload_null
    __scatterload_null
        0x0012818a:    4770        pG      BX       lr
    i.__scatterload_zeroinit
    __scatterload_zeroinit
        0x0012818c:    2000        .       MOVS     r0,#0
        0x0012818e:    e001        ..      B        0x128194 ; __scatterload_zeroinit + 8
        0x00128190:    c101        ..      STM      r1!,{r0}
        0x00128192:    1f12        ..      SUBS     r2,r2,#4
        0x00128194:    2a00        .*      CMP      r2,#0
        0x00128196:    d1fb        ..      BNE      0x128190 ; __scatterload_zeroinit + 4
        0x00128198:    4770        pG      BX       lr
    i.bitrev
    bitrev
        0x0012819a:    b530        0.      PUSH     {r4,r5,lr}
        0x0012819c:    4602        .F      MOV      r2,r0
        0x0012819e:    2000        .       MOVS     r0,#0
        0x001281a0:    2300        .#      MOVS     r3,#0
        0x001281a2:    e00a        ..      B        0x1281ba ; bitrev + 32
        0x001281a4:    07d4        ..      LSLS     r4,r2,#31
        0x001281a6:    0fe4        ..      LSRS     r4,r4,#31
        0x001281a8:    2c00        .,      CMP      r4,#0
        0x001281aa:    d004        ..      BEQ      0x1281b6 ; bitrev + 28
        0x001281ac:    1e4c        L.      SUBS     r4,r1,#1
        0x001281ae:    1ae5        ..      SUBS     r5,r4,r3
        0x001281b0:    2401        .$      MOVS     r4,#1
        0x001281b2:    40ac        .@      LSLS     r4,r4,r5
        0x001281b4:    4320         C      ORRS     r0,r0,r4
        0x001281b6:    0852        R.      LSRS     r2,r2,#1
        0x001281b8:    1c5b        [.      ADDS     r3,r3,#1
        0x001281ba:    428b        .B      CMP      r3,r1
        0x001281bc:    dbf2        ..      BLT      0x1281a4 ; bitrev + 10
        0x001281be:    bd30        0.      POP      {r4,r5,pc}
    i.clk_gate_per_g0
    clk_gate_per_g0
        0x001281c0:    bf00        ..      NOP      
        0x001281c2:    490f        .I      LDR      r1,[pc,#60] ; [0x128200] = 0x20149000
        0x001281c4:    6108        .a      STR      r0,[r1,#0x10]
        0x001281c6:    bf00        ..      NOP      
        0x001281c8:    490e        .I      LDR      r1,[pc,#56] ; [0x128204] = 0xaaaaaaaa
        0x001281ca:    4001        .@      ANDS     r1,r1,r0
        0x001281cc:    2900        .)      CMP      r1,#0
        0x001281ce:    d007        ..      BEQ      0x1281e0 ; clk_gate_per_g0 + 32
        0x001281d0:    bf00        ..      NOP      
        0x001281d2:    bf00        ..      NOP      
        0x001281d4:    490a        .I      LDR      r1,[pc,#40] ; [0x128200] = 0x20149000
        0x001281d6:    6909        .i      LDR      r1,[r1,#0x10]
        0x001281d8:    4001        .@      ANDS     r1,r1,r0
        0x001281da:    2900        .)      CMP      r1,#0
        0x001281dc:    d1f9        ..      BNE      0x1281d2 ; clk_gate_per_g0 + 18
        0x001281de:    e00e        ..      B        0x1281fe ; clk_gate_per_g0 + 62
        0x001281e0:    4908        .I      LDR      r1,[pc,#32] ; [0x128204] = 0xaaaaaaaa
        0x001281e2:    43c9        .C      MVNS     r1,r1
        0x001281e4:    4001        .@      ANDS     r1,r1,r0
        0x001281e6:    2900        .)      CMP      r1,#0
        0x001281e8:    d008        ..      BEQ      0x1281fc ; clk_gate_per_g0 + 60
        0x001281ea:    bf00        ..      NOP      
        0x001281ec:    bf00        ..      NOP      
        0x001281ee:    4904        .I      LDR      r1,[pc,#16] ; [0x128200] = 0x20149000
        0x001281f0:    6909        .i      LDR      r1,[r1,#0x10]
        0x001281f2:    4602        .F      MOV      r2,r0
        0x001281f4:    438a        .C      BICS     r2,r2,r1
        0x001281f6:    2a00        .*      CMP      r2,#0
        0x001281f8:    d1f8        ..      BNE      0x1281ec ; clk_gate_per_g0 + 44
        0x001281fa:    e000        ..      B        0x1281fe ; clk_gate_per_g0 + 62
        0x001281fc:    bf00        ..      NOP      
        0x001281fe:    4770        pG      BX       lr
    $d
        0x00128200:    20149000    ...     DCD    538218496
        0x00128204:    aaaaaaaa    ....    DCD    2863311530
    $t
    i.clk_uart0_921600
    clk_uart0_921600
        0x00128208:    b510        ..      PUSH     {r4,lr}
        0x0012820a:    2001        .       MOVS     r0,#1
        0x0012820c:    0240        @.      LSLS     r0,r0,#9
        0x0012820e:    f000fc95    ....    BL       sysc_per_clkg0_set ; 0x128b3c
        0x00128212:    2302        .#      MOVS     r3,#2
        0x00128214:    2201        ."      MOVS     r2,#1
        0x00128216:    4619        .F      MOV      r1,r3
        0x00128218:    200c        .       MOVS     r0,#0xc
        0x0012821a:    f000fc1f    ....    BL       sysc_per_clk_div_param1_pack ; 0x128a5c
        0x0012821e:    2001        .       MOVS     r0,#1
        0x00128220:    f000fc28    ..(.    BL       sysc_per_clk_sel_uart0_setf ; 0x128a74
        0x00128224:    20ff        .       MOVS     r0,#0xff
        0x00128226:    3001        .0      ADDS     r0,#1
        0x00128228:    f000fc88    ....    BL       sysc_per_clkg0_set ; 0x128b3c
        0x0012822c:    bd10        ..      POP      {r4,pc}
        0x0012822e:    0000        ..      MOVS     r0,r0
    i.crc32_calc
    crc32_calc
        0x00128230:    b5f0        ..      PUSH     {r4-r7,lr}
        0x00128232:    4603        .F      MOV      r3,r0
        0x00128234:    460c        .F      MOV      r4,r1
        0x00128236:    4615        .F      MOV      r5,r2
        0x00128238:    4619        .F      MOV      r1,r3
        0x0012823a:    2200        ."      MOVS     r2,#0
        0x0012823c:    4626        &F      MOV      r6,r4
        0x0012823e:    bf00        ..      NOP      
        0x00128240:    e009        ..      B        0x128256 ; crc32_calc + 38
        0x00128242:    5cb0        .\      LDRB     r0,[r6,r2]
        0x00128244:    4048        H@      EORS     r0,r0,r1
        0x00128246:    0600        ..      LSLS     r0,r0,#24
        0x00128248:    0d80        ..      LSRS     r0,r0,#22
        0x0012824a:    4f05        .O      LDR      r7,[pc,#20] ; [0x128260] = 0x12c0f4
        0x0012824c:    5838        8X      LDR      r0,[r7,r0]
        0x0012824e:    0a0f        ..      LSRS     r7,r1,#8
        0x00128250:    4078        x@      EORS     r0,r0,r7
        0x00128252:    4601        .F      MOV      r1,r0
        0x00128254:    1c52        R.      ADDS     r2,r2,#1
        0x00128256:    42aa        .B      CMP      r2,r5
        0x00128258:    d3f3        ..      BCC      0x128242 ; crc32_calc + 18
        0x0012825a:    43c8        .C      MVNS     r0,r1
        0x0012825c:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012825e:    0000        ..      DCW    0
        0x00128260:    0012c0f4    ....    DCD    1229044
    $t
    i.crc32_init
    crc32_init
        0x00128264:    b5f8        ..      PUSH     {r3-r7,lr}
        0x00128266:    4606        .F      MOV      r6,r0
        0x00128268:    2120         !      MOVS     r1,#0x20
        0x0012826a:    4630        0F      MOV      r0,r6
        0x0012826c:    f7ffff95    ....    BL       bitrev ; 0x12819a
        0x00128270:    4606        .F      MOV      r6,r0
        0x00128272:    2400        .$      MOVS     r4,#0
        0x00128274:    e012        ..      B        0x12829c ; crc32_init + 56
        0x00128276:    4625        %F      MOV      r5,r4
        0x00128278:    2700        .'      MOVS     r7,#0
        0x0012827a:    e009        ..      B        0x128290 ; crc32_init + 44
        0x0012827c:    07e8        ..      LSLS     r0,r5,#31
        0x0012827e:    0fc0        ..      LSRS     r0,r0,#31
        0x00128280:    2800        .(      CMP      r0,#0
        0x00128282:    d003        ..      BEQ      0x12828c ; crc32_init + 40
        0x00128284:    0868        h.      LSRS     r0,r5,#1
        0x00128286:    4070        p@      EORS     r0,r0,r6
        0x00128288:    4605        .F      MOV      r5,r0
        0x0012828a:    e000        ..      B        0x12828e ; crc32_init + 42
        0x0012828c:    086d        m.      LSRS     r5,r5,#1
        0x0012828e:    1c7f        ..      ADDS     r7,r7,#1
        0x00128290:    2f08        ./      CMP      r7,#8
        0x00128292:    dbf3        ..      BLT      0x12827c ; crc32_init + 24
        0x00128294:    00a0        ..      LSLS     r0,r4,#2
        0x00128296:    4903        .I      LDR      r1,[pc,#12] ; [0x1282a4] = 0x12c0f4
        0x00128298:    500d        .P      STR      r5,[r1,r0]
        0x0012829a:    1c64        d.      ADDS     r4,r4,#1
        0x0012829c:    2cff        .,      CMP      r4,#0xff
        0x0012829e:    ddea        ..      BLE      0x128276 ; crc32_init + 18
        0x001282a0:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x001282a2:    0000        ..      DCW    0
        0x001282a4:    0012c0f4    ....    DCD    1229044
    $t
    i.dmac_block_ts_config
    dmac_block_ts_config
        0x001282a8:    b530        0.      PUSH     {r4,r5,lr}
        0x001282aa:    4a15        .J      LDR      r2,[pc,#84] ; [0x128300] = 0xfff
        0x001282ac:    4290        .B      CMP      r0,r2
        0x001282ae:    d913        ..      BLS      0x1282d8 ; dmac_block_ts_config + 48
        0x001282b0:    bf00        ..      NOP      
        0x001282b2:    2458        X$      MOVS     r4,#0x58
        0x001282b4:    434c        LC      MULS     r4,r1,r4
        0x001282b6:    4d13        .M      LDR      r5,[pc,#76] ; [0x128304] = 0x20120000
        0x001282b8:    1964        d.      ADDS     r4,r4,r5
        0x001282ba:    69e2        .i      LDR      r2,[r4,#0x1c]
        0x001282bc:    0b14        ..      LSRS     r4,r2,#12
        0x001282be:    0324        $.      LSLS     r4,r4,#12
        0x001282c0:    4d0f        .M      LDR      r5,[pc,#60] ; [0x128300] = 0xfff
        0x001282c2:    432c        ,C      ORRS     r4,r4,r5
        0x001282c4:    4623        #F      MOV      r3,r4
        0x001282c6:    2458        X$      MOVS     r4,#0x58
        0x001282c8:    434c        LC      MULS     r4,r1,r4
        0x001282ca:    4d0e        .M      LDR      r5,[pc,#56] ; [0x128304] = 0x20120000
        0x001282cc:    1964        d.      ADDS     r4,r4,r5
        0x001282ce:    61e3        .a      STR      r3,[r4,#0x1c]
        0x001282d0:    bf00        ..      NOP      
        0x001282d2:    4a0b        .J      LDR      r2,[pc,#44] ; [0x128300] = 0xfff
        0x001282d4:    1a80        ..      SUBS     r0,r0,r2
        0x001282d6:    e012        ..      B        0x1282fe ; dmac_block_ts_config + 86
        0x001282d8:    bf00        ..      NOP      
        0x001282da:    2458        X$      MOVS     r4,#0x58
        0x001282dc:    434c        LC      MULS     r4,r1,r4
        0x001282de:    4d09        .M      LDR      r5,[pc,#36] ; [0x128304] = 0x20120000
        0x001282e0:    1964        d.      ADDS     r4,r4,r5
        0x001282e2:    69e2        .i      LDR      r2,[r4,#0x1c]
        0x001282e4:    0504        ..      LSLS     r4,r0,#20
        0x001282e6:    0d24        $.      LSRS     r4,r4,#20
        0x001282e8:    0b15        ..      LSRS     r5,r2,#12
        0x001282ea:    032d        -.      LSLS     r5,r5,#12
        0x001282ec:    432c        ,C      ORRS     r4,r4,r5
        0x001282ee:    4623        #F      MOV      r3,r4
        0x001282f0:    2458        X$      MOVS     r4,#0x58
        0x001282f2:    434c        LC      MULS     r4,r1,r4
        0x001282f4:    4d03        .M      LDR      r5,[pc,#12] ; [0x128304] = 0x20120000
        0x001282f6:    1964        d.      ADDS     r4,r4,r5
        0x001282f8:    61e3        .a      STR      r3,[r4,#0x1c]
        0x001282fa:    bf00        ..      NOP      
        0x001282fc:    2000        .       MOVS     r0,#0
        0x001282fe:    bd30        0.      POP      {r4,r5,pc}
    $d
        0x00128300:    00000fff    ....    DCD    4095
        0x00128304:    20120000    ...     DCD    538050560
    $t
    i.dmac_init
    dmac_init
        0x00128308:    2001        .       MOVS     r0,#1
        0x0012830a:    4903        .I      LDR      r1,[pc,#12] ; [0x128318] = 0x20120380
        0x0012830c:    6188        .a      STR      r0,[r1,#0x18]
        0x0012830e:    20ff        .       MOVS     r0,#0xff
        0x00128310:    4901        .I      LDR      r1,[pc,#4] ; [0x128318] = 0x20120380
        0x00128312:    3980        .9      SUBS     r1,r1,#0x80
        0x00128314:    6388        .c      STR      r0,[r1,#0x38]
        0x00128316:    4770        pG      BX       lr
    $d
        0x00128318:    20120380    ...     DCD    538051456
    $t
    i.dmac_start
    dmac_start
        0x0012831c:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012831e:    4604        .F      MOV      r4,r0
        0x00128320:    460d        .F      MOV      r5,r1
        0x00128322:    4629        )F      MOV      r1,r5
        0x00128324:    68a0        .h      LDR      r0,[r4,#8]
        0x00128326:    f7ffffbf    ....    BL       dmac_block_ts_config ; 0x1282a8
        0x0012832a:    9000        ..      STR      r0,[sp,#0]
        0x0012832c:    89a0        ..      LDRH     r0,[r4,#0xc]
        0x0012832e:    0440        @.      LSLS     r0,r0,#17
        0x00128330:    0f40        @.      LSRS     r0,r0,#29
        0x00128332:    2800        .(      CMP      r0,#0
        0x00128334:    d009        ..      BEQ      0x12834a ; dmac_start + 46
        0x00128336:    2801        .(      CMP      r0,#1
        0x00128338:    d004        ..      BEQ      0x128344 ; dmac_start + 40
        0x0012833a:    2802        .(      CMP      r0,#2
        0x0012833c:    d108        ..      BNE      0x128350 ; dmac_start + 52
        0x0012833e:    2602        .&      MOVS     r6,#2
        0x00128340:    2700        .'      MOVS     r7,#0
        0x00128342:    e006        ..      B        0x128352 ; dmac_start + 54
        0x00128344:    2600        .&      MOVS     r6,#0
        0x00128346:    2702        .'      MOVS     r7,#2
        0x00128348:    e003        ..      B        0x128352 ; dmac_start + 54
        0x0012834a:    2600        .&      MOVS     r6,#0
        0x0012834c:    2700        .'      MOVS     r7,#0
        0x0012834e:    e000        ..      B        0x128352 ; dmac_start + 54
        0x00128350:    bf00        ..      NOP      
        0x00128352:    bf00        ..      NOP      
        0x00128354:    89a0        ..      LDRH     r0,[r4,#0xc]
        0x00128356:    0440        @.      LSLS     r0,r0,#17
        0x00128358:    0f40        @.      LSRS     r0,r0,#29
        0x0012835a:    0500        ..      LSLS     r0,r0,#20
        0x0012835c:    2103        .!      MOVS     r1,#3
        0x0012835e:    0509        ..      LSLS     r1,r1,#20
        0x00128360:    4008        .@      ANDS     r0,r0,r1
        0x00128362:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x00128364:    05c9        ..      LSLS     r1,r1,#23
        0x00128366:    0f49        I.      LSRS     r1,r1,#29
        0x00128368:    0389        ..      LSLS     r1,r1,#14
        0x0012836a:    2207        ."      MOVS     r2,#7
        0x0012836c:    0392        ..      LSLS     r2,r2,#14
        0x0012836e:    4011        .@      ANDS     r1,r1,r2
        0x00128370:    4308        .C      ORRS     r0,r0,r1
        0x00128372:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x00128374:    0509        ..      LSLS     r1,r1,#20
        0x00128376:    0f49        I.      LSRS     r1,r1,#29
        0x00128378:    02c9        ..      LSLS     r1,r1,#11
        0x0012837a:    10d2        ..      ASRS     r2,r2,#3
        0x0012837c:    4011        .@      ANDS     r1,r1,r2
        0x0012837e:    4308        .C      ORRS     r0,r0,r1
        0x00128380:    0271        q.      LSLS     r1,r6,#9
        0x00128382:    2203        ."      MOVS     r2,#3
        0x00128384:    0252        R.      LSLS     r2,r2,#9
        0x00128386:    4011        .@      ANDS     r1,r1,r2
        0x00128388:    4308        .C      ORRS     r0,r0,r1
        0x0012838a:    01f9        ..      LSLS     r1,r7,#7
        0x0012838c:    1092        ..      ASRS     r2,r2,#2
        0x0012838e:    4011        .@      ANDS     r1,r1,r2
        0x00128390:    4308        .C      ORRS     r0,r0,r1
        0x00128392:    68e1        .h      LDR      r1,[r4,#0xc]
        0x00128394:    0109        ..      LSLS     r1,r1,#4
        0x00128396:    2270        p"      MOVS     r2,#0x70
        0x00128398:    4011        .@      ANDS     r1,r1,r2
        0x0012839a:    4308        .C      ORRS     r0,r0,r1
        0x0012839c:    7b21        !{      LDRB     r1,[r4,#0xc]
        0x0012839e:    0689        ..      LSLS     r1,r1,#26
        0x001283a0:    0f49        I.      LSRS     r1,r1,#29
        0x001283a2:    0049        I.      LSLS     r1,r1,#1
        0x001283a4:    220e        ."      MOVS     r2,#0xe
        0x001283a6:    4011        .@      ANDS     r1,r1,r2
        0x001283a8:    4308        .C      ORRS     r0,r0,r1
        0x001283aa:    2158        X!      MOVS     r1,#0x58
        0x001283ac:    4369        iC      MULS     r1,r5,r1
        0x001283ae:    4a1b        .J      LDR      r2,[pc,#108] ; [0x12841c] = 0x20120000
        0x001283b0:    1889        ..      ADDS     r1,r1,r2
        0x001283b2:    6188        .a      STR      r0,[r1,#0x18]
        0x001283b4:    2100        .!      MOVS     r1,#0
        0x001283b6:    2058        X       MOVS     r0,#0x58
        0x001283b8:    4368        hC      MULS     r0,r5,r0
        0x001283ba:    1880        ..      ADDS     r0,r0,r2
        0x001283bc:    6401        .d      STR      r1,[r0,#0x40]
        0x001283be:    68e0        .h      LDR      r0,[r4,#0xc]
        0x001283c0:    0240        @.      LSLS     r0,r0,#9
        0x001283c2:    0f00        ..      LSRS     r0,r0,#28
        0x001283c4:    02c0        ..      LSLS     r0,r0,#11
        0x001283c6:    210f        .!      MOVS     r1,#0xf
        0x001283c8:    02c9        ..      LSLS     r1,r1,#11
        0x001283ca:    4008        .@      ANDS     r0,r0,r1
        0x001283cc:    68e1        .h      LDR      r1,[r4,#0xc]
        0x001283ce:    0349        I.      LSLS     r1,r1,#13
        0x001283d0:    0f09        ..      LSRS     r1,r1,#28
        0x001283d2:    01c9        ..      LSLS     r1,r1,#7
        0x001283d4:    220f        ."      MOVS     r2,#0xf
        0x001283d6:    01d2        ..      LSLS     r2,r2,#7
        0x001283d8:    4011        .@      ANDS     r1,r1,r2
        0x001283da:    4308        .C      ORRS     r0,r0,r1
        0x001283dc:    1c80        ..      ADDS     r0,r0,#2
        0x001283de:    2158        X!      MOVS     r1,#0x58
        0x001283e0:    4369        iC      MULS     r1,r5,r1
        0x001283e2:    4a0e        .J      LDR      r2,[pc,#56] ; [0x12841c] = 0x20120000
        0x001283e4:    1889        ..      ADDS     r1,r1,r2
        0x001283e6:    6448        Hd      STR      r0,[r1,#0x44]
        0x001283e8:    2058        X       MOVS     r0,#0x58
        0x001283ea:    4368        hC      MULS     r0,r5,r0
        0x001283ec:    1880        ..      ADDS     r0,r0,r2
        0x001283ee:    6821        !h      LDR      r1,[r4,#0]
        0x001283f0:    6001        .`      STR      r1,[r0,#0]
        0x001283f2:    2058        X       MOVS     r0,#0x58
        0x001283f4:    4368        hC      MULS     r0,r5,r0
        0x001283f6:    1880        ..      ADDS     r0,r0,r2
        0x001283f8:    6861        ah      LDR      r1,[r4,#4]
        0x001283fa:    6081        .`      STR      r1,[r0,#8]
        0x001283fc:    2001        .       MOVS     r0,#1
        0x001283fe:    40a8        .@      LSLS     r0,r0,r5
        0x00128400:    0680        ..      LSLS     r0,r0,#26
        0x00128402:    0e80        ..      LSRS     r0,r0,#26
        0x00128404:    2101        .!      MOVS     r1,#1
        0x00128406:    40a9        .@      LSLS     r1,r1,r5
        0x00128408:    0209        ..      LSLS     r1,r1,#8
        0x0012840a:    223f        ?"      MOVS     r2,#0x3f
        0x0012840c:    0212        ..      LSLS     r2,r2,#8
        0x0012840e:    4011        .@      ANDS     r1,r1,r2
        0x00128410:    4308        .C      ORRS     r0,r0,r1
        0x00128412:    4903        .I      LDR      r1,[pc,#12] ; [0x128420] = 0x20120380
        0x00128414:    6208        .b      STR      r0,[r1,#0x20]
        0x00128416:    9800        ..      LDR      r0,[sp,#0]
        0x00128418:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012841a:    0000        ..      DCW    0
        0x0012841c:    20120000    ...     DCD    538050560
        0x00128420:    20120380    ...     DCD    538051456
    $t
    i.dmac_transfer_wait
    dmac_transfer_wait
        0x00128424:    b570        p.      PUSH     {r4-r6,lr}
        0x00128426:    4605        .F      MOV      r5,r0
        0x00128428:    460c        .F      MOV      r4,r1
        0x0012842a:    bf00        ..      NOP      
        0x0012842c:    4817        .H      LDR      r0,[pc,#92] ; [0x12848c] = 0x201202c0
        0x0012842e:    6800        .h      LDR      r0,[r0,#0]
        0x00128430:    2101        .!      MOVS     r1,#1
        0x00128432:    40a1        .@      LSLS     r1,r1,r4
        0x00128434:    4008        .@      ANDS     r0,r0,r1
        0x00128436:    2800        .(      CMP      r0,#0
        0x00128438:    d0f8        ..      BEQ      0x12842c ; dmac_transfer_wait + 8
        0x0012843a:    2001        .       MOVS     r0,#1
        0x0012843c:    40a0        .@      LSLS     r0,r0,r4
        0x0012843e:    4913        .I      LDR      r1,[pc,#76] ; [0x12848c] = 0x201202c0
        0x00128440:    3140        @1      ADDS     r1,r1,#0x40
        0x00128442:    6388        .c      STR      r0,[r1,#0x38]
        0x00128444:    e01f        ..      B        0x128486 ; dmac_transfer_wait + 98
        0x00128446:    4621        !F      MOV      r1,r4
        0x00128448:    4628        (F      MOV      r0,r5
        0x0012844a:    f7ffff2d    ..-.    BL       dmac_block_ts_config ; 0x1282a8
        0x0012844e:    4605        .F      MOV      r5,r0
        0x00128450:    2001        .       MOVS     r0,#1
        0x00128452:    40a0        .@      LSLS     r0,r0,r4
        0x00128454:    0680        ..      LSLS     r0,r0,#26
        0x00128456:    0e80        ..      LSRS     r0,r0,#26
        0x00128458:    2101        .!      MOVS     r1,#1
        0x0012845a:    40a1        .@      LSLS     r1,r1,r4
        0x0012845c:    0209        ..      LSLS     r1,r1,#8
        0x0012845e:    223f        ?"      MOVS     r2,#0x3f
        0x00128460:    0212        ..      LSLS     r2,r2,#8
        0x00128462:    4011        .@      ANDS     r1,r1,r2
        0x00128464:    4308        .C      ORRS     r0,r0,r1
        0x00128466:    4909        .I      LDR      r1,[pc,#36] ; [0x12848c] = 0x201202c0
        0x00128468:    31c0        .1      ADDS     r1,r1,#0xc0
        0x0012846a:    6208        .b      STR      r0,[r1,#0x20]
        0x0012846c:    bf00        ..      NOP      
        0x0012846e:    4807        .H      LDR      r0,[pc,#28] ; [0x12848c] = 0x201202c0
        0x00128470:    6800        .h      LDR      r0,[r0,#0]
        0x00128472:    2101        .!      MOVS     r1,#1
        0x00128474:    40a1        .@      LSLS     r1,r1,r4
        0x00128476:    4008        .@      ANDS     r0,r0,r1
        0x00128478:    2800        .(      CMP      r0,#0
        0x0012847a:    d0f8        ..      BEQ      0x12846e ; dmac_transfer_wait + 74
        0x0012847c:    2001        .       MOVS     r0,#1
        0x0012847e:    40a0        .@      LSLS     r0,r0,r4
        0x00128480:    4902        .I      LDR      r1,[pc,#8] ; [0x12848c] = 0x201202c0
        0x00128482:    3140        @1      ADDS     r1,r1,#0x40
        0x00128484:    6388        .c      STR      r0,[r1,#0x38]
        0x00128486:    2d00        .-      CMP      r5,#0
        0x00128488:    d1dd        ..      BNE      0x128446 ; dmac_transfer_wait + 34
        0x0012848a:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012848c:    201202c0    ...     DCD    538051264
    $t
    i.flash_erase
    flash_erase
        0x00128490:    b5f8        ..      PUSH     {r3-r7,lr}
        0x00128492:    4604        .F      MOV      r4,r0
        0x00128494:    460d        .F      MOV      r5,r1
        0x00128496:    f000f99b    ....    BL       flash_write_enable ; 0x1287d0
        0x0012849a:    2d03        .-      CMP      r5,#3
        0x0012849c:    d108        ..      BNE      0x1284b0 ; flash_erase + 32
        0x0012849e:    a022        ".      ADR      r0,{pc}+0x8a ; 0x128528
        0x001284a0:    6800        .h      LDR      r0,[r0,#0]
        0x001284a2:    9000        ..      STR      r0,[sp,#0]
        0x001284a4:    2601        .&      MOVS     r6,#1
        0x001284a6:    4631        1F      MOV      r1,r6
        0x001284a8:    4668        hF      MOV      r0,sp
        0x001284aa:    f000faad    ....    BL       qspi_std_write ; 0x128a08
        0x001284ae:    e036        6.      B        0x12851e ; flash_erase + 142
        0x001284b0:    2d02        .-      CMP      r5,#2
        0x001284b2:    d110        ..      BNE      0x1284d6 ; flash_erase + 70
        0x001284b4:    a01d        ..      ADR      r0,{pc}+0x78 ; 0x12852c
        0x001284b6:    6800        .h      LDR      r0,[r0,#0]
        0x001284b8:    9000        ..      STR      r0,[sp,#0]
        0x001284ba:    0220         .      LSLS     r0,r4,#8
        0x001284bc:    0e01        ..      LSRS     r1,r0,#24
        0x001284be:    4668        hF      MOV      r0,sp
        0x001284c0:    7041        Ap      STRB     r1,[r0,#1]
        0x001284c2:    0420         .      LSLS     r0,r4,#16
        0x001284c4:    0e01        ..      LSRS     r1,r0,#24
        0x001284c6:    4668        hF      MOV      r0,sp
        0x001284c8:    7081        .p      STRB     r1,[r0,#2]
        0x001284ca:    70c4        .p      STRB     r4,[r0,#3]
        0x001284cc:    2604        .&      MOVS     r6,#4
        0x001284ce:    4631        1F      MOV      r1,r6
        0x001284d0:    f000fa9a    ....    BL       qspi_std_write ; 0x128a08
        0x001284d4:    e023        #.      B        0x12851e ; flash_erase + 142
        0x001284d6:    2d01        .-      CMP      r5,#1
        0x001284d8:    d110        ..      BNE      0x1284fc ; flash_erase + 108
        0x001284da:    a015        ..      ADR      r0,{pc}+0x56 ; 0x128530
        0x001284dc:    6800        .h      LDR      r0,[r0,#0]
        0x001284de:    9000        ..      STR      r0,[sp,#0]
        0x001284e0:    0220         .      LSLS     r0,r4,#8
        0x001284e2:    0e01        ..      LSRS     r1,r0,#24
        0x001284e4:    4668        hF      MOV      r0,sp
        0x001284e6:    7041        Ap      STRB     r1,[r0,#1]
        0x001284e8:    0420         .      LSLS     r0,r4,#16
        0x001284ea:    0e01        ..      LSRS     r1,r0,#24
        0x001284ec:    4668        hF      MOV      r0,sp
        0x001284ee:    7081        .p      STRB     r1,[r0,#2]
        0x001284f0:    70c4        .p      STRB     r4,[r0,#3]
        0x001284f2:    2604        .&      MOVS     r6,#4
        0x001284f4:    4631        1F      MOV      r1,r6
        0x001284f6:    f000fa87    ....    BL       qspi_std_write ; 0x128a08
        0x001284fa:    e010        ..      B        0x12851e ; flash_erase + 142
        0x001284fc:    a00d        ..      ADR      r0,{pc}+0x38 ; 0x128534
        0x001284fe:    6800        .h      LDR      r0,[r0,#0]
        0x00128500:    9000        ..      STR      r0,[sp,#0]
        0x00128502:    0220         .      LSLS     r0,r4,#8
        0x00128504:    0e01        ..      LSRS     r1,r0,#24
        0x00128506:    4668        hF      MOV      r0,sp
        0x00128508:    7041        Ap      STRB     r1,[r0,#1]
        0x0012850a:    0420         .      LSLS     r0,r4,#16
        0x0012850c:    0e01        ..      LSRS     r1,r0,#24
        0x0012850e:    4668        hF      MOV      r0,sp
        0x00128510:    7081        .p      STRB     r1,[r0,#2]
        0x00128512:    70c4        .p      STRB     r4,[r0,#3]
        0x00128514:    2604        .&      MOVS     r6,#4
        0x00128516:    4631        1F      MOV      r1,r6
        0x00128518:    f000fa76    ..v.    BL       qspi_std_write ; 0x128a08
        0x0012851c:    bf00        ..      NOP      
        0x0012851e:    f000f80b    ....    BL       flash_operation_wait ; 0x128538
        0x00128522:    2000        .       MOVS     r0,#0
        0x00128524:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x00128526:    0000        ..      DCW    0
        0x00128528:    00000060    `...    DCD    96
        0x0012852c:    000000d8    ....    DCD    216
        0x00128530:    00000052    R...    DCD    82
        0x00128534:    00000020     ...    DCD    32
    $t
    i.flash_operation_wait
    flash_operation_wait
        0x00128538:    b510        ..      PUSH     {r4,lr}
        0x0012853a:    bf00        ..      NOP      
        0x0012853c:    f000f89a    ....    BL       flash_read_status_reg_1 ; 0x128674
        0x00128540:    07c1        ..      LSLS     r1,r0,#31
        0x00128542:    0fc9        ..      LSRS     r1,r1,#31
        0x00128544:    2900        .)      CMP      r1,#0
        0x00128546:    d1f9        ..      BNE      0x12853c ; flash_operation_wait + 4
        0x00128548:    bd10        ..      POP      {r4,pc}
    i.flash_program
    flash_program
        0x0012854a:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012854c:    b085        ..      SUB      sp,sp,#0x14
        0x0012854e:    4604        .F      MOV      r4,r0
        0x00128550:    460d        .F      MOV      r5,r1
        0x00128552:    4617        .F      MOV      r7,r2
        0x00128554:    0a20         .      LSRS     r0,r4,#8
        0x00128556:    0200        ..      LSLS     r0,r0,#8
        0x00128558:    9004        ..      STR      r0,[sp,#0x10]
        0x0012855a:    9804        ..      LDR      r0,[sp,#0x10]
        0x0012855c:    42a0        .B      CMP      r0,r4
        0x0012855e:    d014        ..      BEQ      0x12858a ; flash_program + 64
        0x00128560:    9804        ..      LDR      r0,[sp,#0x10]
        0x00128562:    30ff        .0      ADDS     r0,r0,#0xff
        0x00128564:    3001        .0      ADDS     r0,#1
        0x00128566:    1b00        ..      SUBS     r0,r0,r4
        0x00128568:    9000        ..      STR      r0,[sp,#0]
        0x0012856a:    9800        ..      LDR      r0,[sp,#0]
        0x0012856c:    4285        .B      CMP      r5,r0
        0x0012856e:    d201        ..      BCS      0x128574 ; flash_program + 42
        0x00128570:    4628        (F      MOV      r0,r5
        0x00128572:    e000        ..      B        0x128576 ; flash_program + 44
        0x00128574:    9800        ..      LDR      r0,[sp,#0]
        0x00128576:    4606        .F      MOV      r6,r0
        0x00128578:    463a        :F      MOV      r2,r7
        0x0012857a:    4631        1F      MOV      r1,r6
        0x0012857c:    4620         F      MOV      r0,r4
        0x0012857e:    f000f829    ..).    BL       flash_program_base ; 0x1285d4
        0x00128582:    19bf        ..      ADDS     r7,r7,r6
        0x00128584:    19a4        ..      ADDS     r4,r4,r6
        0x00128586:    1bad        ..      SUBS     r5,r5,r6
        0x00128588:    bf00        ..      NOP      
        0x0012858a:    0228        (.      LSLS     r0,r5,#8
        0x0012858c:    0c00        ..      LSRS     r0,r0,#16
        0x0012858e:    9003        ..      STR      r0,[sp,#0xc]
        0x00128590:    b2e8        ..      UXTB     r0,r5
        0x00128592:    9002        ..      STR      r0,[sp,#8]
        0x00128594:    2000        .       MOVS     r0,#0
        0x00128596:    9001        ..      STR      r0,[sp,#4]
        0x00128598:    e00d        ..      B        0x1285b6 ; flash_program + 108
        0x0012859a:    463a        :F      MOV      r2,r7
        0x0012859c:    21ff        .!      MOVS     r1,#0xff
        0x0012859e:    3101        .1      ADDS     r1,#1
        0x001285a0:    4620         F      MOV      r0,r4
        0x001285a2:    f000f817    ....    BL       flash_program_base ; 0x1285d4
        0x001285a6:    34ff        .4      ADDS     r4,r4,#0xff
        0x001285a8:    3401        .4      ADDS     r4,#1
        0x001285aa:    37ff        .7      ADDS     r7,r7,#0xff
        0x001285ac:    3701        .7      ADDS     r7,#1
        0x001285ae:    9801        ..      LDR      r0,[sp,#4]
        0x001285b0:    1c40        @.      ADDS     r0,r0,#1
        0x001285b2:    b280        ..      UXTH     r0,r0
        0x001285b4:    9001        ..      STR      r0,[sp,#4]
        0x001285b6:    9903        ..      LDR      r1,[sp,#0xc]
        0x001285b8:    9801        ..      LDR      r0,[sp,#4]
        0x001285ba:    4288        .B      CMP      r0,r1
        0x001285bc:    dbed        ..      BLT      0x12859a ; flash_program + 80
        0x001285be:    9802        ..      LDR      r0,[sp,#8]
        0x001285c0:    2800        .(      CMP      r0,#0
        0x001285c2:    d004        ..      BEQ      0x1285ce ; flash_program + 132
        0x001285c4:    463a        :F      MOV      r2,r7
        0x001285c6:    4620         F      MOV      r0,r4
        0x001285c8:    9902        ..      LDR      r1,[sp,#8]
        0x001285ca:    f000f803    ....    BL       flash_program_base ; 0x1285d4
        0x001285ce:    2000        .       MOVS     r0,#0
        0x001285d0:    b005        ..      ADD      sp,sp,#0x14
        0x001285d2:    bdf0        ..      POP      {r4-r7,pc}
    i.flash_program_base
    flash_program_base
        0x001285d4:    b570        p.      PUSH     {r4-r6,lr}
        0x001285d6:    4604        .F      MOV      r4,r0
        0x001285d8:    460d        .F      MOV      r5,r1
        0x001285da:    4616        .F      MOV      r6,r2
        0x001285dc:    f000f8f8    ....    BL       flash_write_enable ; 0x1287d0
        0x001285e0:    462b        +F      MOV      r3,r5
        0x001285e2:    4632        2F      MOV      r2,r6
        0x001285e4:    4621        !F      MOV      r1,r4
        0x001285e6:    2002        .       MOVS     r0,#2
        0x001285e8:    f000f960    ..`.    BL       qspi_flash_program ; 0x1288ac
        0x001285ec:    f7ffffa4    ....    BL       flash_operation_wait ; 0x128538
        0x001285f0:    bd70        p.      POP      {r4-r6,pc}
        0x001285f2:    0000        ..      MOVS     r0,r0
    i.flash_read
    flash_read
        0x001285f4:    b5fe        ..      PUSH     {r1-r7,lr}
        0x001285f6:    4604        .F      MOV      r4,r0
        0x001285f8:    460d        .F      MOV      r5,r1
        0x001285fa:    4616        .F      MOV      r6,r2
        0x001285fc:    0c28        (.      LSRS     r0,r5,#16
        0x001285fe:    9002        ..      STR      r0,[sp,#8]
        0x00128600:    b2af        ..      UXTH     r7,r5
        0x00128602:    2000        .       MOVS     r0,#0
        0x00128604:    9001        ..      STR      r0,[sp,#4]
        0x00128606:    e019        ..      B        0x12863c ; flash_read + 72
        0x00128608:    a019        ..      ADR      r0,{pc}+0x68 ; 0x128670
        0x0012860a:    6800        .h      LDR      r0,[r0,#0]
        0x0012860c:    9000        ..      STR      r0,[sp,#0]
        0x0012860e:    0220         .      LSLS     r0,r4,#8
        0x00128610:    0e01        ..      LSRS     r1,r0,#24
        0x00128612:    4668        hF      MOV      r0,sp
        0x00128614:    7041        Ap      STRB     r1,[r0,#1]
        0x00128616:    0420         .      LSLS     r0,r4,#16
        0x00128618:    0e01        ..      LSRS     r1,r0,#24
        0x0012861a:    4668        hF      MOV      r0,sp
        0x0012861c:    7081        .p      STRB     r1,[r0,#2]
        0x0012861e:    70c4        .p      STRB     r4,[r0,#3]
        0x00128620:    2301        .#      MOVS     r3,#1
        0x00128622:    041b        ..      LSLS     r3,r3,#16
        0x00128624:    4632        2F      MOV      r2,r6
        0x00128626:    2104        .!      MOVS     r1,#4
        0x00128628:    f000f9ac    ....    BL       qspi_std_read ; 0x128984
        0x0012862c:    2001        .       MOVS     r0,#1
        0x0012862e:    0400        ..      LSLS     r0,r0,#16
        0x00128630:    1824        $.      ADDS     r4,r4,r0
        0x00128632:    1836        6.      ADDS     r6,r6,r0
        0x00128634:    9801        ..      LDR      r0,[sp,#4]
        0x00128636:    1c40        @.      ADDS     r0,r0,#1
        0x00128638:    b280        ..      UXTH     r0,r0
        0x0012863a:    9001        ..      STR      r0,[sp,#4]
        0x0012863c:    9902        ..      LDR      r1,[sp,#8]
        0x0012863e:    9801        ..      LDR      r0,[sp,#4]
        0x00128640:    4288        .B      CMP      r0,r1
        0x00128642:    dbe1        ..      BLT      0x128608 ; flash_read + 20
        0x00128644:    2f00        ./      CMP      r7,#0
        0x00128646:    d011        ..      BEQ      0x12866c ; flash_read + 120
        0x00128648:    a009        ..      ADR      r0,{pc}+0x28 ; 0x128670
        0x0012864a:    6800        .h      LDR      r0,[r0,#0]
        0x0012864c:    9000        ..      STR      r0,[sp,#0]
        0x0012864e:    0220         .      LSLS     r0,r4,#8
        0x00128650:    0e01        ..      LSRS     r1,r0,#24
        0x00128652:    4668        hF      MOV      r0,sp
        0x00128654:    7041        Ap      STRB     r1,[r0,#1]
        0x00128656:    0420         .      LSLS     r0,r4,#16
        0x00128658:    0e01        ..      LSRS     r1,r0,#24
        0x0012865a:    4668        hF      MOV      r0,sp
        0x0012865c:    7081        .p      STRB     r1,[r0,#2]
        0x0012865e:    70c4        .p      STRB     r4,[r0,#3]
        0x00128660:    463b        ;F      MOV      r3,r7
        0x00128662:    4632        2F      MOV      r2,r6
        0x00128664:    2104        .!      MOVS     r1,#4
        0x00128666:    f000f98d    ....    BL       qspi_std_read ; 0x128984
        0x0012866a:    bf00        ..      NOP      
        0x0012866c:    2000        .       MOVS     r0,#0
        0x0012866e:    bdfe        ..      POP      {r1-r7,pc}
    $d
        0x00128670:    00000003    ....    DCD    3
    $t
    i.flash_read_status_reg_1
    flash_read_status_reg_1
        0x00128674:    b51c        ..      PUSH     {r2-r4,lr}
        0x00128676:    2105        .!      MOVS     r1,#5
        0x00128678:    9100        ..      STR      r1,[sp,#0]
        0x0012867a:    2301        .#      MOVS     r3,#1
        0x0012867c:    aa01        ..      ADD      r2,sp,#4
        0x0012867e:    4619        .F      MOV      r1,r3
        0x00128680:    4668        hF      MOV      r0,sp
        0x00128682:    f000f97f    ....    BL       qspi_std_read ; 0x128984
        0x00128686:    4668        hF      MOV      r0,sp
        0x00128688:    7900        .y      LDRB     r0,[r0,#4]
        0x0012868a:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_reset
    flash_reset
        0x0012868c:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012868e:    2166        f!      MOVS     r1,#0x66
        0x00128690:    9101        ..      STR      r1,[sp,#4]
        0x00128692:    2101        .!      MOVS     r1,#1
        0x00128694:    a801        ..      ADD      r0,sp,#4
        0x00128696:    f000f9b7    ....    BL       qspi_std_write ; 0x128a08
        0x0012869a:    2199        .!      MOVS     r1,#0x99
        0x0012869c:    9100        ..      STR      r1,[sp,#0]
        0x0012869e:    2101        .!      MOVS     r1,#1
        0x001286a0:    4668        hF      MOV      r0,sp
        0x001286a2:    f000f9b1    ....    BL       qspi_std_write ; 0x128a08
        0x001286a6:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_status_read
    flash_status_read
        0x001286a8:    b51c        ..      PUSH     {r2-r4,lr}
        0x001286aa:    4604        .F      MOV      r4,r0
        0x001286ac:    2000        .       MOVS     r0,#0
        0x001286ae:    9000        ..      STR      r0,[sp,#0]
        0x001286b0:    2c00        .,      CMP      r4,#0
        0x001286b2:    d004        ..      BEQ      0x1286be ; flash_status_read + 22
        0x001286b4:    2c01        .,      CMP      r4,#1
        0x001286b6:    d00b        ..      BEQ      0x1286d0 ; flash_status_read + 40
        0x001286b8:    2c02        .,      CMP      r4,#2
        0x001286ba:    d11b        ..      BNE      0x1286f4 ; flash_status_read + 76
        0x001286bc:    e011        ..      B        0x1286e2 ; flash_status_read + 58
        0x001286be:    2005        .       MOVS     r0,#5
        0x001286c0:    9000        ..      STR      r0,[sp,#0]
        0x001286c2:    2301        .#      MOVS     r3,#1
        0x001286c4:    aa01        ..      ADD      r2,sp,#4
        0x001286c6:    4619        .F      MOV      r1,r3
        0x001286c8:    4668        hF      MOV      r0,sp
        0x001286ca:    f000f95b    ..[.    BL       qspi_std_read ; 0x128984
        0x001286ce:    e012        ..      B        0x1286f6 ; flash_status_read + 78
        0x001286d0:    2035        5       MOVS     r0,#0x35
        0x001286d2:    9000        ..      STR      r0,[sp,#0]
        0x001286d4:    2301        .#      MOVS     r3,#1
        0x001286d6:    aa01        ..      ADD      r2,sp,#4
        0x001286d8:    4619        .F      MOV      r1,r3
        0x001286da:    4668        hF      MOV      r0,sp
        0x001286dc:    f000f952    ..R.    BL       qspi_std_read ; 0x128984
        0x001286e0:    e009        ..      B        0x1286f6 ; flash_status_read + 78
        0x001286e2:    2015        .       MOVS     r0,#0x15
        0x001286e4:    9000        ..      STR      r0,[sp,#0]
        0x001286e6:    2301        .#      MOVS     r3,#1
        0x001286e8:    aa01        ..      ADD      r2,sp,#4
        0x001286ea:    4619        .F      MOV      r1,r3
        0x001286ec:    4668        hF      MOV      r0,sp
        0x001286ee:    f000f949    ..I.    BL       qspi_std_read ; 0x128984
        0x001286f2:    e000        ..      B        0x1286f6 ; flash_status_read + 78
        0x001286f4:    bf00        ..      NOP      
        0x001286f6:    bf00        ..      NOP      
        0x001286f8:    4668        hF      MOV      r0,sp
        0x001286fa:    7900        .y      LDRB     r0,[r0,#4]
        0x001286fc:    bd1c        ..      POP      {r2-r4,pc}
        0x001286fe:    0000        ..      MOVS     r0,r0
    i.flash_status_write
    flash_status_write
        0x00128700:    b5f8        ..      PUSH     {r3-r7,lr}
        0x00128702:    4604        .F      MOV      r4,r0
        0x00128704:    460d        .F      MOV      r5,r1
        0x00128706:    a020         .      ADR      r0,{pc}+0x82 ; 0x128788
        0x00128708:    6800        .h      LDR      r0,[r0,#0]
        0x0012870a:    9000        ..      STR      r0,[sp,#0]
        0x0012870c:    4668        hF      MOV      r0,sp
        0x0012870e:    7045        Ep      STRB     r5,[r0,#1]
        0x00128710:    2600        .&      MOVS     r6,#0
        0x00128712:    2c00        .,      CMP      r4,#0
        0x00128714:    d004        ..      BEQ      0x128720 ; flash_status_write + 32
        0x00128716:    2c01        .,      CMP      r4,#1
        0x00128718:    d012        ..      BEQ      0x128740 ; flash_status_write + 64
        0x0012871a:    2c02        .,      CMP      r4,#2
        0x0012871c:    d130        0.      BNE      0x128780 ; flash_status_write + 128
        0x0012871e:    e01f        ..      B        0x128760 ; flash_status_write + 96
        0x00128720:    f000f856    ..V.    BL       flash_write_enable ; 0x1287d0
        0x00128724:    2001        .       MOVS     r0,#1
        0x00128726:    4669        iF      MOV      r1,sp
        0x00128728:    7008        .p      STRB     r0,[r1,#0]
        0x0012872a:    2102        .!      MOVS     r1,#2
        0x0012872c:    4668        hF      MOV      r0,sp
        0x0012872e:    f000f96b    ..k.    BL       qspi_std_write ; 0x128a08
        0x00128732:    f7ffff01    ....    BL       flash_operation_wait ; 0x128538
        0x00128736:    4620         F      MOV      r0,r4
        0x00128738:    f7ffffb6    ....    BL       flash_status_read ; 0x1286a8
        0x0012873c:    4606        .F      MOV      r6,r0
        0x0012873e:    e020         .      B        0x128782 ; flash_status_write + 130
        0x00128740:    f000f846    ..F.    BL       flash_write_enable ; 0x1287d0
        0x00128744:    2031        1       MOVS     r0,#0x31
        0x00128746:    4669        iF      MOV      r1,sp
        0x00128748:    7008        .p      STRB     r0,[r1,#0]
        0x0012874a:    2102        .!      MOVS     r1,#2
        0x0012874c:    4668        hF      MOV      r0,sp
        0x0012874e:    f000f95b    ..[.    BL       qspi_std_write ; 0x128a08
        0x00128752:    f7fffef1    ....    BL       flash_operation_wait ; 0x128538
        0x00128756:    4620         F      MOV      r0,r4
        0x00128758:    f7ffffa6    ....    BL       flash_status_read ; 0x1286a8
        0x0012875c:    4606        .F      MOV      r6,r0
        0x0012875e:    e010        ..      B        0x128782 ; flash_status_write + 130
        0x00128760:    f000f836    ..6.    BL       flash_write_enable ; 0x1287d0
        0x00128764:    2011        .       MOVS     r0,#0x11
        0x00128766:    4669        iF      MOV      r1,sp
        0x00128768:    7008        .p      STRB     r0,[r1,#0]
        0x0012876a:    2102        .!      MOVS     r1,#2
        0x0012876c:    4668        hF      MOV      r0,sp
        0x0012876e:    f000f94b    ..K.    BL       qspi_std_write ; 0x128a08
        0x00128772:    f7fffee1    ....    BL       flash_operation_wait ; 0x128538
        0x00128776:    4620         F      MOV      r0,r4
        0x00128778:    f7ffff96    ....    BL       flash_status_read ; 0x1286a8
        0x0012877c:    4606        .F      MOV      r6,r0
        0x0012877e:    e000        ..      B        0x128782 ; flash_status_write + 130
        0x00128780:    bf00        ..      NOP      
        0x00128782:    bf00        ..      NOP      
        0x00128784:    4630        0F      MOV      r0,r6
        0x00128786:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x00128788:    00000031    1...    DCD    49
    $t
    i.flash_status_write_2bytes
    flash_status_write_2bytes
        0x0012878c:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012878e:    4604        .F      MOV      r4,r0
        0x00128790:    460d        .F      MOV      r5,r1
        0x00128792:    a00e        ..      ADR      r0,{pc}+0x3a ; 0x1287cc
        0x00128794:    6800        .h      LDR      r0,[r0,#0]
        0x00128796:    9000        ..      STR      r0,[sp,#0]
        0x00128798:    4668        hF      MOV      r0,sp
        0x0012879a:    7044        Dp      STRB     r4,[r0,#1]
        0x0012879c:    7085        .p      STRB     r5,[r0,#2]
        0x0012879e:    2600        .&      MOVS     r6,#0
        0x001287a0:    f000f816    ....    BL       flash_write_enable ; 0x1287d0
        0x001287a4:    2103        .!      MOVS     r1,#3
        0x001287a6:    4668        hF      MOV      r0,sp
        0x001287a8:    f000f92e    ....    BL       qspi_std_write ; 0x128a08
        0x001287ac:    f7fffec4    ....    BL       flash_operation_wait ; 0x128538
        0x001287b0:    2000        .       MOVS     r0,#0
        0x001287b2:    f7ffff79    ..y.    BL       flash_status_read ; 0x1286a8
        0x001287b6:    4606        .F      MOV      r6,r0
        0x001287b8:    2001        .       MOVS     r0,#1
        0x001287ba:    f7ffff75    ..u.    BL       flash_status_read ; 0x1286a8
        0x001287be:    0200        ..      LSLS     r0,r0,#8
        0x001287c0:    21ff        .!      MOVS     r1,#0xff
        0x001287c2:    0209        ..      LSLS     r1,r1,#8
        0x001287c4:    4008        .@      ANDS     r0,r0,r1
        0x001287c6:    4306        .C      ORRS     r6,r6,r0
        0x001287c8:    4630        0F      MOV      r0,r6
        0x001287ca:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x001287cc:    00000001    ....    DCD    1
    $t
    i.flash_write_enable
    flash_write_enable
        0x001287d0:    b508        ..      PUSH     {r3,lr}
        0x001287d2:    2006        .       MOVS     r0,#6
        0x001287d4:    9000        ..      STR      r0,[sp,#0]
        0x001287d6:    2101        .!      MOVS     r1,#1
        0x001287d8:    4668        hF      MOV      r0,sp
        0x001287da:    f000f915    ....    BL       qspi_std_write ; 0x128a08
        0x001287de:    bd08        ..      POP      {r3,pc}
    i.main
    main
        0x001287e0:    b510        ..      PUSH     {r4,lr}
        0x001287e2:    2022        "       MOVS     r0,#0x22
        0x001287e4:    4914        .I      LDR      r1,[pc,#80] ; [0x128838] = 0x130100
        0x001287e6:    6008        .`      STR      r0,[r1,#0]
        0x001287e8:    f000f830    ..0.    BL       open_pinshare ; 0x12884c
        0x001287ec:    f7fffd8c    ....    BL       dmac_init ; 0x128308
        0x001287f0:    2101        .!      MOVS     r1,#1
        0x001287f2:    2002        .       MOVS     r0,#2
        0x001287f4:    f000f87a    ..z.    BL       qspi_init ; 0x1288ec
        0x001287f8:    4810        .H      LDR      r0,[pc,#64] ; [0x12883c] = 0x4c11db7
        0x001287fa:    f7fffd33    ..3.    BL       crc32_init ; 0x128264
        0x001287fe:    f000f9b1    ....    BL       uart_download_init ; 0x128b64
        0x00128802:    f000fd7d    ..}.    BL       uart_sync_to_pc ; 0x129300
        0x00128806:    bf00        ..      NOP      
        0x00128808:    480d        .H      LDR      r0,[pc,#52] ; [0x128840] = 0x129492
        0x0012880a:    7800        .x      LDRB     r0,[r0,#0]
        0x0012880c:    2800        .(      CMP      r0,#0
        0x0012880e:    d0fb        ..      BEQ      0x128808 ; main + 40
        0x00128810:    f000f9ce    ....    BL       uart_download_start ; 0x128bb0
        0x00128814:    e009        ..      B        0x12882a ; main + 74
        0x00128816:    480b        .H      LDR      r0,[pc,#44] ; [0x128844] = 0x129493
        0x00128818:    7800        .x      LDRB     r0,[r0,#0]
        0x0012881a:    2800        .(      CMP      r0,#0
        0x0012881c:    d005        ..      BEQ      0x12882a ; main + 74
        0x0012881e:    207d        }       MOVS     r0,#0x7d
        0x00128820:    0180        ..      LSLS     r0,r0,#6
        0x00128822:    f7fffc59    ..Y.    BL       bx_delay_asm ; 0x1280d8
        0x00128826:    f000f8ff    ....    BL       start_ram_run_function ; 0x128a28
        0x0012882a:    4807        .H      LDR      r0,[pc,#28] ; [0x128848] = 0x129494
        0x0012882c:    7800        .x      LDRB     r0,[r0,#0]
        0x0012882e:    2800        .(      CMP      r0,#0
        0x00128830:    d0f1        ..      BEQ      0x128816 ; main + 54
        0x00128832:    2000        .       MOVS     r0,#0
        0x00128834:    bd10        ..      POP      {r4,pc}
    $d
        0x00128836:    0000        ..      DCW    0
        0x00128838:    00130100    ....    DCD    1245440
        0x0012883c:    04c11db7    ....    DCD    79764919
        0x00128840:    00129492    ....    DCD    1217682
        0x00128844:    00129493    ....    DCD    1217683
        0x00128848:    00129494    ....    DCD    1217684
    $t
    i.open_pinshare
    open_pinshare
        0x0012884c:    2003        .       MOVS     r0,#3
        0x0012884e:    4916        .I      LDR      r1,[pc,#88] ; [0x1288a8] = 0x20132000
        0x00128850:    6a09        .j      LDR      r1,[r1,#0x20]
        0x00128852:    0909        ..      LSRS     r1,r1,#4
        0x00128854:    0109        ..      LSLS     r1,r1,#4
        0x00128856:    4301        .C      ORRS     r1,r1,r0
        0x00128858:    4a13        .J      LDR      r2,[pc,#76] ; [0x1288a8] = 0x20132000
        0x0012885a:    6211        .b      STR      r1,[r2,#0x20]
        0x0012885c:    bf00        ..      NOP      
        0x0012885e:    2000        .       MOVS     r0,#0
        0x00128860:    4611        .F      MOV      r1,r2
        0x00128862:    6b49        Ik      LDR      r1,[r1,#0x34]
        0x00128864:    220f        ."      MOVS     r2,#0xf
        0x00128866:    0212        ..      LSLS     r2,r2,#8
        0x00128868:    4391        .C      BICS     r1,r1,r2
        0x0012886a:    0202        ..      LSLS     r2,r0,#8
        0x0012886c:    230f        .#      MOVS     r3,#0xf
        0x0012886e:    021b        ..      LSLS     r3,r3,#8
        0x00128870:    401a        .@      ANDS     r2,r2,r3
        0x00128872:    4311        .C      ORRS     r1,r1,r2
        0x00128874:    4a0c        .J      LDR      r2,[pc,#48] ; [0x1288a8] = 0x20132000
        0x00128876:    6351        Qc      STR      r1,[r2,#0x34]
        0x00128878:    bf00        ..      NOP      
        0x0012887a:    2001        .       MOVS     r0,#1
        0x0012887c:    4611        .F      MOV      r1,r2
        0x0012887e:    6b49        Ik      LDR      r1,[r1,#0x34]
        0x00128880:    011a        ..      LSLS     r2,r3,#4
        0x00128882:    4391        .C      BICS     r1,r1,r2
        0x00128884:    0302        ..      LSLS     r2,r0,#12
        0x00128886:    011b        ..      LSLS     r3,r3,#4
        0x00128888:    401a        .@      ANDS     r2,r2,r3
        0x0012888a:    4311        .C      ORRS     r1,r1,r2
        0x0012888c:    4a06        .J      LDR      r2,[pc,#24] ; [0x1288a8] = 0x20132000
        0x0012888e:    6351        Qc      STR      r1,[r2,#0x34]
        0x00128890:    bf00        ..      NOP      
        0x00128892:    2003        .       MOVS     r0,#3
        0x00128894:    0280        ..      LSLS     r0,r0,#10
        0x00128896:    4611        .F      MOV      r1,r2
        0x00128898:    6a49        Ij      LDR      r1,[r1,#0x24]
        0x0012889a:    0d89        ..      LSRS     r1,r1,#22
        0x0012889c:    0589        ..      LSLS     r1,r1,#22
        0x0012889e:    4301        .C      ORRS     r1,r1,r0
        0x001288a0:    6251        Qb      STR      r1,[r2,#0x24]
        0x001288a2:    bf00        ..      NOP      
        0x001288a4:    4770        pG      BX       lr
    $d
        0x001288a6:    0000        ..      DCW    0
        0x001288a8:    20132000    . .     DCD    538124288
    $t
    i.qspi_flash_program
    qspi_flash_program
        0x001288ac:    b5f8        ..      PUSH     {r3-r7,lr}
        0x001288ae:    4605        .F      MOV      r5,r0
        0x001288b0:    460c        .F      MOV      r4,r1
        0x001288b2:    4616        .F      MOV      r6,r2
        0x001288b4:    461f        .F      MOV      r7,r3
        0x001288b6:    2140        @!      MOVS     r1,#0x40
        0x001288b8:    480b        .H      LDR      r0,[pc,#44] ; [0x1288e8] = 0x20300000
        0x001288ba:    f000f825    ..%.    BL       qspi_std_byte_write_dma_config ; 0x128908
        0x001288be:    480a        .H      LDR      r0,[pc,#40] ; [0x1288e8] = 0x20300000
        0x001288c0:    3040        @0      ADDS     r0,r0,#0x40
        0x001288c2:    6205        .b      STR      r5,[r0,#0x20]
        0x001288c4:    0220         .      LSLS     r0,r4,#8
        0x001288c6:    0e00        ..      LSRS     r0,r0,#24
        0x001288c8:    4907        .I      LDR      r1,[pc,#28] ; [0x1288e8] = 0x20300000
        0x001288ca:    3140        @1      ADDS     r1,r1,#0x40
        0x001288cc:    6208        .b      STR      r0,[r1,#0x20]
        0x001288ce:    0420         .      LSLS     r0,r4,#16
        0x001288d0:    0e00        ..      LSRS     r0,r0,#24
        0x001288d2:    6208        .b      STR      r0,[r1,#0x20]
        0x001288d4:    b2e0        ..      UXTB     r0,r4
        0x001288d6:    6208        .b      STR      r0,[r1,#0x20]
        0x001288d8:    463b        ;F      MOV      r3,r7
        0x001288da:    4632        2F      MOV      r2,r6
        0x001288dc:    2101        .!      MOVS     r1,#1
        0x001288de:    4802        .H      LDR      r0,[pc,#8] ; [0x1288e8] = 0x20300000
        0x001288e0:    f000f820    .. .    BL       qspi_std_byte_write_dma_start ; 0x128924
        0x001288e4:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x001288e6:    0000        ..      DCW    0
        0x001288e8:    20300000    ..0     DCD    540016640
    $t
    i.qspi_init
    qspi_init
        0x001288ec:    2200        ."      MOVS     r2,#0
        0x001288ee:    4b05        .K      LDR      r3,[pc,#20] ; [0x128904] = 0x20300000
        0x001288f0:    609a        .`      STR      r2,[r3,#8]
        0x001288f2:    461a        .F      MOV      r2,r3
        0x001288f4:    6150        Pa      STR      r0,[r2,#0x14]
        0x001288f6:    4a03        .J      LDR      r2,[pc,#12] ; [0x128904] = 0x20300000
        0x001288f8:    32c0        .2      ADDS     r2,r2,#0xc0
        0x001288fa:    6311        .c      STR      r1,[r2,#0x30]
        0x001288fc:    2200        ."      MOVS     r2,#0
        0x001288fe:    611a        .a      STR      r2,[r3,#0x10]
        0x00128900:    4770        pG      BX       lr
    $d
        0x00128902:    0000        ..      DCW    0
        0x00128904:    20300000    ..0     DCD    540016640
    $t
    i.qspi_std_byte_write_dma_config
    qspi_std_byte_write_dma_config
        0x00128908:    4a05        .J      LDR      r2,[pc,#20] ; [0x128920] = 0x70100
        0x0012890a:    6002        .`      STR      r2,[r0,#0]
        0x0012890c:    1f0a        ..      SUBS     r2,r1,#4
        0x0012890e:    6502        .e      STR      r2,[r0,#0x50]
        0x00128910:    2200        ."      MOVS     r2,#0
        0x00128912:    64c2        .d      STR      r2,[r0,#0x4c]
        0x00128914:    2202        ."      MOVS     r2,#2
        0x00128916:    64c2        .d      STR      r2,[r0,#0x4c]
        0x00128918:    2201        ."      MOVS     r2,#1
        0x0012891a:    6082        .`      STR      r2,[r0,#8]
        0x0012891c:    4770        pG      BX       lr
    $d
        0x0012891e:    0000        ..      DCW    0
        0x00128920:    00070100    ....    DCD    459008
    $t
    i.qspi_std_byte_write_dma_start
    qspi_std_byte_write_dma_start
        0x00128924:    b5f0        ..      PUSH     {r4-r7,lr}
        0x00128926:    b085        ..      SUB      sp,sp,#0x14
        0x00128928:    4604        .F      MOV      r4,r0
        0x0012892a:    460d        .F      MOV      r5,r1
        0x0012892c:    4616        .F      MOV      r6,r2
        0x0012892e:    461f        .F      MOV      r7,r3
        0x00128930:    4813        .H      LDR      r0,[pc,#76] ; [0x128980] = 0x129450
        0x00128932:    4601        .F      MOV      r1,r0
        0x00128934:    c90e        ..      LDM      r1,{r1-r3}
        0x00128936:    68c0        .h      LDR      r0,[r0,#0xc]
        0x00128938:    9303        ..      STR      r3,[sp,#0xc]
        0x0012893a:    9202        ..      STR      r2,[sp,#8]
        0x0012893c:    9101        ..      STR      r1,[sp,#4]
        0x0012893e:    9004        ..      STR      r0,[sp,#0x10]
        0x00128940:    9601        ..      STR      r6,[sp,#4]
        0x00128942:    4620         F      MOV      r0,r4
        0x00128944:    3060        `0      ADDS     r0,r0,#0x60
        0x00128946:    9002        ..      STR      r0,[sp,#8]
        0x00128948:    9703        ..      STR      r7,[sp,#0xc]
        0x0012894a:    2100        .!      MOVS     r1,#0
        0x0012894c:    a801        ..      ADD      r0,sp,#4
        0x0012894e:    f7fffce5    ....    BL       dmac_start ; 0x12831c
        0x00128952:    9000        ..      STR      r0,[sp,#0]
        0x00128954:    6125        %a      STR      r5,[r4,#0x10]
        0x00128956:    2100        .!      MOVS     r1,#0
        0x00128958:    9800        ..      LDR      r0,[sp,#0]
        0x0012895a:    f7fffd63    ..c.    BL       dmac_transfer_wait ; 0x128424
        0x0012895e:    bf00        ..      NOP      
        0x00128960:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x00128962:    2104        .!      MOVS     r1,#4
        0x00128964:    4008        .@      ANDS     r0,r0,r1
        0x00128966:    0880        ..      LSRS     r0,r0,#2
        0x00128968:    2800        .(      CMP      r0,#0
        0x0012896a:    d0f9        ..      BEQ      0x128960 ; qspi_std_byte_write_dma_start + 60
        0x0012896c:    bf00        ..      NOP      
        0x0012896e:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x00128970:    07c0        ..      LSLS     r0,r0,#31
        0x00128972:    0fc0        ..      LSRS     r0,r0,#31
        0x00128974:    2800        .(      CMP      r0,#0
        0x00128976:    d1fa        ..      BNE      0x12896e ; qspi_std_byte_write_dma_start + 74
        0x00128978:    60a0        .`      STR      r0,[r4,#8]
        0x0012897a:    6120         a      STR      r0,[r4,#0x10]
        0x0012897c:    b005        ..      ADD      sp,sp,#0x14
        0x0012897e:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x00128980:    00129450    P...    DCD    1217616
    $t
    i.qspi_std_read
    qspi_std_read
        0x00128984:    b5ff        ..      PUSH     {r0-r7,lr}
        0x00128986:    b085        ..      SUB      sp,sp,#0x14
        0x00128988:    4606        .F      MOV      r6,r0
        0x0012898a:    460f        .F      MOV      r7,r1
        0x0012898c:    461c        .F      MOV      r4,r3
        0x0012898e:    481b        .H      LDR      r0,[pc,#108] ; [0x1289fc] = 0x70300
        0x00128990:    491b        .I      LDR      r1,[pc,#108] ; [0x128a00] = 0x20300000
        0x00128992:    6008        .`      STR      r0,[r1,#0]
        0x00128994:    1e60        `.      SUBS     r0,r4,#1
        0x00128996:    6048        H`      STR      r0,[r1,#4]
        0x00128998:    2007        .       MOVS     r0,#7
        0x0012899a:    4919        .I      LDR      r1,[pc,#100] ; [0x128a00] = 0x20300000
        0x0012899c:    3140        @1      ADDS     r1,r1,#0x40
        0x0012899e:    6148        Ha      STR      r0,[r1,#0x14]
        0x001289a0:    2000        .       MOVS     r0,#0
        0x001289a2:    60c8        .`      STR      r0,[r1,#0xc]
        0x001289a4:    2001        .       MOVS     r0,#1
        0x001289a6:    60c8        .`      STR      r0,[r1,#0xc]
        0x001289a8:    4915        .I      LDR      r1,[pc,#84] ; [0x128a00] = 0x20300000
        0x001289aa:    6088        .`      STR      r0,[r1,#8]
        0x001289ac:    2500        .%      MOVS     r5,#0
        0x001289ae:    e005        ..      B        0x1289bc ; qspi_std_read + 56
        0x001289b0:    5d70        p]      LDRB     r0,[r6,r5]
        0x001289b2:    4913        .I      LDR      r1,[pc,#76] ; [0x128a00] = 0x20300000
        0x001289b4:    3140        @1      ADDS     r1,r1,#0x40
        0x001289b6:    6208        .b      STR      r0,[r1,#0x20]
        0x001289b8:    1c68        h.      ADDS     r0,r5,#1
        0x001289ba:    b2c5        ..      UXTB     r5,r0
        0x001289bc:    42bd        .B      CMP      r5,r7
        0x001289be:    dbf7        ..      BLT      0x1289b0 ; qspi_std_read + 44
        0x001289c0:    4810        .H      LDR      r0,[pc,#64] ; [0x128a04] = 0x129460
        0x001289c2:    4601        .F      MOV      r1,r0
        0x001289c4:    c90e        ..      LDM      r1,{r1-r3}
        0x001289c6:    68c0        .h      LDR      r0,[r0,#0xc]
        0x001289c8:    9303        ..      STR      r3,[sp,#0xc]
        0x001289ca:    9202        ..      STR      r2,[sp,#8]
        0x001289cc:    9101        ..      STR      r1,[sp,#4]
        0x001289ce:    9004        ..      STR      r0,[sp,#0x10]
        0x001289d0:    9807        ..      LDR      r0,[sp,#0x1c]
        0x001289d2:    9002        ..      STR      r0,[sp,#8]
        0x001289d4:    9403        ..      STR      r4,[sp,#0xc]
        0x001289d6:    2100        .!      MOVS     r1,#0
        0x001289d8:    a801        ..      ADD      r0,sp,#4
        0x001289da:    f7fffc9f    ....    BL       dmac_start ; 0x12831c
        0x001289de:    9000        ..      STR      r0,[sp,#0]
        0x001289e0:    2001        .       MOVS     r0,#1
        0x001289e2:    4907        .I      LDR      r1,[pc,#28] ; [0x128a00] = 0x20300000
        0x001289e4:    6108        .a      STR      r0,[r1,#0x10]
        0x001289e6:    2100        .!      MOVS     r1,#0
        0x001289e8:    9800        ..      LDR      r0,[sp,#0]
        0x001289ea:    f7fffd1b    ....    BL       dmac_transfer_wait ; 0x128424
        0x001289ee:    2000        .       MOVS     r0,#0
        0x001289f0:    4903        .I      LDR      r1,[pc,#12] ; [0x128a00] = 0x20300000
        0x001289f2:    6088        .`      STR      r0,[r1,#8]
        0x001289f4:    6108        .a      STR      r0,[r1,#0x10]
        0x001289f6:    b009        ..      ADD      sp,sp,#0x24
        0x001289f8:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x001289fa:    0000        ..      DCW    0
        0x001289fc:    00070300    ....    DCD    459520
        0x00128a00:    20300000    ..0     DCD    540016640
        0x00128a04:    00129460    `...    DCD    1217632
    $t
    i.qspi_std_write
    qspi_std_write
        0x00128a08:    b570        p.      PUSH     {r4-r6,lr}
        0x00128a0a:    4604        .F      MOV      r4,r0
        0x00128a0c:    460d        .F      MOV      r5,r1
        0x00128a0e:    2140        @!      MOVS     r1,#0x40
        0x00128a10:    4804        .H      LDR      r0,[pc,#16] ; [0x128a24] = 0x20300000
        0x00128a12:    f7ffff79    ..y.    BL       qspi_std_byte_write_dma_config ; 0x128908
        0x00128a16:    462b        +F      MOV      r3,r5
        0x00128a18:    4622        "F      MOV      r2,r4
        0x00128a1a:    2101        .!      MOVS     r1,#1
        0x00128a1c:    4801        .H      LDR      r0,[pc,#4] ; [0x128a24] = 0x20300000
        0x00128a1e:    f7ffff81    ....    BL       qspi_std_byte_write_dma_start ; 0x128924
        0x00128a22:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x00128a24:    20300000    ..0     DCD    540016640
    $t
    i.start_ram_run_function
    start_ram_run_function
        0x00128a28:    b570        p.      PUSH     {r4-r6,lr}
        0x00128a2a:    f000fcf1    ....    BL       uart_uninit ; 0x129410
        0x00128a2e:    2001        .       MOVS     r0,#1
        0x00128a30:    0500        ..      LSLS     r0,r0,#20
        0x00128a32:    4908        .I      LDR      r1,[pc,#32] ; [0x128a54] = 0x1294c4
        0x00128a34:    6008        .`      STR      r0,[r1,#0]
        0x00128a36:    6809        .h      LDR      r1,[r1,#0]
        0x00128a38:    6808        .h      LDR      r0,[r1,#0]
        0x00128a3a:    f3808808    ....    MSR      MSP,r0
        0x00128a3e:    bf00        ..      NOP      
        0x00128a40:    4804        .H      LDR      r0,[pc,#16] ; [0x128a54] = 0x1294c4
        0x00128a42:    6800        .h      LDR      r0,[r0,#0]
        0x00128a44:    1d04        ..      ADDS     r4,r0,#4
        0x00128a46:    4b04        .K      LDR      r3,[pc,#16] ; [0x128a58] = 0x3399
        0x00128a48:    2200        ."      MOVS     r2,#0
        0x00128a4a:    4611        .F      MOV      r1,r2
        0x00128a4c:    4610        .F      MOV      r0,r2
        0x00128a4e:    6825        %h      LDR      r5,[r4,#0]
        0x00128a50:    47a8        .G      BLX      r5
        0x00128a52:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x00128a54:    001294c4    ....    DCD    1217732
        0x00128a58:    00003399    .3..    DCD    13209
    $t
    i.sysc_per_clk_div_param1_pack
    sysc_per_clk_div_param1_pack
        0x00128a5c:    b530        0.      PUSH     {r4,r5,lr}
        0x00128a5e:    0604        ..      LSLS     r4,r0,#24
        0x00128a60:    040d        ..      LSLS     r5,r1,#16
        0x00128a62:    432c        ,C      ORRS     r4,r4,r5
        0x00128a64:    0215        ..      LSLS     r5,r2,#8
        0x00128a66:    432c        ,C      ORRS     r4,r4,r5
        0x00128a68:    431c        .C      ORRS     r4,r4,r3
        0x00128a6a:    4d01        .M      LDR      r5,[pc,#4] ; [0x128a70] = 0x20149000
        0x00128a6c:    60ac        .`      STR      r4,[r5,#8]
        0x00128a6e:    bd30        0.      POP      {r4,r5,pc}
    $d
        0x00128a70:    20149000    ...     DCD    538218496
    $t
    i.sysc_per_clk_sel_uart0_setf
    sysc_per_clk_sel_uart0_setf
        0x00128a74:    4906        .I      LDR      r1,[pc,#24] ; [0x128a90] = 0x20149000
        0x00128a76:    6809        .h      LDR      r1,[r1,#0]
        0x00128a78:    2203        ."      MOVS     r2,#3
        0x00128a7a:    0212        ..      LSLS     r2,r2,#8
        0x00128a7c:    4391        .C      BICS     r1,r1,r2
        0x00128a7e:    0202        ..      LSLS     r2,r0,#8
        0x00128a80:    2303        .#      MOVS     r3,#3
        0x00128a82:    021b        ..      LSLS     r3,r3,#8
        0x00128a84:    401a        .@      ANDS     r2,r2,r3
        0x00128a86:    4311        .C      ORRS     r1,r1,r2
        0x00128a88:    4a01        .J      LDR      r2,[pc,#4] ; [0x128a90] = 0x20149000
        0x00128a8a:    6011        .`      STR      r1,[r2,#0]
        0x00128a8c:    4770        pG      BX       lr
    $d
        0x00128a8e:    0000        ..      DCW    0
        0x00128a90:    20149000    ...     DCD    538218496
    $t
    i.sysc_per_clkg0_pack
    sysc_per_clkg0_pack
        0x00128a94:    b5f0        ..      PUSH     {r4-r7,lr}
        0x00128a96:    9d06        ..      LDR      r5,[sp,#0x18]
        0x00128a98:    9c05        ..      LDR      r4,[sp,#0x14]
        0x00128a9a:    06c6        ..      LSLS     r6,r0,#27
        0x00128a9c:    068f        ..      LSLS     r7,r1,#26
        0x00128a9e:    433e        >C      ORRS     r6,r6,r7
        0x00128aa0:    0657        W.      LSLS     r7,r2,#25
        0x00128aa2:    433e        >C      ORRS     r6,r6,r7
        0x00128aa4:    061f        ..      LSLS     r7,r3,#24
        0x00128aa6:    433e        >C      ORRS     r6,r6,r7
        0x00128aa8:    05e7        ..      LSLS     r7,r4,#23
        0x00128aaa:    433e        >C      ORRS     r6,r6,r7
        0x00128aac:    05af        ..      LSLS     r7,r5,#22
        0x00128aae:    433e        >C      ORRS     r6,r6,r7
        0x00128ab0:    9f07        ..      LDR      r7,[sp,#0x1c]
        0x00128ab2:    057f        ..      LSLS     r7,r7,#21
        0x00128ab4:    433e        >C      ORRS     r6,r6,r7
        0x00128ab6:    9f08        ..      LDR      r7,[sp,#0x20]
        0x00128ab8:    053f        ?.      LSLS     r7,r7,#20
        0x00128aba:    433e        >C      ORRS     r6,r6,r7
        0x00128abc:    9f09        ..      LDR      r7,[sp,#0x24]
        0x00128abe:    04ff        ..      LSLS     r7,r7,#19
        0x00128ac0:    433e        >C      ORRS     r6,r6,r7
        0x00128ac2:    9f0a        ..      LDR      r7,[sp,#0x28]
        0x00128ac4:    04bf        ..      LSLS     r7,r7,#18
        0x00128ac6:    433e        >C      ORRS     r6,r6,r7
        0x00128ac8:    9f0b        ..      LDR      r7,[sp,#0x2c]
        0x00128aca:    047f        ..      LSLS     r7,r7,#17
        0x00128acc:    433e        >C      ORRS     r6,r6,r7
        0x00128ace:    9f0c        ..      LDR      r7,[sp,#0x30]
        0x00128ad0:    043f        ?.      LSLS     r7,r7,#16
        0x00128ad2:    433e        >C      ORRS     r6,r6,r7
        0x00128ad4:    9f0d        ..      LDR      r7,[sp,#0x34]
        0x00128ad6:    03ff        ..      LSLS     r7,r7,#15
        0x00128ad8:    433e        >C      ORRS     r6,r6,r7
        0x00128ada:    9f0e        ..      LDR      r7,[sp,#0x38]
        0x00128adc:    03bf        ..      LSLS     r7,r7,#14
        0x00128ade:    433e        >C      ORRS     r6,r6,r7
        0x00128ae0:    9f0f        ..      LDR      r7,[sp,#0x3c]
        0x00128ae2:    037f        ..      LSLS     r7,r7,#13
        0x00128ae4:    433e        >C      ORRS     r6,r6,r7
        0x00128ae6:    9f10        ..      LDR      r7,[sp,#0x40]
        0x00128ae8:    033f        ?.      LSLS     r7,r7,#12
        0x00128aea:    433e        >C      ORRS     r6,r6,r7
        0x00128aec:    9f11        ..      LDR      r7,[sp,#0x44]
        0x00128aee:    02ff        ..      LSLS     r7,r7,#11
        0x00128af0:    433e        >C      ORRS     r6,r6,r7
        0x00128af2:    9f12        ..      LDR      r7,[sp,#0x48]
        0x00128af4:    02bf        ..      LSLS     r7,r7,#10
        0x00128af6:    433e        >C      ORRS     r6,r6,r7
        0x00128af8:    9f13        ..      LDR      r7,[sp,#0x4c]
        0x00128afa:    027f        ..      LSLS     r7,r7,#9
        0x00128afc:    433e        >C      ORRS     r6,r6,r7
        0x00128afe:    9f14        ..      LDR      r7,[sp,#0x50]
        0x00128b00:    023f        ?.      LSLS     r7,r7,#8
        0x00128b02:    433e        >C      ORRS     r6,r6,r7
        0x00128b04:    9f15        ..      LDR      r7,[sp,#0x54]
        0x00128b06:    01ff        ..      LSLS     r7,r7,#7
        0x00128b08:    433e        >C      ORRS     r6,r6,r7
        0x00128b0a:    9f16        ..      LDR      r7,[sp,#0x58]
        0x00128b0c:    01bf        ..      LSLS     r7,r7,#6
        0x00128b0e:    433e        >C      ORRS     r6,r6,r7
        0x00128b10:    9f17        ..      LDR      r7,[sp,#0x5c]
        0x00128b12:    017f        ..      LSLS     r7,r7,#5
        0x00128b14:    433e        >C      ORRS     r6,r6,r7
        0x00128b16:    9f18        ..      LDR      r7,[sp,#0x60]
        0x00128b18:    013f        ?.      LSLS     r7,r7,#4
        0x00128b1a:    433e        >C      ORRS     r6,r6,r7
        0x00128b1c:    9f19        ..      LDR      r7,[sp,#0x64]
        0x00128b1e:    00ff        ..      LSLS     r7,r7,#3
        0x00128b20:    433e        >C      ORRS     r6,r6,r7
        0x00128b22:    9f1a        ..      LDR      r7,[sp,#0x68]
        0x00128b24:    00bf        ..      LSLS     r7,r7,#2
        0x00128b26:    433e        >C      ORRS     r6,r6,r7
        0x00128b28:    9f1b        ..      LDR      r7,[sp,#0x6c]
        0x00128b2a:    007f        ..      LSLS     r7,r7,#1
        0x00128b2c:    433e        >C      ORRS     r6,r6,r7
        0x00128b2e:    9f1c        ..      LDR      r7,[sp,#0x70]
        0x00128b30:    433e        >C      ORRS     r6,r6,r7
        0x00128b32:    4f01        .O      LDR      r7,[pc,#4] ; [0x128b38] = 0x20149000
        0x00128b34:    613e        >a      STR      r6,[r7,#0x10]
        0x00128b36:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x00128b38:    20149000    ...     DCD    538218496
    $t
    i.sysc_per_clkg0_set
    sysc_per_clkg0_set
        0x00128b3c:    4901        .I      LDR      r1,[pc,#4] ; [0x128b44] = 0x20149000
        0x00128b3e:    6108        .a      STR      r0,[r1,#0x10]
        0x00128b40:    4770        pG      BX       lr
    $d
        0x00128b42:    0000        ..      DCW    0
        0x00128b44:    20149000    ...     DCD    538218496
    $t
    i.uart_dlab_setf
    uart_dlab_setf
        0x00128b48:    4905        .I      LDR      r1,[pc,#20] ; [0x128b60] = 0x20143000
        0x00128b4a:    68c9        .h      LDR      r1,[r1,#0xc]
        0x00128b4c:    2280        ."      MOVS     r2,#0x80
        0x00128b4e:    4391        .C      BICS     r1,r1,r2
        0x00128b50:    01c2        ..      LSLS     r2,r0,#7
        0x00128b52:    2380        .#      MOVS     r3,#0x80
        0x00128b54:    401a        .@      ANDS     r2,r2,r3
        0x00128b56:    4311        .C      ORRS     r1,r1,r2
        0x00128b58:    4a01        .J      LDR      r2,[pc,#4] ; [0x128b60] = 0x20143000
        0x00128b5a:    60d1        .`      STR      r1,[r2,#0xc]
        0x00128b5c:    4770        pG      BX       lr
    $d
        0x00128b5e:    0000        ..      DCW    0
        0x00128b60:    20143000    .0.     DCD    538193920
    $t
    i.uart_download_init
    uart_download_init
        0x00128b64:    b510        ..      PUSH     {r4,lr}
        0x00128b66:    4808        .H      LDR      r0,[pc,#32] ; [0x128b88] = 0x128b99
        0x00128b68:    4908        .I      LDR      r1,[pc,#32] ; [0x128b8c] = 0x1294a4
        0x00128b6a:    6048        H`      STR      r0,[r1,#4]
        0x00128b6c:    4808        .H      LDR      r0,[pc,#32] ; [0x128b90] = 0x128be1
        0x00128b6e:    6008        .`      STR      r0,[r1,#0]
        0x00128b70:    f7fffb4a    ..J.    BL       clk_uart0_921600 ; 0x128208
        0x00128b74:    2001        .       MOVS     r0,#1
        0x00128b76:    f000f859    ..Y.    BL       uart_init ; 0x128c2c
        0x00128b7a:    200b        .       MOVS     r0,#0xb
        0x00128b7c:    2101        .!      MOVS     r1,#1
        0x00128b7e:    4081        .@      LSLS     r1,r1,r0
        0x00128b80:    4a04        .J      LDR      r2,[pc,#16] ; [0x128b94] = 0xe000e100
        0x00128b82:    6011        .`      STR      r1,[r2,#0]
        0x00128b84:    bf00        ..      NOP      
        0x00128b86:    bd10        ..      POP      {r4,pc}
    $d
        0x00128b88:    00128b99    ....    DCD    1215385
        0x00128b8c:    001294a4    ....    DCD    1217700
        0x00128b90:    00128be1    ....    DCD    1215457
        0x00128b94:    e000e100    ....    DCD    3758153984
    $t
    i.uart_download_read
    uart_download_read
        0x00128b98:    b570        p.      PUSH     {r4-r6,lr}
        0x00128b9a:    4604        .F      MOV      r4,r0
        0x00128b9c:    460d        .F      MOV      r5,r1
        0x00128b9e:    4616        .F      MOV      r6,r2
        0x00128ba0:    2300        .#      MOVS     r3,#0
        0x00128ba2:    4632        2F      MOV      r2,r6
        0x00128ba4:    4629        )F      MOV      r1,r5
        0x00128ba6:    4620         F      MOV      r0,r4
        0x00128ba8:    f000f8d6    ....    BL       uart_read ; 0x128d58
        0x00128bac:    bd70        p.      POP      {r4-r6,pc}
        0x00128bae:    0000        ..      MOVS     r0,r0
    i.uart_download_start
    uart_download_start
        0x00128bb0:    b510        ..      PUSH     {r4,lr}
        0x00128bb2:    4806        .H      LDR      r0,[pc,#24] ; [0x128bcc] = 0x1294a4
        0x00128bb4:    4a06        .J      LDR      r2,[pc,#24] ; [0x128bd0] = 0x1293c1
        0x00128bb6:    2105        .!      MOVS     r1,#5
        0x00128bb8:    6803        .h      LDR      r3,[r0,#0]
        0x00128bba:    4806        .H      LDR      r0,[pc,#24] ; [0x128bd4] = 0x1294ac
        0x00128bbc:    4798        .G      BLX      r3
        0x00128bbe:    4803        .H      LDR      r0,[pc,#12] ; [0x128bcc] = 0x1294a4
        0x00128bc0:    4a05        .J      LDR      r2,[pc,#20] ; [0x128bd8] = 0x128e79
        0x00128bc2:    2108        .!      MOVS     r1,#8
        0x00128bc4:    6843        Ch      LDR      r3,[r0,#4]
        0x00128bc6:    4805        .H      LDR      r0,[pc,#20] ; [0x128bdc] = 0x1298c8
        0x00128bc8:    4798        .G      BLX      r3
        0x00128bca:    bd10        ..      POP      {r4,pc}
    $d
        0x00128bcc:    001294a4    ....    DCD    1217700
        0x00128bd0:    001293c1    ....    DCD    1217473
        0x00128bd4:    001294ac    ....    DCD    1217708
        0x00128bd8:    00128e79    y...    DCD    1216121
        0x00128bdc:    001298c8    ....    DCD    1218760
    $t
    i.uart_download_write
    uart_download_write
        0x00128be0:    b570        p.      PUSH     {r4-r6,lr}
        0x00128be2:    4604        .F      MOV      r4,r0
        0x00128be4:    460d        .F      MOV      r5,r1
        0x00128be6:    4616        .F      MOV      r6,r2
        0x00128be8:    2300        .#      MOVS     r3,#0
        0x00128bea:    4632        2F      MOV      r2,r6
        0x00128bec:    4629        )F      MOV      r1,r5
        0x00128bee:    4620         F      MOV      r0,r4
        0x00128bf0:    f000fc1c    ....    BL       uart_write ; 0x12942c
        0x00128bf4:    bd70        p.      POP      {r4-r6,pc}
        0x00128bf6:    0000        ..      MOVS     r0,r0
    i.uart_erbfi_setf
    uart_erbfi_setf
        0x00128bf8:    4904        .I      LDR      r1,[pc,#16] ; [0x128c0c] = 0x20143000
        0x00128bfa:    6849        Ih      LDR      r1,[r1,#4]
        0x00128bfc:    0849        I.      LSRS     r1,r1,#1
        0x00128bfe:    0049        I.      LSLS     r1,r1,#1
        0x00128c00:    07c2        ..      LSLS     r2,r0,#31
        0x00128c02:    0fd2        ..      LSRS     r2,r2,#31
        0x00128c04:    4311        .C      ORRS     r1,r1,r2
        0x00128c06:    4a01        .J      LDR      r2,[pc,#4] ; [0x128c0c] = 0x20143000
        0x00128c08:    6051        Q`      STR      r1,[r2,#4]
        0x00128c0a:    4770        pG      BX       lr
    $d
        0x00128c0c:    20143000    .0.     DCD    538193920
    $t
    i.uart_etbei_setf
    uart_etbei_setf
        0x00128c10:    4905        .I      LDR      r1,[pc,#20] ; [0x128c28] = 0x20143000
        0x00128c12:    6849        Ih      LDR      r1,[r1,#4]
        0x00128c14:    2202        ."      MOVS     r2,#2
        0x00128c16:    4391        .C      BICS     r1,r1,r2
        0x00128c18:    0042        B.      LSLS     r2,r0,#1
        0x00128c1a:    2302        .#      MOVS     r3,#2
        0x00128c1c:    401a        .@      ANDS     r2,r2,r3
        0x00128c1e:    4311        .C      ORRS     r1,r1,r2
        0x00128c20:    4a01        .J      LDR      r2,[pc,#4] ; [0x128c28] = 0x20143000
        0x00128c22:    6051        Q`      STR      r1,[r2,#4]
        0x00128c24:    4770        pG      BX       lr
    $d
        0x00128c26:    0000        ..      DCW    0
        0x00128c28:    20143000    .0.     DCD    538193920
    $t
    i.uart_init
    uart_init
        0x00128c2c:    b5f1        ..      PUSH     {r0,r4-r7,lr}
        0x00128c2e:    b098        ..      SUB      sp,sp,#0x60
        0x00128c30:    2000        .       MOVS     r0,#0
        0x00128c32:    9014        ..      STR      r0,[sp,#0x50]
        0x00128c34:    9015        ..      STR      r0,[sp,#0x54]
        0x00128c36:    9016        ..      STR      r0,[sp,#0x58]
        0x00128c38:    9017        ..      STR      r0,[sp,#0x5c]
        0x00128c3a:    9010        ..      STR      r0,[sp,#0x40]
        0x00128c3c:    9011        ..      STR      r0,[sp,#0x44]
        0x00128c3e:    9012        ..      STR      r0,[sp,#0x48]
        0x00128c40:    9013        ..      STR      r0,[sp,#0x4c]
        0x00128c42:    2001        .       MOVS     r0,#1
        0x00128c44:    2100        .!      MOVS     r1,#0
        0x00128c46:    910c        ..      STR      r1,[sp,#0x30]
        0x00128c48:    900d        ..      STR      r0,[sp,#0x34]
        0x00128c4a:    910e        ..      STR      r1,[sp,#0x38]
        0x00128c4c:    9108        ..      STR      r1,[sp,#0x20]
        0x00128c4e:    9109        ..      STR      r1,[sp,#0x24]
        0x00128c50:    910a        ..      STR      r1,[sp,#0x28]
        0x00128c52:    910b        ..      STR      r1,[sp,#0x2c]
        0x00128c54:    9104        ..      STR      r1,[sp,#0x10]
        0x00128c56:    9105        ..      STR      r1,[sp,#0x14]
        0x00128c58:    9106        ..      STR      r1,[sp,#0x18]
        0x00128c5a:    9107        ..      STR      r1,[sp,#0x1c]
        0x00128c5c:    900f        ..      STR      r0,[sp,#0x3c]
        0x00128c5e:    2000        .       MOVS     r0,#0
        0x00128c60:    9000        ..      STR      r0,[sp,#0]
        0x00128c62:    9001        ..      STR      r0,[sp,#4]
        0x00128c64:    9002        ..      STR      r0,[sp,#8]
        0x00128c66:    4603        .F      MOV      r3,r0
        0x00128c68:    4602        .F      MOV      r2,r0
        0x00128c6a:    4601        .F      MOV      r1,r0
        0x00128c6c:    9003        ..      STR      r0,[sp,#0xc]
        0x00128c6e:    f7ffff11    ....    BL       sysc_per_clkg0_pack ; 0x128a94
        0x00128c72:    9818        ..      LDR      r0,[sp,#0x60]
        0x00128c74:    f000fb32    ..2.    BL       uart_set_baudrate ; 0x1292dc
        0x00128c78:    2000        .       MOVS     r0,#0
        0x00128c7a:    4921        !I      LDR      r1,[pc,#132] ; [0x128d00] = 0x20143000
        0x00128c7c:    68c9        .h      LDR      r1,[r1,#0xc]
        0x00128c7e:    2208        ."      MOVS     r2,#8
        0x00128c80:    4391        .C      BICS     r1,r1,r2
        0x00128c82:    00c2        ..      LSLS     r2,r0,#3
        0x00128c84:    2308        .#      MOVS     r3,#8
        0x00128c86:    401a        .@      ANDS     r2,r2,r3
        0x00128c88:    4311        .C      ORRS     r1,r1,r2
        0x00128c8a:    4a1d        .J      LDR      r2,[pc,#116] ; [0x128d00] = 0x20143000
        0x00128c8c:    60d1        .`      STR      r1,[r2,#0xc]
        0x00128c8e:    bf00        ..      NOP      
        0x00128c90:    bf00        ..      NOP      
        0x00128c92:    4611        .F      MOV      r1,r2
        0x00128c94:    68c9        .h      LDR      r1,[r1,#0xc]
        0x00128c96:    2204        ."      MOVS     r2,#4
        0x00128c98:    4391        .C      BICS     r1,r1,r2
        0x00128c9a:    0082        ..      LSLS     r2,r0,#2
        0x00128c9c:    2304        .#      MOVS     r3,#4
        0x00128c9e:    401a        .@      ANDS     r2,r2,r3
        0x00128ca0:    4311        .C      ORRS     r1,r1,r2
        0x00128ca2:    4a17        .J      LDR      r2,[pc,#92] ; [0x128d00] = 0x20143000
        0x00128ca4:    60d1        .`      STR      r1,[r2,#0xc]
        0x00128ca6:    bf00        ..      NOP      
        0x00128ca8:    2003        .       MOVS     r0,#3
        0x00128caa:    4611        .F      MOV      r1,r2
        0x00128cac:    68c9        .h      LDR      r1,[r1,#0xc]
        0x00128cae:    4381        .C      BICS     r1,r1,r0
        0x00128cb0:    4301        .C      ORRS     r1,r1,r0
        0x00128cb2:    60d1        .`      STR      r1,[r2,#0xc]
        0x00128cb4:    bf00        ..      NOP      
        0x00128cb6:    2000        .       MOVS     r0,#0
        0x00128cb8:    4601        .F      MOV      r1,r0
        0x00128cba:    2201        ."      MOVS     r2,#1
        0x00128cbc:    4603        .F      MOV      r3,r0
        0x00128cbe:    4604        .F      MOV      r4,r0
        0x00128cc0:    01c5        ..      LSLS     r5,r0,#7
        0x00128cc2:    00ce        ..      LSLS     r6,r1,#3
        0x00128cc4:    4335        5C      ORRS     r5,r5,r6
        0x00128cc6:    0096        ..      LSLS     r6,r2,#2
        0x00128cc8:    4335        5C      ORRS     r5,r5,r6
        0x00128cca:    005e        ^.      LSLS     r6,r3,#1
        0x00128ccc:    4335        5C      ORRS     r5,r5,r6
        0x00128cce:    4325        %C      ORRS     r5,r5,r4
        0x00128cd0:    4e0b        .N      LDR      r6,[pc,#44] ; [0x128d00] = 0x20143000
        0x00128cd2:    6075        u`      STR      r5,[r6,#4]
        0x00128cd4:    bf00        ..      NOP      
        0x00128cd6:    4602        .F      MOV      r2,r0
        0x00128cd8:    2501        .%      MOVS     r5,#1
        0x00128cda:    0186        ..      LSLS     r6,r0,#6
        0x00128cdc:    010f        ..      LSLS     r7,r1,#4
        0x00128cde:    433e        >C      ORRS     r6,r6,r7
        0x00128ce0:    00d7        ..      LSLS     r7,r2,#3
        0x00128ce2:    433e        >C      ORRS     r6,r6,r7
        0x00128ce4:    009f        ..      LSLS     r7,r3,#2
        0x00128ce6:    433e        >C      ORRS     r6,r6,r7
        0x00128ce8:    0067        g.      LSLS     r7,r4,#1
        0x00128cea:    433e        >C      ORRS     r6,r6,r7
        0x00128cec:    432e        .C      ORRS     r6,r6,r5
        0x00128cee:    4f04        .O      LDR      r7,[pc,#16] ; [0x128d00] = 0x20143000
        0x00128cf0:    60be        .`      STR      r6,[r7,#8]
        0x00128cf2:    bf00        ..      NOP      
        0x00128cf4:    2100        .!      MOVS     r1,#0
        0x00128cf6:    4803        .H      LDR      r0,[pc,#12] ; [0x128d04] = 0x12c0f0
        0x00128cf8:    7001        .p      STRB     r1,[r0,#0]
        0x00128cfa:    b019        ..      ADD      sp,sp,#0x64
        0x00128cfc:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x00128cfe:    0000        ..      DCW    0
        0x00128d00:    20143000    .0.     DCD    538193920
        0x00128d04:    0012c0f0    ....    DCD    1229040
    $t
    i.uart_isr
    uart_isr
        0x00128d08:    b510        ..      PUSH     {r4,lr}
        0x00128d0a:    e01f        ..      B        0x128d4c ; uart_isr + 68
        0x00128d0c:    bf00        ..      NOP      
        0x00128d0e:    4811        .H      LDR      r0,[pc,#68] ; [0x128d54] = 0x20143000
        0x00128d10:    6880        .h      LDR      r0,[r0,#8]
        0x00128d12:    0704        ..      LSLS     r4,r0,#28
        0x00128d14:    0f24        $.      LSRS     r4,r4,#28
        0x00128d16:    bf00        ..      NOP      
        0x00128d18:    2c01        .,      CMP      r4,#1
        0x00128d1a:    d100        ..      BNE      0x128d1e ; uart_isr + 22
        0x00128d1c:    e017        ..      B        0x128d4e ; uart_isr + 70
        0x00128d1e:    2c02        .,      CMP      r4,#2
        0x00128d20:    d00c        ..      BEQ      0x128d3c ; uart_isr + 52
        0x00128d22:    2c04        .,      CMP      r4,#4
        0x00128d24:    d007        ..      BEQ      0x128d36 ; uart_isr + 46
        0x00128d26:    2c06        .,      CMP      r4,#6
        0x00128d28:    d002        ..      BEQ      0x128d30 ; uart_isr + 40
        0x00128d2a:    2c0c        .,      CMP      r4,#0xc
        0x00128d2c:    d10c        ..      BNE      0x128d48 ; uart_isr + 64
        0x00128d2e:    e008        ..      B        0x128d42 ; uart_isr + 58
        0x00128d30:    f000f874    ..t.    BL       uart_rec_error_isr ; 0x128e1c
        0x00128d34:    e009        ..      B        0x128d4a ; uart_isr + 66
        0x00128d36:    f000f82d    ..-.    BL       uart_rec_data_avail_isr ; 0x128d94
        0x00128d3a:    e006        ..      B        0x128d4a ; uart_isr + 66
        0x00128d3c:    f000faf8    ....    BL       uart_thr_empty_isr ; 0x129330
        0x00128d40:    e003        ..      B        0x128d4a ; uart_isr + 66
        0x00128d42:    f000f827    ..'.    BL       uart_rec_data_avail_isr ; 0x128d94
        0x00128d46:    e000        ..      B        0x128d4a ; uart_isr + 66
        0x00128d48:    bf00        ..      NOP      
        0x00128d4a:    bf00        ..      NOP      
        0x00128d4c:    e7de        ..      B        0x128d0c ; uart_isr + 4
        0x00128d4e:    bf00        ..      NOP      
        0x00128d50:    bd10        ..      POP      {r4,pc}
    $d
        0x00128d52:    0000        ..      DCW    0
        0x00128d54:    20143000    .0.     DCD    538193920
    $t
    i.uart_read
    uart_read
        0x00128d58:    b5f8        ..      PUSH     {r3-r7,lr}
        0x00128d5a:    4604        .F      MOV      r4,r0
        0x00128d5c:    460d        .F      MOV      r5,r1
        0x00128d5e:    4616        .F      MOV      r6,r2
        0x00128d60:    461f        .F      MOV      r7,r3
        0x00128d62:    4804        .H      LDR      r0,[pc,#16] ; [0x128d74] = 0x12c0d0
        0x00128d64:    6105        .a      STR      r5,[r0,#0x10]
        0x00128d66:    6144        Da      STR      r4,[r0,#0x14]
        0x00128d68:    6186        .a      STR      r6,[r0,#0x18]
        0x00128d6a:    61c7        .a      STR      r7,[r0,#0x1c]
        0x00128d6c:    2001        .       MOVS     r0,#1
        0x00128d6e:    f7ffff43    ..C.    BL       uart_erbfi_setf ; 0x128bf8
        0x00128d72:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x00128d74:    0012c0d0    ....    DCD    1229008
    $t
    i.uart_read32_register
    uart_read32_register
        0x00128d78:    b530        0.      PUSH     {r4,r5,lr}
        0x00128d7a:    4603        .F      MOV      r3,r0
        0x00128d7c:    2000        .       MOVS     r0,#0
        0x00128d7e:    e005        ..      B        0x128d8c ; uart_read32_register + 20
        0x00128d80:    680c        .h      LDR      r4,[r1,#0]
        0x00128d82:    0085        ..      LSLS     r5,r0,#2
        0x00128d84:    515c        \Q      STR      r4,[r3,r5]
        0x00128d86:    1d09        ..      ADDS     r1,r1,#4
        0x00128d88:    1c44        D.      ADDS     r4,r0,#1
        0x00128d8a:    b2a0        ..      UXTH     r0,r4
        0x00128d8c:    4290        .B      CMP      r0,r2
        0x00128d8e:    dbf7        ..      BLT      0x128d80 ; uart_read32_register + 8
        0x00128d90:    bd30        0.      POP      {r4,r5,pc}
        0x00128d92:    0000        ..      MOVS     r0,r0
    i.uart_rec_data_avail_isr
    uart_rec_data_avail_isr
        0x00128d94:    b570        p.      PUSH     {r4-r6,lr}
        0x00128d96:    2400        .$      MOVS     r4,#0
        0x00128d98:    2500        .%      MOVS     r5,#0
        0x00128d9a:    e032        2.      B        0x128e02 ; uart_rec_data_avail_isr + 110
        0x00128d9c:    bf00        ..      NOP      
        0x00128d9e:    481d        .H      LDR      r0,[pc,#116] ; [0x128e14] = 0x20143000
        0x00128da0:    6800        .h      LDR      r0,[r0,#0]
        0x00128da2:    b2c1        ..      UXTB     r1,r0
        0x00128da4:    481c        .H      LDR      r0,[pc,#112] ; [0x128e18] = 0x12c0d0
        0x00128da6:    6940        @i      LDR      r0,[r0,#0x14]
        0x00128da8:    7001        .p      STRB     r1,[r0,#0]
        0x00128daa:    481b        .H      LDR      r0,[pc,#108] ; [0x128e18] = 0x12c0d0
        0x00128dac:    6900        .i      LDR      r0,[r0,#0x10]
        0x00128dae:    1e40        @.      SUBS     r0,r0,#1
        0x00128db0:    4919        .I      LDR      r1,[pc,#100] ; [0x128e18] = 0x12c0d0
        0x00128db2:    6108        .a      STR      r0,[r1,#0x10]
        0x00128db4:    4608        .F      MOV      r0,r1
        0x00128db6:    6940        @i      LDR      r0,[r0,#0x14]
        0x00128db8:    1c40        @.      ADDS     r0,r0,#1
        0x00128dba:    6148        Ha      STR      r0,[r1,#0x14]
        0x00128dbc:    4608        .F      MOV      r0,r1
        0x00128dbe:    6900        .i      LDR      r0,[r0,#0x10]
        0x00128dc0:    2800        .(      CMP      r0,#0
        0x00128dc2:    d11e        ..      BNE      0x128e02 ; uart_rec_data_avail_isr + 110
        0x00128dc4:    2100        .!      MOVS     r1,#0
        0x00128dc6:    4814        .H      LDR      r0,[pc,#80] ; [0x128e18] = 0x12c0d0
        0x00128dc8:    6141        Aa      STR      r1,[r0,#0x14]
        0x00128dca:    2000        .       MOVS     r0,#0
        0x00128dcc:    f7ffff14    ....    BL       uart_erbfi_setf ; 0x128bf8
        0x00128dd0:    4811        .H      LDR      r0,[pc,#68] ; [0x128e18] = 0x12c0d0
        0x00128dd2:    6984        .i      LDR      r4,[r0,#0x18]
        0x00128dd4:    69c5        .i      LDR      r5,[r0,#0x1c]
        0x00128dd6:    2c00        .,      CMP      r4,#0
        0x00128dd8:    d011        ..      BEQ      0x128dfe ; uart_rec_data_avail_isr + 106
        0x00128dda:    2100        .!      MOVS     r1,#0
        0x00128ddc:    6181        .a      STR      r1,[r0,#0x18]
        0x00128dde:    61c1        .a      STR      r1,[r0,#0x1c]
        0x00128de0:    3020         0      ADDS     r0,r0,#0x20
        0x00128de2:    7800        .x      LDRB     r0,[r0,#0]
        0x00128de4:    2800        .(      CMP      r0,#0
        0x00128de6:    d102        ..      BNE      0x128dee ; uart_rec_data_avail_isr + 90
        0x00128de8:    4628        (F      MOV      r0,r5
        0x00128dea:    47a0        .G      BLX      r4
        0x00128dec:    e008        ..      B        0x128e00 ; uart_rec_data_avail_isr + 108
        0x00128dee:    2100        .!      MOVS     r1,#0
        0x00128df0:    4809        .H      LDR      r0,[pc,#36] ; [0x128e18] = 0x12c0d0
        0x00128df2:    3020         0      ADDS     r0,r0,#0x20
        0x00128df4:    7001        .p      STRB     r1,[r0,#0]
        0x00128df6:    2101        .!      MOVS     r1,#1
        0x00128df8:    4628        (F      MOV      r0,r5
        0x00128dfa:    47a0        .G      BLX      r4
        0x00128dfc:    e000        ..      B        0x128e00 ; uart_rec_data_avail_isr + 108
        0x00128dfe:    bf00        ..      NOP      
        0x00128e00:    e006        ..      B        0x128e10 ; uart_rec_data_avail_isr + 124
        0x00128e02:    bf00        ..      NOP      
        0x00128e04:    4803        .H      LDR      r0,[pc,#12] ; [0x128e14] = 0x20143000
        0x00128e06:    6940        @i      LDR      r0,[r0,#0x14]
        0x00128e08:    07c0        ..      LSLS     r0,r0,#31
        0x00128e0a:    0fc0        ..      LSRS     r0,r0,#31
        0x00128e0c:    2800        .(      CMP      r0,#0
        0x00128e0e:    d1c5        ..      BNE      0x128d9c ; uart_rec_data_avail_isr + 8
        0x00128e10:    bf00        ..      NOP      
        0x00128e12:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x00128e14:    20143000    .0.     DCD    538193920
        0x00128e18:    0012c0d0    ....    DCD    1229008
    $t
    i.uart_rec_error_isr
    uart_rec_error_isr
        0x00128e1c:    b570        p.      PUSH     {r4-r6,lr}
        0x00128e1e:    2400        .$      MOVS     r4,#0
        0x00128e20:    2600        .&      MOVS     r6,#0
        0x00128e22:    bf00        ..      NOP      
        0x00128e24:    4812        .H      LDR      r0,[pc,#72] ; [0x128e70] = 0x20143000
        0x00128e26:    6940        @i      LDR      r0,[r0,#0x14]
        0x00128e28:    b2c5        ..      UXTB     r5,r0
        0x00128e2a:    07a8        ..      LSLS     r0,r5,#30
        0x00128e2c:    0fc0        ..      LSRS     r0,r0,#31
        0x00128e2e:    2800        .(      CMP      r0,#0
        0x00128e30:    d014        ..      BEQ      0x128e5c ; uart_rec_error_isr + 64
        0x00128e32:    e003        ..      B        0x128e3c ; uart_rec_error_isr + 32
        0x00128e34:    bf00        ..      NOP      
        0x00128e36:    480e        .H      LDR      r0,[pc,#56] ; [0x128e70] = 0x20143000
        0x00128e38:    6800        .h      LDR      r0,[r0,#0]
        0x00128e3a:    bf00        ..      NOP      
        0x00128e3c:    bf00        ..      NOP      
        0x00128e3e:    480c        .H      LDR      r0,[pc,#48] ; [0x128e70] = 0x20143000
        0x00128e40:    6940        @i      LDR      r0,[r0,#0x14]
        0x00128e42:    07c0        ..      LSLS     r0,r0,#31
        0x00128e44:    0fc0        ..      LSRS     r0,r0,#31
        0x00128e46:    2800        .(      CMP      r0,#0
        0x00128e48:    d1f4        ..      BNE      0x128e34 ; uart_rec_error_isr + 24
        0x00128e4a:    2c00        .,      CMP      r4,#0
        0x00128e4c:    d006        ..      BEQ      0x128e5c ; uart_rec_error_isr + 64
        0x00128e4e:    2100        .!      MOVS     r1,#0
        0x00128e50:    4808        .H      LDR      r0,[pc,#32] ; [0x128e74] = 0x12c0d0
        0x00128e52:    6181        .a      STR      r1,[r0,#0x18]
        0x00128e54:    61c1        .a      STR      r1,[r0,#0x1c]
        0x00128e56:    2101        .!      MOVS     r1,#1
        0x00128e58:    4630        0F      MOV      r0,r6
        0x00128e5a:    47a0        .G      BLX      r4
        0x00128e5c:    0728        (.      LSLS     r0,r5,#28
        0x00128e5e:    0fc0        ..      LSRS     r0,r0,#31
        0x00128e60:    2800        .(      CMP      r0,#0
        0x00128e62:    d003        ..      BEQ      0x128e6c ; uart_rec_error_isr + 80
        0x00128e64:    2101        .!      MOVS     r1,#1
        0x00128e66:    4803        .H      LDR      r0,[pc,#12] ; [0x128e74] = 0x12c0d0
        0x00128e68:    3020         0      ADDS     r0,r0,#0x20
        0x00128e6a:    7001        .p      STRB     r1,[r0,#0]
        0x00128e6c:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x00128e6e:    0000        ..      DCW    0
        0x00128e70:    20143000    .0.     DCD    538193920
        0x00128e74:    0012c0d0    ....    DCD    1229008
    $t
    i.uart_rx_header_finish
    uart_rx_header_finish
        0x00128e78:    b510        ..      PUSH     {r4,lr}
        0x00128e7a:    4804        .H      LDR      r0,[pc,#16] ; [0x128e8c] = 0x1298c8
        0x00128e7c:    8841        A.      LDRH     r1,[r0,#2]
        0x00128e7e:    4804        .H      LDR      r0,[pc,#16] ; [0x128e90] = 0x1294a4
        0x00128e80:    4a04        .J      LDR      r2,[pc,#16] ; [0x128e94] = 0x128e99
        0x00128e82:    6843        Ch      LDR      r3,[r0,#4]
        0x00128e84:    4801        .H      LDR      r0,[pc,#4] ; [0x128e8c] = 0x1298c8
        0x00128e86:    3008        .0      ADDS     r0,r0,#8
        0x00128e88:    4798        .G      BLX      r3
        0x00128e8a:    bd10        ..      POP      {r4,pc}
    $d
        0x00128e8c:    001298c8    ....    DCD    1218760
        0x00128e90:    001294a4    ....    DCD    1217700
        0x00128e94:    00128e99    ....    DCD    1216153
    $t
    i.uart_rx_payload_finish
    uart_rx_payload_finish
        0x00128e98:    b5fe        ..      PUSH     {r1-r7,lr}
        0x00128e9a:    48ea        .H      LDR      r0,[pc,#936] ; [0x129244] = 0x1298c8
        0x00128e9c:    7800        .x      LDRB     r0,[r0,#0]
        0x00128e9e:    28ad        .(      CMP      r0,#0xad
        0x00128ea0:    d006        ..      BEQ      0x128eb0 ; uart_rx_payload_finish + 24
        0x00128ea2:    48e9        .H      LDR      r0,[pc,#932] ; [0x129248] = 0x1294a4
        0x00128ea4:    4ae9        .J      LDR      r2,[pc,#932] ; [0x12924c] = 0x1293c1
        0x00128ea6:    2105        .!      MOVS     r1,#5
        0x00128ea8:    6803        .h      LDR      r3,[r0,#0]
        0x00128eaa:    48e9        .H      LDR      r0,[pc,#932] ; [0x129250] = 0x1294b6
        0x00128eac:    4798        .G      BLX      r3
        0x00128eae:    e1c1        ..      B        0x129234 ; uart_rx_payload_finish + 924
        0x00128eb0:    48e4        .H      LDR      r0,[pc,#912] ; [0x129244] = 0x1298c8
        0x00128eb2:    7840        @x      LDRB     r0,[r0,#1]
        0x00128eb4:    0740        @.      LSLS     r0,r0,#29
        0x00128eb6:    0fc0        ..      LSRS     r0,r0,#31
        0x00128eb8:    2800        .(      CMP      r0,#0
        0x00128eba:    d171        q.      BNE      0x128fa0 ; uart_rx_payload_finish + 264
        0x00128ebc:    9002        ..      STR      r0,[sp,#8]
        0x00128ebe:    48e1        .H      LDR      r0,[pc,#900] ; [0x129244] = 0x1298c8
        0x00128ec0:    7840        @x      LDRB     r0,[r0,#1]
        0x00128ec2:    08c0        ..      LSRS     r0,r0,#3
        0x00128ec4:    0003        ..      MOVS     r3,r0
        0x00128ec6:    f7fff94b    ..K.    BL       __ARM_common_switch8 ; 0x128160
    $d
        0x00128eca:    f00a        ..      DCW    61450
        0x00128ecc:    6f553f06    .?Uo    DCD    1867857670
        0x00128ed0:    d6c4ab92    ....    DCD    3603213202
        0x00128ed4:    f0f1        ..      DCW    61681
    $t
        0x00128ed6:    48db        .H      LDR      r0,[pc,#876] ; [0x129244] = 0x1298c8
        0x00128ed8:    8840        @.      LDRH     r0,[r0,#2]
        0x00128eda:    2802        .(      CMP      r0,#2
        0x00128edc:    d131        1.      BNE      0x128f42 ; uart_rx_payload_finish + 170
        0x00128ede:    48d9        .H      LDR      r0,[pc,#868] ; [0x129244] = 0x1298c8
        0x00128ee0:    7a00        .z      LDRB     r0,[r0,#8]
        0x00128ee2:    2846        F(      CMP      r0,#0x46
        0x00128ee4:    d109        ..      BNE      0x128efa ; uart_rx_payload_finish + 98
        0x00128ee6:    48d7        .H      LDR      r0,[pc,#860] ; [0x129244] = 0x1298c8
        0x00128ee8:    7a40        @z      LDRB     r0,[r0,#9]
        0x00128eea:    2844        D(      CMP      r0,#0x44
        0x00128eec:    d105        ..      BNE      0x128efa ; uart_rx_payload_finish + 98
        0x00128eee:    2001        .       MOVS     r0,#1
        0x00128ef0:    49d8        .I      LDR      r1,[pc,#864] ; [0x129254] = 0x129490
        0x00128ef2:    7008        .p      STRB     r0,[r1,#0]
        0x00128ef4:    f000f9c8    ....    BL       uart_send_crc32_result ; 0x129288
        0x00128ef8:    e025        %.      B        0x128f46 ; uart_rx_payload_finish + 174
        0x00128efa:    48d2        .H      LDR      r0,[pc,#840] ; [0x129244] = 0x1298c8
        0x00128efc:    7a00        .z      LDRB     r0,[r0,#8]
        0x00128efe:    2843        C(      CMP      r0,#0x43
        0x00128f00:    d107        ..      BNE      0x128f12 ; uart_rx_payload_finish + 122
        0x00128f02:    48d0        .H      LDR      r0,[pc,#832] ; [0x129244] = 0x1298c8
        0x00128f04:    7a40        @z      LDRB     r0,[r0,#9]
        0x00128f06:    2846        F(      CMP      r0,#0x46
        0x00128f08:    d103        ..      BNE      0x128f12 ; uart_rx_payload_finish + 122
        0x00128f0a:    20ff        .       MOVS     r0,#0xff
        0x00128f0c:    49d2        .I      LDR      r1,[pc,#840] ; [0x129258] = 0x129491
        0x00128f0e:    7008        .p      STRB     r0,[r1,#0]
        0x00128f10:    bdfe        ..      POP      {r1-r7,pc}
        0x00128f12:    48cc        .H      LDR      r0,[pc,#816] ; [0x129244] = 0x1298c8
        0x00128f14:    7a00        .z      LDRB     r0,[r0,#8]
        0x00128f16:    2843        C(      CMP      r0,#0x43
        0x00128f18:    d107        ..      BNE      0x128f2a ; uart_rx_payload_finish + 146
        0x00128f1a:    48ca        .H      LDR      r0,[pc,#808] ; [0x129244] = 0x1298c8
        0x00128f1c:    7a40        @z      LDRB     r0,[r0,#9]
        0x00128f1e:    2850        P(      CMP      r0,#0x50
        0x00128f20:    d103        ..      BNE      0x128f2a ; uart_rx_payload_finish + 146
        0x00128f22:    2001        .       MOVS     r0,#1
        0x00128f24:    49cc        .I      LDR      r1,[pc,#816] ; [0x129258] = 0x129491
        0x00128f26:    7008        .p      STRB     r0,[r1,#0]
        0x00128f28:    e7f2        ..      B        0x128f10 ; uart_rx_payload_finish + 120
        0x00128f2a:    48c6        .H      LDR      r0,[pc,#792] ; [0x129244] = 0x1298c8
        0x00128f2c:    7a00        .z      LDRB     r0,[r0,#8]
        0x00128f2e:    2850        P(      CMP      r0,#0x50
        0x00128f30:    d109        ..      BNE      0x128f46 ; uart_rx_payload_finish + 174
        0x00128f32:    48c4        .H      LDR      r0,[pc,#784] ; [0x129244] = 0x1298c8
        0x00128f34:    7a40        @z      LDRB     r0,[r0,#9]
        0x00128f36:    2852        R(      CMP      r0,#0x52
        0x00128f38:    d105        ..      BNE      0x128f46 ; uart_rx_payload_finish + 174
        0x00128f3a:    2001        .       MOVS     r0,#1
        0x00128f3c:    49c7        .I      LDR      r1,[pc,#796] ; [0x12925c] = 0x129492
        0x00128f3e:    7008        .p      STRB     r0,[r1,#0]
        0x00128f40:    e7e6        ..      B        0x128f10 ; uart_rx_payload_finish + 120
        0x00128f42:    2001        .       MOVS     r0,#1
        0x00128f44:    9002        ..      STR      r0,[sp,#8]
        0x00128f46:    e0d9        ..      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00128f48:    48be        .H      LDR      r0,[pc,#760] ; [0x129244] = 0x1298c8
        0x00128f4a:    8840        @.      LDRH     r0,[r0,#2]
        0x00128f4c:    2802        .(      CMP      r0,#2
        0x00128f4e:    d10e        ..      BNE      0x128f6e ; uart_rx_payload_finish + 214
        0x00128f50:    2000        .       MOVS     r0,#0
        0x00128f52:    9001        ..      STR      r0,[sp,#4]
        0x00128f54:    48bb        .H      LDR      r0,[pc,#748] ; [0x129244] = 0x1298c8
        0x00128f56:    7a07        .z      LDRB     r7,[r0,#8]
        0x00128f58:    4638        8F      MOV      r0,r7
        0x00128f5a:    f7fffba5    ....    BL       flash_status_read ; 0x1286a8
        0x00128f5e:    9001        ..      STR      r0,[sp,#4]
        0x00128f60:    48b9        .H      LDR      r0,[pc,#740] ; [0x129248] = 0x1294a4
        0x00128f62:    4aba        .J      LDR      r2,[pc,#744] ; [0x12924c] = 0x1293c1
        0x00128f64:    2101        .!      MOVS     r1,#1
        0x00128f66:    6803        .h      LDR      r3,[r0,#0]
        0x00128f68:    a801        ..      ADD      r0,sp,#4
        0x00128f6a:    4798        .G      BLX      r3
        0x00128f6c:    e001        ..      B        0x128f72 ; uart_rx_payload_finish + 218
        0x00128f6e:    2001        .       MOVS     r0,#1
        0x00128f70:    9002        ..      STR      r0,[sp,#8]
        0x00128f72:    e0c3        ..      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00128f74:    48b3        .H      LDR      r0,[pc,#716] ; [0x129244] = 0x1298c8
        0x00128f76:    8840        @.      LDRH     r0,[r0,#2]
        0x00128f78:    2802        .(      CMP      r0,#2
        0x00128f7a:    d112        ..      BNE      0x128fa2 ; uart_rx_payload_finish + 266
        0x00128f7c:    48b1        .H      LDR      r0,[pc,#708] ; [0x129244] = 0x1298c8
        0x00128f7e:    7a04        .z      LDRB     r4,[r0,#8]
        0x00128f80:    4601        .F      MOV      r1,r0
        0x00128f82:    7a49        Iz      LDRB     r1,[r1,#9]
        0x00128f84:    0209        ..      LSLS     r1,r1,#8
        0x00128f86:    430c        .C      ORRS     r4,r4,r1
        0x00128f88:    4622        "F      MOV      r2,r4
        0x00128f8a:    6841        Ah      LDR      r1,[r0,#4]
        0x00128f8c:    48b4        .H      LDR      r0,[pc,#720] ; [0x129260] = 0x1294c8
        0x00128f8e:    f7fffef3    ....    BL       uart_read32_register ; 0x128d78
        0x00128f92:    00a1        ..      LSLS     r1,r4,#2
        0x00128f94:    48ac        .H      LDR      r0,[pc,#688] ; [0x129248] = 0x1294a4
        0x00128f96:    4aad        .J      LDR      r2,[pc,#692] ; [0x12924c] = 0x1293c1
        0x00128f98:    6803        .h      LDR      r3,[r0,#0]
        0x00128f9a:    48b1        .H      LDR      r0,[pc,#708] ; [0x129260] = 0x1294c8
        0x00128f9c:    4798        .G      BLX      r3
        0x00128f9e:    e002        ..      B        0x128fa6 ; uart_rx_payload_finish + 270
        0x00128fa0:    e0b7        ..      B        0x129112 ; uart_rx_payload_finish + 634
        0x00128fa2:    2001        .       MOVS     r0,#1
        0x00128fa4:    9002        ..      STR      r0,[sp,#8]
        0x00128fa6:    e0a9        ..      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00128fa8:    48a6        .H      LDR      r0,[pc,#664] ; [0x129244] = 0x1298c8
        0x00128faa:    8840        @.      LDRH     r0,[r0,#2]
        0x00128fac:    2804        .(      CMP      r0,#4
        0x00128fae:    d11b        ..      BNE      0x128fe8 ; uart_rx_payload_finish + 336
        0x00128fb0:    48a4        .H      LDR      r0,[pc,#656] ; [0x129244] = 0x1298c8
        0x00128fb2:    7a00        .z      LDRB     r0,[r0,#8]
        0x00128fb4:    49a3        .I      LDR      r1,[pc,#652] ; [0x129244] = 0x1298c8
        0x00128fb6:    7a49        Iz      LDRB     r1,[r1,#9]
        0x00128fb8:    0209        ..      LSLS     r1,r1,#8
        0x00128fba:    4308        .C      ORRS     r0,r0,r1
        0x00128fbc:    49a1        .I      LDR      r1,[pc,#644] ; [0x129244] = 0x1298c8
        0x00128fbe:    7a89        .z      LDRB     r1,[r1,#0xa]
        0x00128fc0:    0409        ..      LSLS     r1,r1,#16
        0x00128fc2:    4308        .C      ORRS     r0,r0,r1
        0x00128fc4:    499f        .I      LDR      r1,[pc,#636] ; [0x129244] = 0x1298c8
        0x00128fc6:    7ac9        .z      LDRB     r1,[r1,#0xb]
        0x00128fc8:    0609        ..      LSLS     r1,r1,#24
        0x00128fca:    4308        .C      ORRS     r0,r0,r1
        0x00128fcc:    4607        .F      MOV      r7,r0
        0x00128fce:    499d        .I      LDR      r1,[pc,#628] ; [0x129244] = 0x1298c8
        0x00128fd0:    6848        Hh      LDR      r0,[r1,#4]
        0x00128fd2:    4639        9F      MOV      r1,r7
        0x00128fd4:    f000fa3a    ..:.    BL       uart_write_register ; 0x12944c
        0x00128fd8:    489b        .H      LDR      r0,[pc,#620] ; [0x129248] = 0x1294a4
        0x00128fda:    4a9c        .J      LDR      r2,[pc,#624] ; [0x12924c] = 0x1293c1
        0x00128fdc:    2105        .!      MOVS     r1,#5
        0x00128fde:    6803        .h      LDR      r3,[r0,#0]
        0x00128fe0:    489b        .H      LDR      r0,[pc,#620] ; [0x129250] = 0x1294b6
        0x00128fe2:    1f40        @.      SUBS     r0,r0,#5
        0x00128fe4:    4798        .G      BLX      r3
        0x00128fe6:    e001        ..      B        0x128fec ; uart_rx_payload_finish + 340
        0x00128fe8:    2001        .       MOVS     r0,#1
        0x00128fea:    9002        ..      STR      r0,[sp,#8]
        0x00128fec:    e086        ..      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00128fee:    4895        .H      LDR      r0,[pc,#596] ; [0x129244] = 0x1298c8
        0x00128ff0:    8840        @.      LDRH     r0,[r0,#2]
        0x00128ff2:    2802        .(      CMP      r0,#2
        0x00128ff4:    d111        ..      BNE      0x12901a ; uart_rx_payload_finish + 386
        0x00128ff6:    2000        .       MOVS     r0,#0
        0x00128ff8:    9001        ..      STR      r0,[sp,#4]
        0x00128ffa:    4892        .H      LDR      r0,[pc,#584] ; [0x129244] = 0x1298c8
        0x00128ffc:    7a07        .z      LDRB     r7,[r0,#8]
        0x00128ffe:    7a40        @z      LDRB     r0,[r0,#9]
        0x00129000:    9000        ..      STR      r0,[sp,#0]
        0x00129002:    4638        8F      MOV      r0,r7
        0x00129004:    9900        ..      LDR      r1,[sp,#0]
        0x00129006:    f7fffb7b    ..{.    BL       flash_status_write ; 0x128700
        0x0012900a:    9001        ..      STR      r0,[sp,#4]
        0x0012900c:    488e        .H      LDR      r0,[pc,#568] ; [0x129248] = 0x1294a4
        0x0012900e:    4a8f        .J      LDR      r2,[pc,#572] ; [0x12924c] = 0x1293c1
        0x00129010:    2101        .!      MOVS     r1,#1
        0x00129012:    6803        .h      LDR      r3,[r0,#0]
        0x00129014:    a801        ..      ADD      r0,sp,#4
        0x00129016:    4798        .G      BLX      r3
        0x00129018:    e001        ..      B        0x12901e ; uart_rx_payload_finish + 390
        0x0012901a:    2001        .       MOVS     r0,#1
        0x0012901c:    9002        ..      STR      r0,[sp,#8]
        0x0012901e:    e06d        m.      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00129020:    4888        .H      LDR      r0,[pc,#544] ; [0x129244] = 0x1298c8
        0x00129022:    8840        @.      LDRH     r0,[r0,#2]
        0x00129024:    2802        .(      CMP      r0,#2
        0x00129026:    d111        ..      BNE      0x12904c ; uart_rx_payload_finish + 436
        0x00129028:    2000        .       MOVS     r0,#0
        0x0012902a:    9001        ..      STR      r0,[sp,#4]
        0x0012902c:    4885        .H      LDR      r0,[pc,#532] ; [0x129244] = 0x1298c8
        0x0012902e:    7a07        .z      LDRB     r7,[r0,#8]
        0x00129030:    7a40        @z      LDRB     r0,[r0,#9]
        0x00129032:    9000        ..      STR      r0,[sp,#0]
        0x00129034:    4638        8F      MOV      r0,r7
        0x00129036:    9900        ..      LDR      r1,[sp,#0]
        0x00129038:    f7fffba8    ....    BL       flash_status_write_2bytes ; 0x12878c
        0x0012903c:    9001        ..      STR      r0,[sp,#4]
        0x0012903e:    4882        .H      LDR      r0,[pc,#520] ; [0x129248] = 0x1294a4
        0x00129040:    4a82        .J      LDR      r2,[pc,#520] ; [0x12924c] = 0x1293c1
        0x00129042:    2102        .!      MOVS     r1,#2
        0x00129044:    6803        .h      LDR      r3,[r0,#0]
        0x00129046:    a801        ..      ADD      r0,sp,#4
        0x00129048:    4798        .G      BLX      r3
        0x0012904a:    e001        ..      B        0x129050 ; uart_rx_payload_finish + 440
        0x0012904c:    2001        .       MOVS     r0,#1
        0x0012904e:    9002        ..      STR      r0,[sp,#8]
        0x00129050:    e054        T.      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00129052:    487c        |H      LDR      r0,[pc,#496] ; [0x129244] = 0x1298c8
        0x00129054:    8840        @.      LDRH     r0,[r0,#2]
        0x00129056:    2802        .(      CMP      r0,#2
        0x00129058:    d10a        ..      BNE      0x129070 ; uart_rx_payload_finish + 472
        0x0012905a:    2001        .       MOVS     r0,#1
        0x0012905c:    4981        .I      LDR      r1,[pc,#516] ; [0x129264] = 0x1294bc
        0x0012905e:    7008        .p      STRB     r0,[r1,#0]
        0x00129060:    4879        yH      LDR      r0,[pc,#484] ; [0x129248] = 0x1294a4
        0x00129062:    4a7a        zJ      LDR      r2,[pc,#488] ; [0x12924c] = 0x1293c1
        0x00129064:    2105        .!      MOVS     r1,#5
        0x00129066:    6803        .h      LDR      r3,[r0,#0]
        0x00129068:    4879        yH      LDR      r0,[pc,#484] ; [0x129250] = 0x1294b6
        0x0012906a:    1f40        @.      SUBS     r0,r0,#5
        0x0012906c:    4798        .G      BLX      r3
        0x0012906e:    e001        ..      B        0x129074 ; uart_rx_payload_finish + 476
        0x00129070:    2001        .       MOVS     r0,#1
        0x00129072:    9002        ..      STR      r0,[sp,#8]
        0x00129074:    e042        B.      B        0x1290fc ; uart_rx_payload_finish + 612
        0x00129076:    4873        sH      LDR      r0,[pc,#460] ; [0x129244] = 0x1298c8
        0x00129078:    8840        @.      LDRH     r0,[r0,#2]
        0x0012907a:    2802        .(      CMP      r0,#2
        0x0012907c:    d111        ..      BNE      0x1290a2 ; uart_rx_payload_finish + 522
        0x0012907e:    2101        .!      MOVS     r1,#1
        0x00129080:    4879        yH      LDR      r0,[pc,#484] ; [0x129268] = 0x1294bb
        0x00129082:    7001        .p      STRB     r1,[r0,#0]
        0x00129084:    4879        yH      LDR      r0,[pc,#484] ; [0x12926c] = 0x1294a0
        0x00129086:    7800        .x      LDRB     r0,[r0,#0]
        0x00129088:    2800        .(      CMP      r0,#0
        0x0012908a:    d002        ..      BEQ      0x129092 ; uart_rx_payload_finish + 506
        0x0012908c:    f000f8fc    ....    BL       uart_send_crc32_result ; 0x129288
        0x00129090:    e009        ..      B        0x1290a6 ; uart_rx_payload_finish + 526
        0x00129092:    486d        mH      LDR      r0,[pc,#436] ; [0x129248] = 0x1294a4
        0x00129094:    4a76        vJ      LDR      r2,[pc,#472] ; [0x129270] = 0x1293c5
        0x00129096:    2105        .!      MOVS     r1,#5
        0x00129098:    6803        .h      LDR      r3,[r0,#0]
        0x0012909a:    486d        mH      LDR      r0,[pc,#436] ; [0x129250] = 0x1294b6
        0x0012909c:    1f40        @.      SUBS     r0,r0,#5
        0x0012909e:    4798        .G      BLX      r3
        0x001290a0:    e001        ..      B        0x1290a6 ; uart_rx_payload_finish + 526
        0x001290a2:    2001        .       MOVS     r0,#1
        0x001290a4:    9002        ..      STR      r0,[sp,#8]
        0x001290a6:    e029        ).      B        0x1290fc ; uart_rx_payload_finish + 612
        0x001290a8:    e000        ..      B        0x1290ac ; uart_rx_payload_finish + 532
        0x001290aa:    e024        $.      B        0x1290f6 ; uart_rx_payload_finish + 606
        0x001290ac:    4865        eH      LDR      r0,[pc,#404] ; [0x129244] = 0x1298c8
        0x001290ae:    8840        @.      LDRH     r0,[r0,#2]
        0x001290b0:    2804        .(      CMP      r0,#4
        0x001290b2:    d11d        ..      BNE      0x1290f0 ; uart_rx_payload_finish + 600
        0x001290b4:    2001        .       MOVS     r0,#1
        0x001290b6:    496d        mI      LDR      r1,[pc,#436] ; [0x12926c] = 0x1294a0
        0x001290b8:    7008        .p      STRB     r0,[r1,#0]
        0x001290ba:    1e80        ..      SUBS     r0,r0,#2
        0x001290bc:    496d        mI      LDR      r1,[pc,#436] ; [0x129274] = 0x129498
        0x001290be:    6008        .`      STR      r0,[r1,#0]
        0x001290c0:    4860        `H      LDR      r0,[pc,#384] ; [0x129244] = 0x1298c8
        0x001290c2:    7a00        .z      LDRB     r0,[r0,#8]
        0x001290c4:    495f        _I      LDR      r1,[pc,#380] ; [0x129244] = 0x1298c8
        0x001290c6:    7a49        Iz      LDRB     r1,[r1,#9]
        0x001290c8:    0209        ..      LSLS     r1,r1,#8
        0x001290ca:    4308        .C      ORRS     r0,r0,r1
        0x001290cc:    495d        ]I      LDR      r1,[pc,#372] ; [0x129244] = 0x1298c8
        0x001290ce:    7a89        .z      LDRB     r1,[r1,#0xa]
        0x001290d0:    0409        ..      LSLS     r1,r1,#16
        0x001290d2:    4308        .C      ORRS     r0,r0,r1
        0x001290d4:    495b        [I      LDR      r1,[pc,#364] ; [0x129244] = 0x1298c8
        0x001290d6:    7ac9        .z      LDRB     r1,[r1,#0xb]
        0x001290d8:    0609        ..      LSLS     r1,r1,#24
        0x001290da:    4308        .C      ORRS     r0,r0,r1
        0x001290dc:    4966        fI      LDR      r1,[pc,#408] ; [0x129278] = 0x12949c
        0x001290de:    6008        .`      STR      r0,[r1,#0]
        0x001290e0:    4859        YH      LDR      r0,[pc,#356] ; [0x129248] = 0x1294a4
        0x001290e2:    4a5a        ZJ      LDR      r2,[pc,#360] ; [0x12924c] = 0x1293c1
        0x001290e4:    2105        .!      MOVS     r1,#5
        0x001290e6:    6803        .h      LDR      r3,[r0,#0]
        0x001290e8:    4859        YH      LDR      r0,[pc,#356] ; [0x129250] = 0x1294b6
        0x001290ea:    1f40        @.      SUBS     r0,r0,#5
        0x001290ec:    4798        .G      BLX      r3
        0x001290ee:    e001        ..      B        0x1290f4 ; uart_rx_payload_finish + 604
        0x001290f0:    2001        .       MOVS     r0,#1
        0x001290f2:    9002        ..      STR      r0,[sp,#8]
        0x001290f4:    e002        ..      B        0x1290fc ; uart_rx_payload_finish + 612
        0x001290f6:    2001        .       MOVS     r0,#1
        0x001290f8:    9002        ..      STR      r0,[sp,#8]
        0x001290fa:    bf00        ..      NOP      
        0x001290fc:    bf00        ..      NOP      
        0x001290fe:    9802        ..      LDR      r0,[sp,#8]
        0x00129100:    2801        .(      CMP      r0,#1
        0x00129102:    d105        ..      BNE      0x129110 ; uart_rx_payload_finish + 632
        0x00129104:    4850        PH      LDR      r0,[pc,#320] ; [0x129248] = 0x1294a4
        0x00129106:    4a51        QJ      LDR      r2,[pc,#324] ; [0x12924c] = 0x1293c1
        0x00129108:    2105        .!      MOVS     r1,#5
        0x0012910a:    6803        .h      LDR      r3,[r0,#0]
        0x0012910c:    4850        PH      LDR      r0,[pc,#320] ; [0x129250] = 0x1294b6
        0x0012910e:    4798        .G      BLX      r3
        0x00129110:    e090        ..      B        0x129234 ; uart_rx_payload_finish + 924
        0x00129112:    484c        LH      LDR      r0,[pc,#304] ; [0x129244] = 0x1298c8
        0x00129114:    7840        @x      LDRB     r0,[r0,#1]
        0x00129116:    0780        ..      LSLS     r0,r0,#30
        0x00129118:    0f80        ..      LSRS     r0,r0,#30
        0x0012911a:    2800        .(      CMP      r0,#0
        0x0012911c:    d004        ..      BEQ      0x129128 ; uart_rx_payload_finish + 656
        0x0012911e:    2801        .(      CMP      r0,#1
        0x00129120:    d017        ..      BEQ      0x129152 ; uart_rx_payload_finish + 698
        0x00129122:    2802        .(      CMP      r0,#2
        0x00129124:    d17e        ~.      BNE      0x129224 ; uart_rx_payload_finish + 908
        0x00129126:    e066        f.      B        0x1291f6 ; uart_rx_payload_finish + 862
        0x00129128:    4846        FH      LDR      r0,[pc,#280] ; [0x129244] = 0x1298c8
        0x0012912a:    7a04        .z      LDRB     r4,[r0,#8]
        0x0012912c:    4601        .F      MOV      r1,r0
        0x0012912e:    7a49        Iz      LDRB     r1,[r1,#9]
        0x00129130:    0209        ..      LSLS     r1,r1,#8
        0x00129132:    430c        .C      ORRS     r4,r4,r1
        0x00129134:    4601        .F      MOV      r1,r0
        0x00129136:    460a        .F      MOV      r2,r1
        0x00129138:    3208        .2      ADDS     r2,r2,#8
        0x0012913a:    6848        Hh      LDR      r0,[r1,#4]
        0x0012913c:    4621        !F      MOV      r1,r4
        0x0012913e:    f7fffa59    ..Y.    BL       flash_read ; 0x1285f4
        0x00129142:    4841        AH      LDR      r0,[pc,#260] ; [0x129248] = 0x1294a4
        0x00129144:    4a41        AJ      LDR      r2,[pc,#260] ; [0x12924c] = 0x1293c1
        0x00129146:    4621        !F      MOV      r1,r4
        0x00129148:    6803        .h      LDR      r3,[r0,#0]
        0x0012914a:    483e        >H      LDR      r0,[pc,#248] ; [0x129244] = 0x1298c8
        0x0012914c:    3008        .0      ADDS     r0,r0,#8
        0x0012914e:    4798        .G      BLX      r3
        0x00129150:    e06f        o.      B        0x129232 ; uart_rx_payload_finish + 922
        0x00129152:    4846        FH      LDR      r0,[pc,#280] ; [0x12926c] = 0x1294a0
        0x00129154:    7800        .x      LDRB     r0,[r0,#0]
        0x00129156:    2800        .(      CMP      r0,#0
        0x00129158:    d00d        ..      BEQ      0x129176 ; uart_rx_payload_finish + 734
        0x0012915a:    483a        :H      LDR      r0,[pc,#232] ; [0x129244] = 0x1298c8
        0x0012915c:    8842        B.      LDRH     r2,[r0,#2]
        0x0012915e:    4601        .F      MOV      r1,r0
        0x00129160:    3108        .1      ADDS     r1,r1,#8
        0x00129162:    4844        DH      LDR      r0,[pc,#272] ; [0x129274] = 0x129498
        0x00129164:    6800        .h      LDR      r0,[r0,#0]
        0x00129166:    f7fff863    ..c.    BL       crc32_calc ; 0x128230
        0x0012916a:    4942        BI      LDR      r1,[pc,#264] ; [0x129274] = 0x129498
        0x0012916c:    6008        .`      STR      r0,[r1,#0]
        0x0012916e:    4608        .F      MOV      r0,r1
        0x00129170:    6800        .h      LDR      r0,[r0,#0]
        0x00129172:    43c0        .C      MVNS     r0,r0
        0x00129174:    6008        .`      STR      r0,[r1,#0]
        0x00129176:    483b        ;H      LDR      r0,[pc,#236] ; [0x129264] = 0x1294bc
        0x00129178:    7800        .x      LDRB     r0,[r0,#0]
        0x0012917a:    2800        .(      CMP      r0,#0
        0x0012917c:    d124        $.      BNE      0x1291c8 ; uart_rx_payload_finish + 816
        0x0012917e:    2600        .&      MOVS     r6,#0
        0x00129180:    e01d        ..      B        0x1291be ; uart_rx_payload_finish + 806
        0x00129182:    4830        0H      LDR      r0,[pc,#192] ; [0x129244] = 0x1298c8
        0x00129184:    8840        @.      LDRH     r0,[r0,#2]
        0x00129186:    21ff        .!      MOVS     r1,#0xff
        0x00129188:    3101        .1      ADDS     r1,#1
        0x0012918a:    4288        .B      CMP      r0,r1
        0x0012918c:    dd01        ..      BLE      0x129192 ; uart_rx_payload_finish + 762
        0x0012918e:    4608        .F      MOV      r0,r1
        0x00129190:    e001        ..      B        0x129196 ; uart_rx_payload_finish + 766
        0x00129192:    482c        ,H      LDR      r0,[pc,#176] ; [0x129244] = 0x1298c8
        0x00129194:    8840        @.      LDRH     r0,[r0,#2]
        0x00129196:    4605        .F      MOV      r5,r0
        0x00129198:    492a        *I      LDR      r1,[pc,#168] ; [0x129244] = 0x1298c8
        0x0012919a:    3108        .1      ADDS     r1,r1,#8
        0x0012919c:    198a        ..      ADDS     r2,r1,r6
        0x0012919e:    3908        .9      SUBS     r1,r1,#8
        0x001291a0:    6848        Hh      LDR      r0,[r1,#4]
        0x001291a2:    4629        )F      MOV      r1,r5
        0x001291a4:    f7fff9d1    ....    BL       flash_program ; 0x12854a
        0x001291a8:    1970        p.      ADDS     r0,r6,r5
        0x001291aa:    b286        ..      UXTH     r6,r0
        0x001291ac:    4825        %H      LDR      r0,[pc,#148] ; [0x129244] = 0x1298c8
        0x001291ae:    8840        @.      LDRH     r0,[r0,#2]
        0x001291b0:    1b40        @.      SUBS     r0,r0,r5
        0x001291b2:    4924        $I      LDR      r1,[pc,#144] ; [0x129244] = 0x1298c8
        0x001291b4:    8048        H.      STRH     r0,[r1,#2]
        0x001291b6:    4608        .F      MOV      r0,r1
        0x001291b8:    6840        @h      LDR      r0,[r0,#4]
        0x001291ba:    1940        @.      ADDS     r0,r0,r5
        0x001291bc:    6048        H`      STR      r0,[r1,#4]
        0x001291be:    4821        !H      LDR      r0,[pc,#132] ; [0x129244] = 0x1298c8
        0x001291c0:    8840        @.      LDRH     r0,[r0,#2]
        0x001291c2:    2800        .(      CMP      r0,#0
        0x001291c4:    d1dd        ..      BNE      0x129182 ; uart_rx_payload_finish + 746
        0x001291c6:    e00e        ..      B        0x1291e6 ; uart_rx_payload_finish + 846
        0x001291c8:    481e        .H      LDR      r0,[pc,#120] ; [0x129244] = 0x1298c8
        0x001291ca:    8842        B.      LDRH     r2,[r0,#2]
        0x001291cc:    4601        .F      MOV      r1,r0
        0x001291ce:    3108        .1      ADDS     r1,r1,#8
        0x001291d0:    482a        *H      LDR      r0,[pc,#168] ; [0x12927c] = 0x1294c0
        0x001291d2:    6800        .h      LDR      r0,[r0,#0]
        0x001291d4:    f7feff84    ....    BL       __aeabi_memcpy ; 0x1280e0
        0x001291d8:    481a        .H      LDR      r0,[pc,#104] ; [0x129244] = 0x1298c8
        0x001291da:    8840        @.      LDRH     r0,[r0,#2]
        0x001291dc:    4927        'I      LDR      r1,[pc,#156] ; [0x12927c] = 0x1294c0
        0x001291de:    6809        .h      LDR      r1,[r1,#0]
        0x001291e0:    1840        @.      ADDS     r0,r0,r1
        0x001291e2:    4926        &I      LDR      r1,[pc,#152] ; [0x12927c] = 0x1294c0
        0x001291e4:    6008        .`      STR      r0,[r1,#0]
        0x001291e6:    4818        .H      LDR      r0,[pc,#96] ; [0x129248] = 0x1294a4
        0x001291e8:    4a18        .J      LDR      r2,[pc,#96] ; [0x12924c] = 0x1293c1
        0x001291ea:    2105        .!      MOVS     r1,#5
        0x001291ec:    6803        .h      LDR      r3,[r0,#0]
        0x001291ee:    4818        .H      LDR      r0,[pc,#96] ; [0x129250] = 0x1294b6
        0x001291f0:    1f40        @.      SUBS     r0,r0,#5
        0x001291f2:    4798        .G      BLX      r3
        0x001291f4:    e01d        ..      B        0x129232 ; uart_rx_payload_finish + 922
        0x001291f6:    4813        .H      LDR      r0,[pc,#76] ; [0x129244] = 0x1298c8
        0x001291f8:    7a00        .z      LDRB     r0,[r0,#8]
        0x001291fa:    9002        ..      STR      r0,[sp,#8]
        0x001291fc:    4811        .H      LDR      r0,[pc,#68] ; [0x129244] = 0x1298c8
        0x001291fe:    6847        Gh      LDR      r7,[r0,#4]
        0x00129200:    f7fffa44    ..D.    BL       flash_reset ; 0x12868c
        0x00129204:    481e        .H      LDR      r0,[pc,#120] ; [0x129280] = 0x27100
        0x00129206:    f7feff67    ..g.    BL       bx_delay_asm ; 0x1280d8
        0x0012920a:    4638        8F      MOV      r0,r7
        0x0012920c:    9902        ..      LDR      r1,[sp,#8]
        0x0012920e:    f7fff93f    ..?.    BL       flash_erase ; 0x128490
        0x00129212:    480d        .H      LDR      r0,[pc,#52] ; [0x129248] = 0x1294a4
        0x00129214:    4a0d        .J      LDR      r2,[pc,#52] ; [0x12924c] = 0x1293c1
        0x00129216:    2105        .!      MOVS     r1,#5
        0x00129218:    6803        .h      LDR      r3,[r0,#0]
        0x0012921a:    480d        .H      LDR      r0,[pc,#52] ; [0x129250] = 0x1294b6
        0x0012921c:    1f40        @.      SUBS     r0,r0,#5
        0x0012921e:    4798        .G      BLX      r3
        0x00129220:    e007        ..      B        0x129232 ; uart_rx_payload_finish + 922
        0x00129222:    e7ff        ..      B        0x129224 ; uart_rx_payload_finish + 908
        0x00129224:    4808        .H      LDR      r0,[pc,#32] ; [0x129248] = 0x1294a4
        0x00129226:    4a09        .J      LDR      r2,[pc,#36] ; [0x12924c] = 0x1293c1
        0x00129228:    2105        .!      MOVS     r1,#5
        0x0012922a:    6803        .h      LDR      r3,[r0,#0]
        0x0012922c:    4808        .H      LDR      r0,[pc,#32] ; [0x129250] = 0x1294b6
        0x0012922e:    4798        .G      BLX      r3
        0x00129230:    bf00        ..      NOP      
        0x00129232:    bf00        ..      NOP      
        0x00129234:    4804        .H      LDR      r0,[pc,#16] ; [0x129248] = 0x1294a4
        0x00129236:    4a13        .J      LDR      r2,[pc,#76] ; [0x129284] = 0x128e79
        0x00129238:    2108        .!      MOVS     r1,#8
        0x0012923a:    6843        Ch      LDR      r3,[r0,#4]
        0x0012923c:    4801        .H      LDR      r0,[pc,#4] ; [0x129244] = 0x1298c8
        0x0012923e:    4798        .G      BLX      r3
        0x00129240:    bf00        ..      NOP      
        0x00129242:    e665        e.      B        0x128f10 ; uart_rx_payload_finish + 120
    $d
        0x00129244:    001298c8    ....    DCD    1218760
        0x00129248:    001294a4    ....    DCD    1217700
        0x0012924c:    001293c1    ....    DCD    1217473
        0x00129250:    001294b6    ....    DCD    1217718
        0x00129254:    00129490    ....    DCD    1217680
        0x00129258:    00129491    ....    DCD    1217681
        0x0012925c:    00129492    ....    DCD    1217682
        0x00129260:    001294c8    ....    DCD    1217736
        0x00129264:    001294bc    ....    DCD    1217724
        0x00129268:    001294bb    ....    DCD    1217723
        0x0012926c:    001294a0    ....    DCD    1217696
        0x00129270:    001293c5    ....    DCD    1217477
        0x00129274:    00129498    ....    DCD    1217688
        0x00129278:    0012949c    ....    DCD    1217692
        0x0012927c:    001294c0    ....    DCD    1217728
        0x00129280:    00027100    .q..    DCD    160000
        0x00129284:    00128e79    y...    DCD    1216121
    $t
    i.uart_send_crc32_result
    uart_send_crc32_result
        0x00129288:    b510        ..      PUSH     {r4,lr}
        0x0012928a:    480e        .H      LDR      r0,[pc,#56] ; [0x1292c4] = 0x1294a0
        0x0012928c:    7800        .x      LDRB     r0,[r0,#0]
        0x0012928e:    2800        .(      CMP      r0,#0
        0x00129290:    d017        ..      BEQ      0x1292c2 ; uart_send_crc32_result + 58
        0x00129292:    480d        .H      LDR      r0,[pc,#52] ; [0x1292c8] = 0x129498
        0x00129294:    6800        .h      LDR      r0,[r0,#0]
        0x00129296:    43c0        .C      MVNS     r0,r0
        0x00129298:    490b        .I      LDR      r1,[pc,#44] ; [0x1292c8] = 0x129498
        0x0012929a:    6008        .`      STR      r0,[r1,#0]
        0x0012929c:    480b        .H      LDR      r0,[pc,#44] ; [0x1292cc] = 0x12949c
        0x0012929e:    6800        .h      LDR      r0,[r0,#0]
        0x001292a0:    6809        .h      LDR      r1,[r1,#0]
        0x001292a2:    4288        .B      CMP      r0,r1
        0x001292a4:    d106        ..      BNE      0x1292b4 ; uart_send_crc32_result + 44
        0x001292a6:    480a        .H      LDR      r0,[pc,#40] ; [0x1292d0] = 0x1294a4
        0x001292a8:    4a0a        .J      LDR      r2,[pc,#40] ; [0x1292d4] = 0x1293c5
        0x001292aa:    2105        .!      MOVS     r1,#5
        0x001292ac:    6803        .h      LDR      r3,[r0,#0]
        0x001292ae:    480a        .H      LDR      r0,[pc,#40] ; [0x1292d8] = 0x1294b1
        0x001292b0:    4798        .G      BLX      r3
        0x001292b2:    e006        ..      B        0x1292c2 ; uart_send_crc32_result + 58
        0x001292b4:    4806        .H      LDR      r0,[pc,#24] ; [0x1292d0] = 0x1294a4
        0x001292b6:    4a07        .J      LDR      r2,[pc,#28] ; [0x1292d4] = 0x1293c5
        0x001292b8:    2105        .!      MOVS     r1,#5
        0x001292ba:    6803        .h      LDR      r3,[r0,#0]
        0x001292bc:    4806        .H      LDR      r0,[pc,#24] ; [0x1292d8] = 0x1294b1
        0x001292be:    1d40        @.      ADDS     r0,r0,#5
        0x001292c0:    4798        .G      BLX      r3
        0x001292c2:    bd10        ..      POP      {r4,pc}
    $d
        0x001292c4:    001294a0    ....    DCD    1217696
        0x001292c8:    00129498    ....    DCD    1217688
        0x001292cc:    0012949c    ....    DCD    1217692
        0x001292d0:    001294a4    ....    DCD    1217700
        0x001292d4:    001293c5    ....    DCD    1217477
        0x001292d8:    001294b1    ....    DCD    1217713
    $t
    i.uart_set_baudrate
    uart_set_baudrate
        0x001292dc:    b510        ..      PUSH     {r4,lr}
        0x001292de:    4604        .F      MOV      r4,r0
        0x001292e0:    2001        .       MOVS     r0,#1
        0x001292e2:    f7fffc31    ..1.    BL       uart_dlab_setf ; 0x128b48
        0x001292e6:    b2e0        ..      UXTB     r0,r4
        0x001292e8:    4904        .I      LDR      r1,[pc,#16] ; [0x1292fc] = 0x20143000
        0x001292ea:    6008        .`      STR      r0,[r1,#0]
        0x001292ec:    bf00        ..      NOP      
        0x001292ee:    1220         .      ASRS     r0,r4,#8
        0x001292f0:    6048        H`      STR      r0,[r1,#4]
        0x001292f2:    bf00        ..      NOP      
        0x001292f4:    2000        .       MOVS     r0,#0
        0x001292f6:    f7fffc27    ..'.    BL       uart_dlab_setf ; 0x128b48
        0x001292fa:    bd10        ..      POP      {r4,pc}
    $d
        0x001292fc:    20143000    .0.     DCD    538193920
    $t
    i.uart_sync_to_pc
    uart_sync_to_pc
        0x00129300:    b510        ..      PUSH     {r4,lr}
        0x00129302:    4806        .H      LDR      r0,[pc,#24] ; [0x12931c] = 0x1294a4
        0x00129304:    4a06        .J      LDR      r2,[pc,#24] ; [0x129320] = 0x1293c1
        0x00129306:    2105        .!      MOVS     r1,#5
        0x00129308:    6803        .h      LDR      r3,[r0,#0]
        0x0012930a:    4806        .H      LDR      r0,[pc,#24] ; [0x129324] = 0x1294b6
        0x0012930c:    4798        .G      BLX      r3
        0x0012930e:    4803        .H      LDR      r0,[pc,#12] ; [0x12931c] = 0x1294a4
        0x00129310:    4a05        .J      LDR      r2,[pc,#20] ; [0x129328] = 0x128e79
        0x00129312:    2108        .!      MOVS     r1,#8
        0x00129314:    6843        Ch      LDR      r3,[r0,#4]
        0x00129316:    4805        .H      LDR      r0,[pc,#20] ; [0x12932c] = 0x1298c8
        0x00129318:    4798        .G      BLX      r3
        0x0012931a:    bd10        ..      POP      {r4,pc}
    $d
        0x0012931c:    001294a4    ....    DCD    1217700
        0x00129320:    001293c1    ....    DCD    1217473
        0x00129324:    001294b6    ....    DCD    1217718
        0x00129328:    00128e79    y...    DCD    1216121
        0x0012932c:    001298c8    ....    DCD    1218760
    $t
    i.uart_thr_empty_isr
    uart_thr_empty_isr
        0x00129330:    b570        p.      PUSH     {r4-r6,lr}
        0x00129332:    2400        .$      MOVS     r4,#0
        0x00129334:    2500        .%      MOVS     r5,#0
        0x00129336:    4820         H      LDR      r0,[pc,#128] ; [0x1293b8] = 0x12c0d0
        0x00129338:    6800        .h      LDR      r0,[r0,#0]
        0x0012933a:    2800        .(      CMP      r0,#0
        0x0012933c:    d11b        ..      BNE      0x129376 ; uart_thr_empty_isr + 70
        0x0012933e:    bf00        ..      NOP      
        0x00129340:    bf00        ..      NOP      
        0x00129342:    481e        .H      LDR      r0,[pc,#120] ; [0x1293bc] = 0x20143000
        0x00129344:    6940        @i      LDR      r0,[r0,#0x14]
        0x00129346:    0680        ..      LSLS     r0,r0,#26
        0x00129348:    0fc0        ..      LSRS     r0,r0,#31
        0x0012934a:    2800        .(      CMP      r0,#0
        0x0012934c:    d0f8        ..      BEQ      0x129340 ; uart_thr_empty_isr + 16
        0x0012934e:    2000        .       MOVS     r0,#0
        0x00129350:    4919        .I      LDR      r1,[pc,#100] ; [0x1293b8] = 0x12c0d0
        0x00129352:    6048        H`      STR      r0,[r1,#4]
        0x00129354:    f7fffc5c    ..\.    BL       uart_etbei_setf ; 0x128c10
        0x00129358:    4817        .H      LDR      r0,[pc,#92] ; [0x1293b8] = 0x12c0d0
        0x0012935a:    6884        .h      LDR      r4,[r0,#8]
        0x0012935c:    68c5        .h      LDR      r5,[r0,#0xc]
        0x0012935e:    2c00        .,      CMP      r4,#0
        0x00129360:    d007        ..      BEQ      0x129372 ; uart_thr_empty_isr + 66
        0x00129362:    2000        .       MOVS     r0,#0
        0x00129364:    4914        .I      LDR      r1,[pc,#80] ; [0x1293b8] = 0x12c0d0
        0x00129366:    6088        .`      STR      r0,[r1,#8]
        0x00129368:    60c8        .`      STR      r0,[r1,#0xc]
        0x0012936a:    2100        .!      MOVS     r1,#0
        0x0012936c:    4628        (F      MOV      r0,r5
        0x0012936e:    47a0        .G      BLX      r4
        0x00129370:    e000        ..      B        0x129374 ; uart_thr_empty_isr + 68
        0x00129372:    bf00        ..      NOP      
        0x00129374:    bd70        p.      POP      {r4-r6,pc}
        0x00129376:    e013        ..      B        0x1293a0 ; uart_thr_empty_isr + 112
        0x00129378:    490f        .I      LDR      r1,[pc,#60] ; [0x1293b8] = 0x12c0d0
        0x0012937a:    6849        Ih      LDR      r1,[r1,#4]
        0x0012937c:    7808        .x      LDRB     r0,[r1,#0]
        0x0012937e:    490f        .I      LDR      r1,[pc,#60] ; [0x1293bc] = 0x20143000
        0x00129380:    6008        .`      STR      r0,[r1,#0]
        0x00129382:    bf00        ..      NOP      
        0x00129384:    480c        .H      LDR      r0,[pc,#48] ; [0x1293b8] = 0x12c0d0
        0x00129386:    6800        .h      LDR      r0,[r0,#0]
        0x00129388:    1e40        @.      SUBS     r0,r0,#1
        0x0012938a:    490b        .I      LDR      r1,[pc,#44] ; [0x1293b8] = 0x12c0d0
        0x0012938c:    6008        .`      STR      r0,[r1,#0]
        0x0012938e:    4608        .F      MOV      r0,r1
        0x00129390:    6840        @h      LDR      r0,[r0,#4]
        0x00129392:    1c40        @.      ADDS     r0,r0,#1
        0x00129394:    6048        H`      STR      r0,[r1,#4]
        0x00129396:    4608        .F      MOV      r0,r1
        0x00129398:    6800        .h      LDR      r0,[r0,#0]
        0x0012939a:    2800        .(      CMP      r0,#0
        0x0012939c:    d100        ..      BNE      0x1293a0 ; uart_thr_empty_isr + 112
        0x0012939e:    e007        ..      B        0x1293b0 ; uart_thr_empty_isr + 128
        0x001293a0:    bf00        ..      NOP      
        0x001293a2:    4806        .H      LDR      r0,[pc,#24] ; [0x1293bc] = 0x20143000
        0x001293a4:    3040        @0      ADDS     r0,r0,#0x40
        0x001293a6:    6bc0        .k      LDR      r0,[r0,#0x3c]
        0x001293a8:    0780        ..      LSLS     r0,r0,#30
        0x001293aa:    0fc0        ..      LSRS     r0,r0,#31
        0x001293ac:    2800        .(      CMP      r0,#0
        0x001293ae:    d1e3        ..      BNE      0x129378 ; uart_thr_empty_isr + 72
        0x001293b0:    bf00        ..      NOP      
        0x001293b2:    bf00        ..      NOP      
        0x001293b4:    e7de        ..      B        0x129374 ; uart_thr_empty_isr + 68
    $d
        0x001293b6:    0000        ..      DCW    0
        0x001293b8:    0012c0d0    ....    DCD    1229008
        0x001293bc:    20143000    .0.     DCD    538193920
    $t
    i.uart_tx_finish
    uart_tx_finish
        0x001293c0:    4770        pG      BX       lr
        0x001293c2:    0000        ..      MOVS     r0,r0
    i.uart_tx_finish_ram_run
    uart_tx_finish_ram_run
        0x001293c4:    480c        .H      LDR      r0,[pc,#48] ; [0x1293f8] = 0x1294bc
        0x001293c6:    7800        .x      LDRB     r0,[r0,#0]
        0x001293c8:    2800        .(      CMP      r0,#0
        0x001293ca:    d014        ..      BEQ      0x1293f6 ; uart_tx_finish_ram_run + 50
        0x001293cc:    480b        .H      LDR      r0,[pc,#44] ; [0x1293fc] = 0x1294bb
        0x001293ce:    7800        .x      LDRB     r0,[r0,#0]
        0x001293d0:    2800        .(      CMP      r0,#0
        0x001293d2:    d010        ..      BEQ      0x1293f6 ; uart_tx_finish_ram_run + 50
        0x001293d4:    480a        .H      LDR      r0,[pc,#40] ; [0x129400] = 0x1294a0
        0x001293d6:    7800        .x      LDRB     r0,[r0,#0]
        0x001293d8:    2800        .(      CMP      r0,#0
        0x001293da:    d009        ..      BEQ      0x1293f0 ; uart_tx_finish_ram_run + 44
        0x001293dc:    4809        .H      LDR      r0,[pc,#36] ; [0x129404] = 0x12949c
        0x001293de:    6800        .h      LDR      r0,[r0,#0]
        0x001293e0:    4909        .I      LDR      r1,[pc,#36] ; [0x129408] = 0x129498
        0x001293e2:    6809        .h      LDR      r1,[r1,#0]
        0x001293e4:    4288        .B      CMP      r0,r1
        0x001293e6:    d106        ..      BNE      0x1293f6 ; uart_tx_finish_ram_run + 50
        0x001293e8:    2001        .       MOVS     r0,#1
        0x001293ea:    4908        .I      LDR      r1,[pc,#32] ; [0x12940c] = 0x129493
        0x001293ec:    7008        .p      STRB     r0,[r1,#0]
        0x001293ee:    e002        ..      B        0x1293f6 ; uart_tx_finish_ram_run + 50
        0x001293f0:    2001        .       MOVS     r0,#1
        0x001293f2:    4906        .I      LDR      r1,[pc,#24] ; [0x12940c] = 0x129493
        0x001293f4:    7008        .p      STRB     r0,[r1,#0]
        0x001293f6:    4770        pG      BX       lr
    $d
        0x001293f8:    001294bc    ....    DCD    1217724
        0x001293fc:    001294bb    ....    DCD    1217723
        0x00129400:    001294a0    ....    DCD    1217696
        0x00129404:    0012949c    ....    DCD    1217692
        0x00129408:    00129498    ....    DCD    1217688
        0x0012940c:    00129493    ....    DCD    1217683
    $t
    i.uart_uninit
    uart_uninit
        0x00129410:    b500        ..      PUSH     {lr}
        0x00129412:    2005        .       MOVS     r0,#5
        0x00129414:    0240        @.      LSLS     r0,r0,#9
        0x00129416:    f7fefed3    ....    BL       clk_gate_per_g0 ; 0x1281c0
        0x0012941a:    200b        .       MOVS     r0,#0xb
        0x0012941c:    2101        .!      MOVS     r1,#1
        0x0012941e:    4081        .@      LSLS     r1,r1,r0
        0x00129420:    4a01        .J      LDR      r2,[pc,#4] ; [0x129428] = 0xe000e180
        0x00129422:    6011        .`      STR      r1,[r2,#0]
        0x00129424:    bf00        ..      NOP      
        0x00129426:    bd00        ..      POP      {pc}
    $d
        0x00129428:    e000e180    ....    DCD    3758154112
    $t
    i.uart_write
    uart_write
        0x0012942c:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012942e:    4604        .F      MOV      r4,r0
        0x00129430:    460d        .F      MOV      r5,r1
        0x00129432:    4616        .F      MOV      r6,r2
        0x00129434:    461f        .F      MOV      r7,r3
        0x00129436:    4804        .H      LDR      r0,[pc,#16] ; [0x129448] = 0x12c0d0
        0x00129438:    6005        .`      STR      r5,[r0,#0]
        0x0012943a:    6044        D`      STR      r4,[r0,#4]
        0x0012943c:    6086        .`      STR      r6,[r0,#8]
        0x0012943e:    60c7        .`      STR      r7,[r0,#0xc]
        0x00129440:    2001        .       MOVS     r0,#1
        0x00129442:    f7fffbe5    ....    BL       uart_etbei_setf ; 0x128c10
        0x00129446:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x00129448:    0012c0d0    ....    DCD    1229008
    $t
    i.uart_write_register
    uart_write_register
        0x0012944c:    6001        .`      STR      r1,[r0,#0]
        0x0012944e:    4770        pG      BX       lr
    $d.realdata
    .constdata
        0x00129450:    00000000    ....    DCD    0
        0x00129454:    00000000    ....    DCD    0
        0x00129458:    00000000    ....    DCD    0
        0x0012945c:    00009240    @...    DCD    37440
        0x00129460:    20300060    `.0     DCD    540016736
        0x00129464:    00000000    ....    DCD    0
        0x00129468:    00000000    ....    DCD    0
        0x0012946c:    0000a480    ....    DCD    42112
        0x00129470:    203000ec    ..0     DCD    540016876
        0x00129474:    00000000    ....    DCD    0
        0x00129478:    00000000    ....    DCD    0
        0x0012947c:    0000a482    ....    DCD    42114
    Region$$Table$$Base
        0x00129480:    001294c8    ....    DCD    1217736
        0x00129484:    001294c8    ....    DCD    1217736
        0x00129488:    00005030    0P..    DCD    20528
        0x0012948c:    0012818c    ....    DCD    1212812
    Region$$Table$$Limit

====================================

** Section #3

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x00129490
    File Offset : 5332 (0x14d4)
    Size        : 56 bytes (0x38)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    0x129490:   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
    0x1294a0:   00 00 00 00 00 00 00 00 00 00 00 00 42 58 53 0d    ............BXS.
    0x1294b0:   0a 42 58 41 0d 0a 42 58 4e 0d 0a 00 00 00 00 00    .BXA..BXN.......
    0x1294c0:   00 00 10 00 00 00 00 00                            ........


====================================

** Section #4

    Name        : EXEC_SECTION
    Type        : SHT_NOBITS (0x00000008)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x001294c8
    File Offset : 5388 (0x150c)
    Size        : 20528 bytes (0x5030)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 8
    Entry Size  : 0


====================================

** Section #5

    Name        : .debug_abbrev
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 5388 (0x150c)
    Size        : 1476 bytes (0x5c4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #6

    Name        : .debug_frame
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 6864 (0x1ad0)
    Size        : 2216 bytes (0x8a8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #7

    Name        : .debug_info
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 9080 (0x2378)
    Size        : 100448 bytes (0x18860)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #8

    Name        : .debug_line
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 109528 (0x1abd8)
    Size        : 11516 bytes (0x2cfc)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #9

    Name        : .debug_loc
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 121044 (0x1d8d4)
    Size        : 5584 bytes (0x15d0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #10

    Name        : .debug_macinfo
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 126628 (0x1eea4)
    Size        : 14572 bytes (0x38ec)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #11

    Name        : .debug_pubnames
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 141200 (0x22790)
    Size        : 1516 bytes (0x5ec)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #12

    Name        : .symtab
    Type        : SHT_SYMTAB (0x00000002)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 142716 (0x22d7c)
    Size        : 5728 bytes (0x1660)
    Link        : Section 13 (.strtab)
    Info        : Last local symbol no = 262
    Alignment   : 4
    Entry Size  : 16


====================================

** Section #13

    Name        : .strtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 148444 (0x243dc)
    Size        : 4268 bytes (0x10ac)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #14

    Name        : .note
    Type        : SHT_NOTE (0x00000007)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 152712 (0x25488)
    Size        : 32 bytes (0x20)
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

====================================

** Section #15

    Name        : .comment
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 152744 (0x254a8)
    Size        : 15524 bytes (0x3ca4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0

    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    ArmLink --strict --library_type=microlib --callgraph --load_addr_map_info --map --symbols --xref --diag_suppress=9931 --cpu=Cortex-M0+ --list=.\Listings\boot_ram_download.map --output=.\Objects\boot_ram_download.axf --scatter=.\boot_ram_scatter.txt --info=summarysizes,sizes,totals,unused,veneers

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
    
    p74c-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\boot_ram.o --depend=.\objects\boot_ram.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I.\uart -I.\qspi -I.\flash -I.\dmac -I.\uart_awo -I.\clkg -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\boot_ram.crf boot_ram.c
    
    
    
    
    
    uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\uart_download.o --vfemode=force

    Input Comments:
    
    p5a6c-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\uart_download.o --depend=.\objects\uart_download.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\uart_download.crf uart_download.c
    
    
    
    
    
    startup_uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    ArmAsm --debug --xref --diag_suppress=9931 --cpu=Cortex-M0+ --apcs=interwork --depend=.\objects\startup_uart_download.d  -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:
    
    
    uart.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\uart.o --vfemode=force

    Input Comments:
    
    p6a18-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    uart.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\uart.o --depend=.\objects\uart.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\uart.crf uart\uart.c
    
    
    pshare_rom.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\pshare_rom.o --vfemode=force

    Input Comments:
    
    p2b38-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    pshare_rom.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\pshare_rom.o --depend=.\objects\pshare_rom.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\pshare_rom.crf pshare_rom.c
    
    
    
    segger_rtt.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\segger_rtt.o --depend=.\objects\segger_rtt.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\segger_rtt.crf log\RTT\SEGGER_RTT.c
    
    segger_rtt_printf.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\segger_rtt_printf.o --depend=.\objects\segger_rtt_printf.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\segger_rtt_printf.crf log\RTT\SEGGER_RTT_printf.c
    
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\dma_for_qspi.o --vfemode=force

    Input Comments:
    
    p6764-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\dma_for_qspi.o --depend=.\objects\dma_for_qspi.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\dma_for_qspi.crf ..\boot_ram\dma_for_qspi.c
    
    
    
    flash.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\flash.o --depend=.\objects\flash.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\flash.crf ..\boot_ram\flash.c
    
    qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\qspi.o --vfemode=force

    Input Comments:
    
    p6438-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\qspi.o --depend=.\objects\qspi.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\qspi.crf ..\boot_ram\qspi.c
    
    
    
    bx_crc32.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\bx_crc32.o --depend=.\objects\bx_crc32.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=00 --omf_browse=.\objects\bx_crc32.crf bx_crc32.c
    
    

====================================

** Section #16

    Name        : .shstrtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 168268 (0x2914c)
    Size        : 168 bytes (0xa8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

address     size       variable name                            type
0x0012c0f4  0x400      l_crc32_table                            array[256] of unsigned long

address     size       variable name                            type
0x0012c0d0  0x24       uart_env                                 uart_env_tag
0x0012c0d0  0x10       uart_env.tx                              uart_txrxchannel
0x0012c0d0  0x4        uart_env.tx.size                         uint32_t
0x0012c0d4  0x4        uart_env.tx.bufptr                       pointer to uint8_t
0x0012c0d8  0x4        uart_env.tx.callback                     pointer to function 
0x0012c0dc  0x4        uart_env.tx.dummy                        pointer to unknown Type 
0x0012c0e0  0x10       uart_env.rx                              uart_txrxchannel
0x0012c0e0  0x4        uart_env.rx.size                         uint32_t
0x0012c0e4  0x4        uart_env.rx.bufptr                       pointer to uint8_t
0x0012c0e8  0x4        uart_env.rx.callback                     pointer to function 
0x0012c0ec  0x4        uart_env.rx.dummy                        pointer to unknown Type 
0x0012c0f0  0x1        uart_env.errordetect                     _Bool

address     size       variable name                            type
0x001294c8  0x400      apollo_00_reg_val                        array[256] of uint32_t

address     size       variable name                            type
0x001294a0  0x1        crc32_flag                               volatile _Bool

address     size       variable name                            type
0x0012949c  0x4        crc32_pc                                 volatile uint32_t

address     size       variable name                            type
0x00129498  0x4        crc32_result                             volatile uint32_t

address     size       variable name                            type
0x001294c0  0x4        ram_run_current_location                 uint32_t

address     size       variable name                            type
0x001294bb  0x1        ram_run_flag                             volatile _Bool

address     size       variable name                            type
0x001294ac  0xf        uart2pc                                  array[3] of array[5] of uint8_t

address     size       variable name                            type
0x001298c8  0x2808     uart_buff                                Data_From_Host_t
0x001298c8  0x8        uart_buff.header                         Uart_Ramrun_Header_t
0x001298c8  0x1        uart_buff.header.header_flag             uint8_t
0x001298c9  0x1        uart_buff.header.cmd                     anonymous
0x001298c9  0x1(6:2)   uart_buff.header.cmd.r_w                 uint8_t
0x001298c9  0x1(5:1)   uart_buff.header.cmd.section             uint8_t
0x001298c9  0x1(0:5)   uart_buff.header.cmd.op                  uint8_t
0x001298ca  0x2        uart_buff.header.length                  uint16_t
0x001298cc  0x4        uart_buff.header.addr                    uint32_t
0x001298d0  0x2800     uart_buff.data                           array[10240] of uint8_t

address     size       variable name                            type
0x001294a4  0x8        uart_itf                                 uart_itf_t
0x001294a4  0x4        uart_itf.tx_func                         write_fn_t
0x001294a8  0x4        uart_itf.rx_func                         read_fn_t

address     size       variable name                            type
0x001294c4  0x4        vec_int_base                             pointer to uint32_t

address     size       variable name                            type
0x001294bc  0x1        write_to_ram_run                         volatile _Bool

address     size       variable name                            type
0x00127ff0  0x10       boot_header                              const boot_header_t
0x00127ff0  0x4        boot_header.bx_flag                      array[4] of uint8_t
0x00127ff4  0x4        boot_header.base_addr                    pointer to uint8_t
0x00127ff8  0x4        boot_header.length                       uint32_t
0x00127ffc  0x4        boot_header.entry_point                  entry_point_t

address     size       variable name                            type
0x00129491  0x1        check_result                             uint8_t

address     size       variable name                            type
0x00129494  0x1        fix_warning                              volatile uint8_t

address     size       variable name                            type
0x00129493  0x1        start_ram_run                            volatile uint8_t

address     size       variable name                            type
0x00129492  0x1        uart_baud_changed                        uint8_t

address     size       variable name                            type
0x00129490  0x1        wait_download_finish                     uint8_t

