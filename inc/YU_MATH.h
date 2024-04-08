//
// Created by 17273 on 2024/1/18.
//

#ifndef DEMO_YU_MATH_H
#define DEMO_YU_MATH_H

#include "cstdint"

#define YU_D_MATH_LIMIT(MAX, MIN, DATA) ((DATA > MAX) ? MAX : ((DATA < MIN) ? MIN : DATA))
#define YU_D_MATH_ABS(DATA) (DATA > 0) ? DATA : (-DATA)

#define YU_D_RELATIVE_CAL_PARAM 0.000767944870878f

#endif //DEMO_YU_MATH_H
