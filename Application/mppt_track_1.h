//enum { SLOW_START = 0,START = 1, CHECK = 2, FIX = 3, RETRY_PLUS = 4, RETRY_MINUS = 5}
//MPPT_Track_State = SLOW_START ;

//typedef enum   { SLOW_START = 0,START = 1, CHECK = 2, FIX = 3, RETRY_PLUS = 4, RETRY_MINUS = 5}
//MPPT_Track_State ;
typedef enum   {START = 0, CHECK = 1, FIX = 2, RETRY_PLUS = 3, RETRY_MINUS = 4}
MPPT_Track_State ;
extern MPPT_Track_State mppt_state;
void mppt_track();
void Battery_upto_95();
void init();