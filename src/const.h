  #include <pebble.h>

#ifndef const_h
#define const_h

  const GPathInfo HOUR_DIVIDER_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-5, -80}, // 80 = radius + fudge; 8 = 80*tan(6 degrees); 6 degrees per minute;
    {5,  -80},
  }
};
  const GPathInfo HOUR_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-2, -80}, // 80 = radius + fudge; 8 = 80*tan(6 degrees); 6 degrees per minute;
    {48,  -80},
  }
};

#endif