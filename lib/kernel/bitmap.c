#include "bitmap.h"
#include "stdint.h"
#include "string.h"
#include "print.h"
#include "interrupt.h"
#include "debug.h"

/* 初始化位图btmp */
void bitmap_init(bitmap* btmp) {
    memset(btmp->bits, 0, btmp->btmp_bytes_len);
}

/* 判断bit_idx位是否为1，若为1返回true发，否则false */
bool bitmap_scan_test(bitmap* btmp, uint32_t bit_idx) {
    uint32_t byte_idx = bit_idx / 8;    //向下取整用于索引数组下标
    uint32_t bit_odd = bit_idx % 8;     //取余用于索引数组内的位
    return (btmp->bits[byte_idx] & (BITMAP_MASK << bit_odd));
}

/* 在位图中申请连续cnt个位，成功则返回起始位下标，否则-1 */
int bitmap_scan(bitmap* btmp, uint32_t cnt) {
    uint32_t idx_byte = 0;      //记录空闲位所在字节
    //逐字节比较
    while(( 0xff == btmp->bits[idx_byte]) && (idx_byte < btmp->btmp_bytes_len)) {
        idx_byte++;
    }

    ASSERT(idx_byte < btmp->btmp_bytes_len);
    if(idx_byte == btmp->btmp_bytes_len) {
        return -1;
    }

    //找到了空闲字节则在字节内逐位比对
    int idx_bit = 0;
    while((uint8_t)(BITMAP_MASK << idx_bit) & (btmp->bits[idx_byte])) {
        idx_bit++;
    }

    int bit_idx_start = idx_byte * 8 + idx_bit;
    if(cnt == 1) {
        return bit_idx_start;
    }

    uint32_t bit_left = (btmp->btmp_bytes_len * 8 - bit_idx_start);
    //记录还有多少位可以判断
    uint32_t next_bit = bit_idx_start + 1;
    uint32_t count = 1;     //记录找到的空闲位个数

    bit_idx_start = -1;
    while(bit_left-- > 0) {
        if(!(bitmap_scan_test(btmp,next_bit))) {
            count++;
        } else {
            count = 0;
        }
        if(count == cnt) {
            bit_idx_start = next_bit - cnt + 1;
            break;
        }
        next_bit++;
    }
    return bit_idx_start;
}

/* 将位图btmp的bit_idx位设置为value */
void bitmap_set(bitmap* btmp, uint32_t bit_idx, int8_t value) {
    ASSERT((value == 0) || (value == 1));
    uint32_t byte_idx = bit_idx / 8;    //向下取整用于索引数组下标
    uint32_t bit_odd = bit_idx % 8;     //取余用于索引数组内的位

    if(value) {
        btmp->bits[byte_idx] |= (BITMAP_MASK << bit_odd);
    } else {
        btmp->bits[byte_idx] &= ~(BITMAP_MASK << bit_odd);
    }
}