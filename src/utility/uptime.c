/* Copyright (c) 2012, Peter A. Bigot
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the software nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <bsp430/platform.h>
#include <bsp430/utility/uptime.h>
#include <stdio.h>

#if BSP430_UPTIME - 0
/* Inhibit definition if required components were not provided. */

hBSP430halTIMER xBSP430uptimeTIMER_;
unsigned long ulBSP430uptimeConversionFrequency_Hz_ni_;

#if (BSP430_UPTIME_DELAY_CCIDX - 0)

/** Bit set when alarm has gone off */
#define DELAY_ALARM_FIRED 0x01

/** Bit set when initialization validated that the alarm could be
 * used.  If this bit is clear, the alarm functionality will never be
 * used, simply because the handle isn't valid. */
#define DELAY_ALARM_VALID 0x02

/** Bit set if delay alarm is enabled.  This does not mean it's
 * registered with the timer infrastructure, only that it should be
 * registered when the uptime timer is running. */
#define DELAY_ALARM_ENABLED 0x04

/** Bit set if the timer alarm underlying the delay alarm is enabled,
 * i.e. the uptime timer is running and delay alarm callback hooks are
 * in place. */
#define DELAY_ALARM_REGISTERED 0x08

/** Bit set if the uptime timer is active.  This is used as a filter
 * on whether to register the delay infrastructure. */
#define DELAY_ALARM_TIMER_ACTIVE 0x10

struct sDelayAlarm {
  /** The underlying alarm infrastructure.  First in the structure so
   * we can access the delay-specific metadata from the handler. */
  struct sBSP430timerAlarm alarm;

  /** Other information related to delay management */
  volatile unsigned int flags;
} delayAlarm_;
#define H_delayAlarm (&delayAlarm_.alarm)

static int
delayCallback_ (hBSP430timerAlarm alarm)
{
  struct sDelayAlarm * ap = (struct sDelayAlarm *)alarm;
  ap->flags |= DELAY_ALARM_FIRED;
  return BSP430_HAL_ISR_CALLBACK_EXIT_LPM | BSP430_HAL_ISR_CALLBACK_EXIT_CLEAR_GIE;
}

static int
delayAlarmSetRegistered_ni_ (int enablep)
{
  int rv = 0;
  if (enablep) {
    /* Enable the delay alarm if desired and not already registered. */
    if ((DELAY_ALARM_ENABLED & delayAlarm_.flags)
        && (! (DELAY_ALARM_REGISTERED & delayAlarm_.flags))) {
      rv = iBSP430timerAlarmSetEnabled_ni(H_delayAlarm, 1);
      if (0 == rv) {
        delayAlarm_.flags |= DELAY_ALARM_REGISTERED;
      }
    }
  } else {
    if (DELAY_ALARM_REGISTERED & delayAlarm_.flags) {
      (void)iBSP430timerAlarmCancel_ni(H_delayAlarm);
      (void)iBSP430timerAlarmSetEnabled_ni(H_delayAlarm, 0);
      delayAlarm_.flags &= ~DELAY_ALARM_REGISTERED;
    }
  }
  return rv;
}

#endif /* BSP430_UPTIME_DELAY_CCIDX */

void
vBSP430uptimeStart_ni (void)
{
  xBSP430uptimeTIMER_ = hBSP430timerLookup(BSP430_UPTIME_TIMER_PERIPH_HANDLE);
  xBSP430uptimeTIMER_->hpl->ctl = 0;
  vBSP430timerResetCounter_ni(xBSP430uptimeTIMER_);
  xBSP430uptimeTIMER_->hpl->ctl =
    ((TASSEL0 | TASSEL1) & (BSP430_UPTIME_TASSEL))
    | ((ID0 | ID1) & (BSP430_UPTIME_DIVIDING_SHIFT))
    | TACLR | TAIE;
#if (BSP430_UPTIME_DELAY_CCIDX - 0)
  {
    hBSP430timerAlarm delay_alarm;

    delayAlarm_.flags = 0;
    delay_alarm = hBSP430timerAlarmInitialize(&delayAlarm_.alarm, BSP430_UPTIME_TIMER_PERIPH_HANDLE, BSP430_UPTIME_DELAY_CCIDX, delayCallback_);
    if (H_delayAlarm == delay_alarm) {
      delayAlarm_.flags = DELAY_ALARM_VALID | DELAY_ALARM_ENABLED | DELAY_ALARM_TIMER_ACTIVE;
    }
  }
#endif /* BSP430_UPTIME_DELAY_CCIDX */
  vBSP430uptimeResume_ni();
}

void
vBSP430uptimeSuspend_ni (void)
{
  xBSP430uptimeTIMER_->hpl->ctl &= ~(MC0 | MC1);
#if (BSP430_UPTIME_DELAY_CCIDX - 0)
  (void)delayAlarmSetRegistered_ni_(0);
  delayAlarm_.flags &= ~DELAY_ALARM_TIMER_ACTIVE;
#endif /* BSP430_UPTIME_DELAY_CCIDX */
}

void
vBSP430uptimeResume_ni (void)
{
  ulBSP430uptimeConversionFrequency_Hz_ni_ = ulBSP430timerFrequency_Hz_ni(BSP430_UPTIME_TIMER_PERIPH_HANDLE);
#if (BSP430_UPTIME_DELAY_CCIDX - 0)
  delayAlarm_.flags |= DELAY_ALARM_TIMER_ACTIVE;
  (void)delayAlarmSetRegistered_ni_(1);
#endif /* BSP430_UPTIME_DELAY_CCIDX */
  xBSP430uptimeTIMER_->hpl->ctl |= MC_2;
}

unsigned long
ulBSP430uptimeConversionFrequency_Hz_ni (void)
{
  return ulBSP430uptimeConversionFrequency_Hz_ni_;
}

unsigned long
ulBSP430uptimeSetConversionFrequency_ni (unsigned long frequency_Hz)
{
  unsigned long rv = ulBSP430uptimeConversionFrequency_Hz_ni_;
  ulBSP430uptimeConversionFrequency_Hz_ni_ = frequency_Hz;
  return rv;
}

const char *
xBSP430uptimeAsText_ni (unsigned long duration_utt)
{
  static char buf[sizeof("HHH:MM:SS.mmm")];
  unsigned long conversionFrequency_Hz;
  unsigned int msec;
  unsigned int sec;
  unsigned int min;
  unsigned long q_sec;
  unsigned long r_utt;
  unsigned long q_min;
  unsigned long q_hr;

  conversionFrequency_Hz = ulBSP430uptimeConversionFrequency_Hz_ni_;
  q_sec = duration_utt / conversionFrequency_Hz;
  r_utt = duration_utt - (q_sec * conversionFrequency_Hz);
  msec = (1000L * r_utt) / conversionFrequency_Hz;
  q_min = q_sec / 60;
  sec = q_sec - (q_min * 60);
  q_hr = q_min / 60;
  min = (q_min - (q_hr * 60));
  if (0 < q_hr) {
    snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u", (unsigned int)q_hr, min, sec, msec);
  } else {
    snprintf(buf, sizeof(buf), "%2u:%02u.%03u", min, sec, msec);
  }
  return buf;
}

#if (BSP430_UPTIME_DELAY_CCIDX - 0)

int
iBSP430uptimeDelaySetEnabled_ni (int enablep)
{
  int rv = 0;

  /* If the alarm isn't valid, error. */
  if (! (DELAY_ALARM_VALID & delayAlarm_.flags)) {
    return -1;
  }
  if (enablep) {
    delayAlarm_.flags |= DELAY_ALARM_ENABLED;
    if (delayAlarm_.flags & DELAY_ALARM_TIMER_ACTIVE) {
      rv = delayAlarmSetRegistered_ni_(1);
    }
  } else {
    if (delayAlarm_.flags & DELAY_ALARM_TIMER_ACTIVE) {
      rv = delayAlarmSetRegistered_ni_(0);
    }
    delayAlarm_.flags &= ~DELAY_ALARM_ENABLED;
  }
  return rv;
}

long
lBSP430uptimeSleepUntil_ni (unsigned long setting_utt,
                            unsigned int lpm_bits)
{
  int rc;

  /* Exit immediately unless the alarm callback is registered. */
  if (! (DELAY_ALARM_REGISTERED & delayAlarm_.flags)) {
    return 0;
  }
  delayAlarm_.flags &= ~DELAY_ALARM_FIRED;
  rc = iBSP430timerAlarmSet_ni(H_delayAlarm, setting_utt);
  if (0 != rc) {
    return 0;
  }
  /* Sleep until the alarm goes off, or something else wakes us up. */
  BSP430_CORE_LPM_ENTER_NI(lpm_bits | GIE);

  /* NOTE: GIE may be set here if whatever woke the MCU did not clear
   * it.  That is probably an application error but is not something
   * this routine should attempt to fix. */

  /* Cancel the alarm if it hasn't fired yet. */
  if (! (delayAlarm_.flags & DELAY_ALARM_FIRED)) {
    (void)iBSP430timerAlarmCancel_ni(H_delayAlarm);
  }
  return setting_utt - ulBSP430uptime_ni();
}

#endif /* BSP430_UPTIME_DELAY_CCIDX */

#endif /* BSP430_UPTIME */
