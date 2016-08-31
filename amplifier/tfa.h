#ifndef __TFA_H__
#define __TFA_H__

#include <tinyalsa/asoundlib.h>

typedef struct tfaS tfa_t;

struct tfa_profile {
    int something[4];
};

tfa_t *tfa_new(void);

struct pcm *tfa_mi2s_enable(tfa_t *);
int tfa_mi2s_disable(tfa_t *, struct pcm *pcm);

int tfa_set_register(tfa_t *t, unsigned reg, unsigned value);
int tfa_get_register(tfa_t *t, unsigned reg);
int tfa_set_bitfield(tfa_t *, int bf, unsigned value);
int tfa_get_bitfield(tfa_t *, int bf);

int tfa_set_keys(tfa_t *);

void tfa_destroy(tfa_t *);

#endif
