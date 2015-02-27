/*
 * The system timer, broadcomm manual 172, section 12.
 * Enabling a timer is easy.  E.g., to enable c1: set c1 to clo+X, set c1 = 1.
 */
typedef volatile struct arm_timer {
        struct {
                // system timer match 0.  0 = no timer match, 1 = match
                unsigned _c0:1,
                // system timer match 1.  0 = no timer match, 1 = match
                         c1:1,
                // system timer match 2.  0 = no timer match, 1 = match
                         _c2:1,
                // system timer match 3.  0 = no timer match, 1 = match
                         c3:1,
                         reserved:28;
        }  cs;
        unsigned clo;
        unsigned chi;
        unsigned c0;
        unsigned c1;
        unsigned c2;
        unsigned c3;
} arm_timer_t;
