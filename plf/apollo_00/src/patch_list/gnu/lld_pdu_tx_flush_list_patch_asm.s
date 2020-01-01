    .syntax    unified
    .arch    armv6-m
    .text
    .thumb
    .thumb_func
    .align    1    
    .globl LLD_PDU_TX_FLUSH_LIST_PATCH
    .type LLD_PDU_TX_FLUSH_LIST_PATCH,%function
LLD_PDU_TX_FLUSH_LIST_PATCH:
    MOV R1,R4
    BL lld_pdu_tx_flush_patch_c
    MOV R4,R0
    LDR R0,[pc,#0]
    BX R0
    .word 0x18641
    
    .end
    