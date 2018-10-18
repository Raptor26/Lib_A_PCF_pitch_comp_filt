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
/*==== |End  | <-- Секция - "Extern libraries" ===============================*/
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Определение констант" ###########################*/
/*#### |End  | <-- Секция - "Определение констант" ###########################*/


/*#### |Begin| --> Секция - "Определение типов" ##############################*/
#if !defined (__PCF_FPT__)
#error "Please, set __REGUL_FPT__ (default value is float)"
#endif

typedef struct
{
	/* Коэффициент комплементарного фильтра */
	__PCF_FPT__ compFiltCoeff;

	/* Коэффициент интегральной коррекции ошибки */
	__PCF_FPT__ integralCoeff;

	/* Приращение */
	__PCF_FPT__ dT;
} pcf_all_dta_for_pitch_init_struct_s;

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

	size_t initPitchEn_flag;
} pcf_all_dta_for_pitch_s;
/*#### |End  | <-- Секция - "Определение типов" ##############################*/


/*#### |Begin| --> Секция - "Определение глобальных переменных" ##############*/
extern void
PCF_Init_CompFilt(
	pcf_all_dta_for_pitch_s *p_s,
	pcf_all_dta_for_pitch_init_struct_s *init_s);

extern __PCF_FPT__
PCF_GetPitchByCompFilt(
	pcf_all_dta_for_pitch_s *p_s,
	__PCF_FPT__ *gyrY,
	__PCF_FPT__ accX,
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
