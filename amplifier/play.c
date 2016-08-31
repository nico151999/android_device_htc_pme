#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "tfa.h"
#include "tfa9888.h"
#include "tfa9888-debug.h"

tfa_t *t;

void dump_reg(int reg)
{
    unsigned v = tfa_get_register(t, reg);;
//    printf("REG %x: ", reg);
//    tfa9888_print_bitfield(stdout, reg, v);
}

int main()
{
    time_t start = time(NULL);
    struct pcm *pcm;

    t = tfa_new();

    printf("revision: %x\n", tfa_get_bitfield(t, TFA98XX_BF_DEVICE_REV));

    printf("Enabling clocks\n");
    pcm = tfa_mi2s_enable(t);

    dump_reg(0x5a);
    dump_reg(0x10);

    tfa_set_register(t, 0, 0x02);               // RESET

    dump_reg(1);

    tfa_set_bitfield(t, TFA98XX_BF_CF_RST_DSP, 1);
    tfa_set_keys(t);

    tfa_set_register(t, 0x00, 0x164d);
    tfa_set_register(t, 0x01, 0x828b);
    tfa_set_register(t, 0x02, 0x1dc8);
    tfa_set_register(t, 0x0e, 0x80);
    tfa_set_register(t, 0x20, 0x89e);
    tfa_set_register(t, 0x22, 0x543c);
    tfa_set_register(t, 0x23, 0x06);
    tfa_set_register(t, 0x24, 0x14);
    tfa_set_register(t, 0x25, 0x0a);
    tfa_set_register(t, 0x26, 0x100);
    tfa_set_register(t, 0x28, 0x1000);
    tfa_set_register(t, 0x51, 0x00);
    tfa_set_register(t, 0x52, 0xfafe);
    tfa_set_register(t, 0x70, 0x3ee4);
    tfa_set_register(t, 0x71, 0x1074);
    tfa_set_register(t, 0x83, 0x14);

    dump_reg(2);
    dump_reg(0x20);

    printf("Disabling clocks\n");
    tfa_mi2s_disable(t, pcm);

    printf("nEnabling clocks\n");
    pcm = tfa_mi2s_enable(t);
    printf("\n\nWaiting for clocks\n");
    while (time(NULL) - start < 10 && tfa_get_bitfield(t, TFA98XX_BF_FLAG_LOST_CLK)) sleep(1);
    printf("Clocks enabled, powering up\n");
    tfa_set_bitfield(t, TFA98XX_BF_POWERDOWN, 0);
    dump_reg(0);
    while (time(NULL) - start < 10) sleep(1);

    printf("Disabling\n");
    tfa_mi2s_disable(t, pcm);

    return 0;
}
