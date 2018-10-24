/**
 * @file   	PCF_pitch_compl_filt.h
 * @author 	Kuroha
 * @version
 * @date 	28 сентября 2018 г., 12:55
 * @brief
 */


#ifndef LIB_A_PCF_PITCH_COMP_FILT_H_
#define LIB_A_PCF_PITCH_COMP_FILT_H_


/*#### |Begin| --> Секция - "Include" ########################################*/
/*==== |Begin| --> Секция - "C libraries" ====================================*/
#include <stdlib.h>
#include <math.h>
/*==== |End  | <-- Секция - "C libraries" ====================================*/

/*==== |Begin| --> Секция - "MK peripheral libraries" ========================*/
/*==== |End  | <-- Секция - "MK peripheral libraries" ========================*/

/*==== |Begin| --> Секция - "Extern libraries" ===============================*/
#include "../Lib_A_NINTEG_numerical_integration/Lib_A_NINTEG_numerical_integration.h"
#include "../Lib_A_FILT_filters.c/Lib_A_FILT_filters.h"
/*==== |End  | <-- Секция - "Extern libraries" ===============================*/
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Определение констант" ###########################*/
/*#### |End  | <-- Секция - "Определение констант" ###########################*/


/*#### |Begin| --> Секция - "Определение типов" ##############################*/
#if !defined (__PCF_FPT__)
#error "Please, set __PCF_FPT__ = 'f' or 'd'"
#endif

/* Если __PCF_FPT__ равен float */
#if 	__PCF_FPT__ == 'f'
#undef 	__PCF_FPT__
#define __PCF_FPT__		float

/* Переопределение математических функций */
#define __PCF_atan2(x,y)	atan2f(x,y)
#define __PCF_sqrt(x)		sqrtf(x)

/* Если __PCF_FPT__ равен double */
#elif 	__PCF_FPT__ == 'd'
#undef  __PCF_FPT__
#define __PCF_FPT__		double

/* Переопределение математических функций */
#define __PCF_atan2(x,y)	atan2(x,y)
#define __PCF_sqrt(x)		sqrt(x)
#endif

typedef enum
{
	PCF_ROLL = 0,
	PCF_PITCH,
	PCF_YAW,

	/**
	 * @brief Данное определение должно быть крайним в данном перечисляемом типе.
	 *        Оно используется для определения размеров векторов показаний
	 *        инерциального датчика
	 */
	PCF_VECT_SIZE,
} pcf_coordinate_system_definitions_s;

typedef struct
{
	/* Коэффициент комплементарного фильтра */
	__PCF_FPT__ compFiltCoeff;

	/* Коэффициент интегральной коррекции ошибки */
	__PCF_FPT__ integralCoeff;

	/* Приращение */
	__PCF_FPT__ dT;

	__PCF_FPT__ accNormWindow;
    
    __PCF_FPT__ compFiltValForAcc;
    
    __PCF_FPT__ integralErrorSaturation;
} pcf_data_for_pitch_init_struct_s;

typedef struct
{
	/* Текущий угол наклона (в рад.) */
	__PCF_FPT__ angle;

	/* Коэффициент комплементарного фильтра */
	__PCF_FPT__ compFiltCoeff;

	/* Коэффициент интегральной коррекции ошибки */
	__PCF_FPT__ integralCoeff;

	/* Приращение */
	__PCF_FPT__ dT;

	/* Ошибка */
	__PCF_FPT__ err;

	/* Норма акселерометра */
	__PCF_FPT__ accNorm;

	__PCF_FPT__ accNormWindow;
    
    filt_complementary_s compFiltForAcc_a[PCF_VECT_SIZE];

	size_t initPitchEn_flag;

	ninteg_trapz_s trapzInteg_s;
    
    __PCF_FPT__ integralErrorSaturation;
} pcf_data_for_pitch_s;
/*#### |End  | <-- Секция - "Определение типов" ##############################*/


/*#### |Begin| --> Секция - "Определение глобальных переменных" ##############*/
extern void
PCF_Init_CompFilt(
	pcf_data_for_pitch_s *p_s,
	pcf_data_for_pitch_init_struct_s *pInit_s);

extern void
PCF_CompFilt_StructInit(
	pcf_data_for_pitch_init_struct_s *pInit_s);

extern __PCF_FPT__
PCF_GetPitchByCompFilt(
	pcf_data_for_pitch_s *p_s,
	__PCF_FPT__ *gyrY,
	__PCF_FPT__ accX,
	__PCF_FPT__ accY,
	__PCF_FPT__ accZ);
/*#### |End  | <-- Секция - "Определение глобальных переменных" ##############*/


/*#### |Begin| --> Секция - "Прототипы глобальных функций" ###################*/
/*#### |End  | <-- Секция - "Прототипы глобальных функций" ###################*/


/*#### |Begin| --> Секция - "Определение макросов" ###########################*/
/*#### |End  | <-- Секция - "Определение макросов" ###########################*/

#endif	/* LIB_A_PCF_PITCH_COMP_FILT_H_ */

/*############################################################################*/
/*################################ END OF FILE ###############################*/
/*############################################################################*/
