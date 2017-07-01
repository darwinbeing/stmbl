#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(pmsm_t2c);

// motor values
HAL_PIN(psi);
HAL_PIN(polecount);

HAL_PIN(ac);
HAL_PIN(pc);
HAL_PIN(nc);
HAL_PIN(ae);
HAL_PIN(pe);
HAL_PIN(ne);
HAL_PIN(pos);
HAL_PIN(t);
HAL_PIN(g);

// torque cmd in
HAL_PIN(torque);

// cur cmd out
HAL_PIN(cur);

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct sim_ctx_t * ctx = (struct sim_ctx_t *)ctx_ptr;
  struct pmsm_t2c_pin_ctx_t * pins = (struct pmsm_t2c_pin_ctx_t *)pin_ptr;
  PIN(nc) = 1.0;
  PIN(ne) = 1.0;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   // struct pmsm_t2c_ctx_t * ctx = (struct pmsm_t2c_ctx_t *)ctx_ptr;
   struct pmsm_t2c_pin_ctx_t * pins = (struct pmsm_t2c_pin_ctx_t *)pin_ptr;

   float p = MAX(PIN(polecount), 1.0);
   float psi_m = MAX(PIN(psi), 0.01);
   float torque = PIN(torque);
   
   float tc = PIN(ac) * sinf(PIN(pc) + PIN(pos) * PIN(nc) * p);
   float te = torque * PIN(ae) * sinf(PIN(pe) + PIN(pos) * PIN(ne) * p);
   PIN(t) = tc + te;
   PIN(cur) = (torque + PIN(g) * (tc + te)) / 3.0 * 2.0 / p / psi_m;
}

hal_comp_t pmsm_t2c_comp_struct = {
  .name = "pmsm_t2c",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct pmsm_t2c_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
