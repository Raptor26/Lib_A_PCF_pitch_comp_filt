/**
 * @file   	PCF_pitch_compl_filt.h
 * @author 	Kuroha
 * @version
 * @date 	28 сентября 2018 г., 12:55
 * @brief
 */


#ifndef PCF_PITCH_COMP_FILT_H_
#define PCF_PITCH_COMP_FILT_H_


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
typedef struct
{
	/* Коэффициент комплементарного фильтра */
	float compFiltCoeff;

	/* Коэффициент интегральной коррекции ошибки */
	float integralCoeff;

	/* Приращение */
	float dT;
} pcf_all_dta_for_pitch_init_struct_s;

typedef struct
{
	/* Текущий угол наклона (в рад.) */
	float angle;

	/* Коэффициент комплементарного фильтра */
	float compFiltCoeff;

	/* Коэффициент интегральной коррекции ошибки */
	float integralCoeff;

	/* Приращение */
	float dT;

	/* Ошибка */
	float err;
} pcf_all_dta_for_pitch_s;
/*#### |End  | <-- Секция - "Определение типов" ##############################*/


/*#### |Begin| --> Секция - "Определение глобальных переменных" ##############*/
extern void
PCF_Init_CompFilt(
	pcf_all_dta_for_pitch_s *p_s,
	pcf_all_dta_for_pitch_init_struct_s *init_s);

extern float
PCF_GetPitchByCompFilt(
	pcf_all_dta_for_pitch_s *p_s,
	float gyrY,
	float accX,
	float accZ);
/*#### |End  | <-- Секция - "Определение глобальных переменных" ##############*/


/*#### |Begin| --> Секция - "Прототипы глобальных функций" ###################*/
/*#### |End  | <-- Секция - "Прототипы глобальных функций" ###################*/


/*#### |Begin| --> Секция - "Определение макросов" ###########################*/
/*#### |End  | <-- Секция - "Определение макросов" ###########################*/

#endif	/* PCF_PITCH_COMP_FILT_H_ */

/*############################################################################*/
/*################################ END OF FILE ###############################*/
/*############################################################################*/
