#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    (void)challenge;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t high = (uint32_t)(challenge >> 32);
    uint32_t low  = (uint32_t)challenge;
    uint32_t mid  = (uint32_t)(challenge >> 16);
    uint32_t res1 = (5 * high + mid) ^ 0x8b44;
    uint32_t res0 = (low - high) * 5 + 0xbe1c;
    uint32_t ans = (res0 * 5) ^ (res1 + 0x3b23);
    values[0] = ans;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 6;
    (void)challenge;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = ((challenge >> 12) & 0xFF) * 3;
    uint32_t shift = 13;
    for (size_t i = 1; i < 6; ++i) {
        uint32_t part = (challenge >> shift) & 0x1F;
        values[i] = (part + i) ^ (values[i-1] * 3);
        shift += 5;
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    (void)challenge;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t arg1 = challenge & 0x7;
    uint32_t arg2 = (uint32_t)(challenge >> 32) ^ (uint32_t)(challenge >> 3);
    values[0] = arg1;
    switch (arg1) {
        case 0: values[1] = 3 * arg2 + 0xc0c7; break;
        case 1: values[1] = 5 * arg2 + 0xf940; break;
        case 2: values[1] = 5 * arg2 + 0x4ea02; break;
        case 3: values[1] = (4 * arg2) ^ 0x649c; break;
        case 4: values[1] = (arg2 ^ 0x9028) + (arg2 >> 1); break;
        case 5: values[1] = 5 * arg2 + 0x2288f; break;
        case 6: values[1] = (4 * arg2) ^ 0xc17c; break;
        case 7: values[1] = (arg2 ^ 0x5aa6) + (arg2 >> 1); break;
        default: values[1] = 0; break;
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t target = challenge & 0x7F;
    uint32_t key = (uint32_t)(challenge >> 7) ^ (uint32_t)(challenge >> 32);
    uint32_t low = 0;
    uint32_t high = 0x7F;
    uint32_t ebx_stack[10]; 
    int direction_stack[10];
    int depth = 0;
    uint32_t mid;
    while (1) {
        mid = low + ((high - low) >> 1);
        if (mid == target) {
            break;
        }
        ebx_stack[depth] = mid + key;
        if (target > mid) {
            direction_stack[depth] = 1;
            low = mid + 1;
        } else {
            direction_stack[depth] = 0;
            high = mid - 1;
        }
        depth++;
    }
    uint32_t res = (mid + 0xaf) ^ key;
    for (int i = depth - 1; i >= 0; i--) {
        res = res * 3;
        res = res ^ ebx_stack[i];
        if (direction_stack[i] == 1) {
            res = res ^ 0x8;
        } else {
            res = res ^ 0x1b;
        }
    }
    values[0] = res;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    int next_idx[12] = {9, 4, 1, 10, -1, 0, 2, 8, 11, 3, 6, 5};
    uint32_t val1[12] = {0xa84, 0x28, 0x97f, 0x252, 0x73e, 0x102, 0x41a, 0x872, 0xb28, 0x61f, 0x349, 0x51d};
    uint32_t val2[12] = {0x3943, 0x9f00, 0xbd66, 0x208a, 0x1f09, 0x23a, 0xc2e0, 0xe90d, 0x46e9, 0x620, 0xa4e, 0x5ac2};
    int heads[8] = {7, 8, 11, 5, 0, 9, 3, 10};
    uint32_t head_idx = challenge & 7;
    uint32_t loop_count = (challenge >> 3) & 3;
    uint32_t base_val = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32) ^ 0xf620;
    int cur_idx = heads[head_idx];
    uint32_t eax = 3 * base_val + val2[cur_idx];
    uint32_t second_val = val1[cur_idx];
    eax ^= second_val;
    for (uint32_t i = 1; i <= loop_count; i++) {
        cur_idx = next_idx[cur_idx];
        if (cur_idx == -1) break;
        eax = 3 * eax + val2[cur_idx];
        second_val = val1[cur_idx];
        eax ^= second_val;
    }
    values[0] = eax;
    values[1] = second_val;
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t arg1 = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);
    uint32_t idx1 = challenge & 7;
    uint32_t res1 = 0;
    switch (idx1) {
        case 0: res1 = (arg1 ^ 0x3073) + 4 * arg1; break;
        case 1: res1 = (arg1 * 8 + 0x572d) ^ (arg1 * 3); break;
        case 2: res1 = ((arg1 >> 7) | (arg1 << 25)); break;
        case 3: res1 = (arg1 * 8 + 0x572d) ^ (arg1 * 3); break;
        case 4: res1 = ((arg1 << 2) | (arg1 >> 30)) ^ 0x5142; break;
        case 5: res1 = (arg1 * 8 + 0x572d) ^ (arg1 * 3); break;
        case 6: res1 = ((arg1 >> 5) ^ arg1) + 0x7289; break;
        case 7: res1 = (arg1 + 0x8cc5) ^ (arg1 >> 7); break;
    }
    res1 ^= 0x15d;
    uint32_t arg2 = res1;
    uint32_t idx2 = (challenge >> 3) & 7;
    uint32_t res2 = 0;
    switch (idx2) {
        case 0: res2 = (arg2 ^ 0x3073) + 4 * arg2; break;
        case 1: res2 = (arg2 * 8 + 0x572d) ^ (arg2 * 3); break;
        case 2: res2 = ((arg2 >> 7) | (arg2 << 25)); break;
        case 3: res2 = (arg2 * 8 + 0x572d) ^ (arg2 * 3); break;
        case 4: res2 = ((arg2 << 2) | (arg2 >> 30)) ^ 0x5142; break;
        case 5: res2 = (arg2 * 8 + 0x572d) ^ (arg2 * 3); break;
        case 6: res2 = ((arg2 >> 5) ^ arg2) + 0x7289; break;
        case 7: res2 = (arg2 + 0x8cc5) ^ (arg2 >> 7); break;
    }
    values[0] = res2 + 0xec93;
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    uint32_t context1_low = (uint32_t)challenge;
    uint32_t context1_high = (uint32_t)(challenge >> 32);
    uint32_t context2_low = (uint32_t)context;
    uint32_t context2_high = (uint32_t)(context >> 32);
    uint32_t ecx = context2_low & 0x3;
    uint32_t num1 = ecx;
    uint32_t r9d = (ecx * 0x91e) ^ (uint32_t)(context >> 16);
    uint32_t esi = context1_low ^ context1_high;
    uint32_t r8d = context2_low ^ context2_high;
    uint32_t edi = esi ^ r8d;
    uint32_t mix_res = (edi * 7 + 0x91e) ^ 0xade1;
    uint32_t num2 = mix_res ^ r9d;
    esi >>= 5;
    uint32_t edx = r8d + ecx + 0xade1 + esi;
    uint32_t num3 = num2 ^ edx;
    snprintf(out, cap, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN %u %u %u", num1, num2, num3);
    return 0;
}
