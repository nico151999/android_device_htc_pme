#include <stdio.h>
#include <assert.h>
#include "tfa9888.h"

/* bit fields (bits numbered from 1)
 * reg bit name
 * --- --- ----
 * 00   5  MUTE
 * 01   4? NOCLK or [ probably this one becasue it switches from 1 to 0 on init]
 * 01  11? NOCLK
 *         MANSTATE
 */

#define FULL_NAME(a) TFA98XX_BF_ ##a
#define BITFIELD(a) { FULL_NAME(a), #a }
struct {
    int bf;
    const char *name;
} bitfields[] = {
    BITFIELD(POWERDOWN),
    BITFIELD(RESET),
    BITFIELD(ENABLE_COOLFLUX),
    BITFIELD(ENABLE_AMPLIFIER),
    BITFIELD(ENABLE_BOOST),
    BITFIELD(COOLFLUX_CONFIGURED),
    BITFIELD(SET_ENABL_AMPLIFIER),
    BITFIELD(INT_PAD_IO),
    BITFIELD(FS_PULSE_SEL),
    BITFIELD(BYPASS_OCP),
    BITFIELD(TEST_OCP),
    BITFIELD(VAMP_SEL),
    BITFIELD(SRC_SET_CONFIGURED),
    BITFIELD(EXECUTE_COLD_START),
    BITFIELD(ENABLE_OSC1M_AUTO_OFF),
    BITFIELD(MAN_ENABLE_BROWN_OUT),
    BITFIELD(ENABLE_BOD),
    BITFIELD(ENABLE_BOD_HYST),
    BITFIELD(BOD_DELAY),
    BITFIELD(BOD_LVLSEL),
    BITFIELD(DISABLE_MUTE_TIMEOUT),
    BITFIELD(PWM_SEL_RECV_NS),
    BITFIELD(MAN_ENABLE_WATCHDOG),
    BITFIELD(AUDIO_FS),
    BITFIELD(INPUT_LEVEL),
    BITFIELD(CS_FRAC_DELAY),
    BITFIELD(BYPASS_HVBAT_FILTER),
    BITFIELD(CTRL_RCVLDOP_BYPASS),
    BITFIELD(PLL_CLKIN_SEL),
    BITFIELD(PLL_CLKIN_SEL_OSC),
    BITFIELD(ENABL_SPKR_SS_LEFT),
    BITFIELD(ENABL_SPKR_SS_RIGHT),
    BITFIELD(ENABL_VOLSENSE_LEFT),
    BITFIELD(ENABL_VOLSENSE_RIGHT),
    BITFIELD(ENABL_CURSENSE_LEFT),
    BITFIELD(ENABL_CURSENSE_RIGHT),
    BITFIELD(ENABL_PDM_SS),
    BITFIELD(SIDE_TONE_GAIN_SEL),
    BITFIELD(SIDE_TONE_GAIN),
    BITFIELD(MUTE_SIDE_TONE),
    BITFIELD(CTRL_DIGTOANA),
    BITFIELD(ENBL_CMFB_LEFT),
    BITFIELD(HIDDEN_CODE),
    BITFIELD(FLAG_POR),
    BITFIELD(FLAG_PLL_LOCK),
    BITFIELD(FLAG_OTPOK),
    BITFIELD(FLAG_OVPOK),
    BITFIELD(FLAG_UVPOK),
    BITFIELD(FLAG_CLOCKS_STABLE),
    BITFIELD(FLAG_MTB_BUSY),
    BITFIELD(FLAG_LOST_CLOCK),
    BITFIELD(FLAG_CF_SPEAKERERROR),
    BITFIELD(FLAG_COLD_STARTED),
    BITFIELD(FLAG_ENGAGE),
    BITFIELD(FLAG_WATCHDOG_RESET),
    BITFIELD(FLAG_ENABLE_AMP),
    BITFIELD(FLAG_ENABLE_REF),
    BITFIELD(FLAG_ADC10_READY),
    BITFIELD(FLAG_BOD_VDDD_NOK),
    BITFIELD(FLAG_BST_BSTCUR),
    BITFIELD(FLAG_BST_HIZ),
    BITFIELD(FLAG_BST_OCPOK),
    BITFIELD(FLAG_BST_PEAKCUR),
    BITFIELD(FLAG_BST_VOUTCOMP),
    BITFIELD(FLAG_BST_VOUTCOMP86),
    BITFIELD(FLAG_BST_VOUTCOMP93),
    BITFIELD(FLAG_SOFT_MUTE_BUSY),
    BITFIELD(FLAG_SOFT_MUTE_SATE),
    BITFIELD(FLAG_TDM_LUT_ERROR),
    BITFIELD(FLAG_TDM_STATUS),
    BITFIELD(FLAG_TDM_ERROR),
    BITFIELD(FLAG_HAPTIC_BUSY),
    BITFIELD(FLAG_OCPOKAP_LEFT),
    BITFIELD(FLAG_OCPOKAN_LEFT),
    BITFIELD(FLAG_OCPOKBP_LEFT),
    BITFIELD(FLAG_OCPOKBN_LEFT),
    BITFIELD(FLAG_CLIPA_HIGH_LEFT),
    BITFIELD(FLAG_CLIPA_LOW_LEFT),
    BITFIELD(FLAG_CLIPB_HIGH_LEFT),
    BITFIELD(FLAG_CLIPB_LOW_LEFT),
    BITFIELD(FLAG_OCPOKAP_RECV),
    BITFIELD(FLAG_OCPOKAN_RECV),
    BITFIELD(FLAG_OCPOKBP_RECV),
    BITFIELD(FLAG_OCPOKBN_RECV),
    BITFIELD(FLAG_RCVLDOP_READY),
    BITFIELD(FLAG_RCVLDOP_BYPASSREADY),
    BITFIELD(FLAG_OCP_ALARM_LEFT),
    BITFIELD(FLAG_CLIP_LEFT),
    BITFIELD(FLAG_OCPOKAP_RIGHT),
    BITFIELD(FLAG_OCPOKAN_RIGHT),
    BITFIELD(FLAG_OCPOKBP_RIGHT),
    BITFIELD(FLAG_OCPOKBN_RIGHT),
    BITFIELD(FLAG_CLIPA_HIGH_RIGHT),
    BITFIELD(FLAG_CLIPA_LOW_RIGHT),
    BITFIELD(FLAG_CLIPB_HIGH_RIGHT),
    BITFIELD(FLAG_CLIPB_LOW_RIGHT),
    BITFIELD(FLAG_OCP_ALARM_RIGHT),
    BITFIELD(FLAG_CLIP_RIGHT),
    BITFIELD(FLAG_MIC_OCPOK),
    BITFIELD(FLAG_MAN_ALARM_STATE),
    BITFIELD(FLAG_MAN_WAIT_SRC_SETTINGS),
    BITFIELD(FLAG_MAN_WAIT_CF_CONFIG),
    BITFIELD(FLAG_MAN_START_MUTE_AUDIO),
    BITFIELD(FLAG_MAN_OPERATING_STATE),
};

#define n_bitfields (sizeof(bitfields) / sizeof(bitfields[0]))

static int read_one_inverted_byte()
{
    int v = getc(stdin);
    if (v == EOF) return v;
    return ((v & 0xf) << 4) | (v >> 4);
}

static void dump_bitfield(int reg)
{
    int i, v, v1, v2;
    int any = 0;

    printf(" ");
    v1 = read_one_inverted_byte();
    v2 = read_one_inverted_byte();
    v = v2 | (v1 << 8);
#if 0
printf("%x %x", v1, v1a);
for (i = 15; i >= 0; i--) {
printf("%d", (v1 >> i) & 1);
if (i == 8) printf(" ");
}
#endif
    for (i = 0; i < n_bitfields; i++) {
        if (reg == bitfields[i].bf >> 8) {
            int shift = (bitfields[i].bf & 0xf0) >> 4;
            int nbits = (bitfields[i].bf & 0xf) + 1;
            int mask = (1<<nbits)-1;
            int bits = (v >> shift) & mask;
            int bits2 = (v & (mask << shift)) >> shift;

//printf("\n%s[%d,%d,%d]", bitfields[i].name, shift, nbits, mask);
assert(bits == bits2);
            if (bits) printf(" %s:%d", bitfields[i].name, bits);
            any = 1;
        }
    }
    if (! any) printf("%04x", v);
    printf("\n");
}

int main()
{
    int c, l, d, i;
    int get_reg = -1;

    while((c = getc(stdin)) != EOF && (l = getc(stdin)) != EOF) {
        if (c == 1 && l == 1) {
            if (get_reg != -1) printf("**** ERROR DECODING ****\n");
            get_reg = read_one_inverted_byte();
            continue;
        } else if (get_reg != -1 && c == 0) {
            if (get_reg == 0x30) {
                printf("GET REV");
            } else if (l == 2) {
                printf("GET %x", get_reg);
                dump_bitfield(get_reg);
                get_reg = -1;
                continue;
            }
            get_reg = -1;
        } else if (c == 1 && l == 3) {
            d = read_one_inverted_byte();
            printf("SET %x", d);
            dump_bitfield(d);
            continue;
        } else {
            printf("%s", c ? "WR " : "RD ");
        }
        for (i = 0; i < l && (d = read_one_inverted_byte()) != EOF; i++) printf(" %02x", d);
        if (c == 0 || l > 1) printf("\n");
        else printf("\t");
    }
}
